#pragma once
struct XRayShaderCounter :XRayShader
{
	XRayShaderCounter(const XRayShader& right)
	{
		Name = right.Name;
		for (size_t a = 0; a < 2; a++) for (size_t i = 0; i < 6; i++)E[a][i] = right.E[a][i];
	}
	size_t Counter;
};
