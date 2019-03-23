/*============================================================================*/
/* Compress.cpp -- string compression library                                 */
/*============================================================================*/

#include "System.h"

/*----------------------------------------------------------------------------*/
/* convert given src string into shoco dst                                    */
/*----------------------------------------------------------------------------*/
i8 Compress::iIntoShoco(byte* aDst, i8 iDstLen, char* aSrc, i8 iSrcLen)
{
  i8 iLenCompressed = shoco_compress(aSrc,iSrcLen,(char*)aDst,iDstLen);
  
  return iLenCompressed;
}

/*----------------------------------------------------------------------------*/
/* convert given src shoco into dst string                                    */
/*----------------------------------------------------------------------------*/
i8 Compress::iFromShoco(char* aDst, i8 iDstLen, byte* aSrc, i8 iSrcLen)
{
  i8 iLenDecompressed = shoco_decompress((char*)aSrc,iSrcLen,aDst,iDstLen);

  return iLenDecompressed;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
