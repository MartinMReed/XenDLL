// computron.h : header file
//

#ifndef COMPUTRON_H
#define COMPUTRON_H


class Instruction
{
public:

	Instruction ( )
	{
		reset( );

		previous = NULL;
		next = NULL;

		tracker_jumpA = NULL;
		tracker_jumpB = NULL;
	}

	void reset ( )
	{
		TimedUseLength = 0.0;
		TimedUseStart = 0.0;

		instruc_isSpecial = 0;
		instruc_position = 0;
		instruc_clearReturnStack = 0;
		instruc_useKeystroke = 0;
		strcpy( instruc_keystrokeName, "" );
		instruc_keystrokeCode = -1;
		instruc_UNKNOWN_keystrokeCode = -1;
		instruc_useMouseClick = 0;
		instruc_mouseClickX = 0;
		instruc_mouseClickY = 0;
		instruc_sendPacket = 0;
		strcpy( instruc_packetFile, "" );
		strcpy( instruc_packetFolder, "" );
		instruc_pauseA = 0.0;
		instruc_pauseB = 0.0;
		strcpy( instruc_comment, "" );
		instruc_useTimedUse = 0;
		instruc_TimedUseA = 0.0;
		instruc_TimedUseB = 0.0;
		instruc_notFirstTimedUse = 0;
		safety_surfMin = 0;
		safety_surfMax = 0;
		safety_useSurfCheck = 0;
		safety_useSurfCheckcon = 0;
		safety_useCoordCheck = 0;
		safety_useCoordCheckcon = 0;
		safety_CoordX = 0;
		safety_CoordY = 0;
		safety_useMPWait = 0;
		safety_useHPWait = 0;
		warnings_takeScreenshot = 0;
		warnings_playAudio = 0;
		strcpy( warnings_audioFile, "" );
		strcpy( warnings_audioFolder, "" );
		warnings_jump = 0;
		warnings_jumpA = 0;
		warnings_jumpB = 0;
		warnings_jal = 0;
		warnings_loggOff = 0;
	}

	double TimedUseLength;
	double get_TimedUseLength( ) { return TimedUseLength; }
	void set_TimedUseLength( double a ) { TimedUseLength = a; }

	double TimedUseStart;
	double get_TimedUseStart( ) { return TimedUseStart; }
	void set_TimedUseStart( double a ) { TimedUseStart = a; }

	//Scrip instruction
	/*bool*/int instruc_isSpecial;
	/*bool*/int get_instruc_isSpecial( ) { return instruc_isSpecial; }
	void set_instruc_isSpecial( int a ) { instruc_isSpecial = a; }

	int instruc_position;
	int get_instruc_position( ) { return instruc_position; }
	void set_instruc_position( int a ) { instruc_position = a; }

	/*bool*/int instruc_clearReturnStack;
	/*bool*/int get_instruc_clearReturnStack( ) { return instruc_clearReturnStack; }
	void set_instruc_clearReturnStack( int a ) { instruc_clearReturnStack = a; }

	/*bool*/int instruc_useKeystroke;
	/*bool*/int get_instruc_useKeystroke( ) { return instruc_useKeystroke; }
	void set_instruc_useKeystroke( int a ) { instruc_useKeystroke = a; }

	char instruc_keystrokeName[256];
	char* get_instruc_keystrokeName( ) { return instruc_keystrokeName; }
	void set_instruc_keystrokeName( char* a ) { strcpy( instruc_keystrokeName, a ); }

	int instruc_keystrokeCode;
	int get_instruc_keystrokeCode( ) { return instruc_keystrokeCode; }
	void set_instruc_keystrokeCode( int a ) { instruc_keystrokeCode = a; }

	int instruc_UNKNOWN_keystrokeCode;
	int get_instruc_UNKNOWN_keystrokeCode( ) { return instruc_UNKNOWN_keystrokeCode; }
	void set_instruc_UNKNOWN_keystrokeCode( int a ) { instruc_UNKNOWN_keystrokeCode = a; }

	/*bool*/int instruc_useMouseClick;
	/*bool*/int get_instruc_useMouseClick( ) { return instruc_useMouseClick; }
	void set_instruc_useMouseClick( int a ) { instruc_useMouseClick = a; }

	int instruc_mouseClickX;
	int get_instruc_mouseClickX( ) { return instruc_mouseClickX; }
	void set_instruc_mouseClickX( int a ) { instruc_mouseClickX = a; }

	int instruc_mouseClickY;
	int get_instruc_mouseClickY( ) { return instruc_mouseClickY; }
	void set_instruc_mouseClickY( int a ) { instruc_mouseClickY = a; }

	/*bool*/int instruc_sendPacket;
	/*bool*/int get_instruc_sendPacket( ) { return instruc_sendPacket; }
	void set_instruc_sendPacket( int a ) { instruc_sendPacket = a; }

	char instruc_packetFile[256];
	char* get_instruc_packetFile( ) { return instruc_packetFile; }
	void set_instruc_packetFile( char* a ) { strcpy( instruc_packetFile, a ); }

	char instruc_packetFolder[256];
	char* get_instruc_packetFolder( ) { return instruc_packetFolder; }
	void set_instruc_packetFolder( char* a ) { strcpy( instruc_packetFolder, a ); }

	double instruc_pauseA;
	double get_instruc_pauseA( ) { return instruc_pauseA; }
	void set_instruc_pauseA( double a ) { instruc_pauseA = a; }

	double instruc_pauseB;
	double get_instruc_pauseB( ) { return instruc_pauseB; }
	void set_instruc_pauseB( double a ) { instruc_pauseB = a; }

