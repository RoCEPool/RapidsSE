/*============================================================================*/
/* ScanBuffer.hpp -- Scanning Buffer                                          */
/*============================================================================*/
#pragma once

namespace Structures
{
  class ScanBuffer : public Buffer
  {
  private:
    Str* m_aNextMsg;  // address where next message starts
    Str* m_aEndData;  // address where data ends
    WspFxd<C_1M> m_xWsp; // note this is 1M not 1MB
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ScanBuffer(void);
    ~ScanBuffer(void);
    
    Str* aInto(i8& iAvailableBytes);
    void vMore(i8& iMoreBytes);
    Str* aScan(i8& iMessageBytes);
    
  private:
    i8   iBytesAvailable(void);
    void vCopyUp(Str* aDangling);
    Str* aFirstByte(void);
    Str* aLastByte(void);
    void vReset(void);
    void vWipe(void);
    void vWipe(Str* aFrom);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
