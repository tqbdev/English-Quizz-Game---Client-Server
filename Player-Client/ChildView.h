
// ChildView.h : interface of the CChildView class
//


#pragma once
#include "LogDialog.h"

#define	IDT_TIMER	WM_USER + 200
// CChildView window

class CChildView : public CWnd
{
// Construction
public:
	CChildView();

// Attributes
public:

// Operations
public:

// Overrides
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	void LoadQuestionChoice(CString &str);
	void Split(CString src, CString des[2]);
	char* ConvertToChar(const CString &s);
	void mSend(CString Command);
	int mRecv(CString &Command);
	LRESULT SockMsg(WPARAM wParam, LPARAM lParam);
	void ClearContent();

	CString Command;

	virtual ~CChildView();
	CFont m_font;
	CButton m_btnA;
	CButton m_btnB;
	CButton m_btnC;
	CButton m_btnD;

	CStatic m_question;
	CStatic m_A;
	CString m_txtA;
	CStatic m_B;
	CStatic m_C;
	CStatic m_D;
	CStatic m_score;

	CStatic m_timer;
	CStatic m_ID;

	CLogDialog *clogDlg;

	// Connect
	SOCKET sClient;
	sockaddr_in servAdd;

	// Timer
	unsigned short time;
	// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileLogin();
	afx_msg void OnBnClickedLogin();
	afx_msg void OnBnClickedLogout();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedA();
	afx_msg void OnBnClickedB();
	afx_msg void OnBnClickedC();
	afx_msg void OnBnClickedD();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
