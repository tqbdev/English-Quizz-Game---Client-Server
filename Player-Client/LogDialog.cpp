// LogDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Player-Client.h"
#include "LogDialog.h"
#include "afxdialogex.h"


// CLogDialog dialog

IMPLEMENT_DYNAMIC(CLogDialog, CDialogEx)

CLogDialog::CLogDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOGIN, pParent)
	, m_IP(_T(""))
	, m_ID(_T(""))
{

}

CLogDialog::~CLogDialog()
{
}

void CLogDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_IP, m_IP);
	DDX_Text(pDX, IDC_USER, m_ID);
	DDV_MaxChars(pDX, m_ID, 8);
	DDX_Control(pDX, IDC_IP, m_editIP);
	DDX_Control(pDX, IDC_USER, m_editUser);
}


BEGIN_MESSAGE_MAP(CLogDialog, CDialogEx)
	ON_BN_CLICKED(IDC_LOGIN, &CLogDialog::OnBnClickedLogin)
	ON_BN_CLICKED(IDC_LOGOUT, &CLogDialog::OnBnClickedLogout)
	ON_BN_CLICKED(IDC_CANCEL, &CLogDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// CLogDialog message handlers
BOOL CLogDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog

	m_IP = "127.0.0.1";
	GetDlgItem(IDC_LOGOUT)->EnableWindow(FALSE);
	GetDlgItem(IDC_CANCEL)->EnableWindow(FALSE);
	UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLogDialog::OnBnClickedLogin()
{
	UpdateData(TRUE);
	GetParent()->SendMessage(WM_COMMAND, MAKELONG(IDC_LOGIN, BN_CLICKED));
}

void CLogDialog::OnBnClickedLogout()
{
	UpdateData(TRUE);
	GetParent()->SendMessage(WM_COMMAND, MAKELONG(IDC_LOGOUT, BN_CLICKED));
}

void CLogDialog::OnBnClickedCancel()
{
	UpdateData(TRUE);
	GetParent()->SendMessage(WM_COMMAND, MAKELONG(IDC_CANCEL, BN_CLICKED));
}
