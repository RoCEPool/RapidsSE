/*============================================================================*/
/* Transformer.cpp --                                                           */
/*============================================================================*/

#include "Transformer.hpp"
#include "Xtransforms.hpp"
#include "Optimiser.hpp"
#include "Job.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle --------------------------Ï-------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Transformer,CN_TRANSFORMER)
__aNewFunction(Transformer,CN_TRANSFORMER)
__vDeleteFunction(Transformer,CN_TRANSFORMER)
__aFooterFunction(Transformer)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Transformer::Transformer(Bench* pBench)
: OptimiserTool(CN_TRANSFORMER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMER,_FOOTER_);

  m_pBench       = pBench;
  m_pOptimiser   = m_pBench->pOptimiser();
  m_pBoard       = nullptr;

  m_pCardCtl     = m_pOptimiser->pCardCtl();
  m_pJobTags     = MY::pJob()->pJobTags();
  m_bInteractive = m_pJobTags->bHasName("<interactive>"_Stz);

  m_pTransforms  = new Xtransforms(m_pBench);

  _checkInitialised_(CN_TRANSFORMER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Transformer::~Transformer(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMER,_FOOTER_);

  delete m_pTransforms;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* transform                                                                  */
/*----------------------------------------------------------------------------*/
void Transformer::vTransform(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMER,_FOOTER_);

  if (m_pJobTags->bHasName("<transform3>"_Stz))
    vTransform3();
  else
    vTransform3();
}

/*----------------------------------------------------------------------------*/
/* transforms                                                                 */
/*----------------------------------------------------------------------------*/
Xtransforms* Transformer::pTransforms(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMER,_FOOTER_);

  return m_pTransforms;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* transform 3                                                                */
/*----------------------------------------------------------------------------*/
void Transformer::vTransform3(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMER,_FOOTER_);

  //m_pCardCtl->vReset();
  //m_pCardCtl->vPrintSubCards(false,CK_AGGELIST,CK_GBYELIST,CK_PROJELIST,0);
  //m_pCardCtl->vPrintSubCards(CK_AGGELIST, true);
  //m_pCardCtl->vPrintSubCards(CK_GBYELIST, true);

  m_pCardCtl->vPrintSubCards(false,CK_AGGELIST,CK_GBYELIST,CK_PROJELIST,
                                   CK_RPNCALC,CK_STREAM,0);
  m_pCardCtl->vSaveShorter();
  m_pCardCtl->vSaveXhand();

  vHand_10_Condense_Info(CK_NONE);
  vHand_20_Table_Analysis(CK_NONE);
  vHand_30_Stream_Analysis(CK_NONE);
  vHand_35_Flatten_From(CK_NONE);
  vHand_36_Flatten_Where(CK_NONE);
  vHand_37_Flatten_Having(CK_NONE);
  vHand_40_Predicate_Pullup(CK_NONE);
  vHand_50_FlattenCall     (CK_NONE);
  vHand_52_FlattenIn       (CK_NONE);
  vHand_55_FlattenExists   (CK_NONE);
  vHand_60_Index_Join(CK_NONE);
  vHand_70_Join_Ordering(CK_NONE);
  vHand_75_EE_operators    (CK_NONE);
  vHand_80_Index_Scan(CK_NONE);
  vHand_90_Stream_Ordering(CK_NONE);
  vHand_A0_Filter_Refinement(CK_NONE);
  vHand_Z0_Restate_Pipeline(CK_NONE);
}

#define CARD_BELOW \
  va_list xVarArgs; \
  va_start(xVarArgs,iBelow); \
  m_pCardCtl->vPrintBelow(iBelow,xVarArgs); \
  va_end(xVarArgs);\
EndDefine \

