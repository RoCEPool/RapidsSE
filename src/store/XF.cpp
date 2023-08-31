/*============================================================================*/
/* XF.cpp -- Make Simpler                                            */
/*============================================================================*/

#include "XF.hpp"

#include "Board.hpp"
#include "CardIter.hpp"
#include "Optimiser.hpp"

#include "XF_Epilog_Column_hashing.hpp"
#include "XF_Epilog_Column_mapping.hpp"
#include "XF_Epilog_Dedupe_as.hpp"
#include "XF_Epilog_Promote_filter.hpp"
#include "XF_Epilog_Reinstate_stage.hpp"
#include "XF_Epilog_stage_cast.hpp"
#include "XF_Epilog_stmCol_To_colNm.hpp"
#include "XF_Epilog_tblCol_To_stmCol.hpp"
#include "XF_Flatten.hpp"
#include "XF_Optimise_aggreg_To_sort_ordAgg.hpp"
#include "XF_Optimise_Cull_bound.hpp"
#include "XF_Optimise_Cut_sort.hpp"
#include "XF_Optimise_Derive_streamKey.hpp"
#include "XF_Optimise_Duplicate_filter.hpp"
#include "XF_Optimise_Early_filter.hpp"
#include "XF_Optimise_Early_projection.hpp"
#include "XF_Optimise_Late_projection.hpp"
#include "XF_Optimise_Reduce_gbyAgg_key.hpp"
#include "XF_Optimise_from_To_OP.hpp"
#include "XF_Optimise_gbyAgg_To_ordAgg.hpp"
#include "XF_Optimise_max_To_top.hpp"
#include "XF_Optimise_Static_calc.hpp"
#include "XF_Optimise_scan_To_indexScan.hpp"
#include "XF_Optimise_sort_limit_All_Levels.hpp"
#include "XF_Optimise_sortDesc_limit_To_top.hpp"
#include "XF_Prolog_Add_level.hpp"
#include "XF_Prolog_Erase_construction.hpp"
#include "XF_Prolog_Move_on.hpp"
#include "XF_Prolog_Shorten_queryNm.hpp"
#include "XF_Prolog_as_Review.hpp"
#include "XF_Prolog_colNm_To_stmCol.hpp"
#include "XF_Prolog_index_Analysis.hpp"
#include "XF_Prolog_not_Ops.hpp"
#include "XF_Prolog_projection_Analysis.hpp"
#include "XF_Prolog_rpnCalc_To_boolCalc.hpp"
#include "XF_Prolog_scan_To_from.hpp"
#include "XF_Prolog_subQuery_Analysis.hpp"
#include "XF_Prolog_table_Analysis.hpp"
#include "XF_Synthesise_code.hpp"
#include "XF_Synthesise_pipelineDef.hpp"

#include "eOT.h"

#include "Job.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF, CN_XF)
__aNewFunction(XF, CN_XF)
__vDeleteFunction(XF, CN_XF)
__aFooterFunction(XF)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF::XF(CardIter* pCI)
: XF_TRANSFORM(CN_XF)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF, _FOOTER_);

  m_pCI = pCI;

  m_xSymbol_tableSyn.vInit_Tbl_Syn('a');

  _checkInitialised_(CN_XF);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF::~XF(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF, _FOOTER_);
}



