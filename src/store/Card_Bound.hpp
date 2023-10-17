/*============================================================================*/
/* Card_Bound.hpp -- Abstract Bound                                           */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_Bound : public Card_symbol<Card_Bound>
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Bound(void);
   ~Card_Bound(void) noexcept(false);

    static Card_Bound*    pCreateFrom_CONJ(Card_CONJUNCTION* p_CONJ,
                                           char cTblSyn_boundedTo);
    static Card_Bound*    pBound_New(Stz* szRelnOp);

    static bool bValidBoundForRelnOp(Symbol* pBound, Symbol* pRelnOp);

    void vX_tblCol_To_stmCol(CardIter* pCI);

    void vEnsureTypes_For_stmCol(CardIter *pCI);
    bool bEquivalent(Card_CmpConj* p_cmpPred);

    Card_Term* pGetTerm(i8 i_Term);
    Card_Term* pGetTerm_last(void);

    bool bUsesSqyRsl(void);

    bool bIsEBound(void);
    bool bIsLBound(void);
    bool bIsUBound(void);

    Card_Term* pGet_idxCol(void);

    Card_Term* pGet_IfNot_idxCol(void);
    Card_Term* pGet_tblCol(void);

    void vShortForm(String* pString_b, Stz* szSep);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Bound* _pCard_Bound(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
