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

HBITMAP CreateBitmapFromWindow(HWND hWindow)
{
	RECT rWindow;
	HBITMAP hBitmap;
	HDC hdcWindow, hdcMemory;

	rWindow.left = 126;
	rWindow.top = 208;
	rWindow.right = rWindow.left + 255;
	rWindow.bottom = rWindow.top + 40;

	hdcWindow = GetDC(hWindow);
	hdcMemory = CreateCompatibleDC(hdcWindow);

	hBitmap = CreateCompatibleBitmap(hdcWindow, rWindow.right - rWindow.left, rWindow.bottom - rWindow.top);

	if (hBitmap == NULL)
	{
		ReleaseDC(NULL, hdcWindow);
		DeleteDC(hdcMemory);

		return NULL;
	}

	SelectObject(hdcMemory, hBitmap);

	StretchBlt(hdcMemory,
			   0,
			   0,
			   rWindow.right - rWindow.left,
			   rWindow.bottom - rWindow.top,
			   hdcWindow,
			   rWindow.left,
			   rWindow.top,
			   rWindow.right - rWindow.left,
			   rWindow.bottom - rWindow.top,
			   SRCCOPY);

	DeleteDC(hdcMemory);
	ReleaseDC(NULL, hdcWindow);

	return hBitmap;
}

HBITMAP CreateBitmapFromWindow2 (HWND hWindow)
{
     HBITMAP hBitmap ;
     HDC     hdcScr, hdcMem ;
	 RECT *prect = new RECT;

	 prect->left = 126;
	prect->top = 208;
	prect->right = prect->left + 255;
	prect->bottom = prect->top + 40;

          // Check if the bitmap has a zero dimension

     if (prect->top == prect->bottom || prect->left == prect->right)
          return NULL ;

          // Don't look at me like that! This is a documented use of the
          //   GetDC call to return a device context for the display!

     hdcScr = GetDC (hWindow) ;

          // Create a memory DC and a compatible bitmap

     hdcMem = CreateCompatibleDC (hdcScr) ;

     hBitmap = CreateCompatibleBitmap (hdcScr, abs (prect->right - prect->left), 
                                               abs (prect->bottom - prect->top)) ;

          // Return NULL if there's not enough memory to create the bitmap

     if (hBitmap == NULL)
     {
          ReleaseDC (NULL, hdcScr) ;
          DeleteDC (hdcMem) ;
          return NULL ;
     }

          // Select the bitmap into the memory DC and copy the screen to it
          // Note: Bitmap will be flipped if prect->left > prect->right or
          //   prect->top > prect->bottom. Use abs() or BitBlt() to avoid
          //   flipping.

     SelectObject (hdcMem, hBitmap) ;

     StretchBlt (hdcMem, 0, 0, abs (prect->right - prect->left),
                               abs (prect->bottom - prect->top), 
                 hdcScr, prect->left, prect->top, 
                               prect->right - prect->left,
                               prect->bottom - prect->top, SRCCOPY) ;

          // If the user wants to include the cursor in the image, 
          //   we must now draw the current cursor on the memory device 
          //   context. This isn't as easy as it sounds.

          // Clean up and return the handle to the bitmap

     DeleteDC (hdcMem) ;
     ReleaseDC (NULL, hdcScr) ;

     return hBitmap ;
}

/*-------------------------------------------------------------------------- 
   CopyBitmapFromScreen: Creates GDI bitmap object and copies a rectangular 
                         are of the screen into it. Returns bitmap handle.
  --------------------------------------------------------------------------*/

