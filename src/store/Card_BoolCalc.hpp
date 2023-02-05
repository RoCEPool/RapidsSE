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

    void vBump_Rightwards(i8 nBumpBy);
    void vConvert_stmColOdn_terms_To_tblColOdn(CardIter* pCI);
    void vCopyMarkedPredToTable(Card_Deck* p_deck);
    void vCullPred(char cWithMark);
    bool bCullPred(Card_PRED* p_pred, char cWithMark);
    void vMarkWrt_table(Card_Deck* p_deck);
    void vPush_Correlated_cmpPred_Onto_mq_boolCalc(Card_PRED* p_PRED);
    void vPushPred(Card_PRED* p_pred_toBeAdded);
    void vRemovePred(Card_PRED* p_pred);
    void vSubstitute_Parameters(Card_ParamList* p_paramList);
    bool bYank_PRED(char cWithMark, Card_PRED*& p_pred_yanked);
    bool bYank_PRED(char cWithMark, Card* p_test, Card_PRED*& p_pred_yanked);


    bool bIsBraced(void);
  };

  Card_BoolCalc* _pCard_BoolCalc(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
