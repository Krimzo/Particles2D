static const float CIRCLE_THICKNESS = 5.0f;

float4 BRUSH_COLOR;
int2 BRUSH_POSITION;
float BRUSH_RADIUS;

RWTexture2D<unorm float4> DEST : register(u0);

[numthreads(32, 32, 1)]
void c_shader(uint3 thread_id : SV_DispatchThreadID)
{
    uint2 frame_size = 0;
    DEST.GetDimensions(frame_size.x, frame_size.y);

    if (thread_id.x >= frame_size.x
        || thread_id.y >= frame_size.y)
        return;
    
    const float dist = distance(thread_id.xy, BRUSH_POSITION);
    if (dist >= (BRUSH_RADIUS - CIRCLE_THICKNESS) && dist <= BRUSH_RADIUS)
        DEST[thread_id.xy] = BRUSH_COLOR;
}
