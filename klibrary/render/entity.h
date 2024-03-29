#pragma once

#include "gpu/dx_types.h"
#include "math/math.h"


namespace kl {
    class entity
    {
    public:
        float3 scale = {1.0f, 1.0f, 1.0f};
        float3 position = {};
        float3 rotation = {};

        float3 acceleration = {};
        float3 velocity = {};
        float3 angular = {};

        dx::buffer mesh = nullptr;
        dx::shader_view texture = nullptr;
        float4 color = float4(colors::gray);

        entity();

        void update_physics(float delta_time);

        [[nodiscard]] mat4 matrix() const;
    };
}
