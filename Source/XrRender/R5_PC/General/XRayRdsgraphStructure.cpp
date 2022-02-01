#include "pch.h"
constexpr float	ps_r__ssaDISCARD = 3.5f;					//RO
constexpr float	ps_r1_ssaLOD_A = 64.f;
constexpr float	ps_r1_ssaLOD_B = 48.f;
constexpr float ps_r__LOD = 0.75f;
extern float r_ssaDISCARD;
extern float r_ssaLOD_A, r_ssaLOD_B;
inline float sqr(float a) { return a * a; }

constexpr float O_S_L1_S_ULT = 80.f;
constexpr float O_S_L1_D_ULT = 40.f;
constexpr float O_S_L2_S_ULT = 600.f;
constexpr float O_S_L2_D_ULT = 100.f;
constexpr float O_S_L3_S_ULT = 2500.f;
constexpr float O_S_L3_D_ULT = 120.f;
constexpr float O_S_L4_S_ULT = 5000.f;
constexpr float O_S_L4_D_ULT = 140.f;
constexpr float O_S_L5_S_ULT = 20000.f;
constexpr float O_S_L5_D_ULT = 200.f;


constexpr float O_S_L1_S_MED = 60.f;
constexpr float O_S_L1_D_MED = 45.f;
constexpr float O_S_L2_S_MED = 400.f;
constexpr float O_S_L2_D_MED = 100.f;
constexpr float O_S_L3_S_MED = 2000.f;
constexpr float O_S_L3_D_MED = 160.f;
constexpr float O_S_L4_S_MED = 3800.f;
constexpr float O_S_L4_D_MED = 200.f;
constexpr float O_S_L5_S_MED = 13000.f;
constexpr float O_S_L5_D_MED = 300.f;
u32	ps_r_smapsize = 2048;
float		r_ssaGLOD_start, r_ssaGLOD_end;
ICF float CalcSSA(float& distSQ, Fvector& C, XRayRenderVisual* V)
{
	float R = V->Vis.sphere.R + 0;
	distSQ = Device->vCameraPosition.distance_to_sqr(C) + EPS;
	return R / distSQ;
}
ICF float CalcSSA(float& distSQ, Fvector& C, float R)
{
	distSQ = Device->vCameraPosition.distance_to_sqr(C) + EPS;
	return R / distSQ;
}
ICF float calcLOD(float ssa, float R)
{
	return			_sqrt(clampr((ssa - r_ssaGLOD_end) / (r_ssaGLOD_start - r_ssaGLOD_end), 0.f, 1.f));
}

