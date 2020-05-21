#include "common.h"
BINDING_SRV_0 Texture2D texture1: register( t0 );
BINDING_SAMPLER_0 SamplerState sampler1: register( s0 );

ROOT_SIGNATURE_GRAPHICS
PS_BASE_OUT main(PS_BASE_IN input) 
{
    
    float4 result =  texture1.Sample(sampler1, input.uv);
    
 #ifdef USE_AREF
    clip(result.w-AlphaRef);
#endif
    PS_BASE_OUT output;
    output.color = result;
#ifndef USE_BLEND
    output.compute.z = input.out_position_z;
    output.compute.xyw = float3(1.f, 1.f, 1.f);
    output.bloom = float4(0, 0, 0, 0);
#endif
    return output;
}