#define USE_GLOBAL_UNIFORM
#include "common.h"
BINDING_SRV_0 Texture2D s_base: register( t0 );
BINDING_SRV_1 Texture2D s_compute: register( t1 );
//BINDING_0 Texture2D s_sky: register( t2 );
BINDING_SAMPLER_0 SamplerState s_default: register( s0 );

ROOT_SIGNATURE_GRAPHICS
float4 main(PS_TL_IN input) : SV_Target
{
    float skyblend = 0;
    float4 base = s_base.Sample(s_default, input.uv);
    float4 compute = s_compute.Sample(s_default, input.uv);
    float3 pos = float3(compute.z * (input.uv * PosDecompressParams.zw - PosDecompressParams.xy), compute.z);
    float distance = length(pos.xyz);
    float fog = saturate(distance * FogParam.w + FogParam.x); //
  //  float3 color = lerp(base.rgb, FogColor.rgb, fog); //
    skyblend = saturate(fog * fog);
 //  float3 color = lerp(base.rgb, sky.rgb, skyblend);W
//    color = lerp(sky.rgb, color, skyblend);
        
    
    return float4(base.rgb, (1 - skyblend));

}