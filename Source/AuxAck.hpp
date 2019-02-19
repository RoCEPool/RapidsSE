/*============================================================================*/
/* AuxAck.hpp -- Auxilary Ack                                                 */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class AuxAck : public Object
  {
  private:
    i8 m_nAcksOriginally; // the number of original taps
    i8 m_nAcksRemaining;  // the number of taps remaining
  
    bool m_bPassed;       // has aux job passed the decision point?
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t, AuxAck* pAuxAck);
    void  operator delete(void* aAddress);
    
    AuxAck(void);
    ~AuxAck(void) noexcept(false);
  
    void vCheckInit(void);
    bool bDecAndDecide(void);
    bool bIsSet(void);
    i8   nOriginalAcks(void);
    i8   iPrintOnlyValue(void);
    void vSetRemaining(i8 nAcksRemaining);
    bool bPassed(void);
    void vPassed(bool bTwiddling);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
