// transformerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "keytron.h"
#include "keytronDlg.h"

#include <string>
#include <fstream>
#include <istream>

#define DIRECTINPUT_VERSION         DIRECTINPUT_HEADER_VERSION

#include <dinput.h>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



#define keyCount 117

UINT DIK_Keys[keyCount] = { -1, DIK_1, DIK_2, DIK_3, DIK_4, DIK_5, DIK_6, DIK_7, DIK_8, 
							DIK_9, DIK_0, -1, DIK_A, DIK_B, DIK_C, DIK_D, DIK_E, 
							DIK_F, DIK_G, DIK_H, DIK_I, DIK_J, DIK_K, DIK_L, DIK_M, 
							DIK_N, DIK_O, DIK_P, DIK_Q, DIK_R, DIK_S, DIK_T, DIK_U, 
							DIK_V, DIK_W, DIK_X, DIK_Y, DIK_Z, -1, DIK_F1, DIK_F2, 
							DIK_F3, DIK_F4, DIK_F5, DIK_F6, DIK_F7, DIK_F8, DIK_F9, 
							DIK_F10, DIK_F11, DIK_F12, -1, DIK_ESCAPE, DIK_TAB, -1, 
							DIK_MINUS, DIK_EQUALS, -1, DIK_BACK, DIK_RETURN, -1, 
							DIK_LBRACKET, DIK_RBRACKET, -1, DIK_LCONTROL, 
							DIK_RCONTROL, -1, DIK_LSHIFT, DIK_RSHIFT, -1, DIK_LALT, 
							DIK_RALT, -1, DIK_SEMICOLON, DIK_APOSTROPHE, -1, 
							DIK_BACKSLASH, -1, DIK_COMMA, DIK_PERIOD, -1, DIK_SPACE, 
							-1, DIK_CAPITAL, -1, DIK_NUMLOCK, DIK_SCROLL, -1, DIK_LEFT, 
							DIK_RIGHT, DIK_UP, DIK_DOWN, -1, DIK_PAUSE, DIK_HOME, -1, 
							DIK_INSERT, DIK_DELETE, -1, DIK_NUMPAD1, DIK_NUMPAD2, 
							DIK_NUMPAD3, DIK_NUMPAD4, DIK_NUMPAD5, DIK_NUMPAD6, 
							DIK_NUMPAD7, DIK_NUMPAD8, DIK_NUMPAD9, DIK_NUMPAD0, -1, 
							DIK_NUMPADENTER, DIK_NUMPADEQUALS, DIK_DECIMAL, DIK_ADD, 
							DIK_SUBTRACT, DIK_MULTIPLY, DIK_DIVIDE };


