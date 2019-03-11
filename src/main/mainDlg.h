// mainDlg.h : ͷ�ļ�
//

#pragma once


// CmainDlg �Ի���
class CmainDlg : public CDialog
{
// ����
public:
	CmainDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MAIN_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	CBrush m_brush;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	void RestartExploreOld();
	BOOL RunCMD(LPCTSTR lpszCmdLine, DWORD dwMilliseconds = INFINITE );
	BOOL CreateInstallPath();
	BOOL CreateDirRecursively(const CString&strRoot, const CString&strSubDir);
	void PathWithSlash(CString&strPath);
	BOOL TraverseFolderAndMFile(LPCTSTR lpSrcPath, LPCTSTR lpszDesPath);
	void DeleteDirectory(LPCTSTR lpszDirPath);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();

	void LaunchExplorer();
	void RestartExplore();
	void GetErrorMsg(CString & outPut);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
