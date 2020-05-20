#pragma once
class CTheoraSurface;
class XRayTexture:public XRayResourceNamed
{
public:
	XRayTexture(shared_str texture);
	~XRayTexture();

	bool Update();
	enum TextureType
	{
		TT_Default,
		TT_Cube,
		TT_Seq,
		TT_Movie,
		TT_RT,
		TT_User,
	};
	IC TextureType GetType() const { return m_type; }
	IC Fvector2 GetSize()const { return m_size; }

	BearFactoryPointer<BearRHI::BearRHIShaderResource> Texture;

	inline void								video_Sync(u32 _time) { m_play_time = _time; }
	void								video_Play(BOOL looped, u32 _time = 0xFFFFFFFF);
	void								video_Pause(BOOL state);
	void								video_Stop();
	BOOL								video_IsPlaying();
private:

	BearFactoryPointer<BearRHI::BearRHITexture2D> Texture2D;
	BearFactoryPointer<BearRHI::BearRHITextureCube> TextureCube;
	xr_vector< BearFactoryPointer<BearRHI::BearRHITexture2D>> SeqTextures;
	size_t SeqCurrent;
	Fvector2 m_size;

	TextureType m_type;

	
	size_t m_seq_ms;

	bool m_seq_cycles;
private:
	CTheoraSurface* pTheora;
	u32 m_play_time;
};