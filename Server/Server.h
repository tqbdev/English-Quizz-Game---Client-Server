
// Server.h : main header file for the Server application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CServerApp:
// See Server.cpp for the implementation of this class
//

class CServerApp : public CWinApp
{
public:
	CServerApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CServerApp theApp;
