
#include "common.h"

struct 	v_model_skinned_0
{
 	LOCATION_0 float4 P : position;
	LOCATION_1 uint4  N : normal;
	LOCATION_2 uint4  T : tangent;
	LOCATION_3 uint4  B : binormal;
	LOCATION_4 float2 tc	: uv1t;
};
struct 	v_model_skinned_1   		// 24 bytes
{
 	LOCATION_0 float4 P : position;
	LOCATION_1 uint4  N : normal;
	LOCATION_2 uint4  T : tangent;
	LOCATION_3 uint4  B : binormal;
	LOCATION_4 float2 tc	: uv1t;
};
struct 	v_model_skinned_2		// 28 bytes
{
	LOCATION_0 float4 P : position;
	LOCATION_1 uint4  N : normal;
	LOCATION_2 uint4  T : tangent;
	LOCATION_3 uint4  B : binormal;
	LOCATION_4 float4 tc	: uv1t;
};

struct 	v_model_skinned_3		// 28 bytes
{
	LOCATION_0 float4 P : position;
	LOCATION_1 uint4  N : normal;
	LOCATION_2 uint4  T : tangent;
	LOCATION_3 uint4  B : binormal;
	LOCATION_4 float4 tc	: uv1t;
};

struct 	v_model_skinned_4		// 28 bytes
{
	LOCATION_0 float4 P : position;
	LOCATION_1 uint4  N : normal;
	LOCATION_2 uint4  T : tangent;
	LOCATION_3 uint4  B : binormal;
	LOCATION_4 float2 tc: uv1t;
	LOCATION_5 uint4 ind: indices_t;
};
struct 	v_model
{
	LOCATION_0 float4 position : POSITION;
	LOCATION_1 float2 uv : UV;
	LOCATION_2 float3 normal : NORMAL;
	LOCATION_3 float3	tanget : TANGENT;		// (nx,ny,nz)
	LOCATION_4 float3	binormal : BINORMAL;		// (nx,ny,nz)
};
float4 	u_position	(float4 v)	{ return float4(v.xyz, 1.f);	}	// -12..+12

//////////////////////////////////////////////////////////////////////////////////////////
//uniform float4 	sbones_array	[256-22] : register(vs,c22);
//tbuffer	SkeletonBones
//{
BINDING_UNIFORM_1  cbuffer ConstantBuffer2  : register(b1)
{
	float4 	sbones_array	[256-22];
}
//}

float3 	skinning_dir 	(float3 dir, float4 m0, float4 m1, float4 m2)
{
	float3 	U 	= unpack_normal	(dir);
	return 	float3	
		(
			dot	(m0.xyz, U),
			dot	(m1.xyz, U),
			dot	(m2.xyz, U)
		);
}
float4 	skinning_pos 	(float4 pos, float4 m0, float4 m1, float4 m2)
{
	float4 	P	= u_position	(pos);
	return 	float4
		(
			dot	(m0, P),
			dot	(m1, P),
			dot	(m2, P),
			1
		);
}

