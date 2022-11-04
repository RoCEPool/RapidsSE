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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH10_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vHH_Begin();

  m_pCardCtl->vUseExpanded();
  vPrintBench(BK_SUBPIPE);
  m_pCardCtl->vUseShorter();
}

/*----------------------------------------------------------------------------*/
/* linearize board by deleting all redundant <stage>'s                        */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH10_T10_Delete_Redundant_stages(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "linearize board by deleting all duplicate <stage>'s"_Stz);

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
}

/*----------------------------------------------------------------------------*/
/* hide ignorable <stage>'s                                                   */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH10_T15_Hide_Ignorable_stages(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "hide ignorable <stage>'s"_Stz);

  while (G_bEachMain(CK_STAGE)) {
    vR_H10_T15_Hide_Ignorable_stage();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* delete superfluous <absJoin>'s                                             */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH10_T20_Delete_All_absJoin(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "delete superfluous <absJoin>'s"_Stz);

  while (G_bEachMain(CK_ABSJOIN)) {
    vR_H10_T20_Delete_absJoin();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* delete superfluous <combined>'s                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH10_T30_Delete_All_combined(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "delete superfluous <combined>'s"_Stz);

  while (G_bEachMain(CK_COMBINED)) {
    vR_H10_T30_Delete_combined();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* delete superfluous <send>'s                                                */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH10_T40_Delete_All_send(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "delete superfluous <send>'s"_Stz);

  while (G_bEachMain(CK_SEND)) {
    vR_H10_T40_Delete_send();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* replace all <scan>'s with their children                                   */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH10_T50_Cull_scan_Leaving_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "replace all <scan>'s with their children"_Stz);

  while (G_bEachMain(CK_SCAN)) {
    vR_H10_T50_Cull_scan_Leaving_table();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH10_T99_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vHH_End();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                     Table Analysis Hand 20 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH20_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vHH_Begin();

  m_pCardCtl->vUseShorter();
}

/*----------------------------------------------------------------------------*/
/* add a <tableSyn> to all <table>'s                                          */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH20_T10_Affix_tableSyn_ToAll_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "add a <tableSyn> to all <table>'s"_Stz);

  while (G_bEachMainWithSub(CK_TABLE, CK_TABLENM))  {
    vR_H20_T10_Add_tableSyn_ForAll_table();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* add a <tablePtn> under every <table>                                       */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH20_T20_Add_tablePtn_UnderAll_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "add a <tablePtn> under every <table>"_Stz);

  while (G_bEachMainWithSub(CK_TABLE, CK_TABLENM)) {
    vR_H20_T20_Add_tablePtn_UnderAll_table();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* add a <stream> under every <table>                                         */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH20_T30_Add_stream_UnderAll_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "add a <stream> under every <table>"_Stz);

  vStreamAnalysisReset();

  // E <scan>
  while (G_bEachMainWithSub(CK_TABLE, CK_TABLENM)) {
    vR_H30_T10_Add_stream_UnderAll_table();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH20_T99_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vHH_End();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                    Stream Analysis Hand 30 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH30_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vHH_Begin();

  m_pCardCtl->vUseShorter();
}

/*----------------------------------------------------------------------------*/
/* calculate the ordinal $low..$high range for each <stream>                  */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH30_T20_Calc_ordinalRange_ForAll_stream(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "calculate the ordinal $low..$high range for each <stream>"_Stz);

  while (G_bEachMainWithSub(CK_TABLE,CK_STREAM)) {
    vR_H30_T20_Calc_ordinalRange_ForAll_stream();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH30_T99_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vHH_End();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                  Predicate Pullup Hand 40 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH40_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vHH_Begin();

  m_pCardCtl->vUseShorter();
  m_pCardCtl->vPrintSubCards(true,CK_RPNCALC,0);

  vPrintBench(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/* convert <where>//<calc> to <where>//<calcBool>                             */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH40_T10_Convert_where_rpnCalc_To_boolCalc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "rpcCalc of where becomes boolCalc "_Stz);

  while (G_bEachMainWithSub(CK_WHERE, CK_RPNCALC)) {
    vR_H40_T10_Convert_where_rpnCalc_To_boolCalc();
  }

  m_pCardCtl->vPrintSubCards(true,CK_RPNCALC,0);
  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH40_T15_Convert_stmColOdn_To_tblColOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "all $n become $c.n"_Stz);

  while (G_bEachMain()) {

    if (! (   CK_PROJECT == G.e_MainCard
           || CK_WHERE   == G.e_MainCard
        ))
      continue;

    vR_H40_T15_Convert_stmColOdn_To_tblColOdn();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH40_T20_Mark_where_pred_Wrt_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "put a table @c against where pred"_Stz);

  while (G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    vR_H40_T20_Mark_where_pred_Wrt_table();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH40_T30_Move_Marked_where_pred_To_table_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "move @c predicates to a filter under table"_Stz);

  while (G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    vR_H40_T30_Move_Marked_where_pred_To_table_filter();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* delete <where> if it is effectively empty                                  */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH40_T40_Delete_Empty_where(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "delete where if it is effectively empty"_Stz);

  while (G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    vR_H40_T40_Delete_Empty_where();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH40_T99_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCardCtl->vUseShorter();
  m_pCardCtl->vPrintSubCards(false,CK_WHERE,0);

  vHH_End();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                       Flatten Call Hand 50 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH50_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCardCtl->vUseShorter();

  vHH_Begin();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH50_T10_Extract_subSet_Of_call(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  while (G_bEachMain(CK_CALL)) {

    Stz* szSubQueryNm = G.p_call->szValue();

    if (! G_bPushBoard(szSubQueryNm)) {
      vTracer("DID NOT FIND Board: %s\n"_Stz,szSubQueryNm);
      continue;
    }

    vR_H50_T10_Extract_subSet_Of_call();
    vPrintBoard(__LINE__);
    G_vPopBoard();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH50_T20_Copy_Called_Subset(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

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
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH50_T30(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  while (G_bEachMain(CK_CALL)) {
    vR_H50_T30_RemoveDelete_call();
  }

  vSetPrintSubCards(CK_PROJELIST,true);

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH50_T40(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vR_H50_T40();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH50_T50(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vR_H50_T50();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH50_T60(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vR_H50_T60();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH50_T99_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vHH_End();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                     Flatten Exists Hand 55 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH55_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCardCtl->vUseShorter();

  vHH_Begin();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH55_T10_Rework_Exists_SubQueries(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

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
    vPrintBoard(__LINE__);

    G_vPopIter();
  }

  vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH55_T20_Copy_Up_SubQueries(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

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
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH55_T24_Correct_Streams(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  while (G_bEachMainWithSub(CK_TABLE, CK_KEYEXISTS)) {
    vR_H55_T24_Corrects_Streams();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH55_T26_Correct_Predicates(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  while (G_bEachMainWith2Sub(CK_TABLE, CK_KEYEXISTS, CK_BOOLCALC)) {
    vR_H55_T26_Corrects_Predicates();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH55_T30_Resolve_Parameter_Linkage(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  while (G_bEachMainWithSub(CK_TABLE,CK_KEYEXISTS)) {

    vTracer("Resolving %s"_Stz,G.p_MainCard->szBearing());
    if (!G_bPlumbForSubMember(CK_TABLE, CK_PARAM))
      continue;

    if (!G_bResolveCard(CK_STMCOL, G.p_param->sz(), G.p_table))
      continue;

    vR_H55_T30_Resolve_Parameter_Linkage();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH55_T40_Resolve_Parameter_Terms(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  while (G_bEachMainWith2Sub(CK_TABLE, CK_BOOLCALC, CK_PARAMLIST)) {

    vTracer("Resolving Terms\n  %s\n  %s  %s\n"_Stz,
            G.p_table->szBearing(),
            G.p_boolCalc->szBearing(),
            G.p_paramList->szBearing());

    vR_H55_T40_Resolve_Parameter_Terms();

  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH55_T50_Cull_Exists_Call(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  while (G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    vR_H55_T50_Cull_Exists_Call();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* delete <where> if it is effectively empty                                  */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH55_T60_Delete_Empty_where(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "delete <where> if it is effectively empty"_Stz);

  while (G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    vR_H55_T60_Delete_Empty_where();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH55_T99_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vHH_End();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                       Initial Join Hand 60 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH60_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCardCtl->vUseShorter();

  vHH_Begin();
}

/*----------------------------------------------------------------------------*/
/* add an <indexJoin> under all <table>'s which are joinable                  */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH60_T10_Add_Scratch_indexJoin_UnderAllFiltered_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "add an <indexJoin> under all <table>'s which are joinable"_Stz);

  while (G_bEachMainWithSub(CK_TABLE, CK_FILTER)) {
    vR_H60_T10_Add_indexJoin_UnderFiltered_table();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* convert the column name in every <idxCol> to a $streamOrdinal              */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH60_T20_Affix_idxColOdn_ToAll_idxCol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "convert the column name in every <idxCol> to a $streamOrdinal"_Stz);

  while (G_bEachMainWithSub(CK_TABLE,CK_INDEXJOIN)) {
    vR_H60_T20_Convert_idxCol_name_Of_table_To_ordinal();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* add <bounds> under each filtered <table>                                   */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH60_T30_Add_bounds_UnderAllFiltered_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "add <bounds> under each filtered <table>"_Stz);

  while (G_bEachMainWithSub(CK_TABLE, CK_FILTER)) {
    vR_H60_T30_Add_bounds_UnderFiltered_table();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* remove <table>/<filter>//<pred> that are now <table>/<joinPred>//<pred>    */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH60_T40_Cull_pred_MovedTo_table_bounds(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "remove <table>/<filter>//<pred> that are now <table>/<joinPred>//<pred>"_Stz);

  while (G_bEachMainWithSub(CK_TABLE,CK_FILTER)) {
    vR_H60_T40_Cull_filter_pred_MovedUpTo_joinPred();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* delete any <table>/<filter> which is effectively empty                     */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH60_T50_Delete_Empty_table_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "delete any <table>/<filter> which is effectively empty"_Stz);

  while (G_bEachMainWithSub(CK_TABLE,CK_FILTER)) {
    vR_H60_T50_Delete_Empty_filter();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH60_T99_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vHH_End();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                      Join Ordering Hand 70 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH70_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCardCtl->vUseShorter();

  vHH_Begin();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH70_T00_Extract_tableList_From_queryBoard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

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
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH70_T10_Extract_joinPredList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

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
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH70_T20_Compute_coPtnPredList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

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
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH70_T30_Compute_coPtnJoinList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  //vSetupPartitionBoard();

  //--nn--------------==
  // coPtnJoinList

  vR_H70_T30_Compute_coPtnJoinList();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH70_T40_Compute_crossPtnJoinList(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  //vSetupPartitionBoard();

  //--nn--------------==
  // crossPtnJoinList

  vR_H70_T40_Compute_crossPtnJoinList();


//  G.p_comment = new Card_Comment("((TO BE CODED))"_Stz);
//  G.p_crossPtnJoinList->vAppend(G.p_comment);

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH70_T50_Compute_ptndJoinSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  //vSetupPartitionBoard();

  //--nn--------------==
  // ptndJoinSet

  vR_H70_T50_Compute_ptndJoinSet();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH70_T60_Copy_ptndJoinSet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  //vSetupPermuteBoard();

  vR_H70_T60_Copy_ptndJoinSet();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH70_T70_Generate_ptndPermutations(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

//  PermGen::vTest(4);

  //--nn--------------==
  // x

  vR_H70_T70_Generate_ptndPermutations();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH70_T80_Remove_invalidPermutations(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vR_H70_T80_Remove_invalidPermutations();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH70_T90_Setup_costEst(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  //vSetupCostEstBoard();

  //m_pBoard_costEst->vDuplicateBoard(G.pBoard);

  vR_H70_T90_Setup_costEst();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH70_T92_Estimate_ptndJoins(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vR_H70_T92_Estimate_ptndJoins();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH70_T94_Estimate_network(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vR_H70_T94_Estimate_network();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH70_T96_Estimate_joinOrder(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vR_H70_T96_Estimate_joinOrder();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH70_T98_Extract_joinOrderSeq(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vR_H70_T98_Extract_joinOrderSeq();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH70_T99_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vHH_End();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                       EE operators Hand 75 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH75_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCardCtl->vUseShorter();

  vHH_Begin();
}

/*----------------------------------------------------------------------------*/
/* convert all <table> into either <scan> or <indexJoin> or <keyExists>       */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH75_T10_Convert_table_To_EEop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "convert all <table> into either <scan> or <indexJoin>"_Stz);

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
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH75_T20_Convert_keyExists_filter_To_bounds(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "convert all <table> into either <scan> or <indexJoin>"_Stz);

  while (G_bEachMainWithSub(CK_KEYEXISTS,CK_FILTER)) {
    vR_H75_T20_Convert_keyExists_filter_To_bounds();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH75_T30_Correct_stmColOdn_Of_keyExists_stream(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((description))"_Stz);

  while (G_bEachMainWithSub(CK_KEYEXISTS,CK_STREAM)) {
    vR_H75_T30_Correct_stmColOdn_Of_keyExists_stream();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH75_T40_Convert_stmColOdn_Of_bounds_To_stmColNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "convert all <table> into either <scan> or <indexJoin>"_Stz);

  while (G_bEachMainWithSub(CK_KEYEXISTS,CK_CONSTRAINT)) {
    vR_H75_T40_Convert_stmColOdn_Of_bounds_To_stmColNm();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH75_T99_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vHH_End();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                         Index Scan Hand 80 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH80_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCardCtl->vUseShorter();

  vHH_Begin();
}

/*----------------------------------------------------------------------------*/
/* append <indexList> to <scan>                                               */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH80_T10_Add_Scratch_indexList_Under_scan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "append <indexList> to <scan>"_Stz);

  while (G_bEachMainWith2Sub(CK_SCAN, CK_TABLE, CK_TABLENM)) {
    vR_H80_T10_Add_indexList_Under_scan();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* convert the column name in every <idxCol> to a $streamOrdinal              */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH80_T20_Affix_idxColOdn_ToAll_idxCol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "convert the column name in every <idxCol> to a $streamOrdinal"_Stz);

  while (G_bEachMainWith2Sub(CK_SCAN, CK_STREAM, CK_IDXCOLLIST)) {
    vR_H80_T20_Convert_idxCol_name_Of_scan_To_ordinal();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH80_T30_Mark_ULRangeOf_scan_filter_cmpPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  while (G_bEachMainWithSub(CK_SCAN, CK_FILTER)) {
    vR_H80_T30_Mark_ULBoundsOf_scan_filter_cmpPred();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* mark the <SCAN>//<filter>//cmpPred> which match index ordinals             */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH80_T40_Mark_IndexMatchOf_scan_filter_terms(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "mark <SCAN>//<filter>//<term> ordinals which match index"_Stz);

  while (G_bEachMainWith2Sub(CK_SCAN, CK_FILTER, CK_INDEXLIST)) {
    vR_H80_T40_Mark_IndexMatchOf_scan_filter_terms();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH80_T50_Add_scan_bounds_ByCopyingMarked_filter_cmpPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  while (G_bEachMainWith2Sub(CK_SCAN, CK_FILTER, CK_INDEXLIST)) {
    vR_H80_T50_Add_scan_bounds_ByCopyingMarked_filter_cmpPred();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH80_T60_Cull_Marked_filter_cmpPred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  while (G_bEachMainWithSub(CK_SCAN, CK_FILTER)) {
    vR_H80_T60_Cull_Marked_filter_cmpPred();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH80_T70_Promote_scan_To_indexScan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

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
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH80_T99_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vHH_End();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                    Stream Ordering Hand 90 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH90_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCardCtl->vUseShorter();

  vHH_Begin();
}

/*----------------------------------------------------------------------------*/
/* remove <sort> if <sbyElist> is the same as the <gbyElist>                  */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH90_T10_Remove_sort_IfSameAs_gby(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "remove <sort> if <sbyElist> is the same as the <gbyElist>"_Stz);

  while (G_bEachMain(CK_SORT)) {
    if (! G_bHasPriorToMainIf(CK_AGGREG, C_UPDATE_MEMBER))
      break;
    vR_H90_T10_Remove_sort_IfSameAs_gby();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vH90_T99_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vHH_End();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                  Filter Refinement Hand A0 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vHA0_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCardCtl->vUseShorter();

  vHH_Begin();
}

/*----------------------------------------------------------------------------*/
/* individuate <scan|indexJoin>/<filter> as main <filter>                     */
/*----------------------------------------------------------------------------*/
void Xtransforms::vHA0_T10_Individuate_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "individuate <scan|indexJoin>/<filter> as main <filter>"_Stz);

  while (G_bEachMainWithSub(CK_SCAN,CK_FILTER)) {
    vR_HA0_T10_Individuate_scan_filter();
  }

  G_vRestart();
  while (G_bEachMainWithSub(CK_INDEXJOIN,CK_FILTER)) {
    vR_HA0_T10_Individuate_indexJoin_filter_b();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vHA0_T99_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vHH_End();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                   Restate Pipeline Hand Z0 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vHZ0_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCardCtl->vUseShorter();

  vHH_Begin();
}

/*----------------------------------------------------------------------------*/
/* reinstate stages                                                           */
/*----------------------------------------------------------------------------*/
void Xtransforms::vHZ0_T10_Reinstate_stages(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "reinstate stages"_Stz);

  while (G_bEachMain() && G_bHasSubsequentToMain(C_UPDATE_MEMBER))  {
    vR_HZ0_T10_Reinstate_stages();
  }

  i8 i_Stage = 1;
  G_vRestart();
  while (G_bEachMain(CK_STAGE))
    G.p_stage->vValueSprint("S%02lld"_Stz,i_Stage++);

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vHZ0_T20_Reinstate_combined(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  while (G_bEachMain()) {
    G_vSubsequentToMain();
    if (T_bCombined_Reinstate(G.e_MainCard, G.e_SubsequentToMain)) {
      vR_HZ0_T20_Reinstate_combined();
      G_vSkipNextMain();
    }
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vHZ0_T99_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vHH_End();
}



/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vHH_Begin(void)
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
void Xtransforms::vHH_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}


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