HBITMAP CreateBitmapFromScreen (RECT * prect, BOOL bIncludeCursor)
{
     HBITMAP hBitmap ;
     HDC     hdcScr, hdcMem ;

          // Check if the bitmap has a zero dimension

     if (prect->top == prect->bottom || prect->left == prect->right)
          return NULL ;

          // Don't look at me like that! This is a documented use of the
          //   GetDC call to return a device context for the display!

     hdcScr = GetDC (NULL) ;

          // Create a memory DC and a compatible bitmap

     hdcMem = CreateCompatibleDC (hdcScr) ;

     hBitmap = CreateCompatibleBitmap (hdcScr, abs (prect->right - prect->left), 
                                               abs (prect->bottom - prect->top)) ;

          // Return NULL if there's not enough memory to create the bitmap

     if (hBitmap == NULL)
     {
          ReleaseDC (NULL, hdcScr) ;
          DeleteDC (hdcMem) ;
          return NULL ;
     }

          // Select the bitmap into the memory DC and copy the screen to it
          // Note: Bitmap will be flipped if prect->left > prect->right or
          //   prect->top > prect->bottom. Use abs() or BitBlt() to avoid
          //   flipping.

     SelectObject (hdcMem, hBitmap) ;

     StretchBlt (hdcMem, 0, 0, abs (prect->right - prect->left),
                               abs (prect->bottom - prect->top), 
                 hdcScr, prect->left, prect->top, 
                               prect->right - prect->left,
                               prect->bottom - prect->top, SRCCOPY) ;

          // If the user wants to include the cursor in the image, 
          //   we must now draw the current cursor on the memory device 
          //   context. This isn't as easy as it sounds.

     if (bIncludeCursor)
     {
          DWORD   tidCursor, tidCurrent ;
          HCURSOR hCursor ;
          HWND    hwndCursor ;
          POINT   ptCursor ;

               // The following two calls for getting the cursor handle and 
               //   the cursor position would have worked in 16-bit Windows

          hCursor = GetCursor () ;      // doesn't work!
          GetCursorPos (&ptCursor) ;

               // However, in 32-bit Windows the GetCursor call returns the
               //   handle of the last cursor to appear above the thread 
               //   that calls the GetCursor function. 
               // Usually, that cursor is the window sizing cursor,

               // One possible solution is the GetCursorInfo function. 
               //   However, that function is not documented as supported
               //   under Windows 95 or versions of Windows NT 4.0 prior
               //   to SP6.
               // In reality, GetCursorInfo does not seem to work under
               //   Windows 4.0 SP6 either!

               // A better solution is to determine the thread ID under the 
               //   cursor and the current thread ID (the thread ID of this
               //   process). See Knowledge Base article Q230495.

          hwndCursor = WindowFromPoint (ptCursor) ;
          tidCursor  = GetWindowThreadProcessId (hwndCursor, NULL) ;
          tidCurrent = GetCurrentThreadId () ;

               // Then, if these two thread IDs are not equal, attach
               //   the current thread to the thread of the window
               //   underneath the cursor. This allows the GetCursor
               //   function to return the current cursor handle.

          if (tidCursor != tidCurrent)
          {
               if (AttachThreadInput (tidCurrent, tidCursor, TRUE))
               {
                    hCursor = GetCursor () ;
                    AttachThreadInput (tidCurrent, tidCursor, FALSE) ;
               }
          }

               // Now draw the cursor on the memory device context

          DrawIconEx (hdcMem, ptCursor.x - prect->left, 
                              ptCursor.y - prect->top, 
                      hCursor, 0, 0, 0, NULL, DI_DEFAULTSIZE | DI_NORMAL) ;
     }

          // Clean up and return the handle to the bitmap

     DeleteDC (hdcMem) ;
     ReleaseDC (NULL, hdcScr) ;

     return hBitmap ;
}

/*-------------------------------------------------------
   GetCurrentPalette: Obtains the current system palette.
  -------------------------------------------------------*/

HPALETTE GetCurrentPalette (HWND hwnd)
{
     HDC          hdc ;
     HPALETTE     hPalette ;
     int          iSize ;
     LOGPALETTE * plogpal ;

          // Get a video device context

     hdc = GetDC (hwnd) ;

          // If the video device supports palettes, get palette size
          //   (Betcha a hundred to one it'll be 256!)

     if (RC_PALETTE & GetDeviceCaps (hdc, RASTERCAPS))
     {
          iSize = GetDeviceCaps (hdc, SIZEPALETTE) ;

               // Allocate memory for the palette and get it.

          plogpal = (LOGPALETTE*)malloc (sizeof (LOGPALETTE) + 
                            sizeof (PALETTEENTRY) * (iSize - 1)) ;

          plogpal->palVersion    = 0x0300 ;
          plogpal->palNumEntries = iSize ;

          GetSystemPaletteEntries (hdc, 0, iSize, plogpal->palPalEntry) ;

               // Now create a palette based on the system palette entries

          hPalette = CreatePalette (plogpal) ;

               // free the memory
     
          free (plogpal) ;
     }
          // If the device doesn't support palettes, hPalette is NULL.

     else
          hPalette = NULL ;

          // Release the DC

     ReleaseDC (hwnd, hdc) ;

     return hPalette ;
}

