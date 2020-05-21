#include "common.h"
BINDING_SRV_0 Texture2D texture1: register( t0 );
BINDING_SAMPLER_0 SamplerState sampler1: register( s0 );

ROOT_SIGNATURE_GRAPHICS
float4 main(PS_TL_IN input) : SV_Target
{
	float4 r = texture1.Sample(sampler1, input.uv);
	r.w = 1 - r.w;
	return  r;  
}