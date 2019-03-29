// dllcontext.cpp : Cdllcontext ��ʵ��

#include "stdafx.h"
#include "dllcontext.h"
#ifndef _DEBUGPRINTF_H_ 
#define _DEBUGPRINTF_H_ 

#include<Windows.h> 
#include <tchar.h> 

//���������Ϣ��������������ڵĺ궨�� 
//ʹ��win API��DEBUG�汾��ִ�У�RELEASE�汾�򲻻� 
//������ʹ��DebugView��WinDbg�ȹ��߲鿴��� 

#ifdef _DEBUG 

#define DP0(fmt) {TCHAR sOut[256];_stprintf_s(sOut,_T(fmt));OutputDebugString(sOut);} 
#define DP1(fmt,var) {TCHAR sOut[256];_stprintf_s(sOut,_T(fmt),var);OutputDebugString(sOut);} 
#define DP2(fmt,var1,var2) {TCHAR sOut[256];_stprintf_s(sOut,_T(fmt),var1,var2);OutputDebugString(sOut);} 
#define DP3(fmt,var1,var2,var3) {TCHAR sOut[256];_stprintf_s(sOut,_T(fmt),var1,var2,var3);OutputDebugString(sOut);} 

#endif 

#ifndef _DEBUG 

#define DP0(fmt) ; 
#define DP1(fmt, var) ; 
#define DP2(fmt,var1,var2) ; 
#define DP3(fmt,var1,var2,var3) ; 

#endif 

#endif 

HRESULT STDMETHODCALLTYPE Cdllcontext::Initialize( 
	/* [in] */ LPCITEMIDLIST pidlFolder,
	/* [in] */ IDataObject *pdtobj,
	/* [in] */ HKEY hkeyProgID)
{
	//�ļ��б�
	if( pdtobj != NULL ) 
	{
		STGMEDIUM stg = { TYMED_HGLOBAL };
		FORMATETC etc = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
		//��pdtobj�л�ȡ�ļ��б�ָ��,���Ǵ洢��HDROP��ʽ��
		if( SUCCEEDED( pdtobj->GetData( &etc, &stg ) ) ) 
		{
			//ȡ��HDROP���
			HDROP hDrop = (HDROP) ::GlobalLock( stg.hGlobal );
			//��ȡ�ò����б�ѡ�е��ļ�����Ŀ
			UINT uCount = ::DragQueryFile( hDrop, (UINT) -1, NULL, 0 );
			allFileName.clear();
			for( UINT uIndex = 0; uIndex < uCount; uIndex++ ) 
			{
				TCHAR szFileName[MAX_PATH] = { 0 };
				CString strTemp = _T("");
				//��ȡ�ļ���
				::DragQueryFile(hDrop, uIndex, szFileName, (sizeof(szFileName) / sizeof(TCHAR)) - 1);
				strTemp = szFileName;
				allFileName.push_back(strTemp);
				//szFileNameΪ�ļ�(��)��
			}
			::GlobalUnlock( stg.hGlobal );
			::ReleaseStgMedium( &stg );
		}
	}    
	return S_OK;
}

