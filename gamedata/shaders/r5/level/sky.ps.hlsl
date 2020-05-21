#define USE_GLOBAL_UNIFORM
#include <common.h> 

BINDING_0 TextureCube t_base: register( t0 );
BINDING_1 SamplerState sampler1: register( s0 );

RegRootSignature(DeafultRS)
float4 main(PS_SKY_IN input) : SV_Target
{
    float4 base = t_base.Sample(sampler1, input.uv1);
  //  base.rgb = lerp(base.rgb, FogColor.rgb, saturate((input.position.y) / 200));
    return float4(base.rgb, 1.f);

}