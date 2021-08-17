/*============================================================================*/
/* Wakeup.hpp - when to be woken up ?                                         */
/*============================================================================*/
#pragma once

namespace BASE
{
  class Wakeup : public Object
  {
  public:
    Ivl m_wFrom;     // when to sleep at this ivl
    Ivl m_wDuration; // for duration duration ivl
    Ivl m_wWakeup;   // and to be wokenup at ivl
  
    // must be last
    Footer m_xFooter;
    
  public:
    Wakeup(void);
   ~Wakeup(void) noexcept(false);

    void vClear(void);
    void vSleepFor(i8 iMicroseconds);
    u8   uWakeupInUsec(void);
    bool bWakeupNow(void);
        
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
