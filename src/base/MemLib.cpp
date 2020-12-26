/*============================================================================*/
/* MemLib.cpp -- Memory Library                                               */
/*============================================================================*/

#include "Resources.h"

extern SizeHeap  gxaSizeHeap[];

/*----------------------------------------------------------------------------*/
/* add number of bytes to given address                                       */
/*----------------------------------------------------------------------------*/
void* MemLib::aAddBytes(void* aAddress, i8 nBytes)
{
  byte* ayOrigin = reinterpret_cast<byte*>(aAddress);
  
  byte* ayNew = ayOrigin + nBytes;
  
  return ayNew;
}

/*---------------------------------------------------------------------------*/
/* dump ascii string                                                         */
/*---------------------------------------------------------------------------*/
void MemLib::vAsciiDump(Stz* aText, i8 nBytes)
{
  vTrace(">>>"_Stz);
  while (nBytes--) {
    vTrace("%c"_Stz,*aText->azChar());
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
  i8  nBytes = gxaSizeHeap[eClass].m_nClassSize;
  i8  nWords = nBytes / 8;
  
  // if this fails check the size of the object in SizeHeap[]
  ASSERT(nWords);

  for (i8 i_Word = 0; i_Word < nWords; i_Word++) {
    
    if (C_SCRIBBLE_PATTERN == *pMchWord) {
      vTrace("Initialisation problem: class %s at word %lld\n"_Stz,
             aCN[eClass],i_Word);
      MemLib::vWordDump(aAddress,nBytes,i_Word);
      vDebugPoint();
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
      vDebugPoint();
    }
    
    ASSERT(C_SCRIBBLE_PATTERN != *pMchWord);
    pMchWord++;
  }
}

/*----------------------------------------------------------------------------*/
/* byte compare of arg1 with arg2                                             */
/*----------------------------------------------------------------------------*/
bool MemLib::bCompareBytes(const void* aArg1, const void* aArg2, i8 nBytes)
{
  if (0 == iCompareBytes(aArg1,aArg2,nBytes))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* byte compare of arg1 with arg2                                             */
/*----------------------------------------------------------------------------*/
i8 MemLib::iCompareBytes(const void* aArg1, const void* aArg2, i8 nBytes)
{
  const byte *yByte1 = (byte*)aArg1;
  const byte *yByte2 = (byte*)aArg2;
  
  while (nBytes--) {
    if (*yByte1++ != *yByte2++)
      return yByte2[-1] - yByte1[-1];
  }
  
  return 0;
}

/*----------------------------------------------------------------------------*/
/* word compare of arg1 with arg2                                             */
/*----------------------------------------------------------------------------*/
bool MemLib::bCompareWords(const void* aArg1, const void* aArg2, i8 nBytes)
{
  if (0 == iCompareWords(aArg1,aArg2,nBytes))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* cut bytes off                                                              */
/*----------------------------------------------------------------------------*/
void MemLib::vCut(void* aAddr, i8 nSize, i8 i_Offset, i8 nCutBytes, eCN eClassNo)
{
  i8 iStartMove = i_Offset + nCutBytes ;
  RASSERT(iStartMove <= nSize);
  
  // get the start address
  byte* aSrc = aAdd<byte*>(aAddr, iStartMove);
  
  // get the dst address
  byte* aDst = aAdd<byte*>(aAddr, i_Offset);
  
  // copy nCutBytes from src to dst
  MemLib::vCopy(aDst, aSrc, nSize - (iStartMove), eClassNo);
  
  // calculate the end address after cut
  i8 iLen = nSize - nCutBytes;
  byte* aAddrEnd = aAdd<byte*>(aAddr, iLen);
  
  // zero the last nCutBytes
  MemLib::vZero(aAddrEnd, nCutBytes, eClassNo);
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
      
      if (cChar < C_ASCII_MIN_VIZ || cChar > C_ASCII_MAX_VIZ)
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
    else if (C_DATUM_NULL == iVal) {
      vTrace(" [Is Null]"_Stz);
    }
    else if (C_DATUM_MINUS_INFINITY == iVal) {
      vTrace(" [Minus Infinity]"_Stz);
    }
    else if (C_DATUM_PLUS_INFINITY == iVal) {
      vTrace(" [Plus Infinity]"_Stz);
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
    if (cChar < C_ASCII_MIN_VIZ || cChar > C_ASCII_MAX_VIZ)
      vTrace("%c"_Stz, '.');
    else
      vTrace("%c"_Stz, cChar);
  }
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* dump given bytes of memory at given address                                */
/*----------------------------------------------------------------------------*/
/*
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
      
      if (cChar < C_ASCII_MIN_VIZ || cChar > C_ASCII_MAX_VIZ)
        vTrace("%c"_Stz, '.');
      else
        vTrace("%c"_Stz, cChar);
    }
    
    Header* aHeader = aAdd<Header*>(aAddress,(i_Word * C_BYTES_PER_WORD));
    if (aHeader->bValidMagic()) {
      vTrace(" [Header: Class %lld %s]"_Stz,
             aHeader->eClass(),
             aCN[aHeader->eClass()]);
    }
    Footer* aFooter = aAdd<Footer*>(aAddress,(i_Word * C_BYTES_PER_WORD));
    if (aFooter->bValidFooter()) {
      vTrace(" [Footer]"_Stz);
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
    if (cChar < C_ASCII_MIN_VIZ || cChar > C_ASCII_MAX_VIZ)
      vTrace("%c"_Stz, '.');
    else
      vTrace("%c"_Stz, cChar);
  }
  
  vTrace("\n"_Stz);
}
*/

/*----------------------------------------------------------------------------*/
/* extend bytes to make space for additional data                             */
/*----------------------------------------------------------------------------*/
void MemLib::vExpand(void* aAddr, i8 nSize, i8 i_Offset, i8 nAddBytes, eCN eClassNo)
{
  i8 iStartMove = i_Offset;
  RASSERT(iStartMove + nAddBytes <= nSize);
  
  // get the start address
  byte* aSrc = aAdd<byte*>(aAddr, iStartMove);
  
  // get the dst address
  byte* aDst = aAdd<byte*>(aAddr, i_Offset + nAddBytes);
  
  // copy nExtendBytes from src to dst
  MemLib::vCopy(aDst, aSrc, nSize - (iStartMove), eClassNo);
}

/*----------------------------------------------------------------------------*/
/* find non-zero end of word-array                                            */
/*----------------------------------------------------------------------------*/
w8* MemLib::awFindNonZeroEnd(void* aWords, i8 nWords)
{
  w8* awWords = (w8*)aWords;
  
  for (i8 i_Word = nWords - 1; i_Word >= 0; i_Word--)
    if (awWords[i_Word])
      return &awWords[i_Word];
  
  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* free a block of memory                                                     */
/*----------------------------------------------------------------------------*/
void MemLib::vFreeBlock(void* aBlock, i8 nBytes)
{
  free(aBlock);
  
  //if (AM::bAttached())
  //  AM::pResourceTracker()->vSubMallocAmt(nBytes);

  //    AM::vLogRes(SL_INF,m_iResId,RK_MALLOC,"Free'd : %lld bytes"_Stz,nBytes); //!format-LogRes
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
  i8 nByteInLine = 0;
  
  // write header
  vTrace("\n%18s %8s %49s %16s\n"_Stz,
         "Address"_Stz,
         "Offset"_Stz,
         "(0a is \\n ; 0d is \\r)                    Hex dump"_Stz,
         "Context"_Stz);
  
  vTrace("%18s %8s %49s %16s\n"_Stz,
         "------------------"_Stz,
         "--------"_Stz,
         "-------------------------------------------------"_Stz,
         "----------------"_Stz);
  
  
  // step thru each byte to be dumped
  for (i8 i_Byte = 0; i_Byte < nBytes; i_Byte++) {
    
    // at beginning of line, write address
    if (nByteInLine == 0) {
      vTrace("0x%016x "_Stz, (i8)((u1*)aAddress + iOffset));
      vTrace("0x%06x "_Stz, iOffset);
    }
    
    // dump the hex version of the byte
    vTrace("%02x "_Stz, (i8)(*((u1*)aAddress + i_Byte)));
    nByteInLine++;
    
    // at midpoint of hex version, write separator
    if (nByteInLine == C_BYTES_PER_LINE/2) {
      vTrace("- "_Stz);
    }
    
    // end of hex portion for this line?
    if (nByteInLine != C_BYTES_PER_LINE)
      continue;
    
    // at end of line, write char version of bytes dumped on this line
    for (i8 i_Char = 0; i_Char < C_BYTES_PER_LINE; i_Char++) {
      
      u1 cChar = *((u1*)aAddress + iOffset + i_Char);
      
      if (cChar < C_ASCII_MIN_VIZ || cChar > C_ASCII_MAX_VIZ)
        vTrace("%c"_Stz, '.');
      else
        vTrace("%c"_Stz, cChar);
    }

    // start a new line
    nByteInLine = 0;
    iOffset += C_BYTES_PER_LINE;
    vTrace("\n"_Stz);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 
  // end of dump and on a 16-byte boundary
  if (nByteInLine == 0 || nByteInLine == C_BYTES_PER_LINE) {
    vTrace("\n"_Stz);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // end of dump and not at 16-byte boundary
  
  i8 i_Remaining = C_BYTES_PER_LINE - nByteInLine;

  // write an "__" filler for bytes not dumped
  while (i_Remaining--) {
    vTrace("%s "_Stz, "__"_Stz);
    nByteInLine++;
    if (nByteInLine == C_BYTES_PER_LINE/2) {
      vTrace("- "_Stz);
    }
  }
  
  for (i8 i_Char = 0; i_Char < nByteInLine; i_Char++) {
    u1 cChar = *((u1*)aAddress + iOffset + i_Char);
    if (cChar < C_ASCII_MIN_VIZ || cChar > C_ASCII_MAX_VIZ)
      vTrace("%c"_Stz, '.');
    else
      vTrace("%c"_Stz, cChar);
  }
  
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* allocate a block of memory                                                 */
/*----------------------------------------------------------------------------*/
void* MemLib::aMallocBlock(i8 nBytes)
{
  void* aBlock = (void*)malloc(nBytes);
  
  //if (AM::bAttached())
  //  AM::pResourceTracker()->vAddMallocAmt(nBytes);
  //    AM::vLogRes(SL_INF,m_iResId,RK_MALLOC,"Malloc'd : %lld bytes"_Stz,nBytes); //!format-LogRes

  return aBlock;
}

/*----------------------------------------------------------------------------*/
/* move from src to dst                                                       */
/*----------------------------------------------------------------------------*/
void MemLib::vMove(void* pDst, const void* pSrc, i8 nBytes, eCN eClassNo)
{
  memmove(pDst,pSrc,nBytes);
 
#ifdef WITH_STATISTICS_COUNTERS
  if (gpAttachMap) {
    pStats->Memory.m_iMoveTotal += nBytes;
    pStats->Memory.m_iaMove[eClassNo] += nBytes;
  }
#endif
  
}

/*----------------------------------------------------------------------------*/
/* reset memory statistics                                                    */
/*----------------------------------------------------------------------------*/
void MemLib::vReset(void)
{
  Statistics* pStats = AM::pStatistics();

  pStats->Memory.m_iCopyTotal = 0;
  pStats->Memory.m_iCompareTotal = 0;
  pStats->Memory.m_iMoveTotal = 0;
  pStats->Memory.m_iScribbleTotal = 0;
  pStats->Memory.m_iSetTotal  = 0;
  pStats->Memory.m_iZeroTotal = 0;
  
  for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++) {
    pStats->Memory.m_iaCopy[i_Class] = 0;
    pStats->Memory.m_iaMove[i_Class] = 0;
    pStats->Memory.m_iaScribble[i_Class] = 0;
    pStats->Memory.m_iaSet[i_Class] = 0;
    pStats->Memory.m_iaZero[i_Class] = 0;
  }
}

/*----------------------------------------------------------------------------*/
/* rotate memory by given number of bytes                                     */
/*----------------------------------------------------------------------------*/
void MemLib::vRotate(void* avArg, i8 nBytes, i8 nBy)
{
  byte yaFront[32];//[nBy];

  MemLib::vCopy(yaFront,avArg,nBy,CN_NONE);
  
  MemLib::vMove(avArg,(byte*)avArg+nBy,nBytes-nBy,CN_NONE);

  MemLib::vCopy((byte*)avArg + nBytes - nBy,yaFront,nBy,CN_NONE);
}

/*----------------------------------------------------------------------------*/
/* compute the bytes required to hold n bytes within given quanta             */
/*----------------------------------------------------------------------------*/
i8 MemLib::nRoundUp(i8 nBytes, i8 nSizeQuanta)
{
  i8 nQuanta = nQuantaRequired(nBytes,nSizeQuanta);

  i8 nRoundedBytes = nQuanta * nSizeQuanta;
  
  return nRoundedBytes;
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

#ifdef WITH_STATISTICS_COUNTERS
  if (gpAttachMap) {
    pStats->Memory.m_iScribbleTotal += nBytes;
    pStats->Memory.m_iaScribble[eClassNo] += nBytes;
  }
#endif
  
}

/*----------------------------------------------------------------------------*/
/* memset dst                                                                 */
/*----------------------------------------------------------------------------*/
void MemLib::vSet(void* pDst, char iByte, i8 nBytes, eCN eClassNo)
{
  memset(pDst,iByte,nBytes);

#ifdef WITH_STATISTICS_COUNTERS
  if (gpAttachMap) {
    pStats->Memory.m_iSetTotal += nBytes;
    pStats->Memory.m_iaSet[eClassNo] += nBytes;
  }
#endif
  
}

/*----------------------------------------------------------------------------*/
/* compute the nByte boundary address of given address                        */
/*----------------------------------------------------------------------------*/
void* MemLib::aToBoundary(void* aAddress, i8 nByteBoundary)
{
  u8 uBitMask = BITWISE_NOT((nByteBoundary - 1));
  
  void* aBoundary = pBitAnd<void*>(aAddress,uBitMask);
 
  return aBoundary;
}

/*----------------------------------------------------------------------------*/
/* dump given words of memory at given address                                */
/*----------------------------------------------------------------------------*/
void MemLib::vWordDump(void *aAddress, i8 nBytes, i8 iHighlightWord)
{
  i8 iOffset = 0;
  i8 iWord   = 0;
  i8 nWords  = MemLib::nQuantaRequired(nBytes,C_BYTES_PER_WORD);

  // dump a header
  vTrace("\n"_Stz);
  vTrace("%4s %6s %6s %14s %16s %8s\n"_Stz,
         "SingleWord"_Stz,
         "Offset"_Stz,
         "Offset"_Stz,
         "Address"_Stz,
         "Hex"_Stz,
         "Ascii"_Stz);
  
  vTrace("%4s %6s %6s %14s %16s %8s\n"_Stz,
         "----"_Stz,
         "------"_Stz,
         "------"_Stz,
         "--------------"_Stz,
         "-------><-------"_Stz,
         "--------"_Stz);
  
  // dump the memory
  for (i8 i_Word = 0; i_Word < nWords; i_Word++) {
    
    // beginning of line, dump the address
    if (iWord == 0) {
      vTrace("%4d "_Stz, i_Word);
      vTrace("0x%04llx "_Stz, iOffset);
      vTrace("%06lld "_Stz, iOffset);
      vTrace("0x%012llx "_Stz, (i8)((u1*)aAddress + iOffset));
    }
    
    vTrace("%016llx "_Stz, (i8)(*((i8*)aAddress + i_Word)));
    
    iWord++;
      
    for (i8 i_Byte = 0; i_Byte < (i8)C_BYTES_PER_WORD; i_Byte++) {
      
      u1 cChar = *((u1*)aAddress + iOffset + i_Byte);
      
      if (cChar < C_ASCII_MIN_VIZ || cChar > C_ASCII_MAX_VIZ)
        vTrace("%c"_Stz, '.');
      else
        vTrace("%c"_Stz, cChar);
    }
    
    if (i_Word == iHighlightWord)
      vTrace(" <<==== OBSERVE"_Stz);
    
    if ((u8)(*((i8*)aAddress + i_Word)) == C_MEMORY_MARKER0)
      vTrace(" <<==== MARKER 0"_Stz);
    if ((u8)(*((i8*)aAddress + i_Word)) == C_MEMORY_MARKER1)
      vTrace(" <<==== MARKER 1"_Stz);
    if ((u8)(*((i8*)aAddress + i_Word)) == C_MEMORY_MARKER2)
      vTrace(" <<==== MARKER 2"_Stz);
    if ((u8)(*((i8*)aAddress + i_Word)) == C_MEMORY_MARKER3)
      vTrace(" <<==== MARKER 3"_Stz);
    if ((u8)(*((i8*)aAddress + i_Word)) == C_MEMORY_MARKER4)
      vTrace(" <<==== MARKER 4"_Stz);
    if ((u8)(*((i8*)aAddress + i_Word)) == C_MEMORY_MARKER5)
      vTrace(" <<==== MARKER 5"_Stz);
    if ((u8)(*((i8*)aAddress + i_Word)) == C_MEMORY_MARKER6)
      vTrace(" <<==== MARKER 6"_Stz);
    if ((u8)(*((i8*)aAddress + i_Word)) == C_MEMORY_MARKER7)
      vTrace(" <<==== MARKER 7"_Stz);
    if ((u8)(*((i8*)aAddress + i_Word)) == C_MEMORY_MARKER8)
      vTrace(" <<==== MARKER 8"_Stz);
    if ((u8)(*((i8*)aAddress + i_Word)) == C_MEMORY_MARKER9)
      vTrace(" <<==== MARKER 9"_Stz);
    if ((u8)(*((i8*)aAddress + i_Word)) == C_MEMORY_MARKERA)
      vTrace(" <<==== MARKER A"_Stz);
    if ((u8)(*((i8*)aAddress + i_Word)) == C_MEMORY_MARKERB)
      vTrace(" <<==== MARKER B"_Stz);
    if ((u8)(*((i8*)aAddress + i_Word)) == C_MEMORY_MARKERC)
      vTrace(" <<==== MARKER C"_Stz);
    if ((u8)(*((i8*)aAddress + i_Word)) == C_MEMORY_MARKERD)
      vTrace(" <<==== MARKER D"_Stz);
    if ((u8)(*((i8*)aAddress + i_Word)) == C_MEMORY_MARKERE)
      vTrace(" <<==== MARKER E"_Stz);
    if ((u8)(*((i8*)aAddress + i_Word)) == C_MEMORY_MARKERF)
      vTrace(" <<==== MARKER F"_Stz);

    if ((u8)(*((i8*)aAddress + i_Word)) == C_SEEME)
      vTrace(" <<==== SEE ME"_Stz);
    
    
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
    else if (C_DATUM_NULL == iVal) {
      vTrace(" [Is Null]"_Stz);
    }
    else if (C_DATUM_MINUS_INFINITY == iVal) {
      vTrace(" [Minus Infinity]"_Stz);
    }
    else if (C_DATUM_PLUS_INFINITY == iVal) {
      vTrace(" [Plus Infinity]"_Stz);
    }


    iWord = 0;
    iOffset += C_BYTES_PER_WORD;
    vTrace("\n"_Stz);
  }
  
  // end of buffer and not at word boundary ?
  if (iWord > 0 && iWord < (i8)C_BYTES_PER_WORD) {
    
    for (i8 i = 0; i < iWord; i++) {
      u1 cChar = *((u1*)aAddress + iOffset + i);
      if (cChar < C_ASCII_MIN_VIZ || cChar > C_ASCII_MAX_VIZ) {
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
  _WordAsBytes* aAsBytes = (_WordAsBytes*)aWord;
  
  byte yByte;
  
  yByte = aAsBytes->m_yaByte[0];
  aAsBytes->m_yaByte[0] = aAsBytes->m_yaByte[7];
  aAsBytes->m_yaByte[7] = yByte;
  
  yByte = aAsBytes->m_yaByte[1];
  aAsBytes->m_yaByte[1] = aAsBytes->m_yaByte[6];
  aAsBytes->m_yaByte[6] = yByte;
  
  yByte = aAsBytes->m_yaByte[2];
  aAsBytes->m_yaByte[2] = aAsBytes->m_yaByte[5];
  aAsBytes->m_yaByte[5] = yByte;
  
  yByte = aAsBytes->m_yaByte[3];
  aAsBytes->m_yaByte[3] = aAsBytes->m_yaByte[4];
  aAsBytes->m_yaByte[4] = yByte;
}

/*----------------------------------------------------------------------------*/
/* zero dst                                                                   */
/*----------------------------------------------------------------------------*/
void MemLib::vZero(void* pDst, i8 nBytes, eCN eClassNo)
{
  vSet(pDst,(char)0,nBytes,eClassNo);
 
#ifdef WITH_STATISTICS_COUNTERS
  if (gpAttachMap) {
    pStats->Memory.m_iZeroTotal += nBytes;
    pStats->Memory.m_iaZero[eClassNo] += nBytes;
  }
#endif
  
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
