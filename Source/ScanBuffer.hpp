/*============================================================================*/
/* ScanBuffer.hpp -- Scanning Buffer                                          */
/*============================================================================*/

#pragma once

#include "Structures.h"

namespace Structures
{
  class ScanBuffer : public Buffer
  {
  private:
    static const i8 C_WORKAREA = C_1008KB;
    
    char* m_pNextMsg;  // area where next message starts
    char* m_pEndData;  // area where data ends
    
    char  m_wWorkarea[C_WORKAREA];
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ScanBuffer(void);
    ~ScanBuffer(void);
    
    char* pInto(i8& iAvailableBytes);
    void  vMore(i8& iMoreBytes);
    char* pScan(i8& iMessageBytes);
    
  private:
    i8    iBytesAvailable(void);
    void  vCopyUp(char* pDangling);
    char* pFirstByte(void);
    char* pLastByte(void);
    void  vReset(void);
    void  vWipe(void);
    void  vWipe(char* pFrom);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/