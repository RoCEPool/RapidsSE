/*============================================================================*/
/* Memory.cpp - Memory                                                        */
/*============================================================================*/
#pragma once

#include "Platform.h"

namespace Platform
{
  class Memory : public Static
  {
  private:
    static const u1 C_ASCII_MIN = 32;
    static const u1 C_ASCII_MAX = 127;
    static const i8 C_BYTES_PER_LINE = 16;
    static const i8 SIZEOF_WORD = static_cast<i8>(sizeof(i8));
    
  public:
    static void* pAddBytes(void* pAddress, i8 iBytes);
    static void  vCheckInitialised(void* pAddress, eCN eClass);
    static void  vCheckInitialised(void* pAddress, i8 nBytes);
    static void  vCopy(void* pDst, const void* pSrc, i8 iBytes);
    static void  vHexDump(void *pAddress, i8 iSize);
    static void  vMove(void* pDst, const void* pSrc, i8 iBytes);
    static void  vScribble(void* pAddress, i8 nBytes);
    static void  vSet(void* pDst, char bByte, i8 iTimes);
    static void  vWordDump(void *pAddress, i8 iSize, i8 iHighlightWord=-1);
    static void  vZero(void* pDst, i8 iBytes);
    
  private:
    template <class T>
    static void vPrintColor(const char* pFmt, const T& Value, const char* pColour);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
