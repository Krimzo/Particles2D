#include "simulator.h"


Simulator::Simulator()
{
    std::string error;
    m_fill_air_shader = gpu.create_compute_shader( kl::read_file_string( "shaders/fill_air.hlsl" ), &error );
    if ( !error.empty() )
        kl::print( "Fill Air Shader Error: ", error );
    m_copy_frame_shader = gpu.create_compute_shader( kl::read_file_string( "shaders/copy_frame.hlsl" ), &error );
    if ( !error.empty() )
        kl::print( "Copy Frame Shader Error: ", error );

    window.on_resize.emplace_back( [this]( kl::Int2 size )
        {
            gpu.resize_internal( size );
            gpu.set_viewport_size( size );
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
    gpu.resize_internal( size );
    gpu.set_viewport_size( size );
    kl::dx::TextureDescriptor tex_des{};
    tex_des.Width = size.x;
    tex_des.Height = size.x;
    tex_des.MipLevels = 1;
    tex_des.ArraySize = 1;
    tex_des.Format = DXGI_FORMAT_R8G8B8A8_UINT;
    tex_des.SampleDesc.Count = 1;
    tex_des.Usage = D3D11_USAGE_DEFAULT;
    tex_des.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
    m_particle_texture.texture = gpu.create_texture( &tex_des, nullptr );
    m_particle_texture.create_access_view();
    tex_des.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    m_copy_frame_texture.texture = gpu.create_texture( &tex_des, nullptr );
    m_copy_frame_texture.create_access_view();
}

void Simulator::copy_reformat_frame()
{
    const kl::Int2 frame_res = m_particle_texture.resolution();
    gpu.bind_access_view_for_compute_shader( m_particle_texture.access_view, 0 );
    gpu.bind_access_view_for_compute_shader( m_copy_frame_texture.access_view, 1 );
    gpu.execute_compute_shader( m_copy_frame_shader.shader, kl::ceildiv<32>( frame_res.x ), kl::ceildiv<32>( frame_res.y ) );
    gpu.unbind_access_view_for_compute_shader( 1 );
    gpu.unbind_access_view_for_compute_shader( 0 );
}

void Simulator::fill_air()
{
    const kl::Int2 frame_res = m_particle_texture.resolution();
    gpu.bind_access_view_for_compute_shader( m_particle_texture.access_view, 0 );
    gpu.execute_compute_shader( m_fill_air_shader.shader, kl::ceildiv<32>( frame_res.x ), kl::ceildiv<32>( frame_res.y ) );
    gpu.unbind_access_view_for_compute_shader( 0 );
}
