// SSHToolDlg.h : header file
//

#if !defined(AFX_SSHTOOLDLG_H__845DDA18_06BE_4032_997A_DE70CAE360E2__INCLUDED_)
#define AFX_SSHTOOLDLG_H__845DDA18_06BE_4032_997A_DE70CAE360E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ssh_common.h"

#define WM_LOG (WM_USER + 100 )
/////////////////////////////////////////////////////////////////////////////
// CSSHToolDlg dialog

class CSSHToolDlg;
struct SFTP_Param
{
	CString strIP;
	CString strUser;
	CString strPwd;
	CString strLocalFile;
	CString strDestFile;
	CString strCmdText;
	class CSSHToolDlg* pDlg;
};

class CSSHToolDlg : public CDialog
{
// Construction
public:
	CSSHToolDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSSHToolDlg)
	enum { IDD = IDD_SSHTOOL_DIALOG };
	CProgressCtrl	m_sftpProgress;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSSHToolDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	public:
		CString m_strUser;
		CString m_strPwd;
		CString m_strIP;

		int GetConnectInfo();
		void WriteLog(CString* strText);
		int ConnectSSH(char* ip,char* usr,char* pass,char* cmd );
		int ConnectSFTP(char* ip,char* usr,char* pass,char* src_file,char*dest_file );
		static DWORD WINAPI SftpThreadProc(LPVOID pParam);
		static DWORD WINAPI PingThreadProc(LPVOID pParam);
		static DWORD WINAPI ShellThreadProc(LPVOID pParam);
		static DWORD WINAPI DosShellThreadProc(LPVOID pParam);
		BOOL ExecDosCmd(LPCTSTR szCommandline) ;
		BOOL Ping(LPCTSTR szTarget, DWORD &dwTime);
		CWinThread* mSftpThread;
		CWinThread* mPingThread;
		CWinThread* mShellThread;
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSSHToolDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonConnect();
	afx_msg void OnButtonExec();
	afx_msg void OnButtonCls();
	afx_msg void OnButtonFtp();
	afx_msg void OnButtonSelectSrcFile();
	afx_msg void OnButtonPing();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SSHTOOLDLG_H__845DDA18_06BE_4032_997A_DE70CAE360E2__INCLUDED_)
