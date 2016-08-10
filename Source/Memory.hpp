/*============================================================================*/
/*  Memory.cpp - Memory                                                       */
/*============================================================================*/
#pragma once

#include "Common.h"

namespace Common
{
  class Memory
  {
  private:
    static const i8 C_ASCII_MIN = 32;
    static const i8 C_ASCII_MAX = 127;
    static const i8 C_BYTES_PER_LINE = 16;
    
  public:
    static void* pAddBytes(void* pAddress, i8 iBytes);
    static void  vCopy(void* pDst, const void* pSrc, i8 iBytes);
    static void  vMove(void* pDst, const void* pSrc, i8 iBytes);
    static void  vSet(void* pDst, char bByte, i8 iTimes);
    static void  vHexDump(void *pAddress, i8 iSize);
    static void  vZero(void* pDst, i8 iBytes);
    
    
  private:
    template <class T>
    static void vPrintColor(const char* pFmt, const T& OnStack, const char* pColour);
  };
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/