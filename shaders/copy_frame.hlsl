static const uint MATERIAL_AIR = 0;
static const uint MATERIAL_ROCK = 1;
static const uint MATERIAL_SAND = 2;
static const uint MATERIAL_WATER = 3;

static const float4 MATERIAL_COLOR_AIR = uint4(135, 206, 250, 255) / 255.0f;
static const float4 MATERIAL_COLOR_ROCK = uint4(108, 113, 119, 255) / 255.0f;
static const float4 MATERIAL_COLOR_SAND = uint4(226, 202, 118, 255) / 255.0f;
static const float4 MATERIAL_COLOR_WATER = uint4(3, 71, 112, 255) / 255.0f;

RWTexture2D<uint> SRC : register(u0);
RWTexture2D<unorm float4> DEST : register(u1);

[numthreads(32, 32, 1)]
void c_shader(uint3 thread_id : SV_DispatchThreadID)
{
    uint2 frame_size = 0;
    SRC.GetDimensions(frame_size.x, frame_size.y);

    if (thread_id.x >= frame_size.x
        || thread_id.y >= frame_size.y)
        return;

    switch (SRC[thread_id.xy])
    {
    case MATERIAL_AIR:
        DEST[thread_id.xy] = MATERIAL_COLOR_AIR;
        break;

    case MATERIAL_ROCK:
        DEST[thread_id.xy] = MATERIAL_COLOR_ROCK;
        break;

    case MATERIAL_SAND:
        DEST[thread_id.xy] = MATERIAL_COLOR_SAND;
        break;

    case MATERIAL_WATER:
        DEST[thread_id.xy] = MATERIAL_COLOR_WATER;
        break;
    }
}
