
// Scribblenew.h : Scribblenew ���ε{�����D���Y��
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�� PCH �]�t���ɮ׫e���]�t 'stdafx.h'"
#endif

#include "resource.h"       // �D�n�Ÿ�


// CScribblenewApp:
// �аѾ\��@�����O�� Scribblenew.cpp
//

class CScribblenewApp : public CWinApp
{
public:
	CScribblenewApp();


// �мg
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �{���X��@
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CScribblenewApp theApp;
