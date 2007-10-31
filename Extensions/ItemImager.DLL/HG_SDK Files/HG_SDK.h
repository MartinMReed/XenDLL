#ifndef HG_SDK_H
#define HG_SDK_H

#pragma once

#include "..\..\HGooey.DLL\HGooey Files\HGIncDef_Public.h"

#include "Capture Files\blit.h"
#include "Capture Files\BMP Functions.h"
//#include "Capture Files\DIB Functions.h"
#include "Capture Files\JPG Functions.h"

#include <iostream>

bool configOpen;

HGNodeList* ext_ItemImager;

int thumbnail;

int port;

int itemID;
int minID;
int maxID;

DWORD* g_page;

WORD* g_dropSlotMouseOver;
WORD* g_dropSlotImage;

char* g_mouseOverName;
char* g_mouseOverDescription;
WORD* g_descriptionLineCount;

RECT image;

BYTE startStopKey;

char* filePath;
int screenshotQuality;

UINT screenshotWidth;
UINT screenshotHeight;

IDirect3DTexture8* screenshotCopyTexture;
IDirect3DSurface8* screenshotRenderSurface;
IDirect3DSurface8* screenshotFinalSurface;

bool initialized;

void Initialize(void);

void CaptureScreen(void);

void LostDevice(void);
void ResetDevice(void);
void PreRender(void);
void Render(void);
bool HandleKeyboard(void);

void ResetImages(void);
void ResetImages(int _port);

void ClearScreenshot(void);
bool SetupScreenshot(void);

void DrawCursor(void);

bool Uninitialize(void);

#endif
