#include "stdafx.h"
#include "ILocatorAPI.h"
#include "..\BearBundle\BearCore\BearCore.hpp"
ILocatorAPI* xr_FS = NULL;

ILocatorAPI::ILocatorAPI():dwAllocGranularity(0), dwOpenCounter(0)
{
}

ILocatorAPI::~ILocatorAPI()
{
}

BOOL ILocatorAPI::file_find(LPCSTR full_name, FS_File& f)
{
	return 0;
}

int ILocatorAPI::file_list(FS_FileSet& dest, LPCSTR path, u32 flags, LPCSTR mask)
{
	return 0;
}

void ILocatorAPI::r_close(CStreamReader*& fs)
{
}

CStreamReader* ILocatorAPI::rs_open(LPCSTR initial, LPCSTR N)
{
	return nullptr;
}

xr_vector<LPSTR>* ILocatorAPI::file_list_open(LPCSTR initial, LPCSTR folder, u32 flags)
{
	xr_vector<LPSTR> *files = xr_new < xr_vector<LPSTR>>();
	string_path path;
	update_path(path, initial, folder);
	if(flags & FS_ListFiles)
	{
		BearVector<BearString> Files;
		BearFileManager::FindFiles(Files, path, "*",!(flags& FS_RootOnly),false);
		for (BearString& str : Files)
		{
			string_path fname;
			xr_strcpy(fname, *str);
			if (flags & FS_ClampExt)	if (0 != strext(fname)) *strext(fname) = 0;
			files->push_back(xr_strdup(fname));

		}
	}
	if (flags & FS_ListFolders)
	{
		BearVector<BearString> Files;
		BearFileManager::FindDirectories(Files, path, "*", !(flags & FS_RootOnly), false);
		for (BearString& str : Files)
		{
			string_path fname;
			xr_strcpy(fname, *str);
			if (flags & FS_ClampExt)	if (0 != strext(fname)) *strext(fname) = 0;
			files->push_back(xr_strdup( fname));

		}
	}
	return files;
}

xr_vector<LPSTR>* ILocatorAPI::file_list_open(LPCSTR path, u32 flags)
{
	return nullptr;
}

void ILocatorAPI::file_list_close(xr_vector<LPSTR>*& lst)
{
	for (LPSTR i : (* lst))
	{
		xr_free(i);
	}
	xr_delete(lst);
}

void ILocatorAPI::auth_generate(xr_vector<shared_str>& ignore, xr_vector<shared_str>& important)
{
}

u64 ILocatorAPI::auth_get()
{
	return u64();
}

void ILocatorAPI::auth_runtime(void*)
{
}
