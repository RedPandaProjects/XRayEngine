#include "common.h.hlsl"
BINDING_UAV_0 RWTexture2D< float4 > gOutput : register(u0);
BINDING_SRV_0 RaytracingAccelerationStructure SceneBVH : register(t0);
[shader("raygeneration")]
void Main() 
{
    
    uint2 launchIndex = DispatchRaysIndex();
    
    HitInfo payload; 
    payload.colorAndDistance = float4(0, 0, 0, 0); 
    float2 dims = (float2)DispatchRaysDimensions(); 
    float2 d = (((launchIndex.xy + 0.5f) / dims.xy) * 2.f - 1.f);
    RayDesc ray; 
    ray.Origin = float3(d.x, -d.y, 1); 
    ray.Direction = float3(0, 0, -1); 
    ray.TMin = 0; 
    ray.TMax = 100000;
    TraceRay(SceneBVH, RAY_FLAG_NONE, 0xFF, 0, 0, 0, ray, payload);
    gOutput[launchIndex] = float4( payload.colorAndDistance.rgb, 1.f);
}