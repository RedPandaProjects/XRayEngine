// Blender_Recorder.h: interface for the CBlender_Recorder class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#pragma pack(push,4)

class  CBlenderBuldier
{
public:
	
	CBlenderBuldier(const char*lua);
	~CBlenderBuldier();
	
	

	void				_cpp_Compile		(ShaderElement* _SH);
	ShaderElement* 		_lua_Compile		(LPCSTR namesp, LPCSTR name);
};
#pragma pack(pop)
