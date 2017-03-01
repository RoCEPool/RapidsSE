/*============================================================================*/
/* ScanBuffer.hpp -- Scanning Buffer                                          */
/*============================================================================*/

#pragma once

#include "Structures.h"

namespace Structures
{
  class ScanBuffer : public Buffer,
                     public WspFxd<C_1M> // note this is 1M not 1MB
  {
  private:
    char* m_aNextMsg;  // address where next message starts
    char* m_aEndData;  // address where data ends
        
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ScanBuffer(void);
    ~ScanBuffer(void);
    
    char* aInto(i8& iAvailableBytes);
    void  vMore(i8& iMoreBytes);
    char* aScan(i8& iMessageBytes);
    
  private:
    i8    iBytesAvailable(void);
    void  vCopyUp(char* aDangling);
    char* aFirstByte(void);
    char* aLastByte(void);
    void  vReset(void);
    void  vWipe(void);
    void  vWipe(char* aFrom);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
