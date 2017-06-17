/*============================================================================*/
/* Predicte.hpp - RDP Predicate                                               */
/*============================================================================*/
#pragma once

namespace Plans
{  
  enum ePK // predicate kind
  {
  /* 0 */ PK_NONE=0,
  /* 1 */ PK_COLUMN_OP_COLUMN,
  /* 2 */ PK_COLUMN_OP_LITERAL,
  /* 3 */ PK_LITERAL_OP_LITERAL,
  /* 4 */ PK_LITERAL_OP_COLUMN,
  /* 5 */ PK_ENUMS
  };
  
  static Str* aPK[]
  {
  /* 0 */ "none"_Str,
  /* 1 */ "column op column"_Str,
  /* 2 */ "column op literal"_Str,
  /* 3 */ "literal op literal"_Str,
  /* 4 */ "literal op column"_Str,
  /* 5 */ "enums"_Str,
  };
  
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
