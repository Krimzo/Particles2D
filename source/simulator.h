#pragma once

#include "klibrary.h"


enum struct Material : byte
{
    AIR = 0,
    ROCK,
    SAND,
    WATER,
};

constexpr kl::Float4 material_color( Material material )
{
    switch ( material )
    {
    default: return {};
    case Material::AIR: return kl::RGB{ 135, 206, 250 };
    case Material::ROCK: return kl::RGB{ 108, 113, 119 };
    case Material::SAND: return kl::RGB{ 226, 202, 118 };
    case Material::WATER: return kl::RGB{ 3, 71, 112 };
    }
}

struct Simulator
{
    kl::Window window{ "Particle Simulator" };
    kl::GPU gpu{ window.ptr(), kl::IS_DEBUG, true };
    kl::Timer timer{};

    Material selected_material = Material::SAND;
    float brush_radius = 25.0f;

    Simulator();

    bool update();

private:
    kl::ComputeShader m_fill_air_shader{};
    kl::ComputeShader m_add_material_shader{};
    kl::ComputeShader m_physics_shader{};
    kl::ComputeShader m_copy_frame_shader{};
    kl::ComputeShader m_ui_shader{};

    kl::Texture m_particle_texture{ gpu };
    kl::dx::AccessView m_back_buffer_av{};

    void resize_buffers( kl::Int2 size );
    void fill_air();
    void copy_reformat_frame();
    void add_material( kl::Int2 pos, Material material );
    void draw_ui();

    void handle_input();
    void handle_physics();
    void handle_render();
};
