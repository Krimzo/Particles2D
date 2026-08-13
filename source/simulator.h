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
    kl::GPU gpu{ window.ptr() };
    kl::Timer timer{};

    Material selected_material = MATERIAL_SAND;
    float brush_radius = 25.0f;

    Simulator();

    bool update();

private:
    kl::ComputeShader m_fill_air_shader{};
    kl::ComputeShader m_copy_frame_shader{};
    kl::ComputeShader m_add_material_shader{};
    kl::ComputeShader m_physics_shader{};

    kl::Texture m_particle_texture{ gpu };
    kl::Texture m_copy_frame_texture{ gpu };

    void resize_buffers( kl::Int2 size );
    void fill_air();
    void copy_reformat_frame();
    void add_material( kl::Int2 pos, Material material );

    void handle_input();
    void handle_physics();
    void handle_render();
};
