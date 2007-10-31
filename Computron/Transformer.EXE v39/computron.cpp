// transformerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "transformer.h"
#include "transformerDlg.h"

#include <string>
#include <fstream>
#include <istream>

#define DIRECTINPUT_VERSION         DIRECTINPUT_HEADER_VERSION

#include <dinput.h>

#include "pcap.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




#define keyCount 119

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
							DIK_SUBTRACT, DIK_MULTIPLY, DIK_DIVIDE, -1, -2 };



//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::UpdateDisplay( )
{
	char buffer[256];

	ResetPositions( );

	m_list_DisplayScript.ResetContent( );
	
	int hr = 0;
	int min = 0;
	int sec = 0;
	double timeR = 0.0;

	int hrS = 0;
	int minS = 0;
	int secS = 0;
	double timeS = 0.0;
	
	bool PrevWasSpecial = false;

	Instruction* current = m_oFirst;
	
	for ( int i = 0; i < m_iCount; i++ )
	{
		bool special = false;

		double timeShow = 0.0;
			
		//clear string
		strcpy( buffer, "" );
			
		//write the instruction number
		strcat( buffer, itoa( current->get_instruc_position( ), m_sbuffer, 10 ) );
			
		if ( !PrevWasSpecial )
			timeS = 0.0;
			
		//
		if ( current->get_instruc_isSpecial( ) == 1 )
		{
			strcat( buffer, "      * " );
			special = true;
			PrevWasSpecial = true;
				
			if ( m_time_minimumCBox.GetCheck( ) == 1 )
				timeS += current->get_instruc_pauseA( );
			else if ( m_time_maximumCBox.GetCheck( ) == 1 )
				timeS += current->get_instruc_pauseB( );
			else if ( m_time_randomCBox.GetCheck( ) == 1 )
			{
				if ( (int)current->get_instruc_pauseB( ) == 0 )
					timeS += 0.0;
				else
					timeS += ( rand( ) % (int)( current->get_instruc_pauseB( ) + 1 - current->get_instruc_pauseA( ) ) ) + current->get_instruc_pauseA( );
			}

			timeShow += timeS;
		}
		else
		{
			strcat( buffer, "   " );
			special = false;
			PrevWasSpecial = false;

			if ( m_time_minimumCBox.GetCheck( ) == 1 )
				timeR += current->get_instruc_pauseA( );
			else if ( m_time_maximumCBox.GetCheck( ) == 1 )
				timeR += current->get_instruc_pauseB( );
			else if ( m_time_randomCBox.GetCheck( ) == 1 )
			{
				if ( (int)current->get_instruc_pauseB( ) == 0 )
					timeR += 0.0;
				else
					timeR += ( rand( ) % (int)( current->get_instruc_pauseB( ) + 1 - current->get_instruc_pauseA( ) ) ) + current->get_instruc_pauseA( );
			}

			timeShow += timeR;
		}
			
		//find low time
		hr = (int)timeShow%216000;
		min = hr%3600;
		sec = min%60;
		hr/=3600;
		min/=60;

		if ( hr < 10 ) { strcat( buffer, "0" ); }
		strcat( buffer, itoa( hr, m_sbuffer, 10 ) );
		strcat( buffer, ":" );

		if ( min < 10 ) { strcat( buffer, "0" ); }
		strcat( buffer, itoa( min, m_sbuffer, 10 ) );
		strcat( buffer, ":" );

		if ( sec < 10 ) { strcat( buffer, "0" ); }
		strcat( buffer, itoa( sec, m_sbuffer, 10 ) );
		strcat( buffer, " -> " );
			
		if ( current->get_instruc_isSpecial( ) == 0 )
			strcat( buffer, "       " );

		//write the keystroke or mouse coords
		if ( current->get_instruc_useKeystroke( ) == 1 )
		{
			strcat( buffer, current->get_instruc_keystrokeName( ) );
		}
		else if ( current->get_instruc_useMouseClick( ) == 1 )
		{
			strcat( buffer, "( " );
			strcat( buffer, itoa( current->get_instruc_mouseClickX( ), m_sbuffer, 10 ) );
			strcat( buffer, " , " );
			strcat( buffer, itoa( current->get_instruc_mouseClickY( ), m_sbuffer, 10 ) );
			strcat( buffer, " )" );
		}
		else if ( current->get_instruc_clearReturnStack( ) == 1 )
		{
			strcat( buffer, "Clear Return Stack" );
		}
		else if ( current->get_instruc_sendPacket( ) == 1 )
		{
			strcat( buffer, current->get_instruc_packetFile( ) );
			strcat( buffer, ".cap" );
		}
		else
		{
			strcat( buffer, "Pause Only" );
		}
			
		//write the comment
		if ( m_hide_comment.GetCheck( ) == 0 )
		{
			strcat( buffer, "  -  \"" );
			strcat( buffer, current->get_instruc_comment( ) );
			strcat( buffer, "\"" );
		}
			
		//add the instruction data to the listbox
		m_list_DisplayScript.AddString( buffer );

		current = current->get_next( );
	}
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::saveInstruction( ) 
{
	OnKillfocusSurfMinEBox( );
	OnKillfocusSurfMaxEBox( );
	OnKillfocusSimMouseEBoxA( );
	OnKillfocusSimMouseEBoxB( );
	OnKillfocusJumpEBoxA( );
	OnKillfocusJumpEBoxB( );
	OnKillfocusPauseAfterEBoxA( );
	OnKillfocusPauseAfterEBoxB( );
	OnKillfocusTimedUseEBoxA( );
	OnKillfocusTimedUseEBoxB( );
	OnKillfocusCoordCheckEBoxA( );
	OnKillfocusCoordCheckEBoxB( );

	//SetCurrentDirectory( m_sStartPath );

	Instruction* current;
	Instruction* next;
	Instruction* temp;

	int sel = m_list_DisplayScript.GetCurSel( );

	CString str;
	m_instruc_InsertNumEBox.GetWindowText( str );
	strcpy( m_sbuffer, str );
	int loadedCheck = atoi( m_sbuffer );

	current = m_oFirst;
	next = current->get_next( );
	bool update = loadedCheck == current->get_instruc_position( );

	//if already insterted, then update
	while ( next && !update )
	{
		current = next;
		update = loadedCheck == current->get_instruc_position( );
		next = next->get_next( );
	}

	if ( !update )
	{
		temp = new Instruction( );
		m_iCount++;
		temp->set_previous( current );
		temp->set_next( current->get_next( ) );
		current->set_next( temp );
		current = temp;
	}

	//
	current->set_instruc_isSpecial( m_instruc_SpecialCBox.GetCheck( ) );
	
	//
	m_instruc_InsertNumEBox.GetWindowText( str );
	strcpy( m_sbuffer, str );
	current->set_instruc_position( atoi( m_sbuffer ) );
	
	//
	current->set_instruc_clearReturnStack( m_instruc_ClearReturn.GetCheck( ) );
	
	//
	current->set_instruc_useKeystroke( m_instruc_SimKeyCBox.GetCheck( ) );
	
	//
	current->set_safety_useMPWait( m_safety_MPWaitCBox.GetCheck( ) );
	
	//
	current->set_safety_useHPWait( m_safety_HPWaitCBox.GetCheck( ) );
	
	//
	if ( m_instruc_SimKeyCBox.GetCheck( ) == 1 )
	{
		m_instruc_SimKeyComBox.GetWindowText( str );
		strcpy( m_sbuffer, str );
		current->set_instruc_keystrokeName( m_sbuffer );
		current->set_instruc_keystrokeCode( convertDIK_Key( m_instruc_SimKeyComBox.GetCurSel( ), true ) );
	}
	else
	{
		current->set_instruc_keystrokeName( "" );
		current->set_instruc_keystrokeCode( 0 );
	}
	
	//
	current->set_instruc_useMouseClick( m_instruc_SimMouseCBox.GetCheck( ) );
	
	//
	if ( m_instruc_SimMouseCBox.GetCheck( ) == 1 )
	{
		m_instruc_SimMouseEBoxA.GetWindowText( str );
		strcpy( m_sbuffer, str );
		current->set_instruc_mouseClickX( atoi( m_sbuffer ) );
	}
	else
		current->set_instruc_mouseClickX( 0 );
	
	//
	if ( m_instruc_SimMouseCBox.GetCheck( ) == 1 )
	{
		m_instruc_SimMouseEBoxB.GetWindowText( str );
		strcpy( m_sbuffer, str );
		current->set_instruc_mouseClickY( atoi( m_sbuffer ) );
	}
	else
		current->set_instruc_mouseClickY( 0 );
	
	//
	current->set_instruc_sendPacket( m_instruc_SendPacketCBox.GetCheck( ) );
	
	//
	m_instruc_SendPackeTComBox.GetWindowText( str );
	sprintf( m_sbuffer, "%s", str );
	current->set_instruc_packetFile( m_sbuffer );
	
	//
	if ( !m_sPacketBrowsedPath || strcmp( m_sPacketBrowsedPath,"" ) == 0 )
		current->set_instruc_packetFolder( m_sStartPath );
	else
		current->set_instruc_packetFolder( m_sPacketBrowsedPath );
	
	//
	current->set_safety_useCoordCheck( m_safety_CoordCheckCBox.GetCheck( ) );
	
	//
	if ( m_safety_CoordCheckCBox.GetCheck( ) == 1 )
		current->set_safety_useCoordCheckcon( m_safety_CoordCheckconCBox.GetCheck( ) );
	else
		current->set_safety_useCoordCheckcon( 0 );
	
	//
	m_safety_CoordCheckEBoxA.GetWindowText( str );
	strcpy( m_sbuffer, str );
	current->set_safety_CoordX( atoi( m_sbuffer ) );
	
	//
	m_safety_CoordCheckEBoxB.GetWindowText( str );
	strcpy( m_sbuffer, str );
	current->set_safety_CoordY( atoi( m_sbuffer ) );
	
	//
	m_instruc_PauseAfterEBoxA.GetWindowText( str );
	strcpy( m_sbuffer, str );
	current->set_instruc_pauseA( ( double )atof( m_sbuffer ) );
	
	//
	m_instruc_PauseAfterEBoxB.GetWindowText( str );
	strcpy( m_sbuffer, str );
	current->set_instruc_pauseB( ( double )atof( m_sbuffer ) );
	
	//
	m_instruc_CommentEBox.GetWindowText( str );
	strcpy( m_sbuffer, str );
	current->set_instruc_comment( m_sbuffer );
	
	//
	current->set_instruc_useTimedUse( m_instruc_useTimedUseCBox.GetCheck( ) );
	
	//
	m_instruc_TimedUseEBoxA.GetWindowText( str );
	strcpy( m_sbuffer, str );
	current->set_instruc_TimedUseA( ( double )atof( m_sbuffer ) );
	
	//
	m_instruc_TimedUseEBoxB.GetWindowText( str );
	strcpy( m_sbuffer, str );
	current->set_instruc_TimedUseB( ( double )atof( m_sbuffer ) );
	
	//
	current->set_instruc_notFirstTimedUse( m_instruc_notFirstTimedUseCBox.GetCheck( ) );
	
	//
	m_safety_SurfMinEBox.GetWindowText( str );
	strcpy( m_sbuffer, str );
	current->set_safety_surfMin( atoi( m_sbuffer ) );
	
	//
	m_safety_SurfMaxEBox.GetWindowText( str );
	strcpy( m_sbuffer, str );
	current->set_safety_surfMax( atoi( m_sbuffer ) );
	
	//
	current->set_safety_useSurfCheck( m_safety_SurfCheckCBox.GetCheck( ) );
	
	//
	if ( m_safety_SurfCheckCBox.GetCheck( ) == 1 )
		current->set_safety_useSurfCheckcon( m_safety_SurfCheckconCBox.GetCheck( ) );
	else
		current->set_safety_useSurfCheckcon( 0 );
	
	//
	current->set_warnings_takeScreenshot( m_warn_ScreenshotCBox.GetCheck( ) );
	
	//
	current->set_warnings_playAudio( m_warn_AudioCBox.GetCheck( ) );
	
	//
	m_warn_AudioComBox.GetWindowText( str );
	sprintf( m_sbuffer, "%s", str );
	current->set_warnings_audioFile( m_sbuffer );
	
	//
	if ( !m_sAudioBrowsedPath || strcmp( m_sAudioBrowsedPath,"" ) == 0 )
		current->set_warnings_audioFolder( m_sStartPath );
	else
		current->set_warnings_audioFolder( m_sAudioBrowsedPath );
	
	//
	current->set_warnings_jump( m_warn_JumpCBox.GetCheck( ) );
	
	//
	m_warn_JumpEBoxA.GetWindowText( str );
	strcpy( m_sbuffer, str );

	Instruction* find = m_oFirst;
	for ( int i = 0; i < atoi( m_sbuffer ); i++ )
		find = find->get_next( );

	current->set_tracker_jumpA( find );
	
	//
	m_warn_JumpEBoxB.GetWindowText( str );
	strcpy( m_sbuffer, str );

	find = m_oFirst;
	for ( int i = 0; i < atoi( m_sbuffer ); i++ )
		find = find->get_next( );

	current->set_tracker_jumpB( find );

	//
	current->set_warnings_jal( m_warn_JalCBox.GetCheck( ) );
	
	//
	current->set_warnings_loggOff( m_warn_LogCBox.GetCheck( ) );

	UpdateDisplay( );

	m_list_DisplayScript.SetCurSel( current->get_instruc_position( ) );

	OnViewButton( );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::ResetInputs( ) 
{
	m_instruc_SpecialCBox.SetCheck( 0 );
	m_instruc_InsertNumEBox.SetWindowText( "0" );
	m_instruc_ClearReturn.SetCheck( 0 );
	m_instruc_SimKeyCBox.SetCheck( 0 );
	m_instruc_SimKeyComBox.SetCurSel( 0 );
	m_instruc_SimMouseCBox.SetCheck( 0 );
	m_instruc_SimMouseEBoxA.SetWindowText( "0" );
	m_instruc_SimMouseEBoxB.SetWindowText( "0" );
	m_instruc_SendPacketCBox.SetCheck( 0 );
	m_instruc_SendPackeTComBox.SetWindowText( "" );
	m_instruc_PauseAfterEBoxA.SetWindowText( "0" );
	m_instruc_PauseAfterEBoxB.SetWindowText( "0" );
	m_instruc_CommentEBox.SetWindowText( "" );
	m_instruc_useTimedUseCBox.SetCheck( 0 );
	m_instruc_TimedUseEBoxA.SetWindowText( "0" );
	m_instruc_TimedUseEBoxB.SetWindowText( "0" );
	m_instruc_notFirstTimedUseCBox.SetCheck( 0 );
	m_safety_SurfMinEBox.SetWindowText( "0" );
	m_safety_SurfMaxEBox.SetWindowText( "0" );
	m_safety_SurfCheckCBox.SetCheck( 0 );
	m_safety_SurfCheckconCBox.SetCheck( 0 );
	m_safety_CoordCheckCBox.SetCheck( 0 );
	m_safety_CoordCheckconCBox.SetCheck( 0 );
	m_safety_CoordCheckEBoxA.SetWindowText( "0" );
	m_safety_CoordCheckEBoxB.SetWindowText( "0" );
	m_safety_MPWaitCBox.SetCheck( 0 );
	m_safety_HPWaitCBox.SetCheck( 0 );
	m_warn_ScreenshotCBox.SetCheck( 0 ) ;
	m_warn_AudioCBox.SetCheck( 0 );
	m_warn_AudioComBox.SetWindowText( "" );
	m_warn_JumpCBox.SetCheck( 0 );
	m_warn_JumpEBoxA.SetWindowText( "0" );
	m_warn_JumpEBoxB.SetWindowText( "0" );
	m_warn_JalCBox.SetCheck( 0 );
	m_warn_LogCBox.SetCheck( 0 );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnNewButton( ) 
{	
	ResetInputs( );
	m_instruc_InsertNumEBox.SetWindowText( itoa( m_iCount, m_sbuffer, 10 ) );

	saveInstruction( );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnViewButton( ) 
{
	Instruction* current = m_oFirst;
	
	//cycle through to the node to display
	for ( int i = 0; i < m_list_DisplayScript.GetCurSel( ); i++ )
		current = current->get_next( );
		
	m_safety_SurfCheckconCBox.EnableWindow( current->get_safety_useSurfCheck( ) );
	m_safety_CoordCheckconCBox.EnableWindow( current->get_safety_useCoordCheck( ) );
		
	m_instruc_SpecialCBox.SetCheck( current->get_instruc_isSpecial( ) );
	m_instruc_InsertNumEBox.SetWindowText( itoa( current->get_instruc_position( ), m_sbuffer, 10 ) );
	m_instruc_ClearReturn.SetCheck( current->get_instruc_clearReturnStack( ) );
	m_instruc_SimKeyCBox.SetCheck( current->get_instruc_useKeystroke( ) );
	m_instruc_SimKeyComBox.SetCurSel( convertInput( current->get_instruc_keystrokeCode( ), true ) );

	m_instruc_SimMouseCBox.SetCheck( current->get_instruc_useMouseClick( ) );
	m_instruc_SimMouseEBoxA.SetWindowText( itoa( current->get_instruc_mouseClickX( ), m_sbuffer, 10 ) );
	m_instruc_SimMouseEBoxB.SetWindowText( itoa( current->get_instruc_mouseClickY( ), m_sbuffer, 10 ) );

	m_instruc_SendPacketCBox.SetCheck( current->get_instruc_sendPacket( ) );
	m_instruc_SendPackeTComBox.SetWindowText( current->get_instruc_packetFile( ) );
	strcpy( m_sPacketBrowsedPath, current->get_instruc_packetFolder( ) );

	sprintf( m_sbuffer, "%f", current->get_instruc_pauseA( ) );
	m_instruc_PauseAfterEBoxA.SetWindowText( m_sbuffer );
	sprintf( m_sbuffer, "%f", current->get_instruc_pauseB( ) );
	m_instruc_PauseAfterEBoxB.SetWindowText( m_sbuffer );

	m_instruc_CommentEBox.SetWindowText( current->get_instruc_comment( ) );

	m_instruc_useTimedUseCBox.SetCheck( current->get_instruc_useTimedUse( ) );

	sprintf( m_sbuffer, "%f", current->get_instruc_TimedUseA( ) );
	m_instruc_TimedUseEBoxA.SetWindowText( m_sbuffer );
	sprintf( m_sbuffer, "%f", current->get_instruc_TimedUseB( ) );
	m_instruc_TimedUseEBoxB.SetWindowText( m_sbuffer );

	m_instruc_notFirstTimedUseCBox.SetCheck( current->get_instruc_notFirstTimedUse( ) );

	m_safety_SurfMinEBox.SetWindowText( itoa( current->get_safety_surfMin( ), m_sbuffer, 10 ) );
	m_safety_SurfMaxEBox.SetWindowText( itoa( current->get_safety_surfMax( ), m_sbuffer, 10 ) );

	m_safety_SurfCheckCBox.SetCheck( current->get_safety_useSurfCheck( ) );
	m_safety_SurfCheckconCBox.SetCheck( current->get_safety_useSurfCheckcon( ) );
	m_safety_CoordCheckCBox.SetCheck( current->get_safety_useCoordCheck( ) );
	m_safety_CoordCheckconCBox.SetCheck( current->get_safety_useCoordCheckcon( ) );

	m_safety_CoordCheckEBoxA.SetWindowText( itoa( current->get_safety_CoordX( ), m_sbuffer, 10 ) );
	m_safety_CoordCheckEBoxB.SetWindowText( itoa( current->get_safety_CoordY( ), m_sbuffer, 10 ) );

	m_safety_MPWaitCBox.SetCheck( current->get_safety_useMPWait( ) );
	m_safety_HPWaitCBox.SetCheck( current->get_safety_useHPWait( ) );
	m_warn_ScreenshotCBox.SetCheck( current->get_warnings_takeScreenshot( ) );
	m_warn_AudioCBox.SetCheck( current->get_warnings_playAudio( ) );
	m_warn_AudioComBox.SetWindowText( current->get_warnings_audioFile( ) );
	strcpy( m_sAudioBrowsedPath, current->get_warnings_audioFolder( ) );
	m_warn_JumpCBox.SetCheck( current->get_warnings_jump( ) );

	m_warn_JumpEBoxA.SetWindowText( itoa( ( ( current->get_tracker_jumpA( ) )->get_instruc_position( ) ), m_sbuffer, 10 ) );
	m_warn_JumpEBoxB.SetWindowText( itoa( ( ( current->get_tracker_jumpB( ) )->get_instruc_position( ) ), m_sbuffer, 10 ) );

	m_warn_JalCBox.SetCheck( current->get_warnings_jal( ) );
	m_warn_LogCBox.SetCheck( current->get_warnings_loggOff( ) );

	m_oCurrent = current;

	SearchFiles( 1 );
	SearchFiles( 2 );
}

//-------------------------------------------------------
// 
//-------------------------------------------------------
void CTransformerDlg::OnDumbDown() 
{
	m_instruc_SpecialCBox.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_instruc_ClearReturn.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_instruc_SendPacketCBox.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_instruc_SendPackeTComBox.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_instruc_SendPacketBrowseButton.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_safety_MPWaitCBox.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_safety_HPWaitCBox.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_instruc_useTimedUseCBox.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_instruc_TimedUseEBoxA.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_instruc_TimedUseSTATIC.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_instruc_TimedUseEBoxB.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_instruc_notFirstTimedUseCBox.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_safety_SurfCheckCBox.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_safety_SurfCheckconCBox.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_safety_SurfMinSTATIC.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_safety_SurfMinEBox.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_safety_SurfMaxSTATIC.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_safety_SurfMaxEBox.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_safety_CoordCheckCBox.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_safety_CoordCheckconCBox.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_safety_CoordCheckEBoxA.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_safety_CoordCheckSTATIC.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_safety_CoordCheckEBoxB.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_monitor_HealthCBox.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_monitor_HealthPercentEBox.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_monitor_HealthPercentSTATIC.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_monitor_HealthComBox.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_monitor_ManaCBox.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_monitor_ManaPercentEBox.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_monitor_ManaPercentSTATIC.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_monitor_ManaComBox.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_warn_ScreenshotCBox.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_warn_AudioCBox.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_warn_AudioComBox.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_warn_AudioBrowseButton.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_warn_JumpCBox.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_warn_JumpEBoxA.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_warn_JumpSTATIC.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_warn_JumpEBoxB.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_warn_JalCBox.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_warn_LogCBox.ShowWindow( !m_DumbDown.GetCheck( ) );
	m_AdapterComBox.ShowWindow( !m_DumbDown.GetCheck( ) );
}

//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnKillfocusCoordCheckEBoxA( ) 
{
	CString str;
	
	m_safety_CoordCheckEBoxA.GetWindowText( str );
	strcpy( m_sbuffer, str );
	int A = atoi( m_sbuffer );
	
	if ( A < -1 )
		m_safety_CoordCheckEBoxA.SetWindowText( "0" );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnKillfocusCoordCheckEBoxB( ) 
{
	CString str;
	
	m_safety_CoordCheckEBoxB.GetWindowText( str );
	strcpy( m_sbuffer, str );
	int A = atoi( m_sbuffer );
	
	if ( A < -1 )
		m_safety_CoordCheckEBoxB.SetWindowText( "0" );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnKillfocusPauseAfterEBoxA( ) 
{
	CString str;
	
	m_instruc_PauseAfterEBoxA.GetWindowText( str );
	strcpy( m_sbuffer, str );
	double A = atof( m_sbuffer );
	
	if ( A < 0.0 )
		m_instruc_PauseAfterEBoxA.SetWindowText( "0" );

	OnKillfocusPauseAfterEBoxB( );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnKillfocusPauseAfterEBoxB( ) 
{
	CString str;
	
	m_instruc_PauseAfterEBoxA.GetWindowText( str );
	strcpy( m_sbuffer, str );
	double A = atof( m_sbuffer );
	
	m_instruc_PauseAfterEBoxB.GetWindowText( str );
	strcpy( m_sbuffer, str );
	double B = atof( m_sbuffer );
	
	if ( B < A )
	{
		sprintf( m_sbuffer, "%f", A );
		m_instruc_PauseAfterEBoxB.SetWindowText( m_sbuffer );
	}
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnKillfocusTimedUseEBoxA( ) 
{
	CString str;
	
	m_instruc_TimedUseEBoxA.GetWindowText( str );
	strcpy( m_sbuffer, str );
	double A = atof( m_sbuffer );
	
	if ( A < 0.0 )
		m_instruc_TimedUseEBoxA.SetWindowText( "0" );

	OnKillfocusTimedUseEBoxB( );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnKillfocusTimedUseEBoxB( ) 
{
	CString str;
	
	m_instruc_TimedUseEBoxA.GetWindowText( str );
	strcpy( m_sbuffer, str );
	double A = atof( m_sbuffer );
	
	m_instruc_TimedUseEBoxB.GetWindowText( str );
	strcpy( m_sbuffer, str );
	double B = atof( m_sbuffer );
	
	if ( B < A )
	{
		sprintf( m_sbuffer, "%f", A );
		m_instruc_TimedUseEBoxB.SetWindowText( m_sbuffer );
	}
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnKillfocusJumpEBoxA( ) 
{
	CString str;
	
	m_warn_JumpEBoxA.GetWindowText( str );
	strcpy( m_sbuffer, str );
	int A = atoi( m_sbuffer );
	
	if ( A < 0 )
		m_warn_JumpEBoxA.SetWindowText( "0" );

	OnKillfocusJumpEBoxB( );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnKillfocusJumpEBoxB( ) 
{
	CString str;
	
	m_warn_JumpEBoxA.GetWindowText( str );
	strcpy( m_sbuffer, str );
	int A = atoi( m_sbuffer );
	
	m_warn_JumpEBoxB.GetWindowText( str );
	strcpy( m_sbuffer, str );
	int B = atoi( m_sbuffer );
	
	if ( B < A )
		m_warn_JumpEBoxB.SetWindowText( itoa( A, m_sbuffer, 10 ) );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnKillfocusSimMouseEBoxA( ) 
{	
	CString str;
	
	m_instruc_SimMouseEBoxA.GetWindowText( str );
	strcpy( m_sbuffer, str );
	int A = atoi( m_sbuffer );
	
	if ( A < -1 )
		m_instruc_SimMouseEBoxA.SetWindowText( "0" );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnKillfocusSimMouseEBoxB( ) 
{
	CString str;
	
	m_instruc_SimMouseEBoxB.GetWindowText( str );
	strcpy( m_sbuffer, str );
	int A = atoi( m_sbuffer );
	
	if ( A < -1 )
		m_instruc_SimMouseEBoxB.SetWindowText( "0" );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnKillfocusSurfMinEBox( ) 
{
	CString str;
	
	m_safety_SurfMinEBox.GetWindowText( str );
	strcpy( m_sbuffer, str );
	int A = atoi( m_sbuffer );
	
	if ( A < -1 )
		m_safety_SurfMinEBox.SetWindowText( "0" );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnKillfocusSurfMaxEBox( ) 
{
	CString str;
	
	m_safety_SurfMinEBox.GetWindowText( str );
	strcpy( m_sbuffer, str );
	int A = atoi( m_sbuffer );
	
	m_safety_SurfMaxEBox.GetWindowText( str );
	strcpy( m_sbuffer, str );
	int B = atoi( m_sbuffer );
	
	if ( B < A )
		m_safety_SurfMaxEBox.SetWindowText( itoa( A, m_sbuffer, 10 ) );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnClearReturn( ) 
{
	//m_instruc_ClearReturn.SetCheck( 1 );
	m_instruc_SimMouseCBox.SetCheck( 0 );
	m_instruc_SimKeyCBox.SetCheck( 0 );
	m_instruc_SendPacketCBox.SetCheck( 0 );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnSimKeyCBox( ) 
{
	m_instruc_ClearReturn.SetCheck( 0 );
	m_instruc_SimMouseCBox.SetCheck( 0 );
	//m_instruc_SimKeyCBox.SetCheck( 1 );
	m_instruc_SendPacketCBox.SetCheck( 0 );

}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnSimMouseCBox( ) 
{
	m_instruc_ClearReturn.SetCheck( 0 );
	//m_instruc_SimMouseCBox.SetCheck( 1 );
	m_instruc_SimKeyCBox.SetCheck( 0 );
	m_instruc_SendPacketCBox.SetCheck( 0 );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnSendPacketCBox() 
{
	m_instruc_ClearReturn.SetCheck( 0 );
	m_instruc_SimMouseCBox.SetCheck( 0 );
	m_instruc_SimKeyCBox.SetCheck( 0 );
	//m_instruc_SendPacketCBox.SetCheck( 1 );	
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnminimumCBox( ) 
{
	int sel = m_list_DisplayScript.GetCurSel( );

	m_time_randomCBox.SetCheck( 0 );
	m_time_maximumCBox.SetCheck( 0 );
	m_time_minimumCBox.SetCheck( 1 );

	UpdateDisplay( );

	m_list_DisplayScript.SetCurSel( sel );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnmaximumCBox( ) 
{
	int sel = m_list_DisplayScript.GetCurSel( );

	m_time_randomCBox.SetCheck( 0 );
	m_time_minimumCBox.SetCheck( 0 );
	m_time_maximumCBox.SetCheck( 1 );

	UpdateDisplay( );

	m_list_DisplayScript.SetCurSel( sel );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnrandomCBox( ) 
{
	int sel = m_list_DisplayScript.GetCurSel( );

	m_time_randomCBox.SetCheck( 1 );
	m_time_maximumCBox.SetCheck( 0 );
	m_time_minimumCBox.SetCheck( 0 );

	UpdateDisplay( );

	m_list_DisplayScript.SetCurSel( sel );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::Oncomment( ) 
{
	int sel = m_list_DisplayScript.GetCurSel( );

	UpdateDisplay( );

	m_list_DisplayScript.SetCurSel( sel );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnJalCBox( ) 
{
	if ( m_warn_JumpCBox.GetCheck( ) == 0 )
		m_warn_JalCBox.SetCheck( 0 );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::FirstSetup( ) 
{
	m_oFirst = new Instruction( );

	m_oCurrent = m_oFirst;

	m_iCount++;
	
	m_oFirst->set_warnings_audioFolder( m_sStartPath );
	m_oFirst->set_tracker_jumpA( m_oFirst );
	m_oFirst->set_tracker_jumpB( m_oFirst );

	UpdateDisplay( );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::ClearLinkedItem( int deleteable ) 
{
	Instruction* clearable = m_oFirst;

	while ( clearable )
	{
		if ( clearable->get_instruc_position( ) == deleteable )
		{
			clearable->reset( );
			clearable->set_instruc_isSpecial( 1 );
			clearable->set_instruc_comment( "linked_by_a_jump ( empty )" );
			clearable->set_warnings_audioFolder( m_sStartPath );
			clearable->set_tracker_jumpA( m_oFirst );
			clearable->set_tracker_jumpB( m_oFirst );

			UpdateDisplay( );

			m_list_DisplayScript.SetCurSel( deleteable );

			OnViewButton( );

			return;
		}

		clearable = clearable->get_next( );
	}

}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnDeleteButton( ) 
{
	DeleteItem( false );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::DeleteItem( bool all ) 
{
	if ( !all && m_iCount == 1 )
	{
		ResetScript( false );
		return;
	}

	Instruction* current = m_oFirst;
	int delItem = m_list_DisplayScript.GetCurSel( );

	if ( !all )
	{
		Instruction* savePos = m_oFirst;
		while ( savePos )
		{
			if ( savePos->get_warnings_jump( ) == 1 &&
				( ( savePos->get_tracker_jumpA( ) )->get_instruc_position( ) == delItem ||
				( savePos->get_tracker_jumpB( ) )->get_instruc_position( ) == delItem ) )
			{
				ClearLinkedItem( delItem );
				return;
			}

			savePos = savePos->get_next( );
		}
	}

	for ( int i = 0; i < delItem; i++ )
		current = current->get_next( );
	
	Instruction* next;
	Instruction* prev;

	if ( !all )
	{	
		next = current->get_next( );
		prev = current->get_previous( );

		if ( next )
		{
			next->set_previous( prev );
			if ( current == m_oFirst ) { m_oFirst = next; }
		}

		if ( prev )
		{
			prev->set_next( next );
			if ( current == m_oFirst ) { m_oFirst = prev; }
		}
	}
		
	delete current;

	m_iCount--;
		
	if ( !all )
	{
		UpdateDisplay( );
			
		m_list_DisplayScript.SetCurSel( 0 );
		if ( next ) { m_list_DisplayScript.SetCurSel( delItem ); }
		if ( prev ) { m_list_DisplayScript.SetCurSel( delItem-1 ); }

		OnViewButton( );
	}
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnCopyButton( ) 
{
	Instruction* current = m_oFirst;
	int delItem = m_list_DisplayScript.GetCurSel( );

	for ( int i = 0; i < delItem; i++ )
		current = current->get_next( );
		
	Instruction* newi = new Instruction( );

	Instruction* last = m_oFirst;
	for ( int i = 0; i < m_iCount-1; i++ )
		last = last->get_next( );
		
	last->set_next( newi );
	newi->set_previous( last );

	newi->set_instruc_isSpecial( current->get_instruc_isSpecial( ) );
	newi->set_instruc_position( last->get_instruc_position( ) + 1 );
	newi->set_instruc_useKeystroke( current->get_instruc_useKeystroke( ) );
	newi->set_instruc_keystrokeName( current->get_instruc_keystrokeName( ) );
	newi->set_instruc_keystrokeCode( current->get_instruc_keystrokeCode( ) );
	newi->set_instruc_UNKNOWN_keystrokeCode( current->get_instruc_UNKNOWN_keystrokeCode( ) );
	newi->set_instruc_useMouseClick( current->get_instruc_useMouseClick( ) );
	newi->set_instruc_mouseClickX( current->get_instruc_mouseClickX( ) );
	newi->set_instruc_mouseClickY( current->get_instruc_mouseClickY( ) );
	newi->set_instruc_sendPacket( current->get_instruc_sendPacket( ) );
	newi->set_instruc_packetFile( current->get_instruc_packetFile( ) );
	newi->set_instruc_packetFolder( current->get_instruc_packetFolder( ) );
	newi->set_safety_useCoordCheck( current->get_safety_useCoordCheck( ) );
	newi->set_safety_useCoordCheckcon( current->get_safety_useCoordCheckcon( ) );
	newi->set_safety_CoordX( current->get_safety_CoordX( ) );
	newi->set_safety_CoordY( current->get_safety_CoordY( ) );
	newi->set_instruc_pauseA( current->get_instruc_pauseA( ) );
	newi->set_instruc_pauseB( current->get_instruc_pauseB( ) );
	newi->set_instruc_comment( current->get_instruc_comment( ) );
	newi->set_instruc_useTimedUse( current->get_instruc_useTimedUse( ) );
	newi->set_instruc_TimedUseA( current->get_instruc_TimedUseA( ) );
	newi->set_instruc_TimedUseB( current->get_instruc_TimedUseB( ) );
	newi->set_instruc_notFirstTimedUse( current->get_instruc_notFirstTimedUse( ) );
	newi->set_safety_surfMin( current->get_safety_surfMin( ) );
	newi->set_safety_surfMax( current->get_safety_surfMax( ) );
	newi->set_safety_useSurfCheck( current->get_safety_useSurfCheck( ) );
	newi->set_safety_useSurfCheckcon( current->get_safety_useSurfCheckcon( ) );
	newi->set_warnings_takeScreenshot( current->get_warnings_takeScreenshot( ) );
	newi->set_warnings_playAudio( current->get_warnings_playAudio( ) );
	newi->set_warnings_audioFile( current->get_warnings_audioFile( ) );
	newi->set_warnings_audioFolder( current->get_warnings_audioFolder( ) );
	newi->set_warnings_jump( current->get_warnings_jump( ) );
	newi->set_tracker_jumpA( current->get_tracker_jumpA( ) );
	newi->set_tracker_jumpB( current->get_tracker_jumpB( )  ); 
	newi->set_warnings_jal( current->get_warnings_jal( ) );
	newi->set_safety_useHPWait( current->get_safety_useHPWait( ) );
	newi->set_safety_useMPWait( current->get_safety_useMPWait( ) );
	newi->set_warnings_loggOff( current->get_warnings_loggOff( ) );

	m_iCount++;
		
	UpdateDisplay( );
		
	m_list_DisplayScript.SetCurSel( delItem+1 );

	OnViewButton( );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::ResetPositions( ) 
{
	Instruction* current = m_oFirst;
	
	for ( int k = 0; k < m_iCount; k++ )
	{
		current->set_instruc_position( k );
		current = current->get_next( );
	}
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnMvUpButton( ) 
{
	Instruction* current = m_oFirst;
	int movItem = m_list_DisplayScript.GetCurSel( );
	
	if ( movItem > 0 )
	{
		for ( int i = 0; i < movItem; i++ )
			current = current->get_next( );
		
		Instruction* next = current->get_next( );
		Instruction* prev = current->get_previous( );
		
		if ( prev )
		{
			if ( next ) { next->set_previous( prev ); }
			
			prev->set_next( next );
			
			if ( prev->get_previous( ) ) { ( prev->get_previous( ) )->set_next( current ); }
			
			current->set_previous( prev->get_previous( ) );
			
			prev->set_previous( current );
			
			current->set_next( prev );
		}

		if ( prev == m_oFirst ) { m_oFirst = current; }

		UpdateDisplay( );
		
		m_list_DisplayScript.SetCurSel( movItem-1 );

		OnViewButton( );
	}
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnMvDownButton( ) 
{
	Instruction* current = m_oFirst;
	int movItem = m_list_DisplayScript.GetCurSel( );
	
	if ( movItem < m_iCount-1 )
	{
		for ( int i = 0; i < movItem; i++ )
			current = current->get_next( );
		
		Instruction* next = current->get_next( );
		Instruction* prev = current->get_previous( );
		
		if ( next )
		{
			if ( prev )
				prev->set_next( next );
			
			next->set_previous( prev );
			
			if ( next->get_next( ) )
				( next->get_next( ) )->set_previous( current );
			
			current->set_next( next->get_next( ) );
			
			next->set_next( current );
			
			current->set_previous( next );
		}

		UpdateDisplay( );
		
		m_list_DisplayScript.SetCurSel( movItem+1 );

		OnViewButton( );
	}
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnSaveButton( ) 
{
	SetCurrentDirectory( m_sStartPath );

	saveInstruction( );

	CString str;
	char fname[256];

	m_list_SaveNameComBox.GetWindowText( str );

	if ( strcmp( str, "" ) == 0 || strcmp( str, "untitled" ) == 0 )
		return;

	strcpy( fname, str );
	strcat( fname, ".trans" );

	FILE* out;
	out = fopen( fname, "w" );

	if ( out )
	{
		fprintf( out, "m_iCount=%i\n", m_iCount );

		m_screen_sizeA.GetWindowText( str );
		strcpy( m_sbuffer, str );
		fprintf( out, "screen_sizeA=%i\n", atoi( m_sbuffer ) );
		
		m_screen_sizeB.GetWindowText( str );
		strcpy( m_sbuffer, str );
		fprintf( out, "screen_sizeB=%i\n", atoi( m_sbuffer ) );
		
		fprintf( out, "Health=%i\n", m_monitor_HealthCBox.GetCheck( ) );
		
		m_monitor_HealthPercentEBox.GetWindowText( str );
		strcpy( m_sbuffer, str );
		fprintf( out, "HealthPercent=%i\n", atoi( m_sbuffer ) );
		
		fprintf( out, "HealthCode=%i\n", convertDIK_Key( m_monitor_HealthComBox.GetCurSel( ), false ) );

		fprintf( out, "Mana=%i\n", m_monitor_ManaCBox.GetCheck( ) );
		
		m_monitor_ManaPercentEBox.GetWindowText( str );
		strcpy( m_sbuffer, str );
		fprintf( out, "ManaPercent=%i\n", atoi( m_sbuffer ) );
		
		m_AdapterComBox.GetWindowText( str );
		strcpy( m_sbuffer, str );
		fprintf( out, "NetworkAdapter=%s\n", m_sbuffer );

		fprintf( out, "ManaCode=%i\n\n\n", convertDIK_Key( m_monitor_ManaComBox.GetCurSel( ), false ) );
	
		Instruction* current = m_oFirst;
		for ( int k = 0; current && k < m_iCount; k++ )
		{
			fprintf( out, "isSpecial=%i\nposition=%i\ncomment=%s\nclearReturnStack=%i\nuseKeystroke=%i\nkeystrokeName=%s\nkeystrokeCode=%i\n",
				current->get_instruc_isSpecial( ),
				current->get_instruc_position( ),
				current->get_instruc_comment( ),
				current->get_instruc_clearReturnStack( ),
				current->get_instruc_useKeystroke( ),
				current->get_instruc_keystrokeName( ),
				current->get_instruc_keystrokeCode( ) );
	
			if ( current->get_instruc_mouseClickX( ) < 0 || current->get_instruc_mouseClickY( ) < 0 )
				fprintf( out, "useMouseClick=%i\n", 2 );
			else
				fprintf( out, "useMouseClick=%i\n", current->get_instruc_useMouseClick( ) );

			fprintf( out, "mouseClickX=%i\nmouseClickY=%i\nsendPacket=%i\npacketFile=%s\npacketFolder=%s\nuseCoordCheck=%i\nuseCoordCheckcon=%i\nCoordX=%i\nCoordY=%i\npauseA=%f\npauseB=%f\nuseTimedUse=%i\nTimedUseA=%f\nTimedUseB=%f\nnotFirstTimedUse=%i\nsurfMin=%i\nsurfMax=%i\nuseSurfCheck=%i\nuseSurfCheckcon=%i\nuseHPWait=%i\nuseMPWait=%i\ntakeScreenshot=%i\nplayAudio=%i\naudioFile=%s\naudioFolder=%s\njump=%i\njumpA=%i\njumpB=%i\njal=%i\nloggOff=%i\n\n\n",
				current->get_instruc_mouseClickX( ),
				current->get_instruc_mouseClickY( ),
				current->get_instruc_sendPacket( ),
				current->get_instruc_packetFile( ),
				current->get_instruc_packetFolder( ),
				current->get_safety_useCoordCheck( ),
				current->get_safety_useCoordCheckcon( ),
				current->get_safety_CoordX( ),
				current->get_safety_CoordY( ),
				current->get_instruc_pauseA( ),
				current->get_instruc_pauseB( ),
				current->get_instruc_useTimedUse( ),
				current->get_instruc_TimedUseA( ),
				current->get_instruc_TimedUseB( ),
				current->get_instruc_notFirstTimedUse( ),
				current->get_safety_surfMin( ),
				current->get_safety_surfMax( ),
				current->get_safety_useSurfCheck( ),
				current->get_safety_useSurfCheckcon( ),
				current->get_safety_useHPWait( ),
				current->get_safety_useMPWait( ),
				current->get_warnings_takeScreenshot( ),
				current->get_warnings_playAudio( ),
				current->get_warnings_audioFile( ),
				current->get_warnings_audioFolder( ),
				current->get_warnings_jump( ),
				( current->get_tracker_jumpA( ) )->get_instruc_position( ),
				( current->get_tracker_jumpB( ) )->get_instruc_position( ),
				current->get_warnings_jal( ),
				current->get_warnings_loggOff( ) );
			
			current = current->get_next( );
		}

		fprintf( out, "endfile" );

		fclose( out );
	}

	SearchFiles( 0 );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnLoadButton( ) 
{
	CString str;
	m_list_SaveNameComBox.GetWindowText( str );

	if ( strcmp( str, "" ) == 0 || strcmp( str, "untitled" ) == 0 )
		return;

	ReadFile( );

	m_list_DisplayScript.SetCurSel( 0 );

	OnViewButton( );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnBeginButton( )
{
	ResetScript( true );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::ResetScript( bool all )
{
	EmptyScriptList( );
	FirstSetup( );

	if ( all )
	{
		m_screen_sizeA.SetWindowText( "0" );
		m_screen_sizeB.SetWindowText( "0" );
		m_monitor_HealthCBox.SetCheck( 0 );
		m_monitor_HealthPercentEBox.SetWindowText( "0" );
		m_monitor_ManaCBox.SetCheck( 0 );
		m_monitor_ManaPercentEBox.SetWindowText( "0" );
	}

	m_list_DisplayScript.SetCurSel( 0 );
	OnViewButton( );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::EmptyScriptList( )
{
	for ( int i = m_iCount-1; i >= 0; i-- )
	{
		m_list_DisplayScript.SetCurSel( i );
		DeleteItem( true );
	}
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnSelchangeSimKeyComBox( ) 
{
	int cursel = m_instruc_SimKeyComBox.GetCurSel( );
	if ( DIK_Keys[cursel] == -1 && cursel != 0 )
	{
		m_instruc_SimKeyComBox.SetCurSel( 0 );
		return;
	}


	if ( DIK_Keys[cursel] == -2 )
	{
		for ( int i = 0; i < keyCount; i++ )
			if ( m_oCurrent->get_instruc_UNKNOWN_keystrokeCode( ) == DIK_Keys[i] )
			{
				m_instruc_SimKeyComBox.SetCurSel( convertInput( m_oCurrent->get_instruc_UNKNOWN_keystrokeCode( ), true ) );
				return;
			}

		if ( m_oCurrent->get_instruc_UNKNOWN_keystrokeCode( ) < 0 )
		{
			m_instruc_SimKeyComBox.SetCurSel( 0 );
			return;
		}
	}
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnSelchangeHealthComBox( ) 
{
	int cursel = m_monitor_HealthComBox.GetCurSel( );
	if ( DIK_Keys[cursel] == -1 && cursel != 0 )
		m_monitor_HealthComBox.SetCurSel( 0 );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnSurfCheckCBox( ) 
{
	m_safety_SurfCheckconCBox.EnableWindow( m_safety_SurfCheckCBox.GetCheck( ) );
	m_safety_SurfCheckconCBox.SetCheck( 0 );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnCoordCheckCBox( ) 
{
	m_safety_CoordCheckconCBox.EnableWindow( m_safety_CoordCheckCBox.GetCheck( ) );
	m_safety_CoordCheckconCBox.SetCheck( 0 );	
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::ReadFile( ) 
{
	SetCurrentDirectory( m_sStartPath );

	std::ifstream inFile;

	CString str;
	char fname[256];

	m_list_SaveNameComBox.GetWindowText( str );

	strcpy( m_sLastOpened, str );
	strcpy( fname, str );
	strcat( fname, ".trans" );

	inFile.open( fname, std::ios::in | std::ios::binary );
	
	if ( inFile )
	{
		std::string strBuffer;
		std::getline( inFile, strBuffer );

		EmptyScriptList( );
		m_oFirst = new Instruction( );
		Instruction* current = m_oFirst;
		Instruction* prev = NULL;

		char strData[256];

		while ( !inFile.eof( ) )
		{
			// get the key
			std::string strKey;
			int pos = 0;
			strKey = strBuffer.substr( 0, strBuffer.find_first_of( " =\t\n\r", pos ) );
			pos += strKey.size( );

			if ( pos > 0 )
			{
				// get the '=' character
				pos = strBuffer.find_first_not_of( " \t", pos );
				
				// find the m_oFirst non-whitespace character after the '=' character
				pos = strBuffer.find_first_not_of( " \t", pos + 1 );

				// get the data
				std::string strD;
				strD = strBuffer.substr( pos, strBuffer.find_first_of( "\n\r", pos ) - pos );
				
				strcpy( strData, strD.c_str( ) );
				
				if( strKey == "screen_sizeA" )
					m_screen_sizeA.SetWindowText( strData );
				
				else if ( strKey == "screen_sizeB" )
					m_screen_sizeB.SetWindowText( strData );
				
				else if ( strKey == "Health" )
					m_monitor_HealthCBox.SetCheck( atoi( strData ) );
				
				else if ( strKey == "HealthPercent" )
					m_monitor_HealthPercentEBox.SetWindowText( strData );
				
				else if ( strKey == "HealthCode" )
					m_monitor_HealthComBox.SetCurSel( convertInput( atoi( strData ), false ) );
				
				else if ( strKey == "Mana" )
					m_monitor_ManaCBox.SetCheck( atoi( strData ) );
				
				else if ( strKey == "ManaPercent" )
					m_monitor_ManaPercentEBox.SetWindowText( strData );
				
				else if ( strKey == "ManaCode" )
					m_monitor_ManaComBox.SetCurSel( convertInput( atoi( strData ), false ) );
				
				else if ( strKey == "isSpecial" )
				{
					if ( prev )
					{
						current = new Instruction;
						prev->set_next( current );
					}
					
					current->set_previous( prev );
					prev = current;
					
					current->set_instruc_position( m_iCount );
					current->set_instruc_isSpecial( atoi( strData ) );
					m_iCount++;
				}
				
				else if ( strKey == "clearReturnStack" )
					current->set_instruc_clearReturnStack( atoi( strData ) );

				else if ( strKey == "useKeystroke" )
					current->set_instruc_useKeystroke( atoi( strData ) );
				
				else if ( strKey == "keystrokeName" )
					current->set_instruc_keystrokeName( strData );
				
				else if ( strKey == "keystrokeCode" )
				{
					current->set_instruc_keystrokeCode( atoi( strData ) );
					current->set_instruc_UNKNOWN_keystrokeCode( atoi( strData ) );
				}
				
				else if ( strKey == "useMouseClick" )
					current->set_instruc_useMouseClick( atoi( strData ) );
				
				else if ( strKey == "mouseClickX" )
					current->set_instruc_mouseClickX( atoi( strData ) );
				
				else if ( strKey == "mouseClickY" )
					current->set_instruc_mouseClickY( atoi( strData ) );
				
				else if ( strKey == "sendPacket" )
					current->set_instruc_sendPacket( atoi( strData ) );
				
				else if ( strKey == "packetFile" )
					current->set_instruc_packetFile( strData );
				
				else if ( strKey == "packetFolder" )
					current->set_instruc_packetFolder( strData );
				
				else if ( strKey == "NetworkAdapter" )
					m_AdapterComBox.SetWindowText( strData );
				
				else if ( strKey == "useCoordCheck" )
					current->set_safety_useCoordCheck( atoi( strData ) );
				
				else if ( strKey == "useCoordCheckcon" )
					current->set_safety_useCoordCheckcon( atoi( strData ) );
				
				else if ( strKey == "CoordX" )
					current->set_safety_CoordX( atoi( strData ) );
				
				else if ( strKey == "CoordY" )
					current->set_safety_CoordY( atoi( strData ) );
				
				else if ( strKey == "pauseA" )
					current->set_instruc_pauseA( atof( strData ) );
				
				else if ( strKey == "pauseB" )
					current->set_instruc_pauseB( atof( strData ) );
				
				else if ( strKey == "comment" )
					current->set_instruc_comment( strData );
				
				else if ( strKey == "useTimedUse" )
					current->set_instruc_useTimedUse( atoi( strData ) );
				
				else if ( strKey == "TimedUseA" )
					current->set_instruc_TimedUseA( atof( strData ) );
				
				else if ( strKey == "TimedUseB" )
					current->set_instruc_TimedUseB( atof( strData ) );
				
				else if ( strKey == "notFirstTimedUse" )
					current->set_instruc_notFirstTimedUse( atoi( strData ) );
				
				else if ( strKey == "surfMin" )
					current->set_safety_surfMin( atoi( strData ) );
				
				else if ( strKey == "surfMax" )
					current->set_safety_surfMax( atoi( strData ) );
				
				else if ( strKey == "useSurfCheck" )
					current->set_safety_useSurfCheck( atoi( strData ) );
				
				else if ( strKey == "useSurfCheckcon" )
					current->set_safety_useSurfCheckcon( atoi( strData ) );
				
				else if ( strKey == "useHPWait" )
					current->set_safety_useHPWait( atoi( strData ) );
				
				else if ( strKey == "useMPWait" )
					current->set_safety_useMPWait( atoi( strData ) );
				
				else if ( strKey == "takeScreenshot" )
					current->set_warnings_takeScreenshot( atoi( strData ) );
				
				else if ( strKey == "playAudio" )
					current->set_warnings_playAudio( atoi( strData ) );
				
				else if ( strKey == "audioFile" )
					current->set_warnings_audioFile( strData );
				
				else if ( strKey == "audioFolder" )
					current->set_warnings_audioFolder( strData );
				
				else if ( strKey == "jump" )
					current->set_warnings_jump( atoi( strData ) );
				
				else if ( strKey == "jumpA" )
					current->set_warnings_jumpA( atoi( strData ) );
				
				else if ( strKey == "jumpB" )
					current->set_warnings_jumpB( atoi( strData ) );
				
				else if ( strKey == "jal" )
					current->set_warnings_jal( atoi( strData ) );
				
				else if ( strKey == "loggOff" )
					current->set_warnings_loggOff( atoi( strData ) );
			}

			std::getline( inFile, strBuffer );
		}
	}

	UpdateDisplay( );

	ResetJumpTrackers( m_oFirst, true );

	//if ( m_oCurrent )
	//	SetCurrentDirectory( m_oCurrent->get_warnings_audioFolder( ) );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
int CTransformerDlg::convertDIK_Key( int list_i, bool notHealth ) 
{
	if ( list_i < keyCount ) return (int)DIK_Keys[list_i];

	if ( notHealth && list_i == keyCount-1 )
		return m_oCurrent->get_instruc_UNKNOWN_keystrokeCode( );
	
	return -1;
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
int CTransformerDlg::convertInput( int key_i, bool notHealth ) 
{
	for ( int i = 0; i < keyCount; i++ )
		if ( key_i == DIK_Keys[i] ) return i;

	if ( notHealth && key_i >= 0 )
		return keyCount-1;

	return 0;
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
void CTransformerDlg::SearchAdapter( )
{
	m_AdapterComBox.ResetContent( );

	pcap_if_t *alldevs;
		
	if( pcap_findalldevs( &alldevs, NULL ) == -1 )
		return;
    
    for( ; alldevs; alldevs = alldevs->next )
		m_AdapterComBox.AddString( alldevs->name );

	m_AdapterComBox.SetCurSel( 0 );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::SearchFiles( int n )
{	
    //Declare all needed handles     
    WIN32_FIND_DATA FindFileData;
    HANDLE hFind;

	CString str;

	if ( n == 0 )
	{
		SetCurrentDirectory( m_sStartPath );
		m_list_SaveNameComBox.GetWindowText( str );
		m_list_SaveNameComBox.ResetContent( );
		hFind = FindFirstFile ( "*.trans", &FindFileData );
	}
	else if ( n == 1 )
	{
		SetCurrentDirectory( m_oCurrent->get_warnings_audioFolder( ) );
		m_warn_AudioComBox.ResetContent( );
		hFind = FindFirstFile ( "*.wav", &FindFileData );
	}
	else if ( n == 2 )
	{
		SetCurrentDirectory( m_oCurrent->get_instruc_packetFolder( ) );
		m_instruc_SendPackeTComBox.ResetContent( );
		hFind = FindFirstFile ( "*.cap", &FindFileData );
	}
	
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
    
			if ( n == 0 )
			{
				cutoff = 6;

				for ( int i = 0; i < sizeOf( tempb )-cutoff; i++ )
					tempa[i]= tempb[i] ;

				tempa[sizeOf( tempb )-cutoff]= NULL;
				m_list_SaveNameComBox.AddString( tempa ); 
			}
			else if ( n == 1 || n == 2 )
			{
				cutoff = 4;

				for ( int i = 0; i < sizeOf( tempb )-cutoff; i++ )
					tempa[i]= tempb[i] ;

				tempa[sizeOf( tempb )-cutoff]= NULL;

				if ( n == 1 )
					m_warn_AudioComBox.AddString( tempa );
				else if ( n == 2 )
					m_instruc_SendPackeTComBox.AddString( tempa );
			}
        }  
    }
    while ( FindNextFile ( hFind, &FindFileData ) 
        && hFind != INVALID_HANDLE_VALUE );
    FindClose ( hFind );

	if ( n == 0 )
	{
		m_list_SaveNameComBox.SetWindowText( str );
		//SetCurrentDirectory( m_sStartPath );
	}
	else if ( n == 1 )
	{
		m_warn_AudioComBox.SetWindowText( m_oCurrent->get_warnings_audioFile( ) );
		//SetCurrentDirectory( m_oCurrent->get_warnings_audioFolder( ) );
	}
	else if ( n == 2 )
	{
		m_instruc_SendPackeTComBox.SetWindowText( m_oCurrent->get_instruc_packetFile( ) );
		//SetCurrentDirectory( m_oCurrent->get_instruc_packetFolder( ) );
	}

	SetCurrentDirectory( m_sStartPath );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
int CTransformerDlg::sizeOf( char* str )
{
	int c = 0;

	for ( int i = 0; str[i] != '\0'; i++)
		c++;

	return c;
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnAudioBrowseButton( ) 
{
	Browse( m_sAudioBrowsedPath, 1 );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::OnSendPacketBrowseButton() 
{
	Browse( m_sPacketBrowsedPath, 2 );
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::Browse( char* BrowsedPath, int n ) 
{	
    BROWSEINFO bi = { 0 };
    bi.lpszTitle = ( "Select a folder." );
    LPITEMIDLIST pidl = SHBrowseForFolder ( &bi );
	
    if ( pidl != 0 )
    {
        //Get the name of the folder
        SHGetPathFromIDList ( pidl, m_sbuffer );

		strcpy( BrowsedPath, m_sbuffer );
		if ( m_sbuffer[ sizeOf( m_sbuffer ) - 1 ] != '\\' )
			strcat( BrowsedPath, "\\" );

		if ( n == 1 )
			m_oCurrent->set_warnings_audioFolder( BrowsedPath );
		else if ( n == 2 )
			m_oCurrent->set_instruc_packetFolder( BrowsedPath );

		//SetCurrentDirectory( BrowsedPath );
		SearchFiles( n );

        //Free memory used
        IMalloc * imalloc = 0;
        if ( SUCCEEDED( SHGetMalloc ( &imalloc ) ) )
        {
            imalloc->Free ( pidl );
            imalloc->Release ( );
        }
    }
}


//-------------------------------------------------------
// okay
//-------------------------------------------------------
void CTransformerDlg::ResetJumpTrackers( Instruction* start, bool all )
{
	while ( start )
	{
		Instruction* find = m_oFirst;

		while ( find )
		{
			if ( find->get_instruc_position( ) == start->get_warnings_jumpA( ) )
				start->set_tracker_jumpA( find );

			if ( find->get_instruc_position( ) == start->get_warnings_jumpB( ) )
				start->set_tracker_jumpB( find );

			find = find->get_next( );
		}
 
		start = start->get_next( );

		if ( !all )
			return;
	}
}
