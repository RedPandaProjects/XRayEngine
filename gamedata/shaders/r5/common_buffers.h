
#ifdef VULKAN
#define BINDING_UNIFORM_0 [[vk::binding(0)]]
#define BINDING_UNIFORM_1 [[vk::binding(1)]]
#define BINDING_UNIFORM_2 [[vk::binding(2)]]
#define BINDING_UNIFORM_3 [[vk::binding(3)]]
#define BINDING_UNIFORM_4 [[vk::binding(4)]]
#define BINDING_UNIFORM_5 [[vk::binding(5)]]
#define BINDING_UNIFORM_6 [[vk::binding(6)]]
#define BINDING_UNIFORM_7 [[vk::binding(7)]]
#define BINDING_UNIFORM_8 [[vk::binding(8)]]
#define BINDING_UNIFORM_9 [[vk::binding(9)]]
#define BINDING_UNIFORM_10 [[vk::binding(10)]]
#define BINDING_UNIFORM_11 [[vk::binding(11)]]
#define BINDING_UNIFORM_12 [[vk::binding(12)]]


#define BINDING_SRV_0 [[vk::binding(0+16)]]
#define BINDING_SRV_1 [[vk::binding(1+16)]]
#define BINDING_SRV_2 [[vk::binding(2+16)]]
#define BINDING_SRV_3 [[vk::binding(3+16)]]
#define BINDING_SRV_4 [[vk::binding(4+16)]]
#define BINDING_SRV_5 [[vk::binding(5+16)]]
#define BINDING_SRV_6 [[vk::binding(6+16)]]
#define BINDING_SRV_7 [[vk::binding(7+16)]]


#define BINDING_SAMPLER_0  [[vk::binding(0+32)]]
#define BINDING_SAMPLER_1 [[vk::binding(1+32)]]
#define BINDING_SAMPLER_2 [[vk::binding(2+32)]]
#define BINDING_SAMPLER_3  [[vk::binding(3+32)]]
#else
#define BINDING_UNIFORM_0
#define BINDING_UNIFORM_1
#define BINDING_UNIFORM_2 
#define BINDING_UNIFORM_3 
#define BINDING_UNIFORM_4 
#define BINDING_UNIFORM_5 
#define BINDING_UNIFORM_6 
#define BINDING_UNIFORM_7 
#define BINDING_UNIFORM_8 
#define BINDING_UNIFORM_9 
#define BINDING_UNIFORM_10 
#define BINDING_UNIFORM_11 
#define BINDING_UNIFORM_12 


#define BINDING_SRV_0 
#define BINDING_SRV_1
#define BINDING_SRV_2 
#define BINDING_SRV_3 
#define BINDING_SRV_4 
#define BINDING_SRV_5 
#define BINDING_SRV_6 
#define BINDING_SRV_7 

#define BINDING_SAMPLER_0  
#define BINDING_SAMPLER_1
#define BINDING_SAMPLER_2 
#define BINDING_SAMPLER_3 
#endif

#ifdef USE_AREF_UNIFORM
BINDING_UNIFORM_9 cbuffer ARefBuffer : register(b9)
{
	float        AlphaRef;
}
#endif
#ifdef USE_DETAIL_UNIFORM
BINDING_UNIFORM_10 cbuffer ScalarDetailBuffer : register(b10)
{
	float4        DetailParams;
}

#endif

#ifdef USE_LOCAL_UNIFORM
BINDING_UNIFORM_11 cbuffer LocalBuffer : register(b11)
{
	float4        DetailParams;
}

#endif

#ifdef USE_GLOBAL_UNIFORM
BINDING_UNIFORM_12 cbuffer GlobalBUffer : register(b12)
{
	float4      FogColor;
	float4      FogPlane;
	float4      FogParam;
	float4		SunColor;
	float4		SunDirW;
	float4		SunDirE;
	float4      HemiColor;
	float4 		AmbineColor;
	float4		PosDecompressParams;
	float4		EyePosition;
	float4      Timer;
	float4      BloomParams;
	float4		BloomW[2];
	float		DetailTextureRange;
}
#endif
#ifdef USE_TRANSFORMATION_UNIFORM
BINDING_UNIFORM_0 cbuffer TransformationBuffer : register(b0)
{
	matrix View;
	matrix Project;
	matrix World;
	matrix VPW;
	matrix VP;
	matrix WV;
}
#endif
#ifdef USE_HUD_TRANSFORMATION_UNIFORM
BINDING_UNIFORM_0  cbuffer TransformationBuffer : register(b0)
{
	float4 screen;
}
#endif
#ifdef USE_TREE_UNIFORM
BINDING_UNIFORM_1 cbuffer TreeBuffer : register(b1)
{
	matrix xform;
	matrix xform_v;
	float4 c_scale, c_bias, wind, wave;
	float4 consts;
	float4 c_sun;

}
#endif