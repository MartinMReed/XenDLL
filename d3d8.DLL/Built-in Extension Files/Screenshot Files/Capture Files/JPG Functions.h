#ifndef JPG_SCREENSHOT_H
#define JPG_SCREENSHOT_H

#include <windows.h>
#include <math.h>

#include "jpeglib/jpeglib.h"

bool JpegFromDib(HANDLE hDib, int nQuality, char *csJpeg);

BOOL DibToSamps(HANDLE                      hDib,
                int                         nSampsPerRow,
                struct jpeg_compress_struct cinfo,
                JSAMPARRAY                  jsmpPixels);

RGBQUAD QuadFromWord(WORD b16);

bool JpegFromDib(HANDLE hDib, int nQuality, char *csJpeg);

////////////////////////////////////////////////////////////////
//This function fills a jsmpArray with the RGB values
//for the CBitmap.
//
//It has been improved to handle all legal bitmap formats.
//
//A jsmpArray is a big array of RGB values, 3 bytes per value.
//
//Note that rows of pixels are processed bottom to top:
//The data in the jsamp array must be arranged top to bottom.
////////////////////////////////////////////////////////////////
BOOL DibToSamps(HANDLE                      hDib,
                int                         nSampsPerRow,
                struct jpeg_compress_struct cinfo,
                JSAMPARRAY                  jsmpPixels);

 
////////////////////////////////////////
//This function turns a 16-bit pixel
//into an RGBQUAD value.
////////////////////////////////////////
RGBQUAD QuadFromWord(WORD b16);

#endif