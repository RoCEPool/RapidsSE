/*============================================================================*/
/* Predicte.hpp - RDP Predicate                                               */
/*============================================================================*/
#pragma once

#include "ePK.h"

namespace Plans
{  
  class Predicate : public Object
  {
  private:
    ePK    m_ePredicate;
    Arg*   m_pArg1;
    Arg*   m_pArg2;
    RelOp* m_pRelOp;
  
    // must be last
    Footer m_xFooter;
    
  public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    Predicate(void);
    ~Predicate(void) noexcept(false);
    
    ePK ePredicate(void);
    void vPredicate(ePK ePredicate);
  
    Arg* pArg1(void);
    void vArg1(Arg* pArg1);
  
    Arg* pArg2(void);
    void vArg2(Arg* pArg2);
  
    RelOp* pRelOp(void);
    void vRelOp(RelOp* pRelOp);
  
    void vShow(void);
  
    s256& sColumnNm(void);
    s256& sDatumStr(void);
  
    void vNewLitArg(s256& sDatumArg);
    void vNewRelOp(eRO eRelOp);
  
    Footer* aFooter(void);
  };
  
  Predicate* _pPredicate(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
