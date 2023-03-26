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

    //Card_Term(Card_Term* p_term_arg);
    Card_Term(Stz* szTerm, Stz* szStem=nullptr);
   ~Card_Term(void) noexcept(false);

    static bool bReplaceValueOfSub(CardIter* pCI, Card* pCard, eCK eSub,
                                   Stz* szValue, Stz* szWith);

    void vBump_KeyOrdinal(i8 nBy=1);
    void vBump_StreamColOrdinal(i8 nBy=1);
    Card_Term* pClone(void);
    Card_Term* pClone(Stz* szDiffValue);
    void vConvert_stmColOdn_To_tblColOdn(CardIter* pCI);
    void vEnsure_tblCol_flip(void);
    ePB  eEvalBound(void);
    eRO  eEvalRelOp(void);
    Card_Expr* pExpr_For_col(CardIter* pCI, i8 i_Before=C_UNKNOWN);
    Card_Expr* pExpr_For_colNm(CardIter* pCI, i8 i_Before=C_UNKNOWN);
    void vFlip(Card_Projection* p_projection_needed);
    i8   iGetOdnNo(void);
    char cGetTableSyn(void);
    bool bIsBoardNm(CardIter* pCI);
    bool bIsColOdn(void);    // $n or $c.n
    bool bIsCorPrmOdn(void); // %n
    bool bIsFoldExpr(void);
    bool bIsKeyOdn(void);    // $Kn
    bool bIsSame(Card_Term* p_term_arg);
    bool bIsStmColOdn(void); // $n
    bool bIsTblColOdn(void); // $c.n
    void vMarkIfMatchesAn_idxCol(Card_IdxColList* p_idxColList);
    void vPrint(Ribbon* pRibbon, i8 iDepth);
    void vReplace_sqCall_With_sqResult(Stz* szStmColOdn);
    eTE  eTermEval(void);
  };

  Card_Term* _pCard_Term(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
