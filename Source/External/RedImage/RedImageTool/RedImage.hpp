#pragma once
#include <iostream>
#include <assert.h>
namespace RedImageTool
{
	enum class RedResizeFilter
	{
		Default,  // use same filter type that easy-to-use API chooses
		Box,  // A trapezoid w/1-pixel wide ramps, same result as box for integer scale ratios
		Triangle,  // On upsampling, produces same results as bilinear texture filtering
		Cubicbspline,  // The cubic b-spline (aka Mitchell-Netrevalli with B=1,C=0), gaussian-esque
		Catmullrom,  // An interpolating cubic spline
		Mitchell,  // Mitchell-Netrevalli filter with B=1/3, C=1/3
	};

	enum class RedTexturePixelFormat
	{
		None = 0,
		R8 = 1,
		R8G8,
		R8G8B8,
		R8G8B8A8,
		R32F,
		R32G32F,
		R32G32B32F,
		R32G32B32A32F,
		DXT_1,//not Alpha
		DXT_1_Alpha,//Alpha  1 bit
		DXT_3,//alpga 
		DXT_5,//Alpha
		BC1 = DXT_1,
		BC1a = DXT_1_Alpha,
		BC2 = DXT_3,
		BC3 = DXT_5,
		BC4,//R 32 bit 16 pxiel
		BC5,//RB 64 bit 16 pxiel
		BC6,//RGBA FP16 64 bit 16 pxiel
		BC7//RGBA 64 bit 16 pxiel
	};
	using u8 = unsigned char;
	using u16 = unsigned short;
	using u32 = unsigned int;
};

#include "RedMemery.h"
#include "RedColor.h"
#include "RedTextureUtils.h"
#include "RedImage.h"