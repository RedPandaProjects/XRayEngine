#include "common.h.hlsl"
[shader("closesthit")]
void Main(inout HitInfo payload, Attributes attrib)
{
    float3 barycentrics = float3(1.f - attrib.bary.x - attrib.bary.y, attrib.bary.x, attrib.bary.y); 
    const float3 A = float3(1, 0, 0); 
    const float3 B = float3(0, 1, 0); 
    const float3 C = float3(0, 0, 1); 
    float3 hitColor = A * barycentrics.x + B * barycentrics.y + C * barycentrics.z; 
    payload.colorAndDistance = float4(hitColor, RayTCurrent()); 
}