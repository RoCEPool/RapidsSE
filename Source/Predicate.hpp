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
  /* 2 */ PK_COLUMN_LITERAL_OP,   // this should always be the case
  /* 3 */ PK_LITERAL_LITERAL_OP,
  /* 4 */ PK_LITERAL_COLUMN_OP,
  /* 5 */ PK_ENUMS
  };
  
  static Stz* aPK[]
  {
  /* 0 */ "none"_Stz,
  /* 1 */ "column column op"_Stz,
  /* 2 */ "column literal op"_Stz,
  /* 3 */ "literal literal op"_Stz,
  /* 4 */ "literal column op"_Stz,
  /* 5 */ "enums"_Stz,
  };
  
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
