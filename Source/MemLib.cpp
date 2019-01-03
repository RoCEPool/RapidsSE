/*============================================================================*/
/* MemLib.cpp -- Memory Library                                               */
/*============================================================================*/

#include "StorageEngine.h"
#include "Base.h"

#include "Debugging.h"

extern AttachMap* gpAttachMap;
extern ResourceTracker gxResourceTracker;

extern i8 giClassSize[];

/*----------------------------------------------------------------------------*/
/* map a new an anonymous region of memory (private or shared)                */
/*----------------------------------------------------------------------------*/
void* MemLib::pAddBytes(void* aAddress, i8 iBytes)
{
  char* pOrigin = reinterpret_cast<char*>(aAddress);
  
  char* pNew = pOrigin + iBytes;
  
  return pNew;
}

/*----------------------------------------------------------------------------*/
/* check memory has been intialised                                           */
/*----------------------------------------------------------------------------*/
void MemLib::vCheckInitialised(void* aAddress, eCN eClass)
{
  u8* pMchWord = reinterpret_cast<u8*>(aAddress);
  i8  nBytes = giClassSize[eClass];
  i8  nWords = nBytes / 8;
  
  ASSERT(nWords);

  for (i8 i_Word = 0; i_Word < nWords; i_Word++) {
    
    if (C_SCRIBBLE_PATTERN == *pMchWord) {
      MemLib::vWordDump(aAddress,nBytes,i_Word);
      vDebugTrap();
    }
    
    ASSERT(C_SCRIBBLE_PATTERN != *pMchWord);
    pMchWord++;
  }
}

/*----------------------------------------------------------------------------*/
/* check memory has been intialised                                           */
/*----------------------------------------------------------------------------*/
void MemLib::vCheckInitialised(void* aAddress, i8 nBytes)
{
  u8* pMchWord = reinterpret_cast<u8*>(aAddress);
  i8  nWords = nBytes / 8;
  
  for (i8 i_Word = 0; i_Word < nWords; i_Word++) {
    
    if (C_SCRIBBLE_PATTERN == *pMchWord) {
      MemLib::vWordDump(aAddress,nBytes,i_Word);
      vDebugTrap();
    }
    
    ASSERT(C_SCRIBBLE_PATTERN != *pMchWord);
    pMchWord++;
  }
}

/*----------------------------------------------------------------------------*/
/* copy from src to dst                                                       */
/*----------------------------------------------------------------------------*/
void MemLib::vCopy(void* pDst, const void* pSrc, i8 nBytes, eCN eClassNo)
{
  memcpy(pDst,pSrc,nBytes);
  
  if (gpAttachMap) {
    gpAttachMap->m_iMemCopyBytes += nBytes;
    gpAttachMap->m_xMemCopy[eClassNo] += nBytes;
  }
}

/*----------------------------------------------------------------------------*/
/* free a block of memory                                                     */
/*----------------------------------------------------------------------------*/
void MemLib::vFreeBlock(void* aBlock, i8 nBytes)
{
  free(aBlock);
  
  gxResourceTracker.vSubMallocAmt(nBytes);
}

/*----------------------------------------------------------------------------*/
/* get total amount if memory on the machine                                  */
/*----------------------------------------------------------------------------*/
i8 MemLib::iGetTotalMemory(void)
{
  i8 iPages    = sysconf(_SC_PHYS_PAGES);
  i8 iPageSize = sysconf(_SC_PAGE_SIZE);
  
  return iPages * iPageSize;
}

