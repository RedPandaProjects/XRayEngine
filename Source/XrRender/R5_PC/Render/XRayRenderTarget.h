#pragma once
#include "XRayRTList.h"

class XRayRenderTarget :public IRender_Target
{
public:
	XRayRenderTarget();
	~XRayRenderTarget();
	void Flush();
	void Render();
	void CompileShader();

	void LoadLevel();
	void UnloadLevel();

	virtual void set_blur(float f);
	virtual void set_gray(float f);
	virtual void set_duality_h(float f);
	virtual void set_duality_v(float f);
	virtual void set_noise(float f);
	virtual void set_noise_scale(float f);
	virtual void set_noise_fps(float f);
	virtual void set_color_base(u32 f);
	virtual void set_color_gray(u32 f);
	//virtual void set_color_add (u32 f) = 0;
	virtual void set_color_add(const Fvector& f);
	virtual u32 get_width();
	virtual u32 get_height();;
	virtual void set_cm_imfluence(float f);
	virtual void set_cm_interpolate(float f);
	virtual void set_cm_textures(const shared_str& tex0, const shared_str& tex1);
	inline bool IsRenderHUD() { return m_bRenderHUD; }
	float BloomFactor;
private:
	void RenderLOD();
	void RenderHUD();
	void RenderCombine1();
	void RenderCombine2();
	void RenderBloom();
	bool m_bRenderHUD;
public:
	/*static const size_t FrameCount = 2;
	size_t FrameCurrent;*/

	XRayTexture* RTVBasicColor;
	XRayTexture* RTVCompute;
	XRayTexture* DSVBasic;

	XRayTexture* RTVGeneric1;
	XRayTexture* RTVGeneric2;

	XRayTexture* RTVBloom;
	XRayTexture* RTVBloom1;
	XRayTexture* RTVBloom2;
	XRayTexture* RTVBloom3;

	BearFactoryPointer<BearRHI::BearRHIRenderPass>	RenderPass_Base;
	BearFactoryPointer<BearRHI::BearRHIFrameBuffer>	FrameBuffer_Base;

	BearFactoryPointer<BearRHI::BearRHIRenderPass>	RenderPass_Generic;
	BearFactoryPointer<BearRHI::BearRHIFrameBuffer>	FrameBuffer_Generic1;
	BearFactoryPointer<BearRHI::BearRHIFrameBuffer>	FrameBuffer_Generic2;

	BearFactoryPointer<BearRHI::BearRHIRenderPass>	RenderPass_Bloom;
	BearFactoryPointer<BearRHI::BearRHIFrameBuffer>	FrameBuffer_Bloom_1;
	BearFactoryPointer<BearRHI::BearRHIFrameBuffer>	FrameBuffer_Bloom_2;
	BearFactoryPointer<BearRHI::BearRHIFrameBuffer>	FrameBuffer_Bloom_3;
private:
	XRayShader m_Combine_Shader;
	BearFactoryPointer<BearRHI::BearRHIVertexBuffer> m_ScreenSquare_VertexBuffer;


	XRayShader m_LOD_Shader;
	BearFactoryPointer<BearRHI::BearRHIVertexBuffer> m_LOD_VertexBuffer;

	XRayShader m_Bloom_Shader;
	BearFactoryPointer<BearRHI::BearRHIVertexBuffer> m_Bloom_Build_VertexBuffer;
	BearFactoryPointer<BearRHI::BearRHIVertexBuffer> m_Bloom_FastFilter_VertexBuffer;
	BearFactoryPointer<BearRHI::BearRHIVertexBuffer> m_Bloom_Filter_VertexBuffer;
};
extern XRayRenderTarget* GRenderTarget;