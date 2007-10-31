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
				Initialize();

				// do a version check to make sure users dont have older versions of HGooey
				if (!_HGVersionCheck(HGVersion))
					return Uninitialize();

				// load and create stuff here (ie load extensions, create textures/images/objects/etc)

				// make sure the menu item texture exists
				if (GetFileAttributes("XenDLL Files/Extensions/ItemImager/ItemImager.bmp") == 0xFFFFFFFF)
					return Uninitialize();

				// create the texture for the menu item
				IDDTexture* ItemImagerTexture = _CreateIDDTextureFromFile(64.0f, 64.0f, "XenDLL Files/Extensions/ItemImager/ItemImager.bmp", COLOR_RED);

				// create the extension
				ext_ItemImager = _CreateExtension(	// Function
					ItemImagerTexture,				// Texture
					NULL);							// Rect

				const char* Data = NULL;

				//// load a memory address
				//if (Data = _ReadConfigItem("page", "XenDLL Files/Memory.xml"))
				//{
				//	try
				//	{
				//		g_page = (DWORD*)atoi(Data);
				//	}
				//	catch (...) { return Uninitialize(); }
				//}
				//else
				//{
				//	return Uninitialize();
				//}

				// load a memory address
				if (Data = _ReadConfigItem("dropSlotMouseOver", "XenDLL Files/Memory.xml"))
				{
					try
					{
						g_dropSlotMouseOver = (WORD*)atoi(Data);
					}
					catch (...) { return Uninitialize(); }
				}
				else
				{
					return Uninitialize();
				}

				// load a memory address
				if (Data = _ReadConfigItem("dropSlotImage", "XenDLL Files/Memory.xml"))
				{
					try
					{
						g_dropSlotImage = (WORD*)atoi(Data);
					}
					catch (...) { return Uninitialize(); }
				}
				else
				{
					return Uninitialize();
				}

				// load a memory address
				if (Data = _ReadConfigItem("mouseOverName", "XenDLL Files/Memory.xml"))
				{
					try
					{
						g_mouseOverName = (char*)atoi(Data);
					}
					catch (...) { return Uninitialize(); }
				}
				else
				{
					return Uninitialize();
				}

				// load a memory address
				if (Data = _ReadConfigItem("mouseOverDescription", "XenDLL Files/Memory.xml"))
				{
					try
					{
						g_mouseOverDescription = (char*)atoi(Data);
					}
					catch (...) { return Uninitialize(); }
				}
				else
				{
					return Uninitialize();
				}

				// load a memory address
				if (Data = _ReadConfigItem("descriptionLineCount", "XenDLL Files/Memory.xml"))
				{
					try
					{
						g_descriptionLineCount = (WORD*)atoi(Data);
					}
					catch (...) { return Uninitialize(); }
				}
				else
				{
					return Uninitialize();
				}

				// load start/play key
				Data = _ReadConfigItem("startStopKey", "XenDLL Files/Extensions/ItemImager/ItemImager.xml");
				if (Data == NULL || (startStopKey = _convertDIK_Name(Data)) == 0x00)
				{
					startStopKey = 0x00;
					_WriteConfigItem("startStopKey", "0x00", "XenDLL Files/Extensions/ItemImager/ItemImager.xml");
				}
				
				// read the config file
				Data = _ReadConfigItem("minID", "XenDLL Files/Extensions/ItemImager/ItemImager.xml");
				if (Data)
				{
					minID = atoi(Data);
				}
				else
				{
					minID = -1;
					_WriteConfigItem("minID", "-1", "XenDLL Files/Extensions/ItemImager/ItemImager.xml");
				}
				
				// read the config file
				Data = _ReadConfigItem("maxID", "XenDLL Files/Extensions/ItemImager/ItemImager.xml");
				if (Data)
				{
					maxID = atoi(Data);
				}
				else
				{
					maxID = -1;
					_WriteConfigItem("maxID", "-1", "XenDLL Files/Extensions/ItemImager/ItemImager.xml");
				}

				// read the config file
				if (Data = _ReadConfigItem("filePath", "XenDLL Files/Extensions/ItemImager/ItemImager.xml"))
				{
					if (Data[strlen(Data)-1] == '\\' || Data[strlen(Data)-1] == '/')
						filePath = _restring(Data, 0, strlen(Data)-1);
					else
						filePath = _restring(Data, 0, strlen(Data));
				}
				else
				{
					_WriteConfigItem("filePath", "ItemImager", "XenDLL Files/Extensions/ItemImager/ItemImager.xml");
				}

				// if the folder does not exist, create it
				if (GetFileAttributes(filePath) == 0xFFFFFFFF)
					CreateDirectory(filePath, NULL);
				
				// read the config file
				if (Data = _ReadConfigItem("screenshotQuality", "XenDLL Files/Extensions/ItemImager/ItemImager.xml"))
				{
					screenshotQuality = atoi(Data);
				}
				else
				{
					_WriteConfigItem("screenshotQuality", "100", "XenDLL Files/Extensions/ItemImager/ItemImager.xml");
				}
				
				// read the config file
				if (Data = _ReadConfigItem("thumbnail", "XenDLL Files/Extensions/ItemImager/ItemImager.xml"))
				{
					thumbnail = atoi(Data);
				}
				else
				{
					_WriteConfigItem("thumbnail", "1", "XenDLL Files/Extensions/ItemImager/ItemImager.xml");
				}
				
				///

				if (thumbnail == 0)
				{
					SetRect(&image, 557, 345, 917, 705); // full
				}
				else
				{
					SetRect(&image, 557, 345, 647, 435); // thumb
				}

				if (SetupScreenshot() == false)
					return Uninitialize();

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
void ResetImages()
{
	if (!initialized || !_CheckExtMenuIO(ext_ItemImager)) return;

	//// set the first row of item slots
	//*(g_dropSlotImage+((104/sizeof(WORD))*0)) = itemID;

	//for (int i = 1; i < 20; i++)
	//{
	//	*(g_dropSlotImage+((104/sizeof(WORD))*i)) = 0;
	//}

	//for (int i = 0; i < 20; i++)
	//{
	//	*(g_dropSlotMouseOver+((6/sizeof(WORD))*i)) = itemID;
	//}

	ResetImages(port);
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void ResetImages(int _port)
{
	if (!initialized || !_CheckExtMenuIO(ext_ItemImager)) return;

	// wireshark filter
	//ip.dst == 66.139.75.6 or ip.src == 66.139.75.6

	BYTE vars[4];

	vars[0] = 0x89 + (BYTE)itemID;

	vars[1] = 0x68 + (BYTE)floor(itemID / 256.0);

	vars[2] = 0x4e + (BYTE)itemID + (BYTE)floor(itemID / 256.0);

	vars[3] = 0x4b;
	if (itemID > 383)
	{
		vars[3] -= (BYTE)1;
	}
	else if (itemID > 254)
	{
		//vars[3] -= (BYTE)1;
	}
	else if (itemID > 127)
	{
		vars[3] -= (BYTE)1;
	}

	char SendBuf[] = {	0x55, vars[0], vars[1], 0xd9, 0xd3, 0xab, 0xb8, 0xfe, 
						0x57, 0x7e, 0x69, 0xb1, 0x2e, 0x6c, 0x8b, 0xd0, 
						0x8a, 0x6d, 0x82, 0x3a, 0x9e, 0x7d, 0xae, 0xe2, 
						0x5e, 0x4d, 0x4b, 0x26, 0x87, 0xc8, 0x0d, 0x84, 
						0xc1, 0xcc, 0x53, 0xf8, 0xcd, 0xac, 0xe7, 0xdf, 
						0x6a, 0x1f, 0xf3, 0xcb, 0xb2, 0x66, 0x04, 0x2b, 
						0x2a, 0xeb, 0x9e, 0x59, 0x26, 0x7a, 0x38, 0xfd, 
						0x21, 0x07, 0x58, 0x8c, 0xeb, 0x89, 0x68, 0xd8, 
						0xd3, 0xab, 0xb8, 0xfe, 0x57, 0x7e, 0x69, 0xb1, 
						0x2e, 0x6c, 0x8b, 0xd0, 0x8a, 0x6d, 0x82, 0x3a, 
						0x9e, 0x7d, 0xae, 0xe2, 0x5e, 0x4d, 0x4b, 0x26, 
						0x87, 0xc8, 0x0d, 0x84, 0xc1, 0xcc, 0x53, 0xf8, 
						0xcd, 0xac, 0xe7, 0xdf, 0x6a, 0x1f, 0xf3, 0xcb, 
						0xb2, 0x66, 0x04, 0x2b, 0x2a, 0xeb, 0x9e, 0x59, 
						0x26, 0x7a, 0x38, 0xfd, 0x21, 0x07, 0x58, 0x8c, 
						0xeb, 0x89, 0x68, 0xd8, 0xd3, 0xab, 0xb8, 0xfe, 
						0x57, 0x7e, 0x69, 0xb1, 0x2e, 0x6c, 0x8b, 0xd0, 
						0x8a, 0x6d, 0x82, 0x3a, 0x9e, 0x7d, 0xae, 0xe2, 
						0x5e, 0x4d, 0x4b, 0x26, 0x87, 0xc8, 0x0d, 0x84, 
						0xc1, 0xcc, 0x53, 0xf8, 0xcd, 0xac, 0xe7, 0xdf, 
						0x6a, 0x1f, 0xf3, 0xcb, 0xb2, 0x66, 0x04, 0x2b, 
						0x2a, 0xeb, 0x9e, 0x59, 0x26, 0x7a, 0x38, 0xfd, 
						0x21, 0x07, 0x58, 0x8c, 0xeb, 0x89, 0x68, 0xd8, 
						0xd3, 0xab, 0xb8, 0xfe, 0x57, 0x7e, 0x69, 0xb1, 
						0x2e, 0x6c, 0x8b, 0xd0, 0x8a, 0x6d, 0x82, 0x3a, 
						0x9e, 0x7d, 0xae, 0xe2, 0x5e, vars[2], vars[3], 0x26, 
						0x87 };

	WSADATA wsaData;
	SOCKET SendSocket;
	sockaddr_in RecvAddr;
	int Port = _port;
	char* Addr = "192.168.1.10";

	//---------------------------------------------
	// Initialize Winsock
	WSAStartup(MAKEWORD(2,2), &wsaData);

	//---------------------------------------------
	// Create a socket for sending data
	SendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//---------------------------------------------
	// Set up the RecvAddr structure with the IP address of
	// the receiver (in this example case "123.456.789.1")
	// and the specified port number.
	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(Port);
	RecvAddr.sin_addr.s_addr = inet_addr(Addr);

	//---------------------------------------------
	// Send a datagram to the receiver
	int sent = sendto(SendSocket, 
		SendBuf, 
		sizeof(SendBuf), 
		0, 
		(SOCKADDR *) &RecvAddr, 
		sizeof(RecvAddr));

	//---------------------------------------------
	// When the application is finished sending, close the socket.
	closesocket(SendSocket);

	//---------------------------------------------
	// Clean up and quit.
	WSACleanup();
}




//-------------------------------------------------------
// called before any extensions draw on the screen
//-------------------------------------------------------
void PreRender()
{
	if (!initialized || !_CheckExtMenuIO(ext_ItemImager)) return;

	// DO NOT DRAW IN HERE, ALL DRAWING IS DISABLED.
	// ONLY DRAW IN Render()

	char buffer[MAX_PATH];

	if (itemID > -1 && itemID <= maxID)
	{
		static bool reset = false;

		if (reset)
		{
			static bool skip = false;

			if (itemID == *g_dropSlotImage && itemID == *g_dropSlotMouseOver && skip)
			{
				reset = false;
				skip = false;

				CaptureScreen();

				FILE* out = NULL;
				sprintf_s(buffer, sizeof(buffer), "%s/items.xml", filePath);
				fopen_s(&out, buffer, "a");
				
				fprintf_s(out, "	<item>\n");
				fprintf_s(out, "		<id>%i</id>\n", itemID);
				fprintf_s(out, "		<name>%s</name>\n", g_mouseOverName);

				char* m_MouseOverDescription = g_mouseOverDescription;
				while ((m_MouseOverDescription - g_mouseOverDescription) / 88 < (int)(*g_descriptionLineCount))
				{
					fprintf_s(out, "		<description>%s</description>\n", m_MouseOverDescription);
					m_MouseOverDescription += 88;
				}
				
				fprintf_s(out, "	</item>\n");

				fclose(out);

				itemID++;

				if (itemID > maxID)
				{
					itemID = -1;

					FILE* out = NULL;
					sprintf_s(buffer, sizeof(buffer), "%s/items.xml", filePath);
					fopen_s(&out, buffer, "a");
					fprintf_s(out, "</xenitems>");
					fclose(out);
				}
			}
			else
			{
				skip = true;
			}
		}
		else
		{
			ResetImages();
			reset = true;
		}
	}
}




//-------------------------------------------------------
// do your drawing / call your main code in here
//-------------------------------------------------------
void Render()
{
	if (!initialized || !_CheckExtMenuIO(ext_ItemImager)) return;

	// draw text or execute/call main code

	//*g_page = 11;

	char buffer[MAX_PATH];

	sprintf_s(buffer, sizeof(buffer), "itemID = %i / *g_Image = %i / *g_MouseOver = %i ", itemID, *g_dropSlotImage, *g_dropSlotMouseOver);
	_DrawText(buffer, 0.0f, 0.0f, 1.0f, COLOR_GREEN);

	sprintf_s(buffer, sizeof(buffer), "port = %i", port);
	_DrawText(buffer, 0.0f, 30.0f, 1.0f, COLOR_GREEN);

	float textScale = 1.0f;
	_DrawText("x",
		_X2W(930.0f) - ((textScale *_FontWidth()) / 2.0f),
		_Y2H(379.0f) - ((textScale *_FontHeight()) / 2.0f),
		textScale,
		COLOR_ORANGE);

	DrawCursor();
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void DrawCursor()
{
	char cursor_textx[256];
	char cursor_texty[256];
	
	sprintf_s(cursor_textx, sizeof(cursor_textx), "x:  %i", (int)_Mouse().CursorX);
	sprintf_s(cursor_texty, sizeof(cursor_texty), "y:  %i", (int)_Mouse().CursorY);
	
	float MouseX = _X2W(_Mouse().CursorX);
	float MouseY = _Y2H(_Mouse().CursorY);

	float x = 0.0f;
	float xy = 0.0f;
	float yy = 0.0f;

	float _tscale = 0.5f;
	float FontWidth = _tscale * _FontWidth();
	float FontHeight = _tscale * _FontHeight();
	
	if (MouseY + 85.0f >= _ScreenHeight())
	{
		xy = _ScreenHeight() - (FontHeight * 2.0f);
		yy = _ScreenHeight() - (FontHeight * 1.0f);
	}
	else
	{
		xy = MouseY + 85.0f - (FontHeight * 2.0f);
		yy = MouseY + 85.0f - (FontHeight * 1.0f);
	}
	
	int longer = strlen(cursor_texty);
	if (strlen(cursor_textx) > strlen(cursor_texty))
	{
		longer = strlen(cursor_textx);
	}

	if (MouseX + (FontWidth * longer) >= _ScreenWidth())
	{
		x = _ScreenWidth() - (FontWidth * longer);
	}
	else
	{
		x = MouseX;
	}
	
	_DrawText(cursor_textx, x, xy, _tscale, COLOR_WHITE);
	_DrawText(cursor_texty, x, yy, _tscale, COLOR_WHITE);
}




//-------------------------------------------------------
// adapted from xenhook 5.0 by Achilles and v0id
//-------------------------------------------------------
bool SetupScreenshot()
{
	// find the game window
	HWND hwnd = FindWindow(NULL, NULL);

	// get from HGooey
	IDirect3DDevice8* device = _D3DDevice();

	screenshotWidth = (UINT)_ScreenWidth();
	screenshotHeight = (UINT)_ScreenHeight();
	
	// make sure height and width are even for texture copying
	if (screenshotWidth % 2 == 1)
		screenshotWidth++;
	
	if (screenshotHeight % 2 == 1)
		screenshotHeight++;

	IDirect3DSurface8* pBB = NULL;
	device->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &pBB);
	D3DSURFACE_DESC desc;
	pBB->GetDesc(&desc);
	pBB->Release();
				
	if (FAILED(device->CreateTexture(desc.Width, desc.Height, 1, D3DUSAGE_RENDERTARGET, desc.Format, D3DPOOL_DEFAULT,&screenshotCopyTexture)))
	{		
		if (FAILED(D3DXCreateTexture(device, desc.Width, desc.Height, 1, D3DUSAGE_RENDERTARGET, desc.Format, D3DPOOL_DEFAULT,&screenshotCopyTexture)))
		{
			if (FAILED(device->CreateTexture(desc.Width, desc.Height, 1, D3DUSAGE_RENDERTARGET | D3DUSAGE_SOFTWAREPROCESSING, desc.Format, D3DPOOL_DEFAULT,&screenshotCopyTexture)))
			{
				return false;
			}
		}
	}
	
	screenshotCopyTexture->GetLevelDesc(0, &desc);
	
	if (FAILED(device->CreateRenderTarget(screenshotWidth, screenshotHeight, desc.Format, D3DMULTISAMPLE_NONE, false, &screenshotRenderSurface)))
	{
		if (FAILED(device->CreateRenderTarget(desc.Width, desc.Height, desc.Format, D3DMULTISAMPLE_NONE, false, &screenshotRenderSurface)))
		{
			return false;
		}
	}
				
	screenshotRenderSurface->GetDesc(&desc);
	screenshotWidth = desc.Width;
	screenshotHeight = desc.Height;
				
	device->CreateImageSurface(screenshotWidth, screenshotHeight, desc.Format, &screenshotFinalSurface);

	return true;
}




//-------------------------------------------------------
// adapted from xenhook 5.0 by Achilles and v0id
//-------------------------------------------------------
void CaptureScreen()
{
	if (!initialized || !_CheckExtMenuIO(ext_ItemImager)) return;

	// find the game window
	HWND hwnd = FindWindow(NULL, NULL);

	// get from HGooey
	IDirect3DDevice8* device = _D3DDevice();
	
	D3DSURFACE_DESC desc;

	IDirect3DSurface8* srcSurf = NULL;
	
	device->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &srcSurf);
	srcSurf->GetDesc(&desc);
	
	IDirect3DSurface8* pCopySurface = NULL;
	screenshotCopyTexture->GetSurfaceLevel(0, &pCopySurface);
	
	RECT rWindow;
	SetRect(&rWindow, image.left, image.top, screenshotWidth, screenshotHeight);

	POINT points[] = {{0, 0}};
	HRESULT hr = device->CopyRects(srcSurf, &rWindow, 1, pCopySurface, points);
	srcSurf->Release();
	
	// get copy surface info - needed for scaling on cards with limited texture support
	// to calc text coordinates
	pCopySurface->GetDesc(&desc);
	
	//save the current render target
	IDirect3DSurface8* pSavedRenderTarget = NULL;
	device->GetRenderTarget(&pSavedRenderTarget);
	
	// save the current stencil buffer
	IDirect3DSurface8* pSavedStencilBuffer = NULL;
	device->GetDepthStencilSurface(&pSavedStencilBuffer);
	
	// set new render target for shrinkage
	device->SetRenderTarget(screenshotRenderSurface, NULL);
	
	// blit to our new render target 
	RECT rect = {0, 0, screenshotWidth, screenshotHeight};
	D3DXVECTOR4 srcTextureCoodRect;
	srcTextureCoodRect.x = 0;
	srcTextureCoodRect.y = 0;
	srcTextureCoodRect.z = (float)rWindow.right/desc.Width; //copy surface to backbuffer ratio 
	srcTextureCoodRect.w = (float)rWindow.bottom/desc.Height;
	
	BlitRect(device, screenshotCopyTexture, &rect, &srcTextureCoodRect);
	hr = device->SetRenderTarget(pSavedRenderTarget, pSavedStencilBuffer);
	
	pSavedStencilBuffer->Release();
	pSavedRenderTarget->Release();
	
	SetRect(&rWindow, 0, 0, screenshotWidth, screenshotHeight);
	device->CopyRects(screenshotRenderSurface, &rWindow, 1, screenshotFinalSurface, points);
	
	// lock the surface for reading
	D3DLOCKED_RECT LockedRect;
	screenshotFinalSurface->LockRect(&LockedRect, NULL, D3DLOCK_READONLY);
	screenshotFinalSurface->GetDesc(&desc);
	UINT iBpp;
	
	if (desc.Format == D3DFMT_A8R8G8B8 || desc.Format == D3DFMT_X8R8G8B8)
		iBpp = 4;
	else
		iBpp = 2;
	
	int iBufsize = iBpp * (image.right - image.left) * (image.bottom - image.top);
	int iRowWidth = iBpp * (image.right - image.left);
	
	char* pBitData = new char[iBufsize];
	
	for (int y = 0; y < (int)(image.bottom - image.top); y++)
		memcpy(pBitData + y*iRowWidth, (char*)LockedRect.pBits + y*LockedRect.Pitch, iRowWidth);
	
	// unlock the surface
	screenshotFinalSurface->UnlockRect();
	
	HBITMAP hBitmap = CreateBitmap((image.right - image.left), (image.bottom - image.top), 1, iBpp*8, pBitData);
	delete[] pBitData;

	if (hBitmap)
	{
		HANDLE hDib = (HANDLE)ConvertDDBtoDIB(hwnd, hBitmap, NULL);
		DeleteObject(hBitmap);

		if (hDib)
		{
			char buffer[MAX_PATH];
			sprintf_s(buffer, sizeof(buffer), "%s/item%i.jpg", filePath, itemID);

			JpegFromDib(hDib, screenshotQuality, buffer);
			DeleteObject(hDib);
		}
	}
	
	pCopySurface->Release();
}




