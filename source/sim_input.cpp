#include "simulator.h"


void Simulator::handle_input()
{
    brush_radius += (float) window.mouse.scroll();

    if ( window.keyboard.one.pressed() )
        selected_material = MATERIAL_ROCK;
    if ( window.keyboard.two.pressed() )
        selected_material = MATERIAL_SAND;
    if ( window.keyboard.three.pressed() )
        selected_material = MATERIAL_WATER;

    const kl::Int2 mouse_pos = window.mouse.position();
    if ( frame.in_bounds( mouse_pos ) )
    {
        if ( window.mouse.left )
            frame.draw_circle( mouse_pos, brush_radius, selected_material, true );
        if ( window.mouse.right )
            frame.draw_circle( mouse_pos, brush_radius, MATERIAL_AIR, true );
    }

    if ( window.keyboard.r )
        frame.fill( MATERIAL_AIR );
}
