
// direct2d-transition.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// Cdirect2dtransitionApp:
// See direct2d-transition.cpp for the implementation of this class
//

class Cdirect2dtransitionApp : public CWinApp
{
public:
	Cdirect2dtransitionApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern Cdirect2dtransitionApp theApp;
