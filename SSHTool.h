// SSHTool.h : main header file for the SSHTOOL application
//

#if !defined(AFX_SSHTOOL_H__8E22A7CE_7D8F_470C_9A0A_CE76AE683D03__INCLUDED_)
#define AFX_SSHTOOL_H__8E22A7CE_7D8F_470C_9A0A_CE76AE683D03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSSHToolApp:
// See SSHTool.cpp for the implementation of this class
//

class CSSHToolApp : public CWinApp
{
public:
	CSSHToolApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSSHToolApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSSHToolApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SSHTOOL_H__8E22A7CE_7D8F_470C_9A0A_CE76AE683D03__INCLUDED_)
