/*============================================================================*/
/* Timer.hpp -- timer                                                         */
/*============================================================================*/
#pragma once

namespace Gadgets
{
  class Timer : public Object
  {
  public:
  
  private:
    i8 m_iStart;   // starting ticks
    i8 m_iUpdate;  // update point
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void* pTimerAddress);
  
    Timer(void);
   ~Timer(void) noexcept(false);
    
    void vReset(void);
    void vSet(i8 iStart, i8 iUpdate);
    void vStart(void);
    void vUpdate(void);
    
    i8 iNanoseconds(void);
    i8 iMicroseconds(void);
    i8 iMilliseconds(void);
    i8 iSeconds(void);
    i8 iTickDelta(void);
  
    i8 iStart(void);
    i8 iUpdate(void);

    d8 dRatePerSec(i8 nItems);
  
    Footer* aFooter(void);
  };
  
  Timer* _pTimer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
