/*============================================================================*/
/* MakeOptimal.cpp -- MakeOptimal                                           */
/*============================================================================*/

#include "MakeOptimal.hpp"

#include "Board.hpp"
#include "CardIter.hpp"
#include "Fold.hpp"
#include "Smoosh.hpp"
#include "Smooth.hpp"
#include "Optimiser.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, MakeOptimal, CN_MAKEOPTIMAL)
__aNewFunction(MakeOptimal, CN_MAKEOPTIMAL)
__vDeleteFunction(MakeOptimal, CN_MAKEOPTIMAL)
__aFooterFunction(MakeOptimal)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MakeOptimal::MakeOptimal(CardIter* pCI)
: Object(CN_MAKEOPTIMAL)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKEOPTIMAL, _FOOTER_);

  m_pCI = pCI;
  m_pCI->vUsingBoard(BK_SUBQUERY, 0);


  _checkInitialised_(CN_MAKEOPTIMAL);

  _postcondition_(_ADDRESS_, _MAGIC_, CN_MAKEOPTIMAL, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MakeOptimal::~MakeOptimal(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKEOPTIMAL, _FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void MakeOptimal::vOptimise(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKEOPTIMAL, _FOOTER_);

  vDelete_flat();

  vMorph_table_To_EEop();

  vMorph_scan_To_indexScan();

  vDerive_Projection_Keys();

  vCut_sort();

  vPromote_gbyAgg_To_ordAgg();

  vCullNonusedBounds();
  vCullUsedBoundsFromFilter();
  vIndividuateFilter();

  vEarlyProjections();

  vDebugPoint();
}


/*----------------------------------------------------------------------------*/
/*-- MakeOptimal ------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void MakeOptimal::vOptimiseLoop(void) {

  bool bOpts;
  i8 iOpts = 0;

  //vTrace("%s\n"_Stz,  pOptimiser()->szWeave());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Discover and perform the optimisation transformations

  do {

    bOpts = false;

    bOpts |= bOptimise_CutSort();
    bOpts |= bOptimise_DupFilter();
    bOpts |= bOptimise_ElyFilters();
    bOpts |= bOptimise_ElyProjections();
    bOpts |= bOptimise_MinGroupby();
    bOpts |= bOptimise_OtmCalculations();
    bOpts |= bOptimise_PpnProjections();
    // todo: disuss with Alka
//    bOpts |= bOptimise_RplAggregByOrdAgg();
    bOpts |= bOptimise_RplAggregBySortOrdAgg();
    bOpts |= bOptimise_RplMaxByTop();
    bOpts |= bOptimise_RplScanByIndexScan();
    bOpts |= bOptimise_RplSortDescLimitByTop();
    bOpts |= bOptimise_RplSortLimitOnOriginByAllLevels();

  } while (bOpts);

}

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
//      m_pCI->vPrintBoard(__LINE__);
//
//      vDerive_Projection_Keys();
//
//      m_pCI->vPrintBoard(__LINE__);
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

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void MakeOptimal::vCut_sort(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKEOPTIMAL, _FOOTER_);

  m_pCI->vTransform(__func__, "Cut sort"_Stz);

  m_pCI->G_vRestart();

  RASSERT(nullptr == m_pCI->StreamOrder.m_pProjection_running);
  RASSERT(nullptr == m_pCI->StreamOrder.m_pKey_running);

  while (m_pCI->G_bEachMain()) {

    switch (m_pCI->G.e_MainCard) {

      case CK_SORT:
        m_pCI->G.p_sort->vCut_sort(m_pCI);
        break;

      default:
        if (m_pCI->G.p_MainCard->pSub_projection()) {
          m_pCI->StreamOrder.m_pProjection_running = m_pCI->G.p_MainCard->pSub_projection();
          m_pCI->StreamOrder.m_pKey_running = m_pCI->StreamOrder.m_pProjection_running->pSub_key();
        }
        break;
    }

    m_pCI->vPrintBoard(__LINE__);
  }

  m_pCI->vEndTransform();
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void MakeOptimal::vDelete_flat(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKEOPTIMAL, _FOOTER_);

  m_pCI->vRestart();
  while (m_pCI->G_bEachMain(CK_FLAT))
    m_pCI->G.pDeck->vDeleteChild(m_pCI->G.p_flat);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void MakeOptimal::vDerive_Projection_Keys(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKEOPTIMAL, _FOOTER_);

  m_pCI->vTransform(__func__, "Derive projection keys"_Stz);

  m_pCI->G_vRestart();

  RASSERT(nullptr == m_pCI->StreamOrder.m_pProjection_running);
  RASSERT(nullptr == m_pCI->StreamOrder.m_pKey_running);

  bool bFoundKeyStart = false;
  for (;;) {
    if (m_pCI->pNextChildIf(CK_INDEXSCAN) ||
        m_pCI->pNextChildIf(CK_GBYAGG)    ||
        m_pCI->pNextChildIf(CK_SORT) ) {
      bFoundKeyStart = true;
      break;
    }

    if (! m_pCI->G_bEachMain())
      break;
  }

  if (bFoundKeyStart) {

    while (m_pCI->G_bEachMain()) {

      switch (m_pCI->G.e_MainCard) {

        case CK_GBYAGG:
          m_pCI->G.p_gbyAgg->vDerive_Projection_Key(m_pCI);
          break;
        case CK_INDEXJOIN:
          m_pCI->G.p_indexJoin->vDerive_Projection_Key(m_pCI);
          break;
        case CK_INDEXSCAN:
          m_pCI->G.p_indexScan->vDerive_Projection_Key(m_pCI);
          break;
        case CK_PROJECT:
          m_pCI->G.p_project->vDerive_Projection_Key(m_pCI);
          break;
        case CK_SORT:
          m_pCI->G.p_sort->vDerive_Projection_Key(m_pCI);
          break;

        default:
          break;
      }

      m_pCI->vPrintBoard(__LINE__);

      if (! m_pCI->StreamOrder.m_pKey_running)
        break;
    }
  }

  m_pCI->vEndTransform();
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void MakeOptimal::vEarlyProjections(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKEOPTIMAL, _FOOTER_);

//  if (! AM::JOB::bHasTag(C_EP_ENABLE))
//    return;
  if (AM::JOB::bHasTag(C_EP_DISABLE))
    return;

  m_pCI->vTransform(__func__, "Early projections"_Stz);

  m_pCI->G_vRestart();

  m_pCI->G.pDeck->vRenumberRecursive();

  m_pCI->vPrintBoard();

  RASSERT(nullptr == m_pCI->Early.m_pNeeded_later);

  for (;;) {

    m_pCI->Early.m_pOP_root = nullptr;
    m_pCI->Early.m_pOP_leaf = nullptr;

    bool bEarlyRoot = false;
    while (m_pCI->G_bEachMain_reverse()) {
      if (Card_OP::bIsOp(m_pCI->G.p_MainCard) && m_pCI->G.p_OP->bIsEarlyRoot()) {
        bEarlyRoot = true;
        break;
      }
    }
    if (! bEarlyRoot)
      break;

    m_pCI->G.p_OP->vEarlyProject_root(m_pCI);

    if (m_pCI->Early.m_pNeeded_later) {
      DELETE_CARD(m_pCI->Early.m_pNeeded_later);
    }

    m_pCI->G_vMainResynch(m_pCI->Early.m_pOP_leaf);
  }

  m_pCI->vEndTransform();
  vDebugPoint();
}
/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void MakeOptimal::vMorph_scan_To_indexScan(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKEOPTIMAL, _FOOTER_);

  m_pCI->vTransform(__func__, "((replace with an explanation))"_Stz);

  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMainWith2Sub(CK_SCAN,CK_INDEXLIST,CK_BOUNDS)) {

    if (m_pCI->G.p_bounds->pSub_bound()->bIsLBound() ||
        m_pCI->G.p_bounds->pSub_bound()->bIsUBound()) {


      m_pCI->G.p_scan->vCard(CK_INDEXSCAN);

      m_pCI->G.p_indexList->vChoose_index_For_indexScan();
    }
  }

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* convert all <table> into either <scan> , <indexJoin> or <keyExists>        */
/*----------------------------------------------------------------------------*/
void MakeOptimal::vMorph_table_To_EEop(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKEOPTIMAL, _FOOTER_);

  m_pCI->vTransform(__func__, "convert all <table> into either <scan> or <indexJoin>"_Stz);

  Board::vMorphAll_table_To_EEop(m_pCI);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void MakeOptimal::vPromote_gbyAgg_To_ordAgg(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKEOPTIMAL, _FOOTER_);

  m_pCI->vTransform(__func__, "Promote gbyAgg to ordAgg"_Stz);

  m_pCI->G_vRestart();

  RASSERT(nullptr == m_pCI->StreamOrder.m_pProjection_running);
  RASSERT(nullptr == m_pCI->StreamOrder.m_pKey_running);

  while (m_pCI->G_bEachMain()) {

    switch (m_pCI->G.e_MainCard) {

      case CK_GBYAGG:
        m_pCI->G.p_gbyAgg->vPromote_gbyAgg_To_ordAgg(m_pCI);
        break;

      default:
        if (m_pCI->G.p_MainCard->pSub_projection()) {
          m_pCI->StreamOrder.m_pProjection_running = m_pCI->G.p_MainCard->pSub_projection();
          m_pCI->StreamOrder.m_pKey_running = m_pCI->StreamOrder.m_pProjection_running->pSub_key();
        }
        break;
    }

    m_pCI->vPrintBoard(__LINE__);
  }

  m_pCI->vEndTransform();
  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* inspect filters and remove cmp predicates which are in bounds              */
/*----------------------------------------------------------------------------*/
void MakeOptimal::vCullNonusedBounds(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKEOPTIMAL, _FOOTER_);

  m_pCI->vTransform(__func__, "((desc))"_Stz);

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //

  m_pCI->G_vRestart();

  while (m_pCI->G_bEachMain()) {

    if (!(m_pCI->G.p_MainCard->bIsCard(CK_INDEXJOIN) ||
          m_pCI->G.p_MainCard->bIsCard(CK_INDEXSCAN)))
      continue;
    if (!(m_pCI->G.p_indexList = m_pCI->G.p_MainCard->pSub_indexList()))
      continue;

    // have <bounds> & <filter> under <main>
    Card_Index::vCullNonusedBounds(m_pCI);

  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->vPrintBoard();

  m_pCI->vEndTransform();
}


/*----------------------------------------------------------------------------*/
/* inspect filters and remove cmp predicates which are in bounds              */
/*----------------------------------------------------------------------------*/
void MakeOptimal::vCullUsedBoundsFromFilter(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_MAKEOPTIMAL, _FOOTER_);

  m_pCI->vTransform(__func__, "((desc))"_Stz);

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark any cmpPred that are a bound in bounds

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain()) {

    if (! (m_pCI->G.p_MainCard->bIsCard(CK_INDEXJOIN) ||
           m_pCI->G.p_MainCard->bIsCard(CK_INDEXSCAN) ||
           m_pCI->G.p_MainCard->bIsCard(CK_KEYEXISTS) ||
           m_pCI->G.p_MainCard->bIsCard(CK_KEYNOTEXISTS)
    ))
      continue;
    if (! (m_pCI->G.p_bounds = m_pCI->G.p_MainCard->pSub_bounds()))
      continue;

    if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter())) {

      // have <bounds> & <filter> under <main>

      CardIter xCR(m_pCI->G.p_filter);
      while ((m_pCI->G.p_cmpPred = _pCard_CmpPred(xCR.pRecurseEach(CK_CMPPRED)))) {
        if (m_pCI->G.p_cmpPred->bSubsetOf_bounds(m_pCI->G.p_bounds)) {
          //RASSERT(m_pCI->G.p_cmpPred->bIsJoinPred());
          m_pCI->G.p_cmpPred->vMark1(MARK_AT);
        }
      }

      continue;
    }

    if ((m_pCI->G.p_constraint = m_pCI->G.p_MainCard->pSub_constraint())) {

      // have <bounds> & <filter> under <main>

      CardIter xCR(m_pCI->G.p_constraint);
      while ((m_pCI->G.p_cmpPred = _pCard_CmpPred(xCR.pRecurseEach(CK_CMPPRED)))) {
        if (m_pCI->G.p_cmpPred->bSubsetOf_bounds(m_pCI->G.p_bounds)) {
          //RASSERT(m_pCI->G.p_cmpPred->bIsJoinPred());
          m_pCI->G.p_cmpPred->vMark1(MARK_AT);
        }
      }

      continue;
    }

  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cull marked cmpPred from filter

  m_pCI->G_vRestart();
