#pragma once
class XRayFontRender;
class XRayHardware
{
public:
	XRayHardware();
	~XRayHardware();
	void Update(HWND hWnd, u32& dwWidth, u32& dwHeight, float& fWidth_2, float& fHeight_2);
	bool MeshShaderSupport() const;
	bool DevOldShaderSupport() const;
	BearFactoryPointer<BearRHI::BearRHIContext> Context;
	xr_vector< XRayFontRender*> FontRenders;
	BearFactoryPointer<BearRHI::BearRHIViewport> Viewport;
	BearFactoryPointer<BearRHI::BearRHIRenderPass> RenderPass_Viewport;
	BearFactoryPointer<BearRHI::BearRHIRenderPass> RenderPass_Viewport_ForCombone;
};
extern XRayHardware* HW;