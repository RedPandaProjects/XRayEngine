#include "pch.h"

XRayRenderTarget::XRayRenderTarget()
{
	RTVPosition = GResourcesManager->GetTexture(RT_POSITION);
	RTVPosition->Texture = BearRenderInterface::CreateTexture2D(RDEVICE.dwWidth, RDEVICE.dwHeight, RTF_R32G32B32A32F);

	RTVBasicColor = GResourcesManager->GetTexture(RT_BASIC);
	RTVBasicColor->Texture = BearRenderInterface::CreateTexture2D(RDEVICE.dwWidth, RDEVICE.dwHeight, RTF_R32G32B32A32F);
	DSVBasic = GResourcesManager->GetTexture(DS_BASIC);
	DSVBasic->Texture = BearRenderInterface::CreateTexture2D(RDEVICE.dwWidth, RDEVICE.dwHeight, DSF_DEPTH32F_STENCIL8);

	RTVOutput = GResourcesManager->GetTexture(RT_OUTPUT);
	RTVOutput->Texture = BearRenderInterface::CreateTexture2D(RDEVICE.dwWidth, RDEVICE.dwHeight, RTF_R32G32B32A32F);

	{

		BearRenderPassDescription RenderPassDescription;
		RenderPassDescription.RenderTargets[0].Clear = true;
		RenderPassDescription.RenderTargets[0].Color = BearColor::Transparent;
		RenderPassDescription.RenderTargets[0].Format = RTF_R32G32B32A32F;
		RenderPassDescription.RenderTargets[1].Clear = true;
		RenderPassDescription.RenderTargets[1].Color = BearColor::Transparent;
		RenderPassDescription.RenderTargets[1].Format = RTF_R32G32B32A32F;
		RenderPassDescription.DepthStencil.Clear = true;
		RenderPassDescription.DepthStencil.Depth = 1.f;
		RenderPassDescription.DepthStencil.Stencil = 0;
		RenderPassDescription.DepthStencil.Format = DSF_DEPTH32F_STENCIL8;
		RenderPass_Base = BearRenderInterface::CreateRenderPass(RenderPassDescription);


	}

	{
		BearFrameBufferDescription FrameBufferDescription;
		FrameBufferDescription.RenderPass = RenderPass_Base;
		FrameBufferDescription.RenderTargets[0] = RTVBasicColor->Texture;
		FrameBufferDescription.RenderTargets[1] = RTVPosition->Texture;
		FrameBufferDescription.DepthStencil = DSVBasic->Texture;
		FrameBuffer_Base = BearRenderInterface::CreateFrameBuffer(FrameBufferDescription);
	}
	{

		BearRenderPassDescription RenderPassDescription;
		RenderPassDescription.RenderTargets[0].Clear = false;
		RenderPassDescription.RenderTargets[0].Color = BearColor::Transparent;
		RenderPassDescription.RenderTargets[0].Format = RTF_R32G32B32A32F;
		RenderPassDescription.DepthStencil.Clear = false;
		RenderPassDescription.DepthStencil.Format = DSF_DEPTH32F_STENCIL8;
		RenderPass_Output = BearRenderInterface::CreateRenderPass(RenderPassDescription);
	}
	{
		BearFrameBufferDescription FrameBufferDescription;
		FrameBufferDescription.RenderPass = RenderPass_Output;
		FrameBufferDescription.RenderTargets[0] = RTVOutput->Texture;
		FrameBufferDescription.DepthStencil = DSVBasic->Texture;
		FrameBuffer_Output = BearRenderInterface::CreateFrameBuffer(FrameBufferDescription);
	}
}

XRayRenderTarget::~XRayRenderTarget()
{

	GResourcesManager->Free(RTVOutput);
	GResourcesManager->Free(RTVPosition);
	GResourcesManager->Free(RTVBasicColor);
	GResourcesManager->Free(DSVBasic);
}

void XRayRenderTarget::set_blur(float f)
{
}

void XRayRenderTarget::set_gray(float f)
{
}

void XRayRenderTarget::set_duality_h(float f)
{
}

void XRayRenderTarget::set_duality_v(float f)
{
}

void XRayRenderTarget::set_noise(float f)
{
}

void XRayRenderTarget::set_noise_scale(float f)
{
}

void XRayRenderTarget::set_noise_fps(float f)
{
}

void XRayRenderTarget::set_color_base(u32 f)
{
}

void XRayRenderTarget::set_color_gray(u32 f)
{
}

void XRayRenderTarget::set_color_add(const Fvector & f)
{
}

u32 XRayRenderTarget::get_width()
{
	return RDEVICE.dwWidth;
}

u32 XRayRenderTarget::get_height()
{
	return RDEVICE.dwHeight;
}

void XRayRenderTarget::set_cm_imfluence(float f)
{
}

void XRayRenderTarget::set_cm_interpolate(float f)
{
}

void XRayRenderTarget::set_cm_textures(const shared_str & tex0, const shared_str & tex1)
{
}
XRayRenderTarget* GRenderTarget = 0;