/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF::vXF(CardIter* pCI)
{
  _static_

  XF* pXF = new XF(pCI);

  if (! AM::JOB::bHasTag(C_OPTIMISE_PASSTHRU)) {
    pXF->vTransform();
  }
  else {
    pXF->vTransform_passThru();
  }

  pCI->vValidateBench();

  delete pXF;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* apply transformations                                                      */
/*----------------------------------------------------------------------------*/
void XF::vTransform(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF, _FOOTER_);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 1/5) prolog

  m_pCI->vPhase(OH_PROLOG);

  m_pCI->vBoardIteration();
  while (m_pCI->bEachBoard())
    XF_Prolog_Shorten_queryNm::vProlog(m_pCI);

  m_pCI->vBoardIteration();
  while (m_pCI->bEachBoard()) {

    // initial adjustments
    XF_Prolog_Move_on::vProlog(m_pCI);
    XF_Prolog_Erase_construction::vProlog(m_pCI);
    XF_Prolog_scan_To_from::vProlog(m_pCI);
    XF_Prolog_Add_level::vProlog(m_pCI);
    XF_Prolog_not_Ops::vProlog(m_pCI);

    // meta analysis
    XF_Prolog_table_Analysis::vProlog(m_pCI, &m_xSymbol_tableSyn);
    XF_Prolog_projection_Analysis::vProlog(m_pCI);
    XF_Prolog_index_Analysis::vProlog(m_pCI);

    // preliminary conversions
    XF_Prolog_rpnCalc_To_boolCalc::vProlog(m_pCI);
    XF_Prolog_as_Review::vProlog(m_pCI);
  }

  // sub-query analysis
  XF_Prolog_subQuery_Analysis::vProlog(m_pCI);

  m_pCI->vEndPhase();


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 2/5) flatten

  m_pCI->vPhase(OH_FLATTEN);

  XF_Flatten::vFlatten(m_pCI);

  m_pCI->vEndPhase();


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 3/5) optimisw

  m_pCI->vPhase(OH_OPTIMISE);

  m_pCI->vBoardIteration();
  RASSERT(m_pCI->bEachBoard());

  XF_Optimise_from_To_OP::vOptimise(m_pCI);
  XF_Optimise_scan_To_indexScan::vOptimise(m_pCI);
  XF_Optimise_Derive_streamKey::vOptimise(m_pCI);
  XF_Optimise_Cut_sort::vOptimise(m_pCI);
  XF_Optimise_gbyAgg_To_ordAgg::vOptimise(m_pCI);
  XF_Optimise_Cull_bound::vOptimise(m_pCI);
  XF_Optimise_Early_projection::vOptimise(m_pCI);

  XF_Optimise_Reduce_gbyAgg_key::vOptimise(m_pCI);
  XF_Optimise_Static_calc::vOptimise(m_pCI);
  XF_Optimise_max_To_top::vOptimise(m_pCI);
  XF_Optimise_aggreg_To_sort_ordAgg::vOptimise(m_pCI);
  XF_Optimise_sortDesc_limit_to_top::vOptimise(m_pCI);
  XF_Optimise_sort_limit_All_Levels::vOptimise(m_pCI);
  XF_Optimise_Early_filter::vOptimise(m_pCI);
  XF_Optimise_Late_projection::vOptimise(m_pCI);
  XF_Optimise_Duplicate_filter::vOptimise(m_pCI);

  m_pCI->vEndPhase();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 4/5) epilog

  m_pCI->vPhase(OH_EPILOG);

  m_pCI->vBoardIteration();
  RASSERT(m_pCI->bEachBoard());

  XF_Epilog_Promote_filter::vEpilog(m_pCI);
  XF_Epilog_Reinstate_stage::vEpilog(m_pCI);
  XF_Epilog_stage_cast::vEpilog(m_pCI);
  XF_Epilog_tblCol_To_stmCol::vEpilog(m_pCI);
  XF_Epilog_stmCol_To_colNm::vEpilog(m_pCI);
  XF_Epilog_Column_mapping::vEpilog(m_pCI);
  XF_Epilog_Column_hashing::vEpilog(m_pCI);

  m_pCI->vEndPhase();

  vCrosscheck_Optimisations();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 5/5) synthesise

  m_pCI->vPhase(OH_SYNTHESISE);

  m_pCI->vBoardIteration();
  RASSERT(m_pCI->bEachBoard());

  XF_Synthesise_pipelineDef::vSynthesise(m_pCI);
  XF_Synthesise_code::vSynthesise(m_pCI);

  m_pCI->vEndPhase();
}

