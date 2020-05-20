#include "pch.h"
#include "../../xrEngine/XR_IOConsole.h"
#include "../../xrEngine/xr_ioc_cmd.h"
namespace XRayRenderConsole
{
	float		ps_r_GLOD_ssa_start = 256.f;
	float		ps_r_GLOD_ssa_end = 64.f;
	float		ps_r_ssaDISCARD = 3.5f;					//RO
	float		ps_r_ssaLOD_A = 64.f;
	float		ps_r_ssaLOD_B = 48.f;
	float		ps_r_LOD = 0.75f;
	float		ps_r_Tree_w_rot = 10.0f;
	float 		ps_r_Tree_w_amp = 0;
	float		ps_r_Tree_w_speed = 1.00f;
	Fvector		ps_r_Tree_Wave = { 0.1f, 0.01f, 0.11f };
	float		ps_r_Tree_SBC = 1.5f;
	Flags32     ps_r_testFlags;
	float		ps_r_detail_texture_range = 50;
	int			ps_r_GlowsPerFrame = 16;
	float		ps_r_bloom_threshold = .00001f;
	float		ps_r_bloom_speed = 100.f;
	float		ps_r_bloom_kernel_b = .7f;
	float		ps_r_bloom_kernel_g = 3.f;
	float		ps_r_bloom_kernel_scale = .7f;
};
class CCC_Dump : public IConsole_Command
{
public:
	CCC_Dump(LPCSTR N) : IConsole_Command(N) { bEmptyArgsHandled = TRUE; }
	virtual void Execute(LPCSTR args)
	{
		BearRenderStats::Dump();
	}
};

void XRayRenderConsole::Initialize()
{
	CMD4(CCC_Float, "r_geometry_lod", &ps_r_LOD, 0.1f, 3.0f);
	CMD4(CCC_Float, "r_ssa_lod_a", &ps_r_ssaLOD_A, 16, 96);
	CMD4(CCC_Float, "r_ssa_lod_b", &ps_r_ssaLOD_B, 32, 64);
	CMD4(CCC_Float, "r_detail_texture_range", &ps_r_detail_texture_range, 5, 175);

	CMD3(CCC_Mask, "r_draw_dynamic", &psDeviceFlags, rsDrawDynamic);
	CMD3(CCC_Mask, "r_draw_static", &psDeviceFlags, rsDrawStatic);
	CMD3(CCC_Mask, "r_check_duplicate_object", &ps_r_testFlags, RTF_CheckDuplicateObject);
	CMD1(CCC_Dump,"r_dump");

	//CMD4(CCC_Integer, "r_glows_per_frame", &ps_r_GlowsPerFrame, 2, 32);
	CMD4(CCC_Float, "r_bloom_threshold", &ps_r_bloom_threshold, 0.f, 1.f);
	CMD4(CCC_Float, "r_bloom_speed", &ps_r_bloom_speed, 0.f, 100.f);
	CMD4(CCC_Float, "r_bloom_kernel_b", &ps_r_bloom_kernel_b, 0.01f, 1.f);
	CMD4(CCC_Float, "r_bloom_kernel_g", &ps_r_bloom_kernel_g, 1.f, 7.f);
	CMD4(CCC_Float, "r_bloom_kernel_scale", &ps_r_bloom_kernel_scale, 0.5f, 2.f);
}
