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
    void vBump_stmCol(i8 nBy= 1);
    Card_Term* pClone(void);
    Card_Term* pClone(Symbol* pDiffSymbol);
     void vEnsureTypeFor_stmCol(CardIter* pCI);
    void vCorrectStmCol(Card_Emit* p_emitted_prior);
    void vDeleteFacetIf(eSF eFacet, eSF eAlsoFacet, char cTblSyn);
    ePB  eEvalBound(void);
    eRO  eEvalRelOp(void);
    eTE  eEvalTerm(void);
    i8   iGetOdn(void);
    char cGetTblSyn(void);
    bool bIsColNm(void);    // name
    bool bIsColSym(void);   // $n or $c.n
    bool bIsKeyCol(void);   // $Kn
    bool bIsSqyPrm(void);   // %n
    bool bIsStmCol(void);   // $n
    bool bIsTblCol(void);   // $c.n
    void vPrint(Ribbon* pRibbon, i8 iDepth);

    void vShortForm(String* pString, Stz* szSep);

    void vX_stmCol_To_tblCol(CardIter* pCI);
    void vX_tblCol_To_stmCol(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Term* _pCard_Term(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
