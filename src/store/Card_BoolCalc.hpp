/*============================================================================*/
/* Card_BoolCalc.hpp -- Boolean Calculation                                   */
/*============================================================================*/
#pragma once

#include "Card_PRED.hpp"
#include "Card_ParamList.hpp"

namespace STORE
{
  class Card_BoolCalc : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_BoolCalc(void);
   ~Card_BoolCalc(void) noexcept(false);

    void vBoolCalc_CullPred(char cWithMark);
    bool bBoolCalc_CullPred(Card_PRED* p_pred, char cWithMark);
    void vBoolCalc_MarkWrt_table(Card_Deck* p_deck);
    void vBoolCalc_PushPred(Card_PRED* p_pred_toBeAdded);
    void vBoolCalc_RemovePred(Card_PRED* p_pred);
    void vBoolCalc_Substitute_Parameters(Card_ParamList* p_paramList);
    bool bBoolCalc_Yank_PRED(char cWithMark, Card_PRED*& p_pred_yanked);
    bool bBoolCalc_Yank_PRED(char cWithMark, Card* p_test, Card_PRED*& p_pred_yanked);


    bool bIsBraced(void);
  };

  Card_BoolCalc* _pCard_BoolCalc(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