//  Card_Constraint::vCullUsedBounds(m_pCI);

  while (m_pCI->G_bEachMain()) {
    if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter()))
      m_pCI->G.p_filter->vCullPred(MARK_AT);
    if ((m_pCI->G.p_constraint = m_pCI->G.p_MainCard->pSub_constraint()))
      m_pCI->G.p_constraint->vCullPred(MARK_AT);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete any filter if empty

  m_pCI->G_vRestart();
  Card_Filter::vDeleteEmptyFilters(m_pCI);

//  while (m_pCI->G_bEachMain()) {
//    if ((m_pCI->G.p_filter = m_pCI->G.p_MainCard->pSub_filter()))
//      m_pCI->G.p_MainCard->vDeleteFilterIfEmpty(m_pCI->G.p_filter);
//  }

  m_pCI->vPrintBoard();

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* individuate <scan|indexJoin>/<filter> as main <filter>                     */
/*----------------------------------------------------------------------------*/
void MakeOptimal::vIndividuateFilter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_MAKEOPTIMAL,_FOOTER_);

  m_pCI->vTransform(__func__, "individuate <scan|indexJoin>/<filter> as main <filter>"_Stz);

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_SCAN,CK_FILTER)) {
    Card_Filter::vIndividuate_filter(m_pCI);
  }

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_INDEXSCAN,CK_FILTER)) {
    Card_Filter::vIndividuate_filter(m_pCI);
  }

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_INDEXJOIN,CK_FILTER)) {
    Card_Filter::vIndividuate_filter(m_pCI);
  }

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_CROSSJOIN,CK_FILTER)) {
    Card_Filter::vIndividuate_filter(m_pCI);
  }

  m_pCI->vEndTransform();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
