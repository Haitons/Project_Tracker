
// Project_Tracker.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CProject_TrackerApp: 
// �йش����ʵ�֣������ Project_Tracker.cpp
//

class CProject_TrackerApp : public CWinApp
{
public:
	CProject_TrackerApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CProject_TrackerApp theApp;