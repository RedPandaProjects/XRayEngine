#pragma once
class XRayShaderElement;
struct D3DVERTEXELEMEN_D3D9;
class ISpatial;
class XRayRenderInterface :public IRender_interface,public pureFrame
{
public:
	XRayRenderInterface();

	virtual bool is_sun_static();
	virtual DWORD get_dx_level();

	virtual void create();
	virtual void destroy();
	virtual void reset_begin();
	virtual void reset_end();

	virtual void level_Load(IReader*);
	virtual void level_Unload();

	void shader_option_skinning(s32 mode) { m_skinning = mode; }
	virtual HRESULT shader_compile(LPCSTR name,	DWORD const* pSrcData,	UINT SrcDataLen,	LPCSTR pFunctionName,LPCSTR pTarget,	DWORD Flags,	void*& result);

	virtual void Statistics(CGameFont* F) {};
	virtual LPCSTR getShaderPath();
	virtual IRender_Sector* getSector(int id);
	virtual IRenderVisual* getVisual(int id);
	virtual IRender_Sector* detectSector(const Fvector& P);
	virtual IRender_Target* getTarget();

	// Main
	IC void set_Frustum(CFrustum* O) { VERIFY(O); View = O; }
	virtual void set_Transform(Fmatrix* M);
	virtual void set_HUD(BOOL V);
	virtual BOOL get_HUD();
	virtual void set_Invisible(BOOL V);
	virtual void set_Object(IRenderable* O);
	virtual	GenerationLevel			get_generation() { return GenerationLevel::GENERATION_R1; }

	virtual void add_Occluder(Fbox2& bb_screenspace); 
	virtual void add_Visual(IRenderVisual* V); 
	virtual void add_Geometry(IRenderVisual* V);
	virtual void add_StaticWallmark(const wm_shader& S, const Fvector& P, float s, CDB::TRI* T, Fvector* V);
	virtual void add_StaticWallmark(IWallMarkArray* pArray, const Fvector& P, float s, CDB::TRI* T, Fvector* V);
	virtual void add_SkeletonWallmark(const Fmatrix* xf, IKinematics* obj, IWallMarkArray* pArray, const Fvector& start, const Fvector& dir, float size);

	virtual void clear_static_wallmarks();

	virtual void flush();

	virtual IRender_ObjectSpecific* ros_create(IRenderable* parent);
	virtual void ros_destroy(IRender_ObjectSpecific*&);

	// Lighting/glowing
	virtual IRender_Light* light_create();
	virtual IRender_Glow* glow_create();

	// Models
	virtual IRenderVisual* model_CreateParticles(LPCSTR name);
	virtual IRenderVisual* model_Create(LPCSTR name, IReader* data = 0);
	virtual IRenderVisual* model_CreateChild(LPCSTR name, IReader* data);

	virtual IRenderVisual* model_Duplicate(IRenderVisual* V);

	virtual void model_Delete(IRenderVisual*& V, BOOL bDiscard = FALSE);
	virtual void model_Logging(BOOL bEnable);
	virtual void models_Prefetch();
	virtual void models_Clear(BOOL b_complete);

	virtual BOOL occ_visible(vis_data& V);
	virtual BOOL occ_visible(Fbox& B);
	virtual BOOL occ_visible(sPoly& P);

	virtual void Screenshot(ScreenshotMode mode = SM_NORMAL, LPCSTR name = 0);
	virtual void Screenshot(ScreenshotMode mode, CMemoryWriter& memory_writer);
	virtual void ScreenshotAsyncBegin();
	virtual void ScreenshotAsyncEnd(CMemoryWriter& memory_writer);

	virtual void rmNear();
	virtual void rmFar();
	virtual void rmNormal();
	virtual u32 memory_usage();

	virtual void                    BeforeWorldRender() ; // Перед рендерингом мира
	virtual void                    AfterWorldRender() ; // После рендеринга мира (до UI)

	virtual void					ChangeMark(LPCSTR mark) ; // Каждый кадр проверяем не поменялась ли текстура
	virtual u32                     active_phase();


	void SetView(const Fmatrix& mat);
	void SetProject(const Fmatrix& mat);
	void SetWorld(const Fmatrix& mat);
	IC Fmatrix& GetWorld() { return m_MWorld; }

