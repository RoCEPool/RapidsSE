/*============================================================================*/
/* Xtransforms.cpp --                                                          */
/*============================================================================*/

#include "Xtransforms.hpp"

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

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Discover the optimisation transformations

  do {

    bOpts = false;

    if (bOT_Cut_Sort()) {
      m_eOptTranforms[iOpts++] = OT_CUT_SORT;
      bOpts = true;
    }

    if (bOT_Dup_Filter()) {
      m_eOptTranforms[iOpts++] = OT_DUP_FILTER;
      bOpts = true;
    }

    if (bOT_Ely_Filters()) {
      m_eOptTranforms[iOpts++] = OT_ELY_FILTERS;
      bOpts = true;
    }

    if (bOT_Ely_Projections()) {
      m_eOptTranforms[iOpts++] = OT_ELY_PROJECTIONS;
      bOpts = true;
    }

    if (bOT_Ely_Sort_Limit()) {
      m_eOptTranforms[iOpts++] = OT_ELY_SORT_LIMIT;
      bOpts = true;
    }

    if (bOT_Min_Groupby()) {
      m_eOptTranforms[iOpts++] = OT_MIN_GROUPBY;
      bOpts = true;
    }


    if (bOT_Otm_Calculations()) {
      m_eOptTranforms[iOpts++] = OT_OTM_CALCULATIONS;
      bOpts = true;
    }

    if (bOT_Ppn_Projections()) {
      m_eOptTranforms[iOpts++] = OT_PPT_PROJECTIONS;
      bOpts = true;
    }

    if (bOT_Rpl_Aggreg_By_OrdAgg()) {
      m_eOptTranforms[iOpts++] = OT_RPL_AGGREG_BY_ORDAGG;
      bOpts = true;
    }

    if (bOT_Rpl_Aggreg_By_Sort_OrdAgg()) {
      m_eOptTranforms[iOpts++] = OT_RPL_AGGREG_BY_SORT_ORDAGG;
      bOpts = true;
    }

    if (bOT_Rpl_Max_By_Top()) {
      m_eOptTranforms[iOpts++] = OT_RPL_MAX_BY_TOP;
      bOpts = true;
    }

    if (bOT_Rpl_Scan_by_IndexScan()) {
      m_eOptTranforms[iOpts++] = OT_RPL_SCAN_BY_INDEXSCAN;
      bOpts = true;
    }

    if (bOT_Rpl_SortDesc_Limit_By_Top()) {
      m_eOptTranforms[iOpts++] = OT_RPL_SORTDESC_LIMIT_BY_TOP;
      bOpts = true;
    }

    if (bOT_Rpl_Sort_Limit_OnOrogin_By_AllLevels()) {
      m_eOptTranforms[iOpts++] = OT_RPL_SORT_LIMIT_ONORIGIN_BY_ALLLEVELS;
      bOpts = true;
    }

    if (bOT_Seq_Join_Order()) {
      m_eOptTranforms[iOpts++] = OT_SEQ_JOIN_ORDER;
      bOpts = true;
    }

  } while (bOpts);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Apply the transformations in the order they were discovered

  for (i8 iOptTransform = 0; iOptTransform < iOpts; iOptTransform++) {

    switch(m_eOptTranforms[iOptTransform]) {

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // CUT
      case OT_CUT_SORT:
        vOT_Cut_Sort();
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // DUPLICATE
      case OT_DUP_FILTER:
        vOT_Dup_Filter();
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // EARLY
      case OT_ELY_FILTERS:
        vOT_Ely_Filters();
        break;

      case OT_ELY_PROJECTIONS:
        vOT_Ely_Projections();
        break;

      case OT_ELY_SORT_LIMIT:
        vOT_Ely_Sort_Limit();
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // MINIMIZE
      case OT_MIN_GROUPBY:
        vOT_Min_Groupby();
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // ONE-TIME CALCULATIONS
      case OT_OTM_CALCULATIONS:
        vOT_Otm_Calculations();
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // POSTPONE
      case OT_PPT_PROJECTIONS:
        vOT_Ppn_Projections();
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // REPLACE
      case OT_RPL_AGGREG_BY_ORDAGG:
        vOT_Rpl_Aggreg_By_OrdAgg();
        break;

      case OT_RPL_AGGREG_BY_SORT_ORDAGG:
        vOT_Rpl_Aggreg_By_Sort_OrdAgg();
        break;

      case OT_RPL_MAX_BY_TOP:
        vOT_Rpl_Max_By_Top();
        break;

      case OT_RPL_SCAN_BY_INDEXSCAN:
        vOT_Rpl_Scan_by_IndexScan();
        break;

      case OT_RPL_SORTDESC_LIMIT_BY_TOP:
        vOT_Rpl_SortDesc_Limit_By_Top();
        break;

      case OT_RPL_SORT_LIMIT_ONORIGIN_BY_ALLLEVELS:
        vOT_Rpl_Sort_Limit_OnOrogin_By_AllLevels();
        break;

      // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
      // SEQUENCING
      case OT_SEQ_JOIN_ORDER:
        vOT_Seq_Join_Order();
        break;

      default:
        NEVER_REACH
    }
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bOT_Cut_Sort(void) {

  //
  // Query1, Query4, Query7, Query8, Query12
  //

  // Notes:
  // sbyElist expressions directions are all ascending
  // sbyElist matches the gbyElist of an earlier (on all partitions) stage

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vOT_Cut_Sort(void) {

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bOT_Dup_Filter(void) {

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vOT_Dup_Filter(void) {

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bOT_Ely_Filters(void) {

  // Query2, Query3, Query4, Query5, Query7, Query8,
  //   Query9, Query10, Query11, Query12, Query16, Query17 Query19

  // get columns from cmpPred's and find the tables they come from in previous
  // scan/indexScan/indexJoin cards and move them after the scan/indexJoin/indexJoin
  // of the tables

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vOT_Ely_Filters(void) {

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bOT_Ely_Projections(void) {

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vOT_Ely_Projections(void) {

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bOT_Ely_Sort_Limit(void) {

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vOT_Ely_Sort_Limit(void) {

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bOT_Min_Groupby(void) {

  //
  // Query3, Query10, Query18
  //

  // Notes:
  // * need to get the tables from which the columns of the gbyElist are from
  // * get the indexlist of unique indexes of the tables
  // * match the gbyElist columns with the indexList columns
  // * if a match is found , discard all other columns off the gbyElist
  //   except the match (unique one)


  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vOT_Min_Groupby(void) {

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bOT_Otm_Calculations(void) {

  //
  // Query1, Query4, Query5, Query6, Query10, Query12, Query14, Query15,
  //   Query19, Query20

  // Notes:


  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vOT_Otm_Calculations(void) {

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bOT_Ppn_Projections(void) {

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vOT_Ppn_Projections(void) {

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bOT_Rpl_Aggreg_By_OrdAgg(void) {

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vOT_Rpl_Aggreg_By_OrdAgg(void) {


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
void Xtransforms::vOT_Rpl_Aggreg_By_Sort_OrdAgg(void) {


}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bOT_Rpl_Max_By_Top(void) {

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vOT_Rpl_Max_By_Top(void) {


}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bOT_Rpl_Scan_by_IndexScan(void) {

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vOT_Rpl_Scan_by_IndexScan(void) {


}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bOT_Rpl_SortDesc_Limit_By_Top(void) {

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vOT_Rpl_SortDesc_Limit_By_Top(void) {


}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bOT_Rpl_Sort_Limit_OnOrogin_By_AllLevels(void) {

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vOT_Rpl_Sort_Limit_OnOrogin_By_AllLevels(void) {


}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bOT_Seq_Join_Order(void) {

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vOT_Seq_Join_Order(void) {


}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                   vSeries_10_Condense_Info */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bS10_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! bEnabled)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS10_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

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
/* delete superfluous <absJoin>'s                                             */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS10_T10_Delete_All_absJoin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "delete superfluous <absJoin>'s"_Stz);

  vAssert_Board_any();

  while (G_bEachMain(CK_ABSJOIN)) {
    G_vRemoveDeleteMain(CK_ABSJOIN);
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* delete superfluous <combined>'s                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS10_T20_Delete_All_combined(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "delete superfluous <combined>'s"_Stz);

  vAssert_Board_any();

  while (G_bEachMain(CK_COMBINED)) {
    G_vRemoveDeleteMain(CK_COMBINED);
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* delete superfluous <send>'s                                                */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS10_T30_Delete_All_send(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "delete superfluous <send>'s"_Stz);

  vAssert_Board_any();

  while (G_bEachMain(CK_SEND)) {
    G_vRemoveDeleteMain(CK_SEND);
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* delete all <stage>'s                                                       */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS10_T40_Delete_All_stage(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "delete all <stage>'s"_Stz);

  vAssert_Board_any();

  while (G_bEachMain(CK_STAGE)) {
    G_vRemoveDeleteMain(CK_STAGE);
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* replace all <scan>'s with their children                                   */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS10_T50_Cull_scan_Leaving_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "replace all <scan>'s with their children"_Stz);

  vAssert_Board_any();

  while (G_bEachMain(CK_SCAN)) {
    G.pDeck->vReplaceChildWithItsSubs(G.p_scan);
    DELETE_CARD(G.p_scan);
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* add level to all pile                                                      */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS10_T60_Add_All_level(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "replace all <scan>'s with their children"_Stz);

  vAssert_Board_any();

  G.e_Level = AL_PARTITION;

  while (G_bEachMain()) {

    G.p_level = G.p_MainCard->pSub_level();
    if (G.p_level)
      continue;

    G.p_level = new Card_Level(AL_PARTITION);
    G.p_MainCard->vPush(G.p_level);
  }

  vEndTransform();
}


/*----------------------------------------------------------------------------*/
/* linearize board by deleting all redundant <stage>'s                        */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS10_T80_Delete_Redundant_stages(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "linearize board by deleting all duplicate <stage>'s"_Stz);

  vAssert_Board_any();

  //G.pDeck->vDump(0,(Stz*)(__func__));

  Card_Stage* p_stage_PRIOR = nullptr;

  while (G_bEachMain(CK_STAGE)) {
    if (! T_bStage_IsRedundant(G.p_stage,p_stage_PRIOR))   {
      p_stage_PRIOR = G.p_stage;
      continue;
    }

    vR_S10_T80_Delete_Redundant_stage();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* hide ignorable <stage>'s                                                   */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS10_T90_Hide_Ignorable_stages(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "hide ignorable <stage>'s"_Stz);

  vAssert_Board_any();

  while (G_bEachMain(CK_STAGE)) {
    vR_S10_T90_Hide_Ignorable_stage();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS10_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSTZZ_End();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                  vSeries_20_Table_Analysis */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bS20_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! bEnabled)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS20_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS20_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vST00_Begin();

  m_pCardCtl->vUseShorter();

  m_pBench->vPrefix('s');
}

/*----------------------------------------------------------------------------*/
/* add a <tableSyn> to all <table>'s                                          */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS20_T10_Affix_tableSyn_ToAll_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "add a <tableSyn> to all <table>'s"_Stz);

  vAssert_Board_any();

  while (G_bEachMainWithSub(CK_TABLE, CK_TABLENM))  {
    M_vTable_Set_Syn(G.p_table);
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* add a <tablePtn> under every <table>                                       */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS20_T20_Produce_tablePtn_UnderAll_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "add a <tablePtn> under every <table>"_Stz);

  vAssert_Board_any();

  while (G_bEachMainWithSub(CK_TABLE, CK_TABLENM)) {
    G.p_tablePtn = C_pProduce_tablePtn_Of_table(G.p_table);

    G.p_table->vAppend(G.p_tablePtn);
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS20_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSTZZ_End();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                  vSeries_24_Index_Analysis */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bS24_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! bEnabled)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS24_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS24_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vST00_Begin();

  m_pCardCtl->vUseShorter();

  vPrintBench(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS24_T10_Append_indexList_UnderAll_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "move @c predicates to a filter under table"_Stz);

  vAssert_Board_any();

  while (G_bEachMain(CK_TABLE)) {
    G.p_tableNm = G.p_table->pSub_tableNm();
    Table* pTable = A_pAccess_Table(G.p_tableNm->sz());

    G.p_indexList = C_pProduce_indexList_Of_table(pTable);

    G_vAppendChild(CK_TABLE,CK_INDEXLIST);
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS24_T20_Affix_tblColOdn_ToAll_idxCol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "delete where if it is effectively empty"_Stz);

  vAssert_Board_any();

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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS24_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSTZZ_End();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                 vSeries_22_Stream_Analysis */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bS22_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! bEnabled)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS22_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS22_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vST00_Begin();

  m_pCardCtl->vUseShorter();

  m_pCardCtl->vPrintSubCards(true,CK_AGGELIST,CK_GBYELIST,CK_PROJELIST,CK_STREAM,0);
}

/*----------------------------------------------------------------------------*/
/* add a <stream> under every <table>                                         */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS22_T10_Produce_stream_UnderAll_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "add a <stream> under every <table>"_Stz);

  vAssert_Board_any();

  vStreamAnalysisReset();

  while (G_bEachMainWithSub(CK_TABLE, CK_TABLENM)) {
    m_pTable = A_pAccess_Table(G.p_tableNm->sz());
    G.p_stream = C_pProduce_stream_Of_table(m_pTable);

    G.p_table->vAppend(G.p_stream);
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* add a <stream> under every <project>                                       */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS22_T12_Produce_stream_UnderAll_project(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "add a <stream> under every <project>"_Stz);

  vAssert_Board_any();

  vStreamAnalysisReset();

  while (G_bEachMainWithSub(CK_PROJECT,CK_PROJELIST)) {
    G.p_stream = C_pProduce_stream_Of_project(G.p_projElist);

    G.p_project->vAppend(G.p_stream);
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* add a <stream> under every <aggreg>                                        */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS22_T14_Produce_stream_UnderAll_aggreg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "add a <stream> under every <aggreg>"_Stz);

  vAssert_Board_any();

  vStreamAnalysisReset();

  while (G_bEachMainWithSub(CK_AGGREG,CK_AGGELIST)) {
    G.p_stream = C_pProduce_stream_Of_aggreg(G.p_aggElist);

    G.p_aggreg->vAppend(G.p_stream);
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* derive the ordinal properties &lo..&hi range for each <stream>             */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS22_T20_Derive_ordinalRange_ForAll_table_stream(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "calculate the ordinal $low..$high range for each <stream>"_Stz);

  vAssert_Board_any();

  while (G_bEachMainWithSub(CK_TABLE,CK_STREAM)) {
    M_vStream_Derive_OrdinalRange_table(G.p_stream, G.p_table->cValue());
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* derive the ordinal properties &lo..&hi range for each <stream>             */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS22_T22_Derive_ordinalRange_ForAll_project_stream(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "calculate the ordinal $low..$high range for each <stream>"_Stz);

  vAssert_Board_any();

  while (G_bEachMainWithSub(CK_PROJECT,CK_STREAM)) {
    M_vStream_Derive_OrdinalRange_project(G.p_stream);
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* derive the ordinal properties &lo..&hi range for each <stream>             */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS22_T24_Derive_ordinalRange_ForAll_aggreg_stream(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "calculate the ordinal $low..$high range for each <stream>"_Stz);

  vAssert_Board_any();

  while (G_bEachMainWithSub(CK_AGGREG,CK_STREAM)) {
    M_vStream_Derive_OrdinalRange_aggreg(G.p_stream);
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS22_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSTZZ_End();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                  vSeries_26_SubQuery_Analysis */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bS26_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! bEnabled)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS26_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS26_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vST00_Begin();

  m_pCardCtl->vUseShorter();

  m_pCardCtl->vPrintSubCards(true,CK_RPNCALC,0);
}

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

        //G.p_paramList->vDump();

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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS26_T30(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((30))"_Stz);

  vAssert_Board_mqA();

  vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS26_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSTZZ_End();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                     vSeries_32_To_BoolCalc */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bS32_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! bEnabled)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS32_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS32_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCardCtl->vUseShorter();

  vST00_Begin();
}

/*----------------------------------------------------------------------------*/
/* convert <where>//<rpnCalc> to <where>//<boolCalc>                          */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS32_T10_Convert_sqyOp_not_To_notSqyOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "rpcCalc of where becomes boolCalc "_Stz);

  vAssert_Board_any();

  while (G_bEachMainWithSub(CK_WHERE, CK_RPNCALC)) {
    M_vRpnCalc_Convert_sqyOp_not_To_notSqyOp(G.p_rpnCalc);
  }

  m_pCardCtl->vPrintSubCards(true,CK_RPNCALC,0);
  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* convert <where>//<rpnCalc> to <where>//<boolCalc>                          */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS32_T20_Convert_rpnCalc_To_boolCalc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "rpcCalc of where becomes boolCalc "_Stz);

  vAssert_Board_any();

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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS32_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSTZZ_End();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                    vSeries_33_To_TblColOdn */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bS33_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! bEnabled)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS33_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS33_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCardCtl->vUseShorter();

  vST00_Begin();
}

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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS33_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSTZZ_End();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                vSeries_34_Predicate_Pullup */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bS34_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! bEnabled)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS34_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS34_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vST00_Begin();

  m_pCardCtl->vUseShorter();
  m_pCardCtl->vPrintSubCards(true,CK_RPNCALC,CK_WHERE,0);

  vPrintBench(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/* put a table @c mark against where predicates which can be moved under table*/
/*----------------------------------------------------------------------------*/
void Xtransforms::vS34_T10_AtMark_where_pred_Wrt_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "put a table @c against where pred"_Stz);

  vAssert_Board_any();

  vPrintBoard(__LINE__);

  while (G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    G.p_boolCalc->vDump();

    vPullUp_MarkPredLeftwards(G.p_boolCalc, G.pDeck);
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* move where predicates with an @c table mark to new <table>/<filter>        */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS34_T20_Move_AtMarked_where_pred_To_table_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "move @c predicates to a filter under table"_Stz);

  vAssert_Board_any();

  while (G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    vPullUp_MovePredLeftwards(G.p_boolCalc, G.pDeck);
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* delete <where> if it is effectively empty                                  */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS34_T30_Delete_Empty_where(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "delete where if it is effectively empty"_Stz);

  vAssert_Board_any();

  while (G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    if (0 == G.p_boolCalc->nChildCards()) {
      G_vRemoveDeleteMain(CK_WHERE);
    }
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS34_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  //m_pCardCtl->vUseShorter();
  //m_pCardCtl->vPrintSubCards(false,CK_WHERE,0);

  vSTZZ_End();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                  vSeries_36_Bound_Analysis */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bS36_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! bEnabled)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS36_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS36_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vST00_Begin();

  m_pCardCtl->vUseShorter();

  vPrintBench(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/* copy <table>/<filter> up into <table>/<bounds>                             */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS36_T10_UpCopy_filter_pred_Into_bounds(void)
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
/* delete <table>/<filter>//<pred> that are now <table>/<bounds>//<pred>      */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS36_T20_Cull_UpCopied_filter_pred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "remove <table>/<filter>//<pred> that are now <table>/<joinPred>//<pred>"_Stz);

  vAssert_Board_any();

  while (G_bEachMainWithSub(CK_TABLE,CK_FILTER)) {
    G.p_boolCalc = G.p_filter->pSub_boolCalc();

    M_vBoolCalc_CullPred(G.p_boolCalc, MARK_UP);
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* delete any <table>/<filter> which is effectively empty                     */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS36_T30_Delete_Empty_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "delete any <table>/<filter> which is effectively empty"_Stz);

  vAssert_Board_any();

  while (G_bEachMainWithSub(CK_TABLE,CK_FILTER)) {
    vR_S36_T30_Delete_Empty_filter();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS36_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSTZZ_End();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                   vSeries_40_Flatten_Query */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bS40_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! bEnabled)
    return false;

  if (m_pBench->nItems() > 1)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS40_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS40_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCardCtl->vUseShorter();

  m_pCardCtl->vPrintSubCards(true,
                             CK_AGGELIST,CK_FILTER,CK_GBYELIST,
                             CK_PROJELIST,CK_RPNCALC,CK_STREAM,0);

  vST00_Begin();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS40_T05_Flatten_By_Decision_Tree(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((10))"_Stz);

  vAssert_Board_mqA();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // push a (from) sub-query before the main-query
  // as an op-sequence
  // (refer: Q7,8,9)

  auto L_vFold_10_F_Before_Mq_As_opSeq = [this] (void) {
    vS40_T10_Fold_F_Before_Mq_As_opSeq();
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // push a (single-value aggregate correlated ro) sub-query before the main-query
  // as an aggregation-fan (projecting key-of-correlation and aggregated value)
  // further: the key-of-correlation needs to be added to group-by)
  // (refer: Q2,17)

  auto L_vFold_20_W_A_C_ro_Before_Mq_As_AggFanKcorrAval_GbyKey = [this] (void) {
    vS40_T20_Fold_W_A_C_ro_Before_Mq_As_AggFanKcorrAval_GbyKey();
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // push a (single-value aggregate non-correlated in) sub-query before the main-query
  // as an aggregation-fan (projecting a single aggregated value)
  // (refer: Q18)

  auto L_vFold_30_W_A_NC_I_Before_Mq_As_aggFanAval = [this] (void) {
    vS40_T30_Fold_W_A_NC_I_Before_Mq_As_aggFanAval();
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // push a (single-value aggregate non-correlated in) sub-query before the main-query
  // as an aggregation-fan (projecting a single aggregated value)
  // (refer: Q15)

  auto L_vFold_40_W_A_NC_ro_Before_Mq_As_aggFanAval = [this] (void) {
    vS40_T40_Fold_W_A_NC_ro_Before_Mq_As_aggFanAval();
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // push a (linear correlated exists) sub-query before the main-query
  // as a keyExists.  join ordering will be required to make it runnable
  // (refer: Q4)

  auto L_vFold_50_W_L_C_E_Before_Mq_As_keyExists = [this] (void) {
    vS40_T50_Fold_W_L_C_E_Before_Mq_As_keyExists();
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // push a (linear non-correlated not-in) sub-query before the main-query
  // as a keyExists
  // (refer: Q16)

  auto L_vFold_60_W_L_NC_NI_Into_Mq_As_keyNotExists = [this] (void) {
    vS40_T60_Fold_W_L_NC_NI_Into_Mq_As_keyNotExists();
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // push a (single-value aggregate non-correlated ro) sub query before main queryy
  // as am aggregation-fan
  // further: the aggregation-value is injected/factored into the main-query
  // (refer: Q11)

  auto L_vFold_70_H_A_NC_ro_Before_Mq_As_AggFan_AvalInjectMq = [this] (void) {
    vS40_T70_Fold_H_A_NC_ro_Before_Mq_As_AggFan_AvalInjectMq();
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.pSubQuery = m_pBench->pGetBoard(C_SQ_BOARD)->pSubQuery();

  if (G.pSubQuery->bViaFrom())                /* Fsq_       Q7,8,9 */ L_vFold_10_F_Before_Mq_As_opSeq();
  else
  if (G.pSubQuery->bViaWhere()) {
    if (G.pSubQuery->bIsAggregation()) {
      if (G.pSubQuery->bIsCorrelated()) {
             if (G.pSubQuery->bByExists())    /* WAsq_C_E          */ NEVER_REACH
        else if (G.pSubQuery->bByNotExists()) /* WAsq_C_NE         */ NEVER_REACH
        else if (G.pSubQuery->bByIn())        /* WAsq_C_I          */ NEVER_REACH
        else if (G.pSubQuery->bByNotIn())     /* WAsq_C_NI         */ NEVER_REACH
        else                                  /* WAsq_C_ro   Q2,17 */ L_vFold_20_W_A_C_ro_Before_Mq_As_AggFanKcorrAval_GbyKey();
      } else {
             if (G.pSubQuery->bByExists())    /* WAsq_NC_E         */ NEVER_REACH
        else if (G.pSubQuery->bByNotExists()) /* WAsq_NC_NE        */ NEVER_REACH
        else if (G.pSubQuery->bByIn())        /* WAsq_NC_I     Q18 */ L_vFold_30_W_A_NC_I_Before_Mq_As_aggFanAval();
        else if (G.pSubQuery->bByNotIn())     /* WAsq_NC_NI        */ NEVER_REACH
        else                                  /* WAsq_NC_ro    Q15 */ L_vFold_40_W_A_NC_ro_Before_Mq_As_aggFanAval();
      }
    } else {
      if (G.pSubQuery->bIsCorrelated()) {
             if (G.pSubQuery->bByExists())    /* WLsq_C_E       Q4 */ L_vFold_50_W_L_C_E_Before_Mq_As_keyExists();
        else if (G.pSubQuery->bByNotExists()) /* WLsq_C_NE         */ NEVER_REACH
        else if (G.pSubQuery->bByIn())        /* WLsq_C_I          */ NEVER_REACH
        else if (G.pSubQuery->bByNotIn())     /* WLsq_C_NI         */ NEVER_REACH
        else                                  /* WLsq_C_ro         */ NEVER_REACH
      } else {
             if (G.pSubQuery->bByExists())    /* WLsq_NC_E         */ NEVER_REACH
        else if (G.pSubQuery->bByNotExists()) /* WLsq_NC_NE        */ NEVER_REACH
        else if (G.pSubQuery->bByIn())        /* WLsq_NC_I         */ NEVER_REACH
        else if (G.pSubQuery->bByNotIn())     /* WLsq_NC_NI    Q16 */ L_vFold_60_W_L_NC_NI_Into_Mq_As_keyNotExists();
        else                                  /* WLsq_NC_ro        */ NEVER_REACH
      }
    }
  }
  else
  if (G.pSubQuery->bViaHaving()) {
    if (G.pSubQuery->bIsAggregation()) {
      if (G.pSubQuery->bIsCorrelated()) {
             if (G.pSubQuery->bByExists())    /* HAsq_C_E          */ NEVER_REACH
        else if (G.pSubQuery->bByNotExists()) /* HAsq_C_NE         */ NEVER_REACH
        else if (G.pSubQuery->bByIn())        /* HAsq_C_I          */ NEVER_REACH
        else if (G.pSubQuery->bByNotIn())     /* HAsq_C_NI         */ NEVER_REACH
        else                                  /* HAsq_C_ro         */ NEVER_REACH
      } else {
             if (G.pSubQuery->bByExists())    /* HAsq_NC_E         */ NEVER_REACH
        else if (G.pSubQuery->bByNotExists()) /* HAsq_NC_NE        */ NEVER_REACH
        else if (G.pSubQuery->bByIn())        /* HAsq_NC_I         */ NEVER_REACH
        else if (G.pSubQuery->bByNotIn())     /* HAsq_NC_NI        */ NEVER_REACH
        else                                  /* HAsq_NC_ro    Q11 */ L_vFold_70_H_A_NC_ro_Before_Mq_As_AggFan_AvalInjectMq();
      }
    } else {
      if (G.pSubQuery->bIsCorrelated()) {
             if (G.pSubQuery->bByExists())    /* HLsq_C_E          */ NEVER_REACH
        else if (G.pSubQuery->bByNotExists()) /* HLsq_C_NE         */ NEVER_REACH
        else if (G.pSubQuery->bByIn())        /* HLsq_C_I          */ NEVER_REACH
        else if (G.pSubQuery->bByNotIn())     /* HLsq_C_NI         */ NEVER_REACH
        else                                  /* HLsq_C_ro         */ NEVER_REACH
      } else {
             if (G.pSubQuery->bByExists())    /* HLsq_NC_E         */ NEVER_REACH
        else if (G.pSubQuery->bByNotExists()) /* HLsq_NC_NE        */ NEVER_REACH
        else if (G.pSubQuery->bByIn())        /* HLsq_NC_I         */ NEVER_REACH
        else if (G.pSubQuery->bByNotIn())     /* HLsq_NC_NI        */ NEVER_REACH
        else                                  /* HLsq_NC_ro        */ NEVER_REACH
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
void Xtransforms::vS40_T10_Fold_F_Before_Mq_As_opSeq(void)
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
void Xtransforms::vS40_T20_Fold_W_A_C_ro_Before_Mq_As_AggFanKcorrAval_GbyKey(void)
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
  m_pTerm_corrPred->vDump();
  R_vUTE_Find_stmCol_Definition_Of_Correlation_Column();
  RASSERT(G.p_stmCol);       // eg L_PARTKEY
  G.p_stmCol->vDump();


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
void Xtransforms::vS40_T30_Fold_W_A_NC_I_Before_Mq_As_aggFanAval(void)
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
void Xtransforms::vS40_T40_Fold_W_A_NC_ro_Before_Mq_As_aggFanAval(void)
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
      G.p_stream = C_pProduce_stream_Of_aggreg((Card_AggElist*)(G.p_projElist));
      G.p_aggreg->vAppend(G.p_stream);
      RASSERT(G_bEachMainWithSub(CK_AGGREG, CK_PROJELIST));
      G.p_stream = C_pProduce_stream_Of_aggreg((Card_AggElist*)(G.p_projElist));
      G.p_aggreg->vAppend(G.p_stream);
      RASSERT(G_bEachMainWithSub(CK_AGGREG, CK_PROJELIST));
      G.p_stream = C_pProduce_stream_Of_aggreg((Card_AggElist*)(G.p_projElist));
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
void Xtransforms::vS40_T50_Fold_W_L_C_E_Before_Mq_As_keyExists(void)
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
void Xtransforms::vS40_T60_Fold_W_L_NC_NI_Into_Mq_As_keyNotExists(void)
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
void Xtransforms::vS40_T70_Fold_H_A_NC_ro_Before_Mq_As_AggFan_AvalInjectMq(void)
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS40_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSTZZ_End();
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
/*----------------------------------------------------------------------------*/
/*                                                      vSeries_60_Index_Join */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bS60_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! bEnabled)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS60_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS60_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCardCtl->vUseShorter();

  m_pBench->vPrefix('m');

  vST00_Begin();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS60_T05_Twist_in_Becomes_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "add an <indexJoin> under all joinable <table>'s"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMainWithSub(CK_TWIST, CK_ROPRED, "in"_Stz)) {
    vR_S60_T05_Twist_in_Becomes_filter();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* add a scratch <maybeIndexJoin> under all joinable <table>'s                */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS60_T10_Produce_maybeIndexJoin_Under_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "add an <indexJoin> under all joinable <table>'s"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMainWithSub(CK_TABLE, CK_FILTER)) {
    M_vTable_Produce_MaybeIndexJoin(G.p_table);
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* affix $c.n to each <idxCol> wrt table stream                               */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS60_T20_Affix_tblColOdn_ToAll_idxCol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "Affix $c.n to each <idxCol> wrt table stream"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMainWithSub(CK_TABLE,CK_MAYBEINDEXJOIN)) {
    G.p_stream = G.p_table->pSub_stream();;
    G.p_stmColList = G.p_table->pSub_stmColList();
    G.p_idxColList = G.p_maybeIndexJoin->pSub_idxColList();

    M_vIdxColList_Affix_tblColOdn(G.p_idxColList, G.p_stream);
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* delete any <table>/<filter> which is effectively empty                     */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS60_T60_Delete_Twist(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((desc))"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMain(CK_TWIST)) {
    G_vRemoveDeleteMain(CK_TWIST);
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS60_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSTZZ_End();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                    vSeries_75_EE_operators */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bS75_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! bEnabled)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS75_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS75_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCardCtl->vUseShorter();

  vST00_Begin();
}

/*----------------------------------------------------------------------------*/
/* convert all <table> into either <scan> , <indexJoin> or <keyExists>        */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS75_T05_ChooseIndexes()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "convert all <table> into either <scan> or <indexJoin>"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMainWithSub(CK_TABLE,CK_INDEX)) {
    G.p_index->Y_vSet(P_ChosenIndex);
  }

  while (G_bEachMainWithSub(CK_KEYEXISTS,CK_INDEX)) {
    G.p_index->Y_vSet(P_ChosenIndex);
  }

  vEndTransform();
}

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
    G.p_maybeIndexJoin = G.p_table->pSub_maybeIndexJoin();

    vTrace("MainCard %p : [%lld] %s %s\n"_Stz,
           G.p_table,G.p_table->iCardNo(),aCK[G.p_table->eCard()],G.p_table->szValue());

    vR_S75_T10_Morph_table_To_EEop();

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
void Xtransforms::vS75_T30_Correct_stmColOdn_Of_keyExists_stream(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((description))"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMainWithSub(CK_KEYEXISTS,CK_STREAM)) {
    vR_S75_T30_Correct_stmColOdn_Of_keyExists_stream();
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS75_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSTZZ_End();
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
void Xtransforms::vS80_T10_bounds_For_scan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "append <indexList> to <scan>"_Stz);

  vAssert_Board_mqA();

  if (G_bEachMainWith2Sub(CK_SCAN,CK_TABLE,CK_INDEXLIST)) {

    G.p_index = G.p_indexList->pSub_index();
    G.p_bounds = G.p_scan->pSub_bounds();
    char cTblSyn = G.p_table->cValue();
    
    if (G_bEachMainWithSub(CK_WHERE,CK_BOOLCALC)) {

      // some /<boolCalc>/<PRED> which is a bound of an index under scan
      Card_PRED *p_rootPRED = (Card_PRED *) G.p_boolCalc->pGetChild(0);

      Card_PRED *p_boundingPRED = T_pPRED_Containing_MaybeBoundsOf_index(p_rootPRED, G.p_index, false);







      //p_boundingPRED->vDump();

      // mark such <PRED> & copy as a bound
      if (p_boundingPRED) {
        p_boundingPRED->vMark1(MARK_AT);

        RASSERT(! G.p_bounds);
//        G.p_bounds = new Card_Bounds;
//        G.p_index->vAppend(G.p_bounds);
//        G.p_bound = C_pBound_CreateFrom_PRED(p_boundingPRED, cTblSyn);
//        G.p_bounds->vAppend(G.p_bound);
        G.p_bounds = C_pBounds_CreateFrom_PRED(p_boundingPRED, cTblSyn);
        G.p_index->vAppend(G.p_bounds);
        G.p_bounds->vDump();
      }
    }
  }

  vPrintBoard();

  // cull pred
  M_vBoolCalc_CullPred(G.p_boolCalc,MARK_AT);

  // delete empty where

  G_vRestart();
  while (G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    if (0 == G.p_boolCalc->nChildCards()) {
      G_vRemoveDeleteMain(CK_WHERE);
    }
  }

  G.pDeck->vRenumberRecursive();
  vPrintBoard();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G_vRestart();
  if (G_bEachMainWithSub(CK_SCAN,CK_BOUNDS)) {

//    if (T_bBounds_Is_lbound(G.p_bounds))
//      G.p_bounds->vCard(CK_LBOUND);
//    else
//    if (T_bBounds_Is_ubound(G.p_bounds))
//      G.p_bounds->vCard(CK_UBOUND);

    if (G.p_bounds->pSub_lBound() || G.p_bounds->pSub_uBound()) {
      G.p_scan->vCard(CK_INDEXSCAN);
    }
  }

  vPrintBoard();

  vEndTransform();
}


/*----------------------------------------------------------------------------*/
/* append <indexList> to <scan>                                               */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS80_TF1_Produce_Scratch_indexList_Under_scan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "append <indexList> to <scan>"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMainWith2Sub(CK_SCAN, CK_TABLE, CK_TABLENM)) {
    M_vScan_Produce_indexList(G.p_scan);
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* convert the column name in every <idxCol> to a $streamOrdinal              */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS80_TF2_Affix_idxColOdn_ToAll_idxCol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "convert the column name in every <idxCol> to a $streamOrdinal"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMainWith2Sub(CK_SCAN, CK_STREAM, CK_IDXCOLLIST)) {
    M_vIdxColList_Affix_tblColOdn(G.p_idxColList, G.p_stream);
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS80_TF3_Mark_ULRangeOf_scan_filter_cmpPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMainWithSub(CK_SCAN, CK_FILTER)) {
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    auto bfMarkPredBounds = [this](Card* pCard) {
      Card_CmpPred* p_cmpPred = _pCard_CmpPred(pCard);
      M_vCmpPred_MarkBound(p_cmpPred);
      return C_CONTINUE_RECURSION;
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    G.p_filter->bApplyRecursive(CK_CMPPRED, bfMarkPredBounds);
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* mark the <SCAN>//<filter>//cmpPred> which match index ordinals             */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS80_TF4_Mark_IndexMatchOf_scan_filter_terms(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "mark <SCAN>//<filter>//<term> ordinals which match index"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMainWith2Sub(CK_SCAN, CK_FILTER, CK_INDEXLIST)) {
    if (! (G.p_index = _pCard_Index(G.p_indexList->pGetChild(0))))
      continue;
    if (! (G.p_idxColList = (G.p_index->pSub_idxColList())))
      continue;

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    auto bfPredBounds = [this](Card* pCard) {
      Card_Term* p_term = _pCard_Term(pCard);
      M_vTerm_MarkIfMatchesAn_idxCol(p_term, G.p_idxColList);
      return C_CONTINUE_RECURSION;
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    G.p_filter->bApplyRecursive(CK_TERM, bfPredBounds);
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS80_TF5_Add_scan_bounds_ByCopyingMarked_filter_cmpPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMainWith2Sub(CK_SCAN, CK_FILTER, CK_INDEXLIST)) {
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    auto bfAdd = [this](Card* pCard) {
      G.p_cmpPred = _pCard_CmpPred(pCard);
      M_vScan_Add_cmpPred_To_bounds(G.p_scan, G.p_cmpPred);
      return C_CONTINUE_RECURSION;
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    G.p_filter->bApplyRecursive(CK_CMPPRED, bfAdd);
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS80_TF6_Cull_Marked_filter_cmpPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMainWithSub(CK_SCAN, CK_FILTER)) {
    vTracer("Culling <filter> of L & U bounds %s\n"_Stz,G.p_filter->szBearing());
    M_vFilter_CullPred(G.p_filter, 'L');
    M_vFilter_CullPred(G.p_filter, 'U');

    //G.p_filter->vDump();
    if (!T_bFilter_IsEmpty(G.p_filter))
      continue;

    vTracer("Deleting empty <filter> %s\n"_Stz,G.p_filter->szBearing());
    G.p_MainCard->vRemoveSubCard(G.p_filter);
    DELETE_CARD(G.p_filter);
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS80_TF7_Morph_scan_To_indexScan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMainWithSub(CK_SCAN, CK_BOUNDS)) {
    //vTracer("Morphing <scan> to <indexScan> %s\n"_Stz,G.p_scan->szBearing());
    G.p_scan->vCard(CK_INDEXSCAN);
  }

  G_vRestart();
  while (G_bEachMain(CK_INDEXSCAN)) {
    i8 i_Child;
    G.p_note = _pCard_Note(G.p_indexScan->pFindChild(CK_NOTE,i_Child));
    G.p_note->vValue(SEP_DERIVED);
  }

  G_vRestart();
  while (G_bEachMainWithSub(CK_INDEXSCAN, CK_INDEXLIST)) {
    G.p_indexScan->vReplaceChildWithItsSubs(G.p_indexList);
    DELETE_CARD((G.p_indexList));
  }

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
/*                                                      vSeries_85_Cross_Join */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bS85_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! bEnabled)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS85_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS85_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCardCtl->vUseShorter();

  vST00_Begin();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS85_T10_Transmute_All_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((85 10))"_Stz);

  vAssert_Board_mqA();

  if (G_bEachMain(CK_TABLE)) {
    G.p_scan = new Card_Scan;
    G.p_scan->vTransferChildren(G.p_table);

    G.pBoard->pDeck()->vReplace(G.p_table,G.p_scan);
    G.p_scan->vInsert(G.p_table);

    while (G_bEachMain(CK_TABLE)) {
      if ( (! G.p_table->pSub_indexJoin())
           && (! G.p_table->pSub_keyExists())
           && (! G.p_table->pSub_keyNotExists())
           && (! G.p_table->pSub_bounds())
          ) {
        G.p_crossJoin = new Card_CrossJoin;
        G.p_crossJoin->vTransferChildren(G.p_table);

        G.pBoard->pDeck()->vReplace(G.p_table,G.p_crossJoin);
        G.p_crossJoin->vInsert(G.p_table);
      }
      break;
    }
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS85_T20_Add_index_ForAll_crossJoin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((85 20))"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMainWithSub(CK_CROSSJOIN,CK_TABLENM)) {
    m_pTable = A_pAccess_Table(G.p_tableNm->szValue());
    G.p_index = C_pProduce_index_Of_table(m_pTable);

    G.p_note = new Card_Note(SEP_DERIVED);
    G.p_crossJoin->vAppend(G.p_note);

    G.p_crossJoin->vAppend(G.p_index);
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS85_T30_Add_bounds_ForAll_crossJoin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((85 20))"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMainWithSub(CK_CROSSJOIN,CK_INDEX)) {
    G_vNewMember(CK_BOUNDS);
    G.p_crossJoin->vAppend(G.p_bounds);

    G_vNewMember(CK_TERM,"bound"_Stz);
    G.p_bounds->vAppend(G.p_term);

    G_vNewMember(CK_TERM,"$I1"_Stz);
    G.p_bounds->vAppend(G.p_term);
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS85_TZZ_End(void)
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
void Xtransforms::vS90_T30_Add_stmOrdList(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "remove <sort> if <sbyElist> is the same as the <gbyElist>"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMain()) {

    if (! G.p_MainCard->bIsCard(CK_GBYAGG))
      continue;

    R_vUTE_Derive_stmOrdList_For_gbyAgg();

    if (G_bIsNextMain(CK_GBYAGG)) {
      G_bEachMain();
      R_vUTE_Derive_stmOrdList_For_gbyAgg();

      if (G_bIsNextMain(CK_GBYAGG)) {
        G_bEachMain();
        R_vUTE_Derive_stmOrdList_For_gbyAgg();
      }
    }

  }

  G.pDeck->vRenumberRecursive();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G_vRestart();
  while (G_bEachMain()) {

    if (!G.p_MainCard->bIsCard(CK_GBYAGG))
      continue;

    G.p_stmOrdList = G.p_gbyAgg->pSub_stmOrdList();
    while (G_bIsNextMain(CK_GBYAGG)) {
      G_bEachMain();
      G.p_stmOrdList = G.p_gbyAgg->pSub_stmOrdList();
    }

    if (G_bIsNextMain(CK_PROJECT)) {
      G_bEachMain();
      R_vUTE_Derive_stmOrdList_For_project();
    }

    break;
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* remove <sort> if <sbyElist> is the same as the <gbyElist>                  */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS90_TF0_Remove_sort_IfSameAs_gby(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "remove <sort> if <sbyElist> is the same as the <gbyElist>"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMain(CK_SORT)) {
    if (! G_bHasPriorToMainIf(CK_AGGREG, C_UPDATE_MEMBER))
      break;

    G.p_gbyElist = G.p_aggreg->pSub_gbyElist();
    G.p_sbyElist = G.p_sort->pSub_sbyElist();

    // TODO: needs to be a T_ test
    if (! G.p_gbyElist->bValuesSameRecursive(G.p_sbyElist))
      continue;

    G_vRemoveDeleteMain(CK_SORT);
  }

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
/*                                                  vSeries_92_Ordered_Aggreg */
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

  m_pCardCtl->vUseShorter();

  vST00_Begin();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS92_T10(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((10))"_Stz);

  vAssert_Board_mqA();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS92_T20(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((20))"_Stz);

  vAssert_Board_mqA();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS92_T30(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((30))"_Stz);

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

  while (G_bEachMain()) {

    if (! (G.p_MainCard->bIsCard(CK_INDEXJOIN) ||
           G.p_MainCard->bIsCard(CK_INDEXJOIN)))
      continue;

    if (! (G.p_bounds = G.p_MainCard->pSub_bounds()))
      continue;
    if (! (G.p_filter = G.p_MainCard->pSub_filter()))
      continue;

    // todo: complete...
  }
}

/*----------------------------------------------------------------------------*/
/* individuate <scan|indexJoin>/<filter> as main <filter>                     */
/*----------------------------------------------------------------------------*/
void Xtransforms::vSA0_T20_Individuate_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "individuate <scan|indexJoin>/<filter> as main <filter>"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMainWithSub(CK_SCAN,CK_FILTER)) {
    G.p_scan->vRemoveChild(G.p_filter);
    G.pDeck->vInsertAfter(G.p_filter, G.p_scan);

    if (! G.p_filter->pSub_level()) {
      G.p_filter->vPush(G.p_scan->pSub_level()->pDupCardAndSubCards());
    }
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
/*                                                vSeries_Z0_Restate_Pipeline */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bSB0_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! bEnabled)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vSB0_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vSB0_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCardCtl->vUseShorter();

  vST00_Begin();
}



/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vSB0_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSTZZ_End();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                       vSeries_E0_To_StmOdn */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bSE0_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! bEnabled)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vSE0_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vSE0_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCardCtl->vUseLonger();

  vST00_Begin();
}

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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vSE0_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSTZZ_End();
}



/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                vSeries_Z0_Restate_Pipeline */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bSZ0_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! bEnabled)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vSZ0_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vSZ0_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCardCtl->vUseShorter();

  m_pBench->vPrefix('w');

  vST00_Begin();
}

/*----------------------------------------------------------------------------*/
/* reinstate stages                                                           */
/*----------------------------------------------------------------------------*/
void Xtransforms::vSZ0_T20_Reinstate_stages(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "reinstate stages"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMain() && G_bHasSubsequentToMain(C_UPDATE_MEMBER))  {
    vR_SZ0_T20_Reinstate_stages();
  }

  i8 i_Stage = 1;
  G_vRestart();
  while (G_bEachMain(CK_STAGE))
    G.p_stage->vValueSprint("S%02lld"_Stz,i_Stage++);

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* reinstate stages                                                           */
/*----------------------------------------------------------------------------*/
void Xtransforms::vSZ0_T20_Reinstate_stagesV2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "reinstate stages"_Stz);

  vAssert_Board_mqA();

  G.e_Level = AL_PARTITION;

//  G.p_stage = new Card_Stage;
//
//  G.p_topology = new Card_Topology(YG_ALL_PARTITIONS);
//  G.p_stage->vAppend(G.p_topology);
//  G.pDeck->vPush(G.p_stage);

  G.p_PriorToMain = nullptr;
  while (G_bEachMain())  {
    vR_SZ0_T20_Reinstate_stagesV2();
  }

  i8 i_Stage = 1;
  G_vRestart();
  while (G_bEachMain(CK_STAGE))
    G.p_stage->vValueSprint("S%02lld"_Stz,i_Stage++);

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* reinstate stages                                                           */
/*----------------------------------------------------------------------------*/
void Xtransforms::vSZ0_T20_Reinstate_stagesV3(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "reinstate stages"_Stz);

  vAssert_Board_mqA();

  vPrintBoard(__LINE__);

  vR_SZ0_T20_Reinstate_stagesV3();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vSZ0_T30_Reinstate_combined(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMain()) {
    G_vSubsequentToMain();
    if (T_bCombined_Reinstate(G.e_MainCard, G.e_SubsequentToMain)) {
      vR_SZ0_T30_Reinstate_combined();
      G_vSkipNextMain();
    }
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vSZ0_T40_Reinstate_next(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMain(CK_STAGE)) {
    if (! G.i_MainCard)
      continue;

    G.p_MainCard->vDump();

    vR_SZ0_T40_Reinstate_next();
  }

  vR_SZ0_T40_Reinstate_next_b();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vSZ0_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSTZZ_End();
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

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vSH_DeleteMain(eCK eCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (G_bEachMain(eCard)) {
    G_vRemoveDeleteMain(eCard);
  }

}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                    vSeries_35X_Flatten_From */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bS35X_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! bEnabled)
    return false;

  SubQuery* pSubQuery = m_pBench->pGetBoard(1)->pSubQuery();

  if (QC_FROM_CLAUSE == pSubQuery->eClause())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS35X_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS35X_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vST00_Begin();

  m_pCardCtl->vUseShorter();

  //m_pCardCtl->vPrintSubCards(true,CK_AGGELIST,CK_GBYELIST,CK_PROJELIST,CK_STREAM,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS35X_T10_Push_sqB_Into_mqA(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((10))"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMain(CK_CALL)) {

    Stz* szSubQueryNm = G.p_call->szValue();

    if (! G_bPushBoard(szSubQueryNm)) {
      vTracer("DID NOT FIND Board: %s\n"_Stz,szSubQueryNm);
      continue;
    }

    vR_S35X_T10a_Copy_From_SubQuery();

    G_vPopBoard();

    vR_S35X_T10b_Push_sqB_Before_mqA();
    break;
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS35X_T20_Delete_call(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((20))"_Stz);

  vAssert_Board_mqA();

  vSH_DeleteMain(CK_CALL);

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS35X_T30_Delete_twist(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((30))"_Stz);

  vAssert_Board_mqA();

  vSH_DeleteMain(CK_TWIST);

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS35X_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSTZZ_End();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                   vSeries_36X_Flatten_Where */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bS36X_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! bEnabled)
    return false;

  SubQuery* pSubQuery = m_pBench->pGetBoard(1)->pSubQuery();

  if (QC_WHERE_CLAUSE == pSubQuery->eClause())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS36X_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS36X_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vST00_Begin();

  m_pCardCtl->vUseShorter();

  //m_pCardCtl->vPrintSubCards(true,CK_AGGELIST,CK_GBYELIST,CK_PROJELIST,CK_STREAM,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS36X_T04_Mark_pmzWhere_In_sqB(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((04))"_Stz);

  vAssert_Board_mqA();

//  if (G_bPushBoard(1)) {
//
//    while (G_bEachMain(CK_WHERE)) {
//      vR_S36X_T104_Mark_pmzWhere_In_sqB();
//    }
//
//    G_vPopBoard();
//  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS36X_T05_Pullup_pmzWhere_Into_mqA(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((04))"_Stz);

  vAssert_Board_mqA();

  vR_S36X_T105_Pullup_pmzWhere_Into_mqA();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS36X_T10_Push_sqB_Into_mqA(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((yank from subquery))"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMainWithSub(CK_WHERE, CK_ROPRED, "exists"_Stz)) {

    Stz* szSubQueryNm = G.p_roPred->pGetChild(1)->szValue();
    vTracer("SubQueryNm: %s\n"_Stz,szSubQueryNm);

    if (! G_bPushBoard(szSubQueryNm)) {
      vTracer("DID NOT FIND Board: %s\n"_Stz,szSubQueryNm);
      continue;
    }

    vR_S36X_T10a_Copy_From_SubQuery();

    G_vPopBoard();

    vR_S36X_T10b_Push_sqB_Before_mqA();

    break;
  }

  G.pDeck->vRenumberRecursive();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS36X_T20_Transfer_whereB_To_whereA(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "Transfer where-B to where-A"_Stz);

  vAssert_Board_mqA();

  G.pDeck->vDump();

  while (G_bEachMain(CK_WHERE)) {
    vR_S36X_T20a_Get_cmpPred_Of_whereB();
    break;
  }

  while (G_bEachMain(CK_WHERE)) {
    vR_S36X_T20b_Push_cmpPred_Onto_whereA();
  }

//  G_vRestart();
//  while (G_bEachMainWithSub(CK_TWIST,CK_COMMENT,"- xfer whereB"_Stz)) {
//    G.p_comment->vValue("+ xfer whereB"_Stz);
//  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS36X_T30_Replace_param_In_whereA(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "Replace param in where-A"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMainWithSub(CK_WHERE,CK_BOOLCALC)) {
    vR_S36X_T30a_Replace_param_In_whereA();
    break;
  }

  G_vRestart();
  while (G_bEachMain(CK_PARAMLIST)) {
    vR_S36X_T30b_Delete_param_In_whereB();
  }

//  G_vRestart();
//  while (G_bEachMainWithSub(CK_TWIST,CK_COMMENT,"- paramListB"_Stz)) {
//    G.p_comment->vValue("+ paramListB"_Stz);
//  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS36X_T40_Delete_tstPred_In_whereA(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "Delete tstPred in where-A"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMainWithSub(CK_WHERE,CK_BOOLCALC)) {
    vR_S36X_T40_Delete_tstPred_In_whereA();
    break;
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS36X_T50_Adjust_StmOdn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "Adjust stream lo/ho ordinals"_Stz);

  vAssert_Board_mqA();

  vR_S36X_T50_Adjust_StmOdn();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS36X_T60_Adjust_StmColOdn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "Adjust stream lo/ho ordinals"_Stz);

  vAssert_Board_mqA();

  vR_S36X_T60_Adjust_StmColOdn();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS36X_T70_Replace_term_In_cmpPred(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "AReplace term in cmpPred"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMainWithSub(CK_WHERE,CK_BOOLCALC)) {
    vR_S36X_T70_Replace_term_In_cmpPred();
  }

  vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS36X_T80_Delete_twist(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "AReplace term in cmpPred"_Stz);

  vAssert_Board_mqA();

//  while (G_bEachMain(CK_TWIST)) {
//    G_vRemoveDeleteMain(CK_TWIST);
//    break;
//  }

  vSH_DeleteMain(CK_TWIST);

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS36X_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSTZZ_End();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                  vSeries_37X_Flatten_Having */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bS37X_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! bEnabled)
    return false;

  SubQuery* pSubQuery = m_pBench->pGetBoard(1)->pSubQuery();

  if (QC_HAVING_CLAUSE == pSubQuery->eClause())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37X_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37X_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vST00_Begin();

  m_pCardCtl->vUseShorter();

  m_pCardCtl->vPrintSubCards(false,CK_STREAM,0);

  //m_pCardCtl->vPrintSubCards(true,CK_AGGELIST,CK_GBYELIST,CK_PROJELIST,CK_STREAM,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37X_T10_Append_sqB_After_mqA(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "Append subQuery-B after mainQuery-A"_Stz);

  vAssert_Board_mqA();

  //while (G_bEachMainWithSub(CK_WHERE, CK_ROPRED, "exists"_Stz)) {

  //Stz* szSubQueryNm = G.p_roPred->pGetChild(1)->szValue();
  Stz* szSubQueryNm = "__subquery0002"_Stz;
  vTracer("SubQueryNm: %s\n"_Stz,szSubQueryNm);

  if (! G_bPushBoard(szSubQueryNm)) {
    vTracer("DID NOT FIND Board: %s\n"_Stz,szSubQueryNm);
    NEVER_REACH;
  }

  vR_S37X_T10a_Copy_sqB();

  G_vPopBoard();

  vR_S37X_T10a_Append_sqB_After_mqA();

  //break;
  //}

  G.pDeck->vRenumberRecursive();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37X_T20_Find_stmCol_Link(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((30))"_Stz);

  vAssert_Board_mqA();

  while (G_bResolveCard(CK_PROJECT,C_UNKNOWN)) {
    vR_S37X_T20_Find_stmCol_Link();
    break;
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37X_T30_Link_sqCall_In_having(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((20))"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMainWithSub(CK_HAVING,CK_RPNCALC)) {
    vR_S37X_T30_Link_sqCall_In_having();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37X_T35_Bump_gbyElist_rpnCalc_In_sqB(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((20))"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMainWithSub(CK_GBYAGG, CK_GBYELIST)) {
    vR_S37X_T35_Bump_gbyElist_rpnCalc_In_sqB();
  }

  vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37X_T40_Add_sqCol_To_aggGbyElist(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((30))"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMain()) {

    if (G.e_MainCard == CK_HAVING)
      break;

    if (G.e_MainCard != CK_GBYAGG)
      continue;

    vR_S37X_T40Add_sqCol_To_AggGbyElist();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37X_T45_Bump_aggElist_rpnCalc_In_sqB(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((20))"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMainWithSub(CK_GBYAGG, CK_AGGELIST)) {
    vR_S37X_T45_Bump_aggElist_rpnCalc_In_sqB();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37X_T50_Add_sqCol_To_aggAggElist(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((30))"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMain()) {

    if (G.e_MainCard == CK_HAVING)
      break;

    if (G.e_MainCard != CK_GBYAGG)
      continue;

    vR_S37X_T50_Add_sqCol_To_aggAggElist();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37X_T55_Bump_having_rpnCalc_In_sqB(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((20))"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMainWithSub(CK_HAVING,CK_RPNCALC)) {
    vR_S37X_T55_Bump_having_rpnCalc_In_sqB();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37X_T60_Unproject_sqCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((30))"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMain(CK_TWIST)) {

    vR_S37X_T60_Unproject_sqCol();
    break;
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37X_T70_Exhange_sqB_With_mqA(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((30))"_Stz);

  vAssert_Board_mqA();

  while (G_bEachMain(CK_TWIST)) {

    vR_S37X_T70_Exhange_sqB_With_mqA();
    break;
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37X_T80_Adjust_StmOdn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((30))"_Stz);

  vAssert_Board_mqA();

  vR_S37X_T80_AdjustStmOdn();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37X_T90_Adjust_StmColOdn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((30))"_Stz);

  vAssert_Board_mqA();

  vR_S37X_T90_Adjust_StmColOdn();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37X_TA0_Delete_twist(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((30))"_Stz);

  vAssert_Board_mqA();

  vSH_DeleteMain(CK_TWIST);

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37X_TB0(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((30))"_Stz);

  vAssert_Board_mqA();

  vR_S37X_TB0();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37X_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSTZZ_End();
}


