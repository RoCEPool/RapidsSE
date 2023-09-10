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

    static Card_Bound*    pCreateFrom_PRED(Card_CONJUNCTION* p_PRED,
                                           char cTblSyn_boundedTo);
    static Card_Bound*    pBound_New(Stz* szRelnOp);

    static bool bValidBoundForRelnOp(Symbol* pBound, Symbol* pRelnOp);

    void vConvert_colOdn_To_stmOdn(CardIter* pCI);

    void vEnsureTypes_For_stmOdn(CardIter *pCI);
    bool bEquivalent(Card_CmpPred* p_cmpPred);

    Card_Term* pGetTerm(i8 i_Term);
    Card_Term* pGetTerm_last(void);

    bool bUsesSqyRsl(void);

    bool bIsEBound(void);
    bool bIsLBound(void);
    bool bIsUBound(void);

    Card_Term* pGet_idxColOdn(void);

    Card_Term* pGet_colOdn(void);
    Card_Term* pGet_tblColOdn(void);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Bound* _pCard_Bound(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
