/*============================================================================*/
/* Compress.hpp -- string compression library                                 */
/*============================================================================*/
#pragma once

namespace System
{
  class Compress
  {
  public:
    static i8 iIntoShoco(byte* aDst, i8 iDstLen, char* aSrc, i8 iSrcLen);
    static i8 iFromShoco(char* aDst, i8 iDstLen, byte* aSrc, i8 iSrcLen);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