/*----------------------------------------------------------------------------*/
/* apply transformations                                                      */
/*----------------------------------------------------------------------------*/
void XF::vTransform_passThru(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF, _FOOTER_);

  m_pCI->vBoardIteration();
  while (m_pCI->bEachBoard())
    XF_Prolog_Shorten_queryNm::vProlog(m_pCI);

  m_pCI->vBoardIteration();
  while (m_pCI->bEachBoard()) {

    XF_Prolog_table_Analysis::vXF_table_Analysis_passThru(m_pCI, &m_xSymbol_tableSyn);
    XF_Prolog_projection_Analysis::vXF_projection_Analysis_passThru(m_pCI);

    m_pCI->vTransform(__func__, XF_EPILOG_STMCOL_TO_TBLCOL, "all $n become $c.n"_Stz);
    while (m_pCI->G_bEachMain_NONE())
      m_pCI->G.p_OP->vConvert_stmColOdn_To_tblColOdn(m_pCI);
    m_pCI->vEndTransform();

    // todo: needs discussion with Alka re constraint
    XF_Prolog_colNm_To_stmCol::vXF_colNm_To_stmCol(m_pCI);


    m_pCI->vTransform(__func__, XF_PASSTHRU_ADJUST_SORT, "adjust sort"_Stz);
    while (m_pCI->G_bEachMainWith2Sub(CK_SORT,CK_KEY,CK_PROJECTION)) {
      m_pCI->G.p_projection->vAdjust_Wrt_key(m_pCI->G.p_key);
    }
    m_pCI->vEndTransform();

    m_pCI->vTransform(__func__, XF_PASSTHRU_ADJUST_TOP, "adjust top"_Stz);
    while (m_pCI->G_bEachMainWith2Sub(CK_TOP,CK_KEY,CK_PROJECTION)) {
      m_pCI->G.p_projection->vAdjust_Wrt_key(m_pCI->G.p_key);
    }
    m_pCI->vEndTransform();



    m_pCI->vTransform(__func__, XF_PASSTHRU_TWEAK_INDEXJOIN, "tweak index join"_Stz);
    while (m_pCI->G_bEachMain(CK_INDEXJOIN)) {
      m_pCI->G.p_indexJoin->pSub_index()->pSymbol()->Y_vSet(P_ChosenIndex);
      m_pCI->G.p_indexJoin->pSub_bound()->pSymbol()->Y_vSet(P_ChosenBound);
    }
    m_pCI->vEndTransform();
    m_pCI->vTransform(__func__, XF_PASSTHRU_TWEAK_INDEXSCAN, "tweak index scan"_Stz);
    while (m_pCI->G_bEachMain(CK_INDEXSCAN)) {
      m_pCI->G.p_indexScan->pSub_index()->pSymbol()->Y_vSet(P_ChosenIndex);
      m_pCI->G.p_indexScan->pSub_bound()->pSymbol()->Y_vSet(P_ChosenBound);
    }
    m_pCI->vEndTransform();
    m_pCI->vTransform(__func__, XF_PASSTHRU_TWEAK_KEYEXISTS, "tweak key exists"_Stz);
    while (m_pCI->G_bEachMain(CK_KEYEXISTS)) {
      m_pCI->G.p_keyExists->pSub_index()->pSymbol()->Y_vSet(P_ChosenIndex);
      m_pCI->G.p_keyExists->pSub_bound()->pSymbol()->Y_vSet(P_ChosenBound);
    }
    m_pCI->vEndTransform();
    m_pCI->vTransform(__func__, XF_PASSTHRU_TWEAK_KEYNOTEXISTS, "tweak key not exists"_Stz);
    while (m_pCI->G_bEachMain(CK_KEYNOTEXISTS)) {
      m_pCI->G.p_keyNotExists->pSub_index()->pSymbol()->Y_vSet(P_ChosenIndex);
      m_pCI->G.p_keyNotExists->pSub_bound()->pSymbol()->Y_vSet(P_ChosenBound);
    }
    m_pCI->vEndTransform();

    XF_Prolog_rpnCalc_To_boolCalc::vProlog(m_pCI);

    vDebugPoint();
  }

  m_pCI->vValidateBench();

  m_pCI->vUsingBoard((i8)0);
  XF_Epilog_tblCol_To_stmCol::vEpilog(m_pCI);

  XF_Epilog_Column_mapping::vEpilog(m_pCI);
  XF_Epilog_Column_hashing::vEpilog(m_pCI);

  XF_Epilog_Dedupe_as::vXF_Dedupe_as(m_pCI);

  vCrosscheck_Optimisations();

  XF_Synthesise_pipelineDef::vSynthesise(m_pCI);

  XF_Synthesise_code::vSynthesise(m_pCI);
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* crosscheck optimisations                                                   */
/*----------------------------------------------------------------------------*/
void XF::vCrosscheck_Optimisations(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF, _FOOTER_);

  //MY::pJob()->pJobTags()->vDumpSet();

  s32 sOT;

  for (i8 i_OT = 0; i_OT < OT_ENUMS; i_OT++) {
    Stz *szOptimisation = aOT[i_OT];

    sOT.vSprint("<%s>"_Stz,szOptimisation);
    if (! AM::JOB::bHasTag(sOT))
      continue;

    if (m_pCI->pBoard()->pState()->bHasName(szOptimisation))
      continue;

   // m_pCI->vPrintBoard();
    vTrace("Missing optimisation : %lld %lls\n"_Stz,i_OT,szOptimisation);
    NEVER_REACH
  }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

