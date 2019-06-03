/*============================================================================*/
/* MemLib.cpp -- Memory Library                                               */
/*============================================================================*/
#pragma once

namespace Base
{
  static const u8 C_SCRIBBLE_PATTERN = 0x21454D54494E4921; // !INITME!
  static const u8 C_MEMORY_MARKER0   = 0x0706050403020100;
  static const u8 C_MEMORY_MARKER1   = 0x0706050403020101;
  static const u8 C_MEMORY_MARKER2   = 0x0706050403020102;
  static const u8 C_MEMORY_MARKER3   = 0x0706050403020103;
  static const u8 C_MEMORY_MARKER4   = 0x0706050403020104;
  static const u8 C_MEMORY_MARKER5   = 0x0706050403020105;
  static const u8 C_MEMORY_MARKER6   = 0x0706050403020106;
  static const u8 C_MEMORY_MARKER7   = 0x0706050403020107;
  static const u8 C_MEMORY_MARKER8   = 0x0706050403020108;
  static const u8 C_MEMORY_MARKER9   = 0x0706050403020109;
  static const u8 C_MEMORY_MARKERA   = 0x070605040302010a;
  static const u8 C_MEMORY_MARKERB   = 0x070605040302010b;
  static const u8 C_MEMORY_MARKERC   = 0x070605040302010c;
  static const u8 C_MEMORY_MARKERD   = 0x070605040302010d;
  static const u8 C_MEMORY_MARKERE   = 0x070605040302010e;
  static const u8 C_MEMORY_MARKERF   = 0x070605040302010f;

  static const i8 C_BYTES_PER_LINE   = 16;
  
  class Word;
  
  class MemLib
  {
  private:
    static const u1 C_ASCII_MIN_VIZ = ' ';
    static const u1 C_ASCII_MAX_VIZ = '~';
  
  public:
    static void* aAddBytes(void* aAddress, i8 nBytes);
    static void  vAsciiDump(Stz* aText, i8 nBytes);
    static void  vCheckInitialised(void* aAddress, eCN eClass);
    static void  vCheckInitialised(void* aAddress, i8 nBytes);
    static void  vCopy(void* pDst, const void* pSrc, i8 nBytes, eCN eClassNo);
    static bool  bCompareBytes(const void* aVal1, const void* aVal2, i8 nBytes);
    static i8    iCompareBytes(const void* aVal1, const void* aVal2, i8 nBytes);
    static bool  bCompareWords(const void* aVal1, const void* aVal2, i8 nBytes);
    static void  vCut(void* aAddr, i8 nSize, i8 i_Offset, i8 nCutBytes, eCN eClassNo);
    static void  vDumpWords(void *aAddress, i8 nBytes);
    static void  vExpand(void* aAddr, i8 nSize, i8 i_Offset, i8 nAddBytes, eCN eClassNo);
    static w8*   awFindNonZeroEnd(void* aWords, i8 nWords);
    static void  vFreeBlock(void* aBlock, i8 nBytes);
    static i8    iGetTotalMemory(void);
    static void  vHexDump(void *aAddress, i8 nBytes);
    static void* aMallocBlock(i8 nBytes);
    static void  vMove(void* pDst, const void* pSrc, i8 nBytes, eCN eClassNo);
    static bool  bOnBoundary(void* aAddress, i8 nByteBoundary);
    static i8    nQuantaRequired(i8 nBytes, i8 nSizeQuanta);
    static void  vReset(void);
    static void  vRotate(void* aArg, i8 nBytes, i8 nBy);
    static i8    nRoundUp(i8 nBytes, i8 nSizeQuanta);
    static void  vScribble(void* aAddress, i8 nBytes, eCN eClassNo);
    static void  vSet(void* pDst, char bByte, i8 iTimes, eCN eClassNo);
    static void  vStatistics(void);
    static void* aToBoundary(void* aAddress, i8 nByteBoundary);
    static void  vWordDump(void *aAddress, i8 iSize, i8 iHighlightWord=-1);
    static void  vWordFlip(w8* aWord);
    static void  vZero(void* pDst, i8 iBytes, eCN eClassNo);
  
    static void  vMemoryMap(void);
  
  private:
    template <class T>
    static void vPrintColor(Stz* pFmt, const T& Value, Stz* pColour);
  
  public:
    
    /*----------------------------------------------------------------------------*/
    /* word compare of val1 with val2                                             */
    /*----------------------------------------------------------------------------*/
    static inline i8 iCompareWords(const void* pVal1, const void* pVal2, i8 nBytes)
    {
      //  if (gpAttachMap) {
      //    gpAttachMap->m_oStatistics.Memory.m_iCompareTotal += nBytes;
      //  }
      
      // nBytes should be a multiple of 8
      //RASSERT(nBytes % 8 == 0);
      
      const i8 *iWord1 = (const i8*)pVal1;
      const i8 *iWord2 = (const i8*)pVal2;
      
      while (nBytes > 0) {
        if (*iWord1++ != *iWord2++)
          return iWord1[-1] < iWord2[-1] ? -1 : 1;
        nBytes -=8;
      }
      
      return 0;
    }
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
