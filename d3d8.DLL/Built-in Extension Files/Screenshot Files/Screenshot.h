#ifndef Screenshot_H
#define Screenshot_H

#pragma once

class Screenshot;
extern Screenshot* g_Screenshot;

class Screenshot
{
public:

	bool initialized;
	bool Initialize(void);

	Screenshot(void);
	~Screenshot(void);

	// extension calls
	bool includeGUI;

	// computron calls
	void CaptureScreen(void);

	//extension calls
	void LostDevice(void);
	void ResetDevice(void);
	void PreRender(void);
	void Render(void);
	bool HandleKeyboard(void);

private:

	bool captureScreen;

	unsigned long capTakenAt;

	RECT image;

	char* filePath;
	int screenshotQuality;

	UINT screenshotWidth;
	UINT screenshotHeight;

	IDirect3DTexture8* screenshotCopyTexture;
	IDirect3DSurface8* screenshotRenderSurface;
	IDirect3DSurface8* screenshotFinalSurface;

	void ClearScreenshot(void);
	void SetupScreenshot(void);

	bool Uninitialize(void);
};

#endif
