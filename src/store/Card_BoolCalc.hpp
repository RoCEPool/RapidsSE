/*============================================================================*/
/* Card_BoolCalc.hpp -- Boolean Calculation                                   */
/*============================================================================*/
#pragma once

#include "Card_PRED.hpp"

namespace STORE
{
  class Card_BoolCalc : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_BoolCalc(void);
   ~Card_BoolCalc(void) noexcept(false);

    void vCullPred(char cWithMark);
    void vRemovePred(Card_PRED* p_pred);

  private:
    bool bCullPred(Card_PRED* p_pred, char cWithMark);
  };

  Card_BoolCalc* _pCard_BoolCalc(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
