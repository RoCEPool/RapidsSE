/*============================================================================*/
/*  Memory.cpp - Memory                                                       */
/*============================================================================*/

#include "Common.h"

extern i8 giClassSize[];

/*----------------------------------------------------------------------------*/
/* map a new an anonymous region of memory (private or shared)                */
/*----------------------------------------------------------------------------*/
void* Memory::pAddBytes(void* aAddress, i8 iBytes)
{
  char* pOrigin = reinterpret_cast<char*>(aAddress);
  
  char* pNew = pOrigin + iBytes;
  
  return pNew;
}

/*----------------------------------------------------------------------------*/
/* check memory has been intialised                                           */
/*----------------------------------------------------------------------------*/
void Memory::vCheckInitialised(void* aAddress, eCN eClass)
{
  u8* pMchWord = reinterpret_cast<u8*>(aAddress);
  i8  nBytes = giClassSize[eClass];
  i8  nWords = nBytes / 8;

  for (i8 iWord = 0; iWord < nWords; iWord++) {
    
    if (C_SCRIBBLE_PATTERN == *pMchWord) {
      Memory::vWordDump(aAddress,nBytes,iWord);
      vDebugTrap();
    }
    
    ASSERT(C_SCRIBBLE_PATTERN != *pMchWord);
    pMchWord++;
  }
}

/*----------------------------------------------------------------------------*/
/* check memory has been intialised                                           */
/*----------------------------------------------------------------------------*/
void Memory::vCheckInitialised(void* aAddress, i8 nBytes)
{
  u8* pMchWord = reinterpret_cast<u8*>(aAddress);
  i8  nWords = nBytes / 8;
  
  for (i8 iWord = 0; iWord < nWords; iWord++) {
    
    if (C_SCRIBBLE_PATTERN == *pMchWord) {
      Memory::vWordDump(aAddress,nBytes,iWord);
      vDebugTrap();
    }
    
    ASSERT(C_SCRIBBLE_PATTERN != *pMchWord);
    pMchWord++;
  }
}

/*----------------------------------------------------------------------------*/
/* copy from src to dst                                                       */
/*----------------------------------------------------------------------------*/
void Memory::vCopy(void* pDst, const void* pSrc, i8 iBytes)
{
  memcpy(pDst,pSrc,iBytes);
}

/*----------------------------------------------------------------------------*/
/* dump given bytes of memory at given address                                */
/*----------------------------------------------------------------------------*/
void Memory::vHexDump(void *aAddress, i8 iBytes)
{
  
  i8 iOffset = 0;
  i8 iByte = 0;
  
  // dump a header
  vTrace("\n%18s %8s %49s %16s\n",
         "Memory address", "Offset", "Hex dump", "Context");
  vTrace("%18s %8s %49s %16s\n",
         "------------------", "--------",
         "-------------------------------------------------", "----------------");
  
  // dump the memory
  for (i8 i = 0; i < iBytes; i++) {
    
    // beginning of line, dump the address
    if (iByte == 0) {
      vTrace("0x%016x ", (i8)((u1*)aAddress + iOffset));
      vTrace("0x%06x ", iOffset);
    }
    
    vTrace("%02x ", (i8)(*((u1*)aAddress + i)));
    
    iByte++;
    
    // middle of line ?
    if (iByte == C_BYTES_PER_LINE/2) {
      vTrace("- ");
    }
    
    // newline ?
    if (iByte == C_BYTES_PER_LINE) {
      
      for (i8 i = 0; i < C_BYTES_PER_LINE; i++) {
        
        u1 cChar = *((u1*)aAddress + iOffset + i);
        
        if (cChar < C_ASCII_MIN || cChar > C_ASCII_MAX) {
          vTrace("%c", '.');
        } else {
          vTrace("%c", cChar);
        }
      }
      
      iByte = 0;
      iOffset += C_BYTES_PER_LINE;
      vTrace("\n");
    }
  }
  
  // end of buffer and not at 16-byte boundary ?
  if (iByte > 0 && iByte < C_BYTES_PER_LINE) {
    
    i8 i = C_BYTES_PER_LINE - iByte;
    
    while (i) {
      
      vTrace("%s ", "__");
      iByte++;
      if (iByte == C_BYTES_PER_LINE/2) {
        vTrace("- ");
      }
      i--;
    }
    
    for (i8 i= 0; i < iByte; i++) {
      u1 cChar = *((u1*)aAddress + iOffset + i);
      if (cChar < C_ASCII_MIN || cChar > C_ASCII_MAX) {
        vTrace("%c", '.');
      } else {
        vTrace("%c", cChar);
      }
    }
    vTrace("\n");
  }
  vTrace("\n");
}

