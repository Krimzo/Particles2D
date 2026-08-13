static uint4 create_material(uint r, uint g, uint b)
{
    return uint4(r, g, b, 255);
}

static const uint4 MATERIAL_AIR = create_material(135, 206, 250);
static const uint4 MATERIAL_ROCK = create_material(108, 113, 119);
static const uint4 MATERIAL_SAND = create_material(226, 202, 118);
static const uint4 MATERIAL_WATER = create_material(3, 71, 112);

RWTexture2D<uint4> FRAME : register(u0);

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

    if (all(FRAME[thread_id.xy] == MATERIAL_SAND))
        update_sand_at(thread_id.xy);
    else if (all(FRAME[thread_id.xy] == MATERIAL_WATER))
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

        if (all(FRAME[new_pos] != MATERIAL_AIR)
            && all(FRAME[new_pos] != MATERIAL_WATER))
            continue;

        const uint4 temp = FRAME[new_pos];
        FRAME[new_pos] = FRAME[pos];
        FRAME[pos] = temp;
        break;
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

        if (all(FRAME[new_pos] != MATERIAL_AIR))
            continue;

        const uint4 temp = FRAME[new_pos];
        FRAME[new_pos] = FRAME[pos];
        FRAME[pos] = temp;
        break;
    }
}
