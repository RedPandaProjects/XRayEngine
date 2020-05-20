#pragma once
namespace XRayRenderConsole
{
	extern float		ps_r_GLOD_ssa_start;
	extern float		ps_r_GLOD_ssa_end;
	extern float		ps_r_ssaDISCARD;					
	extern float		ps_r_ssaLOD_A;
	extern float		ps_r_ssaLOD_B;
	extern float		ps_r_LOD ;
	extern float		ps_r_Tree_w_rot;
	extern float		ps_r_Tree_w_speed;
	extern Fvector		ps_r_Tree_Wave;
	extern float		ps_r_Tree_SBC;
	extern float 		ps_r_Tree_w_amp;
	extern float		ps_r_detail_texture_range;
	extern 	int			ps_r_GlowsPerFrame;
	extern float		ps_r_bloom_threshold;
	extern float		ps_r_bloom_speed;
	extern float		ps_r_bloom_kernel_b;
	extern float		ps_r_bloom_kernel_g;
	extern float		ps_r_bloom_kernel_scale;
	constexpr bool      ps_r_mesh_shader_enable = true;
	constexpr bool      ps_r_mesh_shader_debug  = true;
	constexpr size_t      ps_r_mesh_shader_thread = 32;
	enum ERenderTestFlags
	{
		RTF_None =0,
		RTF_CheckDuplicateObject = 1<<0,
	};
	extern Flags32          ps_r_testFlags;
	void Initialize();
}