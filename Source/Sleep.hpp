/*============================================================================*/
/* Sleep.hpp - how long to sleep for ?                                        */
/*============================================================================*/
#pragma once

namespace Gadgets
{
  class Sleep : public Object
  {
  public:
    Dtm m_wSleptTsp;    // tsp when slept
    Dtm m_wWakeupTsp;   // wakeup tsp
    Dtm m_wWakeupIvl;   // wakeup ivl epoch
    Dtm m_wDurationIvl; // sleep duration ivl
  
    // must be last
    Footer m_xFooter;
    
  public:
    Sleep();
    ~Sleep() noexcept(false);

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
