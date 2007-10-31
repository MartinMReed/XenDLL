#ifndef DIB_SCREENSHOT_H
#define DIB_SCREENSHOT_H

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>

#define DibWidth(pDib)    (((BITMAPINFOHEADER *)pDib)->biWidth)
#define DibHeight(pDib)   (((BITMAPINFOHEADER *)pDib)->biHeight)
#define DibBitCount(pDib) (((BITMAPINFOHEADER *)pDib)->biBitCount)
#define DibClrUsed(pDib)  (((BITMAPINFOHEADER *)pDib)->biClrUsed)
#define DibNumColors(pDib) (DibClrUsed(pDib) ? DibClrUsed (pDib) :         \
               (DibBitCount(pDib) <= 8 ? 1 << DibBitCount(pDib) : 0))

#define DibClrTablePtr(pDib) ((RGBQUAD *) (pDib + sizeof (BITMAPINFOHEADER)))

#define DibBitsPtr(pDib) (pDib + sizeof (BITMAPINFOHEADER) +               \
                                 sizeof (RGBQUAD) * DibNumColors(pDib))

#define DibRowLength(pDib) (((DibWidth(pDib) *                             \
                                        DibBitCount(pDib) + 31) & ~31) >> 3)

#define DibRowPtr(pDib,iRow) (DibBitsPtr(pDib) +                           \
                         DibRowLength(pDib) * (DibHeight(pDib) - (iRow) - 1))

#define DibPixelPtr(pDib,iRow,iCol) (DibRowPtr(pDib,iRow) +                \
                                   (iCol) * DibBitCount(pDib) / 8)

#define DibBitsSize(pDib)  (DibHeight(pDib) * DibRowLength(pDib))

#define DibTotalSize(pDib) (sizeof (BITMAPINFOHEADER) +                    \
                            sizeof (RGBQUAD) * DibNumColors(pDib) +        \
                            DibBitsSize(pDib))

#define DibGetColor(pDib,i) (* (DibClrTablePtr(pDib) + (i)))
#define DibSetColor(pDib,i,rgb) (* (DibClrTablePtr(pDib) + (i)) = (rgb))

     // Next are the seven functions in ScreenSeizeDib.c

BYTE * DibCreate (DWORD dwWidth, DWORD dwHeight, WORD wBitCount, 
                                                DWORD dwNumColors) ;

DWORD DibGetPixel (PBYTE pDib, int iRow, int iCol) ;
void  DibSetPixel (PBYTE pDib, int iRow, int iCol, DWORD dwPixel) ;

int     DibSave (BYTE * pDib, char * pFileName) ;
HGLOBAL DibCopyForClipboard (BYTE * pDib) ;

BYTE * DibSortColorTable (BYTE * pDib) ;
BYTE * DibReduceColorFormat (HWND hwnd, BYTE * pDib) ;

BYTE * DibCreate (DWORD dwWidth, DWORD dwHeight, WORD wBitCount, DWORD dwNumColors);

/*--------------------------------------------------------------------------
   DibGetPixel: Gets a pixel value for a particular row and column address.
                NOTE: No bounds check on row and column!
  --------------------------------------------------------------------------*/

DWORD DibGetPixel (PBYTE pDib, int iRow, int iCol);

/*--------------------------------------------------------------------------
   DibSetPixel: Sets a pixel value for a particular row and column address.
                NOTE: No bounds check on row and column!
  --------------------------------------------------------------------------*/

void DibSetPixel (PBYTE pDib, int iRow, int iCol, DWORD dwPixel);

/*----------------------------------------------------------------------
   DibSortColorTable: Sorts a DIB color table to put most-used colors
                      at the beginning. Possibly reduces size of DIB
                      for unused colors, so returns pointer to new DIB.
  ----------------------------------------------------------------------*/

     // The function requires two qsort routines...

int Compare1 (const void * p1, const void * p2) ;
int Compare2 (const void * p1, const void * p2) ;

     // ... and a structure used in the comparison routines.
 
typedef struct
{
     int     indexOld ;       // The original palette index
     int     indexNew ;       // The new palette index
     DWORD   dwCount ;        // The number of pixels of that color
     RGBQUAD rgb ;            // The RGB value for that index
}
COLORCOUNT ;

     // Here's the actual function

BYTE * DibSortColorTable (BYTE * pDib);

     // Sorts the array of COLORCOUNT structures to put the highest
     //   counts at the beginning.

int Compare1 (const void * p1, const void * p2);

     // Sorts the array of COLORCOUNT structures to put the lowest
     //   old index at the beginning.

int Compare2 (const void * p1, const void * p2);

/*------------------------------------------------------------------------
   DibReduceColorFormat: Creates a new DIB with a different color format
                         if the existing DIB can be reduced to that size.
                         NOTE: Assumes DibSortColorTable has already 
                         been called for the DIB.
  ------------------------------------------------------------------------*/

BYTE * DibReduceColorFormat (HWND hwnd, BYTE * pDib);

/*-------------------------------------
   DibSave: Saves a DIB to disk (yawn)
  -------------------------------------*/

int DibSave (BYTE * pDib, char * pFileName);

/*--------------------------------------------------------
   DibCopyForClipboard: Returns a global handle ready for 
                        SetClipboardData.with CF_DIB.
  --------------------------------------------------------*/

HGLOBAL DibCopyForClipboard (BYTE * pDib);

#endif