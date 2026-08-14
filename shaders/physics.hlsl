static const uint MATERIAL_AIR = 0;
static const uint MATERIAL_ROCK = 1;
static const uint MATERIAL_SAND = 2;
static const uint MATERIAL_WATER = 3;

int RAND_SEED;

RWTexture2D<uint> FRAME : register(u0);

int random_sign(int2 pos);
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

int random_sign(int2 pos)
{
    uint2 frame_size = 0;
    FRAME.GetDimensions(frame_size.x, frame_size.y);
    const int index = pos.x + pos.y * (int) frame_size.x;
    int v = RAND_SEED + index;
    v ^= v >> 16;
    v *= 0x7feb352du;
    v ^= v >> 15;
    v *= 0x846ca68bu;
    v ^= v >> 16;
    return (v & 1) ? 1 : -1;
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
    const int rand_dir = random_sign(pos);
    const int2 OFFSETS[3] = {
        int2(0, 1),
        int2(-rand_dir, 1),
        int2(rand_dir, 1) };

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
    const int rand_dir = random_sign(pos);
    const int2 OFFSETS[3] = {
        int2(0, 1),
        int2(-rand_dir, 0),
        int2(rand_dir, 0) };

    [unroll]
    for (int i = 0; i < 3; i++)
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
