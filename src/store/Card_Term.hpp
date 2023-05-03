/*============================================================================*/
/* Card_Term.hpp -- Term                                                      */
/*============================================================================*/
#pragma once

#include "Card.hpp"

#include "Card_Expr.hpp"

#include "eTE.h"

namespace STORE
{
  class Card_Term : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Term(Stz* szTerm);
   ~Card_Term(void) noexcept(false);

    void vBump_KeyOrdinal(i8 nBy=1);
    void vBump_StreamColOrdinal(i8 nBy=1);
    Card_Term* pClone(void);
    Card_Term* pClone(Stz* szDiffSymbol);
    void vConvertIf_stmColOdn_To_tblColOdn(CardIter* pCI);
    ePB  eEvalBound(void);
    eRO  eEvalRelOp(void);
    eTE  eEvalTerm(void);
    i8   iGetOdn(void);
    char cGetTableSyn(void);
    bool bIsColOdn(void);    // $n or $c.n
    bool bIsCorPrmOdn(void); // %n
    bool bIsKeyOdn(void);    // $Kn
    bool bIsSame(Card_Term* p_term_arg);
    bool bIsStmColOdn(void); // $n
    bool bIsSymbol(Stz* szSymbol);
    bool bIsTblColOdn(void); // $c.n
    void vPrint(Ribbon* pRibbon, i8 iDepth);
  };

  Card_Term* _pCard_Term(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
