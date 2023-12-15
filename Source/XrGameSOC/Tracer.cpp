// exxZERO Time Stamp AddIn. Document modified at : Thursday, March 07, 2002 14:11:06 , by user : Oles , from computer : OLES
// Tracer.cpp: implementation of the CTracer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tracer.h"
#include "../XrEngine/render.h"
#include "../XrEngine/Render/UIShader.h"
#include "../XrEngine/Render/UIRender.h"   
const u32	MAX_TRACERS	= (1024*5);
const float TRACER_SIZE = 0.13f;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define TRACERS_COLOR_TABLE "tracers_color_table"
CTracer::CTracer()
{
	sh_Tracer->create("effects\\bullet_tracer","fx\\fx_tracer");
	m_aColors.clear();
	for (u8 i=0; i<255; i++)
	{
		shared_str LineName;
		LineName.printf("color_%d", i);
		if (!pSettings->line_exist(TRACERS_COLOR_TABLE, LineName)) break;
		float r, g, b;
		sscanf(pSettings->r_string(TRACERS_COLOR_TABLE, *LineName),	"%f,%f,%f", &r, &g, &b);		
		m_aColors.push_back(color_argb_f(1.0f, r, g, b));
	};
}

CTracer::~CTracer()
{
}


IC void FillSprite_Circle(const Fvector& pos, const float width, const float length, u32 color)
{
	const Fvector& T = Device->vCameraTop;
	const Fvector& R = Device->vCameraRight;
	Fvector Vr, Vt;
	Vr.x = R.x * width;
	Vr.y = R.y * width;
	Vr.z = R.z * width;
	Vt.x = T.x * length;
	Vt.y = T.y * length;
	Vt.z = T.z * length;

	Fvector         a, b, c, d;
	a.sub(Vt, Vr);
	b.add(Vt, Vr);
	c.invert(a);
	d.invert(b);

	Fbox2			t_crcl;
	t_crcl.min.set(32.0f / 64.0f, 0.0f);
	t_crcl.max.set(1.0f, 32.0f / 512.0f);

	//	TODO: return code back to indexed rendering since we use quads
	//	Tri 1
	UIRender->PushPoint(d.x + pos.x, d.y + pos.y, d.z + pos.z, color, t_crcl.min.x, t_crcl.max.y);		// 0.f,1.f);
	UIRender->PushPoint(a.x + pos.x, a.y + pos.y, a.z + pos.z, color, t_crcl.min.x, t_crcl.min.y);		// 0.f,0.f);
	UIRender->PushPoint(c.x + pos.x, c.y + pos.y, c.z + pos.z, color, t_crcl.max.x, t_crcl.max.y);		// 1.f,1.f);
	//	Tri 2																					
	UIRender->PushPoint(c.x + pos.x, c.y + pos.y, c.z + pos.z, color, t_crcl.max.x, t_crcl.max.y);	// 1.f,1.f);
	UIRender->PushPoint(a.x + pos.x, a.y + pos.y, a.z + pos.z, color, t_crcl.min.x, t_crcl.min.y);	// 0.f,0.f);
	UIRender->PushPoint(b.x + pos.x, b.y + pos.y, b.z + pos.z, color, t_crcl.max.x, t_crcl.min.y);	// 1.f,0.f);

	//pv->set         (d.x+pos.x,d.y+pos.y,d.z+pos.z, color, 0.f,1.f);        pv++;
	//pv->set         (a.x+pos.x,a.y+pos.y,a.z+pos.z, color, 0.f,0.f);        pv++;
	//pv->set         (c.x+pos.x,c.y+pos.y,c.z+pos.z, color, 1.f,1.f);        pv++;
	//pv->set         (b.x+pos.x,b.y+pos.y,b.z+pos.z, color, 1.f,0.f);        pv++;
}
IC void FillSprite_Line(const Fvector& pos, const Fvector& dir, const float width, const float length, u32 color)
{
	const Fvector& T = dir;
	Fvector				R;
	R.crossproduct(T, Device->vCameraDirection).normalize_safe();

	Fvector			Vr, Vt;
	Vr.x = R.x * width;
	Vr.y = R.y * width;
	Vr.z = R.z * width;

	Vt.x = T.x * length;
	Vt.y = T.y * length;
	Vt.z = T.z * length;

	Fvector         a, b, c, d;
	a.sub(Vt, Vr);
	b.add(Vt, Vr);
	c.invert(a);
	d.invert(b);


	Fbox2				t_tracer;
	t_tracer.min.set(0.0f, 1.0f);
	t_tracer.max.set(16.0f / 64.0f, 0.0f);

	//	TODO: return code back to indexed rendering since we use quads
	//	Tri 1
	UIRender->PushPoint(d.x + pos.x, d.y + pos.y, d.z + pos.z, color, t_tracer.min.x, t_tracer.max.y);
	UIRender->PushPoint(a.x + pos.x, a.y + pos.y, a.z + pos.z, color, t_tracer.min.x, t_tracer.min.y);
	UIRender->PushPoint(c.x + pos.x, c.y + pos.y, c.z + pos.z, color, t_tracer.max.x, t_tracer.max.y);
	//	Tri 2																					
	UIRender->PushPoint(c.x + pos.x, c.y + pos.y, c.z + pos.z, color, t_tracer.max.x, t_tracer.max.y);
	UIRender->PushPoint(a.x + pos.x, a.y + pos.y, a.z + pos.z, color, t_tracer.min.x, t_tracer.min.y);
	UIRender->PushPoint(b.x + pos.x, b.y + pos.y, b.z + pos.z, color, t_tracer.max.x, t_tracer.min.y);


	//pv->set         (d.x+pos.x,d.y+pos.y,d.z+pos.z, color, 0.f,1.f);        pv++;
	//pv->set         (a.x+pos.x,a.y+pos.y,a.z+pos.z, color, 0.f,0.5f);        pv++;
	//pv->set         (c.x+pos.x,c.y+pos.y,c.z+pos.z, color, 1.f,1.f);        pv++;
	//pv->set         (b.x+pos.x,b.y+pos.y,b.z+pos.z, color, 1.f,0.5f);        pv++;
}

void  CTracer::Render(const Fvector& pos,
	const Fvector& center,
	const Fvector& dir,
	float length,
	float width,
	u8 colorID,
	float speed,
	bool bActor)
{
	if (::Render->ViewBase.testSphere_dirty((Fvector&)center, length * .5f))
	{
		R_ASSERT(colorID < m_aColors.size());

		if (bActor)
		{
			float k_speed = speed / 1000.0f;
			//			float f_distance	= Device->vCameraPosition.distance_to(pos);

			FillSprite_Circle(pos, k_speed * width * m_circle_size_k, k_speed * width * m_circle_size_k, m_aColors[colorID]);
		}

		FillSprite_Line(center, dir, width * .5f, length * .5f, m_aColors[colorID]);
	}
}