#pragma once
class XRayShaderIncluder :public BearIncluder
{
	string_path m_GlobalPath;
	string_path m_LocalPath;
	IReader* m_F;
	void OpenFile(const char*name)
	{
		string_path FilePath;
		{
			xr_strcpy(FilePath, m_GlobalPath);
			xr_strcat(FilePath, name);
		}
		m_F = FS.r_open("$game_shaders$", FilePath);
		if (m_F == nullptr)
		{
			xr_strcpy(FilePath, m_GlobalPath);
			xr_strcat(FilePath, m_LocalPath);
			xr_strcat(FilePath, BEAR_PATH);
			xr_strcat(FilePath, name);
			m_F = FS.r_open("$game_shaders$", FilePath);
		}
	}
public:
	XRayShaderIncluder(const char* global_path, const char* local_path):m_F(nullptr){ xr_strcpy(m_GlobalPath, global_path); xr_strcpy(m_LocalPath, local_path);}
	virtual BearRef<BearInputStream> OpenAsStream(const bchar* name)
	{
		OpenFile(name);
		if (m_F == nullptr)return 0;
		return xr_new<BearMemoryStream>(m_F->pointer(), m_F->length());
	}
	virtual BearRef<BearBufferedReader> OpenAsBuffer(const bchar* name)
	{
		OpenFile(name);
		if (m_F == nullptr)return 0;
		return xr_new<BearMemoryStream>(m_F->pointer(), m_F->length());
	}
	~XRayShaderIncluder()
	{
		FS.r_close(m_F);
	}
};
