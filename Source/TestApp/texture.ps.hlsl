#include "common.h.hlsl"
BINDING_SRV_0 Texture2D t_basic_color : register(t0);
BINDING_SAMPLER_0 SamplerState sdefault : register(s0);

float4 main(PS_TEXTURE_IN input) : SV_Target
{
    return t_basic_color.Sample(sdefault, input.uv);

}