/*----------------------------------------------------------------------------*/
/* apply the reducePipeline set of transformations to all subPipe boards      */
/*----------------------------------------------------------------------------*/
void Transformer::vHand_10_Condense_Info(i8 iBelow, ...)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vHand(BK_SUBPIPE, 0, __func__);

  CARD_BELOW

  if (! m_pTransforms->bH10_T00_Test()) {
    m_pTransforms->vH10_T00_Skip();
    return;
  }

  m_pTransforms->vH10_T00_Begin();

  for (i8 i_Board = 0;
       m_pBench->bBoardExists(BK_SUBPIPE,i_Board);
       i_Board++) {

    m_pTransforms->vUsingBoard(BK_SUBPIPE, i_Board);

    m_pTransforms->vH10_T10_Delete_All_absJoin();
    m_pTransforms->vH10_T20_Delete_All_combined();
    m_pTransforms->vH10_T30_Delete_All_send();
    m_pTransforms->vH10_T40_Delete_All_stage();
    m_pTransforms->vH10_T50_Cull_scan_Leaving_table();
    m_pTransforms->vH10_T60_Add_All_level();

    //m_pTransforms->vH10_T80_Delete_Redundant_stages();
    //m_pTransforms->vH10_T90_Hide_Ignorable_stages();
  }

  m_pTransforms->vH10_T99_End();

  vEndHand(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vHand_20_Table_Analysis(i8 iBelow, ...)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vHand(BK_SUBPIPE, 0, __func__);

  CARD_BELOW

  if (! m_pTransforms->bH20_T00_Test()) {
    m_pTransforms->vH20_T00_Skip();
    return;
  }

  m_pTransforms->vH20_T00_Begin();

  for (i8 i_Board = 0;
       m_pBench->bBoardExists(BK_SUBPIPE,i_Board);
       i_Board++) {

    m_pTransforms->vUsingBoard(BK_SUBPIPE, i_Board);

    m_pTransforms->vH20_T10_Affix_tableSyn_ToAll_table();
    m_pTransforms->vH20_T20_Produce_tablePtn_UnderAll_table();
  }

  m_pTransforms->vH20_T99_End();

  vEndHand(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vHand_30_Stream_Analysis(i8 iBelow, ...)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vHand(BK_SUBPIPE, 0, __func__);

  CARD_BELOW

  if (! m_pTransforms->bH30_T00_Test()) {
    m_pTransforms->vH30_T00_Skip();
    return;
  }

  m_pTransforms->vH30_T00_Begin();

  for (i8 i_Board = 0;
       m_pBench->bBoardExists(BK_SUBPIPE,i_Board);
       i_Board++) {

    m_pTransforms->vUsingBoard(BK_SUBPIPE, i_Board);

    m_pTransforms->vH30_T10_Produce_stream_UnderAll_table();
    m_pTransforms->vH30_T12_Produce_stream_UnderAll_project();
    m_pTransforms->vH30_T14_Produce_stream_UnderAll_aggreg();
    m_pTransforms->vH30_T20_Derive_ordinalRange_ForAll_table_stream();
    m_pTransforms->vH30_T22_Derive_ordinalRange_ForAll_project_stream();
    m_pTransforms->vH30_T24_Derive_ordinalRange_ForAll_aggreg_stream();
    m_pTransforms->vH30_T30_Convert_where_rpnCalc_To_boolCalc();
    m_pTransforms->vH30_T40_Convert_stmColOdn_To_tblColOdn();
  }

  m_pTransforms->vH30_T99_End();

  vEndHand(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/* from pattern : a/b -> ba                                                   */
/*----------------------------------------------------------------------------*/
void Transformer::vHand_35_Flatten_From(i8 iBelow, ...)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vHand(BK_SUBPIPE, 0, __func__);

  CARD_BELOW

  if (! m_pTransforms->bH35_T00_Test()) {
    m_pTransforms->vH35_T00_Skip();
    return;
  }

  m_pTransforms->vH35_T00_Begin();

  m_pTransforms->vH36_T10_Push_sqB_Into_mqA();

  //m_pTransforms->vH36_T30_Adjust_From_SubQuery();

  m_pTransforms->vH35_T99_End();

  vEndHand(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/* where pattern : a/b -> b|a                                                 */
/*----------------------------------------------------------------------------*/
void Transformer::vHand_36_Flatten_Where(i8 iBelow, ...)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vHand(BK_SUBPIPE, 0, __func__);

  CARD_BELOW

  if (! m_pTransforms->bH36_T00_Test()) {
    m_pTransforms->vH36_T00_Skip();
    return;
  }

  m_pTransforms->vH36_T00_Begin();

  m_pTransforms->vH36_T10_Push_sqB_Into_mqA();
  m_pTransforms->vH36_T20_Transfer_whereB_To_whereA();
  m_pTransforms->vH36_T30_Replace_param_In_whereA();
  m_pTransforms->vH36_T40_Delete_tstPred_In_whereA();
  m_pTransforms->vH36_T50_Adjust_StmOdn();

  m_pTransforms->vH36_T99_End();

  vEndHand(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/* having pattern : a/b -> a|b                                                */
/*----------------------------------------------------------------------------*/
void Transformer::vHand_37_Flatten_Having(i8 iBelow, ...)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vHand(BK_SUBPIPE, 0, __func__);

  CARD_BELOW

  if (! m_pTransforms->bH37_T00_Test()) {
    m_pTransforms->vH37_T00_Skip();
    return;
  }

  m_pTransforms->vH37_T00_Begin();

  m_pTransforms->vH37_T10();
  m_pTransforms->vH37_T20();
  m_pTransforms->vH37_T30();

  m_pTransforms->vH37_T99_End();

  vEndHand(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vHand_40_Predicate_Pullup(i8 iBelow, ...)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vHand(BK_SUBPIPE, 0, __func__);

  CARD_BELOW

  if (! m_pTransforms->bH40_T00_Test()) {
    m_pTransforms->vH40_T00_Skip();
    return;
  }

  m_pTransforms->vH40_T00_Begin();

  for (i8 i_Board = 0;
       m_pBench->bBoardExists(BK_SUBPIPE,i_Board);
       i_Board++) {

    m_pTransforms->vUsingBoard(BK_SUBPIPE, i_Board);

    m_pTransforms->vH40_T10_AtMark_where_pred_Wrt_table();
    m_pTransforms->vH40_T20_Move_AtMarked_where_pred_To_table_filter();
    m_pTransforms->vH40_T30_Delete_Empty_where();
  }

  m_pTransforms->vH40_T99_End();

  vEndHand(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vHand_50_FlattenCall(i8 iBelow, ...)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vHand(BK_SUBPIPE, 0, __func__);

  CARD_BELOW

  if (! m_pTransforms->bH50_T00_Test()) {
    m_pTransforms->vH50_T00_Skip();
    return;
  }

  m_pTransforms->vH50_T00_Begin();

  m_pTransforms->vH50_T10_Extract_subSet_Of_call();
  m_pTransforms->vH50_T20_Copy_Called_Subset();
  m_pTransforms->vH50_T30();

  m_pTransforms->vH50_T99_End();

  vEndHand(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vHand_52_FlattenIn(i8 iBelow, ...)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vHand(BK_SUBPIPE, 0, __func__);

  CARD_BELOW

  if (! m_pTransforms->bH52_T00_Test()) {
    m_pTransforms->vH52_T00_Skip();
    return;
  }

  m_pTransforms->vH52_T00_Begin();

  m_pTransforms->vH52_T10_Collect_In_SubQuery_As_SubSet();
  m_pTransforms->vH52_T20_CopyUp_In_Subset_nc();
  m_pTransforms->vH52_T30_Cull_ExeMarked_exePred();
  m_pTransforms->vH52_T40_Copy_TwistMarked_tstPred();
  m_pTransforms->vH52_T50_Cull_TwistMarked_tstPred();
  m_pTransforms->vH52_T60_Delete_Empty_where();
  m_pTransforms->vH52_T70_Adjust_StmOdn_PostTwist();

  m_pTransforms->vH52_T99_End();

  vEndHand(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vHand_55_FlattenExists(i8 iBelow, ...)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vHand(BK_SUBPIPE, 0, __func__);

  CARD_BELOW

  if (! m_pTransforms->bH55_T00_Test()) {
    m_pTransforms->vH55_T00_Skip();
    return;
  }

  m_pTransforms->vH55_T00_Begin();

  m_pTransforms->vH55_T10_Collapse_Exists_SubQueries();
  m_pTransforms->vH55_T20_Copy_Up_ExistsSubQueries();
  m_pTransforms->vH55_T30_Adjust_Stream_OrdinalRange();
  //m_pTransforms->vH55_T40_Adjust_BoolCalc_Terms();
  m_pTransforms->vH75_T30_Correct_stmColOdn_Of_keyExists_stream();
  m_pTransforms->vH55_T50_Add_tblColOdn_Under_param();
  m_pTransforms->vH55_T60_Substitute_paramOdn_With_tblColOdn();
  m_pTransforms->vH55_T70_Cull_CallMarked_CallSub();
  m_pTransforms->vH55_T80_Delete_Empty_where();

  m_pTransforms->vH55_T99_End();

  vEndHand(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vHand_60_Index_Join(i8 iBelow, ...)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vHand(BK_SUBPIPE, 0, __func__);

  CARD_BELOW

  if (! m_pTransforms->bH60_T00_Test()) {
    m_pTransforms->vH60_T00_Skip();
    return;
  }

  m_pTransforms->vH60_T00_Begin();

  m_pTransforms->vH60_T05_Twist_in_Becomes_filter();
  m_pTransforms->vH60_T10_Produce_maybeIndexJoin_Under_filter();
  m_pTransforms->vH60_T20_Affix_tblColOdn_ToAll_idxCol();
  m_pTransforms->vH60_T30_UpCopy_filter_pred_Into_bounds();
  m_pTransforms->vH60_T40_Cull_UpCopied_filter_pred();
  m_pTransforms->vH60_T50_Delete_Empty_filter();
  m_pTransforms->vH60_T60_Delete_Twist();

  m_pTransforms->vH60_T99_End();

  vEndHand(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vHand_70_Join_Ordering(i8 iBelow, ...)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vHand(BK_SUBPIPE, 0, __func__);

  CARD_BELOW

  if (! m_pTransforms->bH70_T00_Test()) {
    m_pTransforms->vH70_T00_Skip();
    return;
  }

  m_pTransforms->vH70_T00_Begin();

/*
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // PARTITION BOARD

  //
  m_pTransforms->vH70_T00_Extract_tableList_From_queryBoard();

  // add <term/<columnRef> to all ordinal <term>
  //m_pTransforms->vAdd_columnRef_Under_term(bPrintXform,bTrace);

  m_pTransforms->vH70_T10_Extract_joinPredList();
  m_pTransforms->vH70_T20_Compute_coPtnPredList();
  m_pTransforms->vH70_T30_Compute_coPtnJoinList();
  m_pTransforms->vH70_T40_Compute_crossPtnJoinList();
  m_pTransforms->vH70_T50_Compute_ptndJoinSet();

  m_pTransforms->vH70_T60_Copy_ptndJoinSet();
  m_pTransforms->vH70_T70_Generate_ptndPermutations();
  m_pTransforms->vH70_T80_Remove_invalidPermutations();

  m_pTransforms->vH70_T90_Setup_costEst();
  m_pTransforms->vH70_T92_Estimate_ptndJoins();
  m_pTransforms->vH70_T94_Estimate_network();
  m_pTransforms->vH70_T96_Estimate_joinOrder();
  m_pTransforms->vH70_T98_Extract_joinOrderSeq();
*/

  m_pTransforms->vH70_T99_End();

  vEndHand(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vHand_75_EE_operators(i8 iBelow, ...)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vHand(BK_SUBPIPE, 0, __func__);

  CARD_BELOW

  if (! m_pTransforms->bH75_T00_Test()) {
    m_pTransforms->vH75_T00_Skip();
    return;
  }

  m_pTransforms->vH75_T00_Begin();

  m_pTransforms->vH75_T10_Morph_table_To_EEop();
  m_pTransforms->vH75_T20_Morph_keyExists_filter_To_constraint();
  m_pTransforms->vH75_T40_Convert_stmColOdn_Of_constraint_To_stmColNm();

  m_pTransforms->vH75_T99_End();

  vEndHand(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vHand_80_Index_Scan(i8 iBelow, ...)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vHand(BK_SUBPIPE, 0, __func__);

  CARD_BELOW

  /*
 *
 *    <scan>       <scan>
 *      <table>    <table>
 *      <stream>   <indexList>
 *      <filter>   <stream>
 *                 <filter>
 *
 */

  if (! m_pTransforms->bH80_T00_Test()) {
    m_pTransforms->vH80_T00_Skip();
    return;
  }

  m_pTransforms->vH80_T00_Begin();

  m_pTransforms->vH80_T10_Produce_Scratch_indexList_Under_scan();
  m_pTransforms->vH80_T20_Affix_idxColOdn_ToAll_idxCol();
  m_pTransforms->vH80_T30_Mark_ULRangeOf_scan_filter_cmpPred();
  m_pTransforms->vH80_T40_Mark_IndexMatchOf_scan_filter_terms();
  m_pTransforms->vH80_T50_Add_scan_bounds_ByCopyingMarked_filter_cmpPred();
  m_pTransforms->vH80_T60_Cull_Marked_filter_cmpPred();
  m_pTransforms->vH80_T70_Morph_scan_To_indexScan();

  m_pTransforms->vH80_T99_End();

  vEndHand(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vHand_90_Stream_Ordering(i8 iBelow, ...)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vHand(BK_SUBPIPE, 0, __func__);

  CARD_BELOW

  if (! m_pTransforms->bH90_T00_Test()) {
    m_pTransforms->vH90_T00_Skip();
    return;
  }

  m_pTransforms->vH90_T00_Begin();

  m_pTransforms->vH90_T10_Remove_sort_IfSameAs_gby();

  m_pTransforms->vH90_T99_End();

  vEndHand(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vHand_A0_Filter_Refinement(i8 iBelow, ...)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vHand(BK_SUBPIPE, 0, __func__);

  CARD_BELOW

  if (! m_pTransforms->bHA0_T00_Test()) {
    m_pTransforms->vHA0_T00_Skip();
    return;
  }

  m_pTransforms->vHA0_T00_Begin();

  m_pTransforms->vHA0_T10_Individuate_filter();

  m_pTransforms->vHA0_T99_End();

  vEndHand(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vHand_Z0_Restate_Pipeline(i8 iBelow, ...)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  vHand(BK_SUBPIPE, 0, __func__);

  CARD_BELOW

  if (! m_pTransforms->bHZ0_T00_Test()) {
    m_pTransforms->vHZ0_T00_Skip();
    return;
  }

  m_pTransforms->vHZ0_T00_Begin();

  m_pTransforms->vHZ0_T10_Restore_stmColOdn_from_tblColOdn();
  //m_pTransforms->vHZ0_T20_Reinstate_stages();
  //m_pTransforms->vHZ0_T20_Reinstate_stagesV2();
  //m_pTransforms->vHZ0_T30_Reinstate_combined();
  //m_pTransforms->vHZ0_T40_Reinstate_next();

  m_pTransforms->vHZ0_T20_Reinstate_stagesV3();

  m_pTransforms->vHZ0_T99_End();

  vEndHand(BK_SUBPIPE,0);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vEndHand(eBK eBoard, i8 i_Board)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  //m_pTransforms->vXform(C_NA);
  //m_pCardCtl->vUseXhand();

  m_pTransforms->vPrintBench(eBoard, i_Board);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transformer::vHand(eBK eBoard, i8 i_Board, const char* azHand)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMER, _FOOTER_);

  m_pTransforms->vUsingBoard(eBoard, i_Board);
  m_pTransforms->vXhand(azHand);

  m_pBench->vClearMarks();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