/*----------------------------------------------------------------------------*/
/* memset dst                                                                 */
/*----------------------------------------------------------------------------*/
void Memory::vSet(void* pDst, char iByte, i8 iTimes)
{
  memset(pDst,iByte,iTimes);
}

/*----------------------------------------------------------------------------*/
/* move from src to dst                                                       */
/*----------------------------------------------------------------------------*/
void Memory::vMove(void* pDst, const void* pSrc, i8 iBytes)
{
  memmove(pDst,pSrc,iBytes);
}

/*----------------------------------------------------------------------------*/
/* scribble on memory                                                         */
/*----------------------------------------------------------------------------*/
void Memory::vScribble(void* aAddress, i8 nBytes)
{
  u8* pMchWord = reinterpret_cast<u8*>(aAddress);
  i8  nWords = nBytes / 8;
  
  for (i8 iWord = 0; iWord < nWords; iWord++)
    *pMchWord++ = C_SCRIBBLE_PATTERN;
}

/*----------------------------------------------------------------------------*/
/* dump given words of memory at given address                                */
/*----------------------------------------------------------------------------*/
void Memory::vWordDump(void *aAddress, i8 iBytes, i8 iHighlightWord)
{
  i8 iOffset = 0;
  i8 iWord   = 0;
  i8 iWords  = iBytes/SIZEOF_WORD;

  // dump a header
  vTrace("\n");
  vTrace("%6s %8s %18s %16s %8s\n",
         "Word",
         "Offset",
         "Address",
         "Hex",
         "Ascii");
  
  vTrace("%6s %8s %18s %16s %8s\n",
         "------",
         "--------",
         "------------------",
         "-------><-------",
         "--------");
  
  // dump the memory
  for (i8 i = 0; i < iWords; i++) {
    
    // beginning of line, dump the address
    if (iWord == 0) {
      vTrace("%6d ", i);
      vTrace("0x%06llx ", iOffset);
      vTrace("0x%016llx ", (i8)((u1*)aAddress + iOffset));
    }
    
    vTrace("%016llx ", (i8)(*((i8*)aAddress + i)));
    
    iWord++;
      
    for (i8 i = 0; i < SIZEOF_WORD; i++) {
      
      u1 cChar = *((u1*)aAddress + iOffset + i);
      
      if (cChar < C_ASCII_MIN || cChar > C_ASCII_MAX) {
        vTrace("%c", '.');
      } else {
        vTrace("%c", cChar);
      }
    }
    
    if (i == iHighlightWord)
      vTrace(" <<==== OBSERVE");
    
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKER0)
      vTrace(" <<==== MARKER 0");
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKER1)
      vTrace(" <<==== MARKER 1");
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKER2)
      vTrace(" <<==== MARKER 2");
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKER3)
      vTrace(" <<==== MARKER 3");
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKER4)
      vTrace(" <<==== MARKER 4");
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKER5)
      vTrace(" <<==== MARKER 5");
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKER6)
      vTrace(" <<==== MARKER 6");
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKER7)
      vTrace(" <<==== MARKER 7");
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKER8)
      vTrace(" <<==== MARKER 8");
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKER9)
      vTrace(" <<==== MARKER 9");
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKERA)
      vTrace(" <<==== MARKER A");
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKERB)
      vTrace(" <<==== MARKER B");
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKERC)
      vTrace(" <<==== MARKER C");
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKERD)
      vTrace(" <<==== MARKER D");
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKERE)
      vTrace(" <<==== MARKER E");
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKERF)
      vTrace(" <<==== MARKER F");
    
    iWord = 0;
    iOffset += SIZEOF_WORD;
    vTrace("\n");
  }
  
  // end of buffer and not at 16-byte boundary ?
  if (iWord > 0 && iWord < SIZEOF_WORD) {
    
    for (i8 i= 0; i < iWord; i++) {
      u1 cChar = *((u1*)aAddress + iOffset + i);
      if (cChar < C_ASCII_MIN || cChar > C_ASCII_MAX) {
        vTrace("%c", '.');
      } else {
        vTrace("%c", cChar);
      }
    }
    
    vTrace("\n");
  }
  
  vTrace("\n");
}

/*----------------------------------------------------------------------------*/
/* zero dst                                                                   */
/*----------------------------------------------------------------------------*/
void Memory::vZero(void* pDst, i8 iBytes)
{
  vSet(pDst,(char)0,iBytes);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