/*----------------------------------------------------------------------------*/
/* dump given bytes of memory at given address                                */
/*----------------------------------------------------------------------------*/
void MemLib::vHexDump(void *aAddress, i8 nBytes)
{
  
  i8 iOffset = 0;
  i8 iByte = 0;
  
  // dump a header
  vTrace("\n%18s %8s %49s %16s\n"_Stz,
         "MemLib address"_Stz,
         "Offset"_Stz,
         "(0a is \\n ; 0d is \\r)                    Hex dump"_Stz,
         "Context"_Stz);
  
  vTrace("%18s %8s %49s %16s\n"_Stz,
         "------------------"_Stz, "--------"_Stz,
         "-------------------------------------------------"_Stz, "----------------"_Stz);
  
  // dump the memory
  for (i8 i = 0; i < nBytes; i++) {
    
    // beginning of line, dump the address
    if (iByte == 0) {
      vTrace("0x%016x "_Stz, (i8)((u1*)aAddress + iOffset));
      vTrace("0x%06x "_Stz, iOffset);
    }
    
    vTrace("%02x "_Stz, (i8)(*((u1*)aAddress + i)));
    
    iByte++;
    
    // middle of line ?
    if (iByte == C_BYTES_PER_LINE/2) {
      vTrace("- "_Stz);
    }
    
    // newline ?
    if (iByte == C_BYTES_PER_LINE) {
      
      for (i8 i = 0; i < C_BYTES_PER_LINE; i++) {
        
        u1 cChar = *((u1*)aAddress + iOffset + i);
        
        if (cChar < C_ASCII_MIN || cChar > C_ASCII_MAX) {
          vTrace("%c"_Stz, '.');
        } else {
          vTrace("%c"_Stz, cChar);
        }
      }
      
      iByte = 0;
      iOffset += C_BYTES_PER_LINE;
      vTrace("\n"_Stz);
    }
  }
  
  // end of buffer and not at 16-byte boundary ?
  if (iByte > 0 && iByte < C_BYTES_PER_LINE) {
    
    i8 i = C_BYTES_PER_LINE - iByte;
    
    while (i) {
      
      vTrace("%s "_Stz, "__"_Stz);
      iByte++;
      if (iByte == C_BYTES_PER_LINE/2) {
        vTrace("- "_Stz);
      }
      i--;
    }
    
    for (i8 i= 0; i < iByte; i++) {
      u1 cChar = *((u1*)aAddress + iOffset + i);
      if (cChar < C_ASCII_MIN || cChar > C_ASCII_MAX) {
        vTrace("%c"_Stz, '.');
      } else {
        vTrace("%c"_Stz, cChar);
      }
    }
    vTrace("\n"_Stz);
  }
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* trace memory statistics                                                    */
/*----------------------------------------------------------------------------*/
void MemLib::vList(void)
{
  s32 sByteMag;
  
  Magnitudes::vByteMag(gpAttachMap->m_iMemCopyBytes, sByteMag);
  vTrace("MemLib::vCopy() total %s\n"_Stz,sByteMag.aStz());
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (gpAttachMap->m_xMemCopy[i_Class]) {
      Magnitudes::vByteMag(gpAttachMap->m_xMemCopy[i_Class], sByteMag);
      vTrace("  %s %s\n"_Stz,aCN[i_Class],sByteMag.aStz());
    }
  }
  
  Magnitudes::vByteMag(gpAttachMap->m_iMemMoveBytes, sByteMag);
  vTrace("MemLib::vMove() total %s\n"_Stz,sByteMag.aStz());
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (gpAttachMap->m_xMemMove[i_Class]) {
      Magnitudes::vByteMag(gpAttachMap->m_xMemMove[i_Class], sByteMag);
      vTrace("  %s %s\n"_Stz,aCN[i_Class],sByteMag.aStz());
    }
  }
  
  Magnitudes::vByteMag(gpAttachMap->m_iMemScribbleBytes, sByteMag);
  vTrace("MemLib::vScribble() total %s\n"_Stz,sByteMag.aStz());
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (gpAttachMap->m_xMemScribble[i_Class]) {
      Magnitudes::vByteMag(gpAttachMap->m_xMemScribble[i_Class], sByteMag);
      vTrace("  %s %s\n"_Stz,aCN[i_Class],sByteMag.aStz());
    }
  }
  
  Magnitudes::vByteMag(gpAttachMap->m_iMemSetBytes, sByteMag);
  vTrace("MemLib::vSet() total %s\n"_Stz,sByteMag.aStz());
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (gpAttachMap->m_xMemSet[i_Class]) {
      Magnitudes::vByteMag(gpAttachMap->m_xMemSet[i_Class], sByteMag);
      vTrace("  %s %s\n"_Stz,aCN[i_Class],sByteMag.aStz());
    }
  }
  
  Magnitudes::vByteMag(gpAttachMap->m_iMemZeroBytes, sByteMag);
  vTrace("MemLib::vZero() total %s\n"_Stz,sByteMag.aStz());
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (gpAttachMap->m_xMemZero[i_Class]) {
      Magnitudes::vByteMag(gpAttachMap->m_xMemZero[i_Class], sByteMag);
      vTrace("  %s %s\n"_Stz,aCN[i_Class],sByteMag.aStz());
    }
  }
  
  i8 iGrandTotal = gpAttachMap->m_iMemCopyBytes
                 + gpAttachMap->m_iMemMoveBytes
                 + gpAttachMap->m_iMemScribbleBytes
                 + gpAttachMap->m_iMemSetBytes
                 + gpAttachMap->m_iMemZeroBytes
                 ;
  Magnitudes::vByteMag(iGrandTotal, sByteMag);
  vTrace("Grand Total %s\n"_Stz,sByteMag.aStz());
}

