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
    Card_Term(Symbol* pSymbol);
   ~Card_Term(void) noexcept(false);

    void vBumpKeyOdn(i8 nBy=1);
    void vBump_stmOdn(i8 nBy= 1);
    Card_Term* pClone(void);
    Card_Term* pClone(Symbol* pDiffSymbol);
    void vConvertIf_stmOdn_To_colOdn(CardIter* pCI);
    void vEnsureTypeFor_stmOdn(CardIter* pCI);
    void vCorrectStmOdn(Card_Projection* p_projection_prior);
    ePB  eEvalBound(void);
    eRO  eEvalRelOp(void);
    eTE  eEvalTerm(void);
    i8   iGetOdn(void);
    char cGetTblSyn(void);
    bool bIsColOdn(void);   // $c.n
    bool bIsKeyOdn(void);   // $Kn
    bool bIsPrmOdn(void);   // %n
    bool bIsStmOdn(void);   // $n
    bool bIsValOdn(void);   // $n or $c.n
    void vPrint(Ribbon* pRibbon, i8 iDepth);
    bool bSameAs(Card_Term* p_term_arg);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Term* _pCard_Term(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