//-------------------------------------------------------
// 
//-------------------------------------------------------
void CKeytronDlg::SAVE( ) 
{
	FILE* out;
	out = fopen( "trans.cfg", "w" );
	CString str;

	if ( out != NULL )
	{
		m_userName.GetWindowText( str );
		strcpy( buff, str );
		fprintf( out, "username=%s\n", buff );

		m_script.GetWindowText( str );
		strcpy( buff, str );
		fprintf( out, "script=%s\n", buff );

		fprintf( out, "useOnlineUpdater=%i\n", m_useOnlineUpdater.GetCheck( ) );

		fprintf( out, "panall=%i\n", m_panall.GetCheck( ) );
		fprintf( out, "cursor=%i\n", m_cursor.GetCheck( ) );

		m_ssQuality.GetWindowText( str );
		strcpy( buff, str );
		fprintf( out, "ssQuality=%i\n", atoi( buff ) );

		m_mem_searchi.GetWindowText( str );
		strcpy( buff, str );
		fprintf( out, "mem_searchi=%i\n", atoi( buff ) );

		m_XenMemHP.GetWindowText( str );
		strcpy( buff, str );
		fprintf( out, "XenMemHP=%i\n", atoi( buff ) );

		m_XenMemMP.GetWindowText( str );
		strcpy( buff, str );
		fprintf( out, "XenMemMP=%i\n", atoi( buff ) );

		m_XenMemXP.GetWindowText( str );
		strcpy( buff, str );
		fprintf( out, "XenMemXP=%i\n", atoi( buff ) );

		m_XenMemX.GetWindowText( str );
		strcpy( buff, str );
		fprintf( out, "XenMemX=%i\n", atoi( buff ) );

		m_XenMemY.GetWindowText( str );
		strcpy( buff, str );
		fprintf( out, "XenMemY=%i\n", atoi( buff ) );

		m_XenMemStr.GetWindowText( str );
		strcpy( buff, str );
		fprintf( out, "XenMemStr=%i\n", atoi( buff ) );

		m_XenMemAgil.GetWindowText( str );
		strcpy( buff, str );
		fprintf( out, "XenMemAgil=%i\n", atoi( buff ) );

		m_XenMemCons.GetWindowText( str );
		strcpy( buff, str );
		fprintf( out, "XenMemCons=%i\n", atoi( buff ) );

		m_XenMemIntel.GetWindowText( str );
		strcpy( buff, str );
		fprintf( out, "XenMemIntel=%i\n", atoi( buff ) );

		m_XenMemWis.GetWindowText( str );
		strcpy( buff, str );
		fprintf( out, "XenMemWis=%i\n", atoi( buff ) );

		m_XenMemLevel.GetWindowText( str );
		strcpy( buff, str );
		fprintf( out, "XenMemLevel=%i\n", atoi( buff ) );

		m_memXLOC.GetWindowText( str );
		strcpy( buff, str );
		fprintf( out, "memXLOC=%i\n", atoi( buff ) );

		m_memYLOC.GetWindowText( str );
		strcpy( buff, str );
		fprintf( out, "memYLOC=%i\n", atoi( buff ) );

		m_memXP.GetWindowText( str );
		strcpy( buff, str );
		fprintf( out, "memXP=%i\n", atoi( buff ) );

		m_memSTR.GetWindowText( str );
		strcpy( buff, str );
		fprintf( out, "memSTR=%i\n", atoi( buff ) );

		m_memAGIL.GetWindowText( str );
		strcpy( buff, str );
		fprintf( out, "memAGIL=%i\n", atoi( buff ) );

		m_memCONS.GetWindowText( str );
		strcpy( buff, str );
		fprintf( out, "memCONS=%i\n", atoi( buff ) );

		m_memINTEL.GetWindowText( str );
		strcpy( buff, str );
		fprintf( out, "memINTEL=%i\n", atoi( buff ) );

		m_memWIS.GetWindowText( str );
		strcpy( buff, str );
		fprintf( out, "memWIS=%i\n", atoi( buff ) );

		m_memLEVEL.GetWindowText( str );
		strcpy( buff, str );
		fprintf( out, "memLEVEL=%i\n", atoi( buff ) );

		fprintf( out, "KEY_START_RECORD=%i\n", convertDIK_Key( m_list_startRec.GetCurSel( ) ) );
		fprintf( out, "KEY_START_PLAY=%i\n", convertDIK_Key( m_list_startPlay.GetCurSel( ) ) );
		fprintf( out, "KEY_PAUSE_PLAY=%i\n", convertDIK_Key( m_list_pausePlay.GetCurSel( ) ) );
		fprintf( out, "KEY_CHANGE_OPACITY=%i\n", convertDIK_Key( m_list_changeOpac.GetCurSel( ) ) );
		fprintf( out, "KEY_HIDE_PANEL=%i\n", convertDIK_Key( m_list_hidePan.GetCurSel( ) ) );
		fprintf( out, "KEY_MEM_SEARCH=%i\n", convertDIK_Key( m_list_memSeach.GetCurSel( ) ) );
		fprintf( out, "KEY_PICVIEW_OPEN=%i\n", convertDIK_Key( m_list_ssViewOpen.GetCurSel( ) ) );
		fprintf( out, "KEY_PICVIEW_LEFT=%i\n", convertDIK_Key( m_list_ssViewLeft.GetCurSel( ) ) );
		fprintf( out, "KEY_PICVIEW_RIGHT=%i\n", convertDIK_Key( m_list_ssViewRight.GetCurSel( ) ) );
		
		fprintf( out, "endfile" );

		fclose( out );

		CDialog::OnCancel();
	}
}

//-------------------------------------------------------
// 
//-------------------------------------------------------
void CKeytronDlg::OnSelchangestartRec() 
{
	int cursel = m_list_startRec.GetCurSel( );
	if ( DIK_Keys[cursel] == -1 && cursel != 0 )
		m_list_startRec.SetCurSel( 0 );
}

//-------------------------------------------------------
// 
//-------------------------------------------------------
void CKeytronDlg::OnSelchangestartPlay() 
{
	int cursel = m_list_startPlay.GetCurSel( );
	if ( DIK_Keys[cursel] == -1 && cursel != 0 )
		m_list_startPlay.SetCurSel( 0 );
}

//-------------------------------------------------------
// 
//-------------------------------------------------------
void CKeytronDlg::OnSelchangessViewRight() 
{
	int cursel = m_list_ssViewRight.GetCurSel( );
	if ( DIK_Keys[cursel] == -1 && cursel != 0 )
		m_list_ssViewRight.SetCurSel( 0 );
}

