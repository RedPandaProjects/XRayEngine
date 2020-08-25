#include "common.h.hlsl"

PS_TEXTURE_IN main(VS_TEXTURE_IN input)
{
    PS_TEXTURE_IN result;
    result.position = float4(input.position, 1.f);
    result.uv = input.uv;
    return result;
}