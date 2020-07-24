#include "common.h.hlsl"

PS_IN main(VS_IN input)
{
    PS_IN result;
    result.position = float4(input.position, 1.f);
    return result;
}