#if 0
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool MakeOptimal::bOptimise_CutSort(void) {

  NEVER_REACH


  /*
  //
  // Query1, Query4, Query7, Query8, Query12
  //

  // Notes:
  // sbyElist matches the gbyElist of an earlier (on all partitions) stage
  //   including the direction

  */

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool MakeOptimal::bOptimise_DupFilter(void) {

  // Query7, Query19

  // Notes:
  // If the query has filters with an OR we need to pick all the values for a column
  // from all the OR filters and copy them close to the table scan/indesxScan/indexJoin
  // of the concerned table.

  // Also Note: These are filters that are not being moved but extracts being
  // copied to the tables scan/indesxScan/indexJoin.

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool MakeOptimal::bOptimise_ElyFilters(void) {

  // Query2, Query3, Query4, Query5, Query7, Query8,
  //   Query9, Query10, Query11, Query12, Query16, Query17 Query19

  // Notes:
  // get columns from cmpPred's and find the tables they come from in previous
  // scan/indexScan/indexJoin cards and move them after the scan/indexJoin/indexJoin
  // of the tables

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool MakeOptimal::bOptimise_ElyProjections(void) {

  // Query2, Query3, Query4, Query5, Query7, Query8,
  //   Query9, Query10, Query11, Query12, Query14,
  //      Query16, Query17, Query18, Query19

  // Notes:
  // get all $ columns from all operators -
  // aggreg, filter, Hash, IndexJoin, keyExists, keyNotExists, OrdAgg, project, Sort, Top
  // (leaving them as is unless its a project being modified)
  // and find the tables they come from in previous scan/indexScan/indexJoin
  // cards and add a project (or add to the project if already there)
  // after the scan/indexJoin/indexJoin of the tables
  // (ensuring no duplicate projections)

  // idea being to get all pprojections realted to a table closest to
  // its scan/indexScan/indexJoin

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool MakeOptimal::bOptimise_MinGroupby(void) {

  //
  // Query3, Query10, Query18
  //

  // Notes:
  // * need to get the index if the groupby colum is coming from an indexscan
  // * and check if index is a unique index on that column
  // * if a match is found , discard all other columns off the gbyElist
  //   except the match (unique one) and need to rejoin with the same table
  //   to get the columns if they were being projected


  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool MakeOptimal::bOptimise_OtmCalculations(void) {

  // Query1, Query4, Query5, Query6, Query10, Query12, Query14, Query15,
  //   Query19, Query20

  // Notes:
  // To figure out if their are any literal calculations that can be done upfront.
  // instead of executing the calculation for every row that is being processed.
  // So need to decipher any arithmetic OP (+-*/) with 2 literals.
  // eg 0.06 0.01 -
  // date '1993-01-01' - interval '1' year

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool MakeOptimal::bOptimise_PpnProjections(void) {

  // Query2, Query10

  // Notes:
  // If a query has an aggregate and either :
  //   the group by columns don't include the columns in the select list
  //   or due to an optimisation OT_MIN_GROUPBY the columns are reduced from
  //     the group by list
  // then we need to rejoin with the particular table/tables and get the required
  // selected columns

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool MakeOptimal::bOptimise_RplAggregByOrdAgg(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  NEVER_REACH

/*
  // Query13, Query17 : do we change the scan to indexScan ????
  // Query18

  // Notes:
  // When we have an aggreg grouped on a column on which their is an index available,
  // we can change the scan to an indexScan (hence data will come in ascending order
  // of the group by key)
  // and then we can change the aggreg to an ordAgm_pCI->G.
  //
  // Also Note: when the ordAgg replaces the aggreg at an all partitions level,
  //  we can delete the aggregates on all nodes and on origin.
  //  This further aggregation is not required.

*/

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool MakeOptimal::bOptimise_RplAggregBySortOrdAgg(void) {

  // Query 3
  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool MakeOptimal::bOptimise_RplMaxByTop(void) {

  // Query15

  // Notes:
  // If a projEList has only one item and it is a max,
  //  then the projectOP can be replace by a TOP of the max column
  //  with 1 as the parameter to TOP
  // e.g
  //   project " type DECIMAL ) $1 max "
  // can be replace by :
  //   top "TOP type DECIMAL $2" 1

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool MakeOptimal::bOptimise_RplScanByIndexScan(void) {

  // Query6, Query10, Query14 and Query15

  // Notes:
  //
  // If there is a Scan followed by a filter and the filter is on a single column
  // with a lower/upper/lower+upper/equal bound then we can replace
  // the scan and part / all of the filter with an indexScan

  // Points to note:
  // ** Some queries (eg Query1) we don't change the scan to an indexscan
  //    because the selection range of the filter covers almost 95% of the rows.
  //    In such a case a scan is better.
  //
  // ** This optimisation can be applied only after OT_ELY_FILTERS.
  //    We won't see it till the Filters are moved close to the scans.

//  m_pCI->G_vRestart();
//  if (m_pCI->G_bEachMainWithSub(CK_SCAN,CK_BOUNDS)) {
//    if (m_pCI->G.p_bounds->pSub_lBound() || m_pCI->G.p_bounds->pSub_uBound()) {
//      m_pCI->G.p_scan->vCard(CK_INDEXSCAN);
//
//
//      vDerive_Projection_Keys();
//
//
//      return true;
//    }
//  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool MakeOptimal::bOptimise_RplSortDescLimitByTop(void) {

  // Query10

  // Notes:
  // sbyElist expressions has a single column and its direction is descending
  // and it is followed by a limit we can replace the sort+limit by TOP.
  // TOP will have the EList from sbyEList and the number (top limit) from limit.

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool MakeOptimal::bOptimise_RplSortLimitOnOriginByAllLevels(void) {

  // Query3

  // Notes:
  // If an aggreg is replaced by an OrdAgg, we know that we don't need the
  // ordAgg at all nodes and origin. It can be only at the partition level.
  // You don't need to aggregate all nodes etc.
  // Because we know that the ordered data being aggregated at all partitions
  // is complete we can replace a sort on origin that follows the aggreg
  // by a sort and limit on all partitions/ on all nodes and on origin.
  //
  // This ways reduced that data to be sorted at each level and reducing the rows
  // moving around as we have applied the limit as well

  return false;
}

#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
