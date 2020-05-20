#pragma once
class XRayRdsgraphStructure
{
public:
	XRayRdsgraphStructure();
	~XRayRdsgraphStructure();
public:
	void set_Transform(Fmatrix* M);
	void set_HUD(BOOL V);
	void set_Invisible(BOOL V);

	BOOL add_Dynamic(XRayRenderVisual* pVisual, u32 planes);	
	void add_Static(XRayRenderVisual* pVisual);
	void add_leafs_Dynamic(XRayRenderVisual* pVisual);
	void add_leafs_Static(XRayRenderVisual* pVisual);

	void Flush();

	inline void PushView(CFrustum* view)
	{
		m_Views.push_back(view);
		m_MasksView.push_back(view->getMask());
	}
	inline void ClearView()
	{
		m_Views.erase(m_Views.begin(), m_Views.end());
		m_MasksView.erase(m_MasksView.begin(), m_MasksView.end());
	}

	using RenderList = XrFastForwardList<XRayObjectRender, XRayRenderFastAlloca>;
	using RenderSort = XrFastMap<float, XRayObjectRender, XRayRenderFastAlloca>;
	RenderList RenderHUDObjects;
	RenderList RenderObjects;

	struct LodItem
	{
		XRayFLOD* lod;
		float ssa;
	};
	XrFastForwardList<LodItem, XRayRenderFastAlloca> RenderLods;
	RenderSort RenderSortObjects;
	enum  Ephase
	{
		PHASE_NORMAL,
		PHASE_POINT,
		PHASE_SPOT
	};
	Ephase Phase;
private:
	bool m_bInvisible;
	bool m_bHUD;
	Fmatrix m_LTransform;
	xr_vector<XRayRenderVisual*>m_dbg_cheak_visuals;
	xr_vector<CFrustum*> m_Views;
	xr_vector<u32> m_MasksView;
private:
	void                            InsertStaticObjectForRender(XRayRenderVisual* pVisual);
	bool                            InsertDynamicObjectForRender(XRayRenderVisual* pVisual);
};