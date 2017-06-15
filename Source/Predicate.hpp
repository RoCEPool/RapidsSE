/*============================================================================*/
/* Predicte.hpp - RDP Predicate                                               */
/*============================================================================*/
#pragma once

namespace Plans
{  
  class Predicate : public Object
  {
  private:
    Op*  m_pOp;
    Arg* m_pArg1;
    Arg* m_pArg2;
  
    // must be last
    Footer m_xFooter;
    
  public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    Predicate(void);
    Predicate(Arg* pArg1, Op* pOp, Arg* pArg2);
    ~Predicate(void);
  
    Arg* pArg1(void);
    void vArg1(Arg* pArg1);
  
    Arg* pArg2(void);
    void vArg2(Arg* pArg2);
  
    Op* pOp(void);
    void vOp(Op* pOp);
  
    Footer* pFooter(void);
  };
  
  Predicate* _pPredicate(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
