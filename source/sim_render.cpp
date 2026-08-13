#include "simulator.h"


void Simulator::handle_render()
{
    m_render_frame = frame;
    m_render_frame.draw_circle( window.mouse.position(), brush_radius, selected_material );
    window.draw_image( m_render_frame );
    window.set_title( kl::format( "FPS: ", int( 1.0f / timer.delta() ), " Brush Radius: ", brush_radius ) );
}
