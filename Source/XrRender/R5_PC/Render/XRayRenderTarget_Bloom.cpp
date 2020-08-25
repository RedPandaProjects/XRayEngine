#include "pch.h"


void XRayRenderTarget::RenderBloom()
{
	BearContextEventLock Event(*HW->Context.get(), "RenderBloom", BearColor::Blue);
	{
		HW->Context->Lock(GRenderTarget->FrameBuffer_Bloom_1);
		HW->Context->SetFrameBuffer(GRenderTarget->FrameBuffer_Bloom_1);
		float		_w = float(Device.dwWidth);
		float		_h = float(Device.dwHeight);
		float		_2w = _w / 2;	float tw = RT_BLOOM_SIZE_X;
		float		_2h = _h / 2; float th = RT_BLOOM_SIZE_Y;
		float		_aspect_w = _2w / tw;
		float		_aspect_h = _2h / th;
		Fvector2	one = { 1.f / _w, 1.f / _h };	one.x *= _aspect_w; one.y *= _aspect_h;
		Fvector2	half = { .5f / _w, .5f / _h };
		Fvector2	a_0 = { half.x + 0,		half.y + 0 };
		Fvector2	a_1 = { half.x + one.x, half.y + 0 };
		Fvector2	a_2 = { half.x + 0,		half.y + one.y };
		Fvector2	a_3 = { half.x + one.x,	half.y + one.y };
		Fvector2	b_0 = { 1 + a_0.x,		1 + a_0.y };
		Fvector2	b_1 = { 1 + a_1.x,		1 + a_1.y };
		Fvector2	b_2 = { 1 + a_2.x,		1 + a_2.y };
		Fvector2	b_3 = { 1 + a_3.x,		1 + a_3.y };

		if (m_Bloom_Build_VertexBuffer.empty())
		{
			m_Bloom_Build_VertexBuffer = BearRenderInterface::CreateVertexBuffer();
			m_Bloom_Build_VertexBuffer->Create(sizeof(FVF::V_BUILD), 6, true);
		}

		// Fill vertex buffer
		FVF::V_BUILD* pv = (FVF::V_BUILD*)m_Bloom_Build_VertexBuffer->Lock();
		pv->p.set(EPS, float(th + EPS), EPS, 1.f);
		pv->uv0.set(a_0.x, b_0.y);	pv->uv1.set(a_1.x, b_1.y);	pv->uv2.set(a_2.x, b_2.y);	pv->uv3.set(a_3.x, b_3.y);
		pv++;
		pv->p.set(EPS, EPS, EPS, 1.f);
		pv->uv0.set(a_0.x, a_0.y);	pv->uv1.set(a_1.x, a_1.y);	pv->uv2.set(a_2.x, a_2.y);	pv->uv3.set(a_3.x, a_3.y);
		pv++;
		pv->p.set(float(tw + EPS), float(th + EPS), EPS, 1.f);
		pv->uv0.set(b_0.x, b_0.y);	pv->uv1.set(b_1.x, b_1.y);	pv->uv2.set(b_2.x, b_2.y);	pv->uv3.set(b_3.x, b_3.y);
		pv++;

		pv->p.set(float(tw + EPS), EPS, EPS, 1.f);
		pv->uv0.set(b_0.x, a_0.y);	pv->uv1.set(b_1.x, a_1.y);	pv->uv2.set(b_2.x, a_2.y);	pv->uv3.set(b_3.x, a_3.y);
		pv++;
		pv->p.set(float(tw + EPS), float(th + EPS), EPS, 1.f);
		pv->uv0.set(b_0.x, b_0.y);	pv->uv1.set(b_1.x, b_1.y);	pv->uv2.set(b_2.x, b_2.y);	pv->uv3.set(b_3.x, b_3.y);
		pv++;
		pv->p.set(EPS, EPS, EPS, 1.f);
		pv->uv0.set(a_0.x, a_0.y);	pv->uv1.set(a_1.x, a_1.y);	pv->uv2.set(a_2.x, a_2.y);	pv->uv3.set(a_3.x, a_3.y);
		pv++;
		m_Bloom_Build_VertexBuffer->Unlock();

		GRenderInterface.UpdateDescriptorHeap(m_Bloom_Shader.E[0][0]);
		m_Bloom_Shader.E[0][0].DescriptorHeap->SetUniformBuffer(XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_GlobalUniform), GUniformAllocator->GlobalUniform, 0);
		m_Bloom_Shader.E[0][0].Set(HW->Context, FVF::F_V_BUILD);
		HW->Context->SetVertexBuffer(m_Bloom_Build_VertexBuffer);
		HW->Context->Draw(6);
		HW->Context->ClearFrameBuffer();
		HW->Context->Unlock(GRenderTarget->FrameBuffer_Bloom_1);
	}
	if(0)
	{

		float	_w = RT_BLOOM_SIZE_X;
		float	_h = RT_BLOOM_SIZE_Y;
		float	ddw = (1.f / _w) * XRayRenderConsole::ps_r_bloom_kernel_b;
		float	ddh = (1.f / _h) * XRayRenderConsole::ps_r_bloom_kernel_b;
		Fvector2	p0;	p0.set(.5f / _w, .5f / _h);
		Fvector2	p1;	p1.set((_w + .5f) / _w, (_h + .5f) / _h);

		if (m_Bloom_FastFilter_VertexBuffer.empty())
		{
			m_Bloom_FastFilter_VertexBuffer = BearRenderInterface::CreateVertexBuffer();
			m_Bloom_FastFilter_VertexBuffer->Create(sizeof(FVF::V_BUILD), 6, true);
		}

		FVF::V_BUILD* pv = (FVF::V_BUILD*)m_Bloom_FastFilter_VertexBuffer->Lock();
		pv->p.set(EPS, float(_h + EPS), EPS, 1.f); pv->uv0.set(p0.x - ddw, p1.y - ddh); pv->uv1.set(p0.x + ddw, p1.y + ddh); pv->uv2.set(p0.x + ddw, p1.y - ddh); pv->uv3.set(p0.x - ddw, p1.y + ddh); pv++;
		pv->p.set(EPS, EPS, EPS, 1.f); pv->uv0.set(p0.x - ddw, p0.y - ddh); pv->uv1.set(p0.x + ddw, p0.y + ddh); pv->uv2.set(p0.x + ddw, p0.y - ddh); pv->uv3.set(p0.x - ddw, p0.y + ddh); pv++;
		pv->p.set(float(_w + EPS), float(_h + EPS), EPS, 1.f); pv->uv0.set(p1.x - ddw, p1.y - ddh); pv->uv1.set(p1.x + ddw, p1.y + ddh); pv->uv2.set(p1.x + ddw, p1.y - ddh); pv->uv3.set(p1.x - ddw, p1.y + ddh); pv++;

		pv->p.set(float(_w + EPS), EPS, EPS, 1.f); pv->uv0.set(p1.x - ddw, p0.y - ddh); pv->uv1.set(p1.x + ddw, p0.y + ddh); pv->uv2.set(p1.x + ddw, p0.y - ddh); pv->uv3.set(p1.x - ddw, p0.y + ddh); pv++;
		pv->p.set(float(_w + EPS), float(_h + EPS), EPS, 1.f); pv->uv0.set(p1.x - ddw, p1.y - ddh); pv->uv1.set(p1.x + ddw, p1.y + ddh); pv->uv2.set(p1.x + ddw, p1.y - ddh); pv->uv3.set(p1.x - ddw, p1.y + ddh); pv++;
		pv->p.set(EPS, EPS, EPS, 1.f); pv->uv0.set(p0.x - ddw, p0.y - ddh); pv->uv1.set(p0.x + ddw, p0.y + ddh); pv->uv2.set(p0.x + ddw, p0.y - ddh); pv->uv3.set(p0.x - ddw, p0.y + ddh); pv++;
		m_Bloom_FastFilter_VertexBuffer->Unlock();


		HW->Context->Lock(GRenderTarget->FrameBuffer_Bloom_2);
		HW->Context->SetFrameBuffer(GRenderTarget->FrameBuffer_Bloom_2);

		GRenderInterface.UpdateDescriptorHeap(m_Bloom_Shader.E[0][3]);
		m_Bloom_Shader.E[0][3].Set(HW->Context, FVF::F_V_BUILD);
		HW->Context->SetVertexBuffer(m_Bloom_FastFilter_VertexBuffer);
		HW->Context->Draw(6);

		HW->Context->ClearFrameBuffer();
		HW->Context->Unlock(GRenderTarget->FrameBuffer_Bloom_2);

		HW->Context->Lock(GRenderTarget->FrameBuffer_Bloom_3);
		HW->Context->SetFrameBuffer(GRenderTarget->FrameBuffer_Bloom_3);

		GRenderInterface.UpdateDescriptorHeap(m_Bloom_Shader.E[0][4]);
		m_Bloom_Shader.E[0][4].Set(HW->Context, FVF::F_V_BUILD);
		HW->Context->SetVertexBuffer(m_Bloom_FastFilter_VertexBuffer);
		HW->Context->Draw(6);

		HW->Context->ClearFrameBuffer();
		HW->Context->Unlock(GRenderTarget->FrameBuffer_Bloom_3);
	}
	else
	{

		if (m_Bloom_Filter_VertexBuffer.empty())
		{
			m_Bloom_Filter_VertexBuffer = BearRenderInterface::CreateVertexBuffer();
			m_Bloom_Filter_VertexBuffer->Create(sizeof(FVF::V_FILTER), 12, true);
		}
		// Fill vertex buffer
		FVF::V_FILTER* pv = (FVF::V_FILTER*)m_Bloom_Filter_VertexBuffer->Lock();

		{
			float		_w = RT_BLOOM_SIZE_X;
			float		_h = RT_BLOOM_SIZE_Y;
			Fvector2	two = { 2.f / _w, 2.f / _h };
			Fvector2	one = { 1.f / _w, 1.f / _h };
			Fvector2	half = { .5f / _w, .5f / _h };
			Fvector4	a_0 = { half.x,					half.y,	half.y,		half.x };	// center
			Fvector4	a_1 = { a_0.x - one.x - half.x, half.y,	half.y,		a_0.w + one.x + half.x };	// -1,+1i
			Fvector4	a_2 = { a_1.x - two.x,			half.y,	half.y,		a_1.w + two.x };	// -2,+2i
			Fvector4	a_3 = { a_2.x - two.x,			half.y,	half.y,		a_2.w + two.x };	// -3,+3i
			Fvector4	a_4 = { a_3.x - two.x,			half.y,	half.y,		a_3.w + two.x };	// -4,+4i
			Fvector4	a_5 = { a_4.x - two.x,			half.y,	half.y,		a_4.w + two.x };	// -5,+5i
			Fvector4	a_6 = { a_5.x - two.x,			half.y,	half.y,		a_5.w + two.x };	// -6,+6i
			Fvector4	a_7 = { a_6.x - two.x,			half.y,	half.y,		a_6.w + two.x };	// -7,+7i


			{
				// 0 - LB
				pv->p.set(EPS, float(_h + EPS), EPS, 1.f);
				pv->uv0.set(a_0.x, 1 + a_0.y, 0, 0);
				pv->uv1.set(a_1.x, 1 + a_1.y, 1 + a_1.z, a_1.w);
				pv->uv2.set(a_2.x, 1 + a_2.y, 1 + a_2.z, a_2.w);
				pv->uv3.set(a_3.x, 1 + a_3.y, 1 + a_3.z, a_3.w);
				pv->uv4.set(a_4.x, 1 + a_4.y, 1 + a_4.z, a_4.w);
				pv->uv5.set(a_5.x, 1 + a_5.y, 1 + a_5.z, a_5.w);
				pv->uv6.set(a_6.x, 1 + a_6.y, 1 + a_6.z, a_6.w);
				pv->uv7.set(a_7.x, 1 + a_7.y, 1 + a_7.z, a_7.w);
				pv++;

				// 1 - LT
				pv->p.set(EPS, EPS, EPS, 1.f);
				pv->uv0.set(a_0.x, a_0.y, 0, 0);
				pv->uv1.set(a_1.x, a_1.y, a_1.z, a_1.w);
				pv->uv2.set(a_2.x, a_2.y, a_2.z, a_2.w);
				pv->uv3.set(a_3.x, a_3.y, a_3.z, a_3.w);
				pv->uv4.set(a_4.x, a_4.y, a_4.z, a_4.w);
				pv->uv5.set(a_5.x, a_5.y, a_5.z, a_5.w);
				pv->uv6.set(a_6.x, a_6.y, a_6.z, a_6.w);
				pv->uv7.set(a_7.x, a_7.y, a_7.z, a_7.w);
				pv++;

				// 2 - RB
				pv->p.set(float(_w + EPS), float(_h + EPS), EPS, 1.f);
				pv->uv0.set(1 + a_0.x, 1 + a_0.y, 0, 0);
				pv->uv1.set(1 + a_1.x, 1 + a_1.y, 1 + a_1.z, 1 + a_1.w);
				pv->uv2.set(1 + a_2.x, 1 + a_2.y, 1 + a_2.z, 1 + a_2.w);
				pv->uv3.set(1 + a_3.x, 1 + a_3.y, 1 + a_3.z, 1 + a_3.w);
				pv->uv4.set(1 + a_4.x, 1 + a_4.y, 1 + a_4.z, 1 + a_4.w);
				pv->uv5.set(1 + a_5.x, 1 + a_5.y, 1 + a_5.z, 1 + a_5.w);
				pv->uv6.set(1 + a_6.x, 1 + a_6.y, 1 + a_6.z, 1 + a_6.w);
				pv->uv7.set(1 + a_7.x, 1 + a_7.y, 1 + a_7.z, 1 + a_7.w);
				pv++;

				// 3 - RT
				pv->p.set(float(_w + EPS), EPS, EPS, 1.f);
				pv->uv0.set(1 + a_0.x, a_0.y, 0, 0);
				pv->uv1.set(1 + a_1.x, a_1.y, a_1.z, 1 + a_1.w);
				pv->uv2.set(1 + a_2.x, a_2.y, a_2.z, 1 + a_2.w);
				pv->uv3.set(1 + a_3.x, a_3.y, a_3.z, 1 + a_3.w);
				pv->uv4.set(1 + a_4.x, a_4.y, a_4.z, 1 + a_4.w);
				pv->uv5.set(1 + a_5.x, a_5.y, a_5.z, 1 + a_5.w);
				pv->uv6.set(1 + a_6.x, a_6.y, a_6.z, 1 + a_6.w);
				pv->uv7.set(1 + a_7.x, a_7.y, a_7.z, 1 + a_7.w);
				pv++;

				// 2 - RB
				pv->p.set(float(_w + EPS), float(_h + EPS), EPS, 1.f);
				pv->uv0.set(1 + a_0.x, 1 + a_0.y, 0, 0);
				pv->uv1.set(1 + a_1.x, 1 + a_1.y, 1 + a_1.z, 1 + a_1.w);
				pv->uv2.set(1 + a_2.x, 1 + a_2.y, 1 + a_2.z, 1 + a_2.w);
				pv->uv3.set(1 + a_3.x, 1 + a_3.y, 1 + a_3.z, 1 + a_3.w);
				pv->uv4.set(1 + a_4.x, 1 + a_4.y, 1 + a_4.z, 1 + a_4.w);
				pv->uv5.set(1 + a_5.x, 1 + a_5.y, 1 + a_5.z, 1 + a_5.w);
				pv->uv6.set(1 + a_6.x, 1 + a_6.y, 1 + a_6.z, 1 + a_6.w);
				pv->uv7.set(1 + a_7.x, 1 + a_7.y, 1 + a_7.z, 1 + a_7.w);
				pv++;

				// 1 - LT
				pv->p.set(EPS, EPS, EPS, 1.f);
				pv->uv0.set(a_0.x, a_0.y, 0, 0);
				pv->uv1.set(a_1.x, a_1.y, a_1.z, a_1.w);
				pv->uv2.set(a_2.x, a_2.y, a_2.z, a_2.w);
				pv->uv3.set(a_3.x, a_3.y, a_3.z, a_3.w);
				pv->uv4.set(a_4.x, a_4.y, a_4.z, a_4.w);
				pv->uv5.set(a_5.x, a_5.y, a_5.z, a_5.w);
				pv->uv6.set(a_6.x, a_6.y, a_6.z, a_6.w);
				pv->uv7.set(a_7.x, a_7.y, a_7.z, a_7.w);
				pv++;
			}
		}
		{
			float		_w = RT_BLOOM_SIZE_X;
			float		_h = RT_BLOOM_SIZE_Y;
			Fvector2	two = { 2.f / _w, 2.f / _h };
			Fvector2	one = { 1.f / _w, 1.f / _h };
			Fvector2	half = { .5f / _w, .5f / _h };
			Fvector4	a_0 = { half.x,	half.y,					half.y,						half.x };	// center
			Fvector4	a_1 = { half.x, a_0.y - one.y - half.y,	half.y + one.y + a_0.z,		half.x };	// -1,+1i
			Fvector4	a_2 = { half.x,			a_1.y - two.y,	two.y + a_1.z,				half.x };	// -2,+2i
			Fvector4	a_3 = { half.x,			a_2.y - two.y,	two.y + a_2.z,				half.x };	// -3,+3i
			Fvector4	a_4 = { half.x,			a_3.y - two.y,	two.y + a_3.z,				half.x };	// -4,+4i
			Fvector4	a_5 = { half.x,			a_4.y - two.y,	two.y + a_4.z,				half.x };	// -5,+5i
			Fvector4	a_6 = { half.x,			a_5.y - two.y,	two.y + a_5.z,				half.x };	// -6,+6i
			Fvector4	a_7 = { half.x,			a_6.y - two.y,	two.y + a_6.z,				half.x };	// -7,+7i

			// Fill vertex buffer
			{

				// 0 - LB
				pv->p.set(EPS, float(_h + EPS), EPS, 1.f);
				pv->uv0.set(a_0.x, 1 + a_0.y, 0, 0);
				pv->uv1.set(a_1.x, 1 + a_1.y, 1 + a_1.z, a_1.w);
				pv->uv2.set(a_2.x, 1 + a_2.y, 1 + a_2.z, a_2.w);
				pv->uv3.set(a_3.x, 1 + a_3.y, 1 + a_3.z, a_3.w);
				pv->uv4.set(a_4.x, 1 + a_4.y, 1 + a_4.z, a_4.w);
				pv->uv5.set(a_5.x, 1 + a_5.y, 1 + a_5.z, a_5.w);
				pv->uv6.set(a_6.x, 1 + a_6.y, 1 + a_6.z, a_6.w);
				pv->uv7.set(a_7.x, 1 + a_7.y, 1 + a_7.z, a_7.w);
				pv++;

				// 1 - LT
				pv->p.set(EPS, EPS, EPS, 1.f);
				pv->uv0.set(a_0.x, a_0.y, 0, 0);
				pv->uv1.set(a_1.x, a_1.y, a_1.z, a_1.w);
				pv->uv2.set(a_2.x, a_2.y, a_2.z, a_2.w);
				pv->uv3.set(a_3.x, a_3.y, a_3.z, a_3.w);
				pv->uv4.set(a_4.x, a_4.y, a_4.z, a_4.w);
				pv->uv5.set(a_5.x, a_5.y, a_5.z, a_5.w);
				pv->uv6.set(a_6.x, a_6.y, a_6.z, a_6.w);
				pv->uv7.set(a_7.x, a_7.y, a_7.z, a_7.w);
				pv++;

				// 2 - RB
				pv->p.set(float(_w + EPS), float(_h + EPS), EPS, 1.f);
				pv->uv0.set(1 + a_0.x, 1 + a_0.y, 0, 0);
				pv->uv1.set(1 + a_1.x, 1 + a_1.y, 1 + a_1.z, 1 + a_1.w);
				pv->uv2.set(1 + a_2.x, 1 + a_2.y, 1 + a_2.z, 1 + a_2.w);
				pv->uv3.set(1 + a_3.x, 1 + a_3.y, 1 + a_3.z, 1 + a_3.w);
				pv->uv4.set(1 + a_4.x, 1 + a_4.y, 1 + a_4.z, 1 + a_4.w);
				pv->uv5.set(1 + a_5.x, 1 + a_5.y, 1 + a_5.z, 1 + a_5.w);
				pv->uv6.set(1 + a_6.x, 1 + a_6.y, 1 + a_6.z, 1 + a_6.w);
				pv->uv7.set(1 + a_7.x, 1 + a_7.y, 1 + a_7.z, 1 + a_7.w);
				pv++;

				// 3 - RT
				pv->p.set(float(_w + EPS), EPS, EPS, 1.f);
				pv->uv0.set(1 + a_0.x, a_0.y, 0, 0);
				pv->uv1.set(1 + a_1.x, a_1.y, a_1.z, 1 + a_1.w);
				pv->uv2.set(1 + a_2.x, a_2.y, a_2.z, 1 + a_2.w);
				pv->uv3.set(1 + a_3.x, a_3.y, a_3.z, 1 + a_3.w);
				pv->uv4.set(1 + a_4.x, a_4.y, a_4.z, 1 + a_4.w);
				pv->uv5.set(1 + a_5.x, a_5.y, a_5.z, 1 + a_5.w);
				pv->uv6.set(1 + a_6.x, a_6.y, a_6.z, 1 + a_6.w);
				pv->uv7.set(1 + a_7.x, a_7.y, a_7.z, 1 + a_7.w);
				pv++;

				// 2 - RB
				pv->p.set(float(_w + EPS), float(_h + EPS), EPS, 1.f);
				pv->uv0.set(1 + a_0.x, 1 + a_0.y, 0, 0);
				pv->uv1.set(1 + a_1.x, 1 + a_1.y, 1 + a_1.z, 1 + a_1.w);
				pv->uv2.set(1 + a_2.x, 1 + a_2.y, 1 + a_2.z, 1 + a_2.w);
				pv->uv3.set(1 + a_3.x, 1 + a_3.y, 1 + a_3.z, 1 + a_3.w);
				pv->uv4.set(1 + a_4.x, 1 + a_4.y, 1 + a_4.z, 1 + a_4.w);
				pv->uv5.set(1 + a_5.x, 1 + a_5.y, 1 + a_5.z, 1 + a_5.w);
				pv->uv6.set(1 + a_6.x, 1 + a_6.y, 1 + a_6.z, 1 + a_6.w);
				pv->uv7.set(1 + a_7.x, 1 + a_7.y, 1 + a_7.z, 1 + a_7.w);
				pv++;

				// 1 - LT
				pv->p.set(EPS, EPS, EPS, 1.f);
				pv->uv0.set(a_0.x, a_0.y, 0, 0);
				pv->uv1.set(a_1.x, a_1.y, a_1.z, a_1.w);
				pv->uv2.set(a_2.x, a_2.y, a_2.z, a_2.w);
				pv->uv3.set(a_3.x, a_3.y, a_3.z, a_3.w);
				pv->uv4.set(a_4.x, a_4.y, a_4.z, a_4.w);
				pv->uv5.set(a_5.x, a_5.y, a_5.z, a_5.w);
				pv->uv6.set(a_6.x, a_6.y, a_6.z, a_6.w);
				pv->uv7.set(a_7.x, a_7.y, a_7.z, a_7.w);
				pv++;
			}

		
		}
		m_Bloom_Filter_VertexBuffer->Unlock();
		// Perform filtering
		HW->Context->Lock(GRenderTarget->FrameBuffer_Bloom_2);
		HW->Context->SetFrameBuffer(GRenderTarget->FrameBuffer_Bloom_2);

		GRenderInterface.UpdateDescriptorHeap(m_Bloom_Shader.E[0][1]);
		m_Bloom_Shader.E[0][1].DescriptorHeap->SetUniformBuffer(XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_GlobalUniform), GUniformAllocator->GlobalUniform, 0);
		m_Bloom_Shader.E[0][1].Set(HW->Context, FVF::F_V_FILTER);
		HW->Context->SetVertexBuffer(m_Bloom_Filter_VertexBuffer);
		HW->Context->Draw(6);

		HW->Context->ClearFrameBuffer();
		HW->Context->Unlock(GRenderTarget->FrameBuffer_Bloom_2);

		HW->Context->Lock(GRenderTarget->FrameBuffer_Bloom_3);
		HW->Context->SetFrameBuffer(GRenderTarget->FrameBuffer_Bloom_3);

		GRenderInterface.UpdateDescriptorHeap(m_Bloom_Shader.E[0][2]);
		m_Bloom_Shader.E[0][2].DescriptorHeap->SetUniformBuffer(XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_GlobalUniform), GUniformAllocator->GlobalUniform, 0);
		m_Bloom_Shader.E[0][2].Set(HW->Context, FVF::F_V_FILTER);
		HW->Context->SetVertexBuffer(m_Bloom_Filter_VertexBuffer);
		HW->Context->Draw(6,6);

		HW->Context->ClearFrameBuffer();
		HW->Context->Unlock(GRenderTarget->FrameBuffer_Bloom_3);
	
	}
}