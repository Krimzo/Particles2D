static const uint MATERIAL_AIR = 0;
static const uint MATERIAL_ROCK = 1;
static const uint MATERIAL_SAND = 2;
static const uint MATERIAL_WATER = 3;

RWTexture2D<uint> FRAME : register(u0);

[numthreads(32, 32, 1)]
void c_shader(uint3 thread_id : SV_DispatchThreadID)
{
    uint2 frame_size = 0;
    FRAME.GetDimensions(frame_size.x, frame_size.y);

    if (thread_id.x >= frame_size.x
        || thread_id.y >= frame_size.y)
        return;

    const uint value = FRAME[thread_id.xy];
    if (value == MATERIAL_AIR
        || value == MATERIAL_ROCK
        || value == MATERIAL_SAND
        || value == MATERIAL_WATER)
        return;

    FRAME[thread_id.xy] = MATERIAL_AIR;
}
