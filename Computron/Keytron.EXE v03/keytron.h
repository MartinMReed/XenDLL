// keytron.h : main header file for the KEYTRON application
//

#if !defined(AFX_KEYTRON_H__89AD833B_562A_41B7_B921_188CB948EAC5__INCLUDED_)
#define AFX_KEYTRON_H__89AD833B_562A_41B7_B921_188CB948EAC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CKeytronApp:
// See keytron.cpp for the implementation of this class
//

class CKeytronApp : public CWinApp
{
public:
	CKeytronApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeytronApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CKeytronApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYTRON_H__89AD833B_562A_41B7_B921_188CB948EAC5__INCLUDED_)
