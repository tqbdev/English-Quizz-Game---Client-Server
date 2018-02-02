
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "Player-Client.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
	VERIFY(m_font.CreateFont(
		32,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		L"Arial"));

	clogDlg = new CLogDialog;
	time = 60;
}

CChildView::~CChildView()
{
	m_font.DeleteObject();
	delete clogDlg;
}

void CChildView::DoDataExchange(CDataExchange* pDX)
{
	//DDX_Text(pDX, IDC_BOX_CHAT, m_msgString);
}

BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_MESSAGE(WM_SOCKET, SockMsg)
	ON_WM_TIMER()
	ON_COMMAND(ID_FILE_LOGIN, &CChildView::OnFileLogin)
	ON_BN_CLICKED(IDC_LOGIN, &CChildView::OnBnClickedLogin)
	ON_BN_CLICKED(IDC_LOGOUT, &CChildView::OnBnClickedLogout)
	ON_BN_CLICKED(IDC_CANCEL, &CChildView::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_A, &CChildView::OnBnClickedA)
	ON_BN_CLICKED(IDC_BTN_B, &CChildView::OnBnClickedB)
	ON_BN_CLICKED(IDC_BTN_C, &CChildView::OnBnClickedC)
	ON_BN_CLICKED(IDC_BTN_D, &CChildView::OnBnClickedD)
END_MESSAGE_MAP()



// CChildView message handlers

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CWnd* pwndParent = this->GetParent();
	CRect rc;
	pwndParent->GetClientRect(&rc);

	CRect rectA(rc.left + 50, rc.bottom - 300, rc.left + 100, rc.bottom - 250);
	m_btnA.Create(L"A", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rectA, this, IDC_BTN_A);
	m_btnA.SetFont(&m_font);

	CRect rectB(rc.left + 50, rc.bottom - 220, rc.left + 100, rc.bottom - 170);
	m_btnB.Create(L"B", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rectB, this, IDC_BTN_B);
	m_btnB.SetFont(&m_font);

	CRect rectC(rc.left + 50, rc.bottom - 140, rc.left + 100, rc.bottom - 90);
	m_btnC.Create(L"C", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rectC, this, IDC_BTN_C);
	m_btnC.SetFont(&m_font);

	CRect rectD(rc.left + 50, rc.bottom - 60, rc.left + 100, rc.bottom - 10);
	m_btnD.Create(L"D", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rectD, this, IDC_BTN_D);
	m_btnD.SetFont(&m_font);

	// ID, Score, Timer
	CRect id(rc.left + 30, rc.top + 10, rc.left + 200, rc.top + 40);
	m_ID.Create(L"ID: ###", WS_CHILD | WS_VISIBLE, id, this, IDC_TEXT_ID);
	m_ID.SetFont(&m_font);

	CRect score(rc.left + 350, rc.top + 10, rc.left + 480, rc.top + 40);
	m_score.Create(L"Score: ###", WS_CHILD | WS_VISIBLE, score, this, IDC_TEXT_SCORE);
	m_score.SetFont(&m_font);

	CRect timer(rc.right - 160, rc.top + 10, rc.right - 30, rc.top + 40);
	m_timer.Create(L"Time: ###", WS_CHILD | WS_VISIBLE, timer, this, IDC_TEXT_TIMER);
	m_timer.SetFont(&m_font);

	// Question
	CRect ques(rc.left + 30, rc.top + 60, rc.right - 30, rc.bottom - 320);
	m_question.Create(L"", WS_CHILD | WS_VISIBLE, ques, this, IDC_TEXT_QUESTION);
	m_question.SetFont(&m_font);

	// Choices
	CRect A(rc.left + 150, rc.bottom - 300, rc.right - 100, rc.bottom - 250);
	m_A.Create(L"", WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, A, this, IDC_TEXT_A);
	m_A.SetFont(&m_font);

	CRect B(rc.left + 150, rc.bottom - 220, rc.right - 100, rc.bottom - 170);
	m_B.Create(L"", WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, B, this, IDC_TEXT_B);
	m_B.SetFont(&m_font);

	CRect C(rc.left + 150, rc.bottom - 140, rc.right - 100, rc.bottom - 90);
	m_C.Create(L"", WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, C, this, IDC_TEXT_C);
	m_C.SetFont(&m_font);

	CRect D(rc.left + 150, rc.bottom - 60, rc.right - 100, rc.bottom - 10);
	m_D.Create(L"", WS_CHILD | WS_VISIBLE | SS_CENTERIMAGE, D, this, IDC_TEXT_D);
	m_D.SetFont(&m_font);

	clogDlg->Create(IDD_LOGIN, this);

	clogDlg->ShowWindow(SW_SHOW);
}

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	//cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS,
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)), NULL);
	
	return TRUE;
}

void CChildView::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here

	// Do not call CWnd::OnPaint() for painting messages
}

void CChildView::OnFileLogin()
{
	clogDlg->GetDlgItem(IDC_CANCEL)->EnableWindow(TRUE);
	clogDlg->ShowWindow(SW_SHOW);
}

