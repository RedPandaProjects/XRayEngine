#include "common.h"


ROOT_SIGNATURE_GRAPHICS
float4 main(PS_TL0uv_IN input) : SV_Target
{
	return  input.color;  
}