#include "common.h.hlsl"
[shader("miss")] 
void Main(inout HitInfo payload : SV_RayPayload) 
{ 
	payload.colorAndDistance = float4(0.2f, 0.2f, 0.8f, -1.f); 
}