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

BYTE * DibCreate (DWORD dwWidth, DWORD dwHeight, WORD wBitCount, 
                                                DWORD dwNumColors)
{
     BITMAPINFOHEADER bmih ;
     BYTE           * pDib ;

          // Set up a BITMAPINFOHEADER describing the DIB

     bmih.biSize          = sizeof (BITMAPINFOHEADER) ;
     bmih.biWidth         = dwWidth ;
     bmih.biHeight        = dwHeight ;
     bmih.biPlanes        = 1 ;
     bmih.biBitCount      = wBitCount ;
     bmih.biCompression   = BI_RGB ;
     bmih.biSizeImage     = 0 ;
     bmih.biXPelsPerMeter = 0 ;
     bmih.biYPelsPerMeter = 0 ;
     bmih.biClrUsed       = dwNumColors ;
     bmih.biClrImportant  = 0 ;

          // The DibTotalSize macro in ScreenSeizeDib.h calculates the 
          //   total size of the DIB based on the header. So, the return
          //   value is used to allocate memory for the DIB.

     pDib = (BYTE*)malloc (DibTotalSize ((BYTE *) & bmih)) ;

          // The information already set in the BITMAPINFOHEADER
          //   structure is copied to the newly allocated memory.
          //   Everything else is uninitialized.

     * (BITMAPINFOHEADER *) pDib = bmih ;

          // Return the pointer to the DIB

     return pDib ;
}

/*--------------------------------------------------------------------------
   DibGetPixel: Gets a pixel value for a particular row and column address.
                NOTE: No bounds check on row and column!
  --------------------------------------------------------------------------*/

DWORD DibGetPixel (PBYTE pDib, int iRow, int iCol)
{
          // Handy macro in ScreenSeizeDib.h

     BYTE * pPixel = DibPixelPtr (pDib, iRow, iCol) ;

          // Another macro and gritty bit work
     
     switch (DibBitCount (pDib))
     {
     case  1:  return 0x01 & (* pPixel >> (7 - (iCol & 7))) ;  
     case  4:  return 0x0F & (* pPixel >> (iCol & 1 ? 0 : 4)) ;
     case  8:  return * pPixel ;
     case 16:  return * (WORD *) pPixel ;
     case 24:  return 0x00FFFFFF & * (DWORD *) pPixel ;
     case 32:  return * (DWORD *) pPixel ;
     }
     return 0 ;
}

/*--------------------------------------------------------------------------
   DibSetPixel: Sets a pixel value for a particular row and column address.
                NOTE: No bounds check on row and column!
  --------------------------------------------------------------------------*/

void DibSetPixel (PBYTE pDib, int iRow, int iCol, DWORD dwPixel)
{
          // Ditto

     BYTE * pPixel = DibPixelPtr (pDib, iRow, iCol) ;

     switch (DibBitCount (pDib))
     {
     case  1:  * pPixel &= ~(1     << (7 - (iCol & 7))) ;
               * pPixel |= dwPixel << (7 - (iCol & 7)) ;
               break ;

     case  4:  * pPixel &= 0x0F    << (iCol & 1 ? 4 : 0) ;
               * pPixel |= dwPixel << (iCol & 1 ? 0 : 4) ;
               break ;

     case  8:  * pPixel = (BYTE) dwPixel ;
               break ;

     case 16:  * (WORD *) pPixel = (WORD) dwPixel ;
               break ;

     case 24:  * (RGBTRIPLE *) pPixel = * (RGBTRIPLE *) &dwPixel ; 
               break ;

     case 32:  * (DWORD *) pPixel = dwPixel ;
               break ;
     }
}

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

