// transformerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "transformer.h"
#include "transformerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransformerDlg dialog

CTransformerDlg::CTransformerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTransformerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTransformerDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTransformerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransformerDlg)
	DDX_Control(pDX, IDOK, m_IDOK);
	DDX_Control(pDX, screen_sizeSTATIC, m_screen_sizeSTATIC);
	DDX_Control(pDX, monitor_HealthPercentSTATIC, m_monitor_HealthPercentSTATIC);
	DDX_Control(pDX, monitor_ManaPercentSTATIC, m_monitor_ManaPercentSTATIC);
	DDX_Control(pDX, instruc_InsertNumSTATIC, m_instruc_InsertNumSTATIC);
	DDX_Control(pDX, instruc_SimMouseSTATIC, m_instruc_SimMouseSTATIC);
	DDX_Control(pDX, instruc_CommentSTATIC, m_instruc_CommentSTATIC);
	DDX_Control(pDX, instruc_PauseAfterSTATICA, m_instruc_PauseAfterSTATICA);
	DDX_Control(pDX, instruc_PauseAfterSTATICB, m_instruc_PauseAfterSTATICB);
	DDX_Control(pDX, instruc_TimedUseSTATIC, m_instruc_TimedUseSTATIC);
	DDX_Control(pDX, warn_JumpSTATIC, m_warn_JumpSTATIC);
	DDX_Control(pDX, safety_CoordCheckSTATIC, m_safety_CoordCheckSTATIC);
	DDX_Control(pDX, list_CopyButton, m_list_CopyButton);
	DDX_Control(pDX, list_NewButton, m_list_NewButton);
	DDX_Control(pDX, list_SaveButton, m_list_SaveButton);
	DDX_Control(pDX, safety_SurfMaxSTATIC, m_safety_SurfMaxSTATIC);
	DDX_Control(pDX, safety_SurfMinSTATIC, m_safety_SurfMinSTATIC);
	DDX_Control(pDX, DumbDown, m_DumbDown);
	DDX_Control(pDX, instruc_SendPackeTComBox, m_instruc_SendPackeTComBox);
	DDX_Control(pDX, instruc_SendPacketBrowseButton, m_instruc_SendPacketBrowseButton);
	DDX_Control(pDX, instruc_SendPacketCBox, m_instruc_SendPacketCBox);
	DDX_Control(pDX, AdapterComBox, m_AdapterComBox);
	DDX_Control(pDX, instruc_notFirstTimedUseCBox, m_instruc_notFirstTimedUseCBox);
	DDX_Control(pDX, instruc_useTimedUseCBox, m_instruc_useTimedUseCBox);
	DDX_Control(pDX, instruc_TimedUseEBoxB, m_instruc_TimedUseEBoxB);
	DDX_Control(pDX, instruc_TimedUseEBoxA, m_instruc_TimedUseEBoxA);
	DDX_Control(pDX, monitor_ManaComBox, m_monitor_ManaComBox);
	DDX_Control(pDX, monitor_ManaPercentEBox, m_monitor_ManaPercentEBox);
	DDX_Control(pDX, monitor_ManaCBox, m_monitor_ManaCBox);
	DDX_Control(pDX, instruc_ClearReturn, m_instruc_ClearReturn);
	DDX_Control(pDX, safety_MPWaitCBox, m_safety_MPWaitCBox);
	DDX_Control(pDX, safety_HPWaitCBox, m_safety_HPWaitCBox);
	DDX_Control(pDX, safety_CoordCheckconCBox, m_safety_CoordCheckconCBox);
	DDX_Control(pDX, safety_SurfCheckconCBox, m_safety_SurfCheckconCBox);
	DDX_Control(pDX, list_BeginButton, m_list_BeginButton);
	DDX_Control(pDX, list_LoadButton, m_list_LoadButton);
	DDX_Control(pDX, warn_AudioBrowseButton, m_warn_AudioBrowseButton);
	DDX_Control(pDX, list_SaveNameComBox, m_list_SaveNameComBox);
	DDX_Control(pDX, warn_AudioComBox, m_warn_AudioComBox);
	DDX_Control(pDX, list_InsertButton, m_list_InsertButton);
	DDX_Control(pDX, safety_CoordCheckEBoxB, m_safety_CoordCheckEBoxB);
	DDX_Control(pDX, safety_CoordCheckEBoxA, m_safety_CoordCheckEBoxA);
	DDX_Control(pDX, safety_CoordCheckCBox, m_safety_CoordCheckCBox);
	DDX_Control(pDX, list_MvDownButton, m_list_MvDownButton);
	DDX_Control(pDX, list_MvUpButton, m_list_MvUpButton);
	DDX_Control(pDX, list_DeleteButton, m_list_DeleteButton);
	DDX_Control(pDX, warn_JalCBox, m_warn_JalCBox);
	DDX_Control(pDX, hide_comment, m_hide_comment);
	DDX_Control(pDX, time_maximumCBox, m_time_maximumCBox);
	DDX_Control(pDX, time_minimumCBox, m_time_minimumCBox);
	DDX_Control(pDX, time_randomCBox, m_time_randomCBox);
	DDX_Control(pDX, instruc_SimKeyComBox, m_instruc_SimKeyComBox);
	DDX_Control(pDX, monitor_HealthComBox, m_monitor_HealthComBox);
	DDX_Control(pDX, screen_sizeB, m_screen_sizeB);
	DDX_Control(pDX, screen_sizeA, m_screen_sizeA);
	DDX_Control(pDX, instruc_InsertNumEBox, m_instruc_InsertNumEBox);
	DDX_Control(pDX, list_DisplayScript, m_list_DisplayScript);
	DDX_Control(pDX, monitor_HealthPercentEBox, m_monitor_HealthPercentEBox);
	DDX_Control(pDX, monitor_HealthCBox, m_monitor_HealthCBox);
	DDX_Control(pDX, warn_LogCBox, m_warn_LogCBox);
	DDX_Control(pDX, warn_JumpEBoxB, m_warn_JumpEBoxB);
	DDX_Control(pDX, warn_JumpEBoxA, m_warn_JumpEBoxA);
	DDX_Control(pDX, warn_JumpCBox, m_warn_JumpCBox);
	DDX_Control(pDX, warn_AudioCBox, m_warn_AudioCBox);
	DDX_Control(pDX, warn_ScreenshotCBox, m_warn_ScreenshotCBox);
	DDX_Control(pDX, safety_SurfCheckCBox, m_safety_SurfCheckCBox);
	DDX_Control(pDX, safety_SurfMaxEBox, m_safety_SurfMaxEBox);
	DDX_Control(pDX, safety_SurfMinEBox, m_safety_SurfMinEBox);
	DDX_Control(pDX, instruc_CommentEBox, m_instruc_CommentEBox);
	DDX_Control(pDX, instruc_PauseAfterEBoxB, m_instruc_PauseAfterEBoxB);
	DDX_Control(pDX, instruc_PauseAfterEBoxA, m_instruc_PauseAfterEBoxA);
	DDX_Control(pDX, instruc_SimMouseEBoxB, m_instruc_SimMouseEBoxB);
	DDX_Control(pDX, instruc_SimMouseEBoxA, m_instruc_SimMouseEBoxA);
	DDX_Control(pDX, instruc_SimMouseCBox, m_instruc_SimMouseCBox);
	DDX_Control(pDX, instruc_SimKeyCBox, m_instruc_SimKeyCBox);
	DDX_Control(pDX, instruc_SpecialCBox, m_instruc_SpecialCBox);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTransformerDlg, CDialog)
	//{{AFX_MSG_MAP(CTransformerDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(list_InsertButton, saveInstruction)
	ON_BN_CLICKED(list_NewButton, OnNewButton)
	ON_LBN_SELCHANGE(list_DisplayScript, OnViewButton)
	ON_EN_KILLFOCUS(instruc_PauseAfterEBoxB, OnKillfocusPauseAfterEBoxB)
	ON_BN_CLICKED(instruc_SimKeyCBox, OnSimKeyCBox)
	ON_BN_CLICKED(instruc_SimMouseCBox, OnSimMouseCBox)
	ON_BN_CLICKED(time_minimumCBox, OnminimumCBox)
	ON_BN_CLICKED(time_maximumCBox, OnmaximumCBox)
	ON_BN_CLICKED(time_randomCBox, OnrandomCBox)
	ON_BN_CLICKED(hide_comment, Oncomment)
	ON_EN_KILLFOCUS(instruc_PauseAfterEBoxA, OnKillfocusPauseAfterEBoxA)
	ON_BN_CLICKED(warn_JalCBox, OnJalCBox)
	ON_EN_KILLFOCUS(warn_JumpEBoxB, OnKillfocusJumpEBoxB)
	ON_EN_KILLFOCUS(warn_JumpEBoxA, OnKillfocusJumpEBoxA)
	ON_BN_CLICKED(list_DeleteButton, OnDeleteButton)
	ON_BN_CLICKED(list_MvUpButton, OnMvUpButton)
	ON_BN_CLICKED(list_MvDownButton, OnMvDownButton)
	ON_BN_CLICKED(list_SaveButton, OnSaveButton)
	ON_EN_KILLFOCUS(safety_SurfMaxEBox, OnKillfocusSurfMaxEBox)
	ON_BN_CLICKED(warn_AudioBrowseButton, OnAudioBrowseButton)
	ON_BN_CLICKED(list_LoadButton, OnLoadButton)
	ON_BN_CLICKED(list_BeginButton, OnBeginButton)
	ON_BN_CLICKED(safety_SurfCheckCBox, OnSurfCheckCBox)
	ON_BN_CLICKED(list_CopyButton, OnCopyButton)
	ON_BN_CLICKED(safety_CoordCheckCBox, OnCoordCheckCBox)
	ON_EN_KILLFOCUS(safety_CoordCheckEBoxB, OnKillfocusCoordCheckEBoxB)
	ON_EN_KILLFOCUS(safety_CoordCheckEBoxA, OnKillfocusCoordCheckEBoxA)
	ON_EN_KILLFOCUS(safety_SurfMinEBox, OnKillfocusSurfMinEBox)
	ON_EN_KILLFOCUS(instruc_SimMouseEBoxA, OnKillfocusSimMouseEBoxA)
	ON_EN_KILLFOCUS(instruc_SimMouseEBoxB, OnKillfocusSimMouseEBoxB)
	ON_BN_CLICKED(instruc_ClearReturn, OnClearReturn)
	ON_CBN_SELCHANGE(instruc_SimKeyComBox, OnSelchangeSimKeyComBox)
	ON_CBN_SELCHANGE(monitor_HealthComBox, OnSelchangeHealthComBox)
	ON_EN_KILLFOCUS(instruc_TimedUseEBoxA, OnKillfocusTimedUseEBoxA)
	ON_EN_KILLFOCUS(instruc_TimedUseEBoxB, OnKillfocusTimedUseEBoxB)
	ON_BN_CLICKED(instruc_SendPacketBrowseButton, OnSendPacketBrowseButton)
	ON_BN_CLICKED(DumbDown, OnDumbDown)
	ON_BN_CLICKED(instruc_SendPacketCBox, OnSendPacketCBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTransformerDlg message handlers

BOOL CTransformerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	/*********************************************************************/
	/**/
	/*********************************************************************/
	/**/ 
	/**/ CString Path(*__argv);
	/**/ int i=Path.ReverseFind('\\')+1;
	/**/ if(i) Path=Path.Left(i);
	/**/ strcpy(m_sStartPath, Path);
	/**/ strcpy(m_sAudioBrowsedPath, Path);
	/**/ strcpy(m_sPacketBrowsedPath, Path);
	/**/ strcpy(m_sLastOpened, "");
	/**/ 
	/**/ m_iCount = 0;
	/**/ FirstSetup( );
	/**/ 
	/**/ OnminimumCBox();
	/**/ m_instruc_SimKeyComBox.SetCurSel( 0 );
	/**/ m_monitor_HealthComBox.SetCurSel( 0 );
	/**/ m_monitor_ManaComBox.SetCurSel( 0 );
	/**/ 
	/**/ m_list_DisplayScript.SetCurSel( 0 );
	/**/ OnViewButton( );
	/**/ SearchFiles( 0 );
	/**/ SearchFiles( 1 );
	/**/ SearchFiles( 2 );
	/**/ 
	/**/ SearchAdapter( );
	/**/ 
	/*********************************************************************/
	/**/
	/*********************************************************************/
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTransformerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTransformerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}
