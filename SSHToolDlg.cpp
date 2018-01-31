// SSHToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SSHTool.h"
#include "SSHToolDlg.h"
#include <iostream>
#include <string>
#include <fcntl.h>
#include "ssh_common.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DATALEN 65536

// 判断是否是IP地址
int CommonLib_IsValidIP(char* pstr )
{
	char buf[20]={0};
	int i,k;
	int startIndex = 0, endIndex = 0;
	int length = 0;
    char* pstrTable = "0123456789.";
	if( pstr == NULL )
	{
		return 0;
	}
	i = 0;
	while( pstr[i] != '\0' )
	{
		if( pstr[i]== '\n' || pstr[i] == '\r' || pstr[i] == ' ' || pstr[i] == '\t' )
		{
			i++;
		}
		else
		{
			startIndex = i;
			break;
		}
	}
	i = startIndex;
	while( pstr[i] != '\0')
	{
		endIndex = i;
		if( pstr[i]== '\n' || pstr[i] == '\r' || pstr[i] == ' ' || pstr[i] == '\t' )
		{
			break;
		}
		else
		{
			i++;
		}
	}
	length = endIndex - startIndex;

	if( length >= (sizeof(buf)/sizeof(buf[0]) - 1))
	{
		return 0;
	}


	i = 0;
	while(pstr[startIndex + i] != '\0' )
	{
		buf[i] = pstr[startIndex + i];
		if( i == (endIndex - startIndex) )
		{
			buf[i+1] = '\0';
			break;
		}
		i++;
	}

	length = 0;
	while ( buf[length] != '\0')
	{
		length++;
	}

	if( buf[0] == '.' || buf[length-1] == '.')
	{
		return 0;
	}

	i = 0;
	k = 0;
	while( buf[i] != '\0')
	{
		if( buf[i] == '.' )
		{
			k++;
		}
		i++;
		if( buf[i-1] == '.' && buf[i] == '.' )
		{
			return 0;
		}
	}

	if( k != 3 )
	{
		return 0;
	}


    k = 0;
	for( i = 0; buf[i] != '\0';i++ )
	{
		for( k = 0; pstrTable[k] != '\0';k++ )
		{
			if(buf[i] == pstrTable[k])
			{
				break;
			}
		}
		if( pstrTable[k] == '\0')
		{
			return 0;
		}
	}


	return 1;

}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSSHToolDlg dialog

CSSHToolDlg::CSSHToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSSHToolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSSHToolDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSSHToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSSHToolDlg)
	DDX_Control(pDX, IDC_PROGRESS1, m_sftpProgress);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSSHToolDlg, CDialog)
	//{{AFX_MSG_MAP(CSSHToolDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, OnButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON_EXEC, OnButtonExec)
	ON_BN_CLICKED(IDC_BUTTON_CLS, OnButtonCls)
	ON_BN_CLICKED(IDC_BUTTON_FTP, OnButtonFtp)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_SRC_FILE, OnButtonSelectSrcFile)
	ON_BN_CLICKED(IDC_BUTTON_PING, OnButtonPing)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSSHToolDlg message handlers

BOOL CSSHToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	m_sftpProgress.SetRange(0,100);
	m_sftpProgress.SetPos(0);

	mShellThread=mPingThread = mSftpThread = NULL;

	GetDlgItem(IDC_EDIT_USR)->SetWindowText("");
	
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSSHToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSSHToolDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSSHToolDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

int CSSHToolDlg::GetConnectInfo()
{
	GetDlgItem(IDC_EDIT_IP)->GetWindowText(m_strIP);
	GetDlgItem(IDC_EDIT_USR)->GetWindowText(m_strUser);
	GetDlgItem(IDC_EDIT_PWD)->GetWindowText(m_strPwd);
	
	m_strIP.TrimLeft();
	m_strIP.TrimRight();
	
	m_strUser.TrimLeft();
	m_strUser.TrimRight();
	
	m_strPwd.TrimLeft();
	m_strPwd.TrimRight();
	
	if( m_strIP.IsEmpty())
	{
		return 0;
	}
	if( m_strUser.IsEmpty())
	{
		return 0;
	}
	if( m_strPwd.IsEmpty())
	{
		return 0;
	}

	if( !CommonLib_IsValidIP(m_strIP.GetBuffer(0)) )
	{
		AfxMessageBox("IP地址格式不正确.");
		GetDlgItem(IDC_EDIT_IP)->SetFocus();
		return 0;
	}

	

	return 1;
}


