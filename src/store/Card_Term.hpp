/*============================================================================*/
/* Card_Term.hpp -- Term                                                      */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

#include "Card_Expr.hpp"

#include "eTE.h"

namespace STORE
{
  class Card_Term : public Card_symbol<Card_Term>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Term(void);
   ~Card_Term(void) noexcept(false);

    static void vItemise_colNm(CardIter* pCI);
    static void vItemStrip_colNm(CardIter* pCI);

    void vBumpKeyOdn(i8 nBy=1);
    void vBump_stmOdn(i8 nBy= 1);
    Card_Term* pClone(void);
    Card_Term* pClone(Symbol* pDiffSymbol);
    void vConvert_colOdn_To_stmOdn(CardIter* pCI);
    void vConvert_colOdn_To_stmOdn_X(CardIter* pCI);
    void vConvert_stmOdn_To_colOdn(CardIter* pCI);
    void vEnsureTypeFor_stmOdn(CardIter* pCI);
    void vCorrectStmOdn(Card_Emitted* p_projection_prior);
    ePB  eEvalBound(void);
    eRO  eEvalRelOp(void);
    eTE  eEvalTerm(void);
    i8   iGetOdn(void);
    char cGetTblSyn(void);
    bool bIsColNm(void);    // name
    bool bIsColOdn(void);   // $c.n
    bool bIsKeyOdn(void);   // $Kn
    bool bIsPrmOdn(void);   // %n
    bool bIsStmOdn(void);   // $n
    bool bIsValOdn(void);   // $n or $c.n
    void vPrint(Ribbon* pRibbon, i8 iDepth);

    void vShortForm(String* pString, Stz* szSep);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Term* _pCard_Term(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
