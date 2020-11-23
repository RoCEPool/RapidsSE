/*============================================================================*/
/* AggTimer.hpp -- AggTimer                                                   */
/*============================================================================*/
#pragma once

namespace Gadgets
{
  class AggTimer : public Object
  {
  public:
  
  private:
  
    i8 m_iTicks;   //
  
    i8 m_nTimes;   //
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void* pAggTimerAddress);
  
    AggTimer(void);
   ~AggTimer(void) noexcept(false);
    
    void vAdd(i8 iTicks);
    void vAdd(Timer* pTimer);
    void vReset(void);
    void vSet(i8 iTicks);
    
    i8 iNanoseconds(void);
    i8 iMicroseconds(void);
    i8 iMilliseconds(void);
    i8 iSeconds(void);
    i8 iTicks(void);
    i8 nTimes(void);

    d8 dRatePerSec(i8 nItems);

    Footer* aFooter(void);
  };
  
  AggTimer* _pAggTimer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
