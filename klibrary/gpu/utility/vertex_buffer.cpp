#include "gpu/gpu.h"

#include "utility/file.h"


kl::dx::buffer kl::gpu::new_vertex_buffer(const std::vector<vertex>& vertex_data)
{
    dx::buffer_descriptor descriptor = {};
    descriptor.ByteWidth = static_cast<uint32_t>(vertex_data.size() * sizeof(vertex));
    descriptor.Usage = D3D11_USAGE_DEFAULT;
    descriptor.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    dx::subresource_descriptor data = {};
    data.pSysMem = vertex_data.data();

    return new_buffer(&descriptor, &data);
}

kl::dx::buffer kl::gpu::new_vertex_buffer(const std::string& filepath, const bool flip_z)
{
    return new_vertex_buffer(files::parse_mesh(filepath, flip_z));
}

kl::dx::buffer kl::gpu::generate_screen_mesh()
{
    return new_vertex_buffer({
        vertex({-1.0f, -1.0f, 0.5f}), vertex({-1.0f, 1.0f, 0.5f}), vertex({1.0f, 1.0f, 0.5f}),
        vertex({1.0f, 1.0f, 0.5f}), vertex({1.0f, -1.0f, 0.5f}), vertex({-1.0f, -1.0f, 0.5f})
    });
}

kl::dx::buffer kl::gpu::generate_plane_mesh(const float size, int num_of_points)
{
    // Count fix
    const float increment = size / static_cast<float>(num_of_points);
    num_of_points += 1;

    // Generating points
    std::vector<float2> points = {};
    points.reserve(static_cast<uint64_t>(num_of_points) * num_of_points);
    for (int x = 0; x < num_of_points; x++) {
        for (int z = 0; z < num_of_points; z++) {
            points.emplace_back(
                -size * 0.5f + increment * static_cast<float>(x),
                -size * 0.5f + increment * static_cast<float>(z)
            );
        }
    }

    // Generating triangles
    std::vector<vertex> vertices = {};
    vertices.reserve(static_cast<uint64_t>(num_of_points - 1) * (num_of_points - 1) * 6);
    for (int x = 0; x < num_of_points - 1; x++) {
        for (int z = 0; z < num_of_points - 1; z++) {
            const float2& xz_point = points[z * num_of_points + x];
            const float2& xz_plus_point = points[(z + 1) * num_of_points + (x + 1)];

            vertices.push_back(vertex({xz_plus_point.x, 0, xz_plus_point.y}));
            vertices.push_back(vertex({xz_plus_point.x, 0, xz_point.y}));
            vertices.push_back(vertex({xz_point.x, 0, xz_point.y}));

            vertices.push_back(vertex({xz_point.x, 0, xz_point.y}));
            vertices.push_back(vertex({xz_point.x, 0, xz_plus_point.y}));
            vertices.push_back(vertex({xz_plus_point.x, 0, xz_plus_point.y}));
        }
    }
    return new_vertex_buffer(vertices);
}

void kl::gpu::draw_vertex_buffer(const dx::buffer buffer) const
{
    constexpr uint32_t offset = 0;
    constexpr uint32_t stride = sizeof(vertex);
    context_->IASetVertexBuffers(0, 1, &buffer, &stride, &offset);

    dx::buffer_descriptor descriptor = {};
    buffer->GetDesc(&descriptor);

    context_->Draw(descriptor.ByteWidth / stride, 0);
}
