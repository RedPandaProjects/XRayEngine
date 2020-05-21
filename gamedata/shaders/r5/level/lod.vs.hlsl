#define USE_GLOBAL_UNIFORM
#define USE_TRANSFORMATION_UNIFORM
#include "common.h"
#define L_SCALE 	(1.55)
#define L_SUN_HACK 	(.7)

ROOT_SIGNATURE_GRAPHICS
PS_LOD_IN main(VS_LOD_IN input  )
{
    
    input.sun_af.rgb = input.sun_af.bgr;
    input.rgbh0.rgb = input.rgbh0.bgr;
    input.rgbh1.rgb = input.rgbh1.bgr;
    
    float factor = float(input.sun_af.w)/255.f;
    float4 pos = float4(lerp(input.position0, input.position1, factor), 1);

    PS_LOD_IN res;
    res.position = mul(VPW, pos);
    res.out_position_z = mul(WV, pos).z;
	
    res.uv0 = input.uv0;
    res.uv1 = input.uv1;
    
    float3 normal = normalize(lerp(input.normal0, input.normal1, factor));
    normal.y += 1;
    normal = normalize(normal);
    float4 rgbh = lerp(float4(input.rgbh0) / 255.f, float4(input.rgbh1)/255.f, factor) * L_SCALE;
    float sun = lerp(float(input.sun_af.x) / 255.f, float(input.sun_af.y)/255.f, factor) * L_SCALE;
    float sun_c = 1 + L_SUN_HACK * dot(normal, SunDirW.xyz); 

    float3 L_rgb = rgbh.rgb; 
    float3 L_hemi = HemiColor.rgb * rgbh.w; 
    float3 L_sun = SunColor.rgb * sun * sun_c; 
    float3 L_final = L_rgb + L_hemi +L_sun + AmbineColor.rgb;
	
    res.factor = factor;
    res.color0 = float4(L_final, float(input.sun_af.z)/255.f);
    res.fog = calc_fogging(pos);
	return res;
}