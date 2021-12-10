/*============================================================================*/
/* Decider.hpp -- ack decider                                                 */
/*============================================================================*/
#pragma once

namespace BASE
{
  class Decider : public Object
  {
  private:
    i8 m_nAcksOriginally; // the number of original acks
    i8 m_nAcksRemaining;  // the number of acks remaining
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t, Decider* pDecider);
    void  operator delete(void* aAddress);
    
    Decider(void);
    ~Decider(void) noexcept(false);
  
    void vCheckInit(void);
    bool bDecAndDecide(i8& nAcksWereRemaining);
    bool bIsSet(void);
    i8   nOriginalAcks(void);
    i8   iPrintOnlyValue(void);
    void vReset(void);
    void vResetRemaining(i8 nAcksRemaining);
    void vSetRemaining(i8 nAcksRemaining);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
