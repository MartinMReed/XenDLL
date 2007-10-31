#include "../../CustomDevice Files/CustomDevice.h"

#include "Capture Files\blit.h"
#include "Capture Files\BMP Functions.h"
//#include "Capture Files\DIB Functions.h"
#include "Capture Files\JPG Functions.h"

Screenshot* g_Screenshot = NULL;

//-------------------------------------------------------
// adapted from xenhook 5.0 by Achilles and v0id
//-------------------------------------------------------
bool Screenshot::Initialize(void)
{
	initialized = false;

	// if the folder does not exist, create it
	if (GetFileAttributes("XenDLL Files/Extensions/Screenshot/") == 0xFFFFFFFF)
		CreateDirectory("XenDLL Files/Extensions/Screenshot/", NULL);

	const char* data = NULL;

	// read the config file
	if (data = _ReadConfigItem("filePath", "XenDLL Files/Extensions/Screenshot/Screenshot.xml"))
	{
		if (data[strlen(data)-1] == '\\' || data[strlen(data)-1] == '/')
			filePath = _restring(data, 0, strlen(data)-1);
		else
			filePath = _restring(data, 0, strlen(data));
	}
	else
	{
		_WriteConfigItem("filePath", "screenshots", "XenDLL Files/Extensions/Screenshot/Screenshot.xml");
	}

	// if the folder does not exist, create it
	if (GetFileAttributes(filePath) == 0xFFFFFFFF)
		CreateDirectory(filePath, NULL);
	
	// read the config file
	if (data = _ReadConfigItem("screenshotQuality", "XenDLL Files/Extensions/Screenshot/Screenshot.xml"))
	{
		screenshotQuality = atoi(data);
	}
	else
	{
		char buffer[256];
		sprintf_s(buffer, sizeof(buffer), "%i",screenshotQuality );
		_WriteConfigItem("screenshotQuality", buffer, "XenDLL Files/Extensions/Screenshot/Screenshot.xml");
	}
	
	// read the config file
	data = _ReadConfigItem("includeGUI", "XenDLL Files/Extensions/Screenshot/Screenshot.xml");
	if (data == NULL || _stricmp(data, "true") == 0)
	{
		includeGUI = true;
	}
	
	if (data == NULL)
	{
		_WriteConfigItem("includeGUI", "true", "XenDLL Files/Extensions/Screenshot/Screenshot.xml");
	}
	
	///

	initialized = true;
	
	SetupScreenshot();

	return initialized;
}




//-------------------------------------------------------
// adapted from xenhook 5.0 by Achilles and v0id
//-------------------------------------------------------
void Screenshot::SetupScreenshot(void)
{
	if (initialized == false) return;

	// find the game window
	HWND hwnd = FindWindow(NULL, NULL);

	// get from HGooey
	IDirect3DDevice8* device = _D3DDevice();

	// unscale the screen size
	screenshotWidth = (UINT)floor(_W2X((float)_ScreenWidth()));
	screenshotHeight = (UINT)floor(_H2Y((float)_ScreenHeight()));
	
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
	HRESULT hr = device->CreateTexture(screenshotWidth, screenshotHeight, 1, D3DUSAGE_RENDERTARGET, desc.Format, D3DPOOL_DEFAULT,&screenshotCopyTexture);
				
	if (FAILED(hr))
	{
		hr = D3DXCreateTexture(device, screenshotWidth, screenshotHeight, 1, D3DUSAGE_RENDERTARGET, desc.Format, D3DPOOL_DEFAULT,&screenshotCopyTexture);
					
		if (FAILED(hr))
			hr = device->CreateTexture(screenshotWidth, screenshotHeight, 1, D3DUSAGE_RENDERTARGET | D3DUSAGE_SOFTWAREPROCESSING, desc.Format, D3DPOOL_DEFAULT,&screenshotCopyTexture);
	}
				
	screenshotCopyTexture->GetLevelDesc(0, &desc);
				
	if (FAILED(device->CreateRenderTarget(screenshotWidth, screenshotHeight, desc.Format, D3DMULTISAMPLE_NONE, false, &screenshotRenderSurface)))
		device->CreateRenderTarget(screenshotWidth, screenshotHeight, desc.Format, D3DMULTISAMPLE_NONE, false, &screenshotRenderSurface);
				
	screenshotRenderSurface->GetDesc(&desc);
				
	device->CreateImageSurface(screenshotWidth, screenshotHeight, desc.Format, &screenshotFinalSurface);
	
	SetRect(&image, 0, 0, screenshotWidth, screenshotHeight);
}




