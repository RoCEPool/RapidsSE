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

    void vBumpKeyCol(i8 nBy=1);
    void vBump_expCol(i8 nBy= 1);
    Card_Term* pClone(void);
    Card_Term* pClone(Symbol* pDiffSymbol);
    void vEnsure_Cast(CardIter* pCI, Cast& rxCast_last);
    void vCorrect_expCol(Card_Emit* p_arriving);
    void vDeleteFacetIf(eSF eFacet, eSF eAlsoFacet, char cTblSyn);
    ePB  eEvalBound(void);
    eRO  eEvalRelOp(void);
    eTE  eEvalTerm(void);
    bool bIsColSym(void);   // $n or $c.n
    void vPrint(Ribbon* pRibbon, i8 iDepth);

    void vShortForm(String* pString, Stz* szSep);

    void vX_expCol_To_tblCol(CardIter* pCI);
    void vX_tblCol_To_expCol(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Term* _pCard_Term(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
