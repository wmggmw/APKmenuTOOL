// dll.cpp : DLL ������ʵ�֡�


#include "stdafx.h"
#include "resource.h"
#include "dll.h"

HINSTANCE  _hInstance;
TCHAR g_szModulePath[MAX_PATH]; 
//NOTIFYICONDATA m_NotifyIconData;

class CdllModule : public CAtlDllModuleT< CdllModule >
{
public :
	DECLARE_LIBID(LIBID_dllLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_DLL, "{FA46D921-04EF-4E0D-A7EA-95B46736D1DB}")
};

CdllModule _AtlModule;


#ifdef _MANAGED
#pragma managed(push, off)
#endif

// DLL ��ڵ�
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	_hInstance = hInstance;
	//Tips = FALSE;
	GetModuleFileName(hInstance, g_szModulePath, sizeof(g_szModulePath)/sizeof(TCHAR)); 
	//(_tcsrchr(szModulePath, _T('//')))[1] = 0; //ɾ���ļ�����ֻ���·��
	for (size_t i= wcslen(g_szModulePath)-1;i>=0;i--)
	{
		if (g_szModulePath[i] == _T('\\'))
		{
			g_szModulePath[i+1]=_T('t');
			g_szModulePath[i+2]=_T('o');
			g_szModulePath[i+3]=_T('o');
			g_szModulePath[i+4]=_T('l');
			g_szModulePath[i+5]=_T('s');
			g_szModulePath[i+6]=_T('\\');
			g_szModulePath[i+7]= 0;
			break;
		}
	}
	
	//m_notifyicondata.cbsize = (dword)sizeof(notifyicondata);
	//m_notifyicondata.hwnd = (hwnd)getmodulehandle(null);
	//m_notifyicondata.uid = null;
	//m_notifyicondata.uflags =  nif_info|nif_icon; // ��������ͼ�깦��;
	////m_notifyicondata.ucallbackmessage = null; // ������Ӧ��Ϣid;
	//m_notifyicondata.hicon = loadicon(null,idi_error);//loadicon(getmodulehandle(null), makeintresource(idi_icon1)); // ��ȡͼ��;
	//m_notifyicondata.dwinfoflags = niif_info;
	//shell_notifyicon(nim_add, &m_notifyicondata);
    return _AtlModule.DllMain(dwReason, lpReserved); 
}

#ifdef _MANAGED
#pragma managed(pop)
#endif




// ����ȷ�� DLL �Ƿ���� OLE ж��
STDAPI DllCanUnloadNow(void)
{
    return _AtlModule.DllCanUnloadNow();
}


// ����һ���๤���Դ������������͵Ķ���
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    return _AtlModule.DllGetClassObject(rclsid, riid, ppv);
}


// DllRegisterServer - ������ӵ�ϵͳע���
STDAPI DllRegisterServer(void)
{
    // ע��������Ϳ�����Ϳ��е����нӿ�
	HRESULT hr;
	HKEY hKey;

	static char pszGUID[] = "{E13F371F-7B4C-4D04-AE25-98332A50BEEE}";
	hr = _AtlModule.DllRegisterServer();
	
	if (FAILED(hr))
	{
		return hr;
	}

	if (RegCreateKeyA(HKEY_CLASSES_ROOT, 
		"*\\shellex\\ContextMenuHandlers\\CtxMenu", &hKey) != ERROR_SUCCESS)
	{
		return E_FAIL;
	}

	if (RegSetValueA(hKey, NULL, REG_SZ, pszGUID,
		(DWORD)strlen(pszGUID)) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return E_FAIL;
	}
	//�ļ���֧��
	if (RegCreateKeyA(HKEY_CLASSES_ROOT, 
		"Directory\\shellex\\ContextMenuHandlers\\CtxMenu", &hKey) != ERROR_SUCCESS)
	{
		return E_FAIL;
	}

	if (RegSetValueA(hKey, NULL, REG_SZ, pszGUID,
		(DWORD)strlen(pszGUID)) != ERROR_SUCCESS)
	{
		RegCloseKey(hKey);
		return E_FAIL;
	}
	return hr;
}


// DllUnregisterServer - �����ϵͳע������Ƴ�
STDAPI DllUnregisterServer(void)
{
	RegDeleteKeyA(HKEY_CLASSES_ROOT, "*\\shellex\\ContextMenuHandlers\\CtxMenu");
	RegDeleteKeyA(HKEY_CLASSES_ROOT, "Directory\\shellex\\ContextMenuHandlers\\CtxMenu");

	HRESULT hr = _AtlModule.DllUnregisterServer();
	return hr;
}

