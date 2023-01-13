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

  m_iOdnLo      = 1;
  m_iOdnHi      = C_UNKNOWN;
  m_cTableSyn   = 'a';

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

  vS05_T_Move_On();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vCondenseInfo(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vS10_T_Condense_Info();

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vMetaAnalysis(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vS20_T_Table_Analysis();
  vS22_T_Stream_Analysis();
  vS26_T_Index_Analysis();

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vPredicatePullup(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vS33_T10_Convert_stmColOdn_To_tblColOdn();

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


  vS34_T_Predicate_Pullup();

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

  vS36_T10_Derive_bounds_From_filter();
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

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                   vSeries_10_Make_Runnable */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS10_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

   m_pCI->m_pBench->vST00_Begin();

   m_pCI->m_pBench->vPrefix('r');
  m_pCI->m_pCardCtl->vUseExpanded();
  m_pCI->vPrintBench(BK_SUBPIPE);
  m_pCI->m_pCardCtl->vUseExpanded();
}

/*----------------------------------------------------------------------------*/
/* Condense Info                                                              */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS05_T_Move_On(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_pCI->vTransform(__func__, "move <on> in <absJoin>'s to <where>"_Stz);

   m_pCI->m_pBench->vAssert_Board_any();

  if (m_pCI->G_bEachMain(CK_WHERE))
    {;}

  //vRestart();
  m_pCI->vRestart();

  while (m_pCI->G_bEachMainWithSub_reverse(CK_ABSJOIN,CK_ON)) {
    Card_Where::vMove_on_To_where(m_pCI);
  }

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* Condense Info                                                              */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS10_T_Condense_Info(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_pCI->vTransform(__func__, "delete superfluous <absJoin>'s"_Stz);

   m_pCI->m_pBench->vAssert_Board_any();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Delete All AbsJoin

  while (m_pCI->G_bEachMain(CK_ABSJOIN)) {
    m_pCI->G_vRemoveDeleteMain(CK_ABSJOIN);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Delete All Combined

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain(CK_COMBINED)) {
    m_pCI->G_vRemoveDeleteMain(CK_COMBINED);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Delete All Send

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain(CK_SEND)) {
    m_pCI->G_vRemoveDeleteMain(CK_SEND);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Delete All Stage

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain(CK_STAGE)) {
    m_pCI->G_vRemoveDeleteMain(CK_STAGE);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Cull Scan Leaving Table

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain(CK_SCAN)) {
    m_pCI->G.pDeck->vReplaceChildWithItsSubs(m_pCI->G.p_scan);
    DELETE_CARD(m_pCI->G.p_scan);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Add All Level

  m_pCI->G_vRestart();
  m_pCI->G.e_Level = AL_PARTITION;
  while (m_pCI->G_bEachMain()) {

    m_pCI->G.p_level = m_pCI->G.p_MainCard->pSub_level();
    if (m_pCI->G.p_level)
      continue;

    m_pCI->G.p_level = new Card_Level(AL_PARTITION);
    m_pCI->G.p_MainCard->vPush(m_pCI->G.p_level);
  }

  //
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                  vSeries_20_Table_Analysis */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* Table Analysis                                                             */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS20_T_Table_Analysis(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_pCI->vTransform(__func__, "add a <tableSyn> to all <table>'s"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Affix Table Syntax To All Table

  while (m_pCI->G_bEachMainWithSub(CK_TABLE, CK_TABLENM))  {
    m_pCI->G.p_table->vSet_Syn(&m_cTableSyn);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Produce Table Partition Under All Table

  m_pCI->G_vRestart();

  while (m_pCI->G_bEachMainWithSub(CK_TABLE, CK_TABLENM)) {

    m_pCI->G.p_tablePtn = Card_TablePtn::pDerive(m_pCI->G.p_table);
    m_pCI->G.p_table->vAppend(m_pCI->G.p_tablePtn);

    Card_TableNm* p_tableNm = m_pCI->G.p_table->pSub_tableNm();
    Table* pTable = AM::pTableFind(p_tableNm->sz());

    m_pCI->G.p_size = Card_Size::pDerive(pTable);
    m_pCI->G.p_table->vAppend(m_pCI->G.p_size);
  }

  m_pCI->vEndTransform();
}
/*----------------------------------------------------------------------------*/
//void Xtransforms::vS20b_T_Table_Analysis(void)
//{
//  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
//
//  m_pCI->vTransform(__func__, "add a <tableSyn> to all <table>'s"_Stz);
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // Affix Table Syntax To All Table
//
//  while (m_pCI->G_bEachMainWithSub(CK_TABLE, CK_TABLENM))  {
//    m_pCI->G.p_table->vSet_Syn(&m_cTableSyn);
//  }
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // Produce Table Partition Under All Table
//
//  m_pCI->G_vRestart();
//
//  while (m_pCI->G_bEachMainWithSub(CK_TABLE, CK_TABLENM))
//    m_pCI->G.p_table->vComputeProjection(this);
//
//
//  m_pCI->vEndTransform();
//}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                  vSeries_24_Index_Analysis */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS26_T_Index_Analysis(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_pCI->vTransform(__func__, "move @c predicates to a filter under table"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Append Index List Under All Table

  while (m_pCI->G_bEachMain(CK_TABLE)) {
    m_pCI->G.p_tableNm = m_pCI->G.p_table->pSub_tableNm();
    Table* pTable = Card::pAccess_Table(m_pCI->G.p_tableNm->sz());

    m_pCI->G.p_indexList = Card_IndexList::pDerive(pTable);

    m_pCI->G_vAppendChild(CK_TABLE,CK_INDEXLIST);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Affix TblColOdn To All IdxCol

  m_pCI->G_vRestart();

  while (m_pCI->G_bEachMainWith2Sub(CK_TABLE,CK_STREAM,CK_INDEXLIST)) {
    CardIter xCI(m_pCI->G.p_indexList);
    while ((m_pCI->G.p_index = _pCard_Index(xCI.pEachChild()))) {
      m_pCI->G.p_idxColList = m_pCI->G.p_index->pSub_idxColList();
      m_pCI->G.p_idxColList->vAffix_tblColOdn(m_pCI->G.p_stream);
    }
  }

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                 vSeries_22_Stream_Analysis */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* Stream Analysis                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS22_T_Stream_Analysis(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_pCI->vTransform(__func__, "add a <stream> under every <table>"_Stz);

   m_pCI->m_pBench->vAssert_Board_any();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Produce Stream Under All Table

  vStreamAnalysisReset();

  while (m_pCI->G_bEachMainWithSub(CK_TABLE, CK_TABLENM)) {
    m_pCI->m_pTable = Card::pAccess_Table(m_pCI->G.p_tableNm->sz());
    m_pCI->G.p_stream = Card_Stream::pDerive_Of_table(m_pCI->m_pTable);

    m_pCI->G.p_table->vAppend(m_pCI->G.p_stream);
  }

//  while (m_pCI->G_bEachMainWithSub(CK_TABLE, CK_TABLENM))
//    m_pCI->G.p_table->vComputeProjection(this);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Produce Stream Under All Project

  m_pCI->G_vRestart();
  vStreamAnalysisReset();

  while (m_pCI->G_bEachMainWithSub(CK_PROJECT,CK_PROJELIST)) {
    m_pCI->G.p_stream = Card_Stream::pDerive_Of_project(m_pCI->G.p_projElist);

    m_pCI->G.p_project->vAppend(m_pCI->G.p_stream);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Produce Stream Under All Agg Reg

  m_pCI->G_vRestart();
  vStreamAnalysisReset();

  while (m_pCI->G_bEachMainWithSub(CK_AGGREG,CK_AGGELIST)) {
    m_pCI->G.p_stream = Card_Stream::pDerive_Of_aggreg(m_pCI->G.p_aggElist);

    m_pCI->G.p_aggreg->vAppend(m_pCI->G.p_stream);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Produce Stream Under All GbyAgg

  m_pCI->G_vRestart();
  vStreamAnalysisReset();

  while (m_pCI->G_bEachMainWithSub(CK_GBYAGG,CK_AGGELIST)) {
    m_pCI->G.p_stream = Card_Stream::pDerive_Of_aggreg(m_pCI->G.p_aggElist);

    m_pCI->G.p_gbyAgg->vAppend(m_pCI->G.p_stream);
  }


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Produce Stream Under All Sort

  m_pCI->G_vRestart();
  vStreamAnalysisReset();

  while (m_pCI->G_bEachMain(CK_SORT)) {

    if (m_pCI->G_bResolveSubCard(CK_STREAM,m_pCI->G.i_MainCard)) {
      m_pCI->G.p_stream = _pCard_Stream(m_pCI->G.p_stream->pDupCardAndSubCards());
      m_pCI->G.p_sort->vAppend(m_pCI->G.p_stream);
    }
  }




  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Derive Ordinal Range For All Table Stream

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_TABLE,CK_STREAM)) {
    m_pCI->G.p_stream->vDerive_OrdinalRange_table(m_pCI->G.p_table->cValue(), &m_iOdnHi, &m_iOdnLo);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Derive Ordinal Range For All Project Stream

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_PROJECT,CK_STREAM)) {
    m_pCI->G.p_stream->vDerive_OrdinalRange_project();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Derive Ordinal Range For All Agg Reg Stream

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_AGGREG,CK_STREAM)) {
    m_pCI->G.p_stream->vDerive_OrdinalRange_aggreg();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Derive Ordinal Range For All GbyAgg Stream

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMainWithSub(CK_GBYAGG,CK_STREAM)) {
    m_pCI->G.p_stream->vDerive_OrdinalRange_aggreg();
  }


  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* Projection Analysis                                                        */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS24_T_Projection_Analysis(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_pCI->vTransform(__func__, "((desc))"_Stz);

   m_pCI->m_pBench->vAssert_Board_any();


  while (m_pCI->G_bEachMain()) {

    if (CK_TABLE == m_pCI->G.e_MainCard)
      m_pCI->G.p_table->vComputeProjection(m_pCI);
  }

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                               vSeries_26_SubQuery_Analysis */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS26_T10_SubQuery_Analysis_Of_Main_Query(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCI->vTransform(__func__, "((10))"_Stz);

   m_pCI->m_pBench->vAssert_Board_mqA();

  while (m_pCI->G_bEachMain()) {
    m_pCI->G_vNewMember(CK_GENESIS,"mq"_Stz);
    if ((m_pCI->G.p_level = m_pCI->G.p_MainCard->pSub_level()))
      m_pCI->G.p_MainCard->vInsertAfter(m_pCI->G.p_genesis,m_pCI->G.p_level);
    else
      m_pCI->G.p_MainCard->vPush(m_pCI->G.p_genesis);
  }

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain(CK_AGGREG)) {
    // the main-query has an aggreg

    SubQuery* pMainQuery =  m_pCI->m_pBench->pGetBoard((i8)0)->pSubQuery();

    pMainQuery->vDisposition(QD_AGGREGATION);
  }

  m_pCI->G_vRestart();
  while (m_pCI->G_bEachMain(CK_GBYAGG)) {
    SubQuery* pMainQuery =  m_pCI->m_pBench->pGetBoard((i8)0)->pSubQuery();

    pMainQuery->vDisposition(QD_AGGREGATION);
  }

  i8 nBoards =  m_pCI->m_pBench->nItems();
  if (nBoards > 1) {
    m_pCI->G_vRestart();
    while (m_pCI->G_bEachMain(CK_CALL)) {
      SubQuery* pSubQuery =  m_pCI->m_pBench->pGetBoard((i8)1)->pSubQuery();

      pSubQuery->vCaller(0);

      pSubQuery->vClause(QC_FROM_CLAUSE);

      SubQuery* pMainQuery =  m_pCI->m_pBench->pGetBoard((i8)0)->pSubQuery();
      pMainQuery->vSubQueries(1);
    }
    m_pCI->G_vRestart();
    while (m_pCI->G_bEachMainWithSub(CK_WHERE, CK_RPNCALC)) {
      Board::vS26_T10d_Main_Query_Analysis_Of_where_rpnCalc(m_pCI);
    }
    m_pCI->G_vRestart();
    while (m_pCI->G_bEachMainWithSub(CK_HAVING, CK_RPNCALC)) {
      Board::vS26_T10e_Main_Query_Analysis_Of_having_rpnCalc(m_pCI);
    }
  }

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS26_T20_SubQuery_Analysis_Of_Sub_Query(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCI->vTransform(__func__, "((20))"_Stz);

   m_pCI->m_pBench->vAssert_Board_mqA();


  i8 nBoards =  m_pCI->m_pBench->nItems();
  if (1 == nBoards) {
    // there is no sub-query

    SubQuery* pMainQuery =  m_pCI->m_pBench->pGetBoard((i8)0)->pSubQuery();

    pMainQuery->vSubQueries(0);
  }
  else
  if (2 == nBoards) {

    if (m_pCI->G_bPushBoard(1)) {

      SubQuery* pSubQuery =  m_pCI->m_pBench->pGetBoard(1)->pSubQuery();
      pSubQuery->vDisposition(QD_LINEAR);

      while (m_pCI->G_bEachMain()) {
        m_pCI->G_vNewMember(CK_GENESIS,"sq"_Stz);
        if ((m_pCI->G.p_level = m_pCI->G.p_MainCard->pSub_level()))
          m_pCI->G.p_MainCard->vInsertAfter(m_pCI->G.p_genesis,m_pCI->G.p_level);
        else
          m_pCI->G.p_MainCard->vPush(m_pCI->G.p_genesis);
      }

      m_pCI->G_vRestart();
      while (m_pCI->G_bEachMain(CK_PARAMLIST)) {
        // there is a subQuery and it has a paramList

        SubQuery* pSubQuery =  m_pCI->m_pBench->pGetBoard(1)->pSubQuery();

        // TODO: hack the number of params... fix
        i8 nParams = 1; //m_pCI->G.p_paramList->nChildCards();

        pSubQuery->vCorrCols(nParams);
      }

      m_pCI->G_vRestart();
      while (m_pCI->G_bEachMain(CK_AGGREG)) {
        // there is a subQuery and it has an aggreg

        SubQuery* pSubQuery =  m_pCI->m_pBench->pGetBoard(1)->pSubQuery();

        pSubQuery->vDisposition(QD_AGGREGATION);
      }

      m_pCI->G_vRestart();
      while (m_pCI->G_bEachMain(CK_GBYAGG)) {
        // there is a subQuery and it has an gbyAgg
        SubQuery* pSubQuery =  m_pCI->m_pBench->pGetBoard(1)->pSubQuery();
        pSubQuery->vDisposition(QD_AGGREGATION);
      }

      m_pCI->G_vPopIter();
    }
  }
  else
    NEVER_REACH;

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                     vSeries_32_To_BoolCalc */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* SubQuery Analysis                                                          */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS32_T_SubQuery_Analysis(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_pCI->vTransform(__func__, "rpcCalc of where becomes boolCalc "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Convert SqyOp Not To NotSqyOp

  while (m_pCI->G_bEachMainWithSub(CK_WHERE, CK_RPNCALC)) {
    m_pCI->G.p_rpnCalc->vConvert_sqyOp_not_To_notSqyOp();
  }

  m_pCI->m_pCardCtl->vPrintSubCards(true,CK_RPNCALC,0);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Convert RpnCalc To Bool Calc

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

  m_pCI->m_pCardCtl->vPrintSubCards(true,CK_RPNCALC,0);

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                    vSeries_33_To_TblColOdn */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* convert all <stmColOdn> to <tblColOdn>                                     */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS33_T10_Convert_stmColOdn_To_tblColOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_pCI->vTransform(__func__, "all $n become $c.n"_Stz);

   m_pCI->m_pBench->vAssert_Board_any();

  while (m_pCI->G_bEachMain()) {

    if (! (   CK_AGGREG  == m_pCI->G.e_MainCard
           || CK_GBYAGG  == m_pCI->G.e_MainCard
           || CK_PROJECT == m_pCI->G.e_MainCard
           || CK_WHERE   == m_pCI->G.e_MainCard
        ))
      continue;

    if (CK_AGGREG == m_pCI->G.e_MainCard || CK_GBYAGG == m_pCI->G.e_MainCard || CK_PROJECT == m_pCI->G.e_MainCard) {
      m_pCI->G.p_level = m_pCI->G.p_MainCard->pSub_level();
      if (m_pCI->G.p_level && AL_PARTITION != m_pCI->G.p_level->Y_iGet(P_Level))
        continue;
    }

    Board::vS33_T10_Convert_stmColOdn_To_tblColOdn(m_pCI);
  }

  m_pCI->G.pDeck->vNoTermsStmColOdn();

  m_pCI->G.pBoard->vMayHaveStmColOdn(false);

  m_pCI->vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                vSeries_34_Predicate_Pullup */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* Predicate Pullup                                                           */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS34_T_Predicate_Pullup(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_pCI->vTransform(__func__, "put a table @c against where pred"_Stz);

   m_pCI->m_pBench->vAssert_Board_any();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // At Mark Where Pred Wrt Table

  m_pCI->vPrintBoard(__LINE__);

  while (m_pCI->G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    m_pCI->G.p_boolCalc->vMarkWrt_table(m_pCI->G.pDeck);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Move At Marked Where Pred To Table Filter

  m_pCI->G_vRestart();

  while (m_pCI->G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    m_pCI->G.p_boolCalc->vCopyMarkedPredToTable(m_pCI->G.pDeck);
  }

  m_pCI->vPrintBoard();

  m_pCI->G_vRestart();

  while (m_pCI->G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    m_pCI->G.p_boolCalc->vCullPred(MARK_AT);
  }

  m_pCI->vPrintBoard();


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Delete Empty Where

  m_pCI->G_vRestart();

  while (m_pCI->G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    if (0 == m_pCI->G.p_boolCalc->nChildCards()) {
      m_pCI->G_vRemoveDeleteMain(CK_WHERE);
    }
  }

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* Predicate Pullup                                                           */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS34_T_Predicate_Reposition(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_pCI->vTransform(__func__, "put a table @c against where pred"_Stz);

   m_pCI->m_pBench->vAssert_Board_any();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // At Mark Where Pred Wrt Table

  m_pCI->vPrintBoard(__LINE__);

  while (m_pCI->G_bEachMainWithSub(CK_TABLE, CK_BOOLCALC)) {
    m_pCI->G.p_boolCalc->vMarkWrt_table(m_pCI->G.pDeck);
  }

  m_pCI->vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Move At Marked Where Pred To Table Filter

  m_pCI->G_vRestart();

  while (m_pCI->G_bEachMainWithSub(CK_TABLE, CK_BOOLCALC)) {
    m_pCI->G.p_boolCalc->vCopyMarkedPredToTable(m_pCI->G.pDeck);
  }

  m_pCI->vPrintBoard();

  m_pCI->G_vRestart();

  while (m_pCI->G_bEachMainWithSub(CK_TABLE, CK_BOOLCALC)) {
    m_pCI->G.p_boolCalc->vCullPred(MARK_AT);
  }

  m_pCI->vPrintBoard();


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Delete Empty Where

  m_pCI->G_vRestart();

  while (m_pCI->G_bEachMainWithSub(CK_TABLE, CK_FILTER)) {
    m_pCI->G.p_table->vRemove_filter_IfEmpty(m_pCI->G.p_filter);
  }

  m_pCI->vPrintBoard();

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                  vSeries_36_Bound_Analysis */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* copy <table>/<filter> up into <table>/<bounds>                             */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS36_T10_Derive_bounds_From_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  m_pCI->vTransform(__func__, "Add <bounds> under each filtered <table>"_Stz);

   m_pCI->m_pBench->vAssert_Board_any();

  m_pCI->G.pSubQuery = m_pCI->G.pBoard->pSubQuery();

  while (m_pCI->G_bEachMainWithSub(CK_TABLE, CK_FILTER)) {
    bool bParamAllowedInBounds = false;
    if (m_pCI->G.pSubQuery->bIsCorrelated() && m_pCI->G.pSubQuery->bByExists())
      bParamAllowedInBounds = true;

    m_pCI->G.p_table->vBounds_DeriveFrom_filter(m_pCI->G.p_filter,bParamAllowedInBounds);
  }

  m_pCI->vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vStreamAnalysisReset(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_iOdnLo      = 1;
  m_iOdnHi      = C_UNKNOWN;
  //m_cTableSyn   = 'a';
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