// Cut off Static geometry depending of size of geometry and distance to camera and current geometry optimization settings
IC bool IsValuableToRender(XRayRenderVisual* pVisual, bool sm)
{
	auto GetDistFromCamera = [](const Fvector& from_position)
	{
		float distance = Device->vCameraPosition.distance_to(from_position);
		float fov_K = 70.f / Device->fFOV;
		float adjusted_distane = distance / fov_K;

		return adjusted_distane;
	};

	if (sm && ps_r_smapsize < 4096)
	{
		float sphere_volume = pVisual->getVisData().sphere.volume();

		float adjusted_distane = GetDistFromCamera(pVisual->Vis.sphere.P);

		if (sm && ps_r_smapsize < 4096) // Highest cut off for shadow map
		{
			if (sphere_volume < 50000.f && adjusted_distane > 160) // don't need geometry behind the farest sun shadow cascade
				return false;

			if ((sphere_volume < O_S_L1_S_ULT) && adjusted_distane > O_S_L1_D_ULT)
				return false;
			else if ((sphere_volume < O_S_L2_S_ULT) && adjusted_distane > O_S_L2_D_ULT)
				return false;
			else if ((sphere_volume < O_S_L3_S_ULT) && adjusted_distane > O_S_L3_D_ULT)
				return false;
			else if ((sphere_volume < O_S_L4_S_ULT) && adjusted_distane > O_S_L4_D_ULT)
				return false;
			else if ((sphere_volume < O_S_L5_S_ULT) && adjusted_distane > O_S_L5_D_ULT)
				return false;
			else
				return true;
		}
		else
		{
			if ((sphere_volume < O_S_L1_S_MED) && adjusted_distane > O_S_L1_D_MED)
				return false;
			else if ((sphere_volume < O_S_L2_S_MED) && adjusted_distane > O_S_L2_D_MED)
				return false;
			else if ((sphere_volume < O_S_L3_S_MED) && adjusted_distane > O_S_L3_D_MED)
				return false;
			else if ((sphere_volume < O_S_L4_S_MED) && adjusted_distane > O_S_L4_D_MED)
				return false;
			else if ((sphere_volume < O_S_L5_S_MED) && adjusted_distane > O_S_L5_D_MED)
				return false;
			else
				return true;
		}
	}

	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
constexpr float O_D_L1_S_ULT = 2.0f;
constexpr float O_D_L1_D_ULT = 30.f;
constexpr float O_D_L2_S_ULT = 8.f;
constexpr float O_D_L2_D_ULT = 50.f;
constexpr float O_D_L3_S_ULT = 4000.f;
constexpr float O_D_L3_D_ULT = 110.f;

constexpr float O_D_L1_S_MED = 1.4f;
constexpr float O_D_L1_D_MED = 30.f;
constexpr float O_D_L2_S_MED = 4.f;
constexpr float O_D_L2_D_MED = 80.f;
constexpr float O_D_L3_S_MED = 4000.f;
constexpr float O_D_L3_D_MED = 150.f;

// Cut off Dynamic geometry depending of size of geometryand distance to cameraand current geometry optimization settings
IC bool IsValuableToRenderDyn(XRayRenderVisual* pVisual, Fmatrix& transform_matrix, bool sm)
{
	auto GetDistFromCamera = [](const Fvector& from_position)
	{
		float distance = Device->vCameraPosition.distance_to(from_position);
		float fov_K = 70.f / Device->fFOV;
		float adjusted_distane = distance / fov_K;

		return adjusted_distane;
	};

	if (sm && ps_r_smapsize < 4096)
	{
		float sphere_volume = pVisual->getVisData().sphere.volume();

		Fvector    Tpos;
		transform_matrix.transform_tiny(Tpos, pVisual->Vis.sphere.P);

		float adjusted_distane = GetDistFromCamera(Tpos);

		if (sm && ps_r_smapsize < 4096) // Highest cut off for shadow map
		{
			if (adjusted_distane > 160) // don't need geometry behind the farest sun shadow cascade
				return false;

			if ((sphere_volume < O_D_L1_S_ULT) && adjusted_distane > O_D_L1_D_ULT)
				return false;
			else if ((sphere_volume < O_D_L2_S_ULT) && adjusted_distane > O_D_L2_D_ULT)
				return false;
			else if ((sphere_volume < O_D_L3_S_ULT) && adjusted_distane > O_D_L3_D_ULT)
				return false;
			else
				return true;
		}
		else
		{
			if ((sphere_volume < O_D_L1_S_MED) && adjusted_distane > O_D_L1_D_MED)
				return false;
			else if ((sphere_volume < O_D_L2_S_MED) && adjusted_distane > O_D_L2_D_MED)
				return false;
			else if ((sphere_volume < O_D_L3_S_MED) && adjusted_distane > O_D_L3_D_MED)
				return false;
			else
				return true;
		}
	}

	return true;
}


XRayRdsgraphStructure::XRayRdsgraphStructure()
{
	m_bHUD = false;
	m_bInvisible = false;
}

XRayRdsgraphStructure::~XRayRdsgraphStructure()
{
}

void XRayRdsgraphStructure::set_Transform(Fmatrix* M)
{
	m_LTransform = *M;
}

void XRayRdsgraphStructure::set_HUD(BOOL V)
{
	m_bHUD = V;
}

void XRayRdsgraphStructure::set_Invisible(BOOL V)
{
	m_bInvisible = V;
}


BOOL XRayRdsgraphStructure::add_Dynamic(XRayRenderVisual* pVisual, u32 planes)
{
	if (!IsValuableToRenderDyn(pVisual, m_LTransform, false))
		return FALSE;

	// Check frustum visibility and calculate distance to visual's center
	Fvector		Tpos;	// transformed position
	EFC_Visible	VIS;

	m_LTransform.transform_tiny(Tpos, pVisual->Vis.sphere.P);
	VIS = m_Views[0]->testSphere(Tpos, pVisual->Vis.sphere.R, planes);
	if (fcvNone == VIS) return false;

	// If we get here visual is visible or partially visible
	xr_vector<XRayRenderVisual*>::iterator I, E;	// it may be usefull for 'hierrarhy' visuals

	switch (pVisual->Type)
	{
	case MT_PARTICLE_GROUP:
	{
		/*if (phase == PHASE_SMAP) return true;
		// Add all children, doesn't perform any tests
		PS::CParticleGroup* pG = (PS::CParticleGroup*)pVisual;
		for (PS::CParticleGroup::SItem& refI : pG->items)
		{
			if (fcvPartial == VIS)
			{
				if (refI._effect)		add_Dynamic(refI._effect, planes);
				for (xr_vector<dxRender_Visual*>::iterator pit = refI._children_related.begin(); pit != refI._children_related.end(); pit++)	add_Dynamic(*pit, planes);
				for (xr_vector<dxRender_Visual*>::iterator pit = refI._children_free.begin(); pit != refI._children_free.end(); pit++)	add_Dynamic(*pit, planes);
			}
			else
			{
				if (refI._effect)		add_leafs_Dynamic(refI._effect);
				for (xr_vector<dxRender_Visual*>::iterator pit = refI._children_related.begin(); pit != refI._children_related.end(); pit++)	add_leafs_Dynamic(*pit);
				for (xr_vector<dxRender_Visual*>::iterator pit = refI._children_free.begin(); pit != refI._children_free.end(); pit++)	add_leafs_Dynamic(*pit);
			}
		}*/
	}
	break;
	case MT_HIERRARHY:
	{
		// Add all children
		XRayFHierrarhyVisual* pV = (XRayFHierrarhyVisual*)pVisual;
		I = pV->children.begin();
		E = pV->children.end();
		if (fcvPartial == VIS)
		{
			for (; I != E; I++)	add_Dynamic(*I, planes);
		}
		else {
			for (; I != E; I++)	add_leafs_Dynamic(*I);
		}
	}
	break;
	case MT_SKELETON_ANIM:
	case MT_SKELETON_RIGID:
	{
		// Add all children, doesn't perform any tests
		XRayKinematics* pV = (XRayKinematics*)pVisual;
		BOOL	_use_lod = false;
		if (pV->m_lod)
		{
			Fvector TPos;
			float D;
			m_LTransform.transform_tiny(TPos, pV->Vis.sphere.P);

			// assume dynamics never consume full sphere
			float ssa = CalcSSA(D, TPos, pV->Vis.sphere.R / 2.f);
			if (ssa < r_ssaLOD_A)	_use_lod = true;
		}
		if (_use_lod)
		{
			add_leafs_Dynamic(pV->m_lod);
		}
		else
		{

			pV->CalculateBones(true);

			I = pV->children.begin();
			E = pV->children.end();
			for (; I != E; I++)	add_leafs_Dynamic(*I);
		}
	}
	break;
	default:
	{
		// General type of visual
		return InsertDynamicObjectForRender(pVisual);
	}
	break;
	}
	return true;
}

void XRayRdsgraphStructure::add_Static(XRayRenderVisual* pVisual)
{
	if (!IsValuableToRender(pVisual, false))
		return;

	// Check frustum visibility and calculate distance to visual's center

	vis_data& vis = pVisual->Vis;
	bool bVIS = true;
	bool bPartial = false;
	{
		auto bm = m_MasksView.begin();
		for (auto b = m_Views.begin(), e = m_Views.end(); b != e; b++)
		{
			EFC_Visible	VIS;
			u32 mask = *bm;
			VIS = (*b)->testSAABB(vis.sphere.P, vis.sphere.R, vis.box.data(), mask);
			bm++;
			bVIS = VIS == fcvNone && bVIS;
			bPartial = VIS == fcvPartial || bPartial;
		}
	}
	if (bVIS)return;
	if (!GRenderInterface.HOM.visible(vis))
	{
		return;
	}

	// If we get here visual is visible or partially visible
	switch (pVisual->Type)
	{
	case MT_PARTICLE_GROUP:
	{

	}
	break;
	case MT_HIERRARHY:
	{
		// Add all children
		XRayFHierrarhyVisual* pV = (XRayFHierrarhyVisual*)pVisual;
		if (bPartial)
		{
			for (XRayRenderVisual* childRenderable : pV->children)
			{
				add_Static(childRenderable);
			}
		}
		else
		{
			for (XRayRenderVisual* childRenderable : pV->children)
			{
				add_leafs_Static(childRenderable);
			}
		}
	}
	break;
	case MT_SKELETON_ANIM:
	case MT_SKELETON_RIGID:
	{
		// Add all children, doesn't perform any tests
		XRayKinematics* pV = (XRayKinematics*)pVisual;

		pV->CalculateBones(true);

		if (bPartial)
		{
			for (XRayRenderVisual* childRenderable : pV->children)
			{
				add_Static(childRenderable);
			}
		}
		else
		{
			for (XRayRenderVisual* childRenderable : pV->children)
			{
				add_leafs_Static(childRenderable);
			}
		}
	}
	break;
	case MT_LOD:
	{
		XRayFLOD* pV = (XRayFLOD*)pVisual;
		float		D;
		float		ssa = CalcSSA(D, pV->Vis.sphere.P, pV);
		ssa *= pV->lod_factor;
		if (ssa < r_ssaLOD_A)
		{
			if (ssa < r_ssaDISCARD)	return;

			auto& item = RenderLods.push_back();
			item.lod = pV;
			item.ssa = ssa;
			//mapLOD.emplace_back(std::make_pair(D, _LodItem({ ssa, pVisual })));
		}

		if (ssa > r_ssaLOD_B/* || phase == PHASE_SMAP*/)
		{
			// Add all children, perform tests
			for (XRayRenderVisual* childRenderable : pV->children)
			{
				add_leafs_Static(childRenderable);
			}
		}
	}
	break;
	default:
	{
		InsertStaticObjectForRender(pVisual);
		// General type of visual
	//	r_dsgraph_insert_static(pVisual);

	}
	break;
	}

}
void XRayRdsgraphStructure::add_leafs_Dynamic(XRayRenderVisual* pVisual)
{
	if (pVisual == nullptr) return;

	// Visual is 100% visible - simply add it
	/*if (!bIgnoreOpt && !IsValuableToRenderDyn(pVisual, m_LTransform, false))
		return;*/

	switch (pVisual->Type)
	{
	case MT_PARTICLE_GROUP:
	{
		/*if (phase == PHASE_SMAP) return;
		// Add all children, doesn't perform any tests
		PS::CParticleGroup* pG = (PS::CParticleGroup*)pVisual;
		for (PS::CParticleGroup::SItem& I : pG->items)
		{
			if (I._effect)
				add_leafs_Dynamic(I._effect, bIgnoreOpt);

			for (dxRender_Visual* pChildRelated : I._children_related)
				add_leafs_Dynamic(pChildRelated, bIgnoreOpt);

			for (dxRender_Visual* pChildFree : I._children_free)
				add_leafs_Dynamic(pChildFree, bIgnoreOpt);
		}*/
	}
	return;
	case MT_HIERRARHY:
	{
		// Add all children, doesn't perform any tests
		XRayFHierrarhyVisual* pV = (XRayFHierrarhyVisual*)pVisual;

		for (XRayRenderVisual* pChildVisual : pV->children)
		{
			add_leafs_Dynamic(pChildVisual);
		}
	}
	return;
	case MT_SKELETON_ANIM:
	case MT_SKELETON_RIGID:
	{
		// Add all children, doesn't perform any tests
		XRayKinematics* pV = (XRayKinematics*)pVisual;
		bool	_use_lod = false;
		if (pV->m_lod)
		{
			Fvector							Tpos;	float		D;
			m_LTransform.transform_tiny(Tpos, pV->Vis.sphere.P);
			float		ssa = CalcSSA(D, Tpos, pV->Vis.sphere.R / 2.f);	// assume dynamics never consume full sphere
			if (ssa < r_ssaLOD_A)	_use_lod = true;
		}

		if (_use_lod)
		{
			add_leafs_Dynamic(pV->m_lod);
		}
		else
		{
			pV->CalculateBones(true);

			for (XRayRenderVisual* pChildVisual : pV->children)
			{
				add_leafs_Dynamic(pChildVisual);
			}
		}
	}
	return;
	default:
	{
		// General type of visual
		// Calculate distance to it's center
		InsertDynamicObjectForRender(pVisual);
	}
	return;
	}
}

void XRayRdsgraphStructure::add_leafs_Static(XRayRenderVisual* pVisual)
{
	if (!IsValuableToRender(pVisual, false))
		return;

	if (!GRenderInterface.HOM.visible(pVisual->Vis))		return;

	// Visual is 100% visible - simply add it
	xr_vector<XRayRenderVisual*>::iterator I, E;	// it may be usefull for 'hierrarhy' visuals

	switch (pVisual->Type)
	{
	case MT_PARTICLE_GROUP:
	{
		/*if (phase == PHASE_SMAP) return;
		// Add all children, doesn't perform any tests
		PS::CParticleGroup* pG = (PS::CParticleGroup*)pVisual;
		for (PS::CParticleGroup::SItem& refI : pG->items)
		{
			if (refI._effect)		add_leafs_Dynamic(refI._effect);
			for (xr_vector<dxRender_Visual*>::iterator pit = refI._children_related.begin(); pit != refI._children_related.end(); pit++)	add_leafs_Dynamic(*pit);
			for (xr_vector<dxRender_Visual*>::iterator pit = refI._children_free.begin(); pit != refI._children_free.end(); pit++)	add_leafs_Dynamic(*pit);
		}*/
	}
	return;
	case MT_HIERRARHY:
	{
		// Add all children, doesn't perform any tests
		XRayFHierrarhyVisual* pV = (XRayFHierrarhyVisual*)pVisual;
		I = pV->children.begin();
		E = pV->children.end();

		for (; I != E; I++)
		{
			
			add_leafs_Static(*I);
		}
	}
	return;
	case MT_SKELETON_ANIM:
	case MT_SKELETON_RIGID:
	{
		// Add all children, doesn't perform any tests
		XRayKinematics* pV = (XRayKinematics*)pVisual;

		pV->CalculateBones(true);

		I = pV->children.begin();
		E = pV->children.end();

		for (; I != E; I++)
		{
			
			add_leafs_Static(*I);
		}
	}
	return;
	case MT_LOD:
	{
		XRayFLOD* pV = (XRayFLOD*)pVisual;
		float		D;
		float		ssa = CalcSSA(D, pV->Vis.sphere.P, pV);
		ssa *= pV->lod_factor;
		if (ssa < r_ssaLOD_A)
		{
			if (ssa < r_ssaDISCARD)	return;

			auto& item = RenderLods.push_back();
			item.lod = pV;
			item.ssa = ssa;
			//mapLOD.emplace_back(std::make_pair(D, _LodItem({ ssa, pVisual })));
		}

		if (ssa > r_ssaLOD_B/* || phase == PHASE_SMAP*/)
		{
			// Add all children, perform tests
			for (XRayRenderVisual* childRenderable : pV->children)
			{
				add_leafs_Static(childRenderable);
			}
		}
	}
	return;

	default:
	{
		InsertStaticObjectForRender(pVisual);
	}
	return;
	}
}

void XRayRdsgraphStructure::Flush()
{
	m_bHUD = false;
	m_bInvisible = false;
	RenderHUDObjects.clear();
	RenderObjects.clear();
	RenderSortObjects.clear();
	RenderLods.clear();
	m_dbg_cheak_visuals.erase(m_dbg_cheak_visuals.begin(), m_dbg_cheak_visuals.end());
}

void XRayRdsgraphStructure::InsertStaticObjectForRender(XRayRenderVisual* pVisual)
{
	float distSQ;

	float SSA = CalcSSA(distSQ, pVisual->Vis.sphere.P, pVisual);
	if (SSA <= r_ssaDISCARD)		return;
	if (XRayRenderConsole::ps_r_testFlags.test(XRayRenderConsole::RTF_CheckDuplicateObject))
	{
		auto item = std::find_if(m_dbg_cheak_visuals.begin(), m_dbg_cheak_visuals.end(), [&pVisual](XRayRenderVisual* right) {return right == pVisual; });
		R_ASSERT(item == m_dbg_cheak_visuals.end());
		m_dbg_cheak_visuals.push_back(pVisual);
	}
	EShaderElement ESType;

	switch (Phase)
	{
	case PHASE_NORMAL:
		ESType = ((std::sqrt(distSQ) - pVisual->Vis.sphere.R) < 44) ? EShaderElement::SE_NORMAL_HQ : EShaderElement::SE_NORMAL_LQ;
		break;
	case PHASE_POINT:
		break;
	case PHASE_SPOT:
		break;
	default:
		break;
	}
	XRayObjectRender Item;
	if (pVisual->Render(calcLOD(SSA, pVisual->Vis.sphere.R), ESType, Item))
	{
		Item.UseMWorld = false;

		//auto item = std::lower_bound(m_RenderObjects->begin(), m_RenderObjects->end(), distSQ, [](const std::pair<float, XRayObjectRender>& left, float right) {return left.first > right; });

		if (!Item.Sort)
		{

			RenderObjects.push_back(Item);
		}
		else
		{

			RenderSortObjects.insertInAnyWay(distSQ)->val.Swap(Item);
		}
	}
}

bool XRayRdsgraphStructure::InsertDynamicObjectForRender(XRayRenderVisual* pVisual)
{
	if (m_bInvisible)return false;
	if (m_bHUD)
	{
		if (XRayRenderConsole::ps_r_testFlags.test(XRayRenderConsole::RTF_CheckDuplicateObject))
		{
			auto item = std::find_if(m_dbg_cheak_visuals.begin(), m_dbg_cheak_visuals.end(), [&pVisual](XRayRenderVisual* right) {return right == pVisual; });
			R_ASSERT(item == m_dbg_cheak_visuals.end());
			m_dbg_cheak_visuals.push_back(pVisual);
		}
		XRayObjectRender Item;
		if (pVisual->Render(1.f, SE_NORMAL_HQ, Item))
		{
			Item.UseMWorld = true;
			Item.MWorld = m_LTransform;
			RenderHUDObjects.push_back(Item);
		}

	}
	else
	{
		//if (m_RenderObjects->back().Visual == pVisual)return true;
		if (XRayRenderConsole::ps_r_testFlags.test(XRayRenderConsole::RTF_CheckDuplicateObject))
		{
			auto item = std::find_if(m_dbg_cheak_visuals.begin(), m_dbg_cheak_visuals.end(), [&pVisual](XRayRenderVisual* right) {return right == pVisual; });
			R_ASSERT(item == m_dbg_cheak_visuals.end());
			m_dbg_cheak_visuals.push_back(pVisual);
		}
		Fvector							Tpos;
		m_LTransform.transform_tiny(Tpos, pVisual->Vis.sphere.P);
		float distSQ;
		float SSA = CalcSSA(distSQ, Tpos, pVisual);
		if (SSA <= r_ssaDISCARD)		return true;
		XRayObjectRender Item;
		if (pVisual->Render(calcLOD(SSA, pVisual->Vis.sphere.R), SE_NORMAL_HQ, Item))
		{
			Item.UseMWorld = true;
			Item.MWorld = m_LTransform;
			//auto item = std::lower_bound(m_RenderObjects->begin(), m_RenderObjects->end(), distSQ, [](const std::pair<float, XRayObjectRender>& left, float right) {return left.first > right; });
			if (!Item.Sort)
			{

				RenderObjects.push_back(Item);
			}
			else
			{

				RenderSortObjects.insertInAnyWay(distSQ)->val.Swap(Item);
			}
		}
	}
	return true;
}
