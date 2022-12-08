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
    i8   m_iOdnLo;
    i8   m_iOdnHi;
    char m_cTableSyn;

    // must be last
    Footer m_xFooter;

  public:
    Xmodify(eCN eClass);
   ~Xmodify(void) noexcept(false);

  public:
    void M_vBoolCalc_CullPred(Card_BoolCalc* p_boolCalc, char cWithMark);
    bool M_bBoolCalc_CullPred(Card_BoolCalc* p_boolCalc, Card_PRED* p_pred, char cWithMark);
    //void M_vBoolCalc_DrillDown(Card_BoolCalc* p_bool, std::function<bool(Card* pCard)> bfApply);
    //void M_vBoolCalc_Drilldown(Card_PRED* p_pred, std::function<bool(Card* pCard)> bfApply);
    void M_vBoolCalc_PushPred(Card_BoolCalc* p_boolCalc, Card_PRED* p_pred_toBeAdded);
    void M_vBoolCalc_RemovePred(Card_BoolCalc* p_boolCalc, Card_PRED* p_pred);
    void M_vBoolCalc_Substitute_Parameters(Card_BoolCalc* p_boolCalc, Card_ParamList* p_paramList);
    bool M_bBoolCalc_Yank_PRED(Card_BoolCalc* p_boolCalc, char cWithMark,
                               Card_PRED*& p_pred_yanked);
    bool M_bBoolCalc_Yank_PRED(Card_BoolCalc* p_boolCalc, char cWithMark,
                               Card* p_test, Card_PRED*& p_pred_yanked);

    void M_vBounds_AddPred(Card_Table* p_table, Card_Index* p_index, Card_Bounds* p_bounds, Card_PRED *p_PRED);
    void M_vBounds_AddPredUnder_index(Card_Table* p_table, Card_Index* p_index, Card_PRED *p_PRED);

    void M_vCard_Mark_Changed(Card* pCard);
    void M_vCard_Set_IndexColOrdinal(Card* p_card, i8 iIdxColOdn);
    void M_vCard_Set_KeyOrdinal(Card* p_card, i8 iKeyOdn);
    void M_vCard_Set_ParamOrdinal(Card* p_card, i8 iParamOdn);
    void M_vCard_Set_StreamColOrdinal(Card* p_card, i8 iStmColOdn);
    void M_vCard_Set_TableColOrdinal(Card* p_card, char cTblSyn, i8 iTblColOdn);
    void M_vCard_Set_TableColSyn(Card* p_card, char cTblSyn);
    void M_vCard_Set_Value(Card* pCard, Stz* szValue);
    void M_vCard_Set_Value(Card* pCard, Stz* szValue, char cMark1);

    Card_Term* E_pCmpPred_Extract_Arg_term(Card_CmpPred* p_cmpPred);
    Card_Term* E_pCmpPred_Extract_Param_term(Card_CmpPred* p_cmpPred);

    void M_vCmpPred_MarkBound(Card_CmpPred* p_cmpPred);

    i8 E_iELIST_rpnCalc_Has_stmOrdOdn(Card_ELIST* p_ELIST, Card_StmOrdOdn* p_stmOrdOdn);
    i8 E_iExpr_rpnCalc_WhichGbyKey(Card_Expr* p_expr);

    void M_vFilter_AddPred(Card_Filter* p_filter, Card_PRED *p_PRED);
    void M_vFilter_AddPredUnder_main(Card* pMainCard, Card_PRED *p_PRED);
    void M_vFilter_CullPred(Card_Filter* p_filter, char cWithMark);
    void M_vFilter_DeleteIfEmpty(Card_Filter* p_filter, Card* pMainCard);

    void M_vIdxColList_Affix_tblColOdn(Card_IdxColList* p_idxColList, Card_Stream* p_stream);

    char M_eLevel_To_char(eAL eLevel);
    eYG  M_eLevel_To_StageTopology(eAL eLevel);

    void M_vRpnCalc_Convert_sqyOp_not_To_notSqyOp(Card_RpnCalc* p_rpnCalc);

    void M_vScan_Add_cmpPred_To_bounds(Card_Scan* p_scan, Card_CmpPred* p_cmpPred);
    void M_vScan_Produce_indexList(Card_Scan* p_scan);

    eAL  M_eStageTopology_To_Level(eYG eStageTopo);

    bool bStream_Contains_Pred(Card_Table* p_table, Card_Stream* p_stream,
                               Card_PRED* pCard_PRED);
    void M_vStream_Derive_OrdinalRange_aggreg(Card_Stream* p_stream);
    void M_vStream_Derive_OrdinalRange_project(Card_Stream* p_stream);
    void M_vStream_Derive_OrdinalRange_table(Card_Stream* p_stream, char cTblSyn);
    void M_vStream_Adjust_OrdinalRange(Card_Stream* p_stream, i8 iLo, i8 iHi);


    char cTable_Contains_Pred(Card_Table* p_table, Card_PRED* p_pred);
    char cTable_FindTableFor_Pred(Card_PRED* p_pred, Card_Deck* p_deck);
    void M_vTable_Produce_MaybeIndexJoin(Card_Table* p_table);
    void M_vTable_Set_Syn(Card_Table* p_table);

    void M_vTerm_Bump_KeyOrdinal(Card_Term* p_term, i8 nBy=1);
    void M_vTerm_Bump_StreamColOrdinal(Card_Term* p_term, i8 nBy=1);
    void M_vTerm_MarkIfMatchesAn_idxCol(Card_Term* p_term, Card_IdxColList* p_idxColList);

    // generalised drill down

    bool bPRED_Descend(Card_PRED* p_pred, std::function<bool(Card_PRED* )> bfLambdaFn);

    // predicate pull-up
    void vPullUp_MarkPredLeftwards(Card_BoolCalc* p_bool, Card_Deck* p_deck);
    void vPullUp_MarkPredDrillDown(Card_PRED* p_pred, Card_Deck* p_deck);

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
