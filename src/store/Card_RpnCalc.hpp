/*============================================================================*/
/* Card_RpnCalc.hpp -- RPN Calculation Sequence                               */
/*============================================================================*/
#pragma once

#include "Card_TERMLIST.hpp"

namespace STORE
{
  // todo: Can remove this [296]
  class Card_RpnCalc : public Card_TERMLIST
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_RpnCalc(void);
    Card_RpnCalc(Symbol* pSymbol);
   ~Card_RpnCalc(void) noexcept(false);

    Card_Term* pGetTerm(i8 i_Term);
    void vPrint(Ribbon* pRibbon, i8 iDepth);
    void vTransfer(Card_RpnCalc* p_rpnCalc, eSF eSymbol_limit);
    void vX_expCol_To_tblCol(CardIter* pCI);
    void vX_tblCol_To_expCol(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_RpnCalc* _pCard_RpnCalc(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
