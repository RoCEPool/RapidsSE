/*============================================================================*/
/* Xseries.cpp --                                                           */
/*============================================================================*/

#include "Xseries.hpp"
#include "Xtransforms.hpp"
#include "Optimiser.hpp"
#include "Job.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle --------------------------Ï-------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Xseries,CN_XSERIES)
__aNewFunction(Xseries,CN_XSERIES)
__vDeleteFunction(Xseries,CN_XSERIES)
__aFooterFunction(Xseries)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Xseries::Xseries(Bench* pBench)
: OptimiserTool(CN_XSERIES)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XSERIES,_FOOTER_);

  m_pBench       = pBench;
  m_pOptimiser   = m_pBench->pOptimiser();
  m_pBoard       = nullptr;

  m_pCardCtl     = m_pOptimiser->pCardCtl();
  m_pJobTags     = MY::pJob()->pJobTags();
  m_bInteractive = m_pJobTags->bHasName("<interactive>"_Stz);

  m_pTransforms  = new Xtransforms(m_pBench);

  _checkInitialised_(CN_XSERIES);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_XSERIES,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Xseries::~Xseries(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XSERIES,_FOOTER_);

  delete m_pTransforms;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* transform 3                                                                */
/*----------------------------------------------------------------------------*/
void Xseries::vTransformSeries(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XSERIES,_FOOTER_);

  //m_pCardCtl->vReset();
  //m_pCardCtl->vPrintSubCards(false,CK_AGGELIST,CK_GBYELIST,CK_PROJELIST,0);
  //m_pCardCtl->vPrintSubCards(CK_AGGELIST, true);
  //m_pCardCtl->vPrintSubCards(CK_GBYELIST, true);

  m_pCardCtl->vPrintSubCards(false,CK_AGGELIST,CK_GBYELIST,CK_PROJELIST,
                                   CK_RPNCALC,CK_STREAM,0);
  m_pCardCtl->vSaveShorter();
  m_pCardCtl->vSaveXhand();

  vSeries_10_Condense_Info      (C_ENABLED);

  vSeries_20_Table_Analysis     (C_ENABLED);
  vSeries_22_Stream_Analysis(C_ENABLED);
  vSeries_27_Query_Analysis     (C_ENABLED);
  vSeries_29_To_BoolCalc        (C_ENABLED);

  vSeries_40_Predicate_Pullup   (C_ENABLED);

  vSeries_35_Flatten_From       (C_ENABLED);
  vSeries_36_Flatten_Where      (C_ENABLED);
  vSeries_37_Flatten_Having     (C_ENABLED);

  vSeries_50_Join_Ordering                   (C_DISABLED);

  vSeries_60_Index_Join         (C_ENABLED);
  vSeries_85_Cross_Join                      (C_DISABLED);

  vSeries_75_EE_operators       (C_ENABLED);

  vSeries_80_Index_Scan                      (C_DISABLED);

  vSeries_90_Stream_Ordering    (C_ENABLED);
  vSeries_92_Ordered_Aggreg                  (C_DISABLED);

  vSeries_A0_Filter_Refinement  (C_ENABLED);

  vSeries_Z0_Restate_Pipeline   (C_ENABLED);
}

