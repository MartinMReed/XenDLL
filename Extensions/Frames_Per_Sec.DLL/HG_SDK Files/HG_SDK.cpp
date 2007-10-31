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
				Loaded = false;
				ConfigOn = false;

				// do a version check to make sure users dont have older versions of HGooey
				if (!HGVersionCheck(HGVersion))
					return FALSE;

				// load and create stuff here (ie load extensions, create textures/images/objects/etc)

				// make sure the no one moved the extension
				if (GetFileAttributes("HGooey Files/XenDLL/Extensions/Frames_Per_Sec/HG_SDK.dll") == 0xFFFFFFFF) return FALSE;

				// make sure the menu item texture exists
				if (GetFileAttributes("HGooey Files/XenDLL/Extensions/Frames_Per_Sec/Frames_Per_Sec.bmp") == 0xFFFFFFFF) return FALSE;

				/* example of creating an extension w/ no menu item to start with
				// create the extension
				Frames_Per_Sec = CreateExtension();

				// set the menu item
				SetExtensionMenuItem(
					Frames_Per_Sec,
					CreateIDDTextureFromFile(64.0f, 64.0f, "HGooey Files/XenDLL/Extensions/Frames_Per_Sec/Frames_Per_Sec.bmp", COLOR_RED));
				*/

				// create the texture for the menu item and create the extension
				IDDTexture* texture = CreateIDDTextureFromFile("HGooey Files/XenDLL/Extensions/Frames_Per_Sec/Frames_Per_Sec.bmp", COLOR_RED);
				
				if (!texture || !(Frames_Per_Sec = CreateExtension(texture))) return FALSE;

				float Scale = 1.0f;
				FPS_Display = CreateTextBar(
					"? Frames Per Second",
					Scale,
					Scale);

				if (!AttachObject(Frames_Per_Sec,FPS_Display))
				{
					UnloadExtension(Frames_Per_Sec);
					Frames_Per_Sec = NULL;

					return FALSE;
				}

				FPS_Display->SetBorderWidth(6.0f * Scale);
				FPS_Display->SetBorderHeight(5.0f * Scale);
				FPS_Display->MoveBySize((ScreenWidth() / GuiScaleX()) - FPS_Display->GetWidth(),0.0f);
				FPS_Display->SetAcceptAction(false);
				FPS_Display->SetCancelClick(false);

				const char* Data;
				bool write = false;

				if (Data = ReadConfigItem("MenuItemIO", "HGooey Files/XenDLL/Extensions/Frames_Per_Sec/Frames_Per_Sec.xml"))
				{
					if (strcmp(Data, "true") == 0)
						SetExtMenuIO(Frames_Per_Sec, true);
					else if (strcmp(Data, "false") == 0)
						SetExtMenuIO(Frames_Per_Sec, false);
					else
					{
						SetExtMenuIO(Frames_Per_Sec, false);
						write = true;
					}
				}
				else
				{
					write = true;
				}

				if (write)
				{
					WriteConfigItem("MenuItemIO", "false", "HGooey Files/XenDLL/Extensions/Eye_Map/Eye_Map.xml");
					write = false;
				}

				Loaded = true;
			}
			break;
		case DLL_PROCESS_DETACH:
			{
				if (!Loaded) return TRUE;

				Loaded = false;

				// unload and destroy stuff here (ie unload extensions)

				if (CheckExtMenuIO(Frames_Per_Sec))
					WriteConfigItem("MenuItemIO", "true", "HGooey Files/XenDLL/Extensions/Frames_Per_Sec/Frames_Per_Sec.xml");
				else
					WriteConfigItem("MenuItemIO", "false", "HGooey Files/XenDLL/Extensions/Frames_Per_Sec/Frames_Per_Sec.xml");

				UnloadExtension(Frames_Per_Sec);
				Frames_Per_Sec = NULL;
			}
			break;
    }

    return TRUE;
}




//-------------------------------------------------------
// the device was lost (ie ALT+TAB to minimize the game)
//-------------------------------------------------------
void LostDevice(void)
{
	if (!Loaded) return;

	// any textures created using CreateIDDTextureFromFile(float,float,char*,UINT)
	// are automatically released by HGooey. this does not, however, cover
	// textures created using CreateIDDTexture(float,float,IDirect3DTexture8*)
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void ResetDevice(void)
{
	if (!Loaded) return;

	// any textures created using CreateIDDTextureFromFile(float,float,char*,UINT)
	// are automatically restored by HGooey. this does not, however, cover
	// textures created using CreateIDDTexture(float,float,IDirect3DTexture8*)
}




//-------------------------------------------------------
// called before any extensions draw on the screen
//-------------------------------------------------------
void PreRender(void)
{
	if (!Loaded) return;

	// DO NOT DRAW IN HERE, ALL DRAWING IS DISABLED.
	// ONLY DRAW IN Render()
}




//-------------------------------------------------------
// do your drawing / call your main code in here
//-------------------------------------------------------
void Render(void)
{
	if (!Loaded) return;

	// draw text or execute/call main code

	bool Active = CheckExtMenuIO(Frames_Per_Sec);
	ActivateFull(Frames_Per_Sec, Active);

	static int frameCount = 0;
	static unsigned long previousTime = timeGetTime();
	unsigned long currentTime = timeGetTime();

	frameCount++;

	if (currentTime - previousTime > 500)
	{
		float FPS = frameCount / ((currentTime - previousTime) / 1000.0f);
		previousTime = currentTime;
		frameCount = 0;

		if (Active)
		{
			char buffer[64];
			sprintf_s(buffer, 64, "%.2f Frames Per Second", FPS);
			FPS_Display->SetText(buffer);
		}
	}
	
	if (ConfigOn)
	{
		/**
		if (Config window's close button was pushed)
		{
			ConfigOn = false;
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
	if (!Loaded) return true;

	MouseInfo xMouse = Mouse();

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
// http://calc.xentales.com/HGooey/DIK.html or
// http://www.xentales.com/calc/HGooey/DIK.html
//-------------------------------------------------------
bool HandleKeyboard()
{
	if (!Loaded) return true;

	KeyInfo xKey = Key();

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
void Config(void)
{
	ConfigOn = true;
}




//-------------------------------------------------------
// version check called by XenDLL
//-------------------------------------------------------
UINT HGSDKVersionCheck(void)
{
	// DO NOT EDIT THIS
	return 61225;
}
