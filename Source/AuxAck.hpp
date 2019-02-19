/*============================================================================*/
/* AuxAck.hpp -- Auxilary Ack                                                 */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class AuxAck : public Object
  {
  private:
    i8 m_nTapsOriginally; // the number of original taps
    i8 m_nTapsRemaining;  // the number of taps remaining
  
    bool m_bTwiddling;    // job is twiddling, waiting to be acked
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t, AuxAck* pTapper);
    void  operator delete(void* aAddress);
    
    AuxAck(void);
    ~AuxAck(void) noexcept(false);
  
    void vCheckInit(void);
    bool bDecAndDecide(void);
    bool bIsSet(void);
    i8   nOriginalTaps(void);
    i8   iPrintOnlyValue(void);
    void vSetRemaining(i8 nTapsRemaining);
    bool bTwiddling(void);
    void vTwiddling(bool bTwiddling);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
