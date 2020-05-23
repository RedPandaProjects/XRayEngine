#include "stdafx.h"
#include "ILocatorAPI.h"
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
	return nullptr;
}

xr_vector<LPSTR>* ILocatorAPI::file_list_open(LPCSTR path, u32 flags)
{
	return nullptr;
}

void ILocatorAPI::file_list_close(xr_vector<LPSTR>*& lst)
{
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
