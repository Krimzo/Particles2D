static const uint MATERIAL_AIR = 0;
static const uint MATERIAL_ROCK = 1;
static const uint MATERIAL_SAND = 2;
static const uint MATERIAL_WATER = 3;

RWTexture2D<uint> FRAME : register(u0);

bool in_bounds(int2 pos);
void update_sand_at(int2 pos);
void update_water_at(int2 pos);

[numthreads(32, 32, 1)]
void c_shader(uint3 thread_id : SV_DispatchThreadID)
{
    uint2 frame_size = 0;
    FRAME.GetDimensions(frame_size.x, frame_size.y);

    if (thread_id.x >= frame_size.x
        || thread_id.y >= frame_size.y)
        return;

    const uint value = FRAME[thread_id.xy];
    if (value == MATERIAL_SAND)
        update_sand_at(thread_id.xy);
    else if (value == MATERIAL_WATER)
        update_water_at(thread_id.xy);
}

bool in_bounds(int2 pos)
{
    int2 frame_size = 0;
    FRAME.GetDimensions(frame_size.x, frame_size.y);
    return pos.x >= 0 && pos.x < frame_size.x
        && pos.y >= 0 && pos.y < frame_size.y;
}

void update_sand_at(int2 pos)
{
    static const int2 OFFSETS[3] = {
        int2(0, 1),
        int2(-1, 1),
        int2(1, 1) };

    [unroll]
    for (int i = 0; i < 3; i++)
    {
        const int2 new_pos = pos + OFFSETS[i];
        if (!in_bounds(new_pos))
            continue;

        uint orig;
        InterlockedCompareExchange(FRAME[new_pos], MATERIAL_AIR,
            MATERIAL_SAND, orig);
        if (orig == MATERIAL_AIR)
        {
            FRAME[pos] = MATERIAL_AIR;
            break;
        }

        InterlockedCompareExchange(FRAME[new_pos], MATERIAL_WATER,
            MATERIAL_SAND, orig);
        if (orig == MATERIAL_WATER)
        {
            FRAME[pos] = MATERIAL_WATER;
            break;
        }
    }
}

void update_water_at(int2 pos)
{
    static const int2 OFFSETS[5] = {
        int2(0, 1),
        int2(-1, 1),
        int2(1, 1),
        int2(-1, 0),
        int2(1, 0) };

    [unroll]
    for (int i = 0; i < 5; i++)
    {
        const int2 new_pos = pos + OFFSETS[i];
        if (!in_bounds(new_pos))
            continue;

        uint orig;
        InterlockedCompareExchange(FRAME[new_pos], MATERIAL_AIR,
            MATERIAL_WATER, orig);
        if (orig == MATERIAL_AIR)
        {
            FRAME[pos] = MATERIAL_AIR;
            break;
        }
    }
}
