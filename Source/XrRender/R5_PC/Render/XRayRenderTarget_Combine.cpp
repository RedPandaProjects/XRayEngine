#include "pch.h"
void XRayRenderTarget::RenderCombine1()
{
	HW->Context->Lock(GRenderTarget->FrameBuffer_Generic1);
	HW->Context->SetFrameBuffer(GRenderTarget->FrameBuffer_Generic1);

	BearContextEventLock Event(*HW->Context.get(), "RenderCombine1", BearColor::Brown);
	g_pGamePersistent->Environment().RenderSky();
	g_pGamePersistent->Environment().RenderClouds();

	GRenderInterface.UpdateDescriptorHeap(m_Combine_Shader.E[0][0]);
	m_Combine_Shader.E[0][0].DescriptorHeap->SetUniformBuffer(XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_GlobalUniform), GUniformAllocator->GlobalUniform, 0);
	m_Combine_Shader.E[0][0].Set(HW->Context, FVF::F_TL);
	HW->Context->SetVertexBuffer(m_ScreenSquare_VertexBuffer);
	if (m_Combine_Shader.E[0][0].StencilRef >= 0)
		HW->Context->SetStencilRef(m_Combine_Shader.E[0][0].StencilRef);
	HW->Context->Draw(6);

	g_pGamePersistent->Environment().RenderFlares();


}
void XRayRenderTarget::RenderCombine2()
{
	HW->Context->ClearFrameBuffer();
	HW->Context->Unlock(GRenderTarget->FrameBuffer_Generic1);

	RenderBloom();

	HW->Context->SetFrameBuffer(GRenderTarget->FrameBuffer_Generic2);
	BearContextEventLock Event(*HW->Context.get(), "RenderCombine2", BearColor::Brown);

	GRenderInterface.UpdateDescriptorHeap(m_Combine_Shader.E[0][1]);
	m_Combine_Shader.E[0][1].DescriptorHeap->SetUniformBuffer(XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_GlobalUniform), GUniformAllocator->GlobalUniform, 0);
	m_Combine_Shader.E[0][1].Set(HW->Context, FVF::F_TL);
	HW->Context->SetVertexBuffer(m_ScreenSquare_VertexBuffer);
	HW->Context->Draw(6);

}