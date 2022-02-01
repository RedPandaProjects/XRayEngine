// GlowManager.cpp: implementation of the XRayGlowManager class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "../../xrEngine/xr_object.h"
#include "../../xrEngine/IGame_Level.h"
#define FADE_SCALE_UP		4096.f
#define FADE_SCALE_DOWN		1024.f
#define MAX_GlowsDist1		float(g_pGamePersistent->Environment().CurrentEnv->far_plane)
#define MAX_GlowsDist2		float(MAX_GlowsDist1*MAX_GlowsDist1)

//////////////////////////////////////////////////////////////////////
XRayGlow::XRayGlow		()		: ISpatial(g_SpatialSpace)
{
	flags.bActive	= false;
	position.set	(0,0,0);
	direction.set	(0,0,0);
	radius			= 0.1f;
	color.set		(1,1,1,1);
	bTestResult		= FALSE;
	fade			= 1.f;
	dwFrame			= 0;
	spatial.type	= STYPE_RENDERABLE;
}
XRayGlow::~XRayGlow()
{
	set_active		(false);
}

void	XRayGlow::set_active		(bool a)				
{
	if (a)
	{
		if (flags.bActive)					return;
		flags.bActive						= true;
		spatial_register					();
	}
	else
	{
		if (!flags.bActive)					return;
		flags.bActive						= false;
		spatial_unregister					();
	}
}

