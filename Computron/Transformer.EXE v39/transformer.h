// transformer.h : main header file for the TRANSFORMER application
//

#if !defined(AFX_TRANSFORMER_H__AF15F6DE_9040_4A78_9251_17833358747B__INCLUDED_)
#define AFX_TRANSFORMER_H__AF15F6DE_9040_4A78_9251_17833358747B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTransformerApp:
// See transformer.cpp for the implementation of this class
//

class CTransformerApp : public CWinApp
{
public:
	CTransformerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransformerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTransformerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSFORMER_H__AF15F6DE_9040_4A78_9251_17833358747B__INCLUDED_)
