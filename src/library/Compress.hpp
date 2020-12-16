/*============================================================================*/
/* Compress.hpp -- string compression library                                 */
/*============================================================================*/
#pragma once

namespace Resources
{
  class Compress
  {
  public:
    
    /*----------------------------------------------------------------------------*/
    /* convert given src string into shoco dst                                    */
    /*----------------------------------------------------------------------------*/
    static inline i8 iIntoShoco(byte* aDst, i8 iDstLen, char* aSrc, i8 iSrcLen)
    {
      i8 iLenCompressed = (i8)shoco_compress(aSrc,
                                             (size_t)iSrcLen,
                                             (char*)aDst,
                                             (size_t)iDstLen);
      
      return iLenCompressed;
    }
    
    /*----------------------------------------------------------------------------*/
    /* convert given src shoco into dst string                                    */
    /*----------------------------------------------------------------------------*/
    static inline i8 iFromShoco(char* aDst, i8 iDstLen, byte* aSrc, i8 iSrcLen)
    {
      i8 iLenDecompressed = (i8)shoco_decompress((char*)aSrc,
                                                 (size_t)iSrcLen,
                                                 aDst,
                                                 (size_t)iDstLen);
      
      return iLenDecompressed;
    }
    
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
