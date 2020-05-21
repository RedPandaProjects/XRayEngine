#define USE_GLOBAL_UNIFORM
#include "common.h"
BINDING_SRV_0 Texture2D s_image: register( t0 );
BINDING_SAMPLER_0 SamplerState smp_rtlinear: register( s0 );

ROOT_SIGNATURE_GRAPHICS
float4 main(PS_FILTER_IN input) : SV_Target
{
    float4 accum = BloomW[1].w * s_image.Sample(smp_rtlinear, input.uv0.xy);

    accum += BloomW[0].x * s_image.Sample(smp_rtlinear, input.uv1.xy);
    accum += BloomW[0].x * s_image.Sample(smp_rtlinear, input.uv1.wz);

    accum += BloomW[0].y * s_image.Sample(smp_rtlinear, input.uv2.xy);
    accum += BloomW[0].y * s_image.Sample(smp_rtlinear, input.uv2.wz);
    
    accum += BloomW[0].z * s_image.Sample(smp_rtlinear, input.uv3.xy);
    accum += BloomW[0].z * s_image.Sample(smp_rtlinear, input.uv3.wz);

    accum += BloomW[0].w * s_image.Sample(smp_rtlinear, input.uv4.xy);
    accum += BloomW[0].w * s_image.Sample(smp_rtlinear, input.uv4.wz);

    accum += BloomW[1].x * s_image.Sample(smp_rtlinear, input.uv5.xy);
    accum += BloomW[1].x * s_image.Sample(smp_rtlinear, input.uv5.wz);

    accum += BloomW[1].y * s_image.Sample(smp_rtlinear, input.uv6.xy);
    accum += BloomW[1].y * s_image.Sample(smp_rtlinear, input.uv6.wz);

    accum += BloomW[1].z * s_image.Sample(smp_rtlinear, input.uv7.xy);
    accum += BloomW[1].z * s_image.Sample(smp_rtlinear, input.uv7.wz);

    return accum;
}