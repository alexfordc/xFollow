
// xFollow.h : xFollow Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CxFollowApp:
// �йش����ʵ�֣������ xFollow.cpp
//

class CxFollowApp : public CWinApp
{
public:
	CxFollowApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CxFollowApp theApp;
