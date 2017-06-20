/*============================================================================*/
/* Timer.hpp -- timer                                                         */
/*============================================================================*/
#pragma once

namespace System
{
  class Timer : public Object
  {
  public:
  
  private:
    i8      m_iStart;
    i8      m_iUpdate;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Timer* pTimer);
    void operator delete(void* pTimerAddress);
    Timer(void);
    ~Timer();
    
    void vInit(void);
    void vReset(void);
    void vStart(void);
    void vUpdate(void);
    
    i8   iNanoseconds(void);
    i8   iMicroseconds(void);
    i8   iMilliseconds(void);
    i8   iSeconds(void);
    i8   iTicks(void);

    Footer* aFooter(void);
  };
  
  Timer* _pTimer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
