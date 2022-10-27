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

    vR_H70_T00_Extract_tableList_From_queryBoard();
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

    vR_H70_T10_Extract_joinPredList();
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

    vR_H70_T20_Compute_coPtnPredList();
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

  vR_H70_T30_Compute_coPtnJoinList();

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

  vR_H70_T40_Compute_crossPtnJoinList();


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

  vR_H70_T50_Compute_ptndJoinSet();

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

  vR_H70_T60_Copy_ptndJoinSet();

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

  vR_H70_T70_Generate_ptndPermutations();

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

  vR_H70_T80_Remove_invalidPermutations();

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

  vR_H70_T90_Setup_costEst();

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

  vR_H70_T92_Estimate_ptndJoins();

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

  vR_H70_T94_Estimate_network();

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

  vR_H70_T96_Estimate_joinOrder();

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

  vR_H70_T98_Extract_joinOrderSeq();

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

  while (E_bEachMain(CK_TABLE)) {

    M.p_bounds = M.p_table->pSub_bounds();
    M.p_indexJoin = M.p_table->pSub_indexJoin();
    M.p_index = M.p_table->pSub_index();

    vR_H70_T99_Convert_table_To_scan_Or_indexJoin();
  }

  M_vRestart();
  while (E_bEachMain(CK_SCAN)) {
    vR_H70_T99_Convert_table_To_scan_Or_indexJoin_b();
  }

  M_vRestart();
  while (E_bEachMain(CK_INDEXJOIN)) {
    vR_H70_T99_Convert_table_To_scan_Or_indexJoin_c();
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
