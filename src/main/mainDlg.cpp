// mainDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "main.h"
#include "mainDlg.h"
#include <Shlwapi.h>
#include <tlhelp32.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CmainDlg �Ի���




CmainDlg::CmainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CmainDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CmainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CmainDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CmainDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CmainDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CmainDlg::OnBnClickedButton1)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

CStringA RunCmdAndOutput(LPCTSTR lpszCmdLine, DWORD dwMilliseconds = INFINITE)
{
	SECURITY_ATTRIBUTES sa = { 0 };
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = true;
	sa.lpSecurityDescriptor = NULL;
	HANDLE hReadPipe, hWritePipe;
	if (!CreatePipe(&hReadPipe, &hWritePipe, &sa, NULL))
		return "Create pipe failed!";

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si);
	si.hStdError = hWritePipe;
	si.hStdOutput = hWritePipe;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE;
	if (CreateProcess(NULL, (LPTSTR)(LPCTSTR)lpszCmdLine, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi) == 0)
	{
		DWORD dwError = GetLastError();
		CloseHandle(hWritePipe);
		CloseHandle(hReadPipe);
		return "CreateProcess failed!";
	}
	CloseHandle(hWritePipe);

	const int BUF_LEN = 1024;
	char buffer[BUF_LEN + 1] = { 0 };
	DWORD dwRead = 0;
	CStringA strLog;
	while (ReadFile(hReadPipe, buffer, BUF_LEN * sizeof(BYTE), &dwRead, NULL) != NULL)
	{
		strLog.AppendFormat("%s", buffer);
		RtlZeroMemory(buffer, BUF_LEN);
	}
	CloseHandle(hReadPipe);
	DWORD ret = WaitForSingleObject(pi.hProcess, dwMilliseconds);
	if (ret == WAIT_TIMEOUT)
		TerminateProcess(pi.hProcess, 0);
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	return strLog;
}
// CmainDlg ��Ϣ�������

BOOL CmainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_brush.CreateSolidBrush(RGB(255, 255, 255));//��ˢΪ��ɫ
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CmainDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

BOOL CmainDlg::CreateDirRecursively(const CString&strRoot, const CString&strSubDir)
{
	BOOL ret = TRUE;
	CString strTemp;
	CString strPath = strSubDir;
	PathWithSlash(strPath);

	int nPos = 0;
	int nStart = strRoot.GetLength();
	while (nStart < strPath.GetLength()) {
		nPos = strPath.Find('\\', nStart);
		if (nPos == -1) {
			break;
		}

		strTemp = strPath.Left(nPos);
		if (GetFileAttributes(strTemp) == -1) {
			if (!CreateDirectory(strTemp, NULL))
			{
				ret = FALSE;
				break;
			}
		}
		nStart = nPos + 1;
	}

	return ret;
}

