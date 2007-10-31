// transformerDlg.h : header file
//


//For getcwd
#include <direct.h>

#include <math.h>
#include <mmsystem.h>

#include "computron.h"

#if !defined(AFX_TRANSFORMERDLG_H__9D259EE8_C28D_49AE_B5D7_4D9BB099A9F4__INCLUDED_)
#define AFX_TRANSFORMERDLG_H__9D259EE8_C28D_49AE_B5D7_4D9BB099A9F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTransformerDlg dialog

class CTransformerDlg : public CDialog
{
// Construction
public:
	CTransformerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTransformerDlg)
	enum { IDD = IDD_TRANSFORMER_DIALOG };
	CButton	m_IDOK;
	CStatic	m_screen_sizeSTATIC;
	CStatic	m_monitor_HealthPercentSTATIC;
	CStatic	m_monitor_ManaPercentSTATIC;
	CStatic	m_instruc_InsertNumSTATIC;
	CStatic	m_instruc_SimMouseSTATIC;
	CStatic	m_instruc_CommentSTATIC;
	CStatic	m_instruc_PauseAfterSTATICA;
	CStatic	m_instruc_PauseAfterSTATICB;
	CStatic	m_instruc_TimedUseSTATIC;
	CStatic	m_warn_JumpSTATIC;
	CStatic	m_safety_CoordCheckSTATIC;
	CButton	m_list_CopyButton;
	CButton	m_list_NewButton;
	CButton	m_list_SaveButton;
	CStatic	m_safety_SurfMaxSTATIC;
	CStatic	m_safety_SurfMinSTATIC;
	CButton	m_DumbDown;
	CComboBox	m_instruc_SendPackeTComBox;
	CButton	m_instruc_SendPacketBrowseButton;
	CButton	m_instruc_SendPacketCBox;
	CComboBox	m_AdapterComBox;
	CButton	m_instruc_notFirstTimedUseCBox;
	CButton	m_instruc_useTimedUseCBox;
	CEdit	m_instruc_TimedUseEBoxB;
	CEdit	m_instruc_TimedUseEBoxA;
	CComboBox	m_monitor_ManaComBox;
	CEdit	m_monitor_ManaPercentEBox;
	CButton	m_monitor_ManaCBox;
	CButton	m_instruc_ClearReturn;
	CButton	m_safety_MPWaitCBox;
	CButton	m_safety_HPWaitCBox;
	CButton	m_safety_CoordCheckconCBox;
	CButton	m_safety_SurfCheckconCBox;
	CButton	m_list_BeginButton;
	CButton	m_list_LoadButton;
	CButton	m_warn_AudioBrowseButton;
	CComboBox	m_list_SaveNameComBox;
	CComboBox	m_warn_AudioComBox;
	CButton	m_list_InsertButton;
	CEdit	m_safety_CoordCheckEBoxB;
	CEdit	m_safety_CoordCheckEBoxA;
	CButton	m_safety_CoordCheckCBox;
	CButton	m_list_MvDownButton;
	CButton	m_list_MvUpButton;
	CButton	m_list_DeleteButton;
	CButton	m_warn_JalCBox;
	CButton	m_hide_comment;
	CButton	m_time_maximumCBox;
	CButton	m_time_minimumCBox;
	CButton	m_time_randomCBox;
	CComboBox	m_instruc_SimKeyComBox;
	CComboBox	m_monitor_HealthComBox;
	CEdit	m_screen_sizeB;
	CEdit	m_screen_sizeA;
	CEdit	m_instruc_InsertNumEBox;
	CListBox	m_list_DisplayScript;
	CHotKeyCtrl	m_monitor_HealthHK;
	CEdit	m_monitor_HealthPercentEBox;
	CButton	m_monitor_HealthCBox;
	CButton	m_warn_LogCBox;
	CEdit	m_warn_JumpEBoxB;
	CEdit	m_warn_JumpEBoxA;
	CButton	m_warn_JumpCBox;
	CButton	m_warn_AudioCBox;
	CButton	m_warn_ScreenshotCBox;
	CButton	m_safety_SurfCheckCBox;
	CEdit	m_safety_SurfMaxEBox;
	CEdit	m_safety_SurfMinEBox;
	CEdit	m_instruc_CommentEBox;
	CEdit	m_instruc_PauseAfterEBoxB;
	CEdit	m_instruc_PauseAfterEBoxA;
	CEdit	m_instruc_SimMouseEBoxB;
	CEdit	m_instruc_SimMouseEBoxA;
	CButton	m_instruc_SimMouseCBox;
	CHotKeyCtrl	m_instruc_SimKeyHK;
	CButton	m_instruc_SimKeyCBox;
	CButton	m_instruc_SpecialCBox;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransformerDlg)
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
	/**/ Instruction* m_oFirst;
	/**/ Instruction* m_oCurrent;
	/**/
	/**/ char m_sbuffer[256];
	/**/
	/**/ char m_sStartPath[256];
	/**/ char m_sAudioBrowsedPath[256];
	/**/ char m_sPacketBrowsedPath[256];
	/**/ char m_sLastOpened[256];
	/**/
	/**/ int m_iCount;
	/**/
	/**/ void UpdateDisplay( void );
	/**/ void ResetInputs( );
	/**/ void FirstSetup( void );
	/**/ void ResetPositions( void );
	/**/ void ReadFile( void );
	/**/ int convertDIK_Key( int list_i, bool notHealth );
	/**/ int convertInput( int key_i, bool notHealth );
	/**/ void SearchFiles( int n );
	/**/ void Browse( char* BrowsedPath, int n );
	/**/ int sizeOf( char* str );
	/**/ void SearchAdapter( void );
	/**/ void EmptyScriptList( void );
	/**/ void ResetJumpTrackers( Instruction* start, bool all );
	/**/ void ResetScript( bool all );
	/**/ void ClearLinkedItem( int clearable );
	/**/ void DeleteItem( bool all );
	/**/
	/*********************************************************************/
	/**/
	/*********************************************************************/

	// Generated message map functions
	//{{AFX_MSG(CTransformerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void saveInstruction();
	afx_msg void OnNewButton();
	afx_msg void OnViewButton();
	afx_msg void OnChangePauseAfterEBoxB();
	afx_msg void OnKillfocusPauseAfterEBoxB();
	afx_msg void OnSimKeyCBox();
	afx_msg void OnSimMouseCBox();
	afx_msg void OnminimumCBox();
	afx_msg void OnmaximumCBox();
	afx_msg void OnrandomCBox();
	afx_msg void Onspecials();
	afx_msg void Oncomment();
	afx_msg void OnKillfocusPauseAfterEBoxA();
	afx_msg void OnJalCBox();
	afx_msg void OnKillfocusJumpEBoxB();
	afx_msg void OnKillfocusJumpEBoxA();
	afx_msg void OnDeleteButton();
	afx_msg void OnMvUpButton();
	afx_msg void OnMvDownButton();
	afx_msg void OnSaveButton();
	afx_msg void OnKillfocusSurfMaxEBox();
	afx_msg void OnAudioBrowseButton();
	afx_msg void OnLoadButton();
	afx_msg void OnBeginButton();
	afx_msg void OnSurfCheckCBox();
	afx_msg void OnCopyButton();
	afx_msg void OnCoordCheckCBox();
	afx_msg void OnKillfocusCoordCheckEBoxB();
	afx_msg void OnKillfocusCoordCheckEBoxA();
	afx_msg void OnKillfocusMiscEBoxA();
	afx_msg void OnKillfocusMiscEBoxB();
	afx_msg void OnKillfocusMiscEBoxC();
	afx_msg void OnKillfocusMiscEBoxD();
	afx_msg void OnKillfocusMiscEBoxE();
	afx_msg void OnKillfocusSurfMinEBox();
	afx_msg void OnKillfocusSimMouseEBoxA();
	afx_msg void OnKillfocusSimMouseEBoxB();
	afx_msg void OnButton1();
	afx_msg void OnClearReturn();
	afx_msg void OnKillfocusMiscEBoxF();
	afx_msg void OnSelchangeSimKeyComBox();
	afx_msg void OnSelchangeHealthComBox();
	afx_msg void OnKillfocusTimedUseEBoxA();
	afx_msg void OnKillfocusTimedUseEBoxB();
	afx_msg void OnSendPacketBrowseButton();
	afx_msg void OnDumbDown();
	afx_msg void OnSendPacketCBox();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSFORMERDLG_H__9D259EE8_C28D_49AE_B5D7_4D9BB099A9F4__INCLUDED_)
