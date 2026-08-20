static const uint MATERIAL_AIR = 0;
static const uint MATERIAL_ROCK = 1;
static const uint MATERIAL_SAND = 2;
static const uint MATERIAL_WATER = 3;

int2 BRUSH_POSITION;
float BRUSH_RADIUS;
uint BRUSH_MATERIAL;
int OVERWRITE_EXISTING;

RWTexture2D<uint> FRAME : register(u0);

[numthreads(32, 32, 1)]
void c_shader(uint3 thread_id : SV_DispatchThreadID)
{
    uint2 frame_size = 0;
    FRAME.GetDimensions(frame_size.x, frame_size.y);

    if (thread_id.x >= frame_size.x
        || thread_id.y >= frame_size.y)
        return;
    
    if (!OVERWRITE_EXISTING && FRAME[thread_id.xy] != MATERIAL_AIR)
        return;

    if (distance(thread_id.xy, BRUSH_POSITION) <= BRUSH_RADIUS)
        FRAME[thread_id.xy] = BRUSH_MATERIAL;
}