void CmainDlg::PathWithSlash(CString&strPath)
{
	if (strPath.GetLength()>1) {
		strPath.Replace('/', '\\');
		if (strPath.Right(1) != "\\") {
			strPath += "\\";
		}
	}
}
BOOL CmainDlg::TraverseFolderAndMFile(LPCTSTR lpSrcPath, LPCTSTR lpszDesPath)
{
	BOOL ret = TRUE;
	HANDLE	hFindHandle = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA Win32FindData = { 0 };
	CString		strOldFilePath;
	CString     strNewFilePath;

	strOldFilePath = lpSrcPath;
	strNewFilePath = lpszDesPath;

	PathWithSlash(strOldFilePath);
	PathWithSlash(strNewFilePath);

	if (_taccess(strOldFilePath, 0) != 0)
	{
		ret = FALSE;
		goto fun_ret;
	}

	hFindHandle = FindFirstFile(strOldFilePath + _T("*"), &Win32FindData);
	if (hFindHandle == INVALID_HANDLE_VALUE)
	{
		ret = FALSE;
		goto fun_ret;
	}

	if (!(_tcslen(Win32FindData.cFileName) == 1 && _tcsncmp(Win32FindData.cFileName, _T("."), 1) == 0)
		&& !(_tcslen(Win32FindData.cFileName) == 2 && _tcsncmp(Win32FindData.cFileName, _T(".."), 2) == 0))
	{
		if (!(Win32FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			//DeleteFile(strNewFilePath + Win32FindData.cFileName);
			if (!CopyFile(strOldFilePath + Win32FindData.cFileName, strNewFilePath + Win32FindData.cFileName, FALSE))
			{
				ret = FALSE;
				goto fun_ret;
			}
		}
		else if (Win32FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			ret = CreateDirRecursively(strNewFilePath, strNewFilePath+Win32FindData.cFileName);
			if (!ret)
				goto fun_ret;
			ret = TraverseFolderAndMFile(strOldFilePath + Win32FindData.cFileName, strNewFilePath+Win32FindData.cFileName);
			if (!ret)
				goto fun_ret;
		}
	}

	while (FindNextFile(hFindHandle, &Win32FindData))
	{
		if (!(_tcslen(Win32FindData.cFileName) == 1 && _tcsncmp(Win32FindData.cFileName, _T("."), 1) == 0)
			&& !(_tcslen(Win32FindData.cFileName) == 2 && _tcsncmp(Win32FindData.cFileName, _T(".."), 2) == 0))
		{
			if (!(Win32FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				//DeleteFile(strNewFilePath + Win32FindData.cFileName);
				if (!CopyFile(strOldFilePath + Win32FindData.cFileName, strNewFilePath + Win32FindData.cFileName, FALSE))
				{
					ret = FALSE;
					goto fun_ret;
				}
			}
			else if (Win32FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				ret = CreateDirRecursively(strNewFilePath, strNewFilePath + Win32FindData.cFileName);
				if (!ret)
					goto fun_ret;
				ret = TraverseFolderAndMFile(strOldFilePath + Win32FindData.cFileName, strNewFilePath + Win32FindData.cFileName);
				if (!ret)
					goto fun_ret;
			}
		}
	}


fun_ret:
	if (hFindHandle != INVALID_HANDLE_VALUE)
		FindClose(hFindHandle);

	//CCommon::DeleteDirectory(strOldFilePath);
	return ret;
}

BOOL CmainDlg::CreateInstallPath()
{
	BOOL ret = FALSE;
	TCHAR szModulePath[MAX_PATH]; 
	CString strSrcPath;
	GetCurrentDirectory(MAX_PATH,szModulePath);
	strSrcPath = szModulePath;
	strSrcPath.Replace('/', '\\');
	CString strDllPath = _T("D:\\APKmenuTOOL\\ApkmenuTool.dll");

	CString strDesPath = _T("D:\\APKmenuTOOL\\");
	strSrcPath.Replace('\\', '/');

	if (strSrcPath.Find(_T("D:/APKmenuTOOL")) != -1)
	{
		AfxMessageBox(_T("������D:\\APKmenuTOOL�°�װ���뻻Ŀ¼"));
		return FALSE;
	}
	CString strCmdLine("cmd /c D:");
	CStringA strOutPut = RunCmdAndOutput(strCmdLine);
	//AfxMessageBox(out);
	if (strOutPut.Find("ϵͳ�Ҳ���ָ����������") != -1)
	{
		strDesPath.Replace('D', 'C');
		//AfxMessageBox(strDesPath);
	}
	if (_taccess_s(strDesPath, 0) != 0)
	{
		if (CreateDirectory(strDesPath, NULL))
			ret = TraverseFolderAndMFile(strSrcPath, strDesPath);
	}
	else
	{
		if (_taccess_s(strDllPath, 0) == 0)
		{
			if (!DeleteFile(strDllPath))
			{
				AfxMessageBox(_T("��ж�غ��ٰ�װ"));
				return FALSE;
			}
			DeleteDirectory(strDesPath + _T("tools"));
			if (_taccess_s(strDesPath + _T("tools"), 0) == 0)
			{
				AfxMessageBox(strDesPath + _T("tools�ļ����в��ֳ������ڱ�ʹ�ã��޷���װ����رպ����ԣ�"));
				return FALSE;
			}
		}
		ret = TraverseFolderAndMFile(strSrcPath, strDesPath);
	}

	if (!ret)
	{
		CString strOutput;
		GetErrorMsg(strOutput);
		AfxMessageBox(_T("��װʧ�ܣ�ԭ�� ") + strOutput);
	}
	
	return ret;
}
void CmainDlg::GetErrorMsg(CString& outPut)
{
	LPVOID lpMsgBuf = NULL;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	outPut.Format(_T("%s"), lpMsgBuf);

	if (lpMsgBuf)
		LocalFree(lpMsgBuf);
}

void CmainDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR szModulePath[MAX_PATH]; 
	CString strCmdLine = _T("");
	UINT reInt = 0;
	GetCurrentDirectory(MAX_PATH,szModulePath);
	if (CreateInstallPath())
	{
		strCmdLine = _T("regsvr32 ");
		strCmdLine += _T("D:\\APKmenuTOOL\\ApkmenuTool.dll");
		RunCMD(strCmdLine, INFINITE);

		RestartExplore();

	}

	OnOK();
}


void CmainDlg::RestartExploreOld()
{
	char strShell[1024];
    SHELLEXECUTEINFOA shellExeInfo = {0};
    shellExeInfo.cbSize = sizeof(SHELLEXECUTEINFOA);
    shellExeInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    shellExeInfo.nShow = SW_HIDE;
    shellExeInfo.lpVerb = "open";
     
    GetSystemDirectoryA(strShell,1024);
    ::PathAppendA(strShell,"taskkill.exe");
    shellExeInfo.lpFile = strShell;
     
    shellExeInfo.lpParameters = "/F /IM explorer.exe";
     
    ShellExecuteExA(&shellExeInfo);
     
    WaitForSingleObject(shellExeInfo.hProcess,INFINITE);
     
    GetWindowsDirectoryA(strShell,1024);
    ::PathAppendA(strShell,"explorer.exe");
    WinExec(strShell,SW_SHOW);
	//CStringA strCmdLine = strShell;
	//RunCMD(szCmdLine, INFINITE);
}

void CmainDlg::LaunchExplorer() {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));

	si.cb = sizeof(si);

	CreateProcess(_T("explorer.exe"), NULL, NULL, NULL, false, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
}

