/*============================================================================*/
/* Xseries.cpp --                                                             */
/*============================================================================*/

#include "Xseries.hpp"
#include "Xtransforms.hpp"
#include "Xute.hpp"
#include "Optimiser.hpp"
#include "Job.hpp"

#include "Error.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
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
bool Xseries::bTransformSeries(eEC* peErrorCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XSERIES,_FOOTER_);

  bool bClean = true;

  //m_pCardCtl->vReset();
  //m_pCardCtl->vPrintSubCards(false,CK_AGGELIST,CK_GBYELIST,CK_PROJELIST,0);
  //m_pCardCtl->vPrintSubCards(CK_AGGELIST, true);
  //m_pCardCtl->vPrintSubCards(CK_GBYELIST, true);

  try {
    m_pCardCtl->vPrintSubCards(false, CK_AGGELIST, CK_GBYELIST, CK_PROJELIST,
                               CK_RPNCALC, CK_STREAM, 0);
    m_pCardCtl->vSaveShorter();
    m_pCardCtl->vSaveXhand();

    vSeries_10_Make_Runnable();

    // ---

    vSeries_80_Index_Scan(C_ENABLED);
    vSeries_90_Stream_Ordering(C_ENABLED);

    m_pTransforms->vOptimiseLoop();

    // ---
    vSeries_A0_Make_Weavable();

    bClean = true;
  }

  catch (Exception xException) {
    // set peErrorCode
    bClean = false;
  }

  catch (Error xError) {
    *peErrorCode = xError.eErrorCode();
    // todo: this should read xError.szErrorMsg()
    //Stz* szError = xError.aErrorMsg();

    bClean = false;
  }

  return bClean;
}

/*----------------------------------------------------------------------------*/
/* apply the reducePipeline set of transformations to all subPipe boards      */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries_10_Make_Runnable(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

//  if (! m_pTransforms->bS10_T00_Test(bEnabled)) {
//    m_pTransforms->vS10_T00_Skip();
//    return;
//  }

  m_pTransforms->vS10_T00_Begin();

  for (i8 i_Board = 0;
       m_pBench->bBoardExists(BK_SUBPIPE,i_Board);
       i_Board++) {

    m_pTransforms->vUsingBoard(BK_SUBPIPE, i_Board);

    m_pTransforms->vS05_T_Move_On();
    m_pTransforms->vS10_T_Condense_Info();
    m_pTransforms->vS20_T_Table_Analysis();
    m_pTransforms->vS22_T_Stream_Analysis();
    //m_pTransforms->vS24_T_Projection_Analysis();
    m_pTransforms->vS26_T_Index_Analysis();
  }

  vSeries(BK_SUBPIPE, 0, __func__);

  m_pTransforms->vS26_T10_SubQuery_Analysis_Of_Main_Query();
  m_pTransforms->vS26_T20_SubQuery_Analysis_Of_Sub_Query();

  for (i8 i_Board = 0;
       m_pBench->bBoardExists(BK_SUBPIPE,i_Board);
       i_Board++) {

    m_pTransforms->vUsingBoard(BK_SUBPIPE, i_Board);

    m_pTransforms->vS32_T_SubQuery_Analysis();
    m_pTransforms->vS33_T10_Convert_stmColOdn_To_tblColOdn();

    m_pTransforms->vS34_T_Predicate_Pullup();

#ifndef WITH_OPT_JOINORDER
    if (Card_Deck::bOrder_Joins(static_cast<CardIter*>(m_pTransforms))) {
      m_pTransforms->vS50_T00_Join_Ordering_v2();
      m_pTransforms->vS34_T_Predicate_Reposition();
    }
#endif

  }

  vSeries(BK_SUBPIPE, 0, __func__);

#ifndef WITH_OPT_FLATTENING
  m_pTransforms->vS40_T05_Flatten_By_Decision_Tree();
#endif

  m_pTransforms->vS34_T_Predicate_Pullup();
  m_pTransforms->vS36_T10_Derive_bounds_From_filter();


  m_pTransforms->vS75_T10_Morph_table_To_EEop();

#ifndef WITH_OPT_KEYEXISTS
  m_pTransforms->vS75_T20_Morph_keyExists_filter_To_constraint();
#endif

#ifndef WITH_OPT_TOCOLNM
  m_pTransforms->vS75_T40_Convert_stmColOdn_Of_constraint_To_stmColNm();
#endif

  vEndSeries(BK_SUBPIPE);
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
void Xseries::vSeries_A0_Make_Weavable(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  vSeries(BK_SUBPIPE, 0, __func__);

  m_pTransforms->vSA0_T00_Begin();

  m_pTransforms->vSA0_T05_Cull_Nonused_bounds_of_Chosen_index();
  m_pTransforms->vSA0_T10_Eliminate_bounds_From_filter();
  m_pTransforms->vSA0_T20_Individuate_filter();

  m_pTransforms->vSZ0_T20_Reinstate_stagesV3();
  m_pTransforms->vSZ0_T30_Add_Partitioning();

  m_pTransforms->vSE0_T10_Convert_TblColOdn_To_StmOdn();
  m_pTransforms->vSE0_T20_Convert_tblColOdn_Of_constraint_To_stmColNm();


  m_pTransforms->vSA0_TZZ_End();

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
