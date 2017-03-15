/*============================================================================*/
/* MemLib.cpp -- MemLib                                                       */
/*============================================================================*/

#include "Base.h"

#include "Debugging.h"

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

  for (i8 iWord = 0; iWord < nWords; iWord++) {
    
    if (C_SCRIBBLE_PATTERN == *pMchWord) {
      MemLib::vWordDump(aAddress,nBytes,iWord);
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
  
  for (i8 iWord = 0; iWord < nWords; iWord++) {
    
    if (C_SCRIBBLE_PATTERN == *pMchWord) {
      MemLib::vWordDump(aAddress,nBytes,iWord);
      vDebugTrap();
    }
    
    ASSERT(C_SCRIBBLE_PATTERN != *pMchWord);
    pMchWord++;
  }
}

/*----------------------------------------------------------------------------*/
/* copy from src to dst                                                       */
/*----------------------------------------------------------------------------*/
void MemLib::vCopy(void* pDst, const void* pSrc, i8 iBytes)
{
  memcpy(pDst,pSrc,iBytes);
}

/*----------------------------------------------------------------------------*/
/* copy from src to dst                                                       */
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
void MemLib::vHexDump(void *aAddress, i8 iBytes)
{
  
  i8 iOffset = 0;
  i8 iByte = 0;
  
  // dump a header
  vTrace("\n%18s %8s %49s %16s\n"_Str,
         "MemLib address"_Str, "Offset"_Str, "Hex dump"_Str, "Context"_Str);
  vTrace("%18s %8s %49s %16s\n"_Str,
         "------------------"_Str, "--------"_Str,
         "-------------------------------------------------"_Str, "----------------"_Str);
  
  // dump the memory
  for (i8 i = 0; i < iBytes; i++) {
    
    // beginning of line, dump the address
    if (iByte == 0) {
      vTrace("0x%016x "_Str, (i8)((u1*)aAddress + iOffset));
      vTrace("0x%06x "_Str, iOffset);
    }
    
    vTrace("%02x "_Str, (i8)(*((u1*)aAddress + i)));
    
    iByte++;
    
    // middle of line ?
    if (iByte == C_BYTES_PER_LINE/2) {
      vTrace("- "_Str);
    }
    
    // newline ?
    if (iByte == C_BYTES_PER_LINE) {
      
      for (i8 i = 0; i < C_BYTES_PER_LINE; i++) {
        
        u1 cChar = *((u1*)aAddress + iOffset + i);
        
        if (cChar < C_ASCII_MIN || cChar > C_ASCII_MAX) {
          vTrace("%c"_Str, '.');
        } else {
          vTrace("%c"_Str, cChar);
        }
      }
      
      iByte = 0;
      iOffset += C_BYTES_PER_LINE;
      vTrace("\n"_Str);
    }
  }
  
  // end of buffer and not at 16-byte boundary ?
  if (iByte > 0 && iByte < C_BYTES_PER_LINE) {
    
    i8 i = C_BYTES_PER_LINE - iByte;
    
    while (i) {
      
      vTrace("%s "_Str, "__"_Str);
      iByte++;
      if (iByte == C_BYTES_PER_LINE/2) {
        vTrace("- "_Str);
      }
      i--;
    }
    
    for (i8 i= 0; i < iByte; i++) {
      u1 cChar = *((u1*)aAddress + iOffset + i);
      if (cChar < C_ASCII_MIN || cChar > C_ASCII_MAX) {
        vTrace("%c"_Str, '.');
      } else {
        vTrace("%c"_Str, cChar);
      }
    }
    vTrace("\n"_Str);
  }
  vTrace("\n"_Str);
}

/*----------------------------------------------------------------------------*/
/* memset dst                                                                 */
/*----------------------------------------------------------------------------*/
void MemLib::vSet(void* pDst, char iByte, i8 iTimes)
{
  memset(pDst,iByte,iTimes);
}

/*----------------------------------------------------------------------------*/
/* move from src to dst                                                       */
/*----------------------------------------------------------------------------*/
void MemLib::vMove(void* pDst, const void* pSrc, i8 iBytes)
{
  memmove(pDst,pSrc,iBytes);
}

/*----------------------------------------------------------------------------*/
/* scribble on memory                                                         */
/*----------------------------------------------------------------------------*/
void MemLib::vScribble(void* aAddress, i8 nBytes)
{
  u8* pMchWord = reinterpret_cast<u8*>(aAddress);
  i8  nWords = nBytes / 8;
  
  for (i8 iWord = 0; iWord < nWords; iWord++)
    *pMchWord++ = C_SCRIBBLE_PATTERN;
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
  vTrace("\n"_Str);
  vTrace("%6s %8s %18s %16s %8s\n"_Str,
         "Word"_Str,
         "Offset"_Str,
         "Address"_Str,
         "Hex"_Str,
         "Ascii"_Str);
  
  vTrace("%6s %8s %18s %16s %8s\n"_Str,
         "------"_Str,
         "--------"_Str,
         "------------------"_Str,
         "-------><-------"_Str,
         "--------"_Str);
  
  // dump the memory
  for (i8 i = 0; i < iWords; i++) {
    
    // beginning of line, dump the address
    if (iWord == 0) {
      vTrace("%6d "_Str, i);
      vTrace("0x%06llx "_Str, iOffset);
      vTrace("0x%016llx "_Str, (i8)((u1*)aAddress + iOffset));
    }
    
    vTrace("%016llx "_Str, (i8)(*((i8*)aAddress + i)));
    
    iWord++;
      
    for (i8 i = 0; i < C_SIZEOF_WORD; i++) {
      
      u1 cChar = *((u1*)aAddress + iOffset + i);
      
      if (cChar < C_ASCII_MIN || cChar > C_ASCII_MAX) {
        vTrace("%c"_Str, '.');
      } else {
        vTrace("%c"_Str, cChar);
      }
    }
    
    if (i == iHighlightWord)
      vTrace(" <<==== OBSERVE"_Str);
    
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKER0)
      vTrace(" <<==== MARKER 0"_Str);
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKER1)
      vTrace(" <<==== MARKER 1"_Str);
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKER2)
      vTrace(" <<==== MARKER 2"_Str);
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKER3)
      vTrace(" <<==== MARKER 3"_Str);
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKER4)
      vTrace(" <<==== MARKER 4"_Str);
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKER5)
      vTrace(" <<==== MARKER 5"_Str);
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKER6)
      vTrace(" <<==== MARKER 6"_Str);
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKER7)
      vTrace(" <<==== MARKER 7"_Str);
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKER8)
      vTrace(" <<==== MARKER 8"_Str);
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKER9)
      vTrace(" <<==== MARKER 9"_Str);
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKERA)
      vTrace(" <<==== MARKER A"_Str);
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKERB)
      vTrace(" <<==== MARKER B"_Str);
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKERC)
      vTrace(" <<==== MARKER C"_Str);
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKERD)
      vTrace(" <<==== MARKER D"_Str);
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKERE)
      vTrace(" <<==== MARKER E"_Str);
    if ((u8)(*((i8*)aAddress + i)) == C_MEMORY_MARKERF)
      vTrace(" <<==== MARKER F"_Str);
    
    iWord = 0;
    iOffset += C_SIZEOF_WORD;
    vTrace("\n"_Str);
  }
  
  // end of buffer and not at 16-byte boundary ?
  if (iWord > 0 && iWord < C_SIZEOF_WORD) {
    
    for (i8 i= 0; i < iWord; i++) {
      u1 cChar = *((u1*)aAddress + iOffset + i);
      if (cChar < C_ASCII_MIN || cChar > C_ASCII_MAX) {
        vTrace("%c"_Str, '.');
      } else {
        vTrace("%c"_Str, cChar);
      }
    }
    
    vTrace("\n"_Str);
  }
  
  vTrace("\n"_Str);
}

/*----------------------------------------------------------------------------*/
/* zero dst                                                                   */
/*----------------------------------------------------------------------------*/
void MemLib::vZero(void* pDst, i8 iBytes)
{
  vSet(pDst,(char)0,iBytes);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