	char instruc_comment[256];
	char* get_instruc_comment( ) { return instruc_comment; }
	void set_instruc_comment( char* a ) { strcpy( instruc_comment, a ); }

	/*bool*/int instruc_useTimedUse;
	/*bool*/int get_instruc_useTimedUse( ) { return instruc_useTimedUse; }
	void set_instruc_useTimedUse( int a ) { instruc_useTimedUse = a; }

	double instruc_TimedUseA;
	double get_instruc_TimedUseA( ) { return instruc_TimedUseA; }
	void set_instruc_TimedUseA( double a ) { instruc_TimedUseA = a; }

	double instruc_TimedUseB;
	double get_instruc_TimedUseB( ) { return instruc_TimedUseB; }
	void set_instruc_TimedUseB( double a ) { instruc_TimedUseB = a; }

	/*bool*/int instruc_notFirstTimedUse;
	/*bool*/int get_instruc_notFirstTimedUse( ) { return instruc_notFirstTimedUse; }
	void set_instruc_notFirstTimedUse( int a ) { instruc_notFirstTimedUse = a; }

	//Safety check
	int safety_surfMin;
	int get_safety_surfMin( ) { return safety_surfMin; }
	void set_safety_surfMin( int a ) { safety_surfMin = a; }

	int safety_surfMax;
	int get_safety_surfMax( ) { return safety_surfMax; }
	void set_safety_surfMax( int a ) { safety_surfMax = a; }

	/*bool*/int safety_useSurfCheck;
	/*bool*/int get_safety_useSurfCheck( ) { return safety_useSurfCheck; }
	void set_safety_useSurfCheck( int a ) { safety_useSurfCheck = a; }

	/*bool*/int safety_useSurfCheckcon;
	/*bool*/int get_safety_useSurfCheckcon( ) { return safety_useSurfCheckcon; }
	void set_safety_useSurfCheckcon( int a ) { safety_useSurfCheckcon = a; }

	/*bool*/int safety_useCoordCheck;
	/*bool*/int get_safety_useCoordCheck(void) { return safety_useCoordCheck; }
	void set_safety_useCoordCheck( int a ) { safety_useCoordCheck = a; }

	/*bool*/int safety_useCoordCheckcon;
	/*bool*/int get_safety_useCoordCheckcon(void) { return safety_useCoordCheckcon; }
	void set_safety_useCoordCheckcon( int a ) { safety_useCoordCheckcon = a; }

	int safety_CoordX;
	int get_safety_CoordX(void) { return safety_CoordX; }
	void set_safety_CoordX( int a ) { safety_CoordX = a; }

	int safety_CoordY;
	int get_safety_CoordY(void) { return safety_CoordY; }
	void set_safety_CoordY( int a ) { safety_CoordY = a; }

	/*bool*/int safety_useMPWait;
	/*bool*/int get_safety_useMPWait( ) { return safety_useMPWait; }
	void set_safety_useMPWait( int a ) { safety_useMPWait = a; }

	/*bool*/int safety_useHPWait;
	/*bool*/int get_safety_useHPWait( ) { return safety_useHPWait; }
	void set_safety_useHPWait( int a ) { safety_useHPWait = a; }

	//Warnings
	/*bool*/int warnings_takeScreenshot;
	/*bool*/int get_warnings_takeScreenshot( ) { return warnings_takeScreenshot; }
	void set_warnings_takeScreenshot( int a ) { warnings_takeScreenshot = a; }

	/*bool*/int warnings_playAudio;
	/*bool*/int get_warnings_playAudio( ) { return warnings_playAudio; }
	void set_warnings_playAudio( int a ) { warnings_playAudio = a; }

	char warnings_audioFile[256];
	char* get_warnings_audioFile( ) { return warnings_audioFile; }
	void set_warnings_audioFile( char* a ) { strcpy( warnings_audioFile, a ); }

	char warnings_audioFolder[256];
	char* get_warnings_audioFolder( ) { return warnings_audioFolder; }
	void set_warnings_audioFolder( char* a ) { strcpy( warnings_audioFolder, a ); }

	/*bool*/int warnings_jump;
	/*bool*/int get_warnings_jump( ) { return warnings_jump; }
	void set_warnings_jump( int a ) { warnings_jump = a; }

	int warnings_jumpA;
	int get_warnings_jumpA( ) { return warnings_jumpA; }
	void set_warnings_jumpA( int a ) { warnings_jumpA = a; }

	int warnings_jumpB;
	int get_warnings_jumpB( ) { return warnings_jumpB; }
	void set_warnings_jumpB( int a ) { warnings_jumpB = a; }

	/*bool*/int warnings_jal;
	/*bool*/int get_warnings_jal( ) { return warnings_jal; }
	void set_warnings_jal( int a ) { warnings_jal = a; }

	/*bool*/int warnings_loggOff;
	/*bool*/int get_warnings_loggOff( ) { return warnings_loggOff; }
	void set_warnings_loggOff( int a ) { warnings_loggOff = a; }



	Instruction* previous;
	Instruction* get_previous( ) { return previous; }
	void set_previous( Instruction* a ) { previous = a; }

	Instruction* next;
	Instruction* get_next( ) { return next; }
	void set_next( Instruction* a ) { next = a; }



	Instruction* tracker_jumpA;
	Instruction* get_tracker_jumpA( ) { return tracker_jumpA; }
	void set_tracker_jumpA( Instruction* a ) { tracker_jumpA = a; }

	Instruction* tracker_jumpB;
	Instruction* get_tracker_jumpB( ) { return tracker_jumpB; }
	void set_tracker_jumpB( Instruction* a ) { tracker_jumpB = a; }
};

#endif
