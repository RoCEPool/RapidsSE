/*============================================================================*/
/* MemLib.cpp -- Memory Library                                               */
/*============================================================================*/

#include "StorageEngine.h"
#include "Base.h"

#include "Debugging.h"

extern AttachMap* gpAttachMap;

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

/*---------------------------------------------------------------------------*/
/* dump ascii string                                                         */
/*---------------------------------------------------------------------------*/
void MemLib::vAsciiDump(Stz* aText, i8 nBytes)
{
  vTrace(">>>"_Stz);
  while (nBytes--) {
    vTrace("%c"_Stz,*aText->aChar());
    aText++;
  }
  vTrace("<<<\n"_Stz);
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
    gpAttachMap->m_oStatistics.Memory.m_iCopyTotal += nBytes;
    gpAttachMap->m_oStatistics.Memory.m_iaCopy[eClassNo] += nBytes;
  }
}

/*----------------------------------------------------------------------------*/
/* compare val1 to val2                                                       */
/*----------------------------------------------------------------------------*/
i8 MemLib::iCompareWords(const void* pVal1, const void* pVal2, i8 nBytes)
{
  //  if (gpAttachMap) {
  //    gpAttachMap->m_oStatistics.Memory.m_iCompareTotal += nBytes;
  //  }
  
  // nBytes should be a multiple of 8
  RASSERT(nBytes%8 == 0);
  
  const i8 *iWord1 = (const i8*)pVal1;
  const i8 *iWord2 = (const i8*)pVal2;

  while (nBytes > 0)
  {
    if (*iWord1++ != *iWord2++ )
      return iWord1[-1] < iWord2[-1] ? -1 : 1;
    nBytes -=8;
  }
  
  return 0;
}

