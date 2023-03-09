/*============================================================================*/
/* Card_Term.hpp -- Term                                                      */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "eTE.h"

namespace STORE
{
  class Card_Term : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Term(void);
    Card_Term(Stz* szTerm);
   ~Card_Term(void) noexcept(false);

    static bool bReplaceValueOfSub(CardIter* pCI, Card* pCard, eCK eSub,
                                   Stz* szValue, Stz* szWith);

    void vBump_KeyOrdinal(i8 nBy=1);
    void vBump_StreamColOrdinal(i8 nBy=1);
    void vConvert_stmColOdn_To_tblColOdn(CardIter* pCI);
    ePB  eEvalBound(void);
    eRO  eEvalRelOp(void);
    i8   iGetOdnNo(void);
    char cGetTableSyn(void);
    bool bIsBoardNm(CardIter* pCI);
    bool bIsFoldExpr(void);
    bool bIsCorPrmOdn(void); // %n
    bool bIsStmColOdn(void); // $n
    bool bIsTblColOdn(void); // $c.n
    void vMarkIfMatchesAn_idxCol(Card_IdxColList* p_idxColList);
    void vReplace_sqCall_With_sqResult(Stz* szStmColOdn);
    eTE  eTermEval(void);
  };

  Card_Term* _pCard_Term(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
