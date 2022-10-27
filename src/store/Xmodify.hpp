/*============================================================================*/
/* Xmodify.hpp --                                                       */
/*============================================================================*/
#pragma once

#include "Xcreate.hpp"

namespace STORE
{
  class Table;

  class Xmodify : public Xcreate
  {
  public:

    void T_vPRED_CullByKids(Card_PRED* p_PRED);


    void T_vCmpPred_MarkBound(Card_CmpPred* p_cmpPred);

    bool X_bBoolCalc_CullPred(Card_BoolCalc* p_boolCalc, Card_PRED* p_pred, char cWithMark);
    void X_vBoolCalc_CullPred(Card_BoolCalc* p_boolCalc, char cWithMark);
    void X_vBoolCalc_RemovePred(Card_BoolCalc* p_boolCalc, Card_PRED* p_pred);

    void X_vFilter_CullPred(Card_Filter* p_filter, char cWithMark);

    void X_vIdxColList_Ordinate(Card_IdxColList* p_idxColList, Card_Stream* pCard_Stream);

    void X_vSubCard_ReplaceSub(Card* pSub, Card* pInParent, Card* pWith);


    // predicate pull-up
    void vMarkPredLeftwards(Card_BoolCalc* p_bool, Card_Deck* p_deck);
    void vMarkPredDrillDown(Card_PRED* p_pred, Card_Deck* p_deck);

    char cMovePredLeftwards(Card_PRED* p_pred, Card_Deck* p_deck);
    char cMovePredToTable(Card_Table* p_table, Card_PRED* p_pred);
    bool bStreamContainsPred(Card_Stream* p_stream, Card_PRED* pCard_PRED);
    void vCopyPredLeftwards(Card_BoolCalc* p_calcBool, Card_Deck* p_deck);
    bool bCopyPredDrillDown(Card_PRED*, Card_BoolCalc* p_calcBool, Card_Deck* p_deck);
    void vCopyPredToExistingFilter(Card_Filter* pCard_filter_existing, Card_PRED* pCard_pred);
    void vCopyPredToNewFilter(Card_Filter* p_filter_new, Card_PRED* p_pred);

    void vMarkPredAsMoved(Card* pPredCard);



    // must be last
    Footer m_xFooter;

  public:
    Xmodify(eCN eClass);
   ~Xmodify(void) noexcept(false);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
