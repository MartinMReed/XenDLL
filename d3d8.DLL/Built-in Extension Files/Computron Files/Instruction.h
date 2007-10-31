#ifndef Instruction_H
#define Instruction_H

#pragma once


//-------------------------------------------------------
// 
//-------------------------------------------------------
class Instruction
{
public:

	Instruction(void)
	{
		timedUseLength = 0.0f;
		timedUseStart = 0.0f;

		instrucIsSpecial = false;
		instrucPosition = 0;
		instrucClearReturnStack = false;
		instrucUseKeystroke = false;
		instrucKeystrokeCode = -1;
		instrucUseMouseClick = false;
		instrucMouseClickX = 0;
		instrucMouseClickY = 0;
		instrucSendPacket = false;
		instrucPacketFile = "";
		instrucPauseA = 0.0f;
		instrucPauseB = 0.0f;
		instrucComment = "";
		instrucUseTimedUse = false;
		instrucTimedUseA = 0.0f;
		instrucTimedUseB = 0.0f;
		instrucNotFirstTimedUse = false;

		safetySurfMin = 0;
		safetySurfMax = 0;
		safetyUseSurfCheck = false;
		safetyUseSurfCheckcon = false;
		safetyUseCoordCheck = false;
		safetyUseCoordCheckcon = false;
		safetyCoordX = 0;
		safetyCoordY = 0;
		safetyUseMPWait = false;
		safetyUseHPWait = false;

		warningsTakeScreenshot = false;
		warningsPlayAudio = false;
		warningsAudioFile = "";
		warningsJump = false;
		warningsJumpA = 0;
		warningsJumpB = 0;
		warningsJAL = false;

		next = NULL;
	}

	~Instruction(void)
	{
		if (instrucPacketFile)
		{
			delete instrucPacketFile;
			instrucPacketFile = NULL;
		}

		if (instrucComment)
		{
			delete instrucComment;
			instrucComment = NULL;
		}

		if (warningsAudioFile)
		{
			delete warningsAudioFile;
			warningsAudioFile = NULL;
		}
	}

	float GetTimedUseLength(void) { return timedUseLength; }
	void SetTimedUseLength(float _var) { timedUseLength = _var; }

	float GetTimedUseStart(void) { return timedUseStart; }
	void SetTimedUseStart(float _var) { timedUseStart = _var; }

	Instruction* GetNext(void) { return next; }
	void SetNext(Instruction* _var) { next = _var; }

	int GetInstrucPosition(void) { return instrucPosition; }
	void SetInstrucPosition(int _var) { instrucPosition = _var; }

	// instruction
	bool GetInstrucIsSpecial(void) { return instrucIsSpecial; }
	void SetInstrucIsSpecial(bool _var) { instrucIsSpecial = _var; }

	bool GetInstrucClearReturnStack(void) { return instrucClearReturnStack; }
	void SetInstrucClearReturnStack(bool _var) { instrucClearReturnStack = _var; }

	bool GetInstrucUseKeystroke(void) { return instrucUseKeystroke; }
	void SetInstrucUseKeystroke(bool _var) { instrucUseKeystroke = _var; }

	int GetInstrucKeystrokeCode(void) { return instrucKeystrokeCode; }
	void SetInstrucKeystrokeCode(int _var) { instrucKeystrokeCode = _var; }

	bool GetInstrucUseMouseClick(void) { return instrucUseMouseClick; }
	void SetInstrucUseMouseClick(bool _var) { instrucUseMouseClick = _var; }

	int GetInstrucMouseClickX(void) { return instrucMouseClickX; }
	void SetInstrucMouseClickX(int _var) { instrucMouseClickX = _var; }

	int GetInstrucMouseClickY(void) { return instrucMouseClickY; }
	void SetInstrucMouseClickY(int _var) { instrucMouseClickY = _var; }

	bool GetInstrucSendPacket(void) { return instrucSendPacket; }
	void SetInstrucSendPacket(bool _var) { instrucSendPacket = _var; }

	char* GetInstrucPacketFile(void) { return instrucPacketFile; }
	void SetInstrucPacketFile(char* _var) { instrucPacketFile = _restring(_var, 0, strlen(_var)); }

	float GetInstrucPauseA(void) { return instrucPauseA; }
	void SetInstrucPauseA(float _var) { instrucPauseA = _var; }

	float GetInstrucPauseB(void) { return instrucPauseB; }
	void SetInstrucPauseB(float _var) { instrucPauseB = _var; }

	char* GetInstrucComment(void) { return instrucComment; }
	void SetInstrucComment(char* _var) { instrucComment = _restring(_var, 0, strlen(_var)); }

	bool GetInstrucUseTimedUse(void) { return instrucUseTimedUse; }
	void SetInstrucUseTimedUse(bool _var) { instrucUseTimedUse = _var; }

