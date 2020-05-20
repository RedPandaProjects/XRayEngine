#include "pch.h"
#include "../../xrEngine/irenderable.h"
#include "../../xrEngine/xr_object.h"
#include "../../xrEngine/CustomHUD.h"
inline  bool			pred_sp_sort(ISpatial* _1, ISpatial* _2)
{
	float	d1 = _1->spatial.sphere.P.distance_to_sqr(Device.vCameraPosition);
	float	d2 = _2->spatial.sphere.P.distance_to_sqr(Device.vCameraPosition);
	return	d1 < d2;
}


float r_ssaDISCARD;
float r_ssaLOD_A, r_ssaLOD_B;
extern float		r_ssaGLOD_start, r_ssaGLOD_end;

inline float sqr(float a) { return a * a; }

void XRayRenderInterface::Calculate()
{
	//ScopeStatTimer scopeTimer(Device.Statistic->TEST3);
	float	fov_factor = sqr(90.f / Device.fFOV);
	float  g_fSCREEN = float(GRenderTarget->get_width() * GRenderTarget->get_height()) * fov_factor * (EPS_S + XRayRenderConsole::ps_r_LOD);
	r_ssaDISCARD = sqr(XRayRenderConsole::ps_r_ssaDISCARD) / g_fSCREEN;
	r_ssaLOD_A = sqr(XRayRenderConsole::ps_r_ssaLOD_A / 3) / g_fSCREEN;
	r_ssaLOD_B = sqr(XRayRenderConsole::ps_r_ssaLOD_B / 3) / g_fSCREEN;
	r_ssaGLOD_start = _sqr(XRayRenderConsole::ps_r_GLOD_ssa_start / 3) / g_fSCREEN;
	r_ssaGLOD_end = _sqr(XRayRenderConsole::ps_r_GLOD_ssa_end / 3) / g_fSCREEN;
	ViewBase.CreateFromMatrix(Device.mFullTransform, FRUSTUM_P_LRTB | FRUSTUM_P_FAR);
	View = 0;

	Rdsgraph_Genderal.Phase = XRayRdsgraphStructure::PHASE_NORMAL;

	// Detect camera-sector
	if (!vLastCameraPos.similar(Device.vCameraPosition, EPS_S))
	{
		CSector* pSector = (CSector*)detectSector(Device.vCameraPosition);
		if (pSector && (pSector != pLastSector))
			g_pGamePersistent->OnSectorChanged(TranslateSector(pSector));

		if (0 == pSector) pSector = pLastSector;
		pLastSector = pSector;
		vLastCameraPos.set(Device.vCameraPosition);
	}

	// Check if camera is too near to some portal - if so force DualRender
	if (rmPortals)
	{
		Fvector box_radius;		box_radius.set(EPS_L * 2, EPS_L * 2, EPS_L * 2);
		Sectors_xrc.box_options(CDB::OPT_FULL_TEST);
		Sectors_xrc.box_query(rmPortals, Device.vCameraPosition, box_radius);
		for (int K = 0; K < Sectors_xrc.r_count(); K++)
		{
			CPortal* pPortal = (CPortal*)m_Portals[rmPortals->get_tris()[Sectors_xrc.r_begin()[K].id].dummy];
			pPortal->bDualRender = TRUE;
		}
	}
	//

	if (pLastSector)
	{
		// Traverse sector/portal structure
		GPortalTraverser.traverse
		(
			pLastSector,
			ViewBase,
			Device.vCameraPosition,
			Device.mFullTransform,
			CPortalTraverser::VQ_HOM + CPortalTraverser::VQ_SSA + CPortalTraverser::VQ_FADE
		);

		// Determine visibility for static geometry hierrarhy
		if (psDeviceFlags.test(rsDrawStatic))
		{
			for (u32 s_it = 0; s_it < GPortalTraverser.r_sectors.size(); s_it++)
			{
				CSector* sector = (CSector*)GPortalTraverser.r_sectors[s_it];
				XRayRenderVisual* root = sector->root();
				Rdsgraph_Genderal.ClearView();
				for (u32 v_it = 0; v_it < sector->r_frustums.size(); v_it++)
				{
					Rdsgraph_Genderal.PushView(&(sector->r_frustums[v_it]));
				}
				add_Geometry(root);
			}
		}
		if (psDeviceFlags.test(rsDrawDynamic))
		{
			g_SpatialSpace->q_frustum
			(
				lstRenderables,
				ISpatial_DB::O_ORDERED,
				STYPE_RENDERABLE + STYPE_LIGHTSOURCE,
				ViewBase
			);

			// Exact sorting order (front-to-back)
			std::sort(lstRenderables.begin(), lstRenderables.end(), pred_sp_sort);

			// Determine visibility for dynamic part of scene
			set_Object(0);
			g_hud->Render_First();	// R1 shadows
			g_hud->Render_Last();
			u32 uID_LTRACK = 0xffffffff;
			/*if (phase == PHASE_NORMAL)*/
		/*	{
				uLastLTRACK++;
				if (lstRenderables.size())		uID_LTRACK = uLastLTRACK % lstRenderables.size();

				// update light-vis for current entity / actor
				CObject* O = g_pGameLevel->CurrentViewEntity();
				if (O) {
					CROS_impl* R = (CROS_impl*)O->ROS();
					if (R)		R->update(O);
				}
			}*/
			for (ISpatial* pSpatial : lstRenderables)
			{
				pSpatial->spatial_updatesector();
				CSector* pSector = (CSector*)pSpatial->spatial.sector;
				if (!pSector)
					continue; // Disassociated from S/P structure



				if (GPortalTraverser.i_marker != pSector->r_marker)
					continue; // Inactive (untouched) sector

				for (CFrustum& view : pSector->r_frustums)
				{
					if (!view.testSphere_dirty(pSpatial->spatial.sphere.P, pSpatial->spatial.sphere.R))
						continue;

					if (pSpatial->spatial.type & STYPE_RENDERABLE)
					{
						IRenderable* renderable = pSpatial->dcast_Renderable();
						bool bSphere = view.testSphere_dirty(pSpatial->spatial.sphere.P, pSpatial->spatial.sphere.R);

						if (!renderable)
						{
							/*if (ps_r_flags.is(R_FLAG_GLOW_USE))
								continue;

							CGlow* pGlow = dynamic_cast<CGlow*>(pSpatial);
							VERIFY2(pGlow, "Glow don't created!");

							if (pGlow)
							{
								if (bSphere)
									Glows->add(pGlow);
								else
									pGlow->hide_glow();
							}*/
						}
						else if (bSphere)
						{
							// Occlusion
							// Casting is faster then using getVis method
							vis_data& v_orig = ((XRayRenderVisual*)renderable->renderable.visual)->Vis;
							vis_data v_copy = v_orig;
							v_copy.box.xform(renderable->renderable.xform);
							bool bVisible = HOM.visible(v_copy);
							v_orig.marker = v_copy.marker;
							v_orig.accept_frame = v_copy.accept_frame;
							v_orig.hom_frame = v_copy.hom_frame;
							v_orig.hom_tested = v_copy.hom_tested;

							// Exit loop on frustums
							if (!bVisible)
								break;

							// Rendering
							set_Object(renderable);
							renderable->renderable_Render();
							set_Object(nullptr);
						}
					}
					break; // Exit loop on frustums
				}
			}
		}
	}
	else
	{

	}

	
	
	UniformsCounter();
	UniformsUpdate();
	UniformsSet();
	/*{
		xr_map< BearFactoryPointer<BearRHI::BearRHIDescriptorHeap>,bool> test;
		for (auto b = m_RenderObjects->begin(), e = m_RenderObjects->end(); b != e; b++)
		{
			auto item = test.find(b->DescriptorHeap);
			R_ASSERT(item == test.end());
			test.insert(std::make_pair(b->DescriptorHeap, true));
		}
		for (auto b = m_RenderHUDObjects->begin(), e = m_RenderHUDObjects->end(); b != e; b++)
		{
			auto item = test.find(b->DescriptorHeap);
			R_ASSERT(item == test.end());
			test.insert(std::make_pair(b->DescriptorHeap, true));
		}
	}
	*/
}
