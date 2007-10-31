// keytronDlg.h : header file
//

#if !defined(AFX_KEYTRONDLG_H__50A9467C_2D69_4F91_8432_C6290675C44F__INCLUDED_)
#define AFX_KEYTRONDLG_H__50A9467C_2D69_4F91_8432_C6290675C44F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CKeytronDlg dialog

class CKeytronDlg : public CDialog
{
// Construction
public:
	CKeytronDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CKeytronDlg)
	enum { IDD = IDD_KEYTRON_DIALOG };
	CEdit	m_userName;
	CEdit	m_memLEVEL;
	CEdit	m_memWIS;
	CEdit	m_memINTEL;
	CEdit	m_memCONS;
	CEdit	m_memAGIL;
	CEdit	m_memSTR;
	CEdit	m_XenMemLevel;
	CEdit	m_XenMemWis;
	CEdit	m_XenMemIntel;
	CEdit	m_XenMemCons;
	CEdit	m_XenMemAgil;
	CEdit	m_XenMemStr;
	CComboBox	m_list_ssViewRight;
	CComboBox	m_list_ssViewLeft;
	CComboBox	m_list_ssViewOpen;
	CComboBox	m_list_memSeach;
	CComboBox	m_list_hidePan;
	CComboBox	m_list_changeOpac;
	CComboBox	m_list_pausePlay;
	CComboBox	m_list_startPlay;
	CComboBox	m_list_startRec;
	CEdit	m_memXP;
	CEdit	m_memYLOC;
	CEdit	m_memXLOC;
	CEdit	m_mem_searchi;
	CEdit	m_XenMemY;
	CEdit	m_XenMemX;
	CEdit	m_XenMemXP;
	CEdit	m_XenMemMP;
	CEdit	m_XenMemHP;
	CEdit	m_ssQuality;
	CComboBox	m_script;
	CButton	m_panall;
	CButton	m_cursor;
	CButton	m_useOnlineUpdater;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeytronDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	/*********************************************************************/
	/**/
	/*********************************************************************/
	/**/
	/**/ char buff[256];
	/**/ void readFile( void );
	/**/ void Search( void );
	/**/ int sizeOf( char* str );
	/**/ int convertDIK_Key( int i );
	/**/ int convertInput( int i );
	/**/ void setScriptName( char * name );
	/**/
	/*********************************************************************/
	/**/
	/*********************************************************************/

	// Generated message map functions
	//{{AFX_MSG(CKeytronDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void SAVE();
	afx_msg void OnKillfocusssQuality();
	virtual void OnCancel();
	afx_msg void Onbutton();
	afx_msg void OnSelchangestartRec();
	afx_msg void OnSelchangestartPlay();
	afx_msg void OnSelchangessViewRight();
	afx_msg void OnSelchangessViewOpen();
	afx_msg void OnSelchangessViewLeft();
	afx_msg void OnSelchangepausePlay();
	afx_msg void OnSelchangememSeach();
	afx_msg void OnSelchangehidePan();
	afx_msg void OnSelchangechangeOpac();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYTRONDLG_H__50A9467C_2D69_4F91_8432_C6290675C44F__INCLUDED_)