//-------------------------------------------------------
// 
//-------------------------------------------------------
void CKeytronDlg::OnSelchangessViewOpen() 
{
	int cursel = m_list_ssViewOpen.GetCurSel( );
	if ( DIK_Keys[cursel] == -1 && cursel != 0 )
		m_list_ssViewOpen.SetCurSel( 0 );
}

//-------------------------------------------------------
// 
//-------------------------------------------------------
void CKeytronDlg::OnSelchangessViewLeft() 
{
	int cursel = m_list_ssViewLeft.GetCurSel( );
	if ( DIK_Keys[cursel] == -1 && cursel != 0 )
		m_list_ssViewLeft.SetCurSel( 0 );
}

//-------------------------------------------------------
// 
//-------------------------------------------------------
void CKeytronDlg::OnSelchangepausePlay() 
{
	int cursel = m_list_pausePlay.GetCurSel( );
	if ( DIK_Keys[cursel] == -1 && cursel != 0 )
		m_list_pausePlay.SetCurSel( 0 );
}

//-------------------------------------------------------
// 
//-------------------------------------------------------
void CKeytronDlg::OnSelchangememSeach() 
{
	int cursel = m_list_memSeach.GetCurSel( );
	if ( DIK_Keys[cursel] == -1 && cursel != 0 )
		m_list_memSeach.SetCurSel( 0 );
}

//-------------------------------------------------------
// 
//-------------------------------------------------------
void CKeytronDlg::OnSelchangehidePan() 
{
	int cursel = m_list_hidePan.GetCurSel( );
	if ( DIK_Keys[cursel] == -1 && cursel != 0 )
		m_list_hidePan.SetCurSel( 0 );
}

//-------------------------------------------------------
// 
//-------------------------------------------------------
void CKeytronDlg::OnSelchangechangeOpac() 
{
	int cursel = m_list_changeOpac.GetCurSel( );
	if ( DIK_Keys[cursel] == -1 && cursel != 0 )
		m_list_changeOpac.SetCurSel( 0 );
}

//-------------------------------------------------------
// 
//-------------------------------------------------------
void CKeytronDlg::Search( )
{	
    //Declare all needed handles     
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;

	CString str;

	hFind = FindFirstFile ( "*.trans", &FindFileData );
	
    //Use a do/while so we process whatever FindFirstFile returned     
    do     
    {        
        //Is it valid?         
        if ( hFind != INVALID_HANDLE_VALUE )         
        {           
            //Is it a . or .. directory? If it is, skip, or we'll go forever.             
            if ( ! ( strcmp( FindFileData.cFileName, "." ) ) || 
                ! ( strcmp( FindFileData.cFileName, ".." ) ) )                            
                continue;             
			
			char tempa[256];
			char tempb[256];
			strcpy( tempb, FindFileData.cFileName );
			int cutoff;
    
			cutoff = 6;

			for ( int i = 0; i < sizeOf( tempb )-cutoff; i++ )
				tempa[i]= tempb[i] ;

			tempa[sizeOf( tempb )-cutoff]= NULL;
			m_script.AddString( tempa );
        }  
    }
    while ( FindNextFile ( hFind, &FindFileData ) 
        && hFind != INVALID_HANDLE_VALUE );
    FindClose ( hFind );
}

//-------------------------------------------------------
// 
//-------------------------------------------------------
int CKeytronDlg::sizeOf( char* str )
{
	int c = 0;

	for ( int i = 0; str[i] != '\0'; i++)
		c++;

	return c;
}

//-------------------------------------------------------
// 
//-------------------------------------------------------
void CKeytronDlg::setScriptName( char * name ) 
{
	int sel = m_script.FindString(0,name);

	if ( sel < m_script.GetCount() && sel >= 0 )
		m_script.SetCurSel( sel );
}

