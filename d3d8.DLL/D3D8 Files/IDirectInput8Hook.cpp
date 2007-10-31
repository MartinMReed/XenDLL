#include "../CustomDevice Files/CustomDevice.h"

extern HINSTANCE hInst;

extern CustomDevice* g_CustomDevice;

typedef HRESULT (WINAPI* DIRECTINPUT8CREATE)(HINSTANCE, DWORD, REFIID, LPVOID, LPUNKNOWN);

typedef HRESULT (STDMETHODCALLTYPE* GETDEVICESTATE)(IDirectInputDevice8*, DWORD, LPVOID);

GETDEVICESTATE pfGetDeviceState = NULL;

#define GETDEVICESTATE_OFFSET 9

BYTE codeFragment[5] = {0, 0, 0, 0, 0};
BYTE jmp[5] = {0, 0, 0, 0, 0};
DWORD savedProtection = 0;

bool g_bDIHookInstalled = false;

BOOL bSendKey = FALSE;
DWORD keyToSend = NULL;

char keyboardBuffer[256];

BOOL bSendMouse = FALSE;

DIMOUSESTATE2 mouseBuffer;

bool m_MiddleButtonDown;



//-------------------------------------------------------
// 
//-------------------------------------------------------
IDirectInput8Hook::IDirectInput8Hook(void)
{
	m_bDIHookInstalled =	FALSE;
	m_pDIDevice = NULL;
	KeySent = false;

}



//-------------------------------------------------------
// 
//-------------------------------------------------------
IDirectInput8Hook::~IDirectInput8Hook(void)
{
	if (m_bDIHookInstalled)
		ReleaseDInputHook();

	if (m_pDIDevice)
		m_pDIDevice->Release();
}



//-------------------------------------------------------
// 
//-------------------------------------------------------
EXTERN_C HRESULT _declspec(dllexport) STDMETHODCALLTYPE GetDeviceState_Hook(IDirectInputDevice8* di, DWORD bufSize, PBYTE buffer)
{ 
	// put back saved code fragment
	BYTE* codeDest = (BYTE*)pfGetDeviceState;
	codeDest[0] = codeFragment[0];
	*((DWORD*)(codeDest + 1)) = *((DWORD*)(codeFragment + 1));

	HRESULT hr = 0;

	try
	{
		// call the real GetDeviceState method
		hr = pfGetDeviceState(di, bufSize, buffer);

		if (FAILED(hr))
		{
			di->Unacquire();
			hr = di->Acquire();
		}
		else
		{
			if (bufSize == 256) // keybd input
			{
				if (bSendKey)  
				{
					*(buffer+keyToSend) = keyboardBuffer[keyToSend];
					bSendKey = FALSE;
				}

				if (g_CustomDevice)
				{
					g_CustomDevice->OnKeyEvent((char *)buffer);
				}
			}
			else if (bufSize == sizeof(DIMOUSESTATE2)) // mouse input
			{
				if (bSendMouse)  
				{
					*((DIMOUSESTATE2*)buffer) = mouseBuffer;
					bSendMouse = FALSE;
				}

				if (g_AutoClick && g_AutoClick->initialized)
				{
					static bool autoClicking = false;

					if (g_AutoClick->triggerButton != -1 &&
						MOUSEDOWN(*(DIMOUSESTATE2*)buffer, g_AutoClick->triggerButton) && 
						_CheckExtMenuIO(g_AutoClick->extAutoClick))
					{
						if (autoClicking = !autoClicking)
							((DIMOUSESTATE2*)buffer)->rgbButtons[MOUSEBUTTON_LEFT] = 0x80;
						else
							((DIMOUSESTATE2*)buffer)->rgbButtons[MOUSEBUTTON_LEFT] = 0x0;

						((DIMOUSESTATE2*)buffer)->rgbButtons[g_AutoClick->triggerButton] = 0x0;
					}
				}

				if (g_CustomDevice)
				{
					g_CustomDevice->OnMouseEvent((DIMOUSESTATE2 *)buffer);
				}
			}
		}
	}
	catch(...) { }

	// put JMP instruction again
	codeDest[0] = jmp[0];
	*((DWORD*)(codeDest + 1)) = *((DWORD*)(jmp + 1));

	return hr;
}



//-------------------------------------------------------
// 
//-------------------------------------------------------
void IDirectInput8Hook::InstallDInputHook(void)
{
	if (g_bDIHookInstalled)
		return;

	HMODULE	hmod = LoadLibrary("dinput8.dll");
	DIRECTINPUT8CREATE pfDirectInput8Create = (DIRECTINPUT8CREATE)GetProcAddress(hmod, "DirectInput8Create");

	IDirectInput8* lpDI;
	pfDirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&lpDI, NULL);

	IDirectInputDevice8* lpDIDevice;
	lpDI->CreateDevice(GUID_SysKeyboard, &lpDIDevice, NULL);

	DWORD* vtablePtr = (DWORD*)(*((DWORD*)lpDIDevice));
	pfGetDeviceState = (GETDEVICESTATE)vtablePtr[GETDEVICESTATE_OFFSET];

	// unconditional JMP to relative address is 5 bytes.
	jmp[0] = 0xe9;
	DWORD addr = (DWORD)GetDeviceState_Hook - (DWORD)pfGetDeviceState - 5;
	memcpy(jmp + 1, &addr, sizeof(DWORD));

	memcpy(codeFragment, pfGetDeviceState, 5);
	VirtualProtect(pfGetDeviceState, 8, PAGE_EXECUTE_READWRITE, &savedProtection);
	memcpy(pfGetDeviceState, jmp, 5);

	lpDI->Release();

	g_bDIHookInstalled = TRUE;
}



//-------------------------------------------------------
// 
//-------------------------------------------------------
void IDirectInput8Hook::ReleaseDInputHook(void)
{
	if (m_bDIHookInstalled == FALSE)
		return;

	// put back saved code fragment
	BYTE* codeDest = (BYTE*)pfGetDeviceState;
	codeDest[0] = codeFragment[0];
	*((DWORD*)(codeDest + 1)) = *((DWORD*)(codeFragment + 1));

	VirtualProtect(pfGetDeviceState, 8, PAGE_EXECUTE_READWRITE, &savedProtection);

	m_bDIHookInstalled = FALSE;
}



//-------------------------------------------------------
// 
//-------------------------------------------------------
void IDirectInput8Hook::GetDIKeyBuffer(byte* buf)
{
	// put back saved code fragment
	BYTE* codeDest = (BYTE*)pfGetDeviceState;
	codeDest[0] = codeFragment[0];
	*((DWORD*)(codeDest + 1)) = *((DWORD*)(codeFragment + 1));

	//call the real GetDeviceState method
	HRESULT hr = pfGetDeviceState(m_pDIDevice, 256, buf);

	// put JMP instruction again
	codeDest[0] = jmp[0];
	*((DWORD*)(codeDest + 1)) = *((DWORD*)(jmp + 1));
}



//-------------------------------------------------------
// 
//-------------------------------------------------------
void IDirectInput8Hook::SetDIKeyState(DWORD key, byte val)
{
	keyboardBuffer[key] = val;
	bSendKey = TRUE;
	keyToSend = key;
}



//-------------------------------------------------------
// 
//-------------------------------------------------------
void IDirectInput8Hook::SetMouseClick(LONG x, LONG y, UINT button)
{
	ZeroMemory(&mouseBuffer, sizeof(mouseBuffer));

	mouseBuffer.lX = x;
	mouseBuffer.lY = y;
	mouseBuffer.rgbButtons[button] = 0x80;

	bSendMouse = TRUE;
}
