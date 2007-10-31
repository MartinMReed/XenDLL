#ifndef BMP_SCREENSHOT_H
#define BMP_SCREENSHOT_H

/*----------------------------------------------------------------
   ScreenSeizeBmp.c -- Bitmap Routines for ScreenSeize 1.0
                       Copyright (c) 2000, Ziff-Davis Media, Inc.
                       All rights reserved.

   First published in PC Magazine, US edition, May 23, 2000.
   Programmed by Charles Petzold.
  ----------------------------------------------------------------*/

#include <windows.h>
#include <math.h>

HBITMAP CreateBitmapFromWindow(HWND hWindow);

HBITMAP CreateBitmapFromWindow2 (HWND hWindow);

/*-------------------------------------------------------------------------- 
   CopyBitmapFromScreen: Creates GDI bitmap object and copies a rectangular 
                         are of the screen into it. Returns bitmap handle.
  --------------------------------------------------------------------------*/

HBITMAP CreateBitmapFromScreen (RECT * prect, BOOL bIncludeCursor);

/*-------------------------------------------------------
   GetCurrentPalette: Obtains the current system palette.
  -------------------------------------------------------*/

HPALETTE GetCurrentPalette (HWND hwnd);

/*------------------------------------------------------------------------- 
   ConvertDDBtoDIB: Converts a GDI bitmap object to a DIB using a palette.
                    Function based on Knowledge Base article Q80080.
  -------------------------------------------------------------------------*/

//credit to Charles Petzold

BYTE * ConvertDDBtoDIB (HWND hwnd, HBITMAP hBitmap, HPALETTE hPalette);

#endif