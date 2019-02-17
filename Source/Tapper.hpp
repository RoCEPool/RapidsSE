/*============================================================================*/
/* Tapper.hpp -- forward ack                                                  */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class Tapper : public Object
  {
  private:
    i8 m_nTapsOriginally; // the number of original taps
    i8 m_nTapsRemaining;  // the number of taps remaining
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t, Tapper* pTapper);
    void  operator delete(void* aAddress);
    
    Tapper(void);
    ~Tapper(void) noexcept(false);
  
    void vCheckInit(void);
    bool bDecAndDecide(void);
    bool bIsSet(void);
    i8   nOriginalTaps(void);
    i8   iPrintOnlyValue(void);
    void vSetRemaining(i8 nTapsRemaining);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
