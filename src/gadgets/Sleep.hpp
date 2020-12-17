/*============================================================================*/
/* Sleep.hpp - how long to sleep for ?                                        */
/*============================================================================*/
#pragma once

namespace Resources
{
  class Sleep : public Object
  {
  public:
    Ivl m_wSleptIvl;    // slept ivl
    Ivl m_wWakeupIvl;   // wakeup ivl epoch
    Ivl m_wDurationIvl; // sleep duration ivl
  
    // must be last
    Footer m_xFooter;
    
  public:
    Sleep(void);
   ~Sleep(void) noexcept(false);

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
