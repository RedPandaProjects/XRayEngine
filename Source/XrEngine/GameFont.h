#ifndef GameFontH
#define GameFontH
#pragma once

#include "MbHelpers.h"

#include "../XrRender/Public/FontRender.h"

class ENGINE_API CGameFont
{
	friend class dxFontRender;
	friend class XRayFontRender;
public:
	enum EAligment{
		alLeft				= 0,
		alRight,
		alCenter
	};
private:
	struct String
	{
		string1024	string;
		float		x,y;
		float		height;
		u32			c;
		EAligment	align;
	};
protected:
	EAligment				eCurrentAlignment;
	u32						dwCurrentColor;
	float					fCurrentHeight;
	float					fCurrentX, fCurrentY;
	float					fSize;

	xr_vector<String>		strings;

	IFontRender				*pFontRender;

	u32						uFlags;

public:
	enum
	{
		fsGradient			= (1<<0),
		fsDeviceIndependent	= (1<<1),
		fsValid 			= (1<<2),

		fsMultibyte			= (1<<3),

		fsForceDWORD		= u32(-1)
	};

public:
							CGameFont		(LPCSTR section, u32 flags=0);
							CGameFont		(LPCSTR font, float size, u32 flags);
							~CGameFont		();

	void					Initialize		(LPCSTR font, float size);

	IC void					SetColor		(u32 C)		{dwCurrentColor=C;};

	IC void					SetHeightI		(float S);
	IC void					SetHeight		(float S);

	IC float				GetHeight		(){return fCurrentHeight;};
	IC void					SetAligment		(EAligment aligment){ eCurrentAlignment=aligment; }

	float					GetTextSize			( LPCSTR s );
	float					GetTextSize			( const wide_char *wsStr );

	float					GetTextSize			( const char cChar );  // only ANSII 

	float					CurrentHeight_	();

	void					OutSetI			(float x, float y);
	void					OutSet			(float x, float y);

	void 					MasterOut( 	BOOL bCheckDevice , BOOL bUseCoords , BOOL bScaleCoords , BOOL bUseSkip ,
										float _x , float _y , float _skip , LPCSTR fmt , va_list p );

	u32						smart_strlen( const char* S );
	BOOL					IsMultibyte() { return ( uFlags & fsMultibyte ); };
	u16						SplitByWidth( u16 * puBuffer , u16 uBufferSize , float fTargetWidth , const char * pszText );
	u16						GetCutLengthPos( float fTargetWidth , const char * pszText );

	void  					OutI			( float _x , float _y , LPCSTR fmt , ... );
	void  					Out				( float _x , float _y , LPCSTR fmt , ... );
	void             		OutNext			( LPCSTR fmt , ... );

	void					OutSkip			(float val=1.f);

	void					OnRender		();

	IC	void				Clear			()  { strings.clear(); };

#ifdef DEBUG
	shared_str				m_font_name;
#endif
};

#endif // _XR_GAMEFONT_H_