	float GetInstrucTimedUseA(void) { return instrucTimedUseA; }
	void SetInstrucTimedUseA(float _var) { instrucTimedUseA = _var; }

	float GetInstrucTimedUseB(void) { return instrucTimedUseB; }
	void SetInstrucTimedUseB(float _var) { instrucTimedUseB = _var; }

	bool GetInstrucNotFirstTimedUse(void) { return instrucNotFirstTimedUse; }
	void SetInstrucNotFirstTimedUse(bool _var) { instrucNotFirstTimedUse = _var; }
			
	// safety
	int GetSafetySurfMin(void) { return safetySurfMin; }
	void SetSafetySurfMin(int _var) { safetySurfMin = _var; }

	int GetSafetySurfMax(void) { return safetySurfMax; }
	void SetSafetySurfMax(int _var) { safetySurfMax = _var; }

	bool GetSafetyUseSurfCheck(void) { return safetyUseSurfCheck; }
	void SetSafetyUseSurfCheck(bool _var) { safetyUseSurfCheck = _var; }

	bool GetSafetyUseSurfCheckcon(void) { return safetyUseSurfCheckcon; }
	void SetSafetyUseSurfCheckcon(bool _var) { safetyUseSurfCheckcon = _var; }

	bool GetSafetyUseCoordCheck(void) { return safetyUseCoordCheck; }
	void SetSafetyUseCoordCheck(bool _var) { safetyUseCoordCheck = _var; }

	bool GetSafetyUseCoordCheckcon(void) { return safetyUseCoordCheckcon; }
	void SetSafetyUseCoordCheckcon(bool _var) { safetyUseCoordCheckcon = _var; }

	int GetSafetyCoordX(void) { return safetyCoordX; }
	void SetSafetyCoordX(int _var) { safetyCoordX = _var; }

	int GetSafetyCoordY(void) { return safetyCoordY; }
	void SetSafetyCoordY(int _var) { safetyCoordY = _var; }

	bool GetSafetyUseMPWait(void) { return safetyUseMPWait; }
	void SetSafetyUseMPWait(bool _var) { safetyUseMPWait = _var; }

	bool GetSafetyUseHPWait(void) { return safetyUseHPWait; }
	void SetSafetyUseHPWait(bool _var) { safetyUseHPWait = _var; }
			
	// warnings
	bool GetWarningsTakeScreenshot(void) { return warningsTakeScreenshot; }
	void SetWarningsTakeScreenshot(bool _var) { warningsTakeScreenshot = _var; }

	bool GetWarningsPlayAudio(void) { return warningsPlayAudio; }
	void SetWarningsPlayAudio(bool _var) { warningsPlayAudio = _var; }

	char* GetWarningsAudioFile(void) { return warningsAudioFile; }
	void SetWarningsAudioFile(char* _var) { warningsAudioFile = _restring(_var, 0, strlen(_var)); }

	bool GetWarningsJump(void) { return warningsJump; }
	void SetWarningsJump(bool _var) { warningsJump = _var; }

	int GetWarningsJumpA(void) { return warningsJumpA; }
	void SetWarningsJumpA(int _var) { warningsJumpA = _var; }

	int GetWarningsJumpB(void) { return warningsJumpB; }
	void SetWarningsJumpB(int _var) { warningsJumpB = _var; }

	bool GetWarningsJAL(void) { return warningsJAL; }
	void SetWarningsJAL(bool _var) { warningsJAL = _var; }

private:

	float timedUseLength;
	float timedUseStart;
	Instruction* next;
	int instrucPosition;

	// instruction
	bool instrucIsSpecial;
	bool instrucClearReturnStack;
	bool instrucUseKeystroke;
	int instrucKeystrokeCode;
	bool instrucUseMouseClick;
	int instrucMouseClickX;
	int instrucMouseClickY;
	bool instrucSendPacket;
	char* instrucPacketFile;
	float instrucPauseA;
	float instrucPauseB;
	char* instrucComment;
	bool instrucUseTimedUse;
	float instrucTimedUseA;
	float instrucTimedUseB;
	bool instrucNotFirstTimedUse;
			
	// safety
	int safetySurfMin;
	int safetySurfMax;
	bool safetyUseSurfCheck;
	bool safetyUseSurfCheckcon;
	bool safetyUseCoordCheck;
	bool safetyUseCoordCheckcon;
	int safetyCoordX;
	int safetyCoordY;
	bool safetyUseMPWait;
	bool safetyUseHPWait;
			
	// warnings
	bool warningsTakeScreenshot;
	bool warningsPlayAudio;
	char* warningsAudioFile;
	bool warningsJump;
	int warningsJumpA;
	int warningsJumpB;
	bool warningsJAL;
};

#endif