BYTE * DibSortColorTable (BYTE * pDib)
{
     COLORCOUNT cc[256] ;
     int        cRows, cCols, cClrs, iRow, iCol, i, cClrsNew ;

          // This function is only applicable for palette DIBs

     if (DibBitCount(pDib) > 8)
          return pDib ;

          // Get some useful information about the DIB using macros
          //   defined in ScreenSeizeDib.h

     cRows = DibHeight (pDib) ;
     cCols = DibWidth  (pDib) ;
     cClrs = DibNumColors (pDib) ;

          // Initialize the structure with DIB color table

     for (i = 0 ; i < cClrs ; i++)
     {
          cc[i].indexOld = i ;
          cc[i].indexNew = 0 ;
          cc[i].dwCount  = 0 ;
          cc[i].rgb      = DibGetColor (pDib, i) ;
     }

          // Now accumulate the pixel counts in the structure

     for (iRow = 0 ; iRow < cRows ; iRow++)
     for (iCol = 0 ; iCol < cCols ; iCol++)
          cc[DibGetPixel (pDib, iRow, iCol)].dwCount ++ ;

          // Sort the structure by the pixel counts

     qsort (&cc, cClrs, sizeof (COLORCOUNT), Compare1) ;

          // Transfer the sorted colors into the DIB, but
          //   don't transfer those that aren't used at all.

     for (i = 0 ; i < cClrs && cc[i].dwCount ; i++)
          DibSetColor (pDib, i, cc[i].rgb) ;

          // The new number of colors is now available

     cClrsNew = i ;

          // Put the new color indices into the structure

     for (i = 0 ; i < cClrs ; i++)
          cc[i].indexNew = i ;

          // Sort the structure by the old indices

     qsort (&cc, cClrs, sizeof (COLORCOUNT), Compare2) ;

          // Now we can easily change each color index

     for (iRow = 0 ; iRow < cRows ; iRow++)
     for (iCol = 0 ; iCol < cCols ; iCol++)
          DibSetPixel (pDib, iRow, iCol, 
               cc[DibGetPixel (pDib, iRow, iCol)].indexNew) ;

          // Now the file can be tightened up to get rid of unused colors

     if (cClrsNew < cClrs)
     {
          BYTE * pBitsOld, * pBitsNew ;
          int    iBitsSize ;
          
          pBitsOld = DibBitsPtr (pDib) ;
          ((BITMAPINFOHEADER *) pDib)->biClrUsed = cClrsNew ;
          pBitsNew = DibBitsPtr (pDib) ;
          iBitsSize = DibBitsSize (pDib) ;

          for (i = 0 ; i < iBitsSize ; i++)
               *pBitsNew++ = *pBitsOld++ ;

          pDib = (BYTE*)realloc (pDib, DibTotalSize (pDib)) ;
     }

     return pDib ;
}

     // Sorts the array of COLORCOUNT structures to put the highest
     //   counts at the beginning.

int Compare1 (const void * p1, const void * p2)
{
     COLORCOUNT * pcc1 = (COLORCOUNT *) p1 ;
     COLORCOUNT * pcc2 = (COLORCOUNT *) p2 ;

     if (pcc1->dwCount > pcc2->dwCount)
          return -1 ;

     return (pcc1->dwCount < pcc2->dwCount) ;
}

     // Sorts the array of COLORCOUNT structures to put the lowest
     //   old index at the beginning.

int Compare2 (const void * p1, const void * p2)
{
     COLORCOUNT * pcc1 = (COLORCOUNT *) p1 ;
     COLORCOUNT * pcc2 = (COLORCOUNT *) p2 ;

     if (pcc1->indexOld < pcc2->indexOld)
          return -1 ;

     return (pcc1->indexOld > pcc2->indexOld) ;
}

/*------------------------------------------------------------------------
   DibReduceColorFormat: Creates a new DIB with a different color format
                         if the existing DIB can be reduced to that size.
                         NOTE: Assumes DibSortColorTable has already 
                         been called for the DIB.
  ------------------------------------------------------------------------*/

