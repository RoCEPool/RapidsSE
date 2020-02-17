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
    i8 m_iStart;   // starting ticks
    i8 m_iUpdate;  // update point
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void operator delete(void* pAggTimerAddress);
  
    AggTimer(void);
   ~AggTimer(void) noexcept(false);
    
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
  
  AggTimer* _pAggTimer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