void CSSHToolDlg::OnButtonConnect() 
{
	CString strCmdText="cd /;pwd";
	
	
	if ( strCmdText.IsEmpty())
	{
		return;
	}
	
	if( !GetConnectInfo() )
	{
		return;
	}
	WriteLog(new CString("\r\n\r\nSSH链接测试...\r\n\r\n"+ strCmdText+ "\r\n"));
    ConnectSSH(m_strIP.GetBuffer(0),m_strUser.GetBuffer(0),m_strPwd.GetBuffer(0),strCmdText.GetBuffer(0));
}


int CSSHToolDlg::ConnectSFTP(char* ip,char* usr,char* pass,char* src_file,char*dest_file )
{
	ssh_session session;
    sftp_file to;
	sftp_session sftp;
	int count,i;
	char data[DATALEN]={0};
    session = connect_ssh(ip, usr,pass, 0);
    if (session == NULL) {
        ssh_finalize();
        return 1;
    }

	sftp=sftp_new(session);
	if ( !sftp)
	{
		fprintf(stderr, "sftp error initialising channel: %s\n",
            ssh_get_error(session));
		return 1;
	}
	if(sftp_init(sftp))
	{
		fprintf(stderr, "error initialising sftp: %s\n",
            ssh_get_error(session));
		return 1;
	}
	printf("Additional SFTP extensions provided by the server:\n");
    count = sftp_extensions_get_count(sftp);
    for (i = 0; i < count; i++) {
		printf("\t%s, version: %s\n",
			sftp_extensions_get_name(sftp, i),
			sftp_extensions_get_data(sftp, i));
    }

	to=sftp_open(sftp,dest_file,O_WRONLY|O_CREAT, 0644);
	if(!to){
        fprintf(stderr, "Error opening ssh-copy for writing: %s\n",
            ssh_get_error(session));
        return 1;
    }

	// 打开源文件，准备传输
	FILE* stream = fopen(src_file,"rb");
	if ( stream== NULL)
	{
		sftp_close(to);
		sftp_free(sftp);
		return -1;
	}

	fseek (stream, 0, SEEK_END);   ///将文件指针移动文件结尾
    long file_size=ftell (stream); ///求出当前文件指针距离文件开始的字节数
    fseek (stream, 0, SEEK_SET);

	long bytesCount = 0;

	while( !feof( stream ))
	{

		int rdlen = fread(data,1,DATALEN,stream);
		if( ferror( stream ) )     
		{
			CString * pStr = new CString("sftp:fread ferror \r\n");
			WriteLog(pStr);
			fclose(stream);
			stream=NULL;
			return -1;
		}
		if ( rdlen > 0 )
		{
			int writelen =sftp_write(to,data,rdlen);
			if ( writelen != rdlen )
			{
				CString * pStr = new CString("sftp:sftp_write ferror \r\n");
			    WriteLog(pStr);
				return -1;
			}
			ASSERT(writelen>0);

			bytesCount+= writelen;

			m_sftpProgress.SetPos(((double)bytesCount / (double)file_size)*100);
		}
	}

	fclose(stream);
	stream=NULL;


    sftp_close(to);
	
    /* close the sftp session */
    sftp_free(sftp);

	if ( file_size == bytesCount )
	{
		WriteLog(new CString("sftp transfer complete\r\n"));
	}
	else
	{
		WriteLog(new CString("sftp transfer not complete\r\n"));
	}
	WriteLog(new CString("sftp session terminated\r\n"));

    printf("sftp session terminated\n");

	
	return 0;
}


