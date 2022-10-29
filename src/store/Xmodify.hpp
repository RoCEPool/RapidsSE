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
    i8   m_iOrdLo;
    i8   m_iOrdHi;
    char m_cTableSyn;

    // must be last
    Footer m_xFooter;

  public:
    Xmodify(eCN eClass);
   ~Xmodify(void) noexcept(false);

  public:
    void M_vBoolCalc_CullPred(Card_BoolCalc* p_boolCalc, char cWithMark);
    bool M_bBoolCalc_CullPred(Card_BoolCalc* p_boolCalc, Card_PRED* p_pred, char cWithMark);
    void M_vBoolCalc_RemovePred(Card_BoolCalc* p_boolCalc, Card_PRED* p_pred);

    void M_vCmpPred_MarkBound(Card_CmpPred* p_cmpPred);

    void M_vFilter_CullPred(Card_Filter* p_filter, char cWithMark);

    void M_vIdxColList_ToOrdinals(Card_IdxColList* p_idxColList, Card_Stream* pCard_Stream);

    void M_vScan_Add_cmpPred_To_bounds(Card_Scan* p_scan, Card_CmpPred* p_cmpPred);
    void M_vScan_Add_indexList(Card_Scan* p_scan);

    void M_vStream_OrdinalRange(Card_Stream* p_stream);

    void M_vTable_Bounds(Card_Table* p_table, Card_Filter* p_filter);
    void M_vTable_IndexJoin(Card_Table* p_table);
    void M_vTable_syn(Card_Table* p_table);

    void M_vTerm_MarkIfMatchesAn_idxCol(Card_Term* p_term, Card_IdxColList* p_idxColList);

    // predicate pull-up
    void vPullUp_MarkPredLeftwards(Card_BoolCalc* p_bool, Card_Deck* p_deck);
    void vPullUp_MarkPredDrillDown(Card_PRED* p_pred, Card_Deck* p_deck);

    char cPullup_FindTableForPred(Card_PRED* p_pred, Card_Deck* p_deck);
    char cPullUp_TableContainsPred(Card_Table* p_table, Card_PRED* p_pred);
    bool bPullUp_StreamContainsPred(Card_Stream* p_stream, Card_PRED* pCard_PRED);

    void vPullUp_MovePredLeftwards(Card_BoolCalc* p_calcBool, Card_Deck* p_deck);
    bool bPullUp_MovePredDrillDown(Card_PRED *pCard_pred, Card_BoolCalc* p_calcBool, Card_Deck* p_deck);
    void vPullUp_MovePredToExistingFilter(Card_Filter* p_filter_existing, Card_PRED* pCard_pred);
    void vPullUp_MovePredToNewFilter(Card_Filter* p_filter_new, Card_PRED* p_pred);
    void vPullUp_MovePredToFilter(Card_Filter* p_filter, Card_PRED* p_pred);

    void vStreamAnalysisReset(void);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