	void Flush();
	virtual void Render();
	virtual void OnFrame();
	virtual void Calculate();

	virtual void UniformCounter(size_t i, XRayObjectRender* obj);
	virtual void UniformUpdate(size_t i, XRayObjectRender* obj);
	virtual void UniformSet(size_t i, XRayObjectRender* obj);
	virtual void UniformsCounter();
	virtual void UniformsUpdate();
	virtual void UniformsSet();

	virtual void ScreenshotImpl(ScreenshotMode mode, LPCSTR name, CMemoryWriter* memory_writer);
	int TranslateSector(IRender_Sector* pSector);
	IRender_Sector* detectSector(const Fvector& P, Fvector& D);

	u32 ConvertFVF(D3DVERTEXELEMEN_D3D9*);
	size_t GetSize(D3DVERTEXELEMEN_D3D9*);
	void UpdateDescriptorHeap(XRayShaderElement& ShaderElement, bool update_textures = false);
	IC XRayShader& GetShader(size_t id) { return m_Shader[id]; }
	IC BearFactoryPointer<BearRHI::BearRHIIndexBuffer>& GetIndexBuffer(size_t id)   { return m_IndexBuffers[id]; }
	IC BearFactoryPointer<BearRHI::BearRHIVertexBuffer>& GetVertexBuffer(size_t id) { return m_VertexBuffer[id]; }
	IC u32* GetIndexBufferForMesh(size_t id) { return *m_IndexBuffersForMesh[id]; }
	IC u8* GetVertexBufferForMesh(size_t id) { return *m_VertexBufferForMesh[id]; }
	IC u32 GetVertexState(size_t id) { return m_VertexState[id]; }
	IC XRayRenderVisual* GetVisual(size_t id) { return m_Visuals[id]; }
	IC IRender_Portal* GetPortal(size_t id) { return m_Portals[id]; }
	IC FSlideWindowItem* GetSWI(size_t id) { return &m_SWIs[id]; }

private:
		void								LoadBuffers(CStreamReader* fs);
		void								LoadVisuals(IReader* fs);
		void								LoadSectors(IReader* fs);
		void								LoadSWIs(CStreamReader* base_fs);
public:

	CHOM HOM;
	xrXRC Sectors_xrc;
	XRayRdsgraphStructure Rdsgraph_Genderal;
private:
	CSector* pLastSector;
	CDB::MODEL* rmPortals;
	BearFactoryPointer<BearRHI::BearRHIUniformBuffer> m_BloomScreenTransformation;
	BearFactoryPointer<BearRHI::BearRHIUniformBuffer> m_ScreenTransformation;
	BearFactoryPointer<BearRHI::BearRHIUniformBuffer> m_MatrixBuffer;

	bool m_bHUD;

	xr_vector< BearFactoryPointer<BearRHI::BearRHIIndexBuffer>> m_IndexBuffers;
	xr_vector< BearFactoryPointer<BearRHI::BearRHIVertexBuffer>> m_VertexBuffer;

	std::vector<BearRef<u32>> m_IndexBuffersForMesh;
	std::vector<BearRef<u8>> m_VertexBufferForMesh;

	xr_vector< XRayRenderVisual*> m_Visuals;

	xr_vector< u32> m_VertexState;

	xr_vector< XRayShader> m_Shader;

	xr_vector<IRender_Portal*> m_Portals;

	xr_vector<IRender_Sector*> m_Sectors;

	xr_vector<ISpatial*> lstRenderables;

	u32 uLastLTRACK;

	Fmatrix m_MView, m_MProject, m_MWorld, m_LTransform;
#pragma pack(push,4)
	struct ContatMatrix
	{
		Fmatrix View;
		Fmatrix Project;
		Fmatrix World;
		Fmatrix VPW;
		Fmatrix VP;
		Fmatrix WV;
	};
#pragma pack(pop)

	Fvector														vLastCameraPos;
	xr_vector<FSlideWindowItem>									m_SWIs;

	size_t m_UniformCounter;
	u8* m_UniformPtr;
	
};
 extern XRayRenderInterface GRenderInterface;