/*------------------------------------------------------------------------- 
   ConvertDDBtoDIB: Converts a GDI bitmap object to a DIB using a palette.
                    Function based on Knowledge Base article Q80080.
  -------------------------------------------------------------------------*/

//credit to Charles Petzold

BYTE * ConvertDDBtoDIB (HWND hwnd, HBITMAP hBitmap, HPALETTE hPalette)
{
     BITMAP       bm ;
     BITMAPINFO * pbmi ;
     BYTE       * pBits ;
     HDC          hdc ;
     WORD         wBitCount, wNumColor, wColorEntries, wRowLength ;

          // Get information about the DDB & calculate bits per pixel

     GetObject (hBitmap, sizeof (BITMAP), (PSTR) &bm) ;

     wBitCount = bm.bmPlanes * bm.bmBitsPixel ;

          // Figure out the DIB bit count and number of colors.
          //   (Probably more generalized than it needs to be)

     if (wBitCount > 16)
     {
          wNumColor = 0 ;
          wBitCount = 24 ;
     }
     else if (wBitCount > 8)
     {
          wNumColor = 0 ;
          wBitCount = 16 ;
     }
     else if (wBitCount > 4)
     {
          wNumColor = 1 << wBitCount ;
          wBitCount = 8 ;
     }
     else if (wBitCount > 2)
     {
          wNumColor = 1 << wBitCount ;
          wBitCount = 4 ;
     }
     else if (wBitCount == 1)
     {
          wNumColor = 2 ;
     }

          // Determine the number of entries in the DIB color table

     wColorEntries = wNumColor ;

     if (wColorEntries == 0 && wBitCount <= 8)
          wColorEntries = 1 << wBitCount ;

          // DIB rows are always multiple of 4 bytes long.

     wRowLength = (WORD)(((bm.bmWidth * wBitCount + 31) & ~31) >> 3);

          // Allocate memory for the entire DIB.

     pbmi = (BITMAPINFO*)malloc (sizeof (BITMAPINFOHEADER) + 
                    sizeof (RGBQUAD) * wColorEntries +
                    wRowLength * bm.bmHeight) ;

     if (pbmi == NULL)
          return NULL ;

          // Initialize the BITMAPINFOHEADER

     pbmi->bmiHeader.biSize          = sizeof (BITMAPINFOHEADER) ;
     pbmi->bmiHeader.biWidth         = bm.bmWidth ;
     pbmi->bmiHeader.biHeight        = bm.bmHeight ;
     pbmi->bmiHeader.biPlanes        = 1 ;
     pbmi->bmiHeader.biBitCount      = wBitCount ;
     pbmi->bmiHeader.biCompression   = BI_RGB ;
     pbmi->bmiHeader.biSizeImage     = 0 ;
     pbmi->bmiHeader.biXPelsPerMeter = 0 ;
     pbmi->bmiHeader.biYPelsPerMeter = 0 ;
     pbmi->bmiHeader.biClrUsed       = wNumColor ;
     pbmi->bmiHeader.biClrImportant  = 0 ;

          // Determine the address of the pixel bits in the DIB

     pBits = (BYTE *) pbmi + sizeof (BITMAPINFOHEADER) + 
                             sizeof (RGBQUAD) * wColorEntries ;

          // Get a video device context and select the palette into it

     hdc = GetDC (hwnd) ;
     SelectPalette (hdc, hPalette, FALSE) ;
     RealizePalette (hdc) ;

          // Here's the function we've all been waiting for.

     GetDIBits (hdc, hBitmap, 0, bm.bmHeight, pBits, pbmi, DIB_RGB_COLORS) ;
          
          // Clean up and get out.

     ReleaseDC (hwnd, hdc) ;

     return (BYTE *) pbmi ;
}

#endif