/*----------------------------------------------------------------------------*/
/* allocate a block of memory                                                 */
/*----------------------------------------------------------------------------*/
void* MemLib::aMallocBlock(i8 nBytes)
{
  void* aBlock = (void*)malloc(nBytes);
  
  gxResourceTracker.vAddMallocAmt(nBytes);
  
  return aBlock;
}

/*----------------------------------------------------------------------------*/
/* move from src to dst                                                       */
/*----------------------------------------------------------------------------*/
void MemLib::vMove(void* pDst, const void* pSrc, i8 nBytes, eCN eClassNo)
{
  memmove(pDst,pSrc,nBytes);
  
  if (gpAttachMap) {
    gpAttachMap->m_iMemMoveBytes += nBytes;
    gpAttachMap->m_xMemMove[eClassNo] += nBytes;
  }
}

/*----------------------------------------------------------------------------*/
/* reset memory statistics                                                    */
/*----------------------------------------------------------------------------*/
void MemLib::vReset(void)
{
  gpAttachMap->m_iMemCopyBytes = 0;
  gpAttachMap->m_iMemMoveBytes = 0;
  gpAttachMap->m_iMemScribbleBytes = 0;
  gpAttachMap->m_iMemSetBytes  = 0;
  gpAttachMap->m_iMemZeroBytes = 0;
  
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    gpAttachMap->m_xMemCopy[i_Class] = 0;
    gpAttachMap->m_xMemMove[i_Class] = 0;
    gpAttachMap->m_xMemScribble[i_Class] = 0;
    gpAttachMap->m_xMemSet[i_Class] = 0;
    gpAttachMap->m_xMemZero[i_Class] = 0;
  }
}

/*----------------------------------------------------------------------------*/
/* scribble on memory                                                         */
/*----------------------------------------------------------------------------*/
void MemLib::vScribble(void* aAddress, i8 nBytes, eCN eClassNo)
{
  u8* pMchWord = reinterpret_cast<u8*>(aAddress);
  i8  nWords = nBytes / 8;
  
  for (i8 i_Word = 0; i_Word < nWords; i_Word++)
    *pMchWord++ = C_SCRIBBLE_PATTERN;
  
  if (gpAttachMap) {
    gpAttachMap->m_iMemScribbleBytes += nBytes;
    gpAttachMap->m_xMemScribble[eClassNo] += nBytes;
  }
}

/*----------------------------------------------------------------------------*/
/* memset dst                                                                 */
/*----------------------------------------------------------------------------*/
void MemLib::vSet(void* pDst, char iByte, i8 nBytes, eCN eClassNo)
{
  memset(pDst,iByte,nBytes);
  
  if (gpAttachMap) {
    gpAttachMap->m_iMemSetBytes += nBytes;
    gpAttachMap->m_xMemSet[eClassNo] += nBytes;
  }
}

