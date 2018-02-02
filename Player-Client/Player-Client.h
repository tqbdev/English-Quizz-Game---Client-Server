
// Player-Client.h : main header file for the Player-Client application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CPlayerClientApp:
// See Player-Client.cpp for the implementation of this class
//

class CPlayerClientApp : public CWinApp
{
public:
	CPlayerClientApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CPlayerClientApp theApp;
