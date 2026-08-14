int2 BRUSH_POSITION;
float BRUSH_RADIUS;
uint BRUSH_MATERIAL;

RWTexture2D<uint> FRAME : register(u0);

[numthreads(32, 32, 1)]
void c_shader(uint3 thread_id : SV_DispatchThreadID)
{
    uint2 frame_size = 0;
    FRAME.GetDimensions(frame_size.x, frame_size.y);

    if (thread_id.x >= frame_size.x
        || thread_id.y >= frame_size.y)
        return;

    if (distance(thread_id.xy, BRUSH_POSITION) <= BRUSH_RADIUS)
        FRAME[thread_id.xy] = BRUSH_MATERIAL;
}