/*----------------------------------------------------------------------------*/
/* dump given bytes of memory at given address                                */
/*----------------------------------------------------------------------------*/
void MemLib::vDumpWords(void *aAddress, i8 nBytes)
{
  i8 i_Byte = 0;
  i8 i_Word = 0;
  
  // write header
  vTrace("\n%18s %8s %23s %8s\n"_Stz,
         "Address"_Stz,
         "Offset"_Stz,
         "(0a \\n; 0d \\r)      Hex"_Stz,
         "Chars"_Stz);
  
  vTrace("%18s %8s %23s %8s\n"_Stz,
         "------------------"_Stz,
         "--------"_Stz,
         "-----------------------"_Stz,
         "--------"_Stz);
  
  
  // step thru each byte to be dumped
  for (i_Byte = 0; i_Byte < nBytes; i_Byte++) {
    
    // at beginning of line, write address
    if (i_Byte % C_BYTES_PER_WORD == 0) {
      vTrace("0x%016x "_Stz,(i8)((u1*)aAddress + (i_Word * C_BYTES_PER_WORD)));
      vTrace("0x%06x "_Stz,i_Byte);
    }
    
    // dump the hex version of the byte
    vTrace("%02x "_Stz, (i8)(*((u1*)aAddress + i_Byte)));
    
    // end of hex portion for this line?
    if (i_Byte % C_BYTES_PER_WORD != C_BYTES_PER_WORD - 1)
      continue;
    
    // at end of line, write char version of bytes dumped on this line
    for (u8 i_Char = 0; i_Char < C_BYTES_PER_WORD; i_Char++) {
      
      u1 cChar = *((u1*)aAddress + (i_Word * C_BYTES_PER_WORD) + i_Char);
      
      if (cChar < C_ASCII_MIN || cChar > C_ASCII_MAX)
        vTrace("%c"_Stz, '.');
      else
        vTrace("%c"_Stz, cChar);
    }
    
    Header* aHeader = aAdd<Header*>(aAddress,(i_Word * C_BYTES_PER_WORD));
    Footer* aFooter = aAdd<Footer*>(aAddress,(i_Word * C_BYTES_PER_WORD));
    i8 iVal = *aAdd<i8*>(aAddress,(i_Word * C_BYTES_PER_WORD));
    
    if (aHeader->bValidMagic()) {
      vTrace(" [Header: Class %lld %s]"_Stz,
             aHeader->eClass(),
             aCN[aHeader->eClass()]);
    }
    else if (aFooter->bValidFooter()) {
      vTrace(" [Footer]"_Stz);
    }
    else if (C_NULL_INT == iVal) {
      vTrace(" is null"_Stz);
    }
    else {
      vTrace(" i8 = %lld"_Stz,iVal);
    }
    
    // start a new line
    i_Word++;
    vTrace("\n"_Stz);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // end of dump and on a word boundary
  if (i_Byte % C_BYTES_PER_WORD == 0 ||
      i_Byte % C_BYTES_PER_WORD == C_BYTES_PER_WORD - 1) {
    vTrace("\n"_Stz);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // end of dump and not at 16-byte boundary
  
  i8 i_Remaining = C_BYTES_PER_WORD - i_Byte;
  
  // write an "__" filler for bytes not dumped
  while (i_Remaining--)
    vTrace("%s "_Stz, "__"_Stz);
  
  for (u8 i_Char = 0; i_Char < i_Byte % C_BYTES_PER_WORD; i_Char++) {
    u1 cChar = *((u1*)aAddress + i_Word + i_Char);
    if (cChar < C_ASCII_MIN || cChar > C_ASCII_MAX)
      vTrace("%c"_Stz, '.');
    else
      vTrace("%c"_Stz, cChar);
  }
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* free a block of memory                                                     */
/*----------------------------------------------------------------------------*/
void MemLib::vFreeBlock(void* aBlock, i8 nBytes)
{
  free(aBlock);
  
  if (gpAttachMap) gpAttachMap->m_oResourceTracker.vSubMallocAmt(nBytes);
  if (gpAttachMap->m_bMarkResourceAll || gpAttachMap->m_bMarkResource[SR_MALLOC])
    ActivityMap::vMarkRes(SR_MALLOC,"SysRes: Free'd %lld bytes"_Stz,nBytes);
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
/* allocate a block of memory                                                 */
/*----------------------------------------------------------------------------*/
void* MemLib::aMallocBlock(i8 nBytes)
{
  void* aBlock = (void*)malloc(nBytes);
  
  if (gpAttachMap) gpAttachMap->m_oResourceTracker.vAddMallocAmt(nBytes);
    if (gpAttachMap->m_bMarkResourceAll || gpAttachMap->m_bMarkResource[SR_MALLOC])
  ActivityMap::vMarkRes(SR_MALLOC,"SysRes: Malloc'd %lld bytes"_Stz,nBytes);

  return aBlock;
}

/*----------------------------------------------------------------------------*/
/* move from src to dst                                                       */
/*----------------------------------------------------------------------------*/
void MemLib::vMove(void* pDst, const void* pSrc, i8 nBytes, eCN eClassNo)
{
  memmove(pDst,pSrc,nBytes);
  
  if (gpAttachMap) {
    gpAttachMap->m_oStatistics.Memory.m_iMoveTotal += nBytes;
    gpAttachMap->m_oStatistics.Memory.m_iaMove[eClassNo] += nBytes;
  }
}

/*----------------------------------------------------------------------------*/
/* reset memory statistics                                                    */
/*----------------------------------------------------------------------------*/
void MemLib::vReset(void)
{
  gpAttachMap->m_oStatistics.Memory.m_iCopyTotal = 0;
  gpAttachMap->m_oStatistics.Memory.m_iCompareTotal = 0;
  gpAttachMap->m_oStatistics.Memory.m_iMoveTotal = 0;
  gpAttachMap->m_oStatistics.Memory.m_iScribbleTotal = 0;
  gpAttachMap->m_oStatistics.Memory.m_iSetTotal  = 0;
  gpAttachMap->m_oStatistics.Memory.m_iZeroTotal = 0;
  
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    gpAttachMap->m_oStatistics.Memory.m_iaCopy[i_Class] = 0;
    gpAttachMap->m_oStatistics.Memory.m_iaMove[i_Class] = 0;
    gpAttachMap->m_oStatistics.Memory.m_iaScribble[i_Class] = 0;
    gpAttachMap->m_oStatistics.Memory.m_iaSet[i_Class] = 0;
    gpAttachMap->m_oStatistics.Memory.m_iaZero[i_Class] = 0;
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
    gpAttachMap->m_oStatistics.Memory.m_iScribbleTotal += nBytes;
    gpAttachMap->m_oStatistics.Memory.m_iaScribble[eClassNo] += nBytes;
  }
}

/*----------------------------------------------------------------------------*/
/* memset dst                                                                 */
/*----------------------------------------------------------------------------*/
void MemLib::vSet(void* pDst, char iByte, i8 nBytes, eCN eClassNo)
{
  memset(pDst,iByte,nBytes);
  
  if (gpAttachMap) {
    gpAttachMap->m_oStatistics.Memory.m_iSetTotal += nBytes;
    gpAttachMap->m_oStatistics.Memory.m_iaSet[eClassNo] += nBytes;
  }
}

/*----------------------------------------------------------------------------*/
/* trace memory statistics                                                    */
/*----------------------------------------------------------------------------*/
void MemLib::vStatistics(void)
{
  s32 sByteMag;
  
  Magnitudes::vByteMag(gpAttachMap->m_oStatistics.Memory.m_iCopyTotal, sByteMag);
  vTrace("MemLib::vCopy() total %s\n"_Stz,sByteMag.aStz());
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (gpAttachMap->m_oStatistics.Memory.m_iaCopy[i_Class]) {
      Magnitudes::vByteMag(gpAttachMap->m_oStatistics.Memory.m_iaCopy[i_Class], sByteMag);
      vTrace("  %s %s\n"_Stz,aCN[i_Class],sByteMag.aStz());
    }
  }
  
  Magnitudes::vByteMag(gpAttachMap->m_oStatistics.Memory.m_iCompareTotal, sByteMag);
  vTrace("MemLib::vCompare() total %s\n"_Stz,sByteMag.aStz());
  
  Magnitudes::vByteMag(gpAttachMap->m_oStatistics.Memory.m_iMoveTotal, sByteMag);
  vTrace("MemLib::vMove() total %s\n"_Stz,sByteMag.aStz());
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (gpAttachMap->m_oStatistics.Memory.m_iaMove[i_Class]) {
      Magnitudes::vByteMag(gpAttachMap->m_oStatistics.Memory.m_iaMove[i_Class], sByteMag);
      vTrace("  %s %s\n"_Stz,aCN[i_Class],sByteMag.aStz());
    }
  }
  
  Magnitudes::vByteMag(gpAttachMap->m_oStatistics.Memory.m_iScribbleTotal, sByteMag);
  vTrace("MemLib::vScribble() total %s\n"_Stz,sByteMag.aStz());
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (gpAttachMap->m_oStatistics.Memory.m_iaScribble[i_Class]) {
      Magnitudes::vByteMag(gpAttachMap->m_oStatistics.Memory.m_iaScribble[i_Class], sByteMag);
      vTrace("  %s %s\n"_Stz,aCN[i_Class],sByteMag.aStz());
    }
  }
  
  Magnitudes::vByteMag(gpAttachMap->m_oStatistics.Memory.m_iSetTotal, sByteMag);
  vTrace("MemLib::vSet() total %s\n"_Stz,sByteMag.aStz());
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (gpAttachMap->m_oStatistics.Memory.m_iaSet[i_Class]) {
      Magnitudes::vByteMag(gpAttachMap->m_oStatistics.Memory.m_iaSet[i_Class], sByteMag);
      vTrace("  %s %s\n"_Stz,aCN[i_Class],sByteMag.aStz());
    }
  }
  
  Magnitudes::vByteMag(gpAttachMap->m_oStatistics.Memory.m_iZeroTotal, sByteMag);
  vTrace("MemLib::vZero() total %s\n"_Stz,sByteMag.aStz());
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    if (gpAttachMap->m_oStatistics.Memory.m_iaZero[i_Class]) {
      Magnitudes::vByteMag(gpAttachMap->m_oStatistics.Memory.m_iaZero[i_Class], sByteMag);
      vTrace("  %s %s\n"_Stz,aCN[i_Class],sByteMag.aStz());
    }
  }
  
  i8 iGrandTotal = gpAttachMap->m_oStatistics.Memory.m_iCopyTotal
                    + gpAttachMap->m_oStatistics.Memory.m_iMoveTotal
                    + gpAttachMap->m_oStatistics.Memory.m_iScribbleTotal
                    + gpAttachMap->m_oStatistics.Memory.m_iSetTotal
                    + gpAttachMap->m_oStatistics.Memory.m_iZeroTotal
                    ;
  Magnitudes::vByteMag(iGrandTotal, sByteMag);
  vTrace("Grand Total %s\n"_Stz,sByteMag.aStz());
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
/* flip the bytes in given word                                               */
/*----------------------------------------------------------------------------*/
void MemLib::vWordFlip(w8* aWord)
{
  wordAsBytes* aAsBytes = (wordAsBytes*)aWord;
  
  y1 yByte;
  
  yByte = aAsBytes->m_caByte[0];
  aAsBytes->m_caByte[0] = aAsBytes->m_caByte[7];
  aAsBytes->m_caByte[7] = yByte;
  
  yByte = aAsBytes->m_caByte[1];
  aAsBytes->m_caByte[1] = aAsBytes->m_caByte[6];
  aAsBytes->m_caByte[6] = yByte;
  
  yByte = aAsBytes->m_caByte[2];
  aAsBytes->m_caByte[2] = aAsBytes->m_caByte[5];
  aAsBytes->m_caByte[5] = yByte;
  
  yByte = aAsBytes->m_caByte[3];
  aAsBytes->m_caByte[3] = aAsBytes->m_caByte[4];
  aAsBytes->m_caByte[4] = yByte;
}

/*----------------------------------------------------------------------------*/
/* zero dst                                                                   */
/*----------------------------------------------------------------------------*/
void MemLib::vZero(void* pDst, i8 nBytes, eCN eClassNo)
{
  vSet(pDst,(char)0,nBytes,eClassNo);
  
  if (gpAttachMap) {
    gpAttachMap->m_oStatistics.Memory.m_iZeroTotal += nBytes;
    gpAttachMap->m_oStatistics.Memory.m_iaZero[eClassNo] += nBytes;
  }
}

/*----------------------------------------------------------------------------*/
/* memeory map                                                                */
/*----------------------------------------------------------------------------*/
void MemLib::vMemoryMap(void)
{
  // still to be done
  
  //vm_map_t x = current_map();
  
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