//-------------------------------------------------------
// adapted from xenhook 5.0 by Achilles and v0id
//-------------------------------------------------------
void Screenshot::CaptureScreen(void)
{
	if (initialized == false) return;

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
	srcTextureCoodRect.z = (float)rWindow.right/screenshotWidth; //copy surface to backbuffer ratio 
	srcTextureCoodRect.w = (float)rWindow.bottom/screenshotHeight;
	
	BlitRect(device, screenshotCopyTexture, &rect, &srcTextureCoodRect);
	hr = device->SetRenderTarget(pSavedRenderTarget, pSavedStencilBuffer);
	
	pSavedStencilBuffer->Release();
	pSavedRenderTarget->Release();
	
	SetRect(&rWindow, 0, 0, screenshotWidth, screenshotHeight);
	device->CopyRects(screenshotRenderSurface, &rWindow, 1, screenshotFinalSurface, points);
	
	// lock the surface for reading
	D3DLOCKED_RECT lockedRect;
	screenshotFinalSurface->LockRect(&lockedRect, NULL, D3DLOCK_READONLY);
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
		memcpy(pBitData + y*iRowWidth, (char*)lockedRect.pBits + y*lockedRect.Pitch, iRowWidth);
	
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

			time_t rawTime;
			tm timeInfo;

			time(&rawTime);
			localtime_s(&timeInfo, &rawTime);

			sprintf_s(buffer, 256, "%s/screen_%i-%i-%i_%i.%i.%i.jpg",
				filePath,
				timeInfo.tm_year + 1900,
				timeInfo.tm_mon,
				timeInfo.tm_mday,
				timeInfo.tm_hour,
				timeInfo.tm_min,
				timeInfo.tm_sec);

			JpegFromDib(hDib, screenshotQuality, buffer);
			DeleteObject(hDib);
		}
	}
	
	pCopySurface->Release();
}




//-------------------------------------------------------
// the device was lost (ie ALT+TAB to minimize the game)
//-------------------------------------------------------
void Screenshot::LostDevice(void)
{
	if (initialized == false) return;

	ClearScreenshot();
}




//-------------------------------------------------------
// 
//-------------------------------------------------------
void Screenshot::ResetDevice(void)
{
	if (initialized == false) return;

	SetupScreenshot();
}




//-------------------------------------------------------
// called before any extensions draw on the screen
//-------------------------------------------------------
void Screenshot::PreRender(void)
{
	if (initialized == false) return;

	// DO NOT DRAW IN HERE, ALL DRAWING IS DISABLED.
	// ONLY DRAW IN Render()

	if (captureScreen)
	{
		CaptureScreen();
		captureScreen = false;
		capTakenAt = timeGetTime();
	}
}




//-------------------------------------------------------
// do your drawing / call your main code in here
//-------------------------------------------------------
void Screenshot::Render(void)
{
	if (initialized == false) return;

	// draw text or execute/call main code

	if (capTakenAt + 3000 > timeGetTime())
	{
		_DrawText("Screen Shot Saved", 0.0f, 0.0f, 1, COLOR_PURPLE, false, NULL);
	}
	else
	{
		capTakenAt = 0;
	}
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
bool Screenshot::HandleKeyboard(void)
{
	if (initialized == false) return true;

	KeyInfo xKey = _Key();

	if (xKey.bDown) // key is pressed down
	{
		if (xKey.cKey == DIK_SYSRQ)
		{
			return false;
		}
	}
	else if (xKey.bRepeat == false) // key has been released
	{
		if (xKey.cKey == DIK_SYSRQ)
		{
			captureScreen = true;
			return false;
		}
	}

	return true;
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
Screenshot::Screenshot(void)
{
	initialized = false;

	includeGUI = false;

	captureScreen = false;

	capTakenAt = 0;

	SetRect(&image, 0, 0, 0, 0);

	filePath = "screenshots";
	screenshotQuality = 100;

	screenshotWidth = 0;
	screenshotHeight = 0;

	screenshotCopyTexture = NULL;
	screenshotRenderSurface = NULL;
	screenshotFinalSurface = NULL;
}


//-------------------------------------------------------
// adapted from xenhook 5.0 by Achilles and v0id
//-------------------------------------------------------
void Screenshot::ClearScreenshot(void)
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
bool Screenshot::Uninitialize(void)
{
	initialized = false;

	if (filePath)
	{
		delete[] filePath;
		filePath = NULL;
	}

	ClearScreenshot();

	return initialized;
}


//-------------------------------------------------------
// 
//-------------------------------------------------------
Screenshot::~Screenshot(void)
{
	Uninitialize();
}
