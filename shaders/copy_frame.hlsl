RWTexture2D<uint4> SRC : register(u0);
RWTexture2D<unorm float4> DEST : register(u1);

[numthreads(32, 32, 1)]
void c_shader(uint3 thread_id : SV_DispatchThreadID)
{
    uint2 frame_size = 0;
    SRC.GetDimensions(frame_size.x, frame_size.y);

    if (thread_id.x >= frame_size.x
        || thread_id.y >= frame_size.y)
        return;

    DEST[thread_id.xy] = float4(SRC[thread_id.xy]) / 255.0f;
}