HRESULT STDMETHODCALLTYPE Cdllcontext::QueryContextMenu(THIS_
															 HMENU hmenu,
															 UINT indexMenu,
															 UINT idCmdFirst,
															 UINT idCmdLast,
															 UINT uFlags)
{
	//MENUITEMINFO mii;
	UINT uCmdID = idCmdFirst;
	uCmdID += 22;
	CString strConFileName, strCustomMenu;
	CString strSep = _T("-");

	static WCHAR szMenuText_Popup[] = L"��׿�Ҽ�����";
	static WCHAR szMenuText_0[] = L"����·��";
	static WCHAR szMenuText_1[] = L"DEX->JAR";
	static WCHAR szMenuText_2[] = L"Manifest->TXT|AXML->XML";
	static WCHAR szMenuText_3[] = L"�鿴APK��Ϣ";
	static WCHAR szMenuText_4[] = L"�鿴ǩ����Ϣ";
	static WCHAR szMenuText_5[] = L"ǩ��";
	static WCHAR szMenuText_6[] = L"��װ��ж�ذ�װ��";
	static WCHAR szMenuText_7[] = L"��װ���滻��װ��";
	static WCHAR szMenuText_8[] = L"ж��";
	static WCHAR szMenuText_9[] = L"�鿴�ӿ���Ϣ";
	static WCHAR szMenuText_10[] = L"APK��ȼ��";
	static WCHAR szMenuText_11[] = L"�鿴�ֻ���Ϣ";
	static WCHAR szMenuText_12[] = L"�ֻ���ͼ";
	static WCHAR szMenuText_13[] = L"��ȡͼ��";
	static WCHAR szMenuText_14[] = L"zipalign�Ż�";
	static WCHAR szMenuText_15[] = L"������";
	static WCHAR szMenuText_16[] = L"�ر���";
	static WCHAR szMenuText_17[] = L"��ȡso";
	static WCHAR szMenuText_18[] = L"lib�淶�Լ��";
	static WCHAR szMenuText_19[] = L"�Զ�����";
	static WCHAR szMenuText_20[] = L"���� v1.2";

	strConFileName = g_szModulePath;
	strConFileName += _T("config\\config.ini"); 

	TCHAR szTmp[2048] = { 0 };

	GetPrivateProfileString(_T("CustomMenu"), _T("name"), _T(""), szTmp, 2048, strConFileName);
	strCustomMenu = szTmp;
	m_CustomMenu.clear();
	if (!strCustomMenu.IsEmpty())
	{
		SplitString(strCustomMenu, strSep, m_CustomMenu);
	}

	if (uFlags & CMF_DEFAULTONLY)
	{
		return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0);
	}


	InsertMenu(hmenu, indexMenu, MF_SEPARATOR | MF_BYPOSITION, 0, NULL); 

	indexMenu++;
	HMENU hSubMenu = CreateMenu();
	
	if(hSubMenu)
	{
		InsertMenu(hSubMenu, 0, MF_STRING | MF_BYPOSITION, uCmdID++, szMenuText_0);
		SetMenuItemBitmaps(hSubMenu, 0, MF_BYPOSITION, m_hBitmapCopyPath, m_hBitmapCopyPath);
		InsertMenu(hSubMenu, 1, MF_STRING  | MF_BYPOSITION, uCmdID++, szMenuText_1);
		SetMenuItemBitmaps(hSubMenu, 1, MF_BYPOSITION, m_hBitmapDex2Jar, m_hBitmapDex2Jar);//dex->jar
		InsertMenu(hSubMenu, 2, MF_STRING  | MF_BYPOSITION, uCmdID++, szMenuText_2);
		SetMenuItemBitmaps(hSubMenu, 2, MF_BYPOSITION, m_hBitmapMa2Txt, m_hBitmapMa2Txt);//manifest->txt
		InsertMenu(hSubMenu, 3, MF_STRING  | MF_BYPOSITION, uCmdID++, szMenuText_3);
		SetMenuItemBitmaps(hSubMenu, 3, MF_BYPOSITION, m_hBitmapApkinfo, m_hBitmapApkinfo);//apk info
		InsertMenu(hSubMenu, 4, MF_STRING  | MF_BYPOSITION, uCmdID++, szMenuText_4);
		SetMenuItemBitmaps(hSubMenu, 4, MF_BYPOSITION, m_hBitmapSigninfo, m_hBitmapSigninfo);//ǩ����Ϣ
		InsertMenu(hSubMenu, 5, MF_STRING  | MF_BYPOSITION, uCmdID++, szMenuText_5);
		SetMenuItemBitmaps(hSubMenu, 5, MF_BYPOSITION, m_hBitmapSign, m_hBitmapSign);//ǩ��
		InsertMenu(hSubMenu, 6, MF_STRING | MF_BYPOSITION, uCmdID++, szMenuText_6);
		SetMenuItemBitmaps(hSubMenu, 6, MF_BYPOSITION, m_hBitmapInstall, m_hBitmapInstall);//ж�ذ�װ
		InsertMenu(hSubMenu, 7, MF_STRING | MF_BYPOSITION, uCmdID++, szMenuText_7);
		SetMenuItemBitmaps(hSubMenu, 7, MF_BYPOSITION, m_hBitmapInstall, m_hBitmapInstall);//�滻��װ
		InsertMenu(hSubMenu, 8, MF_STRING | MF_BYPOSITION, uCmdID++, szMenuText_8);
		SetMenuItemBitmaps(hSubMenu, 8, MF_BYPOSITION, m_hBitmapUninstall, m_hBitmapUninstall);//ж��
		InsertMenu(hSubMenu, 9, MF_STRING | MF_BYPOSITION, uCmdID++, szMenuText_9);
		SetMenuItemBitmaps(hSubMenu, 9, MF_BYPOSITION, m_hBitmapDetect, m_hBitmapDetect);//�ӿ���Ϣ

		InsertMenu(hSubMenu, 10, MF_STRING | MF_BYPOSITION, uCmdID++, szMenuText_10);//��ȼ��
		SetMenuItemBitmaps(hSubMenu, 10, MF_BYPOSITION, m_hBitmapDeep, m_hBitmapDeep);


		InsertMenu(hSubMenu, 11, MF_STRING | MF_BYPOSITION, uCmdID++, szMenuText_11);
		SetMenuItemBitmaps(hSubMenu, 11, MF_BYPOSITION, m_hBitmapPhone, m_hBitmapPhone);//�ֻ���Ϣ
		InsertMenu(hSubMenu, 12, MF_STRING | MF_BYPOSITION, uCmdID++, szMenuText_12);
		SetMenuItemBitmaps(hSubMenu, 12, MF_BYPOSITION, m_hBitmapPhoto, m_hBitmapPhoto);//��ͼ
		InsertMenu(hSubMenu, 13, MF_STRING | MF_BYPOSITION, uCmdID++, szMenuText_13);
		SetMenuItemBitmaps(hSubMenu, 13, MF_BYPOSITION, m_hBitmapExtractIcon, m_hBitmapExtractIcon);//��ȡͼ��
		InsertMenu(hSubMenu, 14, MF_STRING | MF_BYPOSITION, uCmdID++, szMenuText_14);
		SetMenuItemBitmaps(hSubMenu, 14, MF_BYPOSITION, m_hBitmapAlign, m_hBitmapAlign);//�Ż�
		InsertMenu(hSubMenu, 15, MF_STRING  | MF_BYPOSITION, uCmdID++, szMenuText_15);
		SetMenuItemBitmaps(hSubMenu, 15, MF_BYPOSITION, m_hBitmapDes, m_hBitmapDes);//������
		InsertMenu(hSubMenu, 16, MF_STRING  | MF_BYPOSITION, uCmdID++, szMenuText_16);
		SetMenuItemBitmaps(hSubMenu, 16, MF_BYPOSITION, m_hBitmapCom, m_hBitmapCom);//�ر���
		InsertMenu(hSubMenu, 17, MF_STRING | MF_BYPOSITION, uCmdID++, szMenuText_17);
		SetMenuItemBitmaps(hSubMenu, 17, MF_BYPOSITION, m_hBitmapLib, m_hBitmapLib);//��ȡSo

		InsertMenu(hSubMenu, 18, MF_STRING | MF_BYPOSITION, uCmdID++, szMenuText_18);
		SetMenuItemBitmaps(hSubMenu, 18, MF_BYPOSITION, m_hBitmapRegular, m_hBitmapRegular);//lib�淶�Լ��
		HMENU hCustomSubMenu = NULL;
		if (m_CustomMenu.size() > 0){}
			hCustomSubMenu = CreateMenu();
		
		if (hCustomSubMenu)
		{
			int i = 0;
			for (list<CString>::iterator it = m_CustomMenu.begin(); it != m_CustomMenu.end(); ++it, i++)
			{
				InsertMenu(hCustomSubMenu, i, MF_STRING | MF_BYPOSITION, uCmdID++, *it);
			}

			InsertMenu(hSubMenu, 19, MF_STRING | MF_POPUP | MF_BYPOSITION, (UINT_PTR)hCustomSubMenu, szMenuText_19);
			SetMenuItemBitmaps(hSubMenu, 19, MF_BYPOSITION, m_hBitmapCustom, m_hBitmapCustom);//�Զ�����

			InsertMenu(hSubMenu, 20, MF_STRING | MF_BYPOSITION, uCmdID++, szMenuText_20);
			SetMenuItemBitmaps(hSubMenu, 20, MF_BYPOSITION, m_hBitmapAbout, m_hBitmapAbout);//����
		}
		else
		{
			InsertMenu(hSubMenu, 19, MF_STRING | MF_BYPOSITION, uCmdID++, szMenuText_20);
			SetMenuItemBitmaps(hSubMenu, 19, MF_BYPOSITION, m_hBitmapAbout, m_hBitmapAbout);
		}
	}
	InsertMenu(hmenu, indexMenu, MF_STRING | MF_POPUP | MF_BYPOSITION, (UINT_PTR)hSubMenu, szMenuText_Popup);
	SetMenuItemBitmaps(hmenu, indexMenu, MF_BYPOSITION, m_hBitmapLogo, m_hBitmapLogo);
	indexMenu++;
	InsertMenu(hmenu, indexMenu, MF_SEPARATOR | MF_BYPOSITION, 0, NULL);
	indexMenu++;

	lstrcpynA(m_pszVerb, "protected_run", 32);
	lstrcpynW(m_pwszVerb, L"protected_run", 32);

	return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, uCmdID-idCmdFirst);
}

