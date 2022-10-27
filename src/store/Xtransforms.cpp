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
/* linearize board by deleting all redundant <stage>'s                        */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH10_T10_Delete_Redundant_stage(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("linearize board by deleting all duplicate <stage>'s"_Stz);
  vTransform((char *) (__func__), C_PRINT_XFORM);
  vTracing(bPrint,bTrace);

  //M.pDeck->vDump(0,(Stz*)(__func__));

  Card_Stage* p_stage_PRIOR = nullptr;

  while (G_bEachMain(CK_STAGE)) {
    if (! T_bStage_IsRedundant(M.p_stage,p_stage_PRIOR))   {
      p_stage_PRIOR = M.p_stage;
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

  while (G_bEachMain(CK_STAGE)) {
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

  while (G_bEachMain(CK_ABSJOIN)) {
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

  while (G_bEachMain(CK_COMBINED)) {
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

  while (G_bEachMain(CK_SEND)) {
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

  while (G_bEachMain(CK_SCAN)) {
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

  while (G_bEachMainWithSub(CK_TABLE, CK_TABLENM))  {
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

  while (G_bEachMainWithSub(CK_TABLE, CK_TABLENM)) {
    vR_H20_T20_Add_tablePtn_UnderAll_table();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                   Stream Analysis Hand 30 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* add a <stream> under every <table>                                         */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH30_T10_Add_stream_UnderAll_table(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("add a <stream> under every <table>"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  // E <scan>
  while (G_bEachMainWithSub(CK_TABLE, CK_TABLENM)) {
    vR_H30_T10_Add_stream_UnderAll_table();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* calculate the ordinal $low..$high range for each <stream>                  */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH30_T20_Calc_ordinalRange_ForAll_stream(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("calculate the ordinal $low..$high range for each <stream>"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (G_bEachMainWithSub(CK_TABLE,CK_STREAM)) {
    vR_H30_T20_Calc_ordinalRange_ForAll_stream();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                  Predicate Pullup Hand 40 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* convert <where>//<calc> to <where>//<calcBool>                             */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH40_T10_Convert_where_rpnCalc_To_calcBool(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("convert <where>//<calc> into <where>//<calcBool> "_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (G_bEachMainWithSub(CK_WHERE, CK_RPNCALC)) {
    vR_H40_T10_Convert_where_rpnCalc_To_calcBool();
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

  while (G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
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

  while (G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
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

  while (G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
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

  while (G_bEachMainWithSub(CK_WHERE, CK_TSTPRED, "exists"_Stz)) {

    Stz* szSubQueryNm = M.p_tstPred->pGetChild(1)->szValue();
    vTracer("SubQueryNm: %s\n"_Stz,szSubQueryNm);

    Board* pSubQueryBoard = m_pBench->pGetBoard(szSubQueryNm);
    if (! pSubQueryBoard) {
      vTracer("DID NOT FIND Board: %s\n"_Stz,szSubQueryNm);
      continue;
    }

    vTracer("Found SubQuery Board: %s\n"_Stz,pSubQueryBoard->szBoardNm());

    G_vPushIter(pSubQueryBoard);

    vPrintBoard();

    if (G_bEachMain(CK_STAGE)) {
      vTracer("SubQuery Stage: %s\n"_Stz, M.p_stage->szBearing());

      vR_H50_T10_Rework_Exists_SubQueries();
    }
    vPrintBoard();

    G_vPopIter();
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

  while (G_bEachMainWithSub(CK_WHERE, CK_TSTPRED, "exists"_Stz)) {

    Stz* szSubQueryNm = M.p_tstPred->pGetChild(1)->szValue();
    vTracer("SubQueryNm: %s\n"_Stz,szSubQueryNm);

    Board* pSubQueryBoard = m_pBench->pGetBoard(szSubQueryNm);
    if (! pSubQueryBoard) {
      vTracer("DID NOT FIND Board: %s\n"_Stz,szSubQueryNm);
      continue;
    }

    vTracer("Found SubQuery Board: %s\n"_Stz,pSubQueryBoard->szBoardNm());

    G_vPushIter(pSubQueryBoard);

    vPrintBoard();

    if (G_bEachMain(CK_KEYEXISTS)) {

      vR_H50_T20_Copy_Up_SubQueries();

    }
    else {
      G_vPopIter();
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

  while (G_bEachMain(CK_KEYEXISTS)) {

    vTracer("Resolving %s"_Stz,M.p_MainCard->szBearing());
    if (!G_bPlumbForSubMember(CK_KEYEXISTS, CK_PARAM))
      continue;

    if (!G_bResolveCard(CK_STMCOL, M.p_param->sz(), M.p_keyExists))
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

  while (G_bEachMainWith2Sub(CK_KEYEXISTS, CK_PARAMLIST, CK_RPNCALC)) {

    vTracer("Resolving Terms\n  %s\n  %s  \n"_Stz,
            M.p_keyExists->szBearing(),
            M.p_paramList->szBearing(),
            M.p_rpnCalc->szBearing());

    CardIter xCI(M.p_rpnCalc);
    while ((M.p_term = _pCard_Term(xCI.pEachChild()))) {
      if (!T_bTerm_IsParamOrdinal(M.p_term))
        continue;

      if (!G_bFindPpy(CK_PARAMLIST, P_ParamOrd, CK_PARAM))
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
void Xtransforms::vH60_T10_Add_indexJoin_UnderAllFiltered_table(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("add an <indexJoin> under all <table>'s which are joinable"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  //M.pDeck->vClearMarksRecursive();

  while (G_bEachMainWithSub(CK_TABLE, CK_FILTER)) {
    vR_H60_T10_Add_indexJoin_UnderFiltered_table();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* convert the column name in every <idxCol> to a $streamOrdinal              */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH60_T20_Convert_idxCol_name_OfAll_table_To_ordinal(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("convert the column name in every <idxCol> to a $streamOrdinal"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  //M.pDeck->vClearMarksRecursive();

  while (G_bEachMainWithSub(CK_TABLE,CK_INDEXJOIN)) {
    vR_H60_T20_Convert_idxCol_name_Of_table_To_ordinal();
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

  while (G_bEachMainWithSub(CK_TABLE, CK_FILTER)) {
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

  while (G_bEachMainWithSub(CK_TABLE,CK_FILTER)) {
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

  while (G_bEachMainWithSub(CK_TABLE,CK_FILTER)) {
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

    if (!T_bTableList_PredCmpIsCoPtn(M.p_tableList, M.p_cmpPred))
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

  while (G_bEachMain(CK_TABLE)) {

    M.p_bounds = M.p_table->pSub_bounds();
    M.p_indexJoin = M.p_table->pSub_indexJoin();
    M.p_index = M.p_table->pSub_index();

    vR_H70_T99_Convert_table_To_scan_Or_indexJoin();
  }

  G_vRestart();
  while (G_bEachMain(CK_SCAN)) {
    vR_H70_T99_Convert_table_To_scan_Or_indexJoin_b();
  }

  G_vRestart();
  while (G_bEachMain(CK_INDEXJOIN)) {
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

  while (G_bEachMainWith2Sub(CK_SCAN, CK_TABLE, CK_TABLENM)) {
    vR_H80_T10_Insert_indexList_ToAll_scan();
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

  while (G_bEachMainWithSub(CK_SCAN, CK_IDXCOLLIST)) {
    vR_H80_T20_Convert_idxCol_name_Of_scan_To_idxCol_ord();
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

  while (G_bEachMainWithSub(CK_SCAN, CK_FILTER)) {
    vR_H80_T30_Mark_scan_pred_bounds();
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

  while (G_bEachMainWith2Sub(CK_SCAN, CK_FILTER, CK_INDEXLIST)) {
    vR_H80_T40_Mark_scan_filter_term_WhichMatchIndex();
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

  while (G_bEachMainWith2Sub(CK_SCAN, CK_FILTER, CK_INDEXLIST)) {
    vR_H80_T50_Add_scanPred();
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

  while (G_bEachMainWithSub(CK_SCAN, CK_FILTER)) {
    vR_H80_T60_Cull_Marked_cmpPred();
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

  while (G_bEachMainWithSub(CK_SCAN, CK_BOUNDS)) {
    vTracer("Morphing <scan> to <indexScan> %s\n"_Stz,M.p_scan->szBearing());
    vR_H80_T70_Promote_scan_To_indexScan();
  }

  G_vRestart();
  while (G_bEachMain(CK_INDEXSCAN)) {
    vR_H80_T70_Promote_scan_To_indexScan_b();
  }

  G_vRestart();
  while (G_bEachMainWithSub(CK_INDEXSCAN, CK_INDEXLIST)) {
    vR_H80_T70_Promote_scan_To_indexScan_c();
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

  while (G_bEachMain(CK_SORT)) {

    if (!G_bHasPriorToMainIf(CK_AGGREG, C_UPDATE_MEMBER))
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

  while (G_bEachMainWithSub(CK_SCAN,CK_FILTER)) {
    vR_HA0_T10_Individuate_filter();
  }

  G_vRestart();
  while (G_bEachMainWithSub(CK_INDEXJOIN,CK_FILTER)) {
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

  while (G_bEachMain() && G_bHasSubsequentToMain(C_UPDATE_MEMBER))  {

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
  G_vRestart();
  while (G_bEachMain(CK_STAGE))
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

  while (G_bEachMain()) {

    G_vSubsequentToMain();
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

    G_vSkipNextMain();
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
