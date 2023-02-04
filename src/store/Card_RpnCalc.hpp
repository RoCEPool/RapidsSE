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
    void vBump_Rightwards(i8 nBumpBy);
    void vConvert_sqyOp_not_To_notSqyOp(void);
    Card_Term* pGetSingle_stmColOdn(void);
    Card_Term* pGetTerm(i8 i_Term);
    bool bIsBraced(void);
    bool bHasWhen(void);
    Card_BoolCalc*   pToBoolCalc(void);
  };

  Card_RpnCalc* _pCard_RpnCalc(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
