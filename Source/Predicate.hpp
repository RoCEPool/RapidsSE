/*============================================================================*/
/* Predicte.hpp - RDP Predicate                                               */
/*============================================================================*/
#pragma once

namespace Plans
{  
  enum ePK // predicate kind
  {
  /* 0 */ PK_NONE=0,
  /* 1 */ PK_COLUMN_COLUMN_OP,
  /* 2 */ PK_COLUMN_LITERAL_OP,
  /* 3 */ PK_LITERAL_LITERAL_OP,
  /* 4 */ PK_LITERAL_COLUMN_OP,
  /* 5 */ PK_ENUMS
  };
  
  static Str* aPK[]
  {
  /* 0 */ "none"_Str,
  /* 1 */ "column column op"_Str,
  /* 2 */ "column literal op"_Str,
  /* 3 */ "literal literal op"_Str,
  /* 4 */ "literal column op"_Str,
  /* 5 */ "enums"_Str,
  };
  
  class Predicate : public Object
  {
  private:
    ePK  m_ePredicate;
    Op*  m_pOp;
    Arg* m_pArg1;
    Arg* m_pArg2;
  
    // must be last
    Footer m_xFooter;
    
  public:
    
    void* operator new(size_t);
    void  operator delete(void*);
    
    Predicate(void);
    Predicate(ePK ePredicate, Arg* pArg1, Op* pOp, Arg* pArg2);
    ~Predicate(void);
    
    ePK ePredicate(void);
    void vPredicate(ePK ePredicate);
  
    Arg* pArg1(void);
    void vArg1(Arg* pArg1);
  
    Arg* pArg2(void);
    void vArg2(Arg* pArg2);
  
    Op* pOp(void);
    void vOp(Op* pOp);
  
    void vShow(void);
  
    Footer* aFooter(void);
  };
  
  Predicate* _pPredicate(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
