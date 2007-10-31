#ifndef CustomDevice_H
#define CustomDevice_H

#pragma once

#define HGSDKVersion 71223

#include "../../HGooey.DLL/HGooey Files/HGIncDef_Private.h"

#include "../D3D8 Files/IDirectInput8Hook.h"

#include "../Built-in Extension Files/Computron Files/Computron.h"
#include "../Built-in Extension Files/Screenshot Files/Screenshot.h"
#include "../Built-in Extension Files/AutoClick Files/AutoClick.h"

#define KEYDOWN(name, key)		((name)[key] & 0x80) 
#define MOUSEDOWN(name, button)	((name).rgbButtons[button] & 0x80)

#define INPUT_OFFSET			(3)
#define INPUT_COUNT				(237 + INPUT_OFFSET)

struct xInputInfo
{
	UINT nTime;
	bool bRepeat, bReleased;
};

typedef void (CALLBACK* voidFunc)(void);
typedef UINT (CALLBACK* VersionFunc)(void);
typedef char* (CALLBACK* charFunc)(void);
typedef bool (CALLBACK* boolFunc)(void);

class CustomDevice;
class FuncList;

extern CustomDevice* g_CustomDevice;


//-------------------------------------------------------
// 
//-------------------------------------------------------
class CustomDevice
{
public:

	bool initialized;

	bool Initialize(IDirect3DDevice8* _device);

	CustomDevice(void);
	~CustomDevice(void);

	//CRITICAL_SECTION m_critical_sec;

	float* g_cursorX;
	float* g_cursorY;

	FuncList* extListFirst;
	FuncList* extListLast;

	HGNodeList* loader;
	HGOImage* loaderBackground;
	HGOTextBar* loaderTitle;
	HGOButton* loadButton;
	HGOButton* unloadButton;
	HGOButton* configButton;
	HGODefaultList* loadList;
	HGODefaultList* unloadList;
	HGODefaultList* configList;

	BYTE menuToggleKey;

	xInputInfo inputInfo[INPUT_COUNT];

	IDirectInput8Hook* directInputHook;

	IDirect3DDevice8* d3dDevice;

	void LoadSavedList(void);
	void SaveLoadedList(void);

	void LoadDLL(char* _filename);
	void UnloadDLL(char* _filename);

	void EndScene(void);

	void LostDevice(void);
	void ResetDevice(void);

	void OnKeyEvent(char* _buffer);
	bool HandleKeyboard(BYTE _cKey, char* _buffer, bool _bDown, bool _bRepeat);

	void OnMouseEvent(DIMOUSESTATE2* _mouse);
	bool HandleMouse(int _cState, DIMOUSESTATE2* _mouse);
	
	void SearchDLL(void);

	bool Uninitialize(void);
};

//-------------------------------------------------------
// 
//-------------------------------------------------------
class FuncList
{
public:

	FuncList(void)
	{
		next = NULL;
		prev = NULL;

		mod = NULL;

		name = NULL;

		configOn = false;

		f_Config = NULL;
		f_LostDevice = NULL;
		f_ResetDevice = NULL;
		f_PreRender = NULL;
		f_Render = NULL;
		f_HandleMouse = NULL;
		f_HandleKeyboard = NULL;
	}

	~FuncList(void)
	{
		if (prev)
			prev->SetNext(next);

		if (next)
			next->SetPrev(prev);

		next = NULL;
		prev = NULL;

		configOn = false;

		if (mod)
		{
			FreeLibrary(mod);
		}

		if (name)
		{
			delete[] name;
			name = NULL;
		}
	}

	FuncList* GetNext(void) { return next; }
	void SetNext(FuncList* _var) { next = _var; }

	FuncList* GetPrev(void) { return prev; }
	void SetPrev(FuncList* _var) { prev = _var; }

	HMODULE GetMod(void) { return mod; }
	void SetMod(char* _path)
	{
		mod = LoadLibrary(_path);
	}

	char* GetName(void) { return name; }
	void SetName(char* _var) { name = _var; }

	bool GetConfigOn(void) { return configOn; }
	void SetConfigOn(bool _var) { configOn = _var; }

	voidFunc f_Config;

	voidFunc f_LostDevice;
	voidFunc f_ResetDevice;

	voidFunc f_PreRender;
	voidFunc f_Render;

	boolFunc f_HandleMouse;
	boolFunc f_HandleKeyboard;

private:

	FuncList* next;
	FuncList* prev;

	HMODULE mod;

	char* name;

	bool configOn;
};

#endif
