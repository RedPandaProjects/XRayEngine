
float2 unpack_tc_base(float2 tc, float du, float dv)
{
	return (tc.xy + float2(du, dv)) * (32.f / 32768.f); //!Increase from 32bit to 64bit floating point
}
float3 	unpack_normal(float3 v) { return 2 * v - 1; }
float2 	unpack_tc_lmap(float2 tc) { return tc * (1.f / 32768.f); } // [-1  .. +1 ]


float2 	calc_xz_wave(float2 dir2D, float frac)
{
	// Beizer
	float2  ctrl_A = float2(0.f, 0.f);
	float2 	ctrl_B = float2(dir2D.x, dir2D.y);
	return  lerp(ctrl_A, ctrl_B, frac);			//!This calculates tree wave. No changes made
}
float 	calc_cyclic(float x)
{
	float 	phase = 1 / (2 * 3.141592653589f);
	float 	sqrt2 = 1.4142136f;
	float 	sqrt2m2 = 2.8284271f;
	float 	f = sqrt2m2 * frac(x) - sqrt2;	// [-sqrt2 .. +sqrt2] !No changes made, but this controls the grass wave (which is violent if I must say)
	return 	f * f - 1.f;				// [-1     .. +1]
}
#ifdef USE_GLOBAL_UNIFORM
float3 	calc_reflection(float3 pos_w, float3 norm_w)
{
	return reflect(normalize(pos_w - EyePosition.xyz), norm_w);
}
float3	v_hemi(float3 n) { return HemiColor.xyz/* *(.5f + .5f*n.y) */; }
float3 	v_sun(float3 n) { return SunColor.xyz * max(0, dot(n, -SunDirW.xyz)); }
float  	calc_fogging(float4 w_pos) { return dot(w_pos, FogPlane); }
float2 	calc_detail(float3 w_pos)
{
	float  	dtl = distance(w_pos, EyePosition.xyz) * DetailTextureRange;
	dtl = min(dtl * dtl, 1);
	half  	dt_mul = 1 - dtl;	// dt*  [1 ..  0 ]
	half  	dt_add = .5 * dtl;	// dt+	[0 .. 0.5]
	return	half2(dt_mul, dt_add);
}
/*float3 	calc_model_hemi(float3 norm_w) { return (norm_w.y * 0.5 + 0.5) * L_dynamic_props.w * HemiColor; }
float3	calc_model_lq_lighting(float3 norm_w) { return calc_model_hemi(norm_w) + L_ambient + L_dynamic_props.xyz * calc_sun(norm_w); }*/

#endif