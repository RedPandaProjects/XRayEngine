#include "stdafx.h"
#pragma hdrstop

#include "GameFont.h"
#include "Render.h"
extern ENGINE_API BOOL g_bRendering; 
ENGINE_API Fvector2		g_current_font_scale={1.0f,1.0f};

#include "../xrAPI/xrAPI.h"
#include "RenderFactory.h"
#include "FontRender.h"

CGameFont::CGameFont(LPCSTR section, u32 flags)
{
	pFontRender					= RenderFactory->CreateFontRender();
	fCurrentHeight				= 0.0f;
	uFlags						= flags;
	Initialize	(pSettings->r_string(section,"font"),pSettings->r_float(section,"size"));
}

CGameFont::CGameFont(LPCSTR font, float size, u32 flags)
{
	pFontRender					= RenderFactory->CreateFontRender();
	fCurrentHeight				= 0.0f;
	uFlags						= flags;
	Initialize					(font, size);
}

void CGameFont::Initialize		(LPCSTR font, float size)
{
	string_path					cTexture;

	LPCSTR _lang				= pSettings->r_string("string_table", "font_prefix");

	uFlags						&=~fsValid;

	eCurrentAlignment			= alLeft;

	strings.reserve				(128);
	fCurrentHeight				= 1;
	fSize = size;
	fCurrentX =0;
	fCurrentY =0;

	// Shading
	pFontRender->Initialize(font, size);
}

CGameFont::~CGameFont()
{

	// Shading
	RenderFactory->DestroyFontRender(pFontRender);
}

#define DI2PX(x) float(iFloor((x+1)*float(::Render->getTarget()->get_width())*0.5f))
#define DI2PY(y) float(iFloor((y+1)*float(::Render->getTarget()->get_height())*0.5f))

void CGameFont::OutSet			(float x, float y)
{
	fCurrentX=x;
	fCurrentY=y;
}

void CGameFont::OutSetI			(float x, float y)
{
	OutSet(DI2PX(x),DI2PY(y));
}

u32 CGameFont::smart_strlen( const char* S )
{
	return ( IsMultibyte() ? mbhMulti2Wide( NULL , NULL , 0 , S ) : xr_strlen( S ) );
}

void CGameFont::OnRender()
{
	pFontRender->OnRender(*this);
	strings.clear();
}

u16 CGameFont::GetCutLengthPos( float fTargetWidth , const char * pszText )
{
	return 0;
}

u16 CGameFont::SplitByWidth( u16 * puBuffer , u16 uBufferSize , float fTargetWidth , const char * pszText )
{
	return 0;
}

void CGameFont::MasterOut(
	BOOL bCheckDevice , BOOL bUseCoords , BOOL bScaleCoords , BOOL bUseSkip , 
	float _x , float _y , float _skip , LPCSTR fmt , va_list p )
{
	if ( bCheckDevice && ( ! Device->b_is_Active ) )
		return;
	if(fmt==nullptr)
		return;
	String rs;

	rs.x = ( bUseCoords ? ( bScaleCoords ? ( DI2PX( _x ) ) : _x ) : fCurrentX );
	rs.y = ( bUseCoords ? ( bScaleCoords ? ( DI2PY( _y ) ) : _y ) : fCurrentY );
	rs.c = dwCurrentColor;
	rs.height = fCurrentHeight;
	rs.align = eCurrentAlignment;
#ifndef	_EDITOR
	int vs_sz = vsprintf_s( rs.string , fmt , p );
#else
	int vs_sz = vsprintf( rs.string , fmt , p );
#endif
	//VERIFY( ( vs_sz != -1 ) && ( rs.string[ vs_sz ] == '\0' ) );

	rs.string[ sizeof(rs.string)-1 ] = 0;
	if ( vs_sz == -1 )
	{
		return;
	}

	if ( vs_sz )
		strings.push_back( rs );

	if ( bUseSkip )
		OutSkip( _skip );
}

#define MASTER_OUT(CHECK_DEVICE,USE_COORDS,SCALE_COORDS,USE_SKIP,X,Y,SKIP,FMT) \
	{ va_list p; va_start ( p , fmt ); \
	  MasterOut( CHECK_DEVICE , USE_COORDS , SCALE_COORDS , USE_SKIP , X , Y , SKIP , FMT, p ); \
	  va_end( p ); }

void __cdecl CGameFont::OutI( float _x , float _y , LPCSTR fmt , ... )
{
	MASTER_OUT( FALSE , TRUE , TRUE , FALSE , _x  , _y , 0.0f , fmt );
};

void __cdecl CGameFont::Out( float _x , float _y , LPCSTR fmt , ... )
{
	MASTER_OUT( TRUE , TRUE , FALSE , FALSE , _x , _y , 0.0f , fmt );
};

void __cdecl CGameFont::OutNext( LPCSTR fmt , ... )
{
	MASTER_OUT( TRUE , FALSE , FALSE , TRUE , 0.0f , 0.0f , 1.0f , fmt );
};


void CGameFont::OutSkip( float val )
{
	fCurrentY += val*CurrentHeight_();
}

float CGameFont::GetTextSize( const char cChar )
{
	return  pFontRender->GetTextSize(cChar);
}

float CGameFont::GetTextSize( LPCSTR s )
{
	if ( ! ( s && s[ 0 ] ) )
		return 0;

	if ( IsMultibyte() ) {
		wide_char wsStr[ MAX_MB_CHARS ];

		mbhMulti2Wide( wsStr , NULL , MAX_MB_CHARS , s );

		return GetTextSize( wsStr );
	}

	return pFontRender->GetTextSize(s);
}

float CGameFont::GetTextSize( const wide_char *wsStr )
{
	return pFontRender->GetTextSize(wsStr);
}

float CGameFont::CurrentHeight_	()
{
	return fCurrentHeight * fSize;
}

void CGameFont::SetHeightI(float S)
{
	VERIFY			( uFlags&fsDeviceIndependent );
	fCurrentHeight	= S*Device->dwHeight / fSize;
};

void CGameFont::SetHeight(float S)
{
	fCurrentHeight	= S/fSize*(Device->dwHeight/1024);
};