void CChildView::Split(CString src, CString des[2])
{
	int p1, p2;

	p1 = src.Find(_T("\r\n"), 0);
	des[0] = src.Mid(0, p1);

	p2 = src.Find(_T("\r\n"), p1 + 1);
	des[1] = src.Mid(p1 + 2, p2 - (p1 + 2));

}

char* CChildView::ConvertToChar(const CString &s)
{
	int nSize = s.GetLength();
	char *pAnsiString = new char[nSize + 1];
	memset(pAnsiString, 0, nSize + 1);
	wcstombs(pAnsiString, s, nSize + 1);
	return pAnsiString;
}

void CChildView::mSend(CString Command)
{
	int Len = Command.GetLength();
	Len += Len; // CString wchar_t 2 byte

	PBYTE sendBuff = new BYTE[Len];

	memset(sendBuff, 0, Len);
	memcpy(sendBuff, (PBYTE)(LPCTSTR)Command, Len);

	send(sClient, (char*)&Len, sizeof(Len), 0);
	send(sClient, (char*)sendBuff, Len, 0);

	delete[] sendBuff;
}

int CChildView::mRecv(CString &Command)
{
	UpdateData();

	int buffLength;
	recv(sClient, (char*)&buffLength, sizeof(int), 0);

	PBYTE buffer = new BYTE[buffLength + 1];
	memset(buffer, 0, buffLength + 1);

	recv(sClient, (char*)buffer, buffLength, 0);

	Command = (TCHAR*)buffer;

	delete[] buffer;

	if (Command.GetLength() == 0)
		return -1;
	return 0;

}

void CChildView::LoadQuestionChoice(CString &str)
{
	int p1, p2, p3, p4, p5;

	p1 = str.Find(L"*", 0);
	m_question.SetWindowTextW(str.Mid(0, p1));

	p2 = str.Find(L"*", p1 + 1);
	m_A.SetWindowTextW(str.Mid(p1 + 1, p2 - (p1 + 1)));
	
	p3 = str.Find(L"*", p2 + 1);
	m_B.SetWindowTextW(str.Mid(p2 + 1, p3 - (p2 + 1)));

	p4 = str.Find(L"*", p3 + 1);
	m_C.SetWindowTextW(str.Mid(p3 + 1, p4 - (p3 + 1)));

	p5 = str.Find(L"*", p4 + 1);
	m_D.SetWindowTextW(str.Mid(p4 + 1, p5 - (p4 + 1)));
}

void CChildView::OnBnClickedA()
{
	Command = "2\r\n0\r\n";
	mSend(Command);
}

void CChildView::OnBnClickedB()
{
	Command = "2\r\n1\r\n";
	mSend(Command);
}

void CChildView::OnBnClickedC()
{
	Command = "2\r\n2\r\n";
	mSend(Command);
}

void CChildView::OnBnClickedD()
{
	Command = "2\r\n3\r\n";
	mSend(Command);
}

void CChildView::ClearContent()
{
	m_question.SetWindowTextW(L"");
	m_A.SetWindowTextW(L"");
	m_B.SetWindowTextW(L"");
	m_C.SetWindowTextW(L"");
	m_D.SetWindowTextW(L"");

	m_timer.SetWindowTextW(L"Time: ###");
	m_ID.SetWindowTextW(L"ID: ###");
	m_score.SetWindowTextW(L"Score: ###");
}

