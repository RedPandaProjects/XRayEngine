#include "common.h"
BINDING_SRV_0 Texture2D t_basic_color: register( t0 );
BINDING_SAMPLER_0 SamplerState sdefault: register( s0 );


ROOT_SIGNATURE_GRAPHICS
float4 main(PS_TL_IN input) : SV_Target
{
    
    float3 basic_color = t_basic_color.Sample(sdefault, input.uv).rgb;
    return float4(basic_color, 1.f);
}