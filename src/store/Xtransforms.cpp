/*============================================================================*/
/* Xtransforms.cpp --                                                          */
/*============================================================================*/

#include "Xtransforms.hpp"
#include "Optimiser.hpp"

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
: Xreshape(CN_TRANSFORMS, pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  for (i8 iIndex = 0; iIndex < OT_ENUMS; iIndex++)
    m_eOptTranforms[iIndex] = OT_NONE;

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
void Xtransforms::vOptimiseLoop(void) {

  bool bOpts;
  i8 iOpts = 0;

  //vTrace("%s\n"_Stz, m_pBench->pOptimiser()->szWeave());

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

  G_vRestart();
  if (G_bEachMain(CK_SORT)) {
    Card_Order* p_order1 = G.p_sort->pSub_stream()->pSub_order();
    if (! p_order1)
      return false;

    if (G_bResolveSubCard(CK_STREAM,G.i_MainCard)) {
      Card_Order *p_order2 = G.p_stream->pSub_order();
      if (! p_order2)
        return false;

      // are the two order's the same?
      if (T_b_Order_AreSame(p_order1, p_order2)) {

        G_vRemoveDeleteMain(CK_SORT);
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
  // and then we can change the aggreg to an ordAgg.
  //
  // Also Note: when the ordAgg replaces the aggreg at an all partitions level,
  //  we can delete the aggregates on all nodes and on origin.
  //  This further aggregation is not required.

  G.pDeck->vRenumberRecursive();

  vPrintBoard(__LINE__);

  G_vRestart();
  if (G_bEachMain(CK_GBYAGG)) {
    Card_Order* p_order1 = G.p_gbyAgg->pSub_stream()->pSub_order();

    if (G_bResolveSubCard(CK_STREAM,G.i_MainCard)) {
      Card_Order *p_order2 = G.p_stream->pSub_order();
      if (! p_order2)
        return false;

      p_order1->vDump(0,"ORDER1"_Stz);
      p_order2->vDump(0,"ORDER2"_Stz);

      // are the two order's the same?
      if (T_b_Order_AreSame(p_order1, p_order2)) {
        G.p_gbyAgg->vCard(CK_ORDAGG);

        vPrintBoard(__LINE__);

        RASSERT(G_bEachMain(CK_GBYAGG));
        G_vRemoveDeleteMain(CK_GBYAGG);

        vPrintBoard(__LINE__);

        RASSERT(G_bEachMain(CK_GBYAGG));
        G_vRemoveDeleteMain(CK_GBYAGG);

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

//  G_vRestart();
//  if (G_bEachMainWithSub(CK_SCAN,CK_BOUNDS)) {
//    if (G.p_bounds->pSub_lBound() || G.p_bounds->pSub_uBound()) {
//      G.p_scan->vCard(CK_INDEXSCAN);
//
//      vPrintBoard(__LINE__);
//
//      vS90_T30_Derive_order();
//
//      vPrintBoard(__LINE__);
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

  vST00_Begin();

  m_pBench->vPrefix('r');
  m_pCardCtl->vUseExpanded();
  vPrintBench(BK_SUBPIPE);
  m_pCardCtl->vUseExpanded();
}

/*----------------------------------------------------------------------------*/
/* Condense Info                                                              */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS10_T_Condense_Info(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "delete superfluous <absJoin>'s"_Stz);

  vAssert_Board_any();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Delete All AbsJoin

  while (G_bEachMain(CK_ABSJOIN)) {
    G_vRemoveDeleteMain(CK_ABSJOIN);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Delete All Combined

  G_vRestart();
  while (G_bEachMain(CK_COMBINED)) {
    G_vRemoveDeleteMain(CK_COMBINED);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Delete All Send

  G_vRestart();
  while (G_bEachMain(CK_SEND)) {
    G_vRemoveDeleteMain(CK_SEND);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Delete All Stage

  G_vRestart();
  while (G_bEachMain(CK_STAGE)) {
    G_vRemoveDeleteMain(CK_STAGE);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Cull Scan Leaving Table

  G_vRestart();
  while (G_bEachMain(CK_SCAN)) {
    G.pDeck->vReplaceChildWithItsSubs(G.p_scan);
    DELETE_CARD(G.p_scan);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Add All Level

  G_vRestart();
  G.e_Level = AL_PARTITION;
  while (G_bEachMain()) {

    G.p_level = G.p_MainCard->pSub_level();
    if (G.p_level)
      continue;

    G.p_level = new Card_Level(AL_PARTITION);
    G.p_MainCard->vPush(G.p_level);
  }

  //
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vEndTransform();
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

  vTransform(__func__, "add a <tableSyn> to all <table>'s"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Affix Table Syntax To All Table

  while (G_bEachMainWithSub(CK_TABLE, CK_TABLENM))  {
    M_vTable_Set_Syn(G.p_table);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Produce Table Partition Under All Table

  G_vRestart();

  while (G_bEachMainWithSub(CK_TABLE, CK_TABLENM)) {
    G.p_tablePtn = Card_TablePtn::pDerive(G.p_table);

    G.p_table->vAppend(G.p_tablePtn);
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                  vSeries_24_Index_Analysis */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS24_T_Index_Analysis(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "move @c predicates to a filter under table"_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Append Index List Under All Table

  while (G_bEachMain(CK_TABLE)) {
    G.p_tableNm = G.p_table->pSub_tableNm();
    Table* pTable = A_pAccess_Table(G.p_tableNm->sz());

    G.p_indexList = Card_IndexList::pDerive(pTable);

    G_vAppendChild(CK_TABLE,CK_INDEXLIST);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Affix TblColOdn To All IdxCol

  G_vRestart();

  while (G_bEachMainWith2Sub(CK_TABLE,CK_STREAM,CK_INDEXLIST)) {
    CardIter xCI(G.p_indexList);
    while ((G.p_index = _pCard_Index(xCI.pEachChild()))) {
      G.p_idxColList = G.p_index->pSub_idxColList();
      M_vIdxColList_Affix_tblColOdn(G.p_idxColList, G.p_stream);
    }
  }

  vEndTransform();
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

  vTransform(__func__, "add a <stream> under every <table>"_Stz);

  vAssert_Board_any();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Produce Stream Under All Table

  vStreamAnalysisReset();

  while (G_bEachMainWithSub(CK_TABLE, CK_TABLENM)) {
    m_pTable = A_pAccess_Table(G.p_tableNm->sz());
    G.p_stream = Card_Stream::pDerive_Of_table(m_pTable);

    G.p_table->vAppend(G.p_stream);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Produce Stream Under All Project

  G_vRestart();
  vStreamAnalysisReset();

  while (G_bEachMainWithSub(CK_PROJECT,CK_PROJELIST)) {
    G.p_stream = Card_Stream::pDerive_Of_project(G.p_projElist);

    G.p_project->vAppend(G.p_stream);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Produce Stream Under All Agg Reg

  G_vRestart();
  vStreamAnalysisReset();

  while (G_bEachMainWithSub(CK_AGGREG,CK_AGGELIST)) {
    G.p_stream = Card_Stream::pDerive_Of_aggreg(G.p_aggElist);

    G.p_aggreg->vAppend(G.p_stream);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Produce Stream Under All GbyAgg

  G_vRestart();
  vStreamAnalysisReset();

  while (G_bEachMainWithSub(CK_GBYAGG,CK_AGGELIST)) {
    G.p_stream = Card_Stream::pDerive_Of_aggreg(G.p_aggElist);

    G.p_gbyAgg->vAppend(G.p_stream);
  }


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Produce Stream Under All Sort

  G_vRestart();
  vStreamAnalysisReset();

  while (G_bEachMain(CK_SORT)) {

    if (G_bResolveSubCard(CK_STREAM,G.i_MainCard)) {
      G.p_stream = _pCard_Stream(G.p_stream->pDupCardAndSubCards());
      G.p_sort->vAppend(G.p_stream);
    }
  }




  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Derive Ordinal Range For All Table Stream

  G_vRestart();
  while (G_bEachMainWithSub(CK_TABLE,CK_STREAM)) {
    M_vStream_Derive_OrdinalRange_table(G.p_stream, G.p_table->cValue());
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Derive Ordinal Range For All Project Stream

  G_vRestart();
  while (G_bEachMainWithSub(CK_PROJECT,CK_STREAM)) {
    M_vStream_Derive_OrdinalRange_project(G.p_stream);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Derive Ordinal Range For All Agg Reg Stream

  G_vRestart();
  while (G_bEachMainWithSub(CK_AGGREG,CK_STREAM)) {
    M_vStream_Derive_OrdinalRange_aggreg(G.p_stream);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Derive Ordinal Range For All GbyAgg Stream

  G_vRestart();
  while (G_bEachMainWithSub(CK_GBYAGG,CK_STREAM)) {
    M_vStream_Derive_OrdinalRange_aggreg(G.p_stream);
  }


  vEndTransform();
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

  vTransform(__func__, "((10))"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMain()) {
    G_vNewMember(CK_GENESIS,"mq"_Stz);
    if ((G.p_level = G.p_MainCard->pSub_level()))
      G.p_MainCard->vInsertAfter(G.p_genesis,G.p_level);
    else
      G.p_MainCard->vPush(G.p_genesis);
  }

  G_vRestart();
  while (G_bEachMain(CK_AGGREG)) {
    // the main-query has an aggreg

    SubQuery* pMainQuery = m_pBench->pGetBoard((i8)0)->pSubQuery();

    pMainQuery->vDisposition(QD_AGGREGATION);
  }

  G_vRestart();
  while (G_bEachMain(CK_GBYAGG)) {
    SubQuery* pMainQuery = m_pBench->pGetBoard((i8)0)->pSubQuery();

    pMainQuery->vDisposition(QD_AGGREGATION);
  }

  i8 nBoards = m_pBench->nItems();
  if (nBoards > 1) {
    G_vRestart();
    while (G_bEachMain(CK_CALL)) {
      SubQuery* pSubQuery = m_pBench->pGetBoard((i8)1)->pSubQuery();

      pSubQuery->vCaller(0);

      pSubQuery->vClause(QC_FROM_CLAUSE);

      SubQuery* pMainQuery = m_pBench->pGetBoard((i8)0)->pSubQuery();
      pMainQuery->vSubQueries(1);
    }
    G_vRestart();
    while (G_bEachMainWithSub(CK_WHERE, CK_RPNCALC)) {
      vR_S26_T10d_Main_Query_Analysis_Of_where_rpnCalc();
    }
    G_vRestart();
    while (G_bEachMainWithSub(CK_HAVING, CK_RPNCALC)) {
      vR_S26_T10e_Main_Query_Analysis_Of_having_rpnCalc();
    }
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS26_T20_SubQuery_Analysis_Of_Sub_Query(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((20))"_Stz);

  vAssert_Board_mqA();


  i8 nBoards = m_pBench->nItems();
  if (1 == nBoards) {
    // there is no sub-query

    SubQuery* pMainQuery = m_pBench->pGetBoard((i8)0)->pSubQuery();

    pMainQuery->vSubQueries(0);
  }
  else
  if (2 == nBoards) {

    if (G_bPushBoard(1)) {

      SubQuery* pSubQuery = m_pBench->pGetBoard(1)->pSubQuery();
      pSubQuery->vDisposition(QD_LINEAR);

      while (G_bEachMain()) {
        G_vNewMember(CK_GENESIS,"sq"_Stz);
        if ((G.p_level = G.p_MainCard->pSub_level()))
          G.p_MainCard->vInsertAfter(G.p_genesis,G.p_level);
        else
          G.p_MainCard->vPush(G.p_genesis);
      }

      G_vRestart();
      while (G_bEachMain(CK_PARAMLIST)) {
        // there is a subQuery and it has a paramList

        SubQuery* pSubQuery = m_pBench->pGetBoard(1)->pSubQuery();

        // TODO: hack the number of params... fix
        i8 nParams = 1; //G.p_paramList->nChildCards();

        pSubQuery->vCorrCols(nParams);
      }

      G_vRestart();
      while (G_bEachMain(CK_AGGREG)) {
        // there is a subQuery and it has an aggreg

        SubQuery* pSubQuery = m_pBench->pGetBoard(1)->pSubQuery();

        pSubQuery->vDisposition(QD_AGGREGATION);
      }

      G_vRestart();
      while (G_bEachMain(CK_GBYAGG)) {
        // there is a subQuery and it has an gbyAgg
        SubQuery* pSubQuery = m_pBench->pGetBoard(1)->pSubQuery();
        pSubQuery->vDisposition(QD_AGGREGATION);
      }

      G_vPopBoard();
    }
  }
  else
    NEVER_REACH;

  vEndTransform();
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

  vTransform(__func__, "rpcCalc of where becomes boolCalc "_Stz);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Convert SqyOp Not To NotSqyOp

  while (G_bEachMainWithSub(CK_WHERE, CK_RPNCALC)) {
    M_vRpnCalc_Convert_sqyOp_not_To_notSqyOp(G.p_rpnCalc);
  }

  m_pCardCtl->vPrintSubCards(true,CK_RPNCALC,0);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Convert RpnCalc To Bool Calc

  G_vRestart();
  while (G_bEachMainWithSub(CK_WHERE, CK_RPNCALC)) {
    G.p_boolCalc = C_pConvert_rpnCalc_To_boolCalc(G.p_rpnCalc);

    G.p_where->vReplaceSubCard(G.p_rpnCalc,G.p_boolCalc);

    G_vDeleteMember(G.p_rpnCalc);
  }

  G_vRestart();
  while (G_bEachMainWithSub(CK_HAVING, CK_RPNCALC)) {
    G.p_boolCalc = C_pConvert_rpnCalc_To_boolCalc(G.p_rpnCalc);

    G.p_having->vReplaceSubCard(G.p_rpnCalc,G.p_boolCalc);

    G_vDeleteMember(G.p_rpnCalc);
  }

  m_pCardCtl->vPrintSubCards(true,CK_RPNCALC,0);

  vEndTransform();
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

  vTransform(__func__, "all $n become $c.n"_Stz);

  vAssert_Board_any();

  while (G_bEachMain()) {

    if (! (   CK_PROJECT == G.e_MainCard
              || CK_WHERE   == G.e_MainCard
    ))
      continue;

    if (CK_PROJECT == G.e_MainCard) {
      G.p_level = G.p_MainCard->pSub_level();
      if (G.p_level && AL_PARTITION != G.p_level->Y_iGet(P_Level))
        continue;
    }

    vR_S33_T10_Convert_stmColOdn_To_tblColOdn();
  }

  V_vNo_Terms_StmColOdn(G.pDeck);

  G.pBoard->vMayHaveStmColOdn(false);

  vEndTransform();
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

  vTransform(__func__, "put a table @c against where pred"_Stz);

  vAssert_Board_any();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // At Mark Where Pred Wrt Table

  vPrintBoard(__LINE__);

  while (G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    M_vBoolCalc_MarkWrt_table(G.p_boolCalc, G.pDeck);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Move At Marked Where Pred To Table Filter

  G_vRestart();

  while (G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    vDeck_CopyMarkedPredLeftwards(G.p_boolCalc, G.pDeck);
  }

  vPrintBoard();

  G_vRestart();

  while (G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    M_vBoolCalc_CullPred(G.p_boolCalc, MARK_AT);
  }

  vPrintBoard();


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Delete Empty Where

  G_vRestart();

  while (G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    if (0 == G.p_boolCalc->nChildCards()) {
      G_vRemoveDeleteMain(CK_WHERE);
    }
  }

  vEndTransform();
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

  vTransform(__func__, "Add <bounds> under each filtered <table>"_Stz);

  vAssert_Board_any();

  G.pSubQuery = G.pBoard->pSubQuery();

  while (G_bEachMainWithSub(CK_TABLE, CK_FILTER)) {
    bool bParamAllowedInBounds = false;
    if (G.pSubQuery->bIsCorrelated() && G.pSubQuery->bByExists())
      bParamAllowedInBounds = true;

    C_vBounds_DeriveFrom_filter(G.p_table, G.p_filter,bParamAllowedInBounds);
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                   vSeries_40_Flatten_Query */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS40_T05_Flatten_By_Decision_Tree(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (1 == m_pBench->nItems())
    return;

  vTransform(__func__, "((10))"_Stz);

  vAssert_Board_mqA();

  G.pSubQuery = m_pBench->pGetBoard(C_SQ_BOARD)->pSubQuery();

  if (G.pSubQuery->bViaFrom())                /* Fsq_       Q7,8,9 */ vS40_T10_Fold_F();
  else
  if (G.pSubQuery->bViaWhere()) {
    if (G.pSubQuery->bIsAggregation()) {
      if (G.pSubQuery->bIsCorrelated()) {
             if (G.pSubQuery->bByExists())    /* W_A_C_E          */ NEVER_REACH
        else if (G.pSubQuery->bByNotExists()) /* W_A_C_NE         */ NEVER_REACH
        else if (G.pSubQuery->bByIn())        /* W_A_C_I          */ NEVER_REACH
        else if (G.pSubQuery->bByNotIn())     /* W_A_C_NI         */ NEVER_REACH
        else                                  /* W_A_C_ro   Q2,17 */ vS40_T20_Fold_W_A_C_ro();
      } else {
             if (G.pSubQuery->bByExists())    /* W_A_NC_E         */ NEVER_REACH
        else if (G.pSubQuery->bByNotExists()) /* W_A_NC_NE        */ NEVER_REACH
        else if (G.pSubQuery->bByIn())        /* W_A_NC_I     Q18 */ vS40_T30_Fold_W_A_NC_I();
        else if (G.pSubQuery->bByNotIn())     /* W_A_NC_NI        */ NEVER_REACH
        else                                  /* W_A_NC_ro    Q15 */ vS40_T40_Fold_W_A_NC_ro();
      }
    } else {
      if (G.pSubQuery->bIsCorrelated()) {
             if (G.pSubQuery->bByExists())    /* W_L_C_E       Q4 */ vS40_T50_Fold_W_L_C_E();
        else if (G.pSubQuery->bByNotExists()) /* W_L_C_NE         */ NEVER_REACH
        else if (G.pSubQuery->bByIn())        /* W_L_C_I          */ NEVER_REACH
        else if (G.pSubQuery->bByNotIn())     /* W_L_C_NI         */ NEVER_REACH
        else                                  /* W_L_C_ro         */ NEVER_REACH
      } else {
             if (G.pSubQuery->bByExists())    /* W_L_NC_E         */ NEVER_REACH
        else if (G.pSubQuery->bByNotExists()) /* W_L_NC_NE        */ NEVER_REACH
        else if (G.pSubQuery->bByIn())        /* W_L_NC_I         */ NEVER_REACH
        else if (G.pSubQuery->bByNotIn())     /* W_L_NC_NI    Q16 */ vS40_T60_Fold_W_L_NC_NI();
        else                                  /* W_L_NC_ro        */ NEVER_REACH
      }
    }
  }
  else
  if (G.pSubQuery->bViaHaving()) {
    if (G.pSubQuery->bIsAggregation()) {
      if (G.pSubQuery->bIsCorrelated()) {
             if (G.pSubQuery->bByExists())    /* H_A_C_E          */ NEVER_REACH
        else if (G.pSubQuery->bByNotExists()) /* H_A_C_NE         */ NEVER_REACH
        else if (G.pSubQuery->bByIn())        /* H_A_C_I          */ NEVER_REACH
        else if (G.pSubQuery->bByNotIn())     /* H_A_C_NI         */ NEVER_REACH
        else                                  /* H_A_C_ro         */ NEVER_REACH
      } else {
             if (G.pSubQuery->bByExists())    /* H_A_NC_E         */ NEVER_REACH
        else if (G.pSubQuery->bByNotExists()) /* H_A_NC_NE        */ NEVER_REACH
        else if (G.pSubQuery->bByIn())        /* H_A_NC_I         */ NEVER_REACH
        else if (G.pSubQuery->bByNotIn())     /* H_A_NC_NI        */ NEVER_REACH
        else                                  /* H_A_NC_ro    Q11 */ vS40_T70_Fold_H_A_NC_ro();
      }
    } else {
      if (G.pSubQuery->bIsCorrelated()) {
             if (G.pSubQuery->bByExists())    /* H_L_C_E          */ NEVER_REACH
        else if (G.pSubQuery->bByNotExists()) /* H_L_C_NE         */ NEVER_REACH
        else if (G.pSubQuery->bByIn())        /* H_L_C_I          */ NEVER_REACH
        else if (G.pSubQuery->bByNotIn())     /* H_L_C_NI         */ NEVER_REACH
        else                                  /* H_L_C_ro         */ NEVER_REACH
      } else {
             if (G.pSubQuery->bByExists())    /* H_L_NC_E         */ NEVER_REACH
        else if (G.pSubQuery->bByNotExists()) /* H_L_NC_NE        */ NEVER_REACH
        else if (G.pSubQuery->bByIn())        /* H_L_NC_I         */ NEVER_REACH
        else if (G.pSubQuery->bByNotIn())     /* H_L_NC_NI        */ NEVER_REACH
        else                                  /* H_L_NC_ro        */ NEVER_REACH
      }
    }
  }
  else
    NEVER_REACH;

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS40_T10_Fold_F(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  // F
  // push a (from) sub-query before the main-query
  // as an op-sequence
  // (refer: Q7,8,9)

  /*
      select count(*)
        from (select *
                from orders, lineitem
               where L_ORDERKEY = O_ORDERKEY)
      ;
   */

  vTransform(__func__, "((20))"_Stz);

  vAssert_Board_mqA();
  vPrintBoard(__LINE__);

  // 1) find W_A_C_ro subquery call in mq:<where>/<boolCalc>
  vAssert_Board_mqA();
  RASSERT(G_bEachMain(CK_CALL));
  vTrace(">>>>>>>>> Found From CALl: %s\n"_Stz,G.p_call->szValue());
  vPrintBoard(__LINE__);


  G_vNewMember(CK_FOLD,"FROM"_Stz);
  G.pDeck->vPush(G.p_fold);

  vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get copy of sq and push before mq

  // x) push to sq
  if (! G_bPushBoard(G.p_call->szValue())) {
    vTrace("DID NOT FIND Board %s\n"_Stz,G.p_call->szValue());
    NEVER_REACH
  }
  vAssert_Board_sqB();

  m_pCard_copied = E_pSubset_ChildrenBetween(G.pDeck, 0);

  vPrintBoard(__LINE__);

  // ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^

  // x) pop back to mq
  G_vPopBoard();
  vAssert_Board_mqA();

  G.pDeck->vPush(m_pCard_copied);
  G.pDeck->vReplaceChildWithItsSubs(m_pCard_copied);
  DELETE_CARD(m_pCard_copied);

  G.pDeck->vRemoveChild(G.p_call);
  DELETE_CARD(G.p_call);

  G.pDeck->vRenumberRecursive();

  vPrintBoard(__LINE__);

  R_vUTE_AdjustStmLoHi();
  vPrintBoard(__LINE__);

  R_vUTE_RenumberStmOdn();
  vPrintBoard(__LINE__);

  R_vUTE_RelabelExpressions();

  m_pBench->vIsFlat(true);
  G.pDeck->vClearMarksRecursive();

  vPrintBoard(__LINE__);


  vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS40_T20_Fold_W_A_C_ro(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  // W A C ro
  // push a (single-value aggregate correlated ro) sub-query before the main-query
  // as an aggregation-fan (projecting key-of-correlation and aggregated value)
  // further: the key-of-correlation needs to be added to group-by)
  // (refer: Q2,17)

  /* ARCHETYPE :
    select count(*)
      from Lineitem, Part, PartSupp
     where P_PARTKEY  = L_PARTKEY
       and PS_PARTKEY = P_PARTKEY
       and l_quantity < (select avg(l_quantity)
                           from lineitem
                          where L_PARTKEY = P_PARTKEY)
    ;
 */

  vTransform(__func__, "((20))"_Stz);

  // 1) find W_A_C_ro subquery call in mq:<where>/<boolCalc>
  vAssert_Board_mqA();
  RASSERT(G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC));
  if (!R_bUTE_FindAndMarkSubQryCall()) {
    vTrace("DID NOT FIND W_A_C_ro SubQuery call\n"_Stz);
    NEVER_REACH
  }
  vTrace(">>>>>>>>> Found W_A_C_ro call:  %s %s %s\n"_Stz,m_sSqyArg.szStz(),m_sSqyRO.szStz(),m_sSqyNm.szStz());
  vPrintBoard(__LINE__);


  G_vNewMember(CK_FOLD,"W_A_C_ro"_Stz);
  G.pDeck->vPush(G.p_fold);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get copy of sq and push before mq

  // x) push to sq
  if (! G_bPushBoard(m_sSqyNm)) {
    vTrace("DID NOT FIND Board %s\n"_Stz,m_sSqyNm.szStz());
    NEVER_REACH
  }
  vAssert_Board_sqB();

  m_pCard_copied = E_pSubset_ChildrenBetween(G.pDeck, 1);

  vPrintBoard(__LINE__);

  // ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^  ^

  // x) pop back to mq
  G_vPopBoard();
  vAssert_Board_mqA();

  G.pDeck->vPush(m_pCard_copied);
  G.pDeck->vReplaceChildWithItsSubs(m_pCard_copied);
  DELETE_CARD(m_pCard_copied);

  G.pDeck->vRenumberRecursive();

  vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // convert the parameterised aggreg-sq into a gbyAgg-sq gby correlation column
  // such that the result of the sq is a stream { __SQ1IGBKY , __SQ1RESULT }

  G_vRestart();

  // v                             //  ]filter
  // T A A A P F T T T W A A A     //  .]boolCalc
  // s                             //  ..]cmpPred ==
  // f                             //           $d.2  ((m_pTerm_corrPred))
  //                               //             %1
  RASSERT(G_bEachMainWith2Sub(CK_TABLE,CK_STREAM,CK_FILTER));
  R_vUTE_Identify_Correlation_Column_In_cmpPred_Of_sq_boolCalc();
  RASSERT(m_pTerm_corrPred); // eg $d.2 (L_PARTKEY)
  R_vUTE_Find_stmCol_Definition_Of_Correlation_Column();
  RASSERT(G.p_stmCol);       // eg L_PARTKEY


  // T A A A P F T T T W A A A     //
  //   v v v                       //
  // T G G G P F T T T W A A A     // push corrPred <expr> into <gbyElist> and <aggElist>

  RASSERT(G_bEachMain(CK_AGGREG));
  R_vUTE_Enrich_sq_aggreg_ToBe_gbyAgg_Gby_Correlation_Column();

  vPrintBoard(__LINE__);

  // T A A A P F T T T W A A A     //
  //         v                     //
  // T G G G P F T T T W A A A     // push corrPred <expr> into <projElist>

  if (G_bIsNextMain(CK_PROJECT)) {
    RASSERT(G_bEachMain(CK_PROJECT));
    R_vUTE_Enhance_sq_project_To_Include_Correlation_Column();
  }
  else {

  }
  RASSERT(G_bEachMain(CK_FOLD));
  R_vUTE_Derive_foldElist_From_project_stream();

  G.pDeck->vRenumberRecursive();
  R_vUTE_RelabelExpressions();
  vPrintBoard(__LINE__);

  // T G G G P F T T T W A A A      //
  //             s s s              // adjust stream ordinsla

  R_vUTE_AdjustStmLoHi();
  R_vUTE_RenumberStmOdn();
  vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // convert the mq to join to the gby correlation column streamed from sq

  G_vRestart();

  // v                             //  ]filter
  // T A A A P F T T T W A A A     //  .]boolCalc
  // s                             //  ..]cmpPred ==
  // f                             //           $d.2  ((m_pTerm_corrPred))
  //                               //             %1

  RASSERT(G_bEachMainWith2Sub(CK_TABLE,CK_FILTER,CK_BOOLCALC))
  R_vUTE_Yank_Correlated_cmpPred_From_sq_boolCalc(G.p_boolCalc,/*r*/G.p_PRED);
  RASSERT(G.p_PRED->Y_bHas(P_CorrPred));
  vPrintBoard(__LINE__);

  R_vUTE_Remove_filter_IfEmpty(G.p_table, G.p_filter);
  vPrintBoard(__LINE__);

  //                   v           //  ]filter
  // T A A A P F T T T W A A A     //  .]boolCalc
  // s                 f           //  ..]cmpPred ==
  // f                 bc          //           $d.2  ((m_pTerm_corrPred))
  //                               //             %1

  RASSERT(G_bEachMain(CK_FOLD));
  RASSERT(G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC));
  R_vUTE_Push_Correlated_cmpPred_Onto_mq_boolCalc(G.p_boolCalc, G.p_PRED);
  vPrintBoard(__LINE__);

  //                   v           //  ]filter
  // T A A A P F T T T W A A A     //  .]boolCalc
  // s                 f           //  ..]roPred <
  // f                 bc          //           $a.5
  //                               //           $b.1
  //                               // __subquery0002

  R_vUTE_Resolve_SubqueryCall_In_mq_boolCalc_Into_cmpPred(G.p_boolCalc);
  vPrintBoard(__LINE__);

  RASSERT(0 == m_pTransformIterList->nItems());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // move the correlation predicate to table as a bounds

  // TODO: this should be in two steps

  G_vRestart();

  //                   v           //  ]filter
  // T A A A P F T T T W A A A     //  .]boolCalc
  // s                 f           //  ..]cmpPred ==
  // f                 bc          //           $d.2  ((m_pTerm_corrPred))
  //             b <-  cp          //             %1

  RASSERT(G_bEachMain(CK_FOLD));
  RASSERT(G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC));
  R_vUTE_Move_corrPred_To_mq_table_As_bounds(G.p_boolCalc);
  vPrintBoard(__LINE__);


//  G_vRestart();
//  if (G_bEachMain(CK_FOLD)) {
//    if (G_bEachMain(CK_WHERE)) {
//      // TODO:
//      // R_vUTE_Unproject_foldExpr_After_where();
//      R_vUTE_Bump_where_BaseOn_foldExpr();
//    }
//  }

  m_pBench->vIsFlat(true);
  G.pDeck->vClearMarksRecursive();
  vPrintBoard(__LINE__);
  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS40_T30_Fold_W_A_NC_I(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  // W A NC I
  // push a (single-value aggregate non-correlated in) sub-query before the main-query
  // as an aggregation-fan (projecting a single aggregated value)
  // (refer: Q18)

  /* ARCHETYPE :
      select count(*)
        from orders, lineitem
       where O_ORDERKEY = L_ORDERKEY
         and O_ORDERKEY in (select L_ORDERKEY
                              from lineitem
                             group by L_ORDERKEY
                            having sum(l_quantity) > 100)
      ;
   */

  vTransform(__func__, "((30))"_Stz);

  vAssert_Board_mqA();


  // W A NC I
  //               //
  // T T W A A A   // find W_L_NC_I subquery call in mq:<where>/<boolCalc>
  //     f         //
  //     bc        //
  //    @ro        //

  vAssert_Board_mqA();
  if (G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    if (!R_bUTE_FindAndMarkSubQryCall()) {
      vTracer("DID NOT FIND W_A_NC_I SubQuery call\n"_Stz);
      NEVER_REACH
    }
  }
  vTrace("Found W_A_NC_I call to %s(%s)->%s\n"_Stz,m_sSqyNm.szStz(),m_sSqyArg.szStz(),m_sSqyRsl.szStz());

  G_vNewMember(CK_FOLD,"W_A_NC_I"_Stz);
  G.pDeck->vPush(G.p_fold);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // x) push to sq
  if (! G_bPushBoard(m_sSqyNm)) {
    vTrace("DID NOT FIND Board %s\n"_Stz,m_sSqyNm.szStz());
    NEVER_REACH
  }
  vAssert_Board_sqB();

  m_pCard_yanked = E_pSubset_ChildrenBetween(G.pDeck, 0);

  vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // x) pop back to mq
  G_vPopBoard();
  vAssert_Board_mqA();

  // W A NC I
  //       I-sqr-> $1            // Folded nc sq provides sqr into mq as $1
  //               ----mq-----   //
  // T G G G H P F T T W A A A   // push sq before Fold
  // ----sq-----                 //

  G.pDeck->vPush(m_pCard_yanked);
  G.pDeck->vReplaceChildWithItsSubs(m_pCard_yanked);
  DELETE_CARD(m_pCard_yanked);

  vPrintBoard(__LINE__);

  G_vRestart();

  // W A NC I
  //       I-sqr-> $1            // Put an expr describing sqr into Fold
  //           v v               //
  // T G G G H P F T T W A A A   // derive foldElist from sq Project
  //           s el              //

  RASSERT(G_bEachMainWithSub(CK_PROJECT,CK_STREAM));
  RASSERT(G_bEachMain(CK_FOLD));
  R_vUTE_Derive_foldElist_From_project_stream();
  vPrintBoard(__LINE__);

  // W A NC I
  //       I-sqr-> $1            // Injection of sqr into mq adds new $1
  //                     v v v   //
  // T G G G H P F T T W G G G   // bump stmOdn:$n in mq gbyElist by 1
  //                     elelel  // (if G is actually G -- might be A)

  if (G_bEachMainWithSub(CK_GBYAGG,CK_GBYELIST)) {
    i8 nBumpRight = G.p_fold->Y_iGet(P_BumpRight);
    R_vUTE_Bump_stmOdn_In_rpnCalc_Of_Elist(G.p_gbyElist);

    G.p_fold->Y_vCut(P_BumpRight);
    G.p_fold->Y_vSet(P_BumpedRight,(nBumpRight));
  }

  // W A NC I
  //       I-sqr-> $1            // The mq IN transforms to arg == $1 (the corr pred)
  //                             //
  // T G G G H P F T T W G G G   // find table for sqy arg (will be an mq table)
  //               f             // derive cmpPred from sqy arg
  //               bc            // add cmpPred to table
  //               cp  @ro       //

  char cTblSyn = E_cValue_TableColSyn(m_sSqyArg);
  G.p_table = G.pDeck->pFindTableCard(cTblSyn, G.p_MainCard);

  G_vNewMember(CK_CMPPRED,"=="_Stz);
  G_vNewMember(CK_TERM,m_sSqyArg.szStz());
  G.p_cmpPred->vAppend(G.p_term);
  //G_vNewMember(CK_TERM,m_sSqyRsl.szStz());
  G_vNewMember(CK_TERM,"$1"_Stz);
  G.p_cmpPred->vAppend(G.p_term);

  R_vUTE_AddPredToTableFilter(G.p_table,G.p_cmpPred);
  vPrintBoard(__LINE__);

  // W A NC I
  //       I-sqr-> $1            // The corr pred in mq is a new bounds
  //                             //
  // T G G G H P F T T W G G G   // derive bounds from filter
  //               b             //
  //               ^             //
  //               f             //
  //               bc            //
  //              @cp            //

  C_vBounds_DeriveFrom_filter(G.p_table,G.p_filter,false);
  vPrintBoard(__LINE__);

  // W A NC I
  //       I-sqr-> $1            // Moved corr pred can be culled
  //                             //
  // T G G G H P F T T W G G G   // cull @cp from filter
  //               b             //
  //               f             //
  //               bc            //

  G.p_boolCalc = G.p_filter->pSub_boolCalc();
  M_vBoolCalc_CullPred(G.p_boolCalc, MARK_UP);
  vPrintBoard(__LINE__);

  // W A NC I
  //       I-sqr-> $1            // Culled corr pred may leave filter empty
  //                             //
  // T G G G H P F T T W G G G   // delete empty filter
  //               b             //

  vR_S36_T30_Delete_Empty_filter();

  // W A NC I
  //       I-sqr-> $1            // The original ro pred can now be removed
  //                             //
  // T G G G H P F T T W A A A   // cull the ro pred
  //                   f         //
  //                   bc        //
  //                  @ro (cull) //

  G_vRestart();
  if (G_bEachMainWithSub(CK_WHERE,CK_BOOLCALC)) {
    M_vBoolCalc_CullPred(G.p_boolCalc, MARK_AT);
  }

  vPrintBoard(__LINE__);

  // W A NC I
  //       I-sqr-> $1            // Removal of ro pred may leave Where empty
  //                             //
  // T G G G H P F T T W A A A   // delete potentially empty where
  //                   f         //
  //                   bc        //
  //                 (empty)     //

  G_vRestart();
  G_vDeleteWhenEmptySub(CK_WHERE,CK_BOOLCALC);

  vPrintBoard(__LINE__);


  // todo: should this be earlier? As soon as $1 is injected into mq??

  R_vUTE_AdjustStmLoHi();
  vPrintBoard(__LINE__);

  R_vUTE_RenumberStmOdn();
  vPrintBoard(__LINE__);

  R_vUTE_RelabelExpressions();

  m_pBench->vIsFlat(true);
  G.pDeck->vClearMarksRecursive();

  vPrintBoard(__LINE__);

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS40_T40_Fold_W_A_NC_ro(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  // W A NC ro
  // push a (single-value aggregate non-correlated in) sub-query before the main-query
  // as an aggregation-fan (projecting a single aggregated value)
  // (refer: Q15)

  /*
   */

  vTransform(__func__, "((30))"_Stz);

  vAssert_Board_mqA();


  // 1) find W_L_C_E subquery call in mq:<where>/<boolCalc>
  vAssert_Board_mqA();
  if (G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    if (!R_bUTE_FindAndMarkSubQryCall()) {
      vTracer("DID NOT FIND W_A_NC_ro SubQuery call\n"_Stz);
      NEVER_REACH
    }
  }
  vTrace("Found W_A_NC_ro call to %s()->%s\n"_Stz,m_sSqyNm.szStz(),m_sSqyRsl.szStz());

  G_vNewMember(CK_FOLD,"W_A_NC_I"_Stz);
  G.pDeck->vPush(G.p_fold);

  /*
  // find table for sqyarg
  char cTblSyn = E_cValue_TableColSyn(m_sSqyArg);
  G.p_table = G.pDeck->pFindTableCard(cTblSyn, G.p_MainCard);

  // TODO: derive cmpPred from marked tstPred and add to corresponding table

  G_vNewMember(CK_CMPPRED,"=="_Stz);
  G_vNewMember(CK_TERM,m_sSqyArg.szStz());
  G.p_cmpPred->vAppend(G.p_term);
  //G_vNewMember(CK_TERM,m_sSqyRsl.szStz());
  G_vNewMember(CK_TERM,"$1"_Stz);
  G.p_cmpPred->vAppend(G.p_term);

  R_vUTE_AddPredToTableFilter(G.p_table,G.p_cmpPred);
  vPrintBoard(__LINE__);

  G_vRestart();
  if (G_bEachMainWithSub(CK_WHERE,CK_BOOLCALC)) {
    M_vBoolCalc_CullPred(G.p_boolCalc, MARK_AT);
  }

  R_vUTE_RelabelExpressions();

  vPrintBoard(__LINE__);

  // delete potentially empty where
  G_vRestart();
  G_vDeleteWhenEmptySub(CK_WHERE,CK_BOOLCALC);

  vPrintBoard(__LINE__);

*/

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // x) push to sq
  if (! G_bPushBoard(m_sSqyNm)) {
    vTrace("DID NOT FIND Board %s\n"_Stz,m_sSqyNm.szStz());
    NEVER_REACH
  }
  vAssert_Board_sqB();

  m_pCard_yanked = E_pSubset_ChildrenBetween(G.pDeck, 0);

  vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // x) pop back to mq
  G_vPopBoard();
  vAssert_Board_mqA();

  G.pDeck->vPush(m_pCard_yanked);
  G.pDeck->vReplaceChildWithItsSubs(m_pCard_yanked);
  DELETE_CARD(m_pCard_yanked);

  vPrintBoard(__LINE__);

  G_vRestart();

  while (! G_bIsNextMain(CK_FOLD)) {

    if (G_bIsNextMain(CK_TABLE)) {
      RASSERT(G_bEachMain());
      continue;
    }
    else
    if (G_bIsNextMain(CK_PROJECT)) {
      RASSERT(G_bEachMainWithSub(CK_PROJECT, CK_STREAM));
      break;
    }
    else
    if (G_bIsNextMain(CK_AGGREG)) {
      RASSERT(G_bEachMainWithSub(CK_AGGREG, CK_PROJELIST));
      G.p_stream = Card_Stream::pDerive_Of_aggreg((Card_AggElist *) (G.p_projElist));
      G.p_aggreg->vAppend(G.p_stream);
      RASSERT(G_bEachMainWithSub(CK_AGGREG, CK_PROJELIST));
      G.p_stream = Card_Stream::pDerive_Of_aggreg((Card_AggElist *) (G.p_projElist));
      G.p_aggreg->vAppend(G.p_stream);
      RASSERT(G_bEachMainWithSub(CK_AGGREG, CK_PROJELIST));
      G.p_stream = Card_Stream::pDerive_Of_aggreg((Card_AggElist *) (G.p_projElist));
      G.p_aggreg->vAppend(G.p_stream);
      break;
    }
    else {

    }
  }

  RASSERT(G_bEachMain(CK_FOLD));
  R_vUTE_Derive_foldElist_From_project_stream();
  vPrintBoard(__LINE__);

  if (G_bEachMainWithSub(CK_GBYAGG,CK_GBYELIST)) {
    i8 nBumpRight = G.p_fold->Y_iGet(P_BumpRight);
    R_vUTE_Bump_stmOdn_In_rpnCalc_Of_Elist(G.p_gbyElist);

    G.p_fold->Y_vCut(P_BumpRight);
    G.p_fold->Y_vSet(P_BumpedRight,(nBumpRight));
  }

  // find table for sqyarg
  char cTblSyn = E_cValue_TableColSyn(m_sSqyArg);
  G.p_table = G.pDeck->pFindTableCard(cTblSyn, G.p_MainCard);

  // TODO: derive cmpPred from marked tstPred and add to corresponding table

  G_vNewMember(CK_CMPPRED,"=="_Stz);
  G_vNewMember(CK_TERM,m_sSqyArg.szStz());
  G.p_cmpPred->vAppend(G.p_term);
  //G_vNewMember(CK_TERM,m_sSqyRsl.szStz());
  G_vNewMember(CK_TERM,"$1"_Stz);
  G.p_cmpPred->vAppend(G.p_term);

  R_vUTE_AddPredToTableFilter(G.p_table,G.p_cmpPred);
  vPrintBoard(__LINE__);


  // filter into bounds

  C_vBounds_DeriveFrom_filter(G.p_table,G.p_filter,false);
  vPrintBoard(__LINE__);

  G.p_boolCalc = G.p_filter->pSub_boolCalc();
  M_vBoolCalc_CullPred(G.p_boolCalc, MARK_UP);
  vPrintBoard(__LINE__);

  vR_S36_T30_Delete_Empty_filter();

  G_vRestart();
  if (G_bEachMainWithSub(CK_WHERE,CK_BOOLCALC)) {
    M_vBoolCalc_CullPred(G.p_boolCalc, MARK_AT);
  }


  vPrintBoard(__LINE__);

  // delete potentially empty where
  G_vRestart();
  G_vDeleteWhenEmptySub(CK_WHERE,CK_BOOLCALC);

  vPrintBoard(__LINE__);





  R_vUTE_AdjustStmLoHi();
  vPrintBoard(__LINE__);

  R_vUTE_RenumberStmOdn();
  vPrintBoard(__LINE__);

  R_vUTE_RelabelExpressions();

  m_pBench->vIsFlat(true);
  G.pDeck->vClearMarksRecursive();

  vPrintBoard(__LINE__);


  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS40_T50_Fold_W_L_C_E(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  // W L C E
  // push a (linear correlated exists) sub-query before the main-query
  // as a keyExists.  join ordering will be required to make it runnable
  // (refer: Q4)

  // assumptions
  // A1) sq is a single <table>
  // A2) will have index and bound [BVO: vSeries_42_Bound_Analysis]

  /* ARCHETYPE :
      select count(*)
        from orders
       where exists (select *
                       from lineitem
                      where L_ORDERKEY = O_ORDERKEY
                        and l_commitdate < l_receiptdate)
      ;
   */

  vTransform(__func__, "((20))"_Stz);
  vAssert_Board_mqA();

  // W L C E
  //             //
  //             //
  // T W A A A   // find W_L_C_E subquery call in mq:<where>/<boolCalc>
  //   f         //
  //   bc        //
  //  @ro exists //

  // 1) find W_L_C_E subquery call in mq:<where>/<boolCalc>
  if (G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    if (!R_bUTE_FindAndMarkSubQryCall()) {
      vTracer("DID NOT FIND W_L_C_E SubQuery call\n"_Stz);
      NEVER_REACH
    }
  }
  vTrace("Found W_L_C_E call to %s(%s)\n"_Stz,m_sSqyNm.szStz(),m_sSqyArg.szStz());
  vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // 2) push to sq
  if (! G_bPushBoard(m_sSqyNm)) {
    vTrace("DID NOT FIND Board %s\n"_Stz,m_sSqyNm.szStz());
    NEVER_REACH
  }
  vAssert_Board_sqB();

  G_vRestart();
  if (G_bEachMainWithSub(CK_TABLE,CK_FILTER)) {
    G_vMorphCard(G.p_filter,CK_CONSTRAINT);
  }
  vPrintBoard(__LINE__);

  G_vRestart();
  if (G_bEachMainWith2Sub(CK_TABLE,CK_INDEX,CK_CONSTRAINT)) {
    G.p_table->vRemoveChild(G.p_constraint);
    G.p_index->vAppend(G.p_constraint);
  }
  vPrintBoard(__LINE__);

  // 3) get copy of sq:<table>/* into cc:<table>
  G_vRestart();
  if (G_bEachMain(CK_TABLE)) {
    G_vCopyCard(G.p_table);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // 4) pop back to mq
  G_vPopBoard();
  vAssert_Board_mqA();

  G_vNewMember(CK_FOLD,"W_L_C_E"_Stz);
  G_vInsertBefore(G.p_fold,G.p_where);


  // W L C E
  //                 //
  //       E-sqr->   // Insert sq table before fold
  // --    ----mq--- //
  // T T F W A A A   // put cc:<table>/* before mq:<where>
  //   --sq          //

  G_vInsertBefore(m_pCard_copied,G.p_fold);
  G_vAppendNewChild(m_pCard_copied, CK_KEYEXISTS);
  vPrintBoard(__LINE__);

  // W L C E
  //                 // Replace %1 parameter with calling argument
  //                 //
  // T T F W A A A   // %1 -> $t.n
  //   b             //
  //   %1->$t.c      //
  //   ke            //

  R_bUTE_ReplaceValueOfSub(m_pCard_copied,CK_TERM,"%1"_Stz,m_sSqyArg);
  G_vCopyCard(nullptr);
  vPrintBoard(__LINE__);

  // W L C E
  //                 // Cull @ro exists from <where>/<filter>
  //                 //
  // T T F W A A A   //
  //       f         //
  //       bc        //
  //      @ro exists //

  G_vRestart();
  if (G_bEachMainWithSub(CK_WHERE,CK_BOOLCALC)) {
    M_vBoolCalc_CullPred(G.p_boolCalc, MARK_AT);
  }
  vPrintBoard(__LINE__);

  // W L C E
  //                 // Delete potentially empty where
  //                 //
  // T T F A A A     //
  //                 //

  G_vRestart();
  G_vDeleteWhenEmptySub(CK_WHERE,CK_BOOLCALC);
  vPrintBoard(__LINE__);

  m_pBench->vIsFlat(true);
  G.pDeck->vClearMarksRecursive();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS40_T60_Fold_W_L_NC_NI(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  // W L NC NI
  // push a (linear non-correlated not in) sub-query before the main-query
  // as a keyExists
  // (refer: Q16)

  /* ARCHETYPE :
      select count(*)
        from partsupp, part
       where PS_PARTKEY = P_PARTKEY
         and PS_SUPPKEY not in (select S_SUPPKEY
                                  from supplier
                                 where s_comment like '%Customer%Complaints%')
      ;
 */

  vTransform(__func__, "((20))"_Stz);

  // 1) find W_L_NC_NI subquery call in mq:<where>/<boolCalc>
  vAssert_Board_mqA();

  // W L NC NI
  //               //
  //               //
  // T T W A A A   // find W_L_NC_NI subquery call in mq:<where>/<boolCalc>
  //     f         //
  //     bc        //
  //    @ro notIn  //
  //        __sq2  // sub-query
  //           #1  // result
  //         $a.2  // argument


  if (G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    if (!R_bUTE_FindAndMarkSubQryCall()) {
      vTracer("DID NOT FIND W_L_NC_NI SubQuery call\n"_Stz);
      NEVER_REACH
    }
  }
  vTrace(">>>>>>>>> Found W_L_NC_NI call to %s(%s)->%s\n"_Stz,m_sSqyNm.szStz(),m_sSqyArg.szStz(),m_sSqyRsl.szStz());
  vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // 2) push to sq
  if (! G_bPushBoard(m_sSqyNm)) {
    vTracer("DID NOT FIND Board %s\n"_Stz,m_sSqyNm.szStz());
    NEVER_REACH
  }
  vAssert_Board_sqB();

  // x) pull <where> into <table>/<filter>



  vPrintBoard(__LINE__);

  G_vRestart();
  if (G_bEachMain(CK_TABLE)) {
    if (G_bEachMain(CK_WHERE)) {
      R_vUTE_MoveWhereToTableAsConstraint(G.p_table,G.p_where);
    }
  }
  vPrintBoard(__LINE__);

  // 3) get copy of sq:<table>/* into cc:<table>
  G_vRestart();
  if (G_bEachMain(CK_TABLE)) {
    G_vCopyCard(G.p_table);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // 4) pop back to mq
  G_vPopBoard();
  vAssert_Board_mqA();

  // 5) fold
  //vR_S40_T50a_Fold_W_L_C_E_Before_Mq_As_keyExists();
  // put cc:<table>/* before mq:<where>

  G_vNewMember(CK_FOLD,"W_L_NC_NI"_Stz);
  G_vInsertBefore(G.p_fold,G.p_where);

  // W L NC NI
  //                   //
  // ------  --mq----  //
  // T T T F W A A A   // find W_L_NC_NI subquery call in mq:<where>/<boolCalc>
  //     --sq          //
  //     kne           //

  G_vInsertBefore(m_pCard_copied,G.p_fold);
  G_vAppendNewChild(m_pCard_copied, CK_KEYNOTEXISTS);

  G.pDeck->vRenumberRecursive();
  R_vUTE_AdjustStmLoHi();
  R_vUTE_RenumberStmOdn();
  vPrintBoard(__LINE__);

  G.p_bounds = C_pBounds_Create_Eq_ForGivenCol(m_sSqyArg);
  //m_pCard_copied->vAppend(G.p_bounds);

  G.p_indexList = G.p_table->pSub_indexList();
  G.p_index = G.p_table->pSub_index();
  G.p_index->vAppend(G.p_bounds);


  vPrintBoard(__LINE__);

  // W L NC NI
  //                   //
  // T T T F W A A A   //
  //     b             //
  //     kne           //

  G_vRestart();
  RASSERT(G_bEachMainWith2Sub(CK_TABLE,CK_KEYNOTEXISTS,CK_CONSTRAINT));
  G.p_table->vRemoveChild(G.p_constraint);
  G.p_indexList = G.p_table->pSub_indexList();
  G.p_index = G.p_table->pSub_index();
  G.p_index->vAppend(G.p_constraint);

  vPrintBoard(__LINE__);

  G_vCopyCard(nullptr);


  // W L NC NI
  //                 // Cull @ro notIn
  //                 //
  // T T T W A A A   //
  //       f         //
  //       bc        //
  //      @ro notIn  //
  //          __sq2  //
  //             #1  //
  //           $a.2  //

  G_vRestart();
  if (G_bEachMainWithSub(CK_WHERE,CK_BOOLCALC)) {
    M_vBoolCalc_CullPred(G.p_boolCalc, MARK_AT);
  }

  vPrintBoard(__LINE__);


  // W L NC NI
  //               // Delete potentially empty where
  //               //
  // T T T A A A   //
  //               //
  //               //

  // delete potentially empty where
  G_vRestart();
  G_vDeleteWhenEmptySub(CK_WHERE,CK_BOOLCALC);

  vPrintBoard(__LINE__);

  m_pBench->vIsFlat(true);
  G.pDeck->vClearMarksRecursive();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS40_T70_Fold_H_A_NC_ro(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  // H A NC ro
  // push a (single-value aggregate non-correlated ro) sub query before main queryy
  // as am aggregation-fan
  // further: the aggregation-value is injected/factored into the main-query
  // (refer: Q11)

  /* ARCHETYPE :
      select ps_partkey,
             sum(ps_supplycost * ps_availqty) as value
        from partsupp, supplier
       where PS_SUPPKEY = S_SUPPKEY
       group by ps_partkey
      having sum(ps_supplycost * ps_availqty) > (select avg(ps_supplycost * ps_availqty)
                                                   from partsupp, supplier
                                                  where PS_SUPPKEY = S_SUPPKEY)
      ;
 */

  vTransform(__func__, "((20))"_Stz);

  vAssert_Board_mqA();

  // H A NC ro
  //                  //
  //                  //
  // T T G G G H      //
  //           bc     //
  //          @ro     //
  //              $2  //
  //           __sq2  //

  // 1) find W_L_NC_NI subquery call in mq:<where>/<boolCalc>
  if (G_bEachMainWithSub(CK_HAVING, CK_BOOLCALC)) {
    if (!R_bUTE_FindAndMarkSubQryCall()) {
      vTrace("DID NOT FIND H_A_NC_ro SubQuery call\n"_Stz);
      NEVER_REACH
    }
  }
  vTrace(">>>>>>>>> Found H_A_NC_ro call:  %s %s %s\n"_Stz,m_sSqyArg.szStz(),m_sSqyRO.szStz(),m_sSqyNm.szStz());
  vPrintBoard(__LINE__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // 2) push to sq
  if (! G_bPushBoard(m_sSqyNm)) {
    vTracer("DID NOT FIND Board %s\n"_Stz,m_sSqyNm.szStz());
    NEVER_REACH
  }
  vAssert_Board_sqB();

  m_pCard_yanked = E_pSubset_ChildrenBetween(G.pDeck, 0);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // 4) pop back to mq
  G_vPopBoard();
  vAssert_Board_mqA();

  G_vNewMember(CK_FOLD,"H_A_NC_ro"_Stz);
  G.pDeck->vPush(G.p_fold);
//  G_vAppendNewChild(G.p_fold,CK_FOLDELIST);
//  G_vAppendNewChild(G.p_foldElist,CK_TYPE,"((type))"_Stz);
//  G_vAppendNewChild(G.p_foldElist,CK_AS,"__SQ1RESULT"_Stz);
//  G_vAppendNewChild(G.p_foldElist,CK_RPNCALC);
//  G_vAppendNewChild(G.p_rpnCalc,CK_TERM,"$1"_Stz);

  // H A NC ro
  //                               //
  //               --mq--------    //
  // T T A A A P F T T G G G H     //
  // --sq--------                  //


  G.pDeck->vPush(m_pCard_yanked);
  G.pDeck->vReplaceChildWithItsSubs(m_pCard_yanked);
  DELETE_CARD(m_pCard_yanked);

  vPrintBoard(__LINE__);

  R_vUTE_AdjustStmLoHi();
  vPrintBoard(__LINE__);

  R_vUTE_RenumberStmOdn();
  vPrintBoard(__LINE__);

  // H A NC ro
  //                               //
  //                               //
  // T T A A A P F T T G G G H     //
  //             fe                //

  G_vRestart();
  if (G_bEachMain(CK_FOLD)) {
    G.i_FoldCard = G.i_MainCard;
    R_vUTE_Derive_foldExpr();
  }
  vPrintBoard(__LINE__);


  // H A NC ro
  //                               //
  //                               //
  // T T A A A P F T T G G G H     //
  //                   gegege      //
  //                   $1$1$1      //
  //                   aeaeae      //
  //                   k1k1k1      //

  R_vUTE_FoldIn_foldExpr();
  G.pDeck->vRenumberRecursive();
  vPrintBoard(__LINE__);

  // H A NC ro
  //                               //
  //                               //
  // T T A A A P F T T G G G H P   //
  //                         bc    //
  //                        @ro    //
  //                         $3    //
  //                         $1    //

  G_vRestart();
  if (G_bEachMain(CK_HAVING)) {
    R_vUTE_Unproject_foldExpr_After_having();
    R_vUTE_Correct_having_BaseOn_foldExpr();
  }

  vPrintBoard(__LINE__);

  m_pBench->vIsFlat(true);
  G.pDeck->vClearMarksRecursive();

  vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                   vSeries_50_Join_Ordering */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bS50_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! bEnabled)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS50_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS50_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCardCtl->vUseShorter();

  vST00_Begin();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS50_T00_Extract_tableList_From_queryBoard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vAssert_Board_mqA();

  //vSetupPartitionBoard();

  //--nn--------------==
  // tableList

  BoardIter xBI_query(G.pBoard);

  G.p_tableList = new Card_TableList;
  G.pDeck->vAppendMain(G.p_tableList);

  while ( (G.p_MainCard = xBI_query.pEachMain()) ) {
    if (! (G.p_table = G.p_MainCard->pSub_table()))
      continue;

    G.p_tableList->vAppend(G.p_table->pDupCardAndSubCards());
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS50_T10_Extract_joinPredList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vAssert_Board_mqA();

  //vSetupPartitionBoard();

  //--nn--------------==
  // joinPredList

  G.p_joinPredList = new Card_JoinPredList;
  G.pDeck->vAppendMain(G.p_joinPredList);

  BoardIter xBI_query(G.pBoard);
  while ((G.p_MainCard = xBI_query.pEachMain(CK_FILTER))) {
    G.p_boolCalc = G.p_MainCard->pSub_boolCalc();
    if (!G.p_boolCalc)
      continue;

    D_vDerive_JoinPredList(G.p_joinPredList, G.p_boolCalc);
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS50_T20_Compute_coPtnPredList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vAssert_Board_mqA();

  //vSetupPartitionBoard();

  //--nn--------------==
  // coPptnPredList

  G.p_coPtnPredList = new Card_CoPtnPredList;
  G.pDeck->vAppendMain(G.p_coPtnPredList);

  G.p_tableList =
      _pCard_TableList(G.pDeck->pFindChild(CK_TABLELIST));
  G.p_joinPredList =
      _pCard_JoinPredList(G.pDeck->pFindChild(CK_JOINPREDLIST));

  i8 nJoinPreds = G.p_joinPredList->nChildCards();
  for (i8 i_joinPred = 0; i_joinPred < nJoinPreds; i_joinPred++) {
    G.p_cmpPred =
        _pCard_CmpPred(G.p_joinPredList->pGetChild(i_joinPred));

    if (!T_bTableList_PredCmpIsCoPtn(G.p_tableList, G.p_cmpPred))
      continue;

    G.p_coPtnPred = new Card_CoPtnPred;
    G.p_coPtnPred->vAppend(G.p_cmpPred->pDupCardAndSubCards());

    G.p_coPtnPredList->vAppend(G.p_coPtnPred);
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS50_T30_Compute_coPtnJoinList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vAssert_Board_mqA();

  //vSetupPartitionBoard();

  //--nn--------------==
  // coPtnJoinList

  vR_S50_T30_Compute_coPtnJoinList();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS50_T40_Compute_crossPtnJoinList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vAssert_Board_mqA();

  //vSetupPartitionBoard();

  //--nn--------------==
  // crossPtnJoinList

  vR_S50_T40_Compute_crossPtnJoinList();


//  G.p_comment = new Card_Comment("((TO BE CODED))"_Stz);
//  G.p_crossPtnJoinList->vAppend(G.p_comment);

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS50_T50_Compute_ptndJoinSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vAssert_Board_mqA();

  //vSetupPartitionBoard();

  //--nn--------------==
  // ptndJoinSet

  vR_S50_T50_Compute_ptndJoinSet();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS50_T60_Copy_ptndJoinSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vAssert_Board_mqA();

  //vSetupPermuteBoard();

  vR_S50_T60_Copy_ptndJoinSet();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS50_T70_Generate_ptndPermutations(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vAssert_Board_mqA();

//  PermGen::vTest(4);

  //--nn--------------==
  // x

  vR_S50_T70_Generate_ptndPermutations();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS50_T80_Remove_invalidPermutations(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vAssert_Board_mqA();

  vR_S50_T80_Remove_invalidPermutations();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS50_T90_Setup_costEst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vAssert_Board_mqA();

  //vSetupCostEstBoard();

  //m_pBoard_costEst->vDuplicateBoard(G.pBoard);

  while ((G.p_MainCard = G.pDeck->pGetChild(0))) {

    if (CK_PERMUTATION == G.p_MainCard->eCard())
      break;

    G.pDeck->vRemoveChild(G.p_MainCard);
    delete G.p_MainCard;
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS50_T92_Estimate_ptndJoins(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vAssert_Board_mqA();

  vR_S50_T92_Estimate_ptndJoins();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS50_T94_Estimate_network(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vAssert_Board_mqA();

  vR_S50_T94_Estimate_network();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS50_T96_Estimate_joinOrder(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vAssert_Board_mqA();

  vR_S50_T96_Estimate_joinOrder();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS50_T98_Extract_joinOrderSeq(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vAssert_Board_mqA();

  vR_S50_T98_Extract_joinOrderSeq();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS50_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSTZZ_End();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS50_T00_Join_Ordering_v2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vAssert_Board_mqA();

  //G.p_joinOrder = Card_JoinOrder::pExtract(G.pDeck);
  //G.pDeck->vApply(G.p_joinOrder);

  vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                    vSeries_75_EE_operators */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* convert all <table> into either <scan> , <indexJoin> or <keyExists>        */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS75_T10_Morph_table_To_EEop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "convert all <table> into either <scan> or <indexJoin>"_Stz);

  vAssert_Board_mqA();

  vPrintBoard(__LINE__);

  while (G_bEachMain(CK_TABLE)) {

    G.p_bounds         = G.p_table->pSub_bounds();
    G.p_index          = G.p_table->pSub_index();
    G.p_indexJoin      = G.p_table->pSub_indexJoin();
    G.p_keyExists      = G.p_table->pSub_keyExists();
    G.p_keyNotExists   = G.p_table->pSub_keyNotExists();

//    vTrace("MainCard %p : [%lld] %s %s\n"_Stz,
//           G.p_table,G.p_table->iCardNo(),aCK[G.p_table->eCard()],G.p_table->szValue());

    RASSERT(! G.p_indexJoin);
    vR_S75_T10_Morph_table_To_EEop_v2();

    G.pDeck->vRenumberRecursive();

    vPrintBoard(__LINE__);

    vDebugPoint();
  }

  vPrintBoard(__LINE__);

  G_vRestart();
  while (G_bEachMain(CK_SCAN)) {
    i8 i_Child;
    G.p_note = _pCard_Note(G.p_scan->pFindChild(CK_NOTE,i_Child));

    if (i_Child == G.p_scan->nChildCards() - 1) {
      G.p_scan->vRemoveChild(G.p_note);
      DELETE_CARD(G.p_note);
    }
  }
  vPrintBoard(__LINE__);

//  G_vRestart();
//  while (G_bEachMain(CK_INDEXJOIN)) {
//    vR_S75_T10_Morph_table_To_EEop_c();
//  }
//  vPrintBoard(__LINE__);

//  G_vRestart();
//  while (G_bEachMain(CK_KEYEXISTS)) {
//    if (G.p_keyExists) {
//      i8 i_Child;
//      G.p_note = _pCard_Note(G.p_keyExists->pFindChild(CK_NOTE, i_Child));
//      G.p_note->vValue(SEP_DERIVED);
//    }
//  }
//  vPrintBoard(__LINE__);

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS75_T20_Morph_keyExists_filter_To_constraint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "convert all <table> into either <scan> or <indexJoin>"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMainWithSub(CK_KEYEXISTS,CK_FILTER)) {
    G.p_filter->vCard(CK_CONSTRAINT);
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS75_T40_Convert_stmColOdn_Of_constraint_To_stmColNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "convert all <table> into either <scan> or <indexJoin>"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMainWithSub(CK_KEYEXISTS,CK_CONSTRAINT)) {
    vR_S75_T40_Convert_stmColOdn_Of_constraint_To_stmColNm();
  }

  vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                      vSeries_80_Index_Scan */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bS80_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! bEnabled)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS80_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS80_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCardCtl->vUseShorter();

  vST00_Begin();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS80_T30_Morph_scan_To_indexScan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vAssert_Board_mqA();

  G_vRestart();
  if (G_bEachMainWithSub(CK_SCAN,CK_BOUNDS)) {
    if (G.p_bounds->pSub_lBound() || G.p_bounds->pSub_uBound()) {
      G.p_scan->vCard(CK_INDEXSCAN);
    }
  }

  vPrintBoard();

  vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS80_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSTZZ_End();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                 vSeries_90_Stream_Ordering */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bS90_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! bEnabled)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS90_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS90_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  //m_pCardCtl->vUseShorter();

  m_pCardCtl->vPrintSubCards(true,
                                  CK_AGGELIST,
                                  CK_GBYELIST,
                                  CK_INDEXLIST,
                                  CK_PROJELIST,
                                  CK_RPNCALC,
                                  CK_STREAM,
                                  0);
  m_pCardCtl->vPrintSubCards(false,
                                   CK_STMCOLLIST,
                                   0);

  m_pCardCtl->vPrintBelow(CK_PROJECT,true);

  vST00_Begin();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS90_T30_Derive_order(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "remove <sort> if <sbyElist> is the same as the <gbyElist>"_Stz);

  vAssert_Board_mqA();

  R_vUTE_Derive_order();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS90_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSTZZ_End();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                  vSeries_92_Project_Pullup */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bS92_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! bEnabled)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS92_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS92_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  //m_pCardCtl->vUseShorter();

  m_pCardCtl->vPrintSubCards(true,
                             CK_AGGELIST,
                             CK_GBYELIST,
                             CK_INDEXLIST,
                             CK_PROJELIST,
                             CK_RPNCALC,
                             CK_STREAM,
                             0);
  m_pCardCtl->vPrintSubCards(false,
                             CK_STMCOLLIST,
                             0);

  m_pCardCtl->vPrintBelow(CK_PROJECT,true);

  vST00_Begin();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS92_T10_Project_Pullup(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "(desc)"_Stz);

  vAssert_Board_mqA();

  vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS92_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSTZZ_End();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                               vSeries_A0_Filter_Refinement */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bSA0_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! bEnabled)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vSA0_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vSA0_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCardCtl->vUseShorter();

  vST00_Begin();
}

/*----------------------------------------------------------------------------*/
/* inspect filters and remove cmp predicates which are in bounds              */
/*----------------------------------------------------------------------------*/
void Xtransforms::vSA0_T10_Eliminate_bounds_From_filter(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((desc))"_Stz);

  vAssert_Board_mqA();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark any cmpPred that are a bound in bounds

  G_vRestart();
  while (G_bEachMain()) {

    if (! (G.p_MainCard->bIsCard(CK_INDEXJOIN) ||
           G.p_MainCard->bIsCard(CK_INDEXSCAN)))
      continue;
    if (! (G.p_bounds = G.p_MainCard->pSub_bounds()))
      continue;
    if (! (G.p_filter = G.p_MainCard->pSub_filter()))
      continue;

    // have <bounds> & <filter> under <main>

    CardRecurse xCR(G.p_filter);
    while ((G.p_cmpPred = _pCard_CmpPred(xCR.pRecurseEach(CK_CMPPRED)))) {
      if (T_bCmpPred_SubsetOf_bounds(G.p_cmpPred,G.p_bounds))
        G.p_cmpPred->vMark1(MARK_AT);
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cull marked cmpPred from filter

  G_vRestart();
  while (G_bEachMain()) {
    if ((G.p_filter = G.p_MainCard->pSub_filter()))
      M_vFilter_CullPred(G.p_filter, MARK_AT);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // delete any filter if empty

  G_vRestart();
  while (G_bEachMain()) {
    if ((G.p_filter = G.p_MainCard->pSub_filter()))
      M_vFilter_DeleteIfEmpty(G.p_filter,G.p_MainCard);
  }

  vPrintBoard();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* individuate <scan|indexJoin>/<filter> as main <filter>                     */
/*----------------------------------------------------------------------------*/
void Xtransforms::vSA0_T20_Individuate_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "individuate <scan|indexJoin>/<filter> as main <filter>"_Stz);

  vAssert_Board_mqA();

  G_vRestart();
  while (G_bEachMainWithSub(CK_SCAN,CK_FILTER)) {
    G.p_scan->vRemoveChild(G.p_filter);
    G.pDeck->vInsertAfter(G.p_filter, G.p_scan);

    if (! G.p_filter->pSub_level()) {
      G.p_filter->vPush(G.p_scan->pSub_level()->pDupCardAndSubCards());
    }
  }

  G_vRestart();
  while (G_bEachMainWithSub(CK_INDEXSCAN,CK_FILTER)) {
    R_vUTE_Individuate_filter();
  }

  G_vRestart();
  while (G_bEachMainWithSub(CK_INDEXJOIN,CK_FILTER)) {
    vR_SA0_T10_Individuate_indexJoin_filter_b();
  }

  G_vRestart();
  while (G_bEachMainWithSub(CK_CROSSJOIN,CK_FILTER)) {
    vR_SA0_T10c_Individuate_crossJoin_filter();
  }


  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vSA0_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSTZZ_End();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                       vSeries_E0_To_StmOdn */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* individuate <scan|indexJoin>/<filter> as main <filter>                     */
/*----------------------------------------------------------------------------*/
void Xtransforms::vSE0_T10_Convert_TblColOdn_To_StmOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "individuate <scan|indexJoin>/<filter> as main <filter>"_Stz);

  vAssert_Board_mqA();

  G.pBoard->vMayHaveStmColOdn(true);

  while (G_bEachMain()) {

    R_vUTE_Convert_tblColOdn_To_stmOdn();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vSE0_T20_Convert_tblColOdn_Of_constraint_To_stmColNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((20))))"_Stz);

  vAssert_Board_mqA();

  G.pBoard->vMayHaveStmColOdn(true);

  while (G_bEachMain()) {

    if (! (   CK_KEYEXISTS    == G.e_MainCard
           || CK_KEYNOTEXISTS == G.e_MainCard
    ))
      continue;

    G.p_constraint = G.p_MainCard->pSub_constraint();

    R_vUTE_Convert_tblColOdn_Of_constraint_To_stmColNm();
  }

  vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                vSeries_Z0_Restate_Pipeline */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* reinstate stages                                                           */
/*----------------------------------------------------------------------------*/
void Xtransforms::vSZ0_T20_Reinstate_stagesV3(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "reinstate stages"_Stz);

  vAssert_Board_mqA();

  //m_pCardCtl->vPrintSubCards(true,CK_PROJELIST,0);
  //m_pCardCtl->vPrintSubCards(true,CK_RPNCALC,0);

  vPrintBoard(__LINE__);

  vR_SZ0_T20_Reinstate_stagesV3();

  vEndTransform();
}



/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vAssert_Board_any(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  // nothing to do?
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vAssert_Board_mqA(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  RASSERT(0 == G.pBoard->i_Board());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vAssert_Board_sqB(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  RASSERT(1 == G.pBoard->i_Board());
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vST00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! m_pCardCtl->bBoardPrint())
    return;

  char* azSeparator = (char*)
      "<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>"
      "<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>"
      "<><><><><><><><><><><><><><><><><><><><><><><><><><><><><><><>";

  vTrace("\n%s\n%186s\n%s\n"_Stz,
         azSeparator,
         szXhand(),
         azSeparator);

  vXdesc(""_Stz);
  vXform(""_Stz);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vSTZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
