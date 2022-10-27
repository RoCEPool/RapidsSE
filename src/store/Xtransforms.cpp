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
/*----------------------------------------------------------------------------*/
/*                                                    Reduce Pipeline Hand 10 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* linearize board by deleting all duplicate <stage>'s                        */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH10_T10_Delete_Redundant_stage(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("linearize board by deleting all duplicate <stage>'s"_Stz);
  vTransform((char *) (__func__), C_PRINT_XFORM);
  vTracing(bPrint,bTrace);

  //M.pDeck->vDump(0,(Stz*)(__func__));

  Card_Stage* p_stage_CURR = nullptr;

  while (E_bEachMain(CK_STAGE)) {
    if (! p_stage_CURR || !M.p_stage->bEquivalentTo(p_stage_CURR))   {
      p_stage_CURR = M.p_stage;
      continue;
    }

    vR_H10_T10_Delete_Redundant_stage();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* lhide ignorable <stage>'s                                                  */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH10_T15_Hide_Ignorable_stage(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("hide ignorable <stage>'s"_Stz);
  vTransform((char *) (__func__), C_PRINT_XFORM);
  vTracing(bPrint,bTrace);

  //M.pDeck->vDump(0,(Stz*)(__func__));

  while (E_bEachMain(CK_STAGE)) {
    if (! (M.p_topology = M.p_stage->pSub_topology()) )
      continue;
    if (YG_ALL_PARTITIONS == (eYG) M.p_topology->Y_iVal(P_StageTopo))
      continue;

    vR_H10_T15_Hide_Ignorable_stage();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* delete superfluous <absJoin>'s                                             */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH10_T20_Delete_absJoin(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("delete superfluous <absJoin>'s"_Stz);
  vTransform((char *) (__func__), C_PRINT_XFORM);
  vTracing(bPrint,bTrace);

  //M.pDeck->vDump(0,(Stz*)(__func__));

  while (E_bEachMain(CK_ABSJOIN)) {
    vR_H10_T20_Delete_absJoin();
   }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* delete superfluous <combined>'s                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH10_T30_Delete_combined(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("delete superfluous <combined>'s"_Stz);
  vTransform((char *) (__func__), C_PRINT_XFORM);
  vTracing(bPrint,bTrace);

  //M.pDeck->vDump(0,(Stz*)(__func__));

  while (E_bEachMain(CK_COMBINED)) {
    vR_H10_T30_Delete_combined();
  }

    vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* delete superfluous <send>'s                                                */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH10_T40_Delete_send(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("delete superfluous <send>'s"_Stz);
  vTransform((char *) (__func__), C_PRINT_XFORM);
  vTracing(bPrint,bTrace);

  //M.pDeck->vDump(0,(Stz*)(__func__));

  while (E_bEachMain(CK_SEND)) {
    vR_H10_T40_Delete_send();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* replace all <scan>'s with their children                                   */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH10_T50_Cull_scan_Leaving_table(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("replace all <scan>'s with their children"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  //M.pDeck->vDump(0,(Stz*)(__func__));

  while (E_bEachMain(CK_SCAN)) {
    vR_H10_T50_Cull_scan_Leaving_table();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                    Access Metadata Hand 20 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* add a <tableSyn> to all <table>'s                                          */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH20_T10_Add_tableSyn_ForAll_table(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("add a <tableSyn> to all <table>'s"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  //M.pDeck->vDump();

  while (E_bEachMainWithSub(CK_TABLE,CK_TABLENM))  {
    vR_H20_T10_Add_tableSyn_ForAll_table();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* add a <tablePtn> under every <table>                                       */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH20_T20_Add_tablePtn_UnderAll_table(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("add a <tablePtn> under every <table>"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (E_bEachMainWithSub(CK_TABLE,CK_TABLENM)) {
    vR_H20_T20_Add_tablePtn_UnderAll_table();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* add a <stream> under every <table>                                         */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH20_T30_Add_stream_UnderAll_table(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("add a <stream> under every <table>"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  // E <scan>
  while (E_bEachMainWithSub(CK_TABLE,CK_TABLENM)) {
    vR_H20_T30_Add_stream_UnderAll_table();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                   Stream Analysis Hand 30 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* calculate the ordinal $low..$high range for each <stream>                  */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH30_T10_Calc_ordinalRange_ForAll_stream(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("calculate the ordinal $low..$high range for each <stream>"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (E_bEachMain(CK_TABLE)) {
    vR_H30_T10_Calc_ordinalRange_ForAll_stream();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                  Predicate Pulluo Hand 40 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* convert <where>//<calc> to <where>//<calcBool>                             */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH40_T10_Convert_where_calc_To_where_calcBool(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("convert <where>//<calc> into <where>//<calcBool> "_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (E_bEachMainWithSub(CK_WHERE, CK_RPNCALC)) {
    vR_H40_T10_Convert_where_calc_To_where_calcBool();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* inspect <where>//<pred> and attempt to find source <table>                 */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH40_T20_Mark_where_pred_Wrt_table(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("inspect <where>//<pred> and attempt to find source <table>"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (E_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    vR_H40_T20_Mark_where_pred_Wrt_table();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* inspect all <where>//<pred> and move <predCmp> to new <table>/<filter>     */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH40_T30_Move_Marked_where_pred_To_table_filter(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("inspect all <where>//<pred> and move <predCmp> to new <table>/<filter>"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (E_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    vR_H40_T30_Move_Marked_where_pred_To_table_filter();
  }
  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* delete <where> if it is effectively empty                                  */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH40_T40_Delete_Empty_where(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("delete <where> if it is effectively empty"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (E_bEachMainWithSub(CK_WHERE,CK_BOOLCALC)) {
    vR_H40_T40_Delete_Empty_where();
   }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                    Flatten Exists Hand 50 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH50_T10_Rework_Exists_SubQueries(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (E_bEachMainWithSub(CK_WHERE,CK_TSTPRED,"exists"_Stz)) {

    Stz* szSubQueryNm = M.p_tstPred->pGetChild(1)->szValue();
    vTracer("SubQueryNm: %s\n"_Stz,szSubQueryNm);

    Board* pSubQueryBoard = m_pBench->pGetBoard(szSubQueryNm);
    if (! pSubQueryBoard) {
      vTracer("DID NOT FIND Board: %s\n"_Stz,szSubQueryNm);
      continue;
    }

    vTracer("Found SubQuery Board: %s\n"_Stz,pSubQueryBoard->szBoardNm());

    M_vPushIter(pSubQueryBoard);

    vPrintBoard();

    if (E_bEachMain(CK_STAGE)) {
      vTracer("SubQuery Stage: %s\n"_Stz, M.p_stage->szBearing());

      vR_H50_T10_Rework_Exists_SubQueries();
    }
    vPrintBoard();

    M_vPopIter();
  }

  vEndTransform();
  vPrintBoard();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH50_T20_Copy_Up_SubQueries(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (E_bEachMainWithSub(CK_WHERE,CK_TSTPRED,"exists"_Stz)) {

    Stz* szSubQueryNm = M.p_tstPred->pGetChild(1)->szValue();
    vTracer("SubQueryNm: %s\n"_Stz,szSubQueryNm);

    Board* pSubQueryBoard = m_pBench->pGetBoard(szSubQueryNm);
    if (! pSubQueryBoard) {
      vTracer("DID NOT FIND Board: %s\n"_Stz,szSubQueryNm);
      continue;
    }

    vTracer("Found SubQuery Board: %s\n"_Stz,pSubQueryBoard->szBoardNm());

    M_vPushIter(pSubQueryBoard);

    vPrintBoard();

    if (E_bEachMain(CK_KEYEXISTS)) {

      vR_H50_T20_Copy_Up_SubQueries();

    }
    else {
      M_vPopIter();
    }
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH50_T30_Resolve_Parameter_Linkage(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (E_bEachMain(CK_KEYEXISTS)) {

    vTracer("Resolving %s"_Stz,M.p_MainCard->szBearing());
    if (!F_bPlumbForSubMember(CK_KEYEXISTS, CK_PARAM))
      continue;

    if (! R_bResolveCard(CK_STMCOL,M.p_param->sz(),M.p_keyExists))
      continue;

    vR_H50_T30_Resolve_Parameter_Linkage();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH50_T40_Resolve_Parameter_Terms(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (E_bEachMainWith2Sub(CK_KEYEXISTS,CK_PARAMLIST,CK_RPNCALC)) {

    vTracer("Resolving Terms\n  %s\n  %s  \n"_Stz,
            M.p_keyExists->szBearing(),
            M.p_paramList->szBearing(),
            M.p_rpnCalc->szBearing());

    CardIter xCI(M.p_rpnCalc);
    while ((M.p_term = _pCard_Term(xCI.pEachChild()))) {
      if (!T_bTerm_IsParamOrdinal(M.p_term))
        continue;

      if (! Y_bFindPpy(CK_PARAMLIST,P_ParamOrd,CK_PARAM))
        continue;

      vR_H50_T40_Resolve_Parameter_Terms();
    }
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                      Initial Join Hand 60 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* add an <indexJoin> under all <table>'s which are joinable                  */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH60_T10_Add_indexJoin_UnderAllJoinable_table(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("add an <indexJoin> under all <table>'s which are joinable"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  M.pDeck->vClearMarksRecursive();

  while (E_bEachMainWithSub(CK_TABLE,CK_FILTER)) {
    vR_H60_T10_Add_indexJoin_UnderAllJoinable_table();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* convert the column name in every <idxCol> to a $streamOrdinal              */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH60_T20_Convert_idxCol_name_Of_table_To_idxCol_ord(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("convert the column name in every <idxCol> to a $streamOrdinal"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  M.pDeck->vClearMarksRecursive();

  while (E_bEachMain(CK_TABLE)) {
    if (! (M.p_indexJoin = M.p_table->pSub_indexJoin()))
      continue;

    vR_H60_T20_Convert_idxCol_name_Of_table_To_idxCol_ord();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* add a <joinPred> under each <indexJoin>                                    */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH60_T30_Add_joinPred_UnderAll_indexJoin(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("add a <joinPred> under each <indexJoin>"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (E_bEachMainWithSub(CK_TABLE,CK_FILTER)) {

    //M.p_table->vDump();

    M.p_boolCalc = M.p_filter->pSub_boolCalc();
    M.p_cmpPred  = M.p_boolCalc->pSub_predCmp();
    m_pTerm_0    = _pCard_Term(M.p_cmpPred->pGetChild(0));
    m_pTerm_1    = _pCard_Term(M.p_cmpPred->pGetChild(1));

    if (!T_bTerm_IsStreamColOrdinal(m_pTerm_0) ||
        !T_bTerm_IsStreamColOrdinal(m_pTerm_1))
      continue;

    vR_H60_T30_Add_joinPred_UnderAll_indexJoin();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* remove <table>/<filter>//<pred> that are now <table>/<joinPred>//<pred>    */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH60_T40_Cull_filter_pred_MovedUpTo_joinPred(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("remove <table>/<filter>//<pred> that are now <table>/<joinPred>//<pred>"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (E_bEachMain(CK_TABLE)) {
    if (! (M.p_filter = M.p_table->pSub_filter()))
      continue;

    M.p_boolCalc = M.p_filter->pSub_boolCalc();

    vR_H60_T40_Cull_filter_pred_MovedUpTo_joinPred();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* delete any <table>/<filter> which is effectively empty                     */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH60_T50_Delete_Empty_filter(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("delete any <table>/<filter> which is effectively empty"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (E_bEachMain()) {
    M.p_filter = M.p_MainCard->pSub_filter();
    if (! M.p_filter)
      continue;

    if (!X_bFilter_IsEmptyFilter(M.p_filter))
      continue;

    vR_H60_T50_Delete_Empty_filter();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                     Join Ordering Hand 70 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH70_T00_Extract_tableList_From_queryBoard(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  //vSetupPartitionBoard();

  //--nn--------------==
  // tableList

  BoardIter xBI_query(M.pBoard);

  M.p_tableList = new Card_TableList;
  M.pDeck->vAppendMain(M.p_tableList);

  while ( (M.p_MainCard = xBI_query.pEachMain()) ) {
    if (! (M.p_table = M.p_MainCard->pSub_table()))
      continue;

    vREPLACE_WITH_RESHAPE();

    M.p_tableList->vAppend(M.p_table->pDupCardAndSubCards());
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH70_T10_Extract_joinPredList(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  //vSetupPartitionBoard();

  //--nn--------------==
  // joinPredList

  M.p_joinPredList = new Card_JoinPredList;
  M.pDeck->vAppendMain(M.p_joinPredList);

  BoardIter xBI_query(M.pBoard);
  while ((M.p_MainCard = xBI_query.pEachMain(CK_FILTER))) {
    M.p_boolCalc = M.p_MainCard->pSub_boolCalc();
    if (!M.p_boolCalc)
      continue;

    vREPLACE_WITH_RESHAPE();

    X_vJoinPredList_Extract(M.p_joinPredList, M.p_boolCalc);
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH70_T20_Compute_coPtnPredList(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  //vSetupPartitionBoard();

  //--nn--------------==
  // coPptnPredList

  M.p_coPtnPredList = new Card_CoPtnPredList;
  M.pDeck->vAppendMain(M.p_coPtnPredList);

  M.p_tableList =
      _pCard_TableList(M.pDeck->pFindChild(CK_TABLELIST));
  M.p_joinPredList =
      _pCard_JoinPredList(M.pDeck->pFindChild(CK_JOINPREDLIST));

  i8 nJoinPreds = M.p_joinPredList->nChildCards();
  for (i8 i_joinPred = 0; i_joinPred < nJoinPreds; i_joinPred++) {
    M.p_cmpPred =
        _pCard_CmpPred(M.p_joinPredList->pGetChild(i_joinPred));

    if (!X_bTableList_PredCmpIsCoPtn(M.p_tableList, M.p_cmpPred))
      continue;

    vREPLACE_WITH_RESHAPE();

    M.p_coPtnPred = new Card_CoPtnPred;
    M.p_coPtnPred->vAppend(M.p_cmpPred->pDupCardAndSubCards());

    M.p_coPtnPredList->vAppend(M.p_coPtnPred);
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH70_T30_Compute_coPtnJoinList(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  //vSetupPartitionBoard();

  //--nn--------------==
  // coPtnJoinList

  M.p_coPtnJoinList = new Card_CoPtnJoinList;
  M.pDeck->vAppendMain(M.p_coPtnJoinList);

  M.p_coPtnPredList =
      _pCard_CoPtnPredList(M.pDeck->pFindChild(CK_COPTNPREDLIST));

  vREPLACE_WITH_RESHAPE();

  i8 nChildren = M.p_coPtnPredList->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    M.p_coPtnPred =
        _pCard_CoPtnPred(M.p_coPtnPredList->pGetChild(i_Child));

    M.p_subSet = M.p_coPtnPred->pDupSubCards();

    if (! (M.p_coPtnJoin = X_pCoPtnJoinList_Find_coPtn_for(M.p_coPtnJoinList, M.p_coPtnPred)) ) {
      M.p_coPtnJoin = new Card_CoPtnJoin;
      M.p_coPtnJoinList->vAppend(M.p_coPtnJoin);
    }

    M.p_coPtnJoin->vTransferChildren(M.p_subSet);
    DELETE_CARD(M.p_subSet);
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH70_T40_Compute_crossPtnJoinList(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  //vSetupPartitionBoard();

  //--nn--------------==
  // crossPtnJoinList

  M.p_joinPredList =
      _pCard_JoinPredList(M.pDeck->pFindChild(CK_JOINPREDLIST));

  M.p_crossPtnJoinList = new Card_CrossPtnJoinList;
  M.pDeck->vAppendMain(M.p_crossPtnJoinList);

  i8 nChildren = M.p_joinPredList->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    M.p_cmpPred =
        _pCard_CmpPred(M.p_joinPredList->pGetChild(i_Child));

    vREPLACE_WITH_RESHAPE();

    if (X_bCoPtnJoinList_HasEqPredCmp(M.p_coPtnJoinList, M.p_cmpPred))
      continue;

    M.p_crossPtnJoin = new Card_CrossPtnJoin;
    M.p_crossPtnJoin->vAppend(M.p_cmpPred->pDupCardAndSubCards());
    M.p_crossPtnJoinList->vAppend(M.p_crossPtnJoin);
  }

//  M.p_comment = new Card_Comment("((TO BE CODED))"_Stz);
//  M.p_crossPtnJoinList->vAppend(M.p_comment);

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH70_T50_Compute_ptndJoinSet(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  //vSetupPartitionBoard();

  //--nn--------------==
  // ptndJoinSet

  vREPLACE_WITH_RESHAPE();


  M.p_ptndJoinSet = new Card_PtndJoinSet;
  M.pDeck->vAppendMain(M.p_ptndJoinSet);

  M.p_coPtnJoinList =
      _pCard_CoPtnJoinList(M.pDeck->pFindChild(CK_COPTNJOINLIST));
  M.p_subSet = M.p_coPtnJoinList->pDupSubCards();
  M.p_ptndJoinSet->vTransferChildren(M.p_subSet);
  DELETE_CARD(M.p_subSet);

  M.p_crossPtnJoinList =
      _pCard_CrossPtnJoinList(M.pDeck->pFindChild(CK_CROSSPTNJOINLIST));
  M.p_subSet = M.p_crossPtnJoinList->pDupSubCards();
  M.p_ptndJoinSet->vTransferChildren(M.p_subSet);
  DELETE_CARD(M.p_subSet);

  i8 nChildren = M.p_ptndJoinSet->nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    m_pCard_child = M.p_ptndJoinSet->pGetChild(i_Child);
    m_pCard_child->vCard(CK_PTNDJOIN);
    m_pCard_child->Y_vSet(P_PtndJoin, i_Child);
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH70_T60_Copy_ptndJoinSet(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  //vSetupPermuteBoard();

  vREPLACE_WITH_RESHAPE();

  m_pBench->pCardCtl()->vNextHorizon(CK_PERMUTATION, true);

  M.p_ptndJoinSet =
      _pCard_PtndJoinSet(M.pDeck->pFindChild(CK_PTNDJOINSET));

  M.pDeck->vAppendMain(new Card_Comment("PERMUTATIONS"_Stz));

  i8 i_PtdnJoin = 0;
  M.p_subSet = M.p_ptndJoinSet->pDupSubCards();
  while ((m_pCard_popped = M.p_subSet->pPop())) {
    M.pDeck->vAppendMain(m_pCard_popped);
  }
  DELETE_CARD(M.p_subSet);

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH70_T70_Generate_ptndPermutations(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

//  PermGen::vTest(4);

  //--nn--------------==
  // x

  vREPLACE_WITH_RESHAPE();

  m_pPermGen = new PermGen(M.p_ptndJoinSet->nChildCards());

  M.p_subSet = new Card_SubSet;

  while (m_pPermGen->bNextPermutation()) {
    M.pDeck->vAppendMain((M.p_permutation = new Card_Permutation));
    M.p_permutation->Y_vSet(P_Permutation, m_pPermGen->nPermutations() - 1);

    RASSERT(0 == M.p_subSet->nChildCards());
    for (i8 i_Item = 0; i_Item < m_pPermGen->nItems(); i_Item++) {
      i8 i_Child = m_pPermGen->iItem(i_Item);
      m_pCard_child = M.p_ptndJoinSet->pGetChild(i_Child);
      M.pDeck->vAppendMain(m_pCard_child->pDupCardAndSubCards());

      M.p_ptndJoin = _pCard_PtndJoin(m_pCard_child);
      if (!X_bPtndJoin_MayFollowAfter(M.p_ptndJoin, M.p_subSet)) {
        M.pDeck->vAppendMain(new Card_Comment("**DOESN'T FOLLOW**"_Stz));
        M.p_permutation->Y_vSet(P_Invalid);
        break;
      }
      M.p_subSet->vAppend(m_pCard_child);
    }
    M.p_subSet->vRemoveAll();
  }

  DELETE_CARD(M.p_subSet);

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH70_T80_Remove_invalidPermutations(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  i8 i_Main = 0;
  i8 i_Permutation = 0;
  while (i_Main < M.pDeck->nChildCards()) {
    M.p_MainCard = M.pDeck->pGetChild(i_Main);

    if (CK_PERMUTATION != M.p_MainCard->eCard()) {
      i_Main++;
      continue;
    }

    if (! M.p_MainCard->bHasProperty(P_Invalid)) {
      M.p_MainCard->Y_vSet(P_Permutation, i_Permutation++);
      i_Main++;
      continue;
    }

    vREPLACE_WITH_RESHAPE();

    M.pDeck->vRemoveChild(M.p_MainCard);
    while (i_Main < M.pDeck->nChildCards()) {
      M.p_MainCard = M.pDeck->pGetChild(i_Main);
      if (CK_PTNDJOIN != M.p_MainCard->eCard() && CK_COMMENT != M.p_MainCard->eCard())
        break;
      M.pDeck->vRemoveChild(M.p_MainCard);
      delete M.p_MainCard;
    }
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH70_T90_Setup_costEst(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  //vSetupCostEstBoard();

  //m_pBoard_costEst->vDuplicateBoard(M.pBoard);

  while ((M.p_MainCard = M.pDeck->pGetChild(0))) {

    if (CK_PERMUTATION == M.p_MainCard->eCard())
      break;

    vREPLACE_WITH_RESHAPE();

    M.pDeck->vRemoveChild(M.p_MainCard);
    delete M.p_MainCard;
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH70_T92_Estimate_ptndJoins(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  M.p_tableList =
      _pCard_TableList(M.pDeck->pFindChild(CK_TABLELIST));


  BoardIter xBI(M.pBoard);

//  while (M.p_ptndJoin = _pCard_PtndJoin(xBI.pEachMain(CK_PTNDJOIN))) {
  while ((M.p_MainCard = xBI.pEachMain())) {
    eCK eCard = M.p_MainCard->eCard();
    if (CK_PERMUTATION == eCard) {
      M.p_tableList->vDeletePropertyRecursive(P_TableUsed);
      continue;
    }
    if (CK_PTNDJOIN != eCard)
      continue;

    vREPLACE_WITH_RESHAPE();

    M.p_ptndJoin = _pCard_PtndJoin(M.p_MainCard);
    X_vPtndJoin_JoinEst(M.p_ptndJoin, M.p_tableList);
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH70_T94_Estimate_network(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  BoardIter xBI(M.pBoard);

  i8 nNetwork = 0;
  i8 nSentOut = 0;
  M.p_permutation = nullptr;
  M.p_ptndJoin    = nullptr;

  while ((M.p_MainCard = xBI.pEachMain())) {

    vREPLACE_WITH_RESHAPE();

    eCK eCard = M.p_MainCard->eCard();
    if (CK_PERMUTATION == eCard) {
      if (M.p_permutation)
        M.p_permutation->Y_vSet(P_Network, nNetwork);
      M.p_permutation = _pCard_Permutation(M.p_MainCard);

      M.p_ptndJoin = nullptr;
      nNetwork = 0;
      nSentOut = 0;
    }
    else
    if (CK_PTNDJOIN == eCard) {
      if (M.p_ptndJoin) {
        nSentOut += M.p_ptndJoin->Y_iVal(P_JoinedBuff);
        nNetwork += nSentOut;
        M.p_ptndJoin->Y_vSet(P_SentOut, nSentOut);
      }
      M.p_ptndJoin = _pCard_PtndJoin(M.p_MainCard);
    }
  }

  if (M.p_permutation)
    M.p_permutation->Y_vSet(P_Network, nNetwork);

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH70_T96_Estimate_joinOrder(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  i8 iLowestNetwork = 999999999;

  BoardIter xBI(M.pBoard);

  while ((M.p_permutation =
              _pCard_Permutation(xBI.pEachMain(CK_PERMUTATION)))) {
    i8 iNetwork = M.p_permutation->Y_iVal(P_Network);
    if (iNetwork >= iLowestNetwork)
      continue;
    M.p_permutation_low = M.p_permutation;
    iLowestNetwork = iNetwork;
  }

  M.pDeck->vAppendMain((M.p_joinOrderSeq = new Card_JoinOrderSeq));
  M.p_joinOrderSeq->Y_vSet(P_Network, iLowestNetwork);

  xBI.vSet(M.p_permutation_low);
  while ((M.p_MainCard = xBI.pEachMain())) {
    if (CK_PTNDJOIN != M.p_MainCard->eCard())
      break;

    M.p_ptndJoin = _pCard_PtndJoin(M.p_MainCard);
    X_vJoinOrderSeq_ExtractJoinOrder(M.p_joinOrderSeq, M.p_ptndJoin);
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH70_T98_Extract_joinOrderSeq(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  M.pDeck->vAppendMain((M.p_joinOrderSeq = new Card_JoinOrderSeq));

  BoardIter xBI(M.pBoard);

  xBI.vSet(M.p_permutation_low);
  while ((M.p_MainCard = xBI.pEachMain())) {
    if (CK_PTNDJOIN != M.p_MainCard->eCard())
      break;

    vREPLACE_WITH_RESHAPE();

    M.p_ptndJoin = _pCard_PtndJoin(M.p_MainCard);
    X_vJoinOrderSeq_ExtractJoinOrder(M.p_joinOrderSeq, M.p_ptndJoin);
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* convert all <table> into either <scan> or <indexJoin>                      */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH70_T99_Convert_table_To_scan_Or_indexJoin(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("convert all <table> into either <scan> or <indexJoin>"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  Card_Deck* p_board = M.pBoard->pDeck();

  while (E_bEachMain(CK_TABLE)) {

    M.p_bounds = M.p_table->pSub_bounds();
    M.p_indexJoin = M.p_table->pSub_indexJoin();
    M.p_index = M.p_table->pSub_index();

    vREPLACE_WITH_RESHAPE();

    if (! M.p_bounds) {

      M.p_scan = new Card_Scan;
      M.p_scan->vTransferChildren(M.p_table);

      p_board->vReplace(M.p_table,M.p_scan);
      M.p_scan->vInsert(M.p_table);

      if (M.p_indexJoin) {
        M.p_scan->vRemoveChild(M.p_indexJoin);
        DELETE_CARD(M.p_indexJoin);
      }
      continue;
    }

    M.p_table->vRemoveSubCard(M.p_indexJoin);
    M.p_indexJoin->vTransferChildren(M.p_table);

    p_board->vReplace(M.p_table,M.p_indexJoin);
    M.p_indexJoin->vInsert(M.p_table);

    M.p_indexJoin->vRemoveSubCard(M.p_index);
    M.p_indexJoin->vAppend(M.p_index);


  }

  M_vRestart();
  while (E_bEachMain(CK_SCAN)) {

    i8 i_Child;
    M.p_comment = _pCard_Comment(M.p_scan->pFindChild(CK_COMMENT,i_Child));

    if (i_Child == M.p_scan->nChildCards() - 1) {
      M.p_scan->vRemoveChild(M.p_comment);
      DELETE_CARD(M.p_comment);
    }

  }

  M_vRestart();
  while (E_bEachMain(CK_INDEXJOIN)) {

    i8 i_Child;
    M.p_comment = _pCard_Comment(M.p_indexJoin->pFindChild(CK_COMMENT,i_Child));
    M.p_comment->vValue(SEP_DERIVED);
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                        Index Scan Hand 80 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* append <indexList> to <scan>                                               */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH80_T10_Insert_indexList_ToAll_scan(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("append <indexList> to <scan>"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (E_bEachMainWith2Sub(CK_SCAN,CK_TABLE,CK_TABLENM)) {

//    if ( (M.p_indexList = M.p_tablePtr->pNew_indexList()) ) {
//      M.p_scan->vInsertAfter(M.p_indexList,M.p_tablePtr);
//
//    }

    vREPLACE_WITH_RESHAPE();

    m_pTable = A_pAccess_Table(M.p_tableNm->sz());

    if ((M.p_indexList = A_pProduce_indexList_ofTable(m_pTable))) {
      M.p_comment = new Card_Comment(SEP_SCRATCH);
      M.p_scan->vAppend(M.p_comment);

      M.p_scan->vAppend(M.p_indexList);
    }
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* convert the column name in every <idxCol> to a $streamOrdinal              */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH80_T20_Convert_idxCol_name_Of_scan_To_idxCol_ord(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("convert the column name in every <idxCol> to a $streamOrdinal"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  M.pDeck->vClearMarksRecursive();

  while (E_bEachMainWithSub(CK_SCAN,CK_IDXCOLLIST)) {
    M.p_stream = M.p_scan->pSub_stream();

    vREPLACE_WITH_RESHAPE();

    X_vIdxColList_Ordinate(M.p_idxColList, M.p_stream);
  }

  vEndTransform();
  vPrintBoard();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH80_T30_Mark_scan_pred_bounds(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bfMarkPredBounds = [this](Card* pCard) {
    Card_CmpPred* p_cmpPred = _pCard_CmpPred(pCard);
    T_vCmpPred_MarkBound(p_cmpPred);
    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  while (E_bEachMainWithSub(CK_SCAN,CK_FILTER)) {
    vREPLACE_WITH_RESHAPE();

    M.p_filter->vApplyRecursive(CK_CMPPRED, bfMarkPredBounds);
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* mark the <SCAN>//<filter>//cmpPred> which match index ordinals             */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH80_T40_Mark_scan_filter_term_WhichMatchIndex(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("mark <SCAN>//<filter>//<term> ordinals which match index"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (E_bEachMainWithSub(CK_SCAN,CK_FILTER)) {

    if (! (M.p_indexList = M.p_MainCard->pSub_indexList()))
      continue;
    if (! (M.p_index = _pCard_Index(M.p_indexList->pGetChild(0))))
      continue;
    if (! (M.p_idxColList = (M.p_index->pSub_idxColList())))
      continue;

    M.p_idxColList->vDump();

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    auto bfPredBounds = [this](Card* pCard) {
      Card_Term* p_term = _pCard_Term(pCard);

      Stz* szTerm = p_term->sz();
      if (!szTerm->bIsStreamColOrd())
        return C_CONTINUE_RECURSION;

      vTracer("Checking %s %s\n"_Stz,p_term->sz(),p_term->szBearing());

      i8 i_IdxCol = C_UNKNOWN;

      if (! (M.p_idxCol =
                 _pCard_IdxCol(M.p_idxColList->pFindChild(CK_IDXCOL, szTerm, i_IdxCol))))
        return C_CONTINUE_RECURSION;

      vTracer("Matches %s.%lld // marked with I%lld\n"_Stz,
              M.p_idxCol->pSub_idxColNm()->szValue(),i_IdxCol,i_IdxCol);

      p_term->vMark1('I');
      p_term->vMark2(i_IdxCol);
      return C_CONTINUE_RECURSION;
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    vREPLACE_WITH_RESHAPE();

    M.p_filter->vApplyRecursive(CK_TERM, bfPredBounds);
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH80_T50_Add_scanPred(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  M.p_bounds = nullptr;

  while (E_bEachMainWith2Sub(CK_SCAN,CK_FILTER,CK_INDEXLIST)) {

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    auto bfAdd = [this](Card* pCard) {
      M.p_cmpPred = _pCard_CmpPred(pCard);
      if (! ('L' == M.p_cmpPred->cMark1() || 'U' == M.p_cmpPred->cMark1()) )
        return true;

      vTracer("Testing %s\n"_Stz,M.p_cmpPred->szBearing());

      if (! M.p_bounds) {
        N_vNewMember(CK_BOUNDS);
        //C_vAppendChild(CK_SCAN,CK_BOUNDS);
        M.p_scan->vInsertBefore(M.p_bounds,M.p_filter);
      }

      if ('L' == M.p_cmpPred->cMark1()) {

        M.p_lBound = M.p_bounds->pSub_lBound();
        if (! M.p_lBound) {
          N_vNewMember(CK_LBOUND);
          C_vAppendChild(CK_BOUNDS,CK_LBOUND);
        }
        //m_pCard_dup = M.p_cmpPred->pDupCardAndSubCards();
        C_vDuplicateMember(CK_CMPPRED);
        StrLib::vInsertChar(m_pCard_dup->pGetChild(0)->sz()->azChar()+1,'I');
        //M.p_lBound->vAppend(m_pCard_dup);
        C_vAppendChild(CK_LBOUND,m_pCard_dup);
        vTracer("Added cmpPred as lower-bound %s %s\n"_Stz,m_pCard_dup->sz(),m_pCard_dup->szBearing());
      }
      else
      if ('U' == M.p_cmpPred->cMark1()) {
        M.p_uBound = M.p_bounds->pSub_uBound();
        if (! M.p_uBound) {
          N_vNewMember(CK_UBOUND);
          C_vAppendChild(CK_BOUNDS,CK_UBOUND);
        }
        //m_pCard_dup = M.p_cmpPred->pDupCardAndSubCards();
        C_vDuplicateMember(CK_CMPPRED);
        StrLib::vInsertChar(m_pCard_dup->pGetChild(0)->sz()->azChar()+1,'I');
        //M.p_uBound->vAppend(m_pCard_dup);
        C_vAppendChild(CK_UBOUND,m_pCard_dup);
        vTracer("Added cmpPred as upper-bound %s %s\n"_Stz,m_pCard_dup->sz(),m_pCard_dup->szBearing());
      }

      return C_CONTINUE_RECURSION;
    };

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    vREPLACE_WITH_RESHAPE();

    M.p_filter->vApplyRecursive(CK_CMPPRED, bfAdd);

  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH80_T60_Cull_Marked_cmpPred(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (E_bEachMainWithSub(CK_SCAN,CK_FILTER)) {

    vREPLACE_WITH_RESHAPE();

    vTracer("Culling <filter> of L & U bounds %s\n"_Stz,M.p_filter->szBearing());
    X_vFilter_CullPred(M.p_filter, 'L');
    X_vFilter_CullPred(M.p_filter, 'U');

    //M.p_filter->vDump();
    if (!X_bFilter_IsEmptyFilter(M.p_filter))
      continue;

    vTracer("Deleting empty <filter> %s\n"_Stz,M.p_filter->szBearing());
    M.p_MainCard->vRemoveSubCard(M.p_filter);
    DELETE_CARD(M.p_filter);
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH80_T70_Promote_scan_To_indexScan(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (E_bEachMainWithSub(CK_SCAN,CK_BOUNDS)) {
    vTracer("Morphing <scan> to <indexScan> %s\n"_Stz,M.p_scan->szBearing());
    vREPLACE_WITH_RESHAPE();

    M.p_scan->vCard(CK_INDEXSCAN);
  }

  M_vRestart();
  while (E_bEachMain(CK_INDEXSCAN)) {

    i8 i_Child;
    M.p_comment = _pCard_Comment(M.p_indexScan->pFindChild(CK_COMMENT,i_Child));
    M.p_comment->vValue(SEP_DERIVED);
  }

  M_vRestart();
  while (E_bEachMainWithSub(CK_INDEXSCAN,CK_INDEXLIST)) {

    M.p_indexScan->vReplaceChildWithItsSubs(M.p_indexList);

    DELETE_CARD((M.p_indexList));
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                   Stream Ordering Hand 90 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* remove <sort> if <sbyElist> is the same as the <gbyElist>                  */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH90_T10_Remove_sort_IfSameAs_gby(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("remove <sort> if <sbyElist> is the same as the <gbyElist>"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (E_bEachMain(CK_SORT)) {

    if (!H_bHasPriorToMainIf(CK_AGGREG, C_UPDATE_MEMBER))
      return;

    M.p_gbyElist = M.p_aggreg->pSub_gbyElist();
    M.p_sbyElist = M.p_sort->pSub_sbyElist();

    // TODO: needs to be a T_ test
    if (M.p_gbyElist->bValuesSameRecursive(M.p_sbyElist)) {
      vR_H90_T10_Remove_sort_IfSameAs_gby();
      break;
    }
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                 Filter Refinement Hand A0 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* individuate <scan|indexJoin>/<filter> as main <filter>                     */
/*----------------------------------------------------------------------------*/
void Xtransforms::vHA0_T10_Individuate_filter(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("individuate <scan|indexJoin>/<filter> as main <filter>"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (E_bEachMain(CK_SCAN)) {

    M.p_filter =  M.p_scan->pSub_filter();
    if (! M.p_filter)
      continue;

    vR_HA0_T10_Individuate_filter();
  }

  M_vRestart();

  while (E_bEachMain(CK_INDEXJOIN)) {

    M.p_filter = M.p_indexJoin->pSub_filter();
    if (! M.p_filter)
      continue;

    vR_HA0_T10_Individuate_filter_b();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                  Restate Pipeline Hand Z0 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* reinstate stages                                                           */
/*----------------------------------------------------------------------------*/
void Xtransforms::vHZ0_T10_Reinstate_stages(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("reinstate stages"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (E_bEachMain() && H_bHasSubsequentToMain(C_UPDATE_MEMBER))  {

    if (CK_STAGE == M.e_MainCard || CK_STAGE == M.e_SubsequentToMain)
      continue;

    if (CK_FILTER == M.e_MainCard && CK_INDEXJOIN == M.e_SubsequentToMain)
      continue;

    if (CK_INDEXJOIN == M.e_MainCard &&
        (CK_PROJECT == M.e_SubsequentToMain || CK_FILTER == M.e_SubsequentToMain))
      continue;

    vR_HZ0_T10_Reinstate_stages();
  }

  i8 i_Stage = 1;
  M_vRestart();
  while (E_bEachMain(CK_STAGE))
    M.p_stage->vValueSprint("S%02lld"_Stz,i_Stage++);

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vHZ0_T20_Reinstate_combined(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (E_bEachMain()) {

    E_vSubsequentToMain();
    if (CK_INDEXJOIN == M.e_MainCard && (CK_AGGREG == M.e_SubsequentToMain ||
                                         CK_FILTER == M.e_SubsequentToMain ||
                                         CK_PROJECT == M.e_SubsequentToMain))
      continue;

    if (! (CK_AGGREG    == M.e_MainCard ||
           CK_FILTER    == M.e_MainCard ||
           CK_INDEXSCAN == M.e_MainCard ||
           CK_INDEXJOIN == M.e_MainCard ||
           CK_PROJECT   == M.e_MainCard ||
           CK_SORT      == M.e_MainCard))
      continue;

    vR_HZ0_T20_Reinstate_combined();

    E_vSkipNextMain();
  }

  vEndTransform();
  vPrintBoard();
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vREPLACE_WITH_RESHAPE(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
