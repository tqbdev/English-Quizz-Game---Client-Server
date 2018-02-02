
// ChildView.h : interface of the CChildView class
//


#pragma once
#include <string>
#include <fstream>
#include <algorithm>
#include <vector> 
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/error/en.h"

using namespace rapidjson;
using namespace std;

#define PORT 25000
#define WM_SOCKET WM_USER+1

// Struct PlayerClient
class PlayerClient
{
public:
	static unsigned int MAX_QUESTION;
	SOCKET sockClient;
	char ID[9];
	vector<int> randomQuestion;
	unsigned int index;
	int score;
	int flagWrong;

	PlayerClient()
	{
		score = 3;
		index = 0;
		flagWrong = 0;
		for (int i = 0; i < MAX_QUESTION; i++)
		{
			randomQuestion.push_back(i);
		}
		std::random_shuffle(randomQuestion.begin(), randomQuestion.end());
	}

	static void SetMaxQuestion(int num)
	{
		MAX_QUESTION = num;
	}
};

// Struct Question
struct Question
{
	unsigned int id;
	CString question;
	CString choices[4]; // A: "###", B: "###", C: "###", D: "###"
	unsigned short correct; // 0: A, 1: B, 2: C, 3: D
};

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
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	LRESULT SockMsg(WPARAM wParam, LPARAM lParam);

// Implementation
public:
	virtual ~CChildView();
protected:
	// GUI
	CFont m_font;
	CEdit m_boxChat;
	CButton m_btnStart;
	CButton m_btnStop;
	CString m_msgString;

	// Database
	vector<Question*> m_dbQuestions;

	// Server
	SOCKET m_sockServer;
	struct sockaddr_in m_serverAdd;

	void Split(CString src, CString des[2]);
	char* ConvertToChar(const CString &s);

	void mSend(SOCKET sk, CString Command);
	int mRecv(SOCKET sk, CString &Command);

	unsigned int m_NumOfClient;
	vector<PlayerClient*> m_Clients;

	// Client Logout
	void LogOutClient(const char* mess, WPARAM wParam);
	// Send Question to Client
	void SendQuestion(SOCKET sk, int id);
	// Send  Score to Client
	void SendScore(SOCKET sk, int score);
	// Send end to Client
	void EndGame(SOCKET sk, int endID);

	// Generated message map functions
protected:	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnButtonStart();
	afx_msg void OnButtonStop();
};