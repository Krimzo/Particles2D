#include "simulator.h"


Simulator::Simulator()
{
    window.on_resize.emplace_back( [this]( kl::Int2 size )
        {
            frame.resize( size );
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

void Simulator::fill_air()
{
    for ( int i = 0; i < frame.pixel_count(); i++ )
    {
        switch ( material_integer( frame[i] ) )
        {
        case iMATERIAL_AIR:
        case iMATERIAL_ROCK:
        case iMATERIAL_SAND:
        case iMATERIAL_WATER:
            break;

        default:
            frame[i] = MATERIAL_AIR;
            break;
        }
    }
}
