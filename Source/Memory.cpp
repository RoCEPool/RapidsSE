/*============================================================================*/
/*  Memory.cpp - Memory                                                       */
/*============================================================================*/

#include "Common.h"

/*----------------------------------------------------------------------------*/
/* map a new an anonymous region of memory (private or shared)                */
/*----------------------------------------------------------------------------*/
void* Memory::pAddBytes(void* pAddress, i8 iBytes)
{
  char* pOrigin = reinterpret_cast<char*>(pAddress);
  
  char* pNew = pOrigin + iBytes;
  
  return pNew;
}

/*----------------------------------------------------------------------------*/
/* copy from src to dst                                                       */
/*----------------------------------------------------------------------------*/
void Memory::vCopy(void* pDst, const void* pSrc, i8 iBytes)
{
  memcpy(pDst,pSrc,iBytes);
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
/* dump given bytes of memory at given address                                */
/*----------------------------------------------------------------------------*/
void Memory::vHexDump(void *pAddress, i8 iBytes)
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
      vTrace("0x%016x ", (i8)((u1*)pAddress + iOffset));
      vTrace("0x%06x ", iOffset);
    }
    
    vTrace("%02x ", (i8)(*((u1*)pAddress + i)));
    
    iByte++;
    
    // middle of line ?
    if (iByte == C_BYTES_PER_LINE/2) {
      vTrace("- ");
    }
    
    // newline ?
    if (iByte == C_BYTES_PER_LINE) {
      
      for (i8 i = 0; i < C_BYTES_PER_LINE; i++) {
        
        u1 cChar = *((u1*)pAddress + iOffset + i);
        
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
      u1 cChar = *((u1*)pAddress + iOffset + i);
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
/* dump given words of memory at given address                                */
/*----------------------------------------------------------------------------*/
void Memory::vWordDump(void *pAddress, i8 iBytes)
{
  
  i8 iOffset = 0;
  i8 iWord = 0;
  i8 iWords = iBytes/SIZEOF_WORD;

  // dump a header
  vTrace("\n%18s %8s %16s %8s\n",
         "Memory address", "Offset", "Hex dump", "Context");
  vTrace("%18s %8s %16s %8s\n",
         "------------------", "--------",
         "----------------", "--------");
  
  // dump the memory
  for (i8 i = 0; i < iWords; i++) {
    
    // beginning of line, dump the address
    if (iWord == 0) {
      vTrace("0x%016x ", (i8)((u1*)pAddress + iOffset));
      vTrace("0x%06x ", iOffset);
    }
    
    vTrace("%016x ", (i8)(*((i8*)pAddress + i)));
    
    iWord++;
      
    for (i8 i = 0; i < SIZEOF_WORD; i++) {
      
      u1 cChar = *((u1*)pAddress + iOffset + i);
      
      if (cChar < C_ASCII_MIN || cChar > C_ASCII_MAX) {
        vTrace("%c", '.');
      } else {
        vTrace("%c", cChar);
      }
    }
    
    iWord = 0;
    iOffset += SIZEOF_WORD;
    vTrace("\n");
  }
  
  // end of buffer and not at 16-byte boundary ?
  if (iWord > 0 && iWord < SIZEOF_WORD) {
    
    for (i8 i= 0; i < iWord; i++) {
      u1 cChar = *((u1*)pAddress + iOffset + i);
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