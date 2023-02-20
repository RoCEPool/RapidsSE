/*============================================================================*/
/* CarryForward.hpp -- Carry Forward                                          */
/*============================================================================*/
#pragma once

#include "Bench.hpp"


namespace STORE
{
  class CarryForward : public Object
  {
  private:
    Card_Expr* m_p_sqCrlKey;
    bool       m_bCorrelationResolved;

    Card_Expr* m_p_sqResult;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    CarryForward(void);
   ~CarryForward(void) noexcept(false);

    bool bCorrelationResolved(void);
    void vCorrelationResolved(bool bCorrelationResolved);

    bool bIsCorrelated(void);

    Card_Expr* p_sqCrlKey(void);
    void v_sqCrlKey(Card_Expr* p_sqCrlKey);

    Card_Expr* p_sqResult(void);
    void v_sqResult(Card_Expr* p_sqResult);

  protected:
    Footer* aFooter(void);
  };

  CarryForward* _pCarryForward(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
