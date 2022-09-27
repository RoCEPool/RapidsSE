/*============================================================================*/
/* Card_CalcBool.hpp -- Boolean Calculation                                   */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_EXPRESSION.hpp"

namespace STORE
{
  class Card_CalcBool : public Card_EXPRESSION
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CalcBool(void);
   ~Card_CalcBool(void) noexcept(false);

    void vCullPred(char cWithMark);
    void vRemovePred(Card_PRED* pCard_pred);

  private:
    bool bCullPred(Card_PRED* pCard_pred, char cWithMark);
  };

  Card_CalcBool* _pCard_CalcBool(void* aAddr);
  bool _bIsCard_CalcBool(void* aAddr);
  Card_CalcBool* _pGetSub_Card_CalcBool(Card* pParent);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
