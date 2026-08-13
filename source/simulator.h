#pragma once

#include "klibrary.h"


using Material = kl::RGB;

constexpr uint32_t material_integer( Material material )
{
    return ( uint32_t( material.b ) << 24 ) |
        ( uint32_t( material.g ) << 16 ) |
        ( uint32_t( material.r ) << 8 ) |
        uint32_t( material.a );
}

inline constexpr Material MATERIAL_AIR = { 135, 206, 250 };
inline constexpr Material MATERIAL_ROCK = { 108, 113, 119 };
inline constexpr Material MATERIAL_SAND = { 226, 202, 118 };
inline constexpr Material MATERIAL_WATER = { 3, 71, 112 };

inline constexpr uint32_t iMATERIAL_AIR = material_integer( MATERIAL_AIR );
inline constexpr uint32_t iMATERIAL_ROCK = material_integer( MATERIAL_ROCK );
inline constexpr uint32_t iMATERIAL_SAND = material_integer( MATERIAL_SAND );
inline constexpr uint32_t iMATERIAL_WATER = material_integer( MATERIAL_WATER );

struct Simulator
{
    kl::Window window{ "Particle Simulator" };
    kl::Timer timer{};
    kl::Image frame{};

    Material selected_material = MATERIAL_SAND;
    float brush_radius = 25.0f;

    Simulator();

    bool update();

private:
    kl::Image m_render_frame;

    void fill_air();

    void handle_input();
    void handle_physics();
    void handle_render();
};
