#ifndef Computron_H
#define Computron_H

#pragma once

#define SCRIPT_VERSION 2

#include <atltime.h>

#include <wininet.h>
#include <string>
using namespace std;

#include "Instruction.h"

class Computron;
class JReturnStack;

extern Computron* g_Computron;


//-------------------------------------------------------
// 
//-------------------------------------------------------
class Computron
{

public:

	bool initialized;
	bool Initialize(void);

	Computron(void);
	~Computron(void);

	// direct 3d device hook calls
	void IncrementSurfaceCount(UINT _count)
	{
		if (initialized == false) return;
		surfCount += _count;
	}

	// direct 3d device hook calls
	void ResetSurfaceCount(void)
	{
		if (initialized == false) return;
		surfSaved = surfCount;
		surfCount = 0;
	}

	// thread calls
	void MemoryCheckProc(void);
	void ScriptCycleProc(void);

	// extension calls
	void Render(void);
	bool HandleKeyboard(void);

private:

	bool logout;

	BYTE* g_progressHP;
	BYTE* g_progressMP;
	BYTE* g_progressXP;

	DWORD* g_locationX;
	DWORD* g_locationY;

	float progressHP;
	float progressMP;
	float progressXP;

	int locationX;
	int locationY;

	BYTE pauseRestartKey;
	BYTE pauseResumeKey;

	HGNodeList* extComputron;

	HGOImage* panel1;
	HGOTextBar* p1TextBar1;
	HGOListViewer* p1ListViewer1;
	HGOListViewer* p1ListViewer2;

	HINTERNET g_hInet;
	HINTERNET g_hInetConnection;
	
	bool captureScreen;
	bool zeroPause;
	bool okayHP;
	bool okayMP;
	bool checkconUsed;
	bool jjump;
	bool minuteJumpTriggered;
	bool transform;
	
	int surfCount;
	int surfSaved;

	int countTotal;
	int countSpecial;
	int countNormal;

	int ranTimes;

	float screenSizeA;
	float screenSizeB;
			
	bool useHealthKey;
	int healthPercentTrigger;
	BYTE healthMonitorKey;
			
	bool useManaKey;
	int manaPercentTrigger;
	BYTE manaMonitorKey;

	float totalPerc;
	float lastXP;
	float hrTillHundert;
	float percPerHr;
	float initPerc;
	float startPerc;
	float endPerc;
	
	char* scriptFilename;
	char* scriptHandle;
	char* scriptPassword;

	char* networkAdapter;
	
	float pauseLength;
	float pauseStart;
	
	Instruction* instrucHeader;
	Instruction* instrucCurrent;
	Instruction* instrucPrevious;
	
	JReturnStack* jreturnStack;

	string ReceiveData(char* _url);

	void DrawCursor(void);

	void SendCap(char* _file, char* _adapter, char* _s);

	void UseWarnings(char* _str, bool _justText = false, int _position = 0);
	void AddWarnLog(int _position, char* _log);
	void ClearWarnLog(void);

	void ContinCheckProc(void);
	void GetPause(int _i = 1);

	void NextNormalInstruction(void);
	void ClearInstructions(void);
	void ClearJReturnStack(void);
	void ResetTransform(void);
	void ReadScriptFile(void);

	bool Uninitialize(void);
};


//-------------------------------------------------------
// 
//-------------------------------------------------------
class JReturnStack
{
public:

	JReturnStack (JReturnStack* _next, int _jreturnFrom, Instruction* _jreturn)
	{
		next = _next;
		jreturnFrom = _jreturnFrom;
		jreturn = _jreturn;
	}

	JReturnStack* next;
	JReturnStack* GetNext(void) { return next; }

	int jreturnFrom;
	int GetJReturnFrom(void) { return jreturnFrom; }

	Instruction* jreturn;
	Instruction* GetJReturn(void) { return jreturn; }
};

#endif
