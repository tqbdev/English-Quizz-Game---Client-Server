
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "Server.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView
unsigned int PlayerClient::MAX_QUESTION = 0;

CChildView::CChildView()
{
	m_NumOfClient = 0;

	VERIFY(m_font.CreateFont(
		16,                        // nHeight
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

	// Load database
	ifstream ifs("questions.json");
	if (!ifs.is_open())
	{
		if (MessageBoxEx(GetSafeHwnd(), L"File questions.json not found!", L"JSON FILE ERROR", NULL, NULL) == IDOK)
		{
			//GetParent()->SendMessage(WM_CLOSE);
			PostQuitMessage(0);
		}
	}
	IStreamWrapper isw(ifs);
	Document document;
	if (document.ParseStream(isw).HasParseError())
	{
		WCHAR buffer[250];
		wsprintf(buffer, L"Error(offset %u) : %s", (unsigned)document.GetErrorOffset(),
			GetParseError_En(document.GetParseError()));
		if (MessageBoxEx(GetSafeHwnd(), buffer, L"JSON FILE ERROR", NULL, NULL) == IDOK)
		{
			//GetParent()->SendMessage(WM_CLOSE);
			PostQuitMessage(0);
		}
	}
	else {
		assert(document.IsObject());
		assert(document.HasMember("Questions"));
		assert(document["Questions"].IsArray());

		Value& questions = document["Questions"];

		int count = 0;
		for (auto& v : questions.GetArray())
		{
			int i = 0;
			Question* ques = new Question;
			ques->id = count;
			for (Value::ConstMemberIterator itr = v.MemberBegin();
				itr != v.MemberEnd(); ++itr, i++)
			{
				switch (i)
				{
				case 0:
					ques->question = itr->value.GetString();
					break;
				case 1:
				{
					int mm = 0;
					for (auto& k : itr->value.GetArray())
					{
						ques->choices[mm] = k.GetString();
						mm++;
					}
				}
				break;
				case 2:
					ques->correct = itr->value.GetInt();
					break;
				}
			}
			count++;
			m_dbQuestions.push_back(ques);
		}
	}
	PlayerClient::SetMaxQuestion(m_dbQuestions.size());
}

CChildView::~CChildView()
{
	m_font.DeleteObject();

	// Free mem database
	for (int i = m_dbQuestions.size() - 1; i >= 0; i--)
	{
		delete m_dbQuestions[i];
		m_dbQuestions.pop_back();
	}

	// Free mem client socket
	for (int i = m_Clients.size() - 1; i >= 0; i--)
	{
		closesocket(m_Clients[i]->sockClient);
		delete m_Clients[i];
		m_Clients.pop_back();
	}
}

void CChildView::DoDataExchange(CDataExchange* pDX)
{
	DDX_Text(pDX, IDC_BOX_CHAT, m_msgString);
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_MESSAGE(WM_SOCKET, SockMsg)
	ON_COMMAND(IDC_BUTTON_START, &CChildView::OnButtonStart)
	ON_COMMAND(IDC_BUTTON_STOP, &CChildView::OnButtonStop)
END_MESSAGE_MAP()



// CChildView message handlers

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	//SetFont(&m_font);
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CWnd* pwndParent = this->GetParent();
	CRect rc;
	pwndParent->GetClientRect(&rc);

	CRect rectStart(rc.left + 50, rc.bottom - 50, rc.left + 150, rc.bottom - 20);
	m_btnStart.Create(L"Start", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rectStart, this, IDC_BUTTON_START);
	m_btnStart.SetFont(&m_font);

	CRect rectStop(rc.right - 150, rc.bottom - 50, rc.right - 50, rc.bottom - 20);
	m_btnStop.Create(L"Stop", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rectStop, this, IDC_BUTTON_STOP);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	m_btnStop.SetFont(&m_font);

	CRect rectBox(rc.left, rc.top, rc.right, rc.bottom - 70);
	m_boxChat.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | WS_VSCROLL | ES_AUTOVSCROLL, rectBox, this, IDC_BOX_CHAT);
	m_boxChat.SetReadOnly();
	m_boxChat.SetFont(&m_font);
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

void CChildView::mSend(SOCKET sk, CString Command)
{
	int Len = Command.GetLength();
	Len += Len; // CString wchar_t 2 byte

	PBYTE sendBuff = new BYTE[Len];

	memset(sendBuff, 0, Len);
	memcpy(sendBuff, (PBYTE)(LPCTSTR)Command, Len);

	send(sk, (char*)&Len, sizeof(Len), 0);
	send(sk, (char*)sendBuff, Len, 0);

	delete sendBuff;
}

int CChildView::mRecv(SOCKET sk, CString &Command)
{
	UpdateData();

	int buffLength;
	recv(sk, (char*)&buffLength, sizeof(int), 0);

	PBYTE buffer = new BYTE[buffLength + 1];
	memset(buffer, 0, buffLength + 1);

	recv(sk, (char*)buffer, buffLength, 0);

	Command = (TCHAR*)buffer;

	delete buffer;

	if (Command.GetLength() == 0)
		return -1;
	return 0;

}

void CChildView::LogOutClient(const char* mess, WPARAM wParam)
{
	int pos = -1;
	for (int i = 0; i < m_Clients.size(); i++)
	{
		if (m_Clients[i]->sockClient == wParam)
		{
			pos = i;
			break;
		}
	}

	if (pos == -1) return;

	m_msgString += "Player: ";
	m_msgString += m_Clients[pos]->ID;
	m_msgString += ". ";
	m_msgString += mess;
	m_msgString += "...\r\n";

	closesocket(wParam);

	for (int j = pos; j < m_Clients.size() - 1; j++)
	{
		m_Clients[j]->sockClient = m_Clients[j + 1]->sockClient;
		strcpy(m_Clients[j]->ID, m_Clients[j + 1]->ID);
	}

	m_NumOfClient--;

	delete m_Clients[m_Clients.size() - 1];
	m_Clients.pop_back();

	UpdateData(FALSE);
}

void CChildView::SendQuestion(SOCKET sk, int id)
{
	CString Command;
	Command = "2\r\n";
	
	// Question
	Command += m_dbQuestions[id]->question;
	Command += "*";

	// Choice A
	Command += m_dbQuestions[id]->choices[0];
	Command += "*";

	// Choice B
	Command += m_dbQuestions[id]->choices[1];
	Command += "*";

	// Choice C
	Command += m_dbQuestions[id]->choices[2];
	Command += "*";

	// Choice D
	Command += m_dbQuestions[id]->choices[3];
	Command += "*\r\n";

	mSend(sk, Command);
}

void CChildView::SendScore(SOCKET sk, int score)
{
	CString Command;
	Command = "3\r\n";

	char pszNum[32] = { 0 };
	Command += _itoa(score, pszNum, 10);
	Command += "\r\n";

	mSend(sk, Command);
}

void CChildView::EndGame(SOCKET sk, int endID)
{
	CString Command;
	Command = "4\r\n";
	Command += to_wstring(endID).c_str();
	Command += "\r\n";
	mSend(sk, Command);
}

LRESULT CChildView::SockMsg(WPARAM wParam, LPARAM lParam)
{
	if (WSAGETSELECTERROR(lParam))
	{
		// Display the error and close the socket
		closesocket(wParam);
	}

	PlayerClient* res = NULL;
	CString Command;
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_ACCEPT:
	{
		res = new PlayerClient;
		res->sockClient = accept(wParam, NULL, NULL);
		m_Clients.push_back(res);
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		break;
	}
	case FD_READ:
	{
		CString temp;
		if (mRecv(wParam, temp) < 0)
			break;

		CString strResult[2];
		Split(temp, strResult);
		int flag = _ttoi(strResult[0]);
		switch (flag)
		{
		case 1://Login
		{
			char* tem = ConvertToChar(strResult[1]);
			bool check = false;
			int size = m_Clients.size();

			for (int i = 0; i < size; i++)
			{
				if (strcmp(tem, m_Clients[i]->ID) == 0)//Trung ID
				{
					check = true;
					if (size != 0)
					{
						delete m_Clients[m_Clients.size() - 1];
						m_Clients.pop_back();
					}
					break;
				}
			}

			if (!check)
			{
				strcpy(m_Clients[size - 1]->ID, tem);

				Command = "1\r\n1\r\n";
				mSend(wParam, Command);

				m_msgString += L"Player: " + strResult[1] + L". Login...\r\n";

				UpdateData(FALSE);
				m_NumOfClient++;				

				int index = m_Clients[size - 1]->index;
				int id = m_Clients[size - 1]->randomQuestion[index];
				SendQuestion(wParam, id);
				SendScore(wParam, m_Clients[size - 1]->score);
			}
			else
			{
				Command = "1\r\n0\r\n";
				mSend(wParam, Command);
			}

			UpdateData(FALSE);
			break;
		}

		case 2: // Client send answer
		{
			int answer = _ttoi(strResult[1]);

			int pos = -1;
			for (int i = 0; i < m_Clients.size(); i++)
			{
				if (m_Clients[i]->sockClient == wParam)
				{
					pos = i;
					break;
				}
			}

			// Get ID Question
			int index = m_Clients[pos]->index;
			int id = m_Clients[pos]->randomQuestion[index];
			if (m_dbQuestions[id]->correct == answer)
			{
				m_Clients[pos]->score++;
				m_Clients[pos]->flagWrong = 0;
			}
			else
			{
				m_Clients[pos]->score--;
				m_Clients[pos]->flagWrong++;
			}

			m_msgString += L"Player: ";
			m_msgString += m_Clients[pos]->ID;
			m_msgString += ". Score: ";
			
			char pszNum[32] = { 0 };
			m_msgString += _itoa(m_Clients[pos]->score, pszNum, 10);
			m_msgString += "\r\n";

			SendScore(wParam, m_Clients[pos]->score);
			
			if (m_Clients[pos]->flagWrong == 3)
			{
				EndGame(wParam, 2);
			}
			else if (m_Clients[pos]->score < 0)
			{
				EndGame(wParam, 1);
			}
			else if (index == PlayerClient::MAX_QUESTION - 1)
			{
				EndGame(wParam, 3);
			}
			else
			{
				id = m_Clients[pos]->randomQuestion[++(m_Clients[pos]->index)];
				SendQuestion(wParam, id);
			}

			UpdateData(FALSE);

			break;
		}

		case 3:
		{
			LogOutClient("Logout", wParam);
			break;
		}

		}
		break;
	}

	case FD_CLOSE:
	{
		LogOutClient("Disconnect", wParam);
		break;
	}
	}
	return 0;
}

void CChildView::OnButtonStart()
{
	UpdateData();
	m_sockServer = socket(AF_INET, SOCK_STREAM, 0);
	m_serverAdd.sin_family = AF_INET;
	m_serverAdd.sin_port = htons(PORT);
	m_serverAdd.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(m_sockServer, (SOCKADDR*)&m_serverAdd, sizeof(m_serverAdd));
	listen(m_sockServer, 5);

	int err = WSAAsyncSelect(m_sockServer, m_hWnd, WM_SOCKET, FD_READ | FD_ACCEPT | FD_CLOSE);
	if (err) MessageBox((LPCTSTR)"Cant call WSAAsyncSelect");

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(TRUE);
}

void CChildView::OnButtonStop()
{
	closesocket(m_sockServer);

	// Free mem client socket
	for (int i = m_Clients.size() - 1; i >= 0; i--)
	{
		closesocket(m_Clients[i]->sockClient);
		delete m_Clients[i];
		m_Clients.pop_back();
	}

	m_msgString = "";
	UpdateData(FALSE);
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
}