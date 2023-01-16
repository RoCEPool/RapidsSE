/*============================================================================*/
/* Xseries.cpp --                                                             */
/*============================================================================*/

#include "Xseries.hpp"
#include "Optimiser.hpp"
#include "Job.hpp"
#include "CardIter.hpp"
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

  _checkInitialised_(CN_XSERIES);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_XSERIES,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Xseries::~Xseries(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_XSERIES,_FOOTER_);

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* apply the reducePipeline set of transformations to all subPipe boards      */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries_10_Make_Runnable(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  m_pBench->vSeries(BK_SUBPIPE, 0, __func__);

//  if (! m_pTransforms->bS10_T00_Test(bEnabled)) {
//    m_pTransforms->vS10_T00_Skip();
//    return;
//  }

  m_pBench->vS10_T00_Begin();

  for (i8 i_Board = 0;
       m_pBench->bBoardExists(BK_SUBPIPE,i_Board);
       i_Board++) {

    m_pBench->m_pCI->vUsingBoard(BK_SUBPIPE, i_Board);

    m_pBench->vS05_T_Move_On();
    m_pBench->vS10_T_Condense_Info();
    m_pBench->vS20_T_Table_Analysis();
    m_pBench->vS22_T_Stream_Analysis();
    //m_pBench->vS24_T_Projection_Analysis();
    m_pBench->vS26_T_Index_Analysis();
  }

  m_pBench->vSeries(BK_SUBPIPE, 0, __func__);

  m_pBench->vS26_T10_SubQuery_Analysis_Of_Main_Query();
  m_pBench->vS26_T20_SubQuery_Analysis_Of_Sub_Query();

  for (i8 i_Board = 0;
       m_pBench->bBoardExists(BK_SUBPIPE,i_Board);
       i_Board++) {

    m_pBench->m_pCI->vUsingBoard(BK_SUBPIPE, i_Board);

    m_pBench->vS32_T_SubQuery_Analysis();
    m_pBench->vS33_T10_Convert_stmColOdn_To_tblColOdn();

    m_pBench->vS34_T_Predicate_Pullup();

#ifndef WITH_OPT_JOINORDER
    if (Card_Deck::bOrder_Joins(m_pBench->m_pCI)) {
      m_pBench->vS50_T00_Join_Ordering_v2();
      m_pBench->vS34_T_Predicate_Reposition();
    }
#endif

  }

  m_pBench->vSeries(BK_SUBPIPE, 0, __func__);

#ifndef WITH_OPT_FLATTENING
  m_pBench->vS40_T05_Flatten_By_Decision_Tree();
#endif

  m_pBench->vS34_T_Predicate_Pullup();
  m_pBench->vS36_T10_Derive_bounds_From_filter();


  m_pBench->vS75_T10_Morph_table_To_EEop();

#ifndef WITH_OPT_KEYEXISTS
  m_pBench->vS75_T20_Morph_keyExists_filter_To_constraint();
#endif

#ifndef WITH_OPT_TOCOLNM
  m_pBench->vS75_T40_Convert_stmColOdn_Of_constraint_To_stmColNm();
#endif

  m_pBench->vEndSeries(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries_80_Index_Scan(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  m_pBench->vSeries(BK_SUBPIPE, 0, __func__);

  /*
 *
 *    <scan>       <scan>
 *      <table>    <table>
 *      <stream>   <indexList>
 *      <filter>   <stream>
 *                 <filter>
 *
 */

  if (! m_pBench->bS80_T00_Test(bEnabled)) {
    m_pBench->vS80_T00_Skip();
    return;
  }

  m_pBench->vS80_T00_Begin();

  m_pBench->vS80_T30_Morph_scan_To_indexScan();

  m_pBench->vS80_TZZ_End();

  m_pBench->vEndSeries(BK_SUBPIPE, 0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries_90_Stream_Ordering(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  m_pBench->vSeries(BK_SUBPIPE, 0, __func__);

  if (! m_pBench->bS90_T00_Test(bEnabled)) {
    m_pBench->vS90_T00_Skip();
    return;
  }

  m_pBench->vS90_T00_Begin();
  m_pBench->vS90_T30_Derive_order();

  //m_pTransforms->vS90_TF0_Remove_sort_IfSameAs_gby();

  m_pBench->vS90_TZZ_End();

  m_pBench->vEndSeries(BK_SUBPIPE, 0);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xseries::vSeries_A0_Make_Weavable(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XSERIES, _FOOTER_);

  m_pBench->vSeries(BK_SUBPIPE, 0, __func__);

  m_pBench->vSA0_T00_Begin();

  m_pBench->vSA0_T05_Cull_Nonused_bounds_of_Chosen_index();
  m_pBench->vSA0_T10_Eliminate_bounds_From_filter();
  m_pBench->vSA0_T20_Individuate_filter();

  m_pBench->vSZ0_T20_Reinstate_stagesV3();
  m_pBench->vSZ0_T30_Add_Partitioning();

  m_pBench->vSE0_T10_Convert_TblColOdn_To_StmOdn();
  m_pBench->vSE0_T20_Convert_tblColOdn_Of_constraint_To_stmColNm();


  m_pBench->vSA0_TZZ_End();

  m_pBench->vEndSeries(BK_SUBPIPE, 0);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