bool	XRayGlow::get_active		()					{return flags.bActive;}
void	XRayGlow::set_position		(const Fvector& P)	{
	if (position.similar(P))	return;
	position.set				(P);
	spatial_move				();
};
void	XRayGlow::set_direction	(const Fvector& D)	{
	direction.normalize_safe	(D);
};
void	XRayGlow::set_radius		(float R)			{
	if (fsimilar(radius,R))		return;
	radius						= R;
	spatial_move				();
};
void	XRayGlow::set_texture		(LPCSTR name)		{
	GResourcesManager->CompileBlender			(shader,"effects\\glow",name);
}
void	XRayGlow::set_color		(const Fcolor& C)	{
	color						= C;
}
void	XRayGlow::set_color		(float r, float g, float b)	{
	color.set					(r,g,b,1);
}
void	XRayGlow::spatial_move		()
{
	spatial.sphere.set			(position,radius);
	ISpatial::spatial_move		();
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

XRayGlowManager::XRayGlowManager()
{
}

XRayGlowManager::~XRayGlowManager	()
{
}

void XRayGlowManager::Load		(IReader* fs)
{
	size_t size	= fs->length();
	R_ASSERT	(size);
	size_t one		= 4*sizeof(float)+1*sizeof(u16);
	R_ASSERT	(size%one == 0);
	size_t count	= size/one;
	Glows.reserve(count);

	for (;count;count--)
	{
		XRayGlow* G			= xr_new<XRayGlow>();
		fs->r				(&G->position,	3*sizeof(float));
		fs->r				(&G->radius,	1*sizeof(float));
		G->spatial.sphere.set(G->position, G->radius);
		G->direction.set	( 0,0,0 );

		u16 S				= fs->r_u16();
		G->shader			= ::GRenderInterface.GetShader(S);

		G->fade				= 255.f;
		G->dwFrame			= 0x0;
		G->bTestResult		= TRUE;

		G->spatial.type		= STYPE_RENDERABLE;

		G->set_active		(true);

		Glows.push_back		(G);
	}
	dwTestID		= 0;

}

void XRayGlowManager::Unload	()
{
	m_VertexBuffer.clear();
	Selected.clear			();
	Glows.clear				();
}


void XRayGlowManager::Add	(ref_glow G_)
{
	XRayGlow*	G		= (XRayGlow*)	G_._get		();
	if (G->dwFrame	==Device->dwFrame)		return;
	G->dwFrame		= Device->dwFrame;


	float	dt		= Device->fTimeDelta;
	float	dlim2 = MAX_GlowsDist2;

	float	range = Device->vCameraPosition.distance_to_sqr	(G->spatial.sphere.P);
	if (range < dlim2) 
	{
		// 2. Use result of test
		if (G->bTestResult) {
			G->fade -= dt*FADE_SCALE_DOWN;
			if (G->fade<1.)		G->fade = 1;
		} else {
			G->fade	+= dt*FADE_SCALE_UP;
			if (G->fade>255.f)	G->fade = 255.f;
		}

		Selected.push_back(G);
		return;
	}
	G->fade -= dt*FADE_SCALE_DOWN;
	if (G->fade<1.) G->fade = 1;

}

IC void FillSprite(FVF::LIT*& pv, const Fvector& pos, float r, u32 clr)
{
	const Fvector& T = Device->vCameraTop;
	const Fvector& R = Device->vCameraRight;
	Fvector		Vr, Vt;
	Vr.mul(R, r);
	Vt.mul(T, r);

	Fvector 	a, b, c, d;
	a.sub(Vt, Vr);
	b.add(Vt, Vr);
	c.invert(a);
	d.invert(b);
	pv->set(d.x + pos.x, d.y + pos.y, d.z + pos.z, clr, 0.f, 1.f);	pv++;
	pv->set(a.x + pos.x, a.y + pos.y, a.z + pos.z, clr, 0.f, 0.f);	pv++;
	pv->set(c.x + pos.x, c.y + pos.y, c.z + pos.z, clr, 1.f, 1.f);	pv++;

	pv->set(b.x + pos.x, b.y + pos.y, b.z + pos.z, clr, 1.f, 0.f);	pv++;
	pv->set(c.x + pos.x, c.y + pos.y, c.z + pos.z, clr, 1.f, 1.f);	pv++;
	pv->set(a.x + pos.x, a.y + pos.y, a.z + pos.z, clr, 0.f, 0.f);	pv++;
}

void XRayGlowManager::Render			()
{
	if (Selected.empty())					return		;

	CObject* o_main = g_pGameLevel->CurrentViewEntity();

	// 1. Test some number of glows
	Fvector start = Device->vCameraPosition;
	for (int i = 0; i < XRayRenderConsole::ps_r_GlowsPerFrame; i++, dwTestID++)
	{
		u32	ID = dwTestID % Selected.size();
		XRayGlow& G = *((XRayGlow*)Selected[ID]._get());
		if (G.dwFrame == 'test')	break;
		G.dwFrame = 'test';
		Fvector		dir;
		dir.sub(G.spatial.sphere.P, start); float range = dir.magnitude();
		if (range >EPS_S) {
			dir.div(range);
			G.bTestResult = g_pGameLevel->ObjectSpace.RayTest(start, dir, range, collide::rqtBoth, &G.RayCache, o_main);
		}
	}

	RenderSelected();

}


void XRayGlowManager::RenderSelected()
{



	Fplane			NP;
	NP.build(Device->vCameraPosition, Device->vCameraDirection);

	if (m_VertexBuffer.empty())
	{
		m_VertexBuffer = BearRenderInterface::CreateVertexBuffer();
		m_VertexBuffer->Create(sizeof(FVF::LIT), bear_recommended_size(Selected.size()) * 6, true);
	}
	else if (m_VertexBuffer->GetCount() / 6 < Selected.size())
	{
		m_VertexBuffer->Create(sizeof(FVF::LIT), bear_recommended_size(Selected.size()) * 6, true);
	}
	FVF::LIT* pv = (FVF::LIT*)m_VertexBuffer->Lock();
	float		dlim2 = MAX_GlowsDist2;


	for (size_t i = 0; i < Selected.size(); i++)
	{

		// Cull invisible 
		XRayGlow& G = *((XRayGlow*)Selected[i]._get());
		if (G.fade <= 1.f)			continue;

		// Now perform dotproduct if need it
		float	scale = 1.f, dist_sq;
		Fvector	dir;
		dir.sub(Device->vCameraPosition, G.position);
		dist_sq = dir.square_magnitude();
		if (G.direction.square_magnitude() > EPS) {
			dir.div(sqrt(dist_sq));
			scale = dir.dotproduct(G.direction);
		}
		if (G.fade * scale <= 1.f)		continue;

		// near fade
		float dist_np = NP.distance(G.position) - VIEWPORT_NEAR;
		float snear = dist_np / 0.15f;	clamp(snear, 0.f, 1.f);
		scale *= snear;
		if (G.fade * scale <= 1.f)		continue;

		u32 C = iFloor(G.fade * scale * (1 - (dist_sq / dlim2)));
		u32 clr = color_rgba(C, C, C, C);
		Fvector	gp;
		gp.mad(G.position, dir, G.radius * scale);
		FillSprite(pv, G.position, G.radius, clr);
		m_RenderGlows.emplace_back(&G);


	}
	m_VertexBuffer->Unlock();

	for (size_t i = 0; i < m_RenderGlows.size(); i++)
	{
		auto&SE =  m_RenderGlows[i]->shader.E[0][0];
		GRenderInterface.UpdateDescriptorHeap(SE);
		SE.DescriptorHeap->SetUniformBuffer(XRayUniformAllocator::GetRegister(XRayUniformAllocator::UT_Transformation), GUniformAllocator->Uniform[XRayUniformAllocator::UT_Transformation], XRayUniformAllocator::RT_Fidentity);
		if (SE.Set(HW->Context, FVF::F_LIT))
		{
			if (SE.StencilRef >= 0)
				HW->Context->SetStencilRef(SE.StencilRef);
			HW->Context->Draw(6, i * 6);
		}
	}


	Selected.erase(Selected.begin(), Selected.end());
	m_RenderGlows.erase(m_RenderGlows.begin(), m_RenderGlows.end());
}
