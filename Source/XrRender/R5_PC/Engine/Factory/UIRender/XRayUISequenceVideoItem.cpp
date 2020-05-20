#include "pch.h"

XRayUISequenceVideoItem::XRayUISequenceVideoItem()
{
	Texture = 0;
}

void XRayUISequenceVideoItem::Copy(IUISequenceVideoItem & _in)
{
	Texture = static_cast<XRayUISequenceVideoItem&>(_in).Texture;
}

bool XRayUISequenceVideoItem::HasTexture()
{
	return Texture;
}
extern XRayTexture* GUITexture ;
void XRayUISequenceVideoItem::CaptureTexture()
{
	Texture = GUITexture;
}

void XRayUISequenceVideoItem::ResetTexture()
{
	Texture = 0;
}

BOOL XRayUISequenceVideoItem::video_IsPlaying()
{
	if (Texture)
		return Texture->video_IsPlaying();
	return 0;
}

void XRayUISequenceVideoItem::video_Sync(u32 _time)
{
	if (Texture)
		return Texture->video_Sync(_time);
}

void XRayUISequenceVideoItem::video_Play(BOOL looped, u32 _time)
{
	if (Texture)
		return Texture->video_Play(looped, _time);
}

void XRayUISequenceVideoItem::video_Stop()
{
	if (Texture)
		return Texture->video_Stop();
}