//-------------------------------------------------------
// the device was lost (ie ALT+TAB to minimize the game)
//-------------------------------------------------------
void LostDevice()
{
	if (initialized == false) return;

	ClearScreenshot();
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void ResetDevice()
{
	if (initialized == false) return;

	SetupScreenshot();
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
	if (!initialized || !_CheckExtMenuIO(ext_ItemImager)) return true;

	KeyInfo xKey = _Key();

	if (xKey.bDown) // key is pressed down
	{
		if (xKey.cKey == DIK_DOWN)
		{
			return false;
		}
		else if (xKey.cKey == DIK_UP)
		{
			return false;
		}
		else if (xKey.cKey == DIK_LEFT)
		{
			return false;
		}
		else if (xKey.cKey == DIK_RIGHT)
		{
			return false;
		}
		else if (xKey.cKey == startStopKey)
		{
			return false;
		}
	}
	else if (xKey.bRepeat == false) // key has been released
	{
		if (xKey.cKey == DIK_DOWN)
		{
			port--;
			return false;
		}
		else if (xKey.cKey == DIK_UP)
		{
			port++;
			return false;
		}
		else if (xKey.cKey == DIK_LEFT)
		{
			port -= 100;
			return false;
		}
		else if (xKey.cKey == DIK_RIGHT)
		{
			port += 100;
			return false;
		}
		else if (xKey.cKey == startStopKey)
		{
			char buffer[MAX_PATH];

			if (itemID == -1)
			{
				itemID = minID;

				FILE* out = NULL;
				sprintf_s(buffer, sizeof(buffer), "%s/items.xml", filePath);
				fopen_s(&out, buffer, "w");
				fprintf_s(out, "<xenitems>\n");
				fclose(out);
			}
			else
			{
				itemID = -1;

				FILE* out = NULL;
				sprintf_s(buffer, sizeof(buffer), "%s/items.xml", filePath);
				fopen_s(&out, buffer, "a");
				fprintf_s(out, "</xenitems>");
				fclose(out);
			}

			return false;
		}
	}

	return true;
}


//-------------------------------------------------------
// adapted from xenhook 5.0 by Achilles and v0id
//-------------------------------------------------------
void ClearScreenshot()
{
	if (screenshotCopyTexture)
	{
		screenshotCopyTexture->Release();
		screenshotCopyTexture = NULL;
	}

	if (screenshotRenderSurface)
	{
		screenshotRenderSurface->Release();
		screenshotRenderSurface = NULL;
	}

	if (screenshotFinalSurface)
	{
		screenshotFinalSurface->Release();
		screenshotFinalSurface = NULL;
	}
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
void Initialize()
{
	initialized = false;
	configOpen = false;

	ext_ItemImager = NULL;

	thumbnail = 1;

	port = 0;

	itemID = -1;

	minID = -1;
	maxID = -1;

	g_page = NULL;

	g_dropSlotMouseOver = NULL;
	g_dropSlotImage = NULL;

	g_mouseOverName = NULL;
	g_mouseOverDescription = NULL;
	g_descriptionLineCount = NULL;

	SetRect(&image, 0, 0, 0, 0);

	startStopKey = 0x00;

	filePath = "ItemImager";
	screenshotQuality = 100;

	screenshotWidth = 0;
	screenshotHeight = 0;

	screenshotCopyTexture = NULL;
	screenshotRenderSurface = NULL;
	screenshotFinalSurface = NULL;
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
bool Uninitialize()
{
	initialized = false;

	if (ext_ItemImager)
	{
		_UnloadExtension(ext_ItemImager);
	}

	if (filePath)
	{
		delete[] filePath;
		filePath = NULL;
	}

	g_page = NULL;

	g_dropSlotMouseOver = NULL;
	g_dropSlotImage = NULL;

	g_mouseOverName = NULL;
	g_mouseOverDescription = NULL;
	g_descriptionLineCount = NULL;

	startStopKey = 0x00;

	ClearScreenshot();

	return initialized;
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
