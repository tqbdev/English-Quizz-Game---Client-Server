#pragma once
#include "afxwin.h"

#define PORT 25000
#define WM_SOCKET WM_USER+2

// CLogDialog dialog

class CLogDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CLogDialog)

public:
	CLogDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLogDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CString m_IP;
	CString m_ID;

	afx_msg void OnBnClickedLogin();
	afx_msg void OnBnClickedLogout();
	CEdit m_editIP;
	CEdit m_editUser;
	afx_msg void OnBnClickedCancel();
};
