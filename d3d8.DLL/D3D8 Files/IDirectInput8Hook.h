#ifndef IDINPUT_H
#define IDINPUT_H

#pragma once

#define DIRECTINPUT_VERSION         DIRECTINPUT_HEADER_VERSION

#include <dinput.h>

class IDirectInput8Hook
{

public:

	IDirectInput8Hook(void);
	~IDirectInput8Hook(void);

	bool KeySent;

	void InstallDInputHook(void);
	void ReleaseDInputHook(void);
	void GetDIKeyBuffer(byte* buf);
	void SetDIKeyState(DWORD key, byte val);
	void SetMouseClick(LONG x, LONG y, UINT button);

	void (*OnKeyEvent)(PBYTE buffer);

private:
	
	BOOL m_bDIHookInstalled;
	IDirectInputDevice8* m_pDIDevice;
};

#endif
