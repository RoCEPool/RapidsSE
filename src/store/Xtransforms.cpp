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

  vSH_Begin();

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

  while (G_bEachMain(CK_ABSJOIN)) {
    vR_H10_T10_Delete_absJoin();
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

  while (G_bEachMain(CK_COMBINED)) {
    vR_H10_T20_Delete_combined();
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

  while (G_bEachMain(CK_SEND)) {
    vR_H10_T30_Delete_send();
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

  while (G_bEachMain(CK_STAGE)) {
    vR_H10_T40_Delete_stage();
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

  while (G_bEachMain(CK_SCAN)) {
    vR_H10_T50_Cull_scan_Leaving_table();
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

  G.e_Level = AL_PARTITION;

  while (G_bEachMain()) {
    vR_H10_T60_Add_level();
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

  //G.pDeck->vDump(0,(Stz*)(__func__));

  Card_Stage* p_stage_PRIOR = nullptr;

  while (G_bEachMain(CK_STAGE)) {
    if (! T_bStage_IsRedundant(G.p_stage,p_stage_PRIOR))   {
      p_stage_PRIOR = G.p_stage;
      continue;
    }

    vR_H10_T80_Delete_Redundant_stage();
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

  while (G_bEachMain(CK_STAGE)) {
    vR_H10_T90_Hide_Ignorable_stage();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS10_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSH_End();
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

  vSH_Begin();

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

  while (G_bEachMainWithSub(CK_TABLE, CK_TABLENM))  {
    vR_H20_T10_AAffix_tableSyn_To_table();
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

  while (G_bEachMainWithSub(CK_TABLE, CK_TABLENM)) {
    vR_H20_T20_Produce_tablePtn_Under_table();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS20_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSH_End();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                 vSeries_25_Stream_Analysis */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bS25_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! bEnabled)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS25_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS25_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSH_Begin();

  m_pCardCtl->vUseShorter();

  m_pCardCtl->vPrintSubCards(true,CK_AGGELIST,CK_GBYELIST,CK_PROJELIST,CK_STREAM,0);
}

/*----------------------------------------------------------------------------*/
/* add a <stream> under every <table>                                         */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS25_T10_Produce_stream_UnderAll_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "add a <stream> under every <table>"_Stz);

  vStreamAnalysisReset();

  while (G_bEachMainWithSub(CK_TABLE, CK_TABLENM)) {
    vR_H25_T10_Produce_stream_Under_table();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* add a <stream> under every <project>                                       */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS25_T12_Produce_stream_UnderAll_project(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "add a <stream> under every <project>"_Stz);

  vStreamAnalysisReset();

  while (G_bEachMainWithSub(CK_PROJECT,CK_PROJELIST)) {
    vR_H25_T12_Produce_stream_Under_project();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* add a <stream> under every <aggreg>                                        */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS25_T14_Produce_stream_UnderAll_aggreg(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "add a <stream> under every <aggreg>"_Stz);

  vStreamAnalysisReset();

  while (G_bEachMainWithSub(CK_AGGREG,CK_AGGELIST)) {
    vR_H25_T14_Produce_stream_Under_aggreg();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* derive the ordinal properties &lo..&hi range for each <stream>             */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS25_T20_Derive_ordinalRange_ForAll_table_stream(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "calculate the ordinal $low..$high range for each <stream>"_Stz);

  while (G_bEachMainWithSub(CK_TABLE,CK_STREAM)) {
    vR_H25_T20_Derive_ordinalRange_For_table_stream();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* derive the ordinal properties &lo..&hi range for each <stream>             */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS25_T22_Derive_ordinalRange_ForAll_project_stream(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "calculate the ordinal $low..$high range for each <stream>"_Stz);

  while (G_bEachMainWithSub(CK_PROJECT,CK_STREAM)) {
    vR_H25_T22_Derive_ordinalRange_For_project_stream();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* derive the ordinal properties &lo..&hi range for each <stream>             */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS25_T24_Derive_ordinalRange_ForAll_aggreg_stream(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "calculate the ordinal $low..$high range for each <stream>"_Stz);

  while (G_bEachMainWithSub(CK_AGGREG,CK_STREAM)) {
    vR_H25_T24_Derive_ordinalRange_For_aggreg_stream();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS25_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSH_End();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                  vSeries_27_Query_Analysis */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bS27_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! bEnabled)
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS27_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS27_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSH_Begin();

  m_pCardCtl->vUseShorter();

  m_pCardCtl->vPrintSubCards(true,CK_RPNCALC,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS27_T10_Main_Query_Analysis(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((10))"_Stz);

  while (G_bEachMain(CK_AGGREG)) {
    vR_H29_T10a_Main_Query_Analysis_aggreg();
  }

  G_vRestart();
  while (G_bEachMain(CK_GBYAGG)) {
    vR_H29_T10b_Main_Query_Analysis_gbyagg();
  }

  i8 nBoards = m_pBench->nItems();
  if (nBoards > 1) {
    G_vRestart();
    while (G_bEachMain(CK_CALL)) {
      vR_H29_T10c_Main_Query_Analysis_Of_from();
    }
    G_vRestart();
    while (G_bEachMainWithSub(CK_WHERE, CK_RPNCALC)) {
      vR_H29_T10d_Main_Query_Analysis_Of_where_rpnCalc();
    }
    G_vRestart();
    while (G_bEachMainWithSub(CK_HAVING, CK_RPNCALC)) {
      vR_H29_T10e_Main_Query_Analysis_Of_having_rpnCalc();
    }
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS27_T20_Sub_Query_Analysis(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((20))"_Stz);

  i8 nBoards = m_pBench->nItems();
  if (1 == nBoards) {
    vR_H29_T20a_Sub_Query_Analysis_Only_main();
  }
  else
  if (2 == nBoards) {

    if (G_bPushBoard(1)) {

      while (G_bEachMain(CK_PARAMLIST)) {
        vR_H29_T20b_Sub_Query_Analysis_paramList();
      }

      G_vRestart();
      while (G_bEachMain(CK_AGGREG)) {
        vR_H29_T20c_Sub_Query_Analysis_aggreg();
      }

      G_vRestart();
      while (G_bEachMain(CK_GBYAGG)) {
        vR_H29_T20d_Sub_Query_Analysis_gbyagg();
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
void Xtransforms::vS27_T30(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((30))"_Stz);

  vR_H29_T30();

  vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS27_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSH_End();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                      vSeries_29_To_RpnForm */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bS29_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! bEnabled)
    return false;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS29_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS29_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCardCtl->vUseShorter();

  vSH_Begin();
}

/*----------------------------------------------------------------------------*/
/* convert <where>//<rpnCalc> to <where>//<boolCalc>                          */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS29_T10_Convert_where_rpnCalc_To_boolCalc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "rpcCalc of where becomes boolCalc "_Stz);

  while (G_bEachMainWithSub(CK_WHERE, CK_RPNCALC)) {
    vR_H29_T10_Convert_where_rpnCalc_To_boolCalc();
  }

  m_pCardCtl->vPrintSubCards(true,CK_RPNCALC,0);
  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* convert all <stmColOdn> to <tblColOdn>                                     */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS29_T20_Convert_stmColOdn_To_tblColOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "all $n become $c.n"_Stz);

  while (G_bEachMain()) {

    if (! (   CK_PROJECT == G.e_MainCard
              || CK_WHERE   == G.e_MainCard
    ))
      continue;

    if (CK_PROJECT == G.e_MainCard) {
      G.p_level = G.p_MainCard->pSub_level();
      if (G.p_level && AL_PARTITION != G.p_level->Y_iVal(P_Level))
        continue;
    }

    vR_H29_T20_Convert_stmColOdn_To_tblColOdn();
  }

  V_vNo_Terms_StmColOdn(G.pDeck);

  G.pBoard->vMayHaveStmColOdn(false);

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS29_T30(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((30))"_Stz);

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS29_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSH_End();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                    vSeries_35_Flatten_From */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bS35_T00_Test(bool bEnabled)
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
void Xtransforms::vS35_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS35_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSH_Begin();

  m_pCardCtl->vUseShorter();

  //m_pCardCtl->vPrintSubCards(true,CK_AGGELIST,CK_GBYELIST,CK_PROJELIST,CK_STREAM,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS35_T10_Push_sqB_Into_mqA(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((10))"_Stz);

  while (G_bEachMain(CK_CALL)) {

    Stz* szSubQueryNm = G.p_call->szValue();

    if (! G_bPushBoard(szSubQueryNm)) {
      vTracer("DID NOT FIND Board: %s\n"_Stz,szSubQueryNm);
      continue;
    }

    vR_H35_T10a_Copy_From_SubQuery();

    G_vPopBoard();

    vR_H35_T10b_Push_sqB_Before_mqA();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS35_T20_Delete_call(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((20))"_Stz);

  vSH_DeleteMain(CK_CALL);

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS35_T30_Delete_twist(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((30))"_Stz);

  vSH_DeleteMain(CK_TWIST);

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS35_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSH_End();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                   vSeries_36_Flatten_Where */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bS36_T00_Test(bool bEnabled)
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

  vSH_Begin();

  m_pCardCtl->vUseShorter();

  //m_pCardCtl->vPrintSubCards(true,CK_AGGELIST,CK_GBYELIST,CK_PROJELIST,CK_STREAM,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS36_T10_Push_sqB_Into_mqA(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((yank from subquery))"_Stz);

  while (G_bEachMainWithSub(CK_WHERE, CK_TSTPRED, "exists"_Stz)) {

    Stz* szSubQueryNm = G.p_tstPred->pGetChild(1)->szValue();
    vTracer("SubQueryNm: %s\n"_Stz,szSubQueryNm);

    if (! G_bPushBoard(szSubQueryNm)) {
      vTracer("DID NOT FIND Board: %s\n"_Stz,szSubQueryNm);
      continue;
    }

    vR_H36_T10a_Copy_From_SubQuery();

    G_vPopBoard();

    vR_H36_T10b_Push_sqB_Before_mqA();

    break;
  }

  G.pDeck->vRenumberRecursive();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS36_T20_Transfer_whereB_To_whereA(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "Transfer where-B to where-A"_Stz);

  G.pDeck->vDump();

  while (G_bEachMain(CK_WHERE)) {
    vR_H36_T20a_Get_cmpPred_Of_whereB();
    break;
  }

  while (G_bEachMain(CK_WHERE)) {
    vR_H36_T20b_Push_cmpPred_Onto_whereA();
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
void Xtransforms::vS36_T30_Replace_param_In_whereA(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "Replace param in where-A"_Stz);

  while (G_bEachMainWithSub(CK_WHERE,CK_BOOLCALC)) {
    vR_H36_T30a_Replace_param_In_whereA();
    break;
  }

  G_vRestart();
  while (G_bEachMain(CK_PARAMLIST)) {
    vR_H36_T30b_Delete_param_In_whereB();
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
void Xtransforms::vS36_T40_Delete_tstPred_In_whereA(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "Delete tstPred in where-A"_Stz);

  while (G_bEachMainWithSub(CK_WHERE,CK_BOOLCALC)) {
    vR_H36_T40_Delete_tstPred_In_whereA();
    break;
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS36_T50_Adjust_StmOdn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "Adjust stream lo/ho ordinals"_Stz);

  vR_H36_T50_Adjust_StmOdn();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS36_T60_Adjust_StmColOdn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "Adjust stream lo/ho ordinals"_Stz);

  vR_H36_T60_Adjust_StmColOdn();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS36_T70_Replace_term_In_cmpPred(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "AReplace term in cmpPred"_Stz);

  while (G_bEachMainWithSub(CK_WHERE,CK_BOOLCALC)) {
    vR_H36_T70_Replace_term_In_cmpPred();
  }

  vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS36_T80_Delete_twist(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "AReplace term in cmpPred"_Stz);

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
void Xtransforms::vS36_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSH_End();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                  vSeries_37_Flatten_Having */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bS37_T00_Test(bool bEnabled)
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
void Xtransforms::vS37_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSH_Begin();

  m_pCardCtl->vUseShorter();

  m_pCardCtl->vPrintSubCards(false,CK_STREAM,0);

  //m_pCardCtl->vPrintSubCards(true,CK_AGGELIST,CK_GBYELIST,CK_PROJELIST,CK_STREAM,0);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37_T10_Append_sqB_After_mqA(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "Append subQuery-B after mainQuery-A"_Stz);

  //while (G_bEachMainWithSub(CK_WHERE, CK_TSTPRED, "exists"_Stz)) {

    //Stz* szSubQueryNm = G.p_tstPred->pGetChild(1)->szValue();
    Stz* szSubQueryNm = "__subquery0002"_Stz;
    vTracer("SubQueryNm: %s\n"_Stz,szSubQueryNm);

    if (! G_bPushBoard(szSubQueryNm)) {
      vTracer("DID NOT FIND Board: %s\n"_Stz,szSubQueryNm);
      NEVER_REACH;
    }

    vR_H37_T10a_Copy_sqB();

    G_vPopBoard();

    vR_H37_T10a_Append_sqB_After_mqA();

    //break;
  //}

  G.pDeck->vRenumberRecursive();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37_T20_Find_stmCol_Link(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((30))"_Stz);

  while (G_bResolveCard(CK_PROJECT,C_UNKNOWN)) {
    vR_H37_T20_Find_stmCol_Link();
    break;
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37_T30_Link_sqCall_In_having(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((20))"_Stz);

  while (G_bEachMainWithSub(CK_HAVING,CK_RPNCALC)) {
    vR_H37_T30_Link_sqCall_In_having();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37_T35_Bump_gbyElist_rpnCalc_In_sqB(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((20))"_Stz);

  while (G_bEachMainWithSub(CK_GBYAGG, CK_GBYELIST)) {
    vR_H37_T35_Bump_gbyElist_rpnCalc_In_sqB();
  }

  vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37_T40_Add_sqCol_To_aggGbyElist(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((30))"_Stz);

  while (G_bEachMain()) {

    if (G.e_MainCard == CK_HAVING)
      break;

    if (G.e_MainCard != CK_GBYAGG)
      continue;

    vR_H37_T40Add_sqCol_To_AggGbyElist();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37_T45_Bump_aggElist_rpnCalc_In_sqB(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((20))"_Stz);

  while (G_bEachMainWithSub(CK_GBYAGG, CK_AGGELIST)) {
    vR_H37_T45_Bump_aggElist_rpnCalc_In_sqB();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37_T50_Add_sqCol_To_aggAggElist(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((30))"_Stz);

  while (G_bEachMain()) {

    if (G.e_MainCard == CK_HAVING)
      break;

    if (G.e_MainCard != CK_GBYAGG)
      continue;

    vR_H37_T50_Add_sqCol_To_aggAggElist();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37_T55_Bump_having_rpnCalc_In_sqB(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((20))"_Stz);

  while (G_bEachMainWithSub(CK_HAVING,CK_RPNCALC)) {
    vR_H37_T55_Bump_having_rpnCalc_In_sqB();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37_T60_Unproject_sqCol(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((30))"_Stz);

  while (G_bEachMain(CK_TWIST)) {

    vR_H37_T60_Unproject_sqCol();
    break;
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37_T70_Exhange_sqB_With_mqA(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((30))"_Stz);

  while (G_bEachMain(CK_TWIST)) {

    vR_H37_T70_Exhange_sqB_With_mqA();
    break;
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37_T80_Adjust_StmOdn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((30))"_Stz);

  vR_H37_T80_AdjustStmOdn();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37_T90_Adjust_StmColOdn(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((30))"_Stz);

  vR_H37_T90_Adjust_StmColOdn();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37_TA0_Delete_twist(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((30))"_Stz);

  vSH_DeleteMain(CK_TWIST);

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37_TB0(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vTransform(__func__, "((30))"_Stz);

  vR_H37_TB0();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS37_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSH_End();
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                vSeries_40_Predicate_Pullup */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bS40_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! bEnabled)
    return false;

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

  vSH_Begin();

  m_pCardCtl->vUseShorter();
  m_pCardCtl->vPrintSubCards(true,CK_RPNCALC,0);

  vPrintBench(BK_SUBPIPE);
}

/*----------------------------------------------------------------------------*/
/* put a table @c mark against where predicates which can be moved under table*/
/*----------------------------------------------------------------------------*/
void Xtransforms::vS40_T10_AtMark_where_pred_Wrt_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "put a table @c against where pred"_Stz);

  while (G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    vR_H40_T10_AtMark_where_pred_Wrt_table();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* move where predicates with an @c table mark to new <table>/<filter>        */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS40_T20_Move_AtMarked_where_pred_To_table_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "move @c predicates to a filter under table"_Stz);

  while (G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    vR_H40_T20_Move_AtMarked_where_pred_To_table_filter();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* delete <where> if it is effectively empty                                  */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS40_T30_Delete_Empty_where(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "delete where if it is effectively empty"_Stz);

  while (G_bEachMainWithSub(CK_WHERE, CK_BOOLCALC)) {
    vR_H40_T30_Delete_Empty_where();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS40_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCardCtl->vUseShorter();
  m_pCardCtl->vPrintSubCards(false,CK_WHERE,0);

  vSH_End();
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

  return false;
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

  vSH_Begin();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS60_T05_Twist_in_Becomes_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "add an <indexJoin> under all joinable <table>'s"_Stz);

  while (G_bEachMainWithSub(CK_TWIST, CK_TSTPRED,"in"_Stz)) {
    vR_H60_T05_Twist_in_Becomes_filter();
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

  while (G_bEachMainWithSub(CK_TABLE, CK_FILTER)) {
    vR_H60_T10_Produce_maybeIndexJoin_Under_filter();
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

  while (G_bEachMainWithSub(CK_TABLE,CK_MAYBEINDEXJOIN)) {
    vR_H60_T20_Affix_tblColOdn_To_idxCol();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* copy <table>/<filter> up into <table>/<bounds>                             */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS60_T30_UpCopy_filter_pred_Into_bounds(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "Add <bounds> under each filtered <table>"_Stz);

  while (G_bEachMainWithSub(CK_TABLE, CK_FILTER)) {
    vR_H60_T30_UpCopy_filter_pred_Into_bounds();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* delete <table>/<filter>//<pred> that are now <table>/<bounds>//<pred>      */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS60_T40_Cull_UpCopied_filter_pred(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "remove <table>/<filter>//<pred> that are now <table>/<joinPred>//<pred>"_Stz);

  while (G_bEachMainWithSub(CK_TABLE,CK_FILTER)) {
    vR_H60_T40_Cull_UpCopied_filter_pred();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* delete any <table>/<filter> which is effectively empty                     */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS60_T50_Delete_Empty_filter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "delete any <table>/<filter> which is effectively empty"_Stz);

  while (G_bEachMainWithSub(CK_TABLE,CK_FILTER)) {
    vR_H60_T50_Delete_Empty_filter();
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

  while (G_bEachMain(CK_TWIST)) {
    vR_H60_T60_Delete_Twist();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS60_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSH_End();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                   vSeries_70_Join_Ordering */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Xtransforms::bS70_T00_Test(bool bEnabled)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  if (! bEnabled)
    return false;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS70_T00_Skip(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS70_T00_Begin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  m_pCardCtl->vUseShorter();

  vSH_Begin();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS70_T00_Extract_tableList_From_queryBoard(void)
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
void Xtransforms::vS70_T10_Extract_joinPredList(void)
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
void Xtransforms::vS70_T20_Compute_coPtnPredList(void)
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
void Xtransforms::vS70_T30_Compute_coPtnJoinList(void)
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
void Xtransforms::vS70_T40_Compute_crossPtnJoinList(void)
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
void Xtransforms::vS70_T50_Compute_ptndJoinSet(void)
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
void Xtransforms::vS70_T60_Copy_ptndJoinSet(void)
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
void Xtransforms::vS70_T70_Generate_ptndPermutations(void)
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
void Xtransforms::vS70_T80_Remove_invalidPermutations(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vR_H70_T80_Remove_invalidPermutations();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS70_T90_Setup_costEst(void)
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
void Xtransforms::vS70_T92_Estimate_ptndJoins(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vR_H70_T92_Estimate_ptndJoins();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS70_T94_Estimate_network(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vR_H70_T94_Estimate_network();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS70_T96_Estimate_joinOrder(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vR_H70_T96_Estimate_joinOrder();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS70_T98_Extract_joinOrderSeq(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  vR_H70_T98_Extract_joinOrderSeq();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS70_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSH_End();
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

  return false;
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

  vSH_Begin();
}

/*----------------------------------------------------------------------------*/
/* convert all <table> into either <scan> , <indexJoin> or <keyExists>        */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS75_T10_Morph_table_To_EEop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "convert all <table> into either <scan> or <indexJoin>"_Stz);

  while (G_bEachMain(CK_TABLE)) {

    G.p_bounds = G.p_table->pSub_bounds();
    G.p_index = G.p_table->pSub_index();
    G.p_maybeIndexJoin = G.p_table->pSub_maybeIndexJoin();
    G.p_keyExists = G.p_table->pSub_keyExists();

    vR_H75_T10_Morph_table_To_EEop();
  }

  G_vRestart();
  while (G_bEachMain(CK_SCAN)) {
    vR_H75_T10_Morph_table_To_EEop_b();
  }

  G_vRestart();
  while (G_bEachMain(CK_INDEXJOIN)) {
    vR_H75_T10_Morph_table_To_EEop_c();
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
void Xtransforms::vS75_T20_Morph_keyExists_filter_To_constraint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "convert all <table> into either <scan> or <indexJoin>"_Stz);

  while (G_bEachMainWithSub(CK_KEYEXISTS,CK_FILTER)) {
    vR_H75_T20_Morph_keyExists_filter_To_constraint();
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

  while (G_bEachMainWithSub(CK_KEYEXISTS,CK_STREAM)) {
    vR_H75_T30_Correct_stmColOdn_Of_keyExists_stream();
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

  while (G_bEachMainWithSub(CK_KEYEXISTS,CK_CONSTRAINT)) {
    vR_H75_T40_Convert_stmColOdn_Of_constraint_To_stmColNm();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS75_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSH_End();
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

  return false;
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

  vSH_Begin();
}

/*----------------------------------------------------------------------------*/
/* append <indexList> to <scan>                                               */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS80_T10_Produce_Scratch_indexList_Under_scan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "append <indexList> to <scan>"_Stz);

  while (G_bEachMainWith2Sub(CK_SCAN, CK_TABLE, CK_TABLENM)) {
    vR_H80_T10_Produce_indexList_Under_scan();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/* convert the column name in every <idxCol> to a $streamOrdinal              */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS80_T20_Affix_idxColOdn_ToAll_idxCol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "convert the column name in every <idxCol> to a $streamOrdinal"_Stz);

  while (G_bEachMainWith2Sub(CK_SCAN, CK_STREAM, CK_IDXCOLLIST)) {
    vR_H80_T20_Affix_idxCol_name_Of_scan_To_ordinal();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS80_T30_Mark_ULRangeOf_scan_filter_cmpPred(void)
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
void Xtransforms::vS80_T40_Mark_IndexMatchOf_scan_filter_terms(void)
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
void Xtransforms::vS80_T50_Add_scan_bounds_ByCopyingMarked_filter_cmpPred(void)
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
void Xtransforms::vS80_T60_Cull_Marked_filter_cmpPred(void)
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
void Xtransforms::vS80_T70_Morph_scan_To_indexScan(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  while (G_bEachMainWithSub(CK_SCAN, CK_BOUNDS)) {
    //vTracer("Morphing <scan> to <indexScan> %s\n"_Stz,G.p_scan->szBearing());
    vR_H80_T70_Morph_scan_To_indexScan();
  }

  G_vRestart();
  while (G_bEachMain(CK_INDEXSCAN)) {
    vR_H80_T70_Morph_scan_To_indexScan_b();
  }

  G_vRestart();
  while (G_bEachMainWithSub(CK_INDEXSCAN, CK_INDEXLIST)) {
    vR_H80_T70_Morph_scan_To_indexScan_c();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS80_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSH_End();
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

  vSH_Begin();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS85_T10_Transmute_All_table(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((85 10))"_Stz);

  if (G_bEachMain(CK_TABLE)) {
    vR_H85_T10a_Transmute_table_To_Scan();
    while (G_bEachMain(CK_TABLE)) {
      vR_H85_T10b_Transmute_table_To_crossJoin();
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

  while (G_bEachMainWithSub(CK_CROSSJOIN,CK_TABLENM)) {
    vR_H85_T20_Add_index_For_crossJoin();
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

  while (G_bEachMainWithSub(CK_CROSSJOIN,CK_INDEX)) {
    vR_H85_T30_Add_bounds_For_crossJoin();
  }

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS85_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSH_End();
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

  return false;
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

  m_pCardCtl->vUseShorter();

  vSH_Begin();
}

/*----------------------------------------------------------------------------*/
/* remove <sort> if <sbyElist> is the same as the <gbyElist>                  */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS90_T10_Remove_sort_IfSameAs_gby(void)
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
void Xtransforms::vS90_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSH_End();
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

  return false;
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

  vSH_Begin();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS92_T10(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((10))"_Stz);

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS92_T20(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((20))"_Stz);

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS92_T30(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((30))"_Stz);

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vS92_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSH_End();
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

  return false;
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

  vSH_Begin();
}

/*----------------------------------------------------------------------------*/
/* individuate <scan|indexJoin>/<filter> as main <filter>                     */
/*----------------------------------------------------------------------------*/
void Xtransforms::vSA0_T10_Individuate_filter(void)
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
void Xtransforms::vSA0_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSH_End();
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

  return false;
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

  vSH_Begin();
}

/*----------------------------------------------------------------------------*/
/* individuate <scan|indexJoin>/<filter> as main <filter>                     */
/*----------------------------------------------------------------------------*/
void Xtransforms::vSZ0_T10_Restore_stmColOdn_from_tblColOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "individuate <scan|indexJoin>/<filter> as main <filter>"_Stz);

  G.pBoard->vMayHaveStmColOdn(true);

  while (G_bEachMain()) {

    if (! (   CK_FILTER    == G.e_MainCard
           || CK_INDEXSCAN == G.e_MainCard
           || CK_INDEXJOIN == G.e_MainCard
           || CK_KEYEXISTS == G.e_MainCard
           || CK_PROJECT   == G.e_MainCard
           || CK_WHERE     == G.e_MainCard
    ))
      continue;

    vR_HZ0_T10_Restore_stmColOdn_from_tblColOdn();
  }

  vEndTransform();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vSB0_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSH_End();
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

  vSH_Begin();
}

/*----------------------------------------------------------------------------*/
/* reinstate stages                                                           */
/*----------------------------------------------------------------------------*/
void Xtransforms::vSZ0_T20_Reinstate_stages(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "reinstate stages"_Stz);

  while (G_bEachMain() && G_bHasSubsequentToMain(C_UPDATE_MEMBER))  {
    vR_HZ0_T20_Reinstate_stages();
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

  G.e_Level = AL_PARTITION;

//  G.p_stage = new Card_Stage;
//
//  G.p_topology = new Card_Topology(YG_ALL_PARTITIONS);
//  G.p_stage->vAppend(G.p_topology);
//  G.pDeck->vPush(G.p_stage);

  G.p_PriorToMain = nullptr;
  while (G_bEachMain())  {
    vR_HZ0_T20_Reinstate_stagesV2();
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

  vR_HZ0_T20_Reinstate_stagesV3();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vSZ0_T30_Reinstate_combined(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vTransform(__func__, "((replace with an explanation))"_Stz);

  while (G_bEachMain()) {
    G_vSubsequentToMain();
    if (T_bCombined_Reinstate(G.e_MainCard, G.e_SubsequentToMain)) {
      vR_HZ0_T30_Reinstate_combined();
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

  while (G_bEachMain(CK_STAGE)) {
    if (! G.i_MainCard)
      continue;

    G.p_MainCard->vDump();

    vR_HZ0_T40_Reinstate_next();
  }

  vR_HZ0_T40_Reinstate_next_b();

  vEndTransform();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vSZ0_TZZ_End(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  vSH_End();
}



/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xtransforms::vSH_Begin(void)
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
void Xtransforms::vSH_End(void)
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