LRESULT CChildView::SockMsg(WPARAM wParam, LPARAM lParam)
{

	if (WSAGETSELECTERROR(lParam))
	{
		// Display the error and close the socket
		closesocket(wParam);
	}
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_READ:
	{
		CString temp;
		if (mRecv(temp)<0)
			break;

		CString strResult[2];
		Split(temp, strResult);
		int flag1 = _ttoi(strResult[0]);
		switch (flag1)
		{
		case 1:
		{
			int flag2 = _ttoi(strResult[1]);
			if (flag2 == 1)
			{
				if (MessageBoxEx(GetSafeHwnd(), L"Đăng nhập thành công.", L"Notification", NULL, NULL) == IDOK)
				{
					clogDlg->ShowWindow(SW_HIDE);
					m_ID.SetWindowTextW(L"ID: " + clogDlg->m_ID);
					clogDlg->m_editIP.SetReadOnly();
					clogDlg->m_editUser.SetReadOnly();
					clogDlg->GetDlgItem(IDC_LOGOUT)->EnableWindow(TRUE);
					clogDlg->GetDlgItem(IDC_LOGIN)->EnableWindow(FALSE);
				}
			}
			else
			{
				MessageBoxEx(GetSafeHwnd(), L"Đăng nhập thất bại.\r\nTrùng ID! Vui lòng nhập ID khác.", L"Notification", NULL, NULL);
			}


			UpdateData(FALSE);
			break;
		}

		case 2: // Question, Choices
		{
			SetTimer(IDT_TIMER, 1000, NULL);
			time = 60;
			LoadQuestionChoice(strResult[1]);
			UpdateData(FALSE);
			break;
		}

		case 3: // Score
		{
			m_score.SetWindowTextW(L"Score: " + strResult[1]);
			UpdateData(FALSE);
			break;
		}

		case 4: // End game
		{
			int flag2 = _ttoi(strResult[1]);

			CString id;
			CString score;
			CString res;
			m_ID.GetWindowTextW(id);
			m_score.GetWindowTextW(score);
			res = id;
			res += "\r\n";
			res += score;
			res == ".\r\nExit...";
			KillTimer(IDT_TIMER);
			switch (flag2)
			{
			case 1: // Dừng cuộc chơi do điểm < 0
				if (MessageBoxEx(GetSafeHwnd(), res, L"Dừng trò chơi - Score < 0", NULL, NULL) == IDOK)
				{
					Command = L"3\r\n";
					mSend(Command);
				}
				break;
			case 2: // Dừng cuộc chơi do sai 3 câu liên tiếp
				if (MessageBoxEx(GetSafeHwnd(), res, L"Dừng trò chơi - Sai 3 câu liên tiếp", NULL, NULL) == IDOK)
				{
					Command = L"3\r\n";
					mSend(Command);
				}
				break;
			case 3: // Hết câu hỏi
			{
				if (MessageBoxEx(GetSafeHwnd(), res, L"Kết thúc trò chơi", NULL, NULL) == IDOK)
				{
					Command = L"3\r\n";
					mSend(Command);
				}
				break; 
			}
			}
			break;
		}
		}
		break;
	}
	case FD_CLOSE:
	{
		closesocket(sClient);
		ClearContent();
		MessageBoxEx(GetSafeHwnd(), L"Server đã đóng kết nối.", L"Notification", NULL, NULL);		
		clogDlg->ShowWindow(SW_SHOW);
		KillTimer(IDT_TIMER);
		clogDlg->m_editIP.SetReadOnly(FALSE);
		clogDlg->m_editUser.SetReadOnly(FALSE);
		clogDlg->GetDlgItem(IDC_LOGOUT)->EnableWindow(FALSE);
		clogDlg->GetDlgItem(IDC_LOGIN)->EnableWindow(TRUE);
		clogDlg->GetDlgItem(IDC_CANCEL)->EnableWindow(FALSE);
		UpdateData(FALSE);
		break;
	}

	}
	return 0;
}

void CChildView::OnBnClickedLogin()
{
	UpdateData(TRUE);
	if (clogDlg->m_ID == "")
	{
		MessageBoxEx(GetSafeHwnd(), L"Vui lòng nhập username (ID)", NULL, NULL, NULL);
		return;
	}

	sClient = socket(AF_INET, SOCK_STREAM, 0);
	hostent* host = NULL;
	if (sClient == INVALID_SOCKET)
	{
		MessageBoxEx(GetSafeHwnd(), L"socket() failed", L"ERROR", NULL, NULL);
		return;
	}

	servAdd.sin_family = AF_INET;
	servAdd.sin_port = htons(PORT);

	char* cIP = ConvertToChar(clogDlg->m_IP);

	servAdd.sin_addr.s_addr = inet_addr(cIP);

	delete[] cIP;

	CStringA cpy_IP(clogDlg->m_IP);

	if (servAdd.sin_addr.s_addr == INADDR_NONE)
	{
		host = (gethostbyname(cpy_IP));
		if (host == NULL)
		{
			MessageBoxEx(GetSafeHwnd(), L"Không thể kết nối đến server.", L"ERROR", NULL, NULL);
		}
		CopyMemory(&servAdd.sin_addr, host->h_addr_list[0],
			host->h_length);
		return;
	}

	int err = connect(sClient, (struct sockaddr*)&servAdd, sizeof(servAdd));
	if (err == SOCKET_ERROR) {
		MessageBoxEx(GetSafeHwnd(), L"Kết nối thất bại.", L"ERROR", NULL, NULL);
		return;
	}


	Command = L"1\r\n";
	Command += clogDlg->m_ID + L"\r\n";

	mSend(Command);
	m_txtA = "TEST";
	WSAAsyncSelect(sClient, m_hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
	UpdateData(FALSE);
}

void CChildView::OnBnClickedLogout()
{
	INT_PTR i = MessageBoxEx(GetSafeHwnd(), L"Bạn muốn logout?", L"Confirm", MB_OKCANCEL, NULL);
	if (i == IDCANCEL)
		return;
	Command = L"3\r\n";
	mSend(Command);
}

void CChildView::OnBnClickedCancel()
{
	clogDlg->ShowWindow(SW_HIDE);
}

void CChildView::OnTimer(UINT_PTR nIDEvent)
{
	if (time != 0)
	{
		time--;
	}

	WCHAR buffer[20];
	wsprintf(buffer, L"Time: %0.2d s", time);
	m_timer.SetWindowTextW(buffer);

	if (time == 0)
	{
		KillTimer(IDT_TIMER);
		MessageBox(L"Bạn bị trừ 1 điểm", L"Timeout", NULL);
		Command = "2\r\n4\r\n"; // 4 chắc chắn là câu tl sai
		mSend(Command);
	}
}