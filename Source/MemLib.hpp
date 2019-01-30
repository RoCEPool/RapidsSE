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

  class MemLib
  {
  private:
    static const u1 C_ASCII_MIN = 32;
    static const u1 C_ASCII_MAX = 127;
    static const i8 C_SIZEOF_WORD = static_cast<i8>(sizeof(i8));
    
  
  public:
    static void* pAddBytes(void* aAddress, i8 iBytes);
    static void  vAsciiDump(Stz* aText, i8 nBytes);
    static void  vCheckInitialised(void* aAddress, eCN eClass);
    static void  vCheckInitialised(void* aAddress, i8 nBytes);
    static void  vCopy(void* pDst, const void* pSrc, i8 nBytes, eCN eClassNo);
    static void  vFreeBlock(void* aBlock, i8 nBytes);
    static i8    iGetTotalMemory(void);
    static void  vHexDump(void *aAddress, i8 nBytes);
    static void* aMallocBlock(i8 nBytes);
    static void  vMove(void* pDst, const void* pSrc, i8 nBytes, eCN eClassNo);
    static void  vReset(void);
    static void  vScribble(void* aAddress, i8 nBytes, eCN eClassNo);
    static void  vSet(void* pDst, char bByte, i8 iTimes, eCN eClassNo);
    static void  vStatistics(void);
    static void  vWordDump(void *aAddress, i8 iSize, i8 iHighlightWord=-1);
    static void  vZero(void* pDst, i8 iBytes, eCN eClassNo);
  
    static void  vMemoryMap(void);
  private:
    template <class T>
    static void vPrintColor(Stz* pFmt, const T& Value, Stz* pColour);
  };
  
#ifdef WITH_MEMORY_ZERO
  #define _memZero_(aDst,nBytes,eClassNo)\
            MemLib::vZero(aDst,nBytes,eClassNo);
#else
  #define _memZero_(aDst,nBytes,eClassNo)
#endif
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
