/*============================================================================*/
/* Xtransforms.cpp --                                                          */
/*============================================================================*/

#include "Xtransforms.hpp"
#include "Optimiser.hpp"
#include "Board.hpp"

#include "Fold_W_A_C_ro.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, Xtransforms, CN_TRANSFORMS)
__aNewFunction(Xtransforms, CN_TRANSFORMS)
__vDeleteFunction(Xtransforms, CN_TRANSFORMS)
__aFooterFunction(Xtransforms)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Xtransforms::Xtransforms(Bench* pBench)
: Object(CN_TRANSFORMS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_pCI = pBench->m_pCI;

  _checkInitialised_(CN_TRANSFORMS);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Xtransforms::~Xtransforms(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/




/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vInitialAdjustments(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCI->m_pBench->vS05_T_Move_On();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vCondenseInfo(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCI->m_pBench->vS10_T_Condense_Info();

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vMetaAnalysis(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCI->m_pBench->vS20_T_Table_Analysis();
  m_pCI->m_pBench->vS22_T_Stream_Analysis();
  m_pCI->m_pBench->vS26_T_Index_Analysis();

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vPredicatePullup(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCI->m_pBench->vS33_T10_Convert_stmColOdn_To_tblColOdn();

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_WHERE, CK_RPNCALC)) {
    m_pCI->G.p_boolCalc = m_pCI->G.p_rpnCalc->pToBoolCalc();

    m_pCI->G.p_where->vReplaceSubCard(m_pCI->G.p_rpnCalc,m_pCI->G.p_boolCalc);

    m_pCI->G_vDeleteMember(m_pCI->G.p_rpnCalc);
  }

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_HAVING, CK_RPNCALC)) {
    m_pCI->G.p_boolCalc = m_pCI->G.p_rpnCalc->pToBoolCalc();

    m_pCI->G.p_having->vReplaceSubCard(m_pCI->G.p_rpnCalc,m_pCI->G.p_boolCalc);

    m_pCI->G_vDeleteMember(m_pCI->G.p_rpnCalc);
  }


  m_pCI->m_pBench->vS34_T_Predicate_Pullup();

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vJoinOrdering(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vIndexBounds(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCI->m_pBench->vS36_T10_Derive_bounds_From_filter();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vFlattenSubquery(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vChooseEEops(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCI->m_pBench->vS75_T10_Morph_table_To_EEop();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vCullBounds(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

   m_pCI->m_pBench->vSA0_T05_Cull_Nonused_bounds_of_Chosen_index();
   m_pCI->m_pBench->vSA0_T10_Eliminate_bounds_From_filter();
   m_pCI->m_pBench->vSA0_T20_Individuate_filter();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vStaging(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

   m_pCI->m_pBench->vSZ0_T20_Reinstate_stagesV3();

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vPartitioning(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

   m_pCI->m_pBench->vSZ0_T30_Add_Partitioning();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vFinalAdjustments(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

   m_pCI->m_pBench->vSE0_T10_Convert_TblColOdn_To_StmOdn();
   m_pCI->m_pBench->vSE0_T20_Convert_tblColOdn_Of_constraint_To_stmColNm();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vOptimiseLoop(void) {

  bool bOpts;
  i8 iOpts = 0;

  //vTrace("%s\n"_Stz,  m_pCI->m_pBench->pOptimiser()->szWeave());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Discover and perform the optimisation transformations

  do {

    bOpts = false;

    bOpts |= bOT_Cut_Sort();
    bOpts |= bOT_Dup_Filter();
    bOpts |= bOT_Ely_Filters();
    bOpts |= bOT_Ely_Projections();
    bOpts |= bOT_Min_Groupby();
    bOpts |= bOT_Otm_Calculations();
    bOpts |= bOT_Ppn_Projections();
    // todo: disuss with Alka
//    bOpts |= bOT_Rpl_Aggreg_By_OrdAgg();
    bOpts |= bOT_Rpl_Aggreg_By_Sort_OrdAgg();
    bOpts |= bOT_Rpl_Max_By_Top();
    bOpts |= bOT_Rpl_Scan_by_IndexScan();
    bOpts |= bOT_Rpl_SortDesc_Limit_By_Top();
    bOpts |= bOT_Rpl_Sort_Limit_OnOrigin_By_AllLevels();
    bOpts |= bOT_Seq_Join_Order();

  } while (bOpts);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bOT_Cut_Sort(void) {

  //
  // Query1, Query4, Query7, Query8, Query12
  //

  // Notes:
  // sbyElist matches the gbyElist of an earlier (on all partitions) stage
  //   including the direction

  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMain(CK_SORT)) {
    Card_Order* p_order1 = m_pCI->G.p_sort->pSub_stream()->pSub_order();
    if (! p_order1)
      return false;

    if (m_pCI->G_bResolveSubCard(CK_STREAM,m_pCI->G.i_MainCard)) {
      Card_Order *p_order2 = m_pCI->G.p_stream->pSub_order();
      if (! p_order2)
        return false;

      // are the two order's the same?
      if (p_order1->bValuesSameRecursive(p_order2)) {

        m_pCI->G_vRemoveDeleteMain(CK_SORT);
        return true;
      }

    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bOT_Dup_Filter(void) {

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
bool Xtransforms::bOT_Ely_Filters(void) {

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
bool Xtransforms::bOT_Ely_Projections(void) {

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
bool Xtransforms::bOT_Min_Groupby(void) {

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
bool Xtransforms::bOT_Otm_Calculations(void) {

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
bool Xtransforms::bOT_Ppn_Projections(void) {

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
bool Xtransforms::bOT_Rpl_Aggreg_By_OrdAgg(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

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

  m_pCI->G.pDeck->vRenumberRecursive();

  m_pCI->vPrintBoard(__LINE__);

  m_pCI->G_vRestart();
  if (m_pCI->G_bEachMain(CK_GBYAGG)) {
    Card_Order* p_order1 = m_pCI->G.p_gbyAgg->pSub_stream()->pSub_order();

    if (m_pCI->G_bResolveSubCard(CK_STREAM,m_pCI->G.i_MainCard)) {
      Card_Order *p_order2 = m_pCI->G.p_stream->pSub_order();
      if (! p_order2)
        return false;

      p_order1->vDump(0,"ORDER1"_Stz);
      p_order2->vDump(0,"ORDER2"_Stz);

      // are the two order's the same?
      if (p_order1->bValuesSameRecursive(p_order2)) {
        m_pCI->G.p_gbyAgg->vCard(CK_ORDAGG);

        m_pCI->vPrintBoard(__LINE__);

        RASSERT(m_pCI->G_bEachMain(CK_GBYAGG));
        m_pCI->G_vRemoveDeleteMain(CK_GBYAGG);

        m_pCI->vPrintBoard(__LINE__);

        RASSERT(m_pCI->G_bEachMain(CK_GBYAGG));
        m_pCI->G_vRemoveDeleteMain(CK_GBYAGG);

        return true;
      }
    }
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bOT_Rpl_Aggreg_By_Sort_OrdAgg(void) {

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bOT_Rpl_Max_By_Top(void) {

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
bool Xtransforms::bOT_Rpl_Scan_by_IndexScan(void) {

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
//      vS90_T30_Derive_order();
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
bool Xtransforms::bOT_Rpl_SortDesc_Limit_By_Top(void) {

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
bool Xtransforms::bOT_Rpl_Sort_Limit_OnOrigin_By_AllLevels(void) {

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
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bOT_Seq_Join_Order(void) {

  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