/*----------------------------------------------------------------------------*/
/* dump given words of memory at given address                                */
/*----------------------------------------------------------------------------*/
void MemLib::vWordDump(void *aAddress, i8 iBytes, i8 iHighlightWord)
{
  i8 iOffset = 0;
  i8 iWord   = 0;
  i8 iWords  = iBytes/C_SIZEOF_WORD;

  // dump a header
  vTrace("\n"_Stz);
  vTrace("%6s %8s %18s %16s %8s\n"_Stz,
         "Word"_Stz,
         "Offset"_Stz,
         "Address"_Stz,
         "Hex"_Stz,
         "Ascii"_Stz);
  
  vTrace("%6s %8s %18s %16s %8s\n"_Stz,
         "------"_Stz,
         "--------"_Stz,
         "------------------"_Stz,
         "-------><-------"_Stz,
         "--------"_Stz);
  
  // dump the memory
  for (i8 i = 0; i < iWords; i++) {
    
    // beginning of line, dump the address
    if (iWord == 0) {
      vTrace("%6d "_Stz, i);
      vTrace("0x%06llx "_Stz, iOffset);
      vTrace("0x%016llx "_Stz, (i8)((u1*)aAddress + iOffset));
    }
    
    vTrace("%016llx "_Stz, (i8)(*((i8*)aAddress + i)));
    
    iWord++;
      
    for (i8 i = 0; i < C_SIZEOF_WORD; i++) {
      
      u1 cChar = *((u1*)aAddress + iOffset + i);
      
      if (cChar < C_ASCII_MIN || cChar > C_ASCII_MAX) {
        vTrace("%c"_Stz, '.');
      } else {
        vTrace("%c"_Stz, cChar);
      }
    }
    
    if (i == iHighlightWord)
      vTrace(" <<==== OBSERVE"_Stz);
    
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKER0)
      vTrace(" <<==== MARKER 0"_Stz);
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKER1)
      vTrace(" <<==== MARKER 1"_Stz);
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKER2)
      vTrace(" <<==== MARKER 2"_Stz);
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKER3)
      vTrace(" <<==== MARKER 3"_Stz);
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKER4)
      vTrace(" <<==== MARKER 4"_Stz);
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKER5)
      vTrace(" <<==== MARKER 5"_Stz);
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKER6)
      vTrace(" <<==== MARKER 6"_Stz);
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKER7)
      vTrace(" <<==== MARKER 7"_Stz);
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKER8)
      vTrace(" <<==== MARKER 8"_Stz);
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKER9)
      vTrace(" <<==== MARKER 9"_Stz);
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKERA)
      vTrace(" <<==== MARKER A"_Stz);
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKERB)
      vTrace(" <<==== MARKER B"_Stz);
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKERC)
      vTrace(" <<==== MARKER C"_Stz);
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKERD)
      vTrace(" <<==== MARKER D"_Stz);
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKERE)
      vTrace(" <<==== MARKER E"_Stz);
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKERF)
      vTrace(" <<==== MARKER F"_Stz);
    
    iWord = 0;
    iOffset += C_SIZEOF_WORD;
    vTrace("\n"_Stz);
  }
  
  // end of buffer and not at 16-byte boundary ?
  if (iWord > 0 && iWord < C_SIZEOF_WORD) {
    
    for (i8 i= 0; i < iWord; i++) {
      u1 cChar = *((u1*)aAddress + iOffset + i);
      if (cChar < C_ASCII_MIN || cChar > C_ASCII_MAX) {
        vTrace("%c"_Stz, '.');
      } else {
        vTrace("%c"_Stz, cChar);
      }
    }
    
    vTrace("\n"_Stz);
  }
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* zero dst                                                                   */
/*----------------------------------------------------------------------------*/
void MemLib::vZero(void* pDst, i8 nBytes, eCN eClassNo)
{
  vSet(pDst,(char)0,nBytes,eClassNo);
  
  if (gpAttachMap) {
    gpAttachMap->m_iMemZeroBytes += nBytes;
    gpAttachMap->m_xMemZero[eClassNo] += nBytes;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
