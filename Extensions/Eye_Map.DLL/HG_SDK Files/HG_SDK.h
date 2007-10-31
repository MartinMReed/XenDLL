#ifndef HG_SDK_H
#define HG_SDK_H

#pragma once

#include "..\..\HGooey.DLL\HGooey Files\HGIncDef_Public.h"

#include <stdio.h>

DWORD *Location_X, *Location_Y, *Screen, *Chatmode;

HGNodeList* Eye_Map;

BYTE HIDESHOW_KEY;

float DisplayInitialScale;
float DisplaySize;
float DisplayX;
float DisplayY;

HGOImage* Background;

char* MapFile[3];
IDDTexture* MapTexture[3];
HGOImage* Map[3];
float MapF2GX[3];
float MapF2GY[3];

char* MapButtonText[3];
HGOButton* MapButton[3];

HGOImage* Location;

RECT Zoom;
HGOButton* ZoomIN;
HGOButton* ZoomOUT;
HGOButton* ZoomRESET;

bool bZoomIN, bZoomOUT;

bool Loaded;

bool ConfigOn;

#endif
