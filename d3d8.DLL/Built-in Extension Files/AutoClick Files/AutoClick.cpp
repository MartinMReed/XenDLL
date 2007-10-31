#include "../../CustomDevice Files/CustomDevice.h"

AutoClick* g_AutoClick = NULL;

//-------------------------------------------------------
// 
//-------------------------------------------------------
bool AutoClick::Initialize(void) 
{
	initialized = false;

	// make sure the menu item texture exists
	if (GetFileAttributes("XenDLL Files/Extensions/AutoClick/AutoClick.bmp") == 0xFFFFFFFF)
		return Uninitialize();

	// create the texture for the menu item and create the extension
	IDDTexture* bmpAutoClick = _CreateIDDTextureFromFile(64.0f, 64.0f, "XenDLL Files/Extensions/AutoClick/AutoClick.bmp", COLOR_RED);

	extAutoClick = _CreateExtension(	// Function
		bmpAutoClick,						// Texture
		NULL);							// Rect

	///

	const char* data = NULL;
	
	data = _ReadConfigItem("MenuItemIO", "XenDLL Files/Extensions/AutoClick/AutoClick.xml");
	if (data && _stricmp(data, "true") == 0)
	{
		_SetExtMenuIO(extAutoClick, true);
	}
	else
	{
		_SetExtMenuIO(extAutoClick, false);
	}

	///

	bool notFound = false;

	data = _ReadConfigItem("trigger", "XenDLL Files/Extensions/AutoClick/AutoClick.xml");
	if (data && (triggerKey = _convertDIK_Name(data)) == 0x00)
	{
		if (_stricmp(data, "RButton") == 0)
			triggerButton = MOUSEBUTTON_RIGHT;
		else if (_stricmp(data, "LButton") == 0)
			triggerButton = MOUSEBUTTON_LEFT;
		else if (_stricmp(data, "MButton") == 0)
			triggerButton = MOUSEBUTTON_MIDDLE;
		else
			notFound = true;
	}
	else if (data)
	{
		triggerButton = -1;
	}
	else
	{
		notFound = true;
	}

	if (notFound)
	{
		triggerKey = 0x00;
		triggerButton = MOUSEBUTTON_MIDDLE;
		_WriteConfigItem("trigger", "MButton", "XenDLL Files/Extensions/AutoClick/AutoClick.xml");
	}

	initialized = true;

	return initialized;
}




//-------------------------------------------------------
// handle keyboard input
//
// cKey = DIK_[Key]
// bDown = pressed down if true, released if false
//
// return true = send command to game
// return false = do not send the command to the game
//
// DirectInput Key-Identifier Table:
// http://calc.xentales.com/HGooey/DIK.html or
// http://www.xentales.com/calc/HGooey/DIK.html
//-------------------------------------------------------
bool AutoClick::HandleKeyboard(void)
{
	if (initialized == false) return true;

	KeyInfo xKey = _Key();

	static bool autoClicking = false;

	if (xKey.bDown && xKey.cKey == triggerKey && _CheckExtMenuIO(extAutoClick))
	{
		if (autoClicking = !autoClicking)
			g_CustomDevice->directInputHook->SetMouseClick(0, 0, MOUSEBUTTON_LEFT);

		return false;
	}

	return true;
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
AutoClick::AutoClick(void)
{
	initialized = false;

	extAutoClick = NULL;
	triggerButton = 0;
	triggerKey = 0x00;
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
bool AutoClick::Uninitialize(void)
{
	initialized = false;

	if (extAutoClick)
	{
		if (_CheckExtMenuIO(extAutoClick))
			_WriteConfigItem("MenuItemIO", "true", "XenDLL Files/Extensions/AutoClick/AutoClick.xml");
		else
			_WriteConfigItem("MenuItemIO", "false", "XenDLL Files/Extensions/AutoClick/AutoClick.xml");

		_UnloadExtension(extAutoClick);
	}

	return initialized;
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
AutoClick::~AutoClick(void)
{
	Uninitialize();
}