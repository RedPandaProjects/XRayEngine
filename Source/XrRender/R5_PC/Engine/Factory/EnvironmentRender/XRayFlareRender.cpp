#include "pch.h"

XRayFlareRender::XRayFlareRender()
{
}

XRayFlareRender::~XRayFlareRender()
{
}

void XRayFlareRender::Copy(IFlareRender & _in)
{
	Shader = static_cast<XRayFlareRender&>(_in).Shader;
}

void XRayFlareRender::CreateShader(LPCSTR sh_name, LPCSTR tex_name)
{
	XRayShader shader;
	if (tex_name && tex_name[0])
		GResourcesManager->CompileBlender(shader, sh_name, tex_name);
	Shader = XRayShaderRef(shader);
}

void XRayFlareRender::DestroyShader()
{
	Shader.clear();
}
