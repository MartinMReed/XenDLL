// keytronDlg.cpp : implementation file
//

#include "stdafx.h"
#include "keytron.h"
#include "keytronDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeytronDlg dialog

CKeytronDlg::CKeytronDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKeytronDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKeytronDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKeytronDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeytronDlg)
	DDX_Control(pDX, userName, m_userName);
	DDX_Control(pDX, memLEVEL, m_memLEVEL);
	DDX_Control(pDX, memWIS, m_memWIS);
	DDX_Control(pDX, memINTEL, m_memINTEL);
	DDX_Control(pDX, memCONS, m_memCONS);
	DDX_Control(pDX, memAGIL, m_memAGIL);
	DDX_Control(pDX, memSTR, m_memSTR);
	DDX_Control(pDX, XenMemLevel, m_XenMemLevel);
	DDX_Control(pDX, XenMemWis, m_XenMemWis);
	DDX_Control(pDX, XenMemIntel, m_XenMemIntel);
	DDX_Control(pDX, XenMemCons, m_XenMemCons);
	DDX_Control(pDX, XenMemAgil, m_XenMemAgil);
	DDX_Control(pDX, XenMemStr, m_XenMemStr);
	DDX_Control(pDX, list_ssViewRight, m_list_ssViewRight);
	DDX_Control(pDX, list_ssViewLeft, m_list_ssViewLeft);
	DDX_Control(pDX, list_ssViewOpen, m_list_ssViewOpen);
	DDX_Control(pDX, list_memSeach, m_list_memSeach);
	DDX_Control(pDX, list_hidePan, m_list_hidePan);
	DDX_Control(pDX, list_changeOpac, m_list_changeOpac);
	DDX_Control(pDX, list_pausePlay, m_list_pausePlay);
	DDX_Control(pDX, list_startPlay, m_list_startPlay);
	DDX_Control(pDX, list_startRec, m_list_startRec);
	DDX_Control(pDX, memXP, m_memXP);
	DDX_Control(pDX, memYLOC, m_memYLOC);
	DDX_Control(pDX, memXLOC, m_memXLOC);
	DDX_Control(pDX, mem_searchi, m_mem_searchi);
	DDX_Control(pDX, XenMemY, m_XenMemY);
	DDX_Control(pDX, XenMemX, m_XenMemX);
	DDX_Control(pDX, XenMemXP, m_XenMemXP);
	DDX_Control(pDX, XenMemMP, m_XenMemMP);
	DDX_Control(pDX, XenMemHP, m_XenMemHP);
	DDX_Control(pDX, ssQuality, m_ssQuality);
	DDX_Control(pDX, list_script, m_script);
	DDX_Control(pDX, panall, m_panall);
	DDX_Control(pDX, cursor, m_cursor);
	DDX_Control(pDX, useOnlineUpdater, m_useOnlineUpdater);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CKeytronDlg, CDialog)
	//{{AFX_MSG_MAP(CKeytronDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, SAVE)
	ON_EN_KILLFOCUS(ssQuality, OnKillfocusssQuality)
	ON_CBN_SELCHANGE(list_startRec, OnSelchangestartRec)
	ON_CBN_SELCHANGE(list_startPlay, OnSelchangestartPlay)
	ON_CBN_SELCHANGE(list_ssViewRight, OnSelchangessViewRight)
	ON_CBN_SELCHANGE(list_ssViewOpen, OnSelchangessViewOpen)
	ON_CBN_SELCHANGE(list_ssViewLeft, OnSelchangessViewLeft)
	ON_CBN_SELCHANGE(list_pausePlay, OnSelchangepausePlay)
	ON_CBN_SELCHANGE(list_memSeach, OnSelchangememSeach)
	ON_CBN_SELCHANGE(list_hidePan, OnSelchangehidePan)
	ON_CBN_SELCHANGE(list_changeOpac, OnSelchangechangeOpac)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeytronDlg message handlers

BOOL CKeytronDlg::OnInitDialog()
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
	/**/ m_list_startRec.SetCurSel( 0 );
	/**/ m_list_startPlay.SetCurSel( 0 );
	/**/ m_list_ssViewRight.SetCurSel( 0 );
	/**/ m_list_ssViewOpen.SetCurSel( 0 );
	/**/ m_list_ssViewLeft.SetCurSel( 0 );
	/**/ m_list_pausePlay.SetCurSel( 0 );
	/**/ m_list_memSeach.SetCurSel( 0 );
	/**/ m_list_hidePan.SetCurSel( 0 );
	/**/ m_list_changeOpac.SetCurSel( 0 );
	/**/ 
	/**/ m_ssQuality.SetWindowText( "100" );
	/**/ 
	/**/ Search( );
	/**/ readFile( );
	/**/ 
	/*********************************************************************/
	/**/
	/*********************************************************************/
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CKeytronDlg::OnPaint() 
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
HCURSOR CKeytronDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CKeytronDlg::OnCancel() 
{
	CDialog::OnCancel();
}
