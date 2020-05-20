#include "../../xrEngine/stdafx.h"

#include "../../xrEngine/Render.h"
#include "../../xrEngine/SkeletonMotions.h"
#include "../../xrEngine/SkeletonMotionDefs.h"

#include "../Public/ApplicationRender.h"
#include "../Public/RenderFactory.h"
#include "../Public/UISequenceVideoItem.h"
#include "../Public/ConsoleRender.h"
#include "../Public/StatGraphRender.h"
#include "../Public/EnvironmentRender.h"
#include "../Public/LensFlareRender.h"
#include "../Public/RainRender.h"
#include "../Public/ThunderboltDescRender.h"
#include "../Public/ThunderboltRender.h"
#include  "../Public/RenderDeviceRender.h"
#include "../Public/StatsRender.h"
#include "../Public/WallMarkArray.h"
#include "../Public/ObjectSpaceRender.h"
#include "../Public/DrawUtils.h"
#include "../Public/FontRender.h"
#include "../Public/UIShader.h"
#include "../Public/UIRender.h"
#include "../Public/DebugRender.h"
#include "../../xrEngine/Render.h"
#include "../../BearBundle/BearGraphics/BearGraphics.hpp"
#include "../Public/DebugRender.h"
#include "../Public/KinematicsAnimated.h"

#include "General/XRayRenderFastAllocator.h"
#include "General/XrFastMap.h"
#include "General/XrFastForwardList.h"
#define REGISTER(name,byte,size,a1,a2) name=byte,
enum D3DVertexState
{
#include "D3D9VertexState.h"
};
#undef REGISTER

#ifdef D3DFVF_TEXTUREFORMAT1
#undef D3DFVF_TEXTUREFORMAT1
#endif
#ifdef D3DFVF_TEXTUREFORMAT2
#undef D3DFVF_TEXTUREFORMAT2
#endif
#ifdef D3DFVF_TEXTUREFORMAT3
#undef D3DFVF_TEXTUREFORMAT3
#endif
#ifdef D3DFVF_TEXTUREFORMAT4
#undef D3DFVF_TEXTUREFORMAT4
#endif
#define D3DFVF_TEXTUREFORMAT2 0         // Two floating point values
#define D3DFVF_TEXTUREFORMAT1 3         // One floating point value
#define D3DFVF_TEXTUREFORMAT3 1         // Three floating point values
#define D3DFVF_TEXTUREFORMAT4 2  

#ifdef D3DFVF_TEXCOORDSIZE1
#undef D3DFVF_TEXCOORDSIZE1
#endif
#ifdef D3DFVF_TEXCOORDSIZE2
#undef D3DFVF_TEXCOORDSIZE2
#endif
#ifdef D3DFVF_TEXCOORDSIZE3
#undef D3DFVF_TEXCOORDSIZE3
#endif
#ifdef D3DFVF_TEXCOORDSIZE4
#undef D3DFVF_TEXCOORDSIZE4
#endif


#define D3DFVF_TEXCOORDSIZE1(CoordIndex) (D3DFVF_TEXTUREFORMAT1 << (CoordIndex*2 + 16))
#define D3DFVF_TEXCOORDSIZE2(CoordIndex) (D3DFVF_TEXTUREFORMAT2)
#define D3DFVF_TEXCOORDSIZE3(CoordIndex) (D3DFVF_TEXTUREFORMAT3 << (CoordIndex*2 + 16))
#define D3DFVF_TEXCOORDSIZE4(CoordIndex) (D3DFVF_TEXTUREFORMAT4 << (CoordIndex*2 + 16))


#include "../../xrEngine/Fmesh.h"

#include "FVF.h"

#include "General/XrayHardware.h"
#include "Resources/XRayResource.h"
#include "Resources/XRayTexture.h"


#include "Resources/XRayUniformAllocator.h"

#include "General/XRayMeshShading.h"

#include "Resources/XRayRenderItem.h"
#include "Resources/XRayShader.h"
#include "Resources/XRayShaderRef.h"
#include "Blenders/XRayBlenderCompiler.h"
#include "Resources/XRayResourcesManager.h"




#include "Resources/Visual/XRayRenderVisual.h"
#include "Resources/Visual/XRayFVisual.h"
#include "Resources/Visual/XRayFHierrarhyVisual.h"
#include "Resources/Visual/XRayFLOD.h"
#include "Resources/Visual/XRayFProgressive.h"
#include "Resources/XRaySkeletonX.h"
#include "Resources/Visual/XRayKinematics.h"
#include "Resources/Visual/XRayKinematicsAnimated.h"
#include "Resources/Visual/XRayFSkinned.h"
#include "Resources/Visual/XRayTreeVisual.h"
#include "Resources/XRayModelPool.h"
#include "Resources/XRayTextureDescrManager.h"


#include "Engine/OCC/XRayHOM.h"
#include "Engine/OCC/XRayRSector.h"
#include "Engine/XRayRenderFactory.h"

#include "General/XRayRdsgraphStructure.h"

#include "Render/XRayRenderTarget.h"

#include "Engine/XRayGlowManager.h"

#include "Engine/RenderInterface/XRayRenderInterface.h"

#include "Engine/XRayDUInterface.h"
#include "Engine/XRayDebugRender.h"

#include "Engine/Factory/UIRender/XRayUIRender.h"
#include "Engine/Factory/UIRender/XRayUISequenceVideoItem.h"
#include "Engine/Factory/UIRender/XRayConsoleRender.h"
#include "Engine/Factory/UIRender/XRayFontRender.h"


#include "Engine/Factory/EnvironmentRender/XRayEnvironmentRender.h"
#include "Engine/Factory/EnvironmentRender/XRayLensFlareRender.h"
#include "Engine/Factory/EnvironmentRender/XRayRainRender.h"


#include "Engine/Factory/Debug/XRayObjectSpaceRender.h"
#include "Engine/Factory/Debug/XRayStatGraphRender.h"
#include "Engine/Factory/Debug/XRayStatsRender.h"

#include "Engine/Factory/XRayRenderDeviceRender.h"

#include "Engine/Factory/XRayThunderboltDescRender.h"
#include "Engine/Factory/XRayThunderboltRender.h"

#include "Engine/Factory/XRayWallMarkArray.h"

#include "Engine/Factory/XRayApplicationRender.h"

#include "Blenders/Blender_CLSID.h"
#include "Blenders/Level/XRayBlenderDefault.h"
#include "Blenders/Level/XRayBlenderDefaultAref.h"
#include "Blenders/Level/XRayBlenderBmmD.h"
#include "Blenders/Level/XRayBlenderLmEbB.h"

#include "Blenders/Flora/XRayBlenderDetail.h"
#include "Blenders/Flora/XRayBlenderTree.h"

#include "Blenders/Model/XRayBlenderModel.h"
#include "Blenders/Model/XRayBlenderModelEbB.h"

#include "Blenders/Debug/XRayBlenderEditorSelection.h"
#include "Blenders/Debug/XRayBlenderEditorWire.h"


#include "Blenders/HUD/XRayBlenderParticle.h"
#include "Blenders/HUD/XRayBlenderScreenSet.h"

#include "Engine/XRayRenderConsole.h"