int CSSHToolDlg::ConnectSSH(char* ip,char* usr,char* pass,char* cmd ) {
    ssh_session session;
    ssh_channel channel;
    char buffer[256];
    int nbytes;
    int rc;
	int total = 0;
    session = connect_ssh(ip, usr,pass, 0);
    if (session == NULL) {
        ssh_finalize();
        return 1;
    }
	
    channel = ssh_channel_new(session);;
    if (channel == NULL) {
        ssh_disconnect(session);
        ssh_free(session);
        ssh_finalize();
        return 1;
    }
	
    rc = ssh_channel_open_session(channel);
    if (rc < 0) {
        goto failed;
    }
	
    rc = ssh_channel_request_exec(channel, cmd);
    if (rc < 0) {
        goto failed;
    }
	
	
	memset(buffer,0,sizeof(buffer));
    nbytes = ssh_channel_read(channel, buffer, sizeof(buffer)-1, 0);
    while (nbytes > 0) {
        //if (fwrite(buffer, 1, nbytes, stdout) != (unsigned int) nbytes) {
          //  goto failed;
        //}
		int i = 0;
		CString* pstrStringShow = new CString();
	//	CString* pstrString = new CString(buffer);
		for ( i = 0; i <  nbytes ; i++ )
		{
			
			if ( buffer[i] == '\x0A')
			{
                *pstrStringShow += "\r\n";
			}
			else
			{
				char buf[2]={0};
				buf[0]=buffer[i];
				*pstrStringShow += buf;
			}
		}




		WriteLog(pstrStringShow);

		total+= nbytes;
		
		memset(buffer,0,sizeof(buffer));
        nbytes = ssh_channel_read(channel, buffer, sizeof(buffer)-1, 0);
    }

	if ( total > 0 )
	{
		CString* pstrString = new CString("\r\n");
		WriteLog(pstrString);
	}
	
	
    if (nbytes < 0) {
        goto failed;
    }
	
    ssh_channel_send_eof(channel);
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    ssh_disconnect(session);
    ssh_free(session);
    ssh_finalize();
	
    return 0;
failed:
    ssh_channel_close(channel);
    ssh_channel_free(channel);
    ssh_disconnect(session);
    ssh_free(session);
    ssh_finalize();
	
    return 1;
}


// 执行ssh命令
void CSSHToolDlg::OnButtonExec() 
{

	SFTP_Param * param = new SFTP_Param;
	CString strCmdText;
	GetDlgItem(IDC_EDIT_CMD)->GetWindowText(strCmdText);
	strCmdText.TrimLeft();
	strCmdText.TrimRight();

	if ( strCmdText.IsEmpty())
	{
		return;
	}

	if( !GetConnectInfo() )
	{
		param->strCmdText = strCmdText;
		param->pDlg =this;
	    AfxBeginThread((AFX_THREADPROC)DosShellThreadProc,param,THREAD_PRIORITY_IDLE);
		return;
	}

	param->strIP = m_strIP;
	param->strUser =m_strUser;
	param->strPwd = m_strPwd;
	param->strLocalFile = "";
	param->strDestFile = "";
	param->strCmdText = strCmdText;
	param->pDlg =this;
	
	if( mShellThread != NULL )
	{

	}

	mShellThread = AfxBeginThread((AFX_THREADPROC)ShellThreadProc,param,THREAD_PRIORITY_IDLE);

}

void CSSHToolDlg::WriteLog(CString* pstrText)
{
	PostMessage(WM_LOG,(WPARAM)pstrText,(LPARAM)0);
} 

BOOL CSSHToolDlg::PreTranslateMessage(MSG* pMsg) 
{
	if ( pMsg->message == WM_LOG )
	{
		CString* pLogText = (CString* )pMsg->wParam;
		CString strTextAll;
		GetDlgItem(IDC_EDIT_CONSOLE)->GetWindowText(strTextAll);

		strTextAll += *pLogText;
		GetDlgItem(IDC_EDIT_CONSOLE)->SetWindowText(strTextAll);

		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_CONSOLE);
		int nLineCount = pEdit->GetLineCount();
		pEdit->LineScroll(nLineCount,0);


		delete pLogText;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CSSHToolDlg::OnButtonCls() 
{

	GetDlgItem(IDC_EDIT_CONSOLE)->SetWindowText("");
	
}



DWORD WINAPI CSSHToolDlg::ShellThreadProc(LPVOID pParam)
{
	SFTP_Param * param = (SFTP_Param*)pParam;
	
	
	param->pDlg->ConnectSSH(param->strIP.GetBuffer(0),param->strUser.GetBuffer(0),param->strPwd.GetBuffer(0),param->strCmdText.GetBuffer(0));
	
	
	delete param;
	return 0;
}


DWORD WINAPI CSSHToolDlg::PingThreadProc(LPVOID pParam)
{
	SFTP_Param * param = (SFTP_Param*)pParam;

	
	DWORD dwTime=0;
	param->pDlg->Ping(param->strIP.GetBuffer(0),dwTime);

	param->pDlg->OnButtonConnect();

	delete param;
	return 0;
}


DWORD WINAPI CSSHToolDlg::DosShellThreadProc(LPVOID pParam)
{
	SFTP_Param * param = (SFTP_Param*)pParam;

	param->pDlg->ExecDosCmd(param->strCmdText);
	
	delete param;
	return 0;
}



DWORD WINAPI CSSHToolDlg::SftpThreadProc(LPVOID pParam)
{
	SFTP_Param * param = (SFTP_Param*)pParam;
	
	param->pDlg->ConnectSFTP(param->strIP.GetBuffer(0),param->strUser.GetBuffer(0),param->strPwd.GetBuffer(0),param->strLocalFile.GetBuffer(0),param->strDestFile.GetBuffer(0));

	delete param;
	return 0;
}

void CSSHToolDlg::OnButtonFtp() 
{
	CString strDestPath,strSrcPath;
	SFTP_Param * param = new SFTP_Param;
	if( !GetConnectInfo() )
	{
		return;
	}

	GetDlgItem(IDC_EDIT_DEST_PATH)->GetWindowText(strDestPath);
	GetDlgItem(IDC_EDIT_SRC_PATH)->GetWindowText(strSrcPath);

	param->strIP = m_strIP;
	param->strUser =m_strUser;
	param->strPwd = m_strPwd;
	param->strLocalFile = strSrcPath;
	param->strDestFile = strDestPath;
	param->pDlg =this;

	if( mSftpThread != NULL )
	{
	//	mSftpThread->ExitInstance();
	//	mSftpThread = NULL;
	}

	mSftpThread = AfxBeginThread((AFX_THREADPROC)SftpThreadProc,param,THREAD_PRIORITY_IDLE);
}






BOOL CSSHToolDlg::ExecDosCmd(LPCTSTR szCommandline) 
{ 
    BOOL bSuccess = FALSE; 
    
    if(szCommandline == NULL) 
    { 
        TRACE0("szCommandline Is NULL\n"); 
        return FALSE; 
    } 
 
    TCHAR szCmdLine[2048]={0}; 
    if(_sntprintf(szCmdLine, sizeof(szCmdLine) / sizeof(TCHAR), 
        _T("cmd.exe /C \"%s\""), szCommandline) == sizeof(szCmdLine) / sizeof(TCHAR)) 
    { 
        TRACE0("Target Is Too Long\n"); 
        return FALSE; 
    } 
 
    HANDLE hWritePipe = NULL; 
    HANDLE hReadPipe = NULL; 
    HANDLE hWriteShell = NULL; 
    HANDLE hReadShell = NULL; 
 
    SECURITY_ATTRIBUTES  sa; 
    memset(&sa, 0, sizeof(sa)); 
    sa.nLength = sizeof(sa); 
    sa.bInheritHandle = TRUE; 
    sa.lpSecurityDescriptor = NULL; 
 
    if(CreatePipe(&hReadPipe, &hReadShell, &sa, 0) 
        && CreatePipe(&hWriteShell, &hWritePipe, &sa, 0)) 
    { 
        STARTUPINFO            si; 
        memset(&si, 0, sizeof(si)); 
        si.cb           = sizeof(si); 
        si.dwFlags      = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES; 
        si.hStdInput    = hWriteShell; 
        si.hStdOutput   = hReadShell; 
        si.hStdError    = hReadShell; 
        si.wShowWindow  = SW_HIDE; 
         
        PROCESS_INFORMATION    pi; 
        memset(&pi, 0, sizeof(pi)); 
        

		//CString * pstrLogText = new CString("正在进行ping测试...\r\n");
	//	WriteLog(pstrLogText);


        int nMin = -1, nMax = -1, nAvg = -1; 
        if(CreateProcess(NULL, szCmdLine, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) 
        { 
            if(WaitForSingleObject(pi.hProcess, 10000) == WAIT_OBJECT_0) 
            { 
                TCHAR szBuffer[1024]; 
                DWORD dwBytes; 
                if(ReadFile(hReadPipe, szBuffer, sizeof(szBuffer), &dwBytes, NULL)) 
                { 
                    szBuffer[dwBytes] = '\0'; 
 
					CString * pstrLogText = new CString(szBuffer);
					WriteLog(pstrLogText);


                    LPTSTR lpszTime = NULL; 
                    lpszTime = _tcsstr(szBuffer, _T("Request timed out")); 
                    if(lpszTime == NULL) 
                    { 
                        lpszTime = _tcsstr(szBuffer, _T("Minimum")); 
                        if(lpszTime != NULL) 
                        { 
                            if(_stscanf(lpszTime, _T("Minimum = %dms, Maximum = %dms, Average = %dms"), 
                                &nMin, &nMax, &nAvg) == 3) 
                            { 
                                TRACE3("%d, %d, %d\n", nMin, nMax, nAvg); 
 
                               // dwTime = nAvg; 
                                bSuccess = TRUE; 
                            } 
                        } 
                        else 
                        { 
                            TRACE0("PING FORMAT is Error\n"); 
                        } 
                    } 
                    else 
                    { 

                        TRACE0("PING is Time Out\n"); 
 
                        bSuccess = TRUE; 
                    } 
                     
                } 
            } 
            else 
            { 
		
                TRACE1("Process(%d) is Time Out\n", pi.dwProcessId); 
                TerminateProcess(pi.hProcess, 0); 
            } 
 
            CloseHandle(pi.hThread); 
            CloseHandle(pi.hProcess); 
 
            TRACE3(_T("Minimum = %dms, Maximum = %dms, Average = %dms\n"), nMin, nMax, nAvg); 
        } 
    } 
 
    if(hWritePipe != NULL) 
        CloseHandle(hWritePipe); 
    if(hReadPipe != NULL) 
        CloseHandle(hReadPipe); 
    if(hWriteShell != NULL) 
        CloseHandle(hWriteShell); 
    if(hReadShell != NULL) 
        CloseHandle(hReadShell); 
 
    return bSuccess; 
}  





// 参数： 
//     szTarget目标地址（可以是域名、IP地址） 
//     dwTime返回时间；如果是超时，等于INFINITE 
BOOL CSSHToolDlg::Ping(LPCTSTR szTarget, DWORD &dwTime) 
{ 
    BOOL bSuccess = FALSE; 
    dwTime = INFINITE; 
 
    if(szTarget == NULL) 
    { 
        TRACE0("Target Is NULL\n"); 
        return FALSE; 
    } 
 
    TCHAR szCmdLine[80]; 
    if(_sntprintf(szCmdLine, sizeof(szCmdLine) / sizeof(TCHAR), 
        _T("ping.exe -n 3 %s"), szTarget) == sizeof(szCmdLine) / sizeof(TCHAR)) 
    { 
        TRACE0("Target Is Too Long\n"); 
        return FALSE; 
    } 
 
    HANDLE hWritePipe = NULL; 
    HANDLE hReadPipe = NULL; 
    HANDLE hWriteShell = NULL; 
    HANDLE hReadShell = NULL; 
 
    SECURITY_ATTRIBUTES  sa; 
    memset(&sa, 0, sizeof(sa)); 
    sa.nLength = sizeof(sa); 
    sa.bInheritHandle = TRUE; 
    sa.lpSecurityDescriptor = NULL; 
 
    if(CreatePipe(&hReadPipe, &hReadShell, &sa, 0) 
        && CreatePipe(&hWriteShell, &hWritePipe, &sa, 0)) 
    { 
        STARTUPINFO            si; 
        memset(&si, 0, sizeof(si)); 
        si.cb           = sizeof(si); 
        si.dwFlags      = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES; 
        si.hStdInput    = hWriteShell; 
        si.hStdOutput   = hReadShell; 
        si.hStdError    = hReadShell; 
        si.wShowWindow  = SW_HIDE; 
         
        PROCESS_INFORMATION    pi; 
        memset(&pi, 0, sizeof(pi)); 
        

		CString * pstrLogText = new CString("正在进行ping测试...\r\n");
		WriteLog(pstrLogText);


        int nMin = -1, nMax = -1, nAvg = -1; 
        if(CreateProcess(NULL, szCmdLine, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) 
        { 
            if(WaitForSingleObject(pi.hProcess, 10000) == WAIT_OBJECT_0) 
            { 
                TCHAR szBuffer[1024]; 
                DWORD dwBytes; 
                if(ReadFile(hReadPipe, szBuffer, sizeof(szBuffer), &dwBytes, NULL)) 
                { 
                    szBuffer[dwBytes] = '\0'; 
 
					CString * pstrLogText = new CString(szBuffer);
					WriteLog(pstrLogText);


                    LPTSTR lpszTime = NULL; 
                    lpszTime = _tcsstr(szBuffer, _T("Request timed out")); 
                    if(lpszTime == NULL) 
                    { 
                        lpszTime = _tcsstr(szBuffer, _T("Minimum")); 
                        if(lpszTime != NULL) 
                        { 
                            if(_stscanf(lpszTime, _T("Minimum = %dms, Maximum = %dms, Average = %dms"), 
                                &nMin, &nMax, &nAvg) == 3) 
                            { 
                                TRACE3("%d, %d, %d\n", nMin, nMax, nAvg); 
 
                                dwTime = nAvg; 
                                bSuccess = TRUE; 
                            } 
                        } 
                        else 
                        { 
                            TRACE0("PING FORMAT is Error\n"); 
                        } 
                    } 
                    else 
                    { 
						CString * pstrLogText = new CString("ping测试超时!\r\n");
		                WriteLog(pstrLogText);

                        TRACE0("PING is Time Out\n"); 
 
                        bSuccess = TRUE; 
                    } 
                     
                } 
            } 
            else 
            { 
				CString * pstrLogText = new CString("ping测试超时!\r\n");
		        WriteLog(pstrLogText);

                TRACE1("Process(%d) is Time Out\n", pi.dwProcessId); 
                TerminateProcess(pi.hProcess, 0); 
            } 
 
            CloseHandle(pi.hThread); 
            CloseHandle(pi.hProcess); 
 
            TRACE3(_T("Minimum = %dms, Maximum = %dms, Average = %dms\n"), nMin, nMax, nAvg); 
        } 
    } 
 
    if(hWritePipe != NULL) 
        CloseHandle(hWritePipe); 
    if(hReadPipe != NULL) 
        CloseHandle(hReadPipe); 
    if(hWriteShell != NULL) 
        CloseHandle(hWriteShell); 
    if(hReadShell != NULL) 
        CloseHandle(hReadShell); 
 
    return bSuccess; 
}  

// 选择需要上传的文件
void CSSHToolDlg::OnButtonSelectSrcFile() 
{
	char szFilter[] = {"All Files(*.*)|*.*||"};  
    CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter,NULL);  
	
    if(dlg.DoModal() == IDOK)  
    {  
        CString strPathName = dlg.GetPathName();  
        CString m_strFileName = dlg.GetFileTitle();  

		CString strShortFileName;
		char buf[512]={0};
		int i = 0,j;
		for( i = strPathName.GetLength() - 1; i>0;i-- )
		{
			char ch = strPathName.GetAt(i);
			if ( ch == '/' || ch == '\\')
			{
				int k = 0;
				for( j = i+1; j < strPathName.GetLength();j++ )
				{
					buf[k++]= strPathName.GetAt(j);
				}
				strShortFileName = buf;
				break;
			}

		}


		GetDlgItem(IDC_EDIT_SRC_PATH)->SetWindowText(strPathName);
		GetDlgItem(IDC_EDIT_DEST_PATH)->SetWindowText("/" + strShortFileName);

		

    }  
    else  
    {  
    }  
}

void CSSHToolDlg::OnButtonPing() 
{

	CString strDestPath,strSrcPath;
	SFTP_Param * param = new SFTP_Param;
	
	GetDlgItem(IDC_EDIT_IP)->GetWindowText(m_strIP);
	GetDlgItem(IDC_EDIT_USR)->GetWindowText(m_strUser);
	GetDlgItem(IDC_EDIT_PWD)->GetWindowText(m_strPwd);
	
	m_strIP.TrimLeft();
	m_strIP.TrimRight();
	
	m_strUser.TrimLeft();
	m_strUser.TrimRight();
	
	m_strPwd.TrimLeft();
	m_strPwd.TrimRight();
	
	if( m_strIP.IsEmpty())
	{
		return ;
	}
	/*
	if( m_strUser.IsEmpty())
	{
		return 0;
	}
	if( m_strPwd.IsEmpty())
	{
		return 0;
	}
	*/
	if( !CommonLib_IsValidIP(m_strIP.GetBuffer(0)) )
	{
		AfxMessageBox("IP地址格式不正确.");
		GetDlgItem(IDC_EDIT_IP)->SetFocus();
		return ;
	}
	
	GetDlgItem(IDC_EDIT_DEST_PATH)->GetWindowText(strDestPath);
	GetDlgItem(IDC_EDIT_SRC_PATH)->GetWindowText(strSrcPath);
	
	param->strIP = m_strIP;
	param->strUser =m_strUser;
	param->strPwd = m_strPwd;
	param->strLocalFile = strSrcPath;
	param->strDestFile = strDestPath;
	param->pDlg =this;
	
	if( mPingThread != NULL )
	{
	//	mPingThread->ExitInstance();
	//	mPingThread = NULL;
	}

	GetDlgItem(IDC_EDIT_CONSOLE)->SetWindowText("");

	mPingThread = AfxBeginThread((AFX_THREADPROC)PingThreadProc,param,THREAD_PRIORITY_IDLE);
	
}