BYTE * DibReduceColorFormat (HWND hwnd, BYTE * pDib)
{
     BYTE    * pDibNew ;
     DWORD     dwPixel, adwPixel[256] ;
     int       i, iNumColors, iRow, iCol, cRows, cCols ;
     RGBQUAD * pTableOld, 
             * pTableNew, rgb ;

          // Get information about the DIB

     cRows = DibHeight (pDib) ;
     cCols = DibWidth  (pDib) ;
     iNumColors = DibNumColors (pDib) ;

          // If the DIB format can be optimized, create the new DIB.
     
     switch (DibBitCount (pDib))
     {
          // One bit-per-pixel is the lowest we can go!

     case 1:
          return pDib ;

          // A 4 bpp DIB can be changed to 1-bit if the
          //   number of colors is 2 or fewer.

     case 4:
          if (iNumColors > 2)
               return pDib ;

          pDibNew = DibCreate (DibWidth(pDib), DibHeight (pDib), 1, 
                               iNumColors == 2 ? 0 : iNumColors) ;
          break ;

          // An 8 bpp DIB may be optimized to 1 bit or 4 bit.

     case 8:
          if (DibNumColors (pDib) > 16)
               return pDib ;

          if (DibNumColors (pDib) > 2)
               pDibNew = DibCreate (DibWidth(pDib), DibHeight (pDib), 4, 
                                    iNumColors == 16 ? 0 : iNumColors) ;
          else
               pDibNew = DibCreate (DibWidth(pDib), DibHeight (pDib), 1, 
                                    iNumColors == 2 ? 0 : iNumColors) ;
          break ;

          // For 16- or 24-bit DIBs, we need to count the colors

     case 16:
     case 24:
          for (iRow = 0 ; iRow < cRows ; iRow++)
          for (iCol = 0 ; iCol < cCols ; iCol++)
          {
               dwPixel = DibGetPixel (pDib, iRow, iCol) ;

               for (i = 0 ; i < iNumColors ; i++)
                    if (dwPixel == adwPixel[i])
                         break ;

               if (i == iNumColors)
               {
                         // If we exceed 256 colors, just return DIB pointer.

                    if (iNumColors == 256)
                         return pDib ;

                    adwPixel[iNumColors++] = dwPixel ;
               }
          }

               // Now we can create a new DIB based on the number of colors.

          if (iNumColors > 16)
               pDibNew = DibCreate (DibWidth(pDib), DibHeight (pDib), 8, 
                                    iNumColors == 256 ? 0 : iNumColors) ;

          else if (iNumColors > 2)
               pDibNew = DibCreate (DibWidth(pDib), DibHeight (pDib), 4, 
                                    iNumColors == 16 ? 0 : iNumColors) ;

          else 
               pDibNew = DibCreate (DibWidth(pDib), DibHeight (pDib), 1, 
                                    iNumColors == 2 ? 0 : iNumColors) ;
          break ;

     default:
          return pDib ;
     }

          // We couldn't create the new DIB, so display a message and 
          //   return the existing DIB.

     if (!pDibNew)
     {
          return pDib ;
     }

          // This section transfers the color table from the existing DIB
          //   to the new DIB.
          // For 4- and 8-bit old DIBs, this is just a matter of copying
          //   the color table.

     switch (DibBitCount (pDib))
     {
     case 4:
     case 8:
          pTableOld = DibClrTablePtr (pDib) ;
          pTableNew = DibClrTablePtr (pDibNew) ;

          for (i = 0 ; i < iNumColors ; i++)
               * pTableNew ++ = * pTableOld ++ ;

          break ;

          // For 16- and 24-bit DIBS, the pixel values in the adwPixel array
          //   (which are really RGB values) must be transferred into the
          //   new color table.

     case 16:
          pTableNew = DibClrTablePtr (pDibNew) ;

          for (i = 0 ; i < iNumColors ; i++)
          {
               rgb.rgbRed      = (BYTE) (255 * ((0x7C00 & adwPixel[i]) >> 10) / 0x1F) ;
               rgb.rgbGreen    = (BYTE) (255 * ((0x03E0 & adwPixel[i]) >>  5) / 0x1F) ;
               rgb.rgbBlue     = (BYTE) (255 * ((0x001F & adwPixel[i]) >>  0) / 0x1F) ;
               rgb.rgbReserved = 0 ;

               * pTableNew ++ = rgb ;
          }
          break ;

     case 24:
          pTableNew = DibClrTablePtr (pDibNew) ;

          for (i = 0 ; i < iNumColors ; i++)
               * pTableNew ++ = * (RGBQUAD *) (adwPixel + i) ;
          break ;
     }

          // Now it's time to transfer the actual pixels

     switch (DibBitCount (pDib))
     {
     case 4:
     case 8:
          for (iRow = 0 ; iRow < cRows ; iRow++)
          for (iCol = 0 ; iCol < cCols ; iCol++)
                DibSetPixel (pDibNew, iRow, iCol, DibGetPixel (pDib, iRow, iCol)) ;
          break ;

          // For 16- and 24-bit DIBS, the pixel values must be indices into the
          //   color table (which is also the adwPixel array).

     case 16:
     case 24:
          for (iRow = 0 ; iRow < cRows ; iRow++)
          for (iCol = 0 ; iCol < cCols ; iCol++)
          {
               dwPixel = DibGetPixel (pDib, iRow, iCol) ;

                    // Search the array of unique colors

               for (i = 0 ; i < iNumColors ; i++)
                    if (dwPixel == adwPixel[i])
                         break ;

                    // This should not happen!
               
               if (i == iNumColors)
               {
                    return pDib ;
               }
                    // Set the pixel with the index

               DibSetPixel (pDibNew, iRow, iCol, i) ;
          }

               // Now sort the color table

          pDibNew = DibSortColorTable (pDibNew) ;
          break ;
     }

          // Clean up and go home.

     free (pDib) ;
     return pDibNew ;
}

