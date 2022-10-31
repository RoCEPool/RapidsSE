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
void Xtransforms::vH10_T10_Delete_Redundant_stages(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("linearize board by deleting all duplicate <stage>'s"_Stz);
  vTransform((char *) (__func__), C_PRINT_XFORM);
  vTracing(bPrint,bTrace);

  //G.pDeck->vDump(0,(Stz*)(__func__));

  Card_Stage* p_stage_PRIOR = nullptr;

  while (G_bEachMain(CK_STAGE)) {
    if (! T_bStage_IsRedundant(G.p_stage,p_stage_PRIOR))   {
      p_stage_PRIOR = G.p_stage;
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
void Xtransforms::vH10_T15_Hide_Ignorable_stages(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("hide ignorable <stage>'s"_Stz);
  vTransform((char *) (__func__), C_PRINT_XFORM);
  vTracing(bPrint,bTrace);

  while (G_bEachMain(CK_STAGE)) {
    vR_H10_T15_Hide_Ignorable_stage();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* delete superfluous <absJoin>'s                                             */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH10_T20_Delete_All_absJoin(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("delete superfluous <absJoin>'s"_Stz);
  vTransform((char *) (__func__), C_PRINT_XFORM);
  vTracing(bPrint,bTrace);

  while (G_bEachMain(CK_ABSJOIN)) {
    vR_H10_T20_Delete_absJoin();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* delete superfluous <combined>'s                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH10_T30_Delete_All_combined(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("delete superfluous <combined>'s"_Stz);
  vTransform((char *) (__func__), C_PRINT_XFORM);
  vTracing(bPrint,bTrace);

  while (G_bEachMain(CK_COMBINED)) {
    vR_H10_T30_Delete_combined();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* delete superfluous <send>'s                                                */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH10_T40_Delete_All_send(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("delete superfluous <send>'s"_Stz);
  vTransform((char *) (__func__), C_PRINT_XFORM);
  vTracing(bPrint,bTrace);

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
void Xtransforms::vH20_T10_Affix_tableSyn_ToAll_table(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("add a <tableSyn> to all <table>'s"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

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

  vStreamAnalysisReset();

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
void Xtransforms::vH40_T10_Convert_where_rpnCalc_To_boolCalc(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("convert <where>//<calc> into <where>//<calcBool> "_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (G_bEachMainWithSub(CK_WHERE, CK_RPNCALC)) {
    vR_H40_T10_Convert_where_rpnCalc_To_boolCalc();
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
/*                                                       Flatten Call Hand 50 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH50_T10_Extract_subSet_Of_call(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint, bTrace);

  while (G_bEachMain(CK_CALL)) {

    Stz* szSubQueryNm = G.p_call->szValue();

    if (! G_bPushBoard(szSubQueryNm)) {
      vTracer("DID NOT FIND Board: %s\n"_Stz,szSubQueryNm);
      continue;
    }

    vR_H50_T10_Extract_subSet_Of_call();
    vPrintBoard(true,__LINE__);
    G_vPopBoard();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH50_T20_Copy_Called_Subset(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint, bTrace);

  while (G_bEachMain(CK_CALL)) {

    Stz* szSubQueryNm = G.p_call->szValue();

    if (! G_bPushBoard(szSubQueryNm)) {
      vTracer("DID NOT FIND Board: %s\n"_Stz,szSubQueryNm);
      continue;
    }

    vR_H50_T20_Yank_Subset();

    G_vPopBoard();

    vR_H50_T20_Put_Yanked();
  }


  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH50_T30(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint, bTrace);

  while (G_bEachMain(CK_CALL)) {
    vR_H50_T30_RemoveDelete_call();
  }

  vSetPrintSubCards(CK_PROJELIST,true);

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH50_T40(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint, bTrace);

  vR_H50_T40();

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH50_T50(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint, bTrace);

  vR_H50_T50();

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH50_T60(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint, bTrace);

  vR_H50_T60();

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                    Flatten Exists Hand 55 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH55_T10_Rework_Exists_SubQueries(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (G_bEachMainWithSub(CK_WHERE, CK_TSTPRED, "exists"_Stz)) {

    Stz* szSubQueryNm = G.p_tstPred->pGetChild(1)->szValue();
    vTracer("SubQueryNm: %s\n"_Stz,szSubQueryNm);

    Board* pSubQueryBoard = m_pBench->pGetBoard(szSubQueryNm);
    if (! pSubQueryBoard) {
      vTracer("DID NOT FIND Board: %s\n"_Stz,szSubQueryNm);
      continue;
    }

    vTracer("Found SubQuery Board: %s\n"_Stz,pSubQueryBoard->szBoardNm());

    G_vPushIter(pSubQueryBoard);

    //vPrintBoard();

    if (G_bEachMain(CK_STAGE)) {
      vTracer("SubQuery Stage: %s\n"_Stz, G.p_stage->szBearing());

      vR_H55_T10_Rework_Exists_SubQueries();
    }
    vPrintBoard(true,__LINE__);

    G_vPopIter();
  }

  vEndTransform();
  vPrintBoard();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH55_T20_Copy_Up_SubQueries(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (G_bEachMainWithSub(CK_WHERE, CK_TSTPRED, "exists"_Stz)) {

    G.p_tstPred->vMark1(MARK_CALLSUB);

    G.sz_SubQueryNm = G.p_tstPred->pGetChild(1)->szValue();
    vTracer("SubQueryNm: %s\n"_Stz,G.sz_SubQueryNm);

    Board* pSubQueryBoard = m_pBench->pGetBoard(G.sz_SubQueryNm);
    if (! pSubQueryBoard) {
      vTracer("DID NOT FIND Board: %s\n"_Stz,G.sz_SubQueryNm);
      continue;
    }

    vTracer("Found SubQuery Board: %s\n"_Stz,pSubQueryBoard->szBoardNm());

    G_vPushIter(pSubQueryBoard);

    //vPrintBoard(false,__LINE__);

    if (G_bEachMainWithSub(CK_TABLE,CK_KEYEXISTS)) {

      vR_H55_T20_Copy_Up_SubQueries();

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
void Xtransforms::vH55_T24_Correct_Streams(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (G_bEachMainWithSub(CK_TABLE, CK_KEYEXISTS)) {
    vR_H55_T24_Corrects_Streams();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH55_T26_Correct_Predicates(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (G_bEachMainWith2Sub(CK_TABLE, CK_KEYEXISTS, CK_BOOLCALC)) {
    vR_H55_T26_Corrects_Predicates();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH55_T30_Resolve_Parameter_Linkage(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (G_bEachMainWithSub(CK_TABLE,CK_KEYEXISTS)) {

    vTracer("Resolving %s"_Stz,G.p_MainCard->szBearing());
    if (!G_bPlumbForSubMember(CK_TABLE, CK_PARAM))
      continue;

    if (!G_bResolveCard(CK_STMCOL, G.p_param->sz(), G.p_table))
      continue;

    vR_H55_T30_Resolve_Parameter_Linkage();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH55_T40_Resolve_Parameter_Terms(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (G_bEachMainWith2Sub(CK_TABLE, CK_BOOLCALC, CK_PARAMLIST)) {

    vTracer("Resolving Terms\n  %s\n  %s  \n"_Stz,
            G.p_table->szBearing(),
            G.p_boolCalc->szBearing(),
            G.p_paramList->szBearing());

    vR_H55_T40_Resolve_Parameter_Terms();

  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH55_T50_Cull_Exists_Call(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint, bTrace);

  while (G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    vR_H55_T50_Cull_Exists_Call();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* delete <where> if it is effectively empty                                  */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH55_T60_Delete_Empty_where(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("delete <where> if it is effectively empty"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    vR_H55_T60_Delete_Empty_where();
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
void Xtransforms::vH60_T10_Add_Scratch_indexJoin_UnderAllFiltered_table(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("add an <indexJoin> under all <table>'s which are joinable"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (G_bEachMainWithSub(CK_TABLE, CK_FILTER)) {
    vR_H60_T10_Add_indexJoin_UnderFiltered_table();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* convert the column name in every <idxCol> to a $streamOrdinal              */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH60_T20_Affix_idxColOdn_ToAll_idxCol(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("convert the column name in every <idxCol> to a $streamOrdinal"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (G_bEachMainWithSub(CK_TABLE,CK_INDEXJOIN)) {
    vR_H60_T20_Convert_idxCol_name_Of_table_To_ordinal();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* add <bounds> under each filtered <table>                                   */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH60_T30_Add_bounds_UnderAllFiltered_table(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("add <bounds> under each filtered <table>"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (G_bEachMainWithSub(CK_TABLE, CK_FILTER)) {
    vR_H60_T30_Add_bounds_UnderFiltered_table();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* remove <table>/<filter>//<pred> that are now <table>/<joinPred>//<pred>    */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH60_T40_Cull_pred_MovedTo_table_bounds(bool bPrint, bool bTrace)
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
void Xtransforms::vH60_T50_Delete_Empty_table_filter(bool bPrint, bool bTrace)
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

  BoardIter xBI_query(G.pBoard);

  G.p_tableList = new Card_TableList;
  G.pDeck->vAppendMain(G.p_tableList);

  while ( (G.p_MainCard = xBI_query.pEachMain()) ) {
    if (! (G.p_table = G.p_MainCard->pSub_table()))
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

  G.p_joinPredList = new Card_JoinPredList;
  G.pDeck->vAppendMain(G.p_joinPredList);

  BoardIter xBI_query(G.pBoard);
  while ((G.p_MainCard = xBI_query.pEachMain(CK_FILTER))) {
    G.p_boolCalc = G.p_MainCard->pSub_boolCalc();
    if (!G.p_boolCalc)
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


//  G.p_comment = new Card_Comment("((TO BE CODED))"_Stz);
//  G.p_crossPtnJoinList->vAppend(G.p_comment);

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

  //m_pBoard_costEst->vDuplicateBoard(G.pBoard);

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
/* convert all <table> into either <scan> or <indexJoin> or <keyExists>       */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH75_T10_Convert_table_To_EEop(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("convert all <table> into either <scan> or <indexJoin>"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (G_bEachMain(CK_TABLE)) {

    G.p_bounds     = G.p_table->pSub_bounds();
    G.p_index     = G.p_table->pSub_index();
    G.p_indexJoin = G.p_table->pSub_indexJoin();
    G.p_keyExists = G.p_table->pSub_keyExists();

    vR_H75_T10_Convert_table_To_EEop();
  }

  G_vRestart();
  while (G_bEachMain(CK_SCAN)) {
    vR_H75_T10_Convert_table_To_EEop_b();
  }

  G_vRestart();
  while (G_bEachMain(CK_INDEXJOIN)) {
    vR_H75_T10_Convert_table_To_EEop_c();
  }

  G_vRestart();
  while (G_bEachMain(CK_KEYEXISTS)) {
    if (G.p_keyExists) {
      i8 i_Child;
      G.p_comment = _pCard_Comment(G.p_keyExists->pFindChild(CK_COMMENT, i_Child));
      G.p_comment->vValue(SEP_DERIVED);
    }
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH75_T20_Convert_keyExists_filter_To_bounds(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("convert all <table> into either <scan> or <indexJoin>"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (G_bEachMainWithSub(CK_KEYEXISTS,CK_FILTER)) {
    vR_H75_T20_Convert_keyExists_filter_To_bounds();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH75_T30_Correct_stmColOdn_Of_keyExists_stream(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vDescription("((description))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (G_bEachMainWithSub(CK_KEYEXISTS,CK_STREAM)) {
    vR_H75_T30_Correct_stmColOdn_Of_keyExists_stream();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH75_T40_Convert_stmColOdn_Of_bounds_To_stmColNm(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("convert all <table> into either <scan> or <indexJoin>"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (G_bEachMainWithSub(CK_KEYEXISTS,CK_CONSTRAINT)) {
    vR_H75_T40_Convert_stmColOdn_Of_bounds_To_stmColNm();
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
void Xtransforms::vH80_T10_Add_Scratch_indexList_Under_scan(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("append <indexList> to <scan>"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (G_bEachMainWith2Sub(CK_SCAN, CK_TABLE, CK_TABLENM)) {
    vR_H80_T10_Add_indexList_Under_scan();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* convert the column name in every <idxCol> to a $streamOrdinal              */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH80_T20_Affix_idxColOdn_ToAll_idxCol(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("convert the column name in every <idxCol> to a $streamOrdinal"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (G_bEachMainWith2Sub(CK_SCAN, CK_STREAM, CK_IDXCOLLIST)) {
    vR_H80_T20_Convert_idxCol_name_Of_scan_To_ordinal();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH80_T30_Mark_ULRangeOf_scan_filter_cmpPred(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (G_bEachMainWithSub(CK_SCAN, CK_FILTER)) {
    vR_H80_T30_Mark_ULBoundsOf_scan_filter_cmpPred();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* mark the <SCAN>//<filter>//cmpPred> which match index ordinals             */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH80_T40_Mark_IndexMatchOf_scan_filter_terms(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("mark <SCAN>//<filter>//<term> ordinals which match index"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (G_bEachMainWith2Sub(CK_SCAN, CK_FILTER, CK_INDEXLIST)) {
    vR_H80_T40_Mark_IndexMatchOf_scan_filter_terms();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH80_T50_Add_scan_bounds_ByCopyingMarked_filter_cmpPred(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (G_bEachMainWith2Sub(CK_SCAN, CK_FILTER, CK_INDEXLIST)) {
    vR_H80_T50_Add_scan_bounds_ByCopyingMarked_filter_cmpPred();
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH80_T60_Cull_Marked_filter_cmpPred(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (G_bEachMainWithSub(CK_SCAN, CK_FILTER)) {
    vR_H80_T60_Cull_Marked_filter_cmpPred();
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
    //vTracer("Morphing <scan> to <indexScan> %s\n"_Stz,G.p_scan->szBearing());
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
    if (! G_bHasPriorToMainIf(CK_AGGREG, C_UPDATE_MEMBER))
      break;
    vR_H90_T10_Remove_sort_IfSameAs_gby();
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
    vR_HA0_T10_Individuate_scan_filter();
  }

  G_vRestart();
  while (G_bEachMainWithSub(CK_INDEXJOIN,CK_FILTER)) {
    vR_HA0_T10_Individuate_indexJoin_filter_b();
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
    vR_HZ0_T10_Reinstate_stages();
  }

  i8 i_Stage = 1;
  G_vRestart();
  while (G_bEachMain(CK_STAGE))
    G.p_stage->vValueSprint("S%02lld"_Stz,i_Stage++);

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
    if (T_bCombined_Reinstate(G.e_MainCard, G.e_SubsequentToMain)) {
      vR_HZ0_T20_Reinstate_combined();
      G_vSkipNextMain();
    }
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
