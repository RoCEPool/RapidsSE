/*============================================================================*/
/* Card_RpnCalc.hpp -- RPN Calculation Sequence                               */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_RpnCalc : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_RpnCalc(void);
   ~Card_RpnCalc(void) noexcept(false);

    void vAppendRpnCalc(Card_RpnCalc* p_rpnCalc);
    void vBump_Rightwards(i8 nBumpBy, bool bSkipFoldExpr=false);
    void vConvert_sqyOp_not_To_notSqyOp(void);
    void vConvert_stmColOdn_terms_To_tblColOdn(CardIter* pCI);
    Card_Term* pGetSingle_stmColOdn(void);
    Card_Term* pGetTerm(i8 i_Term);
    bool bHasWhen(void);
    bool bIsBraced(void);
    bool bIsSame(Card_RpnCalc* p_rpnCalc_arg);
    bool bIsSingleCol(void);
    void vOntoStzList(StzList* pStzList);
    Stz* szProjectedCol(void);
    Card_Term* pProjectedTerm(void);
    void vFlipTerms(Card_Projection* p_projection_needed);
    Card_BoolCalc* pToBoolCalc(CardIter* pCI);
    i8 nTrimTerm(Stz* szValue);
    void vTypes_For_stmColOdn(CardIter* pCI);

  };

  Card_RpnCalc* _pCard_RpnCalc(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
