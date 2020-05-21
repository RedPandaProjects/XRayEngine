#define USE_HUD_TRANSFORMATION_UNIFORM
#include "common.h"

ROOT_SIGNATURE_GRAPHICS
PS_TL0uv_IN main(VS_TL0uv_IN input  )
{
	PS_TL0uv_IN res;
	res.position = input.position;
	res.position.x=res.position.x*screen.z*2-1;
	res.position.y=res.position.y*screen.w*2-1;
	res.position.y*=-1;
	res.color.r=float(input.diffuse[2])/255.f;
	res.color.g=float(input.diffuse[1])/255.f;
	res.color.b=float(input.diffuse[0])/255.f;
	res.color.a=float(input.diffuse[3])/255.f;

	return res;
}