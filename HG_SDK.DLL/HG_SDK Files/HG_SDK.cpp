#include "HG_SDK.h"




//-------------------------------------------------------
// 
//-------------------------------------------------------
BOOL APIENTRY DllMain(HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					)
{
    switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			{
				initialized = false;
				configOpen = false;

				// do a version check to make sure users dont have older versions of HGooey
				if (!_HGVersionCheck(HGVersion))
					return Uninitialize();

				// load and create stuff here (ie load extensions, create textures/images/objects/etc)

				initialized = true;
			}
			break;
		case DLL_PROCESS_DETACH:
			{
				Uninitialize();
			}
			break;
		
    }

    return TRUE;
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
bool Uninitialize()
{
	initialized = false;

	// unload and destroy stuff here (ie unload extensions)

	return initialized;
}




//-------------------------------------------------------
// the device was lost (ie ALT+TAB to minimize the game)
//-------------------------------------------------------
void LostDevice()
{
	if (initialized == false) return;

	// any textures created using CreateIDDTextureFromFile(float,float,char*,UINT)
	// are automatically released by HGooey. this does not, however, cover
	// textures created using CreateIDDTexture(float,float,IDirect3DTexture8*)
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void ResetDevice()
{
	if (initialized == false) return;

	// any textures created using CreateIDDTextureFromFile(float,float,char*,UINT)
	// are automatically restored by HGooey. this does not, however, cover
	// textures created using CreateIDDTexture(float,float,IDirect3DTexture8*)
}




//-------------------------------------------------------
// called before any extensions draw on the screen
//-------------------------------------------------------
void PreRender()
{
	if (initialized == false) return;

	// DO NOT DRAW IN HERE, ALL DRAWING IS DISABLED.
	// ONLY DRAW IN Render()
}




//-------------------------------------------------------
// do your drawing / call your main code in here
//-------------------------------------------------------
void Render()
{
	if (initialized == false) return;

	// draw text or execute/call main code

	if (configOpen)
	{
		/**
		if (Config window's 'close button' was pushed)
		{
			configOpen = false;
		}
		else
		{
			// draw your config window
		}
		/**/
	}
}




//-------------------------------------------------------
// handle mouse input
//
// return true = send command to game
// return false = do not send the command to the game
//-------------------------------------------------------
bool HandleMouse()
{	
	if (initialized == false) return true;

	MouseInfo xMouse = _Mouse();

	if (xMouse.cState == 0) // mouse move
	{
	}
	else if (xMouse.cState == 1) // left button down
	{
		/**
		// (optional) cancel click if other buttons are being pressed
		if (xMouse.MButtonDown || xMouse.RButtonDown)
			return false;
		/**/
	}
	else if (xMouse.cState == 2) // left button up
	{
	}
	else if (xMouse.cState == 3) // middle button down
	{
		/**
		// (optional) cancel click if other buttons are being pressed
		if (xMouse.LButtonDown || xMouse.RButtonDown)
			return false;
		/**/
	}
	else if (xMouse.cState == 4) // middle button up
	{
	}
	else if (xMouse.cState == 5) // right button down
	{
		/**
		// (optional) cancel click if other buttons are being pressed
		if (xMouse.LButtonDown || xMouse.MButtonDown)
			return false;
		/**/
	}
	else if (xMouse.cState == 6) // right button up
	{
	}
	else // mouse scroll
	{
	}

	return true;
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
// http://calc.xentales.com/xendll/DIK.html
//-------------------------------------------------------
bool HandleKeyboard()
{
	if (initialized == false) return true;

	KeyInfo xKey = _Key();

	if (xKey.bDown) // key is pressed down
	{
	}
	else if (!xKey.bRepeat) // key has been released
	{
	}

	return true;
}




//-------------------------------------------------------
// turn on the config window
//
// there is no reason to edit this, build your
// config window when the .dll is first loaded.
// however, there is nothing critical here, so
// you do what you want here.
//-------------------------------------------------------
void Config()
{
	configOpen = true;
}




//-------------------------------------------------------
// version check called by XenDLL
//-------------------------------------------------------
UINT HGSDKVersionCheck()
{
	// DO NOT EDIT THIS
	return 71223;
}