HRESULT STDMETHODCALLTYPE Cdllcontext::InvokeCommand(THIS_
														LPCMINVOKECOMMANDINFO lpici)
{
	BOOL fEx = FALSE;
	BOOL fUnicode = FALSE;

	if(lpici->cbSize == sizeof(CMINVOKECOMMANDINFOEX))
	{
		fEx = TRUE;
		if((lpici->fMask & CMIC_MASK_UNICODE))
		{
			fUnicode = TRUE;
		}
	}

	if( !fUnicode && HIWORD(lpici->lpVerb))
	{
		if(StrCmpIA(lpici->lpVerb, m_pszVerb))
		{
			return E_FAIL;
		}
	}

	else if( fUnicode && HIWORD(((CMINVOKECOMMANDINFOEX *) lpici)->lpVerbW))
	{
		if (StrCmpIW(((CMINVOKECOMMANDINFOEX *)lpici)->lpVerbW, m_pwszVerb))
		{
			return E_FAIL;
		}
		
	}
	else
	{
		switch ( LOWORD( lpici->lpVerb ))
		{
		case 22:
			{
			CString filePath("");
			size_t index = allFileName.size();

			//���ļ�����
			if (index == 1)
			{
				
				CString strFilePath(allFileName.front());
				strFilePath.Replace(_T("\\"), _T("/"));
				filePath.Append(strFilePath);
			}
			else
			{
			for (list<CString>::iterator itfile = allFileName.begin(); itfile != allFileName.end(); ++itfile)
				{
				CString strPathTemp(*itfile);
				strPathTemp.Replace(_T("\\"), _T("/"));
				filePath.Append(strPathTemp);
				filePath.Append(_T("\r\n"));
				}
			filePath.Delete(filePath.GetLength() - 2, 2);
			}
			//MessageBox(NULL, filePath, NULL, MB_OK);
			HANDLE hThread;
			ThreadData* plugData = new ThreadData;
			plugData->strFilePath = filePath;
			hThread = (HANDLE)_beginthreadex(NULL, 0, GetCopyPath, (void*)plugData, 0, NULL);
			CloseHandle(hThread);
			allFileName.clear();
			break;
			}
		case 23:
			{
			//MessageBox(NULL, _T("test1"), NULL, 0);
			//Dex->jar	
				for(list<CString>::iterator itfile = allFileName.begin(); itfile != allFileName.end(); ++itfile)  
				{
					HANDLE hThread;
					ThreadData* plugData = new ThreadData;
					plugData->strFilePath = *itfile;
					hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadDex2Jar, (void*)plugData, 0, NULL);
					CloseHandle(hThread);
				}

				allFileName.clear();

				break;
			}
		case 24:
			{
			//MessageBox(NULL, _T("test2"), NULL, 0);
			//manifest->txt
				size_t index = allFileName.size();

				//���ļ�����
				if (index==1)
				{
					//CString szFile;
					//szFile = allFileName.pop_front();
					HANDLE hThread;
					ThreadData* plugData = new ThreadData;
					plugData->strFilePath = allFileName.front();
					hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadMani2TxtSignal, (void*)plugData, 0, NULL);
					CloseHandle(hThread);
					allFileName.clear();
					break;
				}
				//��������
				for(list<CString>::iterator itfile = allFileName.begin(); itfile != allFileName.end(); ++itfile)  
				{
					HANDLE hThread;
					ThreadData* plugData = new ThreadData;
					plugData->strFilePath = *itfile;
					hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadMani2Txt, (void*)plugData, 0, NULL);
					CloseHandle(hThread);
				}
				allFileName.clear();
				break;
			}
		case 25:
			{
			//MessageBox(NULL, _T("test3"), NULL, 0);
				//�鿴apk��Ϣ
				for(list<CString>::iterator itfile = allFileName.begin(); itfile != allFileName.end(); ++itfile)  
				{
					HANDLE hThread;
					ThreadData* plugData = new ThreadData;
					plugData->strFilePath = *itfile;
					hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadApkInfor, (void*)plugData, 0, NULL);
					CloseHandle(hThread);
				}
				
				allFileName.clear();
				break;
			}
		case 26:
			{
			//MessageBox(NULL, _T("test4"), NULL, 0);
				//��ȡǩ����Ϣ
				for(list<CString>::iterator itfile = allFileName.begin(); itfile != allFileName.end(); ++itfile)  
				{
					HANDLE hThread;
					ThreadData* plugData = new ThreadData;
					plugData->strFilePath = *itfile;
					hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadGetSigninfo, (void*)plugData, 0, NULL);
					CloseHandle(hThread);
				}

				allFileName.clear();
				break;
			}
		case 27:
			{
			//ǩ��
			//MessageBox(NULL, _T("test5"), NULL, 0);
			for (list<CString>::iterator itfile = allFileName.begin(); itfile != allFileName.end(); ++itfile)
			{
				HANDLE hThread;
				ThreadData* plugData = new ThreadData;
				plugData->strFilePath = *itfile;
				hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadApkSign, (void*)plugData, 0, NULL);
				CloseHandle(hThread);
			}
			allFileName.clear();
			break;
			}
		case 28:
			{
			//��װ��ж�ذ�װ��
			//MessageBox(NULL, _T("test6"), NULL, 0);
			for (list<CString>::iterator itfile = allFileName.begin(); itfile != allFileName.end(); ++itfile)
			{
				HANDLE hThread;
				ThreadData* plugData = new ThreadData;
				plugData->strFilePath = *itfile;
				hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadApkInstall, (void*)plugData, 0, NULL);
				CloseHandle(hThread);
			}

			allFileName.clear();

			break;
			
			}
		case 29:
			{
			//MessageBox(NULL, _T("test7"), NULL, 0);
			//��װ���滻��װ��
			for (list<CString>::iterator itfile = allFileName.begin(); itfile != allFileName.end(); ++itfile)
			{
				HANDLE hThread;
				ThreadData* plugData = new ThreadData;
				plugData->strFilePath = *itfile;
				hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadApkInstallfore, (void*)plugData, 0, NULL);
				CloseHandle(hThread);
			}

			allFileName.clear();

			break;
			
			}
		case 30:
			{
			//MessageBox(NULL, _T("test8"), NULL, 0);
			//ж��
			for (list<CString>::iterator itfile = allFileName.begin(); itfile != allFileName.end(); ++itfile)
			{
				HANDLE hThread;
				ThreadData* plugData = new ThreadData;
				plugData->strFilePath = *itfile;
				hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadApkUninstall, (void*)plugData, 0, NULL);
				CloseHandle(hThread);
			}

			allFileName.clear();
			break;
			
			}
		case 31:
			{
			//MessageBox(NULL, _T("test9"), NULL, 0);
			//��ȡ�ӿ���Ϣ
			for (list<CString>::iterator itfile = allFileName.begin(); itfile != allFileName.end(); ++itfile)
			{
				HANDLE hThread;
				ThreadData* plugData = new ThreadData;
				plugData->strFilePath = *itfile;
				hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadGetWrapperInfo, (void*)plugData, 0, NULL);
				CloseHandle(hThread);
			}

			allFileName.clear();
			break;
			}
		case 32:
		{
			for (list<CString>::iterator itfile = allFileName.begin(); itfile != allFileName.end(); ++itfile)
			{
				HANDLE hThread;
				ThreadData* plugData = new ThreadData;
				plugData->strFilePath = *itfile;
				hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadDeepDetect, (void*)plugData, 0, NULL);
				CloseHandle(hThread);
			}

			allFileName.clear();
			break;
		}
		case 33:
			{
			//MessageBox(NULL, _T("test10"), NULL, 0);
			//��ȡ�ֻ���Ϣ
			for (list<CString>::iterator itfile = allFileName.begin(); itfile != allFileName.end(); ++itfile)
			{
				HANDLE hThread;
				ThreadData* plugData = new ThreadData;
				plugData->strFilePath = *itfile;
				hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadGetPhoneInfo, (void*)plugData, 0, NULL);
				CloseHandle(hThread);
			}

			allFileName.clear();
			break;
			}
		case 34:
			{
			//MessageBox(NULL, _T("test11"), NULL, 0);
			//�ֻ���ͼ
			for (list<CString>::iterator itfile = allFileName.begin(); itfile != allFileName.end(); ++itfile)
			{
				//MessageBox(NULL,_T("�ֻ���ͼ"),NULL,0);
				HANDLE hThread;
				ThreadData *plugData = new ThreadData;
				plugData->strFilePath = *itfile;
				hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadScreenCap, (void*)plugData, 0, NULL);
				CloseHandle(hThread);
			}

			allFileName.clear();
			break;
			}
		case 35:
			{
			//MessageBox(NULL, _T("test12"), NULL, 0);
			//��ȡͼ��
			for (list<CString>::iterator itfile = allFileName.begin(); itfile != allFileName.end(); ++itfile)
			{
				HANDLE hThread;
				ThreadData* plugData = new ThreadData;
				plugData->strFilePath = *itfile;
				hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadGetExtraIcon, (void*)plugData, 0, NULL);
				CloseHandle(hThread);
			}

			allFileName.clear();
			break;
			}
		case 36:
			{
			//MessageBox(NULL, _T("test13"), NULL, 0);
			//zipalign�Ż�
			for (list<CString>::iterator itfile = allFileName.begin(); itfile != allFileName.end(); ++itfile)
			{
				//MessageBox(NULL,_T("zipalign�Ż�"),NULL,0);
				HANDLE hThread;
				ThreadData* plugData = new ThreadData;
				plugData->strFilePath = *itfile;
				hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadApkAlign, (void*)plugData, 0, NULL);
				CloseHandle(hThread);
			}

			allFileName.clear();
			break;
			}
		case 37:
			{
			//MessageBox(NULL, _T("test14"), NULL, 0);
				//������
				for(list<CString>::iterator itfile = allFileName.begin(); itfile != allFileName.end(); ++itfile)  
				{
					HANDLE hThread;
					ThreadData* plugData = new ThreadData;
					plugData->strFilePath = *itfile;
					hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadDeApk, (void*)plugData, 0, NULL);
					CloseHandle(hThread);
				}

				allFileName.clear();
				break;
			}
		case 38:
			{
			//MessageBox(NULL, _T("test15"), NULL, 0);
				//�ر���
				for(list<CString>::iterator itfile = allFileName.begin(); itfile != allFileName.end(); ++itfile)  
				{
					HANDLE hThread;
					ThreadData* plugData = new ThreadData;
					plugData->strFilePath = *itfile;
					hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadComApk, (void*)plugData, 0, NULL);
					CloseHandle(hThread);
				}

				allFileName.clear();
				break;
			}
		case 39:
		{
			//��ȡLIB
			for (list<CString>::iterator itfile = allFileName.begin(); itfile != allFileName.end(); ++itfile)
			{
				HANDLE hThread;
				ThreadData* plugData = new ThreadData;
				plugData->strFilePath = *itfile;
				hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadGetExtraLib, (void*)plugData, 0, NULL);
				CloseHandle(hThread);
			}

			allFileName.clear();
			break;
		}
		case 40:
		{
			//lib�淶�Լ��
			for (list<CString>::iterator itfile = allFileName.begin(); itfile != allFileName.end(); ++itfile)
			{
				HANDLE hThread;
				ThreadData* plugData = new ThreadData;
				plugData->strFilePath = *itfile;
				hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadIsRegular, (void*)plugData, 0, NULL);
				CloseHandle(hThread);
			}

			allFileName.clear();
			break;
		}
		default:
			{
			//MessageBox(NULL, _T("test16"), NULL, 0);
			if (LOWORD(lpici->lpVerb) - 41 == m_CustomMenu.size())
				{
					//����
					HANDLE hThread;
					hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadAbout, NULL, 0, NULL);
					CloseHandle(hThread);
					allFileName.clear();
					break;
				}
				else if (LOWORD(lpici->lpVerb) > 40 && LOWORD(lpici->lpVerb) <= m_CustomMenu.size() + 40)
				{
					CString strIndex;
					strIndex.Format(_T("%d"), LOWORD(lpici->lpVerb) - 40);
					//�Զ�����
					for (list<CString>::iterator itfile = allFileName.begin(); itfile != allFileName.end(); ++itfile)
					{
						HANDLE hThread;
						ThreadData* plugData = new ThreadData;
						plugData->strFilePath = *itfile;
						plugData->strPlugIndex = strIndex;
						
						hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadCustomPlugFunc, (void*)plugData, 0, NULL);
						CloseHandle(hThread);
					}
					allFileName.clear();
					break;
				}
				return E_INVALIDARG;
				break;
			}
		}

		//�ڴ˴������¼�.
		return S_OK;

	}

	return E_FAIL;
}

HRESULT STDMETHODCALLTYPE Cdllcontext::GetCommandString(THIS_
															 UINT_PTR    idCmd,
															 UINT        uType,
															 UINT      * pwReserved,
															 LPSTR       pszName,
															 UINT        cchMax)
{
	USES_CONVERSION;
	LPCTSTR szPrompt;
	if ( uType & GCS_HELPTEXT )
	{
		switch ( idCmd )
		{
		case 0:
			szPrompt = _T("����·��");
			break;
		case 1:
			szPrompt = _T("DEX->JAR");
			break;
		case 2:
			szPrompt = _T("manifest->txt");
			break;
		/*case 2:
			szPrompt = _T("AXML->XML");*/
		case 3:
			szPrompt = _T("�鿴apk��Ϣ");
			break;
		case 4:
			szPrompt = _T("�鿴ǩ����Ϣ");
			break;
		case 5:
			szPrompt = _T("ǩ��");
			
			break;
		case 6:
			szPrompt = _T("ж�ذ�װ");
			
			break;
		case 7:
			szPrompt = _T("�滻��װ");
			
			break;
		case 8:
			szPrompt = _T("ж��");
			
			break;
		case 9:
			szPrompt = _T("�鿴�ӿ���Ϣ");
			break;
		case 10:
			szPrompt = _T("APK��ȼ��");
			
			break;
		case 11:
			szPrompt = _T("�鿴�ֻ���Ϣ");
			break;
		case 12:
			szPrompt = _T("�ֻ���ͼ");
			break;
		case 13:
			szPrompt = _T("��ȡͼ��");
			break;
		case 14:
			szPrompt = _T("zipalign�Ż�");
			break;
		case 15:
			szPrompt = _T("������");
			break;
		case 16:
			szPrompt = _T("�ر���");
			break;
		case 17:
			szPrompt = _T("��ȡso");
			break;
		case 18:
			szPrompt = _T("lib�淶�Լ��");
			break;
		default:
			ATLASSERT(0);           // should never get here
			if (idCmd - 19 == m_CustomMenu.size())
			{
				szPrompt = _T("���� v1.2");
			}
			else if (idCmd > 18 && idCmd <= m_CustomMenu.size() + 18)
			{
				szPrompt = _T("�Զ�����");
				break;
			}
			return E_INVALIDARG;
			break;
		}      

		if ( uType & GCS_UNICODE )
		{
			 //������Ҫ�� pszName ת��Ϊһ�� Unicode �ַ���, ����ʹ��Unicode�ַ������� API.
			lstrcpynW ( (LPWSTR) pszName, T2CW(szPrompt), cchMax );
		}
		else
		{
			 //ʹ�� ANSI �ַ�������API �����ذ����ַ���.
			lstrcpynA ( pszName, T2CA(szPrompt), cchMax );
		}
		return S_OK;
	}
	return E_INVALIDARG;
}

