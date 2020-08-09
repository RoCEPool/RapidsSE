/*============================================================================*/
/* LogicTimer.hpp -- Logic Timer                                              */
/*============================================================================*/
#pragma once

namespace Expressions
{
  class LogicTimer : public Object
  {
  private:
  
    Timer    m_oTmr;
    AggTimer m_oAggTmr[EO_ENUMS];
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    LogicTimer(void);
   ~LogicTimer(void) noexcept(false);
  
    void vBeginTmr(void);
    void vEndTmr(eEO eCode);

    void vReport(void);

  private:
  
    Footer* aFooter(void);
  };
  
  LogicTimer* _pLogicTimer(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
