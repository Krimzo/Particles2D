#pragma once

#include "klibrary.h"


enum struct Material : byte
{
    AIR = 0,
    ROCK,
    SAND,
    WATER,
};

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
    kl::ComputeShader m_copy_frame_shader{};
    kl::ComputeShader m_add_material_shader{};
    kl::ComputeShader m_physics_shader{};

    kl::Texture m_particle_texture{ gpu };
    kl::dx::AccessView m_back_buffer_av{};

    void resize_buffers( kl::Int2 size );
    void fill_air();
    void copy_reformat_frame();
    void add_material( kl::Int2 pos, Material material );

    void handle_input();
    void handle_physics();
    void handle_render();
};
