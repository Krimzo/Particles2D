static const uint MATERIAL_AIR = 0;
static const uint MATERIAL_ROCK = 1;
static const uint MATERIAL_SAND = 2;
static const uint MATERIAL_WATER = 3;

static const int2 BAD_INDEX = -1;

Texture2D<uint> FRAME_COPY : register(t0);
RWTexture2D<uint> FRAME : register(u0);

bool in_bounds(int2 pos);
int2 goes_to(int2 pos);
int2 comes_from(int2 pos);
int2 sand_goes_to(int2 pos);
int2 water_goes_to(int2 pos);

[numthreads(32, 32, 1)]
void c_shader(uint3 thread_id : SV_DispatchThreadID)
{
    const int2 current_index = thread_id.xy;
    if (!in_bounds(current_index))
        return;

    const int2 to = goes_to(current_index);
    if (all(to != BAD_INDEX) && all(comes_from(to) == current_index))
    {
        FRAME[current_index] = FRAME_COPY[to];
        return;
    }
    
    const int2 from = comes_from(current_index);
    if (all(from != BAD_INDEX) && all(goes_to(from) == current_index))
    {
        FRAME[current_index] = FRAME_COPY[from];
        return;
    }
}

bool in_bounds(int2 pos)
{
    int2 frame_size = 0;
    FRAME_COPY.GetDimensions(frame_size.x, frame_size.y);
    return pos.x >= 0 && pos.x < frame_size.x
        && pos.y >= 0 && pos.y < frame_size.y;
}

int2 goes_to(int2 pos)
{
    if (!in_bounds(pos))
        return BAD_INDEX;
    switch (FRAME_COPY[pos])
    {
    default:
        return BAD_INDEX;
        break;
        
    case MATERIAL_SAND:
        return sand_goes_to(pos);
        break;

    case MATERIAL_WATER:
        return water_goes_to(pos);
        break;
    }
}

int2 comes_from(int2 pos)
{
    if (!in_bounds(pos))
        return BAD_INDEX;
    static const int NEIGHBOUR_COUNT = 5;
    static const int2 NEIGHBOUR_CELLS[NEIGHBOUR_COUNT] =
    {
        int2(0, -1),
        int2(-1, -1),
        int2(1, -1),
        int2(-1, 0),
        int2(1, 0),
    };
    int2 retval = BAD_INDEX;
    [unroll]
    for (int i = 0; i < NEIGHBOUR_COUNT; i++)
    {
        const int2 neighbour_cell = pos + NEIGHBOUR_CELLS[i];
        if (all(goes_to(neighbour_cell) == pos))
        {
            retval = neighbour_cell;
            break;
        }
    }
    return retval;
}

int2 sand_goes_to(int2 pos)
{
    static const int OFFSET_COUNT = 3;
    static const int2 OFFSETS[OFFSET_COUNT] =
    {
        int2(0, 1),
        int2(-1, 1),
        int2(1, 1),
    };
    int2 retval = BAD_INDEX;
    [unroll]
    for (int i = 0; i < OFFSET_COUNT; i++)
    {
        const int2 offset_pos = pos + OFFSETS[i];
        if (!in_bounds(offset_pos))
            continue;
        const uint offset_mat = FRAME_COPY[offset_pos];
        if (offset_mat == MATERIAL_AIR || offset_mat == MATERIAL_WATER)
        {
            retval = offset_pos;
            break;
        }
    }
    return retval;
}

int2 water_goes_to(int2 pos)
{
    static const int OFFSET_COUNT = 3;
    static const int2 OFFSETS[OFFSET_COUNT] =
    {
        int2(0, 1),
        int2(-1, 0),
        int2(1, 0),
    };
    int2 retval = BAD_INDEX;
    [unroll]
    for (int i = 0; i < OFFSET_COUNT; i++)
    {
        const int2 offset_pos = pos + OFFSETS[i];
        if (!in_bounds(offset_pos))
            continue;
        const uint offset_mat = FRAME_COPY[offset_pos];
        if (offset_mat == MATERIAL_AIR)
        {
            retval = offset_pos;
            break;
        }
    }
    return retval;
}
