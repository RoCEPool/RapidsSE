/*============================================================================*/
/* Compress.hpp -- string compression library                                 */
/*============================================================================*/
#pragma once

namespace System
{
  class Compress
  {
  public:
    
    /*----------------------------------------------------------------------------*/
    /* convert given src string into shoco dst                                    */
    /*----------------------------------------------------------------------------*/
    static inline i8 iIntoShoco(byte* aDst, i8 iDstLen, char* aSrc, i8 iSrcLen)
    {
      i8 iLenCompressed = shoco_compress(aSrc,iSrcLen,(char*)aDst,iDstLen);
      
      return iLenCompressed;
    }
    
    /*----------------------------------------------------------------------------*/
    /* convert given src shoco into dst string                                    */
    /*----------------------------------------------------------------------------*/
    static inline i8 iFromShoco(char* aDst, i8 iDstLen, byte* aSrc, i8 iSrcLen)
    {
      i8 iLenDecompressed = shoco_decompress((char*)aSrc,iSrcLen,aDst,iDstLen);
      
      return iLenDecompressed;
    }
    
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
