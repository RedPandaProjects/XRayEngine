#include "pch.h"
#include "../../xrEngine/xrTheora_Surface.h"
inline void fix_texture_thm_name(LPSTR fn)
{
	LPSTR _ext = strext(fn);
	if (_ext &&
		(0 == stricmp(_ext, ".tga") ||
			0 == stricmp(_ext, ".thm") ||
			0 == stricmp(_ext, ".dds") ||
			0 == stricmp(_ext, ".bmp") ||
			0 == stricmp(_ext, ".ogm")))
		*_ext = 0;
}

XRayTexture::XRayTexture(shared_str texture)
{
	pTheora = 0;
	Name = texture;
	BearImage image;
	if (strstr(*texture, "$rt\\"))
	{
		m_type = TT_RT;
		return;
	}
	if (strstr(*texture, "$user\\"))
	{
		m_type = TT_User;
		return;
	}
	string_path			fn;
	if (FS.exist(fn, "$game_textures$", *texture, ".ogm") || FS.exist(fn, "$game_textures$", *texture, ".ogv"))
	{
		m_type = TT_Movie;
		pTheora = xr_new<CTheoraSurface>();
		m_play_time = 0xFFFFFFFF;

		if (!pTheora->Load(fn))
		{
			xr_delete(pTheora);
			FATAL("Can't open video stream");
		}
		else
		{
			BOOL bstop_at_end = (0 != strstr(texture.c_str(), "intro\\")) || (0 != strstr(texture.c_str(), "outro\\"));
			pTheora->Play(!bstop_at_end, RDEVICE.dwTimeContinual);

			// Now create texture
			u32 _w = pTheora->Width(false);
			u32 _h = pTheora->Height(false);

			Texture2D = BearRenderInterface::CreateTexture2D(_w, _h, 1, 1, BearTexturePixelFormat::R8G8B8A8, BearTextureUsage::Dynamic);;
			Texture = Texture2D;
			m_size.set(static_cast<float>(_w), static_cast<float>(_h));
		}

	}
	else if (FS.exist(fn, "$game_textures$", *texture, ".seq"))
	{
		m_type = TT_Seq;

		string256 buffer;
		IReader* _fs = FS.r_open(fn);

		m_seq_cycles = false;
		_fs->r_string(buffer, sizeof(buffer));
		if (0 ==_stricmp(buffer, "cycled"))
		{
			m_seq_cycles = true;
			_fs->r_string(buffer, sizeof(buffer));
		}
		u32 fps = atoi(buffer);
		m_seq_ms = 1000 / fps;

		while (!_fs->eof())
		{
			_fs->r_string(buffer, sizeof(buffer));
			_Trim(buffer);
			if (buffer[0])
			{
				fix_texture_thm_name(buffer);
				if (FS.exist(fn, "$game_textures$", buffer, ".dds"))
				{
					IReader* fs = FS.r_open(fn);
					BearMemoryStream stream_image(fs->pointer(), fs->length());
					R_ASSERT(image.LoadFromStream(stream_image));
					Texture2D = BearRenderInterface::CreateTexture2D(image.GetSize().x, image.GetSize().y, image.GetMips(), image.GetDepth(), image.GetFormat(), BearTextureUsage::Static, *image);
					SeqTextures.push_back(Texture2D);
					FS.r_close(fs);
				}

			}
		}
		R_ASSERT(SeqTextures.size());
		Texture = SeqTextures[0];
		m_size.set(static_cast<float>(image.GetSize().x), static_cast<float>(image.GetSize().y));
		FS.r_close(_fs);
	}
	else if (FS.exist(fn, "$game_textures$", *texture, ".dds"))
	{
		IReader* _fs = FS.r_open(fn);
		BearMemoryStream stream_image(_fs->pointer(),_fs->length());
		R_ASSERT(image.LoadFromStream(stream_image));
		if (image.IsCubeMap())
		{
			TextureCube = BearRenderInterface::CreateTextureCube(image.GetSize().x, image.GetSize().y, image.GetMips(), image.GetDepth(), image.GetFormat(), BearTextureUsage::Static, *image);
			Texture = TextureCube;
			m_type = TT_Cube;
		}
		else
		{
			Texture2D = BearRenderInterface::CreateTexture2D(image.GetSize().x, image.GetSize().y, image.GetMips(), image.GetDepth(), image.GetFormat(), BearTextureUsage::Static, *image);
			Texture = Texture2D;
			m_type = TT_Default;
		}

		
		m_size.set(static_cast<float>(image.GetSize().x), static_cast<float>(image.GetSize().y));
		FS.r_close(_fs);
	}
	/*else if (FS.exist(fn, "$textures$", *texture, ".dds"))
	{
		IReader* _fs = FS.r_open(fn);
		BearMemoryStream stream_image(_fs->pointer(), _fs->length());
		R_ASSERT(image.LoadFromStream(stream_image));
		Texture2D = BearRenderInterface::CreateTexture2D(image.GetSize().x, image.GetSize().y, image.GetMips(), image.GetDepth(), image.GetFormat(), BearTextureUsage::Static, *image);
		m_type = TT_Default;
		Texture = Texture2D;
		m_size.set(image.GetSize().x, image.GetSize().y);
		FS.r_close(_fs);
	}*/
	else if (FS.exist(fn, "$level$", *texture, ".dds"))
	{
		IReader* _fs = FS.r_open(fn);
		BearMemoryStream stream_image(_fs->pointer(), _fs->length());
		R_ASSERT(image.LoadFromStream(stream_image));
		Texture2D = BearRenderInterface::CreateTexture2D(image.GetSize().x, image.GetSize().y, image.GetMips(), image.GetDepth(), image.GetFormat(), BearTextureUsage::Static, *image);
		m_type = TT_Default;
		Texture = Texture2D;
		m_size.set(static_cast<float>(image.GetSize().x), static_cast<float>(image.GetSize().y));
		FS.r_close(_fs);
	}
	else
	{
		Msg("! Can't load:%s", texture.c_str());
		R_ASSERT(FS.exist(fn, "$game_textures$", "ed\\ed_not_existing_texture", ".dds"));
		IReader* _fs = FS.r_open(fn);
		BearMemoryStream stream_image(_fs->pointer(), _fs->length());
		R_ASSERT(image.LoadFromStream(stream_image));
		Texture2D = BearRenderInterface::CreateTexture2D(image.GetSize().x, image.GetSize().y, image.GetMips(), image.GetDepth(), image.GetFormat(), BearTextureUsage::Static, *image);
		m_size.set(static_cast<float>(image.GetSize().x), static_cast<float>(image.GetSize().y));
		Texture = Texture2D;
		FS.r_close(_fs);
	}
}

