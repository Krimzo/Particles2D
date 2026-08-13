#include "simulator.h"


void Simulator::handle_render()
{
    gpu.clear_internal( kl::colors::RED );
    copy_reformat_frame();
    gpu.copy_resource( gpu.back_target_texture(), m_copy_frame_texture.texture );
    //m_render_frame.draw_circle( window.mouse.position(), brush_radius, selected_material );
    gpu.swap_buffers( true );
    window.set_title( kl::format( "FPS: ", int( 1.0f / timer.delta() ), " Brush Radius: ", brush_radius ) );
}
