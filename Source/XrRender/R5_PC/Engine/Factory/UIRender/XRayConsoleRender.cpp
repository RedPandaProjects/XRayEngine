#include "pch.h"

XRayConsoleRender::XRayConsoleRender()
{
	GResourcesManager->CompileBlender(m_Shader, "hud\\crosshair", "");
}

void XRayConsoleRender::Copy(IConsoleRender & _in)
{
	m_Shader = static_cast<XRayConsoleRender&>(_in).m_Shader;
}

void XRayConsoleRender::OnRender(bool bGame)
{
	Ivector4 R = { 0,0,Device->dwWidth,Device->dwHeight };
	if (bGame) R.w /= 2;
	if (m_VertexBuffer.empty())
	{
		m_VertexBuffer = BearRenderInterface::CreateVertexBuffer();
		m_VertexBuffer->Create(sizeof(FVF::F_TL0uv), 6, true);
	}
	{

		FVF::TL0uv* verts = (FVF::TL0uv*)m_VertexBuffer->Lock();;
		verts->set(R.x, R.w, color_rgba(32, 32, 32,0));		verts++;
		verts->set(R.x, R.z, color_rgba(32, 32, 32, 0));	verts++;
		verts->set(R.y, R.w, color_rgba(32, 32, 32, 0));	verts++;

		verts->set(R.y, R.z, color_rgba(32, 32, 32, 0));	verts++;
		verts->set(R.y, R.w, color_rgba(32, 32, 32, 0));	verts++;
		verts->set(R.x, R.z, color_rgba(32, 32, 32, 0));	verts++;
		m_VertexBuffer->Unlock();
	}
	{
		HW->Context->SetVertexBuffer(m_VertexBuffer);
		GRenderInterface.UpdateDescriptorHeap(m_Shader.E[0][1]);
		if (!m_Shader.E[0][1].Set(HW->Context, FVF::F_TL0uv)) { return; }
		HW->Context->Draw(6);

	}
}