/*----------------------------------------------------------------------------*/
/* apply the reducePipeline set of transformations to all subPipe boards      */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries_10_Condense_Info(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  if (! m_pTransforms->bS10_T00_Test(bEnabled)) {
    m_pTransforms->vS10_T00_Skip();
    return;
  }

  m_pTransforms->vS10_T00_Begin();

  for (i8 i_Board = 0;
       m_pBench->bBoardExists(BK_SUBPIPE,i_Board);
       i_Board++) {

    m_pTransforms->vUsingBoard(BK_SUBPIPE, i_Board);

    m_pTransforms->vS10_T10_Delete_All_absJoin();
    m_pTransforms->vS10_T20_Delete_All_combined();
    m_pTransforms->vS10_T30_Delete_All_send();
    m_pTransforms->vS10_T40_Delete_All_stage();
    m_pTransforms->vS10_T50_Cull_scan_Leaving_table();
    m_pTransforms->vS10_T60_Add_All_level();

    //m_pTransforms->vS10_T80_Delete_Redundant_stages();
    //m_pTransforms->vS10_T90_Hide_Ignorable_stages();
  }

  m_pTransforms->vS10_TZZ_End();

  vEndSeries(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries_20_Table_Analysis(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  if (! m_pTransforms->bS20_T00_Test(bEnabled)) {
    m_pTransforms->vS20_T00_Skip();
    return;
  }

  m_pTransforms->vS20_T00_Begin();

  for (i8 i_Board = 0;
       m_pBench->bBoardExists(BK_SUBPIPE,i_Board);
       i_Board++) {

    m_pTransforms->vUsingBoard(BK_SUBPIPE, i_Board);

    m_pTransforms->vS20_T10_Affix_tableSyn_ToAll_table();
    m_pTransforms->vS20_T20_Produce_tablePtn_UnderAll_table();
  }

  m_pTransforms->vS20_TZZ_End();

  vEndSeries(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries_22_Stream_Analysis(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  if (! m_pTransforms->bS25_T00_Test(bEnabled)) {
    m_pTransforms->vS22_T00_Skip();
    return;
  }

  m_pTransforms->vS22_T00_Begin();

  for (i8 i_Board = 0;
       m_pBench->bBoardExists(BK_SUBPIPE,i_Board);
       i_Board++) {

    m_pTransforms->vUsingBoard(BK_SUBPIPE, i_Board);

    m_pTransforms->vS22_T10_Produce_stream_UnderAll_table();
    m_pTransforms->vS22_T12_Produce_stream_UnderAll_project();
    m_pTransforms->vS22_T14_Produce_stream_UnderAll_aggreg();
    m_pTransforms->vS22_T20_Derive_ordinalRange_ForAll_table_stream();
    m_pTransforms->vS22_T22_Derive_ordinalRange_ForAll_project_stream();
    m_pTransforms->vS22_T24_Derive_ordinalRange_ForAll_aggreg_stream();
  }

  m_pTransforms->vS22_TZZ_End();

  vEndSeries(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries_27_Query_Analysis(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  if (!m_pTransforms->bS27_T00_Test(bEnabled)) {
    m_pTransforms->vS27_T00_Skip();
    return;
  }

  m_pTransforms->vS27_T00_Begin();

  m_pTransforms->vS27_T10_Main_Query_Analysis();
  m_pTransforms->vS27_T20_Sub_Query_Analysis();
  m_pTransforms->vS27_T30();

  //m_pTransforms->vS29_T10_Convert_where_rpnCalc_To_boolCalc();
  //m_pTransforms->vS29_T20_Convert_stmColOdn_To_tblColOdn();

  m_pTransforms->vS27_TZZ_End();

  vEndSeries(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries_29_To_BoolCalc(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  if (!m_pTransforms->bS29_T00_Test(bEnabled)) {
    m_pTransforms->vS29_T00_Skip();
    return;
  }

  m_pTransforms->vS29_T00_Begin();

  for (i8 i_Board = 0;
       m_pBench->bBoardExists(BK_SUBPIPE,i_Board);
       i_Board++) {

    m_pTransforms->vUsingBoard(BK_SUBPIPE, i_Board);

    m_pTransforms->vS29_T10_Convert_where_rpnCalc_To_boolCalc();
    m_pTransforms->vS29_T20_Convert_stmColOdn_To_tblColOdn();
  }

  m_pTransforms->vS29_TZZ_End();

  vEndSeries(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/* from pattern : a/b -> ba                                                   */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries_35_Flatten_From(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  if (! m_pTransforms->bS35_T00_Test(bEnabled)) {
    m_pTransforms->vS35_T00_Skip();
    return;
  }

  m_pTransforms->vS35_T00_Begin();

  m_pTransforms->vS35_T10_Push_sqB_Into_mqA();
  m_pTransforms->vS35_T20_Delete_call();
  m_pTransforms->vS35_T30_Delete_twist();


  //m_pTransforms->vS36_T30_Adjust_From_SubQuery();

  m_pTransforms->vS35_TZZ_End();

  vEndSeries(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/* where pattern : a/b -> b|a                                                 */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries_36_Flatten_Where(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  if (! m_pTransforms->bS36_T00_Test(bEnabled)) {
    m_pTransforms->vS36_T00_Skip();
    return;
  }

  m_pTransforms->vS36_T00_Begin();

  m_pTransforms->vS36_T04_Mark_pmzWhere_In_sqB();
  m_pTransforms->vS36_T05_Pullup_pmzWhere_Into_mqA();

  m_pTransforms->vS36_T10_Push_sqB_Into_mqA();
  m_pTransforms->vS36_T20_Transfer_whereB_To_whereA();
  m_pTransforms->vS36_T30_Replace_param_In_whereA();
  m_pTransforms->vS36_T40_Delete_tstPred_In_whereA();
  m_pTransforms->vS36_T50_Adjust_StmOdn();
  m_pTransforms->vS36_T60_Adjust_StmColOdn();
  m_pTransforms->vS36_T70_Replace_term_In_cmpPred();
  m_pTransforms->vS36_T80_Delete_twist();

  m_pTransforms->vS36_TZZ_End();

  vEndSeries(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/* having pattern : a/b -> a|b                                                */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries_37_Flatten_Having(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  if (! m_pTransforms->bS37_T00_Test(bEnabled)) {
    m_pTransforms->vS37_T00_Skip();
    return;
  }

  m_pTransforms->vS37_T00_Begin();

  m_pTransforms->vS37_T10_Append_sqB_After_mqA();
  m_pTransforms->vS37_T20_Find_stmCol_Link();
  m_pTransforms->vS37_T30_Link_sqCall_In_having();
  m_pTransforms->vS37_T35_Bump_gbyElist_rpnCalc_In_sqB();
  m_pTransforms->vS37_T40_Add_sqCol_To_aggGbyElist();
  m_pTransforms->vS37_T45_Bump_aggElist_rpnCalc_In_sqB();
  m_pTransforms->vS37_T50_Add_sqCol_To_aggAggElist();
  m_pTransforms->vS37_T55_Bump_having_rpnCalc_In_sqB();
  m_pTransforms->vS37_T60_Unproject_sqCol();
  m_pTransforms->vS37_T70_Exhange_sqB_With_mqA();
  m_pTransforms->vS37_T80_Adjust_StmOdn();
  m_pTransforms->vS37_T90_Adjust_StmColOdn();
  m_pTransforms->vS37_TA0_Delete_twist();

  m_pTransforms->vS37_TZZ_End();

  vEndSeries(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries_40_Predicate_Pullup(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  if (! m_pTransforms->bS40_T00_Test(bEnabled)) {
    m_pTransforms->vS40_T00_Skip();
    return;
  }

  m_pTransforms->vS40_T00_Begin();

  for (i8 i_Board = 0;
       m_pBench->bBoardExists(BK_SUBPIPE,i_Board);
       i_Board++) {

    m_pTransforms->vUsingBoard(BK_SUBPIPE, i_Board);

    m_pTransforms->vS40_T10_AtMark_where_pred_Wrt_table();
    m_pTransforms->vS40_T20_Move_AtMarked_where_pred_To_table_filter();
    m_pTransforms->vS40_T30_Delete_Empty_where();
  }

  m_pTransforms->vS40_TZZ_End();

  vEndSeries(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries_50_Join_Ordering(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  if (! m_pTransforms->bS50_T00_Test(bEnabled)) {
    m_pTransforms->vS50_T00_Skip();
    return;
  }

  m_pTransforms->vS50_T00_Begin();


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // PARTITION BOARD

  //
  m_pTransforms->vS50_T00_Extract_tableList_From_queryBoard();

  // add <term/<columnRef> to all ordinal <term>
  //m_pTransforms->vAdd_columnRef_Under_term(bPrintXform,bTrace);

  m_pTransforms->vS50_T10_Extract_joinPredList();
  m_pTransforms->vS50_T20_Compute_coPtnPredList();
  m_pTransforms->vS50_T30_Compute_coPtnJoinList();
  m_pTransforms->vS50_T40_Compute_crossPtnJoinList();
  m_pTransforms->vS50_T50_Compute_ptndJoinSet();

  m_pTransforms->vS50_T60_Copy_ptndJoinSet();
  m_pTransforms->vS50_T70_Generate_ptndPermutations();
  m_pTransforms->vS50_T80_Remove_invalidPermutations();

  m_pTransforms->vS50_T90_Setup_costEst();
  m_pTransforms->vS50_T92_Estimate_ptndJoins();
  m_pTransforms->vS50_T94_Estimate_network();
  m_pTransforms->vS50_T96_Estimate_joinOrder();
  m_pTransforms->vS50_T98_Extract_joinOrderSeq();


  m_pTransforms->vS50_TZZ_End();

  vEndSeries(BK_SUBPIPE, 0);
}
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries_60_Index_Join(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  if (! m_pTransforms->bS60_T00_Test(bEnabled)) {
    m_pTransforms->vS60_T00_Skip();
    return;
  }

  m_pTransforms->vS60_T00_Begin();

  //m_pTransforms->vS60_T05_Twist_in_Becomes_filter();
  m_pTransforms->vS60_T10_Produce_maybeIndexJoin_Under_filter();
  m_pTransforms->vS60_T20_Affix_tblColOdn_ToAll_idxCol();
  m_pTransforms->vS60_T30_UpCopy_filter_pred_Into_bounds();
  m_pTransforms->vS60_T40_Cull_UpCopied_filter_pred();
  m_pTransforms->vS60_T50_Delete_Empty_filter();
  //m_pTransforms->vS60_T60_Delete_Twist();

  m_pTransforms->vS60_TZZ_End();

  vEndSeries(BK_SUBPIPE, 0);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries_75_EE_operators(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  if (! m_pTransforms->bS75_T00_Test(bEnabled)) {
    m_pTransforms->vS75_T00_Skip();
    return;
  }

  m_pTransforms->vS75_T00_Begin();

  m_pTransforms->vS75_T10_Morph_table_To_EEop();
  m_pTransforms->vS75_T20_Morph_keyExists_filter_To_constraint();
  m_pTransforms->vS75_T40_Convert_stmColOdn_Of_constraint_To_stmColNm();

  m_pTransforms->vS75_TZZ_End();

  vEndSeries(BK_SUBPIPE, 0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries_80_Index_Scan(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  /*
 *
 *    <scan>       <scan>
 *      <table>    <table>
 *      <stream>   <indexList>
 *      <filter>   <stream>
 *                 <filter>
 *
 */

  if (! m_pTransforms->bS80_T00_Test(bEnabled)) {
    m_pTransforms->vS80_T00_Skip();
    return;
  }

  m_pTransforms->vS80_T00_Begin();

  m_pTransforms->vS80_T10_Produce_Scratch_indexList_Under_scan();
  m_pTransforms->vS80_T20_Affix_idxColOdn_ToAll_idxCol();
  m_pTransforms->vS80_T30_Mark_ULRangeOf_scan_filter_cmpPred();
  m_pTransforms->vS80_T40_Mark_IndexMatchOf_scan_filter_terms();
  m_pTransforms->vS80_T50_Add_scan_bounds_ByCopyingMarked_filter_cmpPred();
  m_pTransforms->vS80_T60_Cull_Marked_filter_cmpPred();
  m_pTransforms->vS80_T70_Morph_scan_To_indexScan();

  m_pTransforms->vS80_TZZ_End();

  vEndSeries(BK_SUBPIPE, 0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries_85_Cross_Join(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  if (! m_pTransforms->bS85_T00_Test(bEnabled)) {
    m_pTransforms->vS85_T00_Skip();
    return;
  }

  m_pTransforms->vS85_T00_Begin();

  m_pTransforms->vS85_T10_Transmute_All_table();
  m_pTransforms->vS85_T20_Add_index_ForAll_crossJoin();
  m_pTransforms->vS85_T30_Add_bounds_ForAll_crossJoin();

  m_pTransforms->vS85_TZZ_End();

  vEndSeries(BK_SUBPIPE, 0);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries_90_Stream_Ordering(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  if (! m_pTransforms->bS90_T00_Test(bEnabled)) {
    m_pTransforms->vS90_T00_Skip();
    return;
  }

  m_pTransforms->vS90_T00_Begin();

  m_pTransforms->vS90_T10_Remove_sort_IfSameAs_gby();

  m_pTransforms->vS90_TZZ_End();

  vEndSeries(BK_SUBPIPE, 0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries_92_Ordered_Aggreg(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  if (! m_pTransforms->bS92_T00_Test(bEnabled)) {
    m_pTransforms->vS92_T00_Skip();
    return;
  }

  m_pTransforms->vS92_T00_Begin();

  m_pTransforms->vS92_T10();
  m_pTransforms->vS92_T20();
  m_pTransforms->vS92_T30();

  m_pTransforms->vS90_TZZ_End();

  vEndSeries(BK_SUBPIPE, 0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries_A0_Filter_Refinement(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  if (! m_pTransforms->bSA0_T00_Test(bEnabled)) {
    m_pTransforms->vSA0_T00_Skip();
    return;
  }

  m_pTransforms->vSA0_T00_Begin();

  m_pTransforms->vSA0_T10_Individuate_filter();

  m_pTransforms->vSA0_TZZ_End();

  vEndSeries(BK_SUBPIPE, 0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries_Z0_Restate_Pipeline(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  if (! m_pTransforms->bSZ0_T00_Test(bEnabled)) {
    m_pTransforms->vSZ0_T00_Skip();
    return;
  }

  m_pTransforms->vSZ0_T00_Begin();

  m_pTransforms->vSZ0_T10_Restore_stmColOdn_from_tblColOdn();
  //m_pTransforms->vSZ0_T20_Reinstate_stages();
  //m_pTransforms->vSZ0_T20_Reinstate_stagesV2();
  //m_pTransforms->vSZ0_T30_Reinstate_combined();
  //m_pTransforms->vSZ0_T40_Reinstate_next();

  m_pTransforms->vSZ0_T20_Reinstate_stagesV3();

  m_pTransforms->vSZ0_TZZ_End();

  vEndSeries(BK_SUBPIPE, 0);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xseries::vEndSeries(eBK eBoard, i8 i_Board)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  RASSERT(0 == m_pTransforms->iTransformDepth());

  //m_pTransforms->vXform(C_NA);
  //m_pCardCtl->vUseXhand();

  m_pTransforms->vPrintBench(eBoard, i_Board);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries(eBK eBoard, i8 i_Board, const char* azHand)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  m_pTransforms->vUsingBoard(eBoard, i_Board);
  m_pTransforms->vXhand(azHand);

  m_pBench->vClearMarks();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
