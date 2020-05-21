#define USE_HUD_TRANSFORMATION_UNIFORM
#include "common.h"



ROOT_SIGNATURE_GRAPHICS
PS_BUILD_IN main(VS_BUILD_IN input)
{
    PS_BUILD_IN res;
    input.position.xy += 0.5f;
	res.position = input.position;
	res.position.x=res.position.x*screen.z*2-1;
	res.position.y=res.position.y*screen.w*2-1;
	res.position.y*=-1;
    res.uv0 = input.uv0;
    res.uv1 = input.uv1;
    res.uv2 = input.uv2;
    res.uv3 = input.uv3;
	
	return res;
}