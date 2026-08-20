#include "simulator.h"


Simulator::Simulator()
{
    std::string error;
    m_fill_air_shader = gpu.create_compute_shader( kl::read_file_string( "shaders/fill_air.hlsl" ), &error );
    if ( !error.empty() )
        kl::print( "Fill Air Shader Error: ", error );
    m_add_material_shader = gpu.create_compute_shader( kl::read_file_string( "shaders/add_material.hlsl" ), &error );
    if ( !error.empty() )
        kl::print( "Add Material Shader Error: ", error );
    m_physics_shader = gpu.create_compute_shader( kl::read_file_string( "shaders/physics.hlsl" ), &error );
    if ( !error.empty() )
        kl::print( "Physics Shader Error: ", error );
    m_copy_frame_shader = gpu.create_compute_shader( kl::read_file_string( "shaders/copy_frame.hlsl" ), &error );
    if ( !error.empty() )
        kl::print( "Copy Frame Shader Error: ", error );
    m_ui_shader = gpu.create_compute_shader( kl::read_file_string( "shaders/ui.hlsl" ), &error );
    if ( !error.empty() )
        kl::print( "UI Shader Error: ", error );

    window.on_resize.emplace_back( [this]( kl::Int2 size )
        {
            resize_buffers( size );
            fill_air();
        } );
    window.resize( { 800, 800 } );
    window.set_icon( "resource/sand.ico" );
}

bool Simulator::update()
{
    timer.update();
    handle_input();
    handle_physics();
    handle_render();
    return window.process();
}

void Simulator::resize_buffers( kl::Int2 size )
{
    m_back_buffer_av = {};
    gpu.resize_internal( size );
    gpu.set_viewport_size( size );
    m_back_buffer_av = gpu.create_access_view( gpu.back_target_texture(), nullptr );
    kl::dx::TextureDescriptor tex_des{};
    tex_des.Width = size.x;
    tex_des.Height = size.y;
    tex_des.MipLevels = 1;
    tex_des.ArraySize = 1;
    tex_des.Format = DXGI_FORMAT_R32_UINT;
    tex_des.SampleDesc.Count = 1;
    tex_des.Usage = D3D11_USAGE_DEFAULT;
    tex_des.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
    m_particle_texture.texture = gpu.create_texture( &tex_des, nullptr );
    m_particle_texture.create_access_view();
    tex_des.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    m_particle_texture_copy.texture = gpu.create_texture( &tex_des, nullptr );
    m_particle_texture_copy.create_shader_view();
}

void Simulator::fill_air()
{
    const kl::Int2 frame_res = m_particle_texture.resolution();
    gpu.bind_access_view_for_compute_shader( m_particle_texture.access_view, 0 );
    gpu.execute_compute_shader( m_fill_air_shader.shader, kl::ceildiv<32>( frame_res.x ), kl::ceildiv<32>( frame_res.y ) );
    gpu.unbind_access_view_for_compute_shader( 0 );
}

void Simulator::copy_reformat_frame()
{
    const kl::Int2 frame_res = m_particle_texture.resolution();
    gpu.bind_access_view_for_compute_shader( m_particle_texture.access_view, 0 );
    gpu.bind_access_view_for_compute_shader( m_back_buffer_av, 1 );
    gpu.execute_compute_shader( m_copy_frame_shader.shader, kl::ceildiv<32>( frame_res.x ), kl::ceildiv<32>( frame_res.y ) );
    gpu.unbind_access_view_for_compute_shader( 1 );
    gpu.unbind_access_view_for_compute_shader( 0 );
}

void Simulator::add_material( kl::Int2 pos, Material material )
{
    struct alignas( 16 ) CB
    {
        kl::Int2 BRUSH_POSITION;
        float BRUSH_RADIUS;
        int BRUSH_MATERIAL;
    } cb = {};

    cb.BRUSH_POSITION = pos;
    cb.BRUSH_RADIUS = brush_radius;
    cb.BRUSH_MATERIAL = int( material );
    m_add_material_shader.upload( cb );

    const kl::Int2 frame_res = m_particle_texture.resolution();
    gpu.bind_access_view_for_compute_shader( m_particle_texture.access_view, 0 );
    gpu.execute_compute_shader( m_add_material_shader.shader, kl::ceildiv<32>( frame_res.x ), kl::ceildiv<32>( frame_res.y ) );
    gpu.unbind_access_view_for_compute_shader( 0 );
}

void Simulator::draw_ui()
{
    struct alignas( 16 ) CB
    {
        kl::Float4 BRUSH_MATERIAL;
        kl::Int2 BRUSH_POSITION;
        float BRUSH_RADIUS;
    } cb = {};

    cb.BRUSH_MATERIAL = material_color( selected_material );
    cb.BRUSH_POSITION = window.mouse.position();
    cb.BRUSH_RADIUS = brush_radius;
    m_ui_shader.upload( cb );

    const kl::Int2 frame_res = m_particle_texture.resolution();
    gpu.bind_access_view_for_compute_shader( m_back_buffer_av, 0 );
    gpu.execute_compute_shader( m_ui_shader.shader, kl::ceildiv<32>( frame_res.x ), kl::ceildiv<32>( frame_res.y ) );
    gpu.unbind_access_view_for_compute_shader( 0 );
}

void Simulator::handle_input()
{
    brush_radius -= (float) window.mouse.scroll();

    if ( window.keyboard.one.pressed() )
        selected_material = Material::ROCK;
    if ( window.keyboard.two.pressed() )
        selected_material = Material::SAND;
    if ( window.keyboard.three.pressed() )
        selected_material = Material::WATER;

    const kl::Int2 frame_res = m_particle_texture.resolution();
    const kl::Int2 mouse_pos = window.mouse.position();

    if ( mouse_pos.in_bounds( frame_res ) )
    {
        if ( window.mouse.left )
            add_material( mouse_pos, selected_material );
        if ( window.mouse.right )
            add_material( mouse_pos, Material::AIR );
    }

    if ( window.keyboard.r )
    {
        static constexpr UINT AIR[4] = { UINT( Material::AIR ), 0, 0, 0 };
        gpu.context()->ClearUnorderedAccessViewUint( m_particle_texture.access_view.get(), AIR );
    }
}

void Simulator::handle_physics()
{
    gpu.copy_resource( m_particle_texture_copy.texture, m_particle_texture.texture );
    const kl::Int2 frame_res = m_particle_texture.resolution();
    gpu.bind_shader_view_for_compute_shader( m_particle_texture_copy.shader_view, 0 );
    gpu.bind_access_view_for_compute_shader( m_particle_texture.access_view, 0 );
    gpu.execute_compute_shader( m_physics_shader.shader, kl::ceildiv<32>( frame_res.x ), kl::ceildiv<32>( frame_res.y ) );
    gpu.unbind_access_view_for_compute_shader( 0 );
    gpu.unbind_shader_view_for_compute_shader( 0 );
}

void Simulator::handle_render()
{
    gpu.clear_internal( kl::colors::RED );
    copy_reformat_frame();
    draw_ui();
    gpu.swap_buffers( false );
    window.set_title( kl::format( "FPS: ", int( 1.0f / timer.delta() ), " Brush Radius: ", brush_radius ) );
}