v_model skinning_0	(v_model_skinned_0	v)
{
	//	Swizzle for D3DCOLOUR format
	v.N.xyz		= v.N.zyx;
	v.T.xyz		= v.T.zyx;
	v.B.xyz		= v.B.zyx;

	// skinning
	v_model 	o;
	o.position 		= u_position	(v.P);
	o.normal 		= unpack_normal(float3(v.N.xyz)/255.f);
	o.tanget		= unpack_normal(float3(v.T.xyz) / 255.f);
	o.binormal 		= unpack_normal(float3(v.B.xyz) / 255.f);
	o.uv 		= v.tc;
	return o;
}
v_model skinning_1 	(v_model_skinned_1	v)
{
	//	Swizzle for D3DCOLOUR format
	v.N.xyz		= v.N.zyx;
	v.T.xyz		= v.T.zyx;
	v.B.xyz		= v.B.zyx;

	// matrices
	int 	mid = v.N.w;
	float4  m0 	= sbones_array[mid+0];
	float4  m1 	= sbones_array[mid+1];
	float4  m2 	= sbones_array[mid+2];

	// skinning
	v_model 	o;
	o.position 		= skinning_pos(v.P, m0,m1,m2 );
	o.normal = skinning_dir(float3(v.N.xyz) / 255.f, m0,m1,m2 );
	o.tanget = skinning_dir(float3(v.T.xyz) / 255.f, m0,m1,m2 );
	o.binormal = skinning_dir(float3(v.B.xyz) / 255.f, m0,m1,m2 );
	o.uv 		= v.tc;
	return o;
}
v_model skinning_2 	(v_model_skinned_2	v)
{
	//	Swizzle for D3DCOLOUR format
	v.N.xyz		= v.N.zyx;
	v.T.xyz		= v.T.zyx;
	v.B.xyz		= v.B.zyx;

	// matrices
	int 	id_0 	= int(v.tc.z+0.3);
	float4  m0_0 	= sbones_array[id_0+0];
	float4  m1_0 	= sbones_array[id_0+1];
	float4  m2_0 	= sbones_array[id_0+2];
	int 	id_1 	= int(v.tc.w+0.3);
	float4  m0_1 	= sbones_array[id_1+0];
	float4  m1_1 	= sbones_array[id_1+1];
	float4  m2_1 	= sbones_array[id_1+2];

	// lerp
	float 	w 	= float(v.N.w )/255.f;
	float4  m0 	= lerp(m0_0,m0_1,w);
	float4  m1 	= lerp(m1_0,m1_1,w);
	float4  m2 	= lerp(m2_0,m2_1,w);

	// skinning
	v_model 	o;
	o.position 		= skinning_pos(v.P, m0,m1,m2 );

	o.normal = skinning_dir(float3(v.N.xyz) / 255.f, m0, m1, m2);
	o.tanget = skinning_dir(float3(v.T.xyz) / 255.f, m0, m1, m2);
	o.binormal = skinning_dir(float3(v.B.xyz) / 255.f, m0, m1, m2);
	o.uv 		= v.tc.xy;
	return o;
}
v_model skinning_3 	(v_model_skinned_3	v)
{
	//	Swizzle for D3DCOLOUR format
	v.N.xyz		= v.N.zyx;
	v.T.xyz		= v.T.zyx;
	v.B.xyz		= v.B.zyx;

	// matrices
	int 	id_0 	= v.tc.z;
	float4  m0_0 	= sbones_array[id_0+0];
	float4  m1_0 	= sbones_array[id_0+1];
	float4  m2_0 	= sbones_array[id_0+2];
	int 	id_1 	= v.tc.w;
	float4  m0_1 	= sbones_array[id_1+0];
	float4  m1_1 	= sbones_array[id_1+1];
	float4  m2_1 	= sbones_array[id_1+2];
	int 	id_2 	= v.B.w;
	float4  m0_2 	= sbones_array[id_2+0];
	float4  m1_2 	= sbones_array[id_2+1];
	float4  m2_2 	= sbones_array[id_2+2];

	// lerp
	float 	w0 	= float(v.N.w)/255.f;
	float 	w1 	=  float(v.T.w)/255.f;
	float 	w2 	= 1-w0-w1;
	float4  m0 	= m0_0*w0;
	float4  m1 	= m1_0*w0;
	float4  m2 	= m2_0*w0;

			m0 	+= m0_1*w1;
			m1 	+= m1_1*w1;
			m2 	+= m2_1*w1;

			m0 	+= m0_2*w2;
			m1 	+= m1_2*w2;
			m2 	+= m2_2*w2;

	// skinning
	v_model 	o;
	o.position 		= skinning_pos(v.P, m0,m1,m2 );

	o.normal = skinning_dir(float3(v.N.xyz) / 255.f, m0, m1, m2);
	o.tanget = skinning_dir(float3(v.T.xyz) / 255.f, m0, m1, m2);
	o.binormal = skinning_dir(float3(v.B.xyz) / 255.f, m0, m1, m2);
	o.uv 		= v.tc.xy;

	return o;
}
v_model skinning_4 	(v_model_skinned_4	v)
{
	//	Swizzle for D3DCOLOUR format
	v.N.xyz		= v.N.zyx;
	v.T.xyz		= v.T.zyx;
	v.B.xyz		= v.B.zyx;
	v.ind.xyz	= v.ind.zyx;

	// matrices
	float	id[4];
	float4	m[4][3];	//	[bone index][matrix row or column???]
	[unroll]
	for (int i=0; i<4; ++i)
	{		
		id[i] = v.ind[i];
		[unroll]
		for (int j=0; j<3; ++j)
			m[i][j] = sbones_array[id[i]+j];
	}

	// lerp
	float	w[4];
	w[0] 	= float(v.N.w)/255.f;
	w[1] 	= float(v.T.w)/255.f;
	w[2] 	= float(v.B.w)/255.f;
	w[3]	= 1-w[0]-w[1]-w[2];

	float4  m0 	= m[0][0]*w[0];
	float4  m1 	= m[0][1]*w[0];
	float4  m2 	= m[0][2]*w[0];

	[unroll]
	for (int t=1; t<4; ++t)
	{
		m0 	+= m[t][0]*w[t];
		m1 	+= m[t][1]*w[t];
		m2 	+= m[t][2]*w[t];
	}

	// skinning
	v_model 	o;
	o.position 		= skinning_pos(v.P, m0,m1,m2 );
	o.normal = skinning_dir(float3(v.N.xyz) / 255.f, m0, m1, m2);
	o.tanget = skinning_dir(float3(v.T.xyz) / 255.f, m0, m1, m2);
	o.binormal = skinning_dir(float3(v.B.xyz) / 255.f, m0, m1, m2);
	o.uv 		= v.tc;

	return o;
}
