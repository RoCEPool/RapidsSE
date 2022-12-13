/*============================================================================*/
/* Xseries.cpp --                                                             */
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

  vSeries_10_Condense_Info     (C_ENABLED);
//  vSeries_20_Table_Analysis    (C_ENABLED);
//  vSeries_22_Stream_Analysis   (C_ENABLED);
//  vSeries_24_Index_Analysis    (C_ENABLED);
//  vSeries_26_SubQuery_Analysis (C_ENABLED);
//vSeries_30_Literal_Arithmetic(C_ENABLED);
  vSeries_32_To_BoolCalc       (C_ENABLED);
  vSeries_33_To_TblColOdn      (C_ENABLED);
  vSeries_34_Predicate_Pullup  (C_ENABLED);
  vSeries_36_Bound_Analysis    (C_ENABLED);

  // ---
  vSeries_40_Flatten_Query     (C_ENABLED);
  vSeries_50_Join_Ordering                   (C_DISABLED);
  vSeries_75_EE_operators      (C_ENABLED);

  vSeries_80_Index_Scan        (C_ENABLED);
  vSeries_90_Stream_Ordering   (C_ENABLED);

  m_pTransforms->vOptimiseLoop();

  vSeries_92_Project_Pullup                  (C_DISABLED);

  // ---
  vSeries_A0_Filter_Refinement (C_ENABLED);
  vSeries_E0_To_StmOdn         (C_ENABLED);
  vSeries_Z0_Restate_Pipeline  (C_ENABLED);

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

    m_pTransforms->vS10_T_Condense_Info();
    m_pTransforms->vS20_T_Table_Analysis();
    m_pTransforms->vS22_T_Stream_Analysis();
    m_pTransforms->vS24_T_Index_Analysis();
  }

  vSeries(BK_SUBPIPE, 0, __func__);

  m_pTransforms->vS26_T10_SubQuery_Analysis_Of_Main_Query();
  m_pTransforms->vS26_T20_SubQuery_Analysis_Of_Sub_Query();

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

    m_pTransforms->vS20_T_Table_Analysis();
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

  if (!m_pTransforms->bS22_T00_Test(bEnabled)) {
    m_pTransforms->vS22_T00_Skip();
    return;
  }

  m_pTransforms->vS22_T00_Begin();

  for (i8 i_Board = 0;
       m_pBench->bBoardExists(BK_SUBPIPE,i_Board);
       i_Board++) {

    m_pTransforms->vUsingBoard(BK_SUBPIPE, i_Board);

    m_pTransforms->vS22_T_Stream_Analysis();
  }

  m_pTransforms->vS22_TZZ_End();

  vEndSeries(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries_24_Index_Analysis(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  if (! m_pTransforms->bS24_T00_Test(bEnabled)) {
    m_pTransforms->vS24_T00_Skip();
    return;
  }

  m_pTransforms->vS24_T00_Begin();

  for (i8 i_Board = 0;
       m_pBench->bBoardExists(BK_SUBPIPE,i_Board);
       i_Board++) {

    m_pTransforms->vUsingBoard(BK_SUBPIPE, i_Board);

    m_pTransforms->vS24_T_Index_Analysis();
  }

  m_pTransforms->vS24_TZZ_End();

  vEndSeries(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries_26_SubQuery_Analysis(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  if (!m_pTransforms->bS26_T00_Test(bEnabled)) {
    m_pTransforms->vS26_T00_Skip();
    return;
  }

  m_pTransforms->vS26_T00_Begin();

  m_pTransforms->vS26_T10_SubQuery_Analysis_Of_Main_Query();
  m_pTransforms->vS26_T20_SubQuery_Analysis_Of_Sub_Query();

  m_pTransforms->vS26_TZZ_End();

  vEndSeries(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries_32_To_BoolCalc(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  if (!m_pTransforms->bS32_T00_Test(bEnabled)) {
    m_pTransforms->vS32_T00_Skip();
    return;
  }

  m_pTransforms->vS32_T00_Begin();

  for (i8 i_Board = 0;
       m_pBench->bBoardExists(BK_SUBPIPE,i_Board);
       i_Board++) {

    m_pTransforms->vUsingBoard(BK_SUBPIPE, i_Board);

    m_pTransforms->vS32_T_SubQuery_Analysis();
  }

  m_pTransforms->vS32_TZZ_End();

  vEndSeries(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries_33_To_TblColOdn(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  if (!m_pTransforms->bS33_T00_Test(bEnabled)) {
    m_pTransforms->vS33_T00_Skip();
    return;
  }

  m_pTransforms->vS33_T00_Begin();

  for (i8 i_Board = 0;
       m_pBench->bBoardExists(BK_SUBPIPE,i_Board);
       i_Board++) {

    m_pTransforms->vUsingBoard(BK_SUBPIPE, i_Board);

    m_pTransforms->vS33_T10_Convert_stmColOdn_To_tblColOdn();
  }

  m_pTransforms->vS33_TZZ_End();

  vEndSeries(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries_34_Predicate_Pullup(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  if (! m_pTransforms->bS34_T00_Test(bEnabled)) {
    m_pTransforms->vS34_T00_Skip();
    return;
  }

  m_pTransforms->vS34_T00_Begin();

  for (i8 i_Board = 0;
       m_pBench->bBoardExists(BK_SUBPIPE,i_Board);
       i_Board++) {

    m_pTransforms->vUsingBoard(BK_SUBPIPE, i_Board);

    m_pTransforms->vS34_T_Predicate_Pullup();
  }

  m_pTransforms->vS34_TZZ_End();

  vEndSeries(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries_36_Bound_Analysis(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  if (! m_pTransforms->bS36_T00_Test(bEnabled)) {
    m_pTransforms->vS36_T00_Skip();
    return;
  }

  m_pTransforms->vS36_T00_Begin();

  for (i8 i_Board = 0;
       m_pBench->bBoardExists(BK_SUBPIPE,i_Board);
       i_Board++) {

    m_pTransforms->vUsingBoard(BK_SUBPIPE, i_Board);

    m_pTransforms->vS36_T10_Derive_bounds_From_filter();
  }

  m_pTransforms->vS36_TZZ_End();

  vEndSeries(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/* flatten query                                                              */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries_40_Flatten_Query(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  if (!m_pTransforms->bS40_T00_Test(bEnabled)) {
    m_pTransforms->vS40_T00_Skip();
    return;
  }

  m_pTransforms->vS40_T00_Begin();

  m_pTransforms->vS40_T05_Flatten_By_Decision_Tree();

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
void Xseries::vSeries_75_EE_operators(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  if (! m_pTransforms->bS75_T00_Test(bEnabled)) {
    m_pTransforms->vS75_T00_Skip();
    return;
  }

  m_pTransforms->vS75_T00_Begin();

  //m_pTransforms->vS75_T05_ChooseIndexes();
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

  m_pTransforms->vS80_T30_Morph_scan_To_indexScan();

  m_pTransforms->vS80_TZZ_End();

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
  m_pTransforms->vS90_T30_Derive_order();

  //m_pTransforms->vS90_TF0_Remove_sort_IfSameAs_gby();

  m_pTransforms->vS90_TZZ_End();

  vEndSeries(BK_SUBPIPE, 0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries_92_Project_Pullup(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  if (! m_pTransforms->bS92_T00_Test(bEnabled)) {
    m_pTransforms->vS92_T00_Skip();
    return;
  }

  m_pTransforms->vS92_T00_Begin();

  m_pTransforms->vS92_T10_Project_Pullup();

  m_pTransforms->vS92_TZZ_End();

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

  m_pTransforms->vSA0_T10_Eliminate_bounds_From_filter();
  m_pTransforms->vSA0_T20_Individuate_filter();

  m_pTransforms->vSA0_TZZ_End();

  vEndSeries(BK_SUBPIPE, 0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries_E0_To_StmOdn(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  if (! m_pTransforms->bSE0_T00_Test(bEnabled)) {
    m_pTransforms->vSE0_T00_Skip();
    return;
  }

  m_pTransforms->vSE0_T00_Begin();

  m_pTransforms->vSE0_T10_Convert_TblColOdn_To_StmOdn();
  m_pTransforms->vSE0_T20_Convert_tblColOdn_Of_constraint_To_stmColNm();

  m_pTransforms->vSE0_TZZ_End();

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