//-------------------------------------------------------
// 
//-------------------------------------------------------
void CKeytronDlg::OnKillfocusssQuality() 
{
	CString str;
	
	m_ssQuality.GetWindowText( str );
	strcpy( buff, str );
	int A = atoi( buff );
	
	if ( A < 0 )
		m_ssQuality.SetWindowText( "0" );
	else if ( A > 100 )
		m_ssQuality.SetWindowText( "100" );
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
int CKeytronDlg::convertDIK_Key( int list_i ) 
{
	if ( list_i < keyCount ) return (int)DIK_Keys[list_i];
	return -1;
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
int CKeytronDlg::convertInput( int key_i ) 
{
	for ( int i = 0; i < keyCount; i++ )
		if ( key_i == DIK_Keys[i] ) return i;

	return -1;
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
void CKeytronDlg::readFile( ) 
{
	std::ifstream inFile;

	CString str;

	inFile.open( "trans.cfg", std::ios::in | std::ios::binary);
	
	if ( inFile )
	{
		std::string strBuffer;
		std::getline(inFile, strBuffer);

		char strData[256];

		while (!inFile.eof())
		{
			// get the key
			std::string strKey;
			int pos = 0;
			strKey = strBuffer.substr(0, strBuffer.find_first_of(" =\t\n\r", pos));
			pos += strKey.size();

			if ( pos > 0 )
			{
				// get the '=' character
				pos = strBuffer.find_first_not_of(" \t", pos);
				
				// find the first non-whitespace character after the '=' character
				pos = strBuffer.find_first_not_of(" \t", pos + 1);

				// get the data
				std::string strD;
				strD = strBuffer.substr(pos, strBuffer.find_first_of("\n\r", pos) - pos);
				
				strcpy( strData, strD.c_str( ) );
				
				if (strKey == "username")
					m_userName.SetWindowText( strData );

				else if (strKey == "script")
					setScriptName( strData );

				else if (strKey == "useOnlineUpdater" )
					m_useOnlineUpdater.SetCheck( atoi( strData ) );

				else if (strKey == "panall" )
					m_panall.SetCheck( atoi( strData ) );

				else if (strKey == "cursor" )
					m_cursor.SetCheck( atoi( strData ) );

				else if (strKey == "ssQuality" )
					m_ssQuality.SetWindowText( strData );

				else if (strKey == "mem_searchi" )
					m_mem_searchi.SetWindowText( strData );

				else if (strKey == "XenMemHP" )
					m_XenMemHP.SetWindowText( strData );

				else if (strKey == "XenMemMP" )
					m_XenMemMP.SetWindowText( strData );

				else if (strKey == "XenMemXP" )
					m_XenMemXP.SetWindowText( strData );

				else if (strKey == "XenMemX" )
					m_XenMemX.SetWindowText( strData );

				else if (strKey == "XenMemY" )
					m_XenMemY.SetWindowText( strData );

				else if (strKey == "XenMemStr" )
					m_XenMemStr.SetWindowText( strData );

				else if (strKey == "XenMemAgil" )
					m_XenMemAgil.SetWindowText( strData );

				else if (strKey == "XenMemCons" )
					m_XenMemCons.SetWindowText( strData );

				else if (strKey == "XenMemIntel" )
					m_XenMemIntel.SetWindowText( strData );

				else if (strKey == "XenMemWis" )
					m_XenMemWis.SetWindowText( strData );

				else if (strKey == "XenMemLevel" )
					m_XenMemLevel.SetWindowText( strData );

				else if (strKey == "memXLOC" )
					m_memXLOC.SetWindowText( strData );

				else if (strKey == "memYLOC" )
					m_memYLOC.SetWindowText( strData );

				else if (strKey == "memXP" )
					m_memXP.SetWindowText( strData );

				else if (strKey == "memSTR" )
					m_memSTR.SetWindowText( strData );

				else if (strKey == "memAGIL" )
					m_memAGIL.SetWindowText( strData );

				else if (strKey == "memCONS" )
					m_memCONS.SetWindowText( strData );

				else if (strKey == "memINTEL" )
					m_memINTEL.SetWindowText( strData );

				else if (strKey == "memWIS" )
					m_memWIS.SetWindowText( strData );

				else if (strKey == "memLEVEL" )
					m_memLEVEL.SetWindowText( strData );

				else if (strKey == "KEY_START_RECORD" )
					m_list_startRec.SetCurSel( convertInput( atoi( strData ) ) );

				else if (strKey == "KEY_START_PLAY" )
					m_list_startPlay.SetCurSel( convertInput( atoi( strData ) ) );

				else if (strKey == "KEY_PAUSE_PLAY" )
					m_list_pausePlay.SetCurSel( convertInput( atoi( strData ) ) );

				else if (strKey == "KEY_CHANGE_OPACITY" )
					m_list_changeOpac.SetCurSel( convertInput( atoi( strData ) ) );

				else if (strKey == "KEY_HIDE_PANEL" )
					m_list_hidePan.SetCurSel( convertInput( atoi( strData ) ) );

				else if (strKey == "KEY_MEM_SEARCH" )
					m_list_memSeach.SetCurSel( convertInput( atoi( strData ) ) );

				else if (strKey == "KEY_PICVIEW_OPEN" )
					m_list_ssViewOpen.SetCurSel( convertInput( atoi( strData ) ) );

				else if (strKey == "KEY_PICVIEW_LEFT" )
					m_list_ssViewLeft.SetCurSel( convertInput( atoi( strData ) ) );

				else if (strKey == "KEY_PICVIEW_RIGHT" )
					m_list_ssViewRight.SetCurSel( convertInput( atoi( strData ) ) );
			}

			std::getline(inFile, strBuffer);
		}
	}
}