void CmainDlg::RestartExplore()
{
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (snapshot == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("����explorer����ʧ�ܣ����ֶ�����"));
		return;
	}

	BOOL find = FALSE;

	if (Process32First(snapshot, &entry) == TRUE) {
		while (Process32Next(snapshot, &entry) == TRUE) {
			if (_tcscmp(entry.szExeFile, _T("explorer.exe")) == 0) {
				//AfxMessageBox(_T("TEST"));
				HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_TERMINATE, FALSE, entry.th32ProcessID);

				TerminateProcess(hProcess, 0);

				CloseHandle(hProcess);
				find = TRUE;
				break;
			}
		}
		if (find)
		{
			LaunchExplorer();
		}
	}
}


BOOL CmainDlg::RunCMD( LPCTSTR lpszCmdLine, DWORD dwMilliseconds /*= INFINITE */ )
{
	CString strCmdLine = lpszCmdLine;
	STARTUPINFO si = { sizeof(si) };
	GetStartupInfo(&si);
	si.dwFlags=STARTF_USESHOWWINDOW; 
	si.wShowWindow = SW_SHOW;
	PROCESS_INFORMATION pi;

	BOOL bSuccess = ::CreateProcess(NULL, (LPTSTR)(LPCTSTR)strCmdLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);
	if (!bSuccess)
		_tprintf_s(_T("CreateProcess failed. lpszCmdLine is %s. error code is %d\r\n"), lpszCmdLine, GetLastError());
	else
	{
		CloseHandle(pi.hThread);
		if (WAIT_TIMEOUT == WaitForSingleObject(pi.hProcess, dwMilliseconds))
			TerminateProcess(pi.hProcess, 0);
		CloseHandle(pi.hProcess);
	}
	return bSuccess;
}


void CmainDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR szModulePath[MAX_PATH]; 
	CString strCmdLine = _T("");
	UINT reInt = 0;
	GetCurrentDirectory(MAX_PATH,szModulePath);


	//Integrity::SetProcessIntegrityLevel(Integrity::INTEGRITY_LEVEL_MEDIUM);

	if (_taccess_s(_T("D:\\APKmenuTOOL\\APKmenuTool.dll"), 0) == 0)
	{
		strCmdLine = _T("regsvr32 /u ");
		strCmdLine += _T("D:\\APKmenuTOOL\\APKmenuTool.dll");

		RunCMD(strCmdLine, INFINITE);

		RestartExplore();
		
		/*Sleep(400);
		DeleteFile(_T("D:\\apktools\\Apktoollib.dll"));*/
		//DeleteDirectory(_T("D:\\apktools"));
	}
	else
	{
		AfxMessageBox(_T("����δ��װ��"));
	}
}


void CmainDlg::DeleteDirectory(LPCTSTR lpszDirPath)
{
	if (lpszDirPath == NULL || GetFileAttributes(lpszDirPath) == -1) {
		return;
	}

	CString strDirPath = lpszDirPath;
	PathWithSlash(strDirPath);

	WIN32_FIND_DATA stFindData = { 0 };
	HANDLE hFind = ::FindFirstFile(strDirPath + _T("*.*"), &stFindData);
	if (hFind != INVALID_HANDLE_VALUE) {

		do
		{
			if (stFindData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY) {
				if (stFindData.cFileName[0] != '.') {
					DeleteDirectory(strDirPath + stFindData.cFileName);
				}
			}
			else {
				::DeleteFile(strDirPath + stFindData.cFileName);
			}
		} while (::FindNextFile(hFind, &stFindData));

		::FindClose(hFind);
	}

	::RemoveDirectory(lpszDirPath);
}

HBRUSH CmainDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����  IDC_STATIC_TIPS

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_TIPS)
	{
		pDC->SetTextColor(RGB(255, 0, 0));//����Ϊ��ɫ
		return m_brush;
	}
	return hbr;
}