XRayTexture::~XRayTexture()
{
	if (m_type == TT_Movie)
	{
		xr_delete(pTheora);

	}

}

bool XRayTexture::Update()
{
	switch (m_type)
	{
	case XRayTexture::TT_RT:
		return true;
	case XRayTexture::TT_Seq:
	{
		size_t	frame = RDEVICE.dwTimeContinual / m_seq_ms;
		u32	frame_data = SeqTextures.size();
		if (m_seq_cycles) {
			u32	frame_id = frame % (frame_data * 2);
			if (frame_id >= frame_data)	frame_id = (frame_data - 1) - (frame_id % frame_data);
			if (frame_id != SeqCurrent)
			{
				SeqCurrent = frame_id;
				Texture = SeqTextures[frame_id];
				return true;
			}
		}
		else {
			u32	frame_id = frame % frame_data;
			if (frame_id != SeqCurrent)
			{
				SeqCurrent = frame_id;
				Texture = SeqTextures[frame_id];
				return true;
			}

		}
	}
	break;
	case XRayTexture::TT_Movie:
	{
		if (pTheora->Update(m_play_time != 0xFFFFFFFF ? m_play_time : RDEVICE.dwTimeContinual))
		{

			int _pos = 0;
			pTheora->DecompressFrame((u32*)Texture2D->Lock(), pTheora->Width(false)- pTheora->Width(true), _pos);
			Texture2D->Unlock();
			return true;
		}
	}
		break;
	default:
		break;
	}
	return false;
}

void XRayTexture::video_Play(BOOL looped, u32 _time)
{
	if (pTheora) pTheora->Play(looped, (_time != 0xFFFFFFFF) ? (m_play_time = _time) : Device.dwTimeContinual);
}

void XRayTexture::video_Pause(BOOL state)
{
	if (pTheora) pTheora->Pause(state);
}

void XRayTexture::video_Stop()
{
	if (pTheora) pTheora->Stop();
}

BOOL XRayTexture::video_IsPlaying()
{
	if (pTheora)return  pTheora->IsPlaying(); return 0;
}
