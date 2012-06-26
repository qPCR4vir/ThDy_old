
// ThDy_DNAHybrid.MFC.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CThDy_DNAHybridMFCApp:
// See ThDy_DNAHybrid.MFC.cpp for the implementation of this class
//

class CThDy_DNAHybridMFCApp : public CWinApp
{
public:
	CThDy_DNAHybridMFCApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CThDy_DNAHybridMFCApp theApp;