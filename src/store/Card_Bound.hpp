/*============================================================================*/
/* Card_Bound.hpp -- Abstract Bound                                           */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Bound : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Bound(void);
    Card_Bound(Stz* szRelnOp);
   ~Card_Bound(void) noexcept(false);

    static Card_Bound*    pCreateFrom_PRED(Card_PRED* p_PRED,
                                           char cTblSyn_boundedTo);
    static Card_Bound*    pBound_New(Stz* szRelnOp);

    static bool bValidBoundForRelnOp(Symbol* pBound, Symbol* pRelnOp);

    void vEnsureTypes_For_stmOdn(CardIter *pCI);
    bool bEquivalent(Card_CmpPred* p_cmpPred);

    Card_Term* pGetTerm(i8 i_Term);
    Card_Term* pGetTerm_last(void);

    bool bUsesSqyRsl(void);

    bool bIsEBound(void);
    bool bIsLBound(void);
    bool bIsUBound(void);

    Card_Term* pGet_idxColOdn(void);
    Stz* szGet_idxColOdn(void);

    Card_Term* pGet_colOdn(void);
    Card_Term* pGet_tblColOdn(void);
    Stz* szGet_tblColOdn(void);

  };

  Card_Bound* _pCard_Bound(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
