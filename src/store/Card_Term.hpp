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

    void vApplyCast(CardIter* pCI, Card_Emit* p_Dic_expr, Cast& rxCast_last);
    void vBump_ordCol(eSF eFacet, i8 nBy= 1);
    Card_Term* pClone(void);
    Card_Term* pClone(Symbol* pDiffSymbol);
    void vDeleteFacetIf(eSF eFacet, eSF eAlsoFacet, char cTblSyn);
    eTE  eEvalTerm(void);
    void vPrint(Ribbon* pRibbon, i8 iDepth);
    void vShortForm(String* pString, Stz* szSep);
    void vUpdate_expCol(Card_Emit* p_Dic_expr);

    void vX_expCol_To_tblCol(CardIter* pCI);
    void vX_tblCol_To_expCol(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Term* _pCard_Term(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
