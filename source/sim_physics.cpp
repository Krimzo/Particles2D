#include "simulator.h"


void update_at( kl::Image& frame, kl::Int2 pos );
void update_sand_at( kl::Image& frame, kl::Int2 pos );
void update_water_at( kl::Image& frame, kl::Int2 pos );

void update_at( kl::Image& frame, kl::Int2 pos )
{
    switch ( material_integer( frame[pos] ) )
    {
    case iMATERIAL_SAND: update_sand_at( frame, pos ); break;
    case iMATERIAL_WATER: update_water_at( frame, pos ); break;
    }
}

void update_sand_at( kl::Image& frame, kl::Int2 pos )
{
    static constexpr auto swappable_at = []( kl::Image const& frame, kl::Int2 pos ) -> bool
        {
            if ( !frame.in_bounds( pos ) )
                return false;
            switch ( material_integer( frame[pos] ) )
            {
            case iMATERIAL_AIR:
            case iMATERIAL_WATER:
                return true;

            default:
                return false;
            }
        };

    const int rand_dir = kl::random::gen_bool() ? 1 : -1;

    for ( kl::Int2 swap_pos : { pos + kl::Int2{ 0, 1 },
        pos + kl::Int2{ rand_dir, 1 },
        pos + kl::Int2{ -rand_dir, 1 } } )
    {
        if ( swappable_at( frame, swap_pos ) )
        {
            std::swap( frame[pos], frame[swap_pos] );
            break;
        }
    }
}

void update_water_at( kl::Image& frame, kl::Int2 pos )
{
    static constexpr auto swappable_at = []( kl::Image const& frame, kl::Int2 pos ) -> bool
        {
            if ( !frame.in_bounds( pos ) )
                return false;
            return frame[pos] == MATERIAL_AIR;
        };

    const int rand_dir = kl::random::gen_bool() ? -1 : 1;

    for ( kl::Int2 swap_pos : { pos + kl::Int2{ 0, 1 },
        pos + kl::Int2{ rand_dir, 1 },
        pos + kl::Int2{ -rand_dir, 1 },
        pos + kl::Int2{ -rand_dir, 0 },
        pos + kl::Int2{ rand_dir, 0 } } )
    {
        if ( swappable_at( frame, swap_pos ) )
        {
            std::swap( frame[pos], frame[swap_pos] );
            break;
        }
    }
}