/*-------------------------------------
   DibSave: Saves a DIB to disk (yawn)
  -------------------------------------*/

int DibSave (BYTE * pDib, char * pFileName)
{
     BITMAPFILEHEADER   bmfh ;
     DWORD              dwTotalSize, dwBitsSize ;
     FILE             * file ;

          // Get the size of the DIB and the bits

     dwBitsSize  = DibBitsSize (pDib) ;
     dwTotalSize = DibTotalSize (pDib) ;

          // Now we're ready to define the BITMAPFILEHEADER

     bmfh.bfType      = * (WORD *) "BM" ;
     bmfh.bfSize      = sizeof (BITMAPFILEHEADER) + dwTotalSize ;
     bmfh.bfReserved1 = 0 ;
     bmfh.bfReserved2 = 0 ;
     bmfh.bfOffBits   = bmfh.bfSize - dwBitsSize ; 

          // Open the output file

	 fopen_s(&file, pFileName, "wb");

     if (!file)
          return 0 ;

          // Write out the BITMAPFILEHEADER

     if (!fwrite (&bmfh, sizeof (bmfh), 1, file))
     {
          fclose (file) ;
          remove (pFileName) ;
          return 0 ;
     }
          // Write out the DIB

     if (!fwrite (pDib, dwTotalSize, 1, file))
     {
          fclose (file) ;
          remove (pFileName) ;
          return 0 ;
     }

     fclose (file) ;
     return 0 ;          // indicates success
}

/*--------------------------------------------------------
   DibCopyForClipboard: Returns a global handle ready for 
                        SetClipboardData.with CF_DIB.
  --------------------------------------------------------*/

HGLOBAL DibCopyForClipboard (BYTE * pDib)
{
     BYTE    * pDibClip ;
     DWORD     dwTotalSize ;
     HGLOBAL   hGlobal ;

     if (!pDib)
          return NULL ;

          // Get the size of the DIB and the bits

     dwTotalSize = DibTotalSize (pDib) ;

          // Allocate some memory for the copy

     hGlobal = GlobalAlloc (GHND | GMEM_SHARE, dwTotalSize) ;

     if (!hGlobal)
          return NULL ;

          // Get a pointer

     pDibClip = (BYTE*)GlobalLock (hGlobal) ;

          // Copy the whole darn thing

     CopyMemory (pDibClip, pDib, dwTotalSize) ;

          // Unlock and return

     GlobalUnlock (hGlobal) ;

     return hGlobal ;
}



          
     
