#pragma once
struct ISE_Abstract;
class XrGameManager
{
public:
	XrGameManager();
	~XrGameManager();
	ISE_Abstract*   create_entity(LPCSTR section);
	void			destroy_entity(ISE_Abstract*& abstract);
private:
	HMODULE m_Module;

	void(__cdecl*m_pFInitialize)(void);
	void(__cdecl* m_pFDestroy)(void);
	ISE_Abstract* (__cdecl* m_pFCreateEntity)(LPCSTR);
	void(__cdecl* m_pFDestroyEntity)(ISE_Abstract*& );
};
extern XrGameManager* g_SEFactoryManager;