/*============================================================================*/
/* Timer.hpp -- timer                                                         */
/*============================================================================*/
#pragma once

namespace Measurement
{
  class Timer : public Object
  {
  public:
  
  private:
    i8 m_iStart;
    i8 m_iUpdate;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Timer* pTimer);
    void operator delete(void* pTimerAddress);
    Timer(void);
    ~Timer() noexcept(false);
    
    void vInit(void);
    void vReset(void);
    void vSet(i8 iStart, i8 iUpdate);
    void vStart(void);
    void vUpdate(void);
    
    i8 iNanoseconds(void);
    i8 iMicroseconds(void);
    i8 iMilliseconds(void);
    i8 iSeconds(void);
    i8 iTicks(void);
  
    i8 iStart(void);
    i8 iUpdate(void);

    Footer* aFooter(void);
  };
  
  Timer* _pTimer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
