
#define USE_GLOBAL_UNIFORM
#define USE_TRANSFORMATION_UNIFORM
#define USE_TREE_UNIFORM
#include "common.h"

ROOT_SIGNATURE_GRAPHICS
PS_BASE_IN main(VS_MUModel_IN input  )
{

    PS_BASE_IN output;

	
	
	float3 	pos		= mul		((float3x4)xform,  input.position);
#ifdef USE_WIND	
	float 	base 	= xform._24;			// take base height from matrix
	float 	dp		= calc_cyclic  (wave.w+dot(pos,(float3)wave));
	float 	H 		= pos.y - base;			// height of vertex (scaled, rotated, etc.)
	float 	frac 	= input.uv.z*consts.x;		// fractional (or rigidity)
	float 	inten 	= H * dp;			// intensity
	float2 	result	= calc_xz_wave	(wind.xz*inten, frac);
	float4 	f_pos 	= float4(pos.x+result.x, pos.y, pos.z+result.y, 1);
#else
    float4 f_pos = float4(pos.x, pos.y, pos.z ,1);
#endif
    output.fog = calc_fogging(f_pos);
	
    output.position = mul(VPW, f_pos);
	output.out_position_z = mul(WV, f_pos).z;
	

    input.normal.xyz = input.normal.zyx;
	// Lighting
    output.normal = unpack_normal(float3(input.normal.xyz) / 255.f);
    float3 N = mul((float3x3) xform, output.normal); //normalize 	(mul (m_xform,  unpack_normal(v.nc)));
    float L_base = float(input.normal.w) / 255.f; // base hemisphere
	float4 	L_unpack= c_scale*L_base+c_bias;						// unpacked and decompressed
	float3 	L_rgb 	= L_unpack.xyz;								// precalculated RGB lighting
	float3 	L_hemi 	= v_hemi(N)* L_unpack.w;					// hemisphere
	float3 	L_sun 	= v_sun (N)* (L_base*c_sun.x+c_sun.y);			// sun
	//float3 	L_sun 	= L_sun_color*max( 0, (1+dot(N,-L_sun_dir_w))/2) * (L_base*c_sun.x+c_sun.y)*1.0f;			// sun
    output.color1 = L_rgb + L_hemi + L_sun;;

	output.uv = float2(input.uv.xy) * consts.xy;
#ifdef MESH_SHADER_DEBUG
    output.MeshletIndex = 0; 
#endif
#ifdef USE_DETAIL
    output.uv_detail.xy = output.uv * DetailParams.x; // dt tc
    float2 dt = calc_detail(f_pos.xyz); // 
    output.uv_detail.zw = dt.xy;
#endif
	
	return output;
}