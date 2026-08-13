static uint4 create_material(uint r, uint g, uint b)
{
    return uint4(r, g, b, 255);
}

static const uint4 MATERIAL_AIR = create_material(135, 206, 250);
static const uint4 MATERIAL_ROCK = create_material(108, 113, 119);
static const uint4 MATERIAL_SAND = create_material(226, 202, 118);
static const uint4 MATERIAL_WATER = create_material(3, 71, 112);

RWTexture2D<uint4> FRAME : register(u0);

[numthreads(32, 32, 1)]
void c_shader(uint3 thread_id : SV_DispatchThreadID)
{
    uint2 frame_size = 0;
    FRAME.GetDimensions(frame_size.x, frame_size.y);

    if (thread_id.x >= frame_size.x
        || thread_id.y >= frame_size.y)
        return;

    // ...
}
