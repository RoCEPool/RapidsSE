/*============================================================================*/
/* ScanBuffer.hpp -- Scanning Buffer                                          */
/*============================================================================*/
#pragma once

namespace Structures
{
  class ScanBuffer : public Object
  {
    static const i8 C_WSP_SCAN = C_1M; // note this is 1M not 1MB
  private:
    Stz* m_aNextMsg;  // address where next message starts
    Stz* m_aEndData;  // address where data ends
  
    Wsp_t<C_WSP_SCAN> m_xWsp;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    ScanBuffer(void);
    ~ScanBuffer(void) noexcept(false);
    
    Stz* aInto(i8& iAvailableBytes);
    void vMore(i8& iMoreBytes);
    Stz* aScan(i8& iMessageBytes);
    
  private:
    i8   iBytesAvailable(void);
    void vCopyUp(Stz* aDangling);
    Stz* aFirstByte(void);
    Stz* aLastByte(void);
    void vReset(void);
    void vWipe(void);
    void vWipe(Stz* aFrom);
  
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
