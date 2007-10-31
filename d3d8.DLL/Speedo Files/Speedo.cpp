#include "../CustomDevice Files/CustomDevice.h"

#include "../D3D8 Files/IDirect3D8Hook.h"

typedef IDirect3D8* (WINAPI* Direct3DCreate8_t)(UINT SDKVersion);

HMODULE hOrigDll;
HINSTANCE hInst;

IDirect3D8Hook D3D8Hook;

IDirect3D8* pD3D = NULL;
IDirect3D8Hook* pD3DHook = NULL;
IDirect3DDevice8* pD3DDevice = NULL;
extern IDirectInput8Hook* g_pDIHook;
IDirect3DDevice8Hook* pD3DDeviceHook = NULL;

FARPROC OldValidatePixelShader;
FARPROC OldValidateVertexShader;
FARPROC OldDebugSetMute;
Direct3DCreate8_t OldDirect3DCreate8;

extern CustomDevice* g_CustomDevice;

// Functions
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpvreserved)	// Entry point.
{
	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{
			hInst = hInstance; // Save the DLL instance for later.

			char* next_opt = NULL;
			char* cmdline = GetCommandLine();
			char* opt = strtok_s(cmdline, " ", &next_opt);

			do
			{
				//if (_stricmp(opt, "-window") == 0)

				opt = strtok_s(NULL, " ", &next_opt);
			}
			while (opt);
		}
		break;

		case DLL_PROCESS_DETACH:
		{
			if (hOrigDll) // If the original dll is loaded...
			{
				FreeLibrary(hOrigDll); // Unload it.
				hOrigDll = NULL;
			}
		}
		break;
	}
	return true;
}

void __declspec(naked) NewValidatePixelShader(void)
{
	__asm jmp OldValidatePixelShader; // Jump to original ValidatePixelShader()
}

void __declspec(naked) NewValidateVertexShader(void)
{
	__asm jmp OldValidateVertexShader; // Jump to original ValidateVertexShader()
}

void __declspec(naked) NewDebugSetMute(void)
{
	__asm jmp OldDebugSetMute; // Jump to original DebugSetMute()
}

IDirect3D8* WINAPI NewDirect3DCreate8(UINT SDKVersion)
{
	char strOrigDll[MAX_PATH];

	GetSystemDirectory(strOrigDll, sizeof(strOrigDll));
	strcat_s(strOrigDll, MAX_PATH, "/d3d8.dll");

	// Load the original d3d8.dll
	hOrigDll = LoadLibrary(strOrigDll);

	// Get function addresses for all d3d8.dll exports
	OldValidatePixelShader	= GetProcAddress(hOrigDll, "ValidatePixelShader");
	OldValidateVertexShader = GetProcAddress(hOrigDll, "ValidateVertexShader");
	OldDebugSetMute			= GetProcAddress(hOrigDll, "DebugSetMute");
	OldDirect3DCreate8		= (Direct3DCreate8_t)GetProcAddress(hOrigDll, "Direct3DCreate8");

	pD3D = OldDirect3DCreate8(SDKVersion); // Create a real IDirect3D8 interface for us.
	pD3DHook = &D3D8Hook; // Get a pointer to our IDirect3D8Hook interface.

	return pD3DHook; // Give Client app our fake IDirect3D8 interface pointer.
}
