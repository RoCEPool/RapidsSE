/*============================================================================*/
/* Decider.hpp -- ack decider                                                 */
/*============================================================================*/
#pragma once

namespace System
{
  class Decider : public Object
  {
  private:
    i8 m_nAcksRemaining;  // the number of acks remaining
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t, Decider* pDecider);
    void  operator delete(void* aAddress);
    
    Decider(void);
    ~Decider(void);
    
    void operator =(i8 nAcksRemaining);
  
    void vCheckInit(void);
    bool bDecAndDecide(void);
    i8   iPrintOnlyValue(void);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
