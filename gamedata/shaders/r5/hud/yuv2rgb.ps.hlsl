#include "common.h"
BINDING_SRV_0 Texture2D texture1: register( t0 );
BINDING_SAMPLER_0 SamplerState  sampler1: register( s0 );

ROOT_SIGNATURE_GRAPHICS
float4 main(PS_TL_IN input) : SV_Target
{
	float3 YUV =  texture1.Sample(sampler1, input.uv).rgb;
    float	Y	= YUV.z;
	float	U	= YUV.y;
	float	V	= YUV.x;

	/*
	float	_T = 1.16406f*Y;
	float R1 = _T 				+ 1.59765f*V 	- 0.86961f;
	float G1 = _T - 0.390625f*U 	- 0.8125f*V  	+ 0.53076f;
	float B1 = _T + 2.01562f*U 					- 1.0786f;
	*/

	float	c	= 1.16406f	;
	float3	_Y	= float3		(c,				c,			c)			*Y	;
	float3	_U	= float3		(0, 			-0.390625f, +2.01562f)	*U	;
	float3	_V	= float3		(+1.59765f,		-0.8125f,	0)			*V	;
	float3	_S	= float3		(-0.86961f,		+0.53076f,	-1.0786f)		;
	

	return float4(_Y+_U+_V+_S,1);

}