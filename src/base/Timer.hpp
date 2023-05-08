/*============================================================================*/
/* Timer.hpp -- for timing things in nanoseconds (as opposed to "ticks")      */
/*============================================================================*/
#pragma once

namespace BASE
{
  class Timer : public Object
  {
  public:
  
  private:
    u8 m_uStart;   // starting nanos
    u8 m_uUpdate;  // update point
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void* pTimerAddress);
  
    Timer(void);
   ~Timer(void) noexcept(false);

    void operator =(Timer xTimer);

    static void vRepair(Object* pObject);

    void vReset(void);
    void vSet(u8 uStart, u8 uUpdate);
    void vStart(void);
    void vUpdate(void);
    
    i8 iMagnitude(i8 iBase = 1000);
    i8 iMicroseconds(void);
    i8 iMilliseconds(void);
    i8 iNanoseconds(void);
    i8 iSeconds(void);
    i8 iDelta(void);

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
