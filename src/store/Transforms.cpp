/*============================================================================*/
/* Transforms.cpp --                                                          */
/*============================================================================*/

#include "Transforms.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Transforms,CN_TRANSFORMS)
__aNewFunction(Transforms,CN_TRANSFORMS)
__vDeleteFunction(Transforms,CN_TRANSFORMS)
__aFooterFunction(Transforms)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Transforms::Transforms(Bench* pBench)
: Xlator(CN_TRANSFORMS, pBench)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  _checkInitialised_(CN_TRANSFORMS);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Transforms::~Transforms(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                   Reduce Pipeline Phase 10 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* linearize board by deleting all duplicate <stage>'s                        */
/*----------------------------------------------------------------------------*/
void Transforms::vP10_T10_Delete_Redundant_stage(bool bPrint, bool bTrace)
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
    E_vRemoveDeleteMain(CK_STAGE);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  M.xBI.vRestart();
  while (E_bEachMain(CK_STAGE)) {
    if (! (M.p_topology = M.p_stage->pSub_topology()) )
      continue;
    if (YG_ALL_PARTITIONS == (eYG) M.p_topology->Y_iVal(P_StageTopo))
      continue;

    M.p_stage->Y_vSet(P_Exclude);
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* delete superfluous <absJoin>'s                                             */
/*----------------------------------------------------------------------------*/
void Transforms::vP10_T20_Delete_absJoin(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("delete superfluous <absJoin>'s"_Stz);
  vTransform((char *) (__func__), C_PRINT_XFORM);
  vTracing(bPrint,bTrace);

  //M.pDeck->vDump(0,(Stz*)(__func__));

  while (E_bEachMain(CK_ABSJOIN))
    E_vRemoveDeleteMain(CK_ABSJOIN);

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* delete superfluous <combined>'s                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vP10_T30_Delete_combined(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("delete superfluous <combined>'s"_Stz);
  vTransform((char *) (__func__), C_PRINT_XFORM);
  vTracing(bPrint,bTrace);

  //M.pDeck->vDump(0,(Stz*)(__func__));

  while (E_bEachMain(CK_COMBINED))
    E_vRemoveDeleteMain(CK_COMBINED);

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* delete superfluous <send>'s                                                */
/*----------------------------------------------------------------------------*/
void Transforms::vP10_T40_Delete_send(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("delete superfluous <send>'s"_Stz);
  vTransform((char *) (__func__), C_PRINT_XFORM);
  vTracing(bPrint,bTrace);

  //M.pDeck->vDump(0,(Stz*)(__func__));

  while (E_bEachMain(CK_SEND))
    E_vRemoveDeleteMain(CK_SEND);

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* replace all <scan>'s with their children                                   */
/*----------------------------------------------------------------------------*/
void Transforms::vP10_T50_Cull_scan_Leaving_table(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("replace all <scan>'s with their children"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  //M.pDeck->vDump(0,(Stz*)(__func__));

  while (E_bEachMain(CK_SCAN)) {
    M.pDeck->vReplaceChildWithItsSubs(M.p_MainCard);
    DELETE_CARD(M.p_MainCard);
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                   Access Metadata Phase 20 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* add a <tableSyn> to all <table>'s                                          */
/*----------------------------------------------------------------------------*/
void Transforms::vP20_T10_Add_tableSyn_ForAll_table(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("add a <tableSyn> to all <table>'s"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  //M.pDeck->vDump();

  while (E_bEachMainWithSub(CK_TABLE,CK_TABLENM))  {

    m_pTable = A_pAccess_Table(M.p_tableNm->sz());

    M.p_table->vMark1(MARK_CHANGED);
    char cTableSyn_actual = m_cTableSyn;
    if (m_pTable->bIsReplicated())
      cTableSyn_actual = StrLib::cToUpper(cTableSyn_actual);

    M.p_table->vValueSprint("%c"_Stz,cTableSyn_actual);

    m_cTableSyn++;
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* add a <tablePtn> under every <table>                                       */
/*----------------------------------------------------------------------------*/
void Transforms::vP20_T20_Add_tablePtn_UnderAll_table(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("add a <tablePtn> under every <table>"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (E_bEachMainWithSub(CK_TABLE,CK_TABLENM)) {

    m_pTable = A_pAccess_Table(M.p_tableNm->sz());
    A_vAccess_PartitionOfTable();

    //N_vNewMember(CK_TABLEPTN);
    //M.p_tablePtn->vAdd_TablePtn(M.p_table);
    M.p_table->vAppend(M.p_tablePtn);
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* add a <stream> under every <table>                                         */
/*----------------------------------------------------------------------------*/
void Transforms::vP20_T30_Add_stream_UnderAll_table(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("add a <stream> under every <table>"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  // E <scan>
  while (E_bEachMainWithSub(CK_TABLE,CK_TABLENM)) {

    m_pTable = A_pAccess_Table(M.p_tableNm->sz());
    A_vAccess_StreamOfTable();

    M.p_table->vAppend(M.p_stream);


    /*
    // ? <scan>/<tablePtr>
    if (! (M.p_tablePtr = M.p_table->pSub_tablePtr()))
      continue;
    // ? <scan>/<tablePtr>:pStreamDef
    if (! (m_pStreamDef = M.p_tablePtr->pStreamDef()))
      continue;

    // + <scan>/<stream>
    M.p_table->vAppend((M.p_stream = new Card_Stream));
    // + <scan>/<stream>/<streamColList>
    M.p_stream->vAppend((M.p_stmColList = new Card_StmColList(m_pStreamDef)));
    */
  }


  vEndTransform();
  vPrintBoard();

  vSetPrintByKind(CK_STREAM, true);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                   Stream Analysis Phase 30 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* calculate the ordinal $low..$high range for each <stream>                  */
/*----------------------------------------------------------------------------*/
void Transforms::vP30_T10_Calc_ordinalRange_ForAll_stream(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("calculate the ordinal $low..$high range for each <stream>"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  i8 iOrdLo = 1;
  i8 iOrdHi = C_UNKNOWN;
  i8 nStreamCols = C_UNKNOWN;

  while (E_bEachMain(CK_TABLE)) {

    M.p_stream = M.p_table->pSub_stream();

    // <scan>/<stream>:iOrdLo,iOrdHi
    nStreamCols = M.p_stream->pSub_stmColList()->nChildCards();
    M.p_stream->Y_vSet(P_OrdLo, iOrdLo);
    iOrdHi = iOrdLo + nStreamCols - 1;
    M.p_stream->Y_vSet(P_OrdHi, iOrdHi);

    M.p_stream->vMark1(MARK_CHANGED);

    CardIter xCI(M.p_stream->pSub_stmColList());
    i8 iOrdinal = iOrdLo;
    while (M.p_stmCol = _pCard_StmCol(xCI.pEachChild(CK_STMCOL))) {
      M.p_stmColOrd = new Card_StmColOrd;
      M.p_stmColOrd->vValueSprint("$%lld"_Stz, iOrdinal);
      M.p_stmCol->vAppend(M.p_stmColOrd);
      iOrdinal++;
    }

    iOrdLo = iOrdHi + 1;
  }

  vEndTransform();
  vPrintBoard();

}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                  Predicate Pulluo Phase 40 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* convert <where>//<calc> to <where>//<calcBool>                             */
/*----------------------------------------------------------------------------*/
void Transforms::vP40_T10_Convert_where_calc_To_where_calcBool(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("convert <where>//<calc> into <where>//<calcBool> "_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (E_bEachMainWithSub(CK_WHERE, CK_RPNCALC)) {
    X_vCalc_To_calcBool();
    X_vSubCard_ReplaceSub(M.p_rpnCalc, M.p_where, M.p_boolCalc);
    D_vDeleteMember(M.p_rpnCalc);
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* inspect <where>//<pred> and attempt to find source <table>                 */
/*----------------------------------------------------------------------------*/
void Transforms::vP40_T20_Mark_where_pred_Wrt_table(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("inspect <where>//<pred> and attempt to find source <table>"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (E_bEachMainWithSub(CK_WHERE, CK_BOOLCALC))
    vMarkPredLeftwards(M.p_boolCalc);

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* inspect all <where>//<pred> and move <predCmp> to new <table>/<filter>     */
/*----------------------------------------------------------------------------*/
void Transforms::vP40_T30_Move_Marked_where_pred_To_table_filter(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("inspect all <where>//<pred> and move <predCmp> to new <table>/<filter>"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (E_bEachMainWithSub(CK_WHERE, CK_BOOLCALC))
    vCopyPredLeftwards(M.p_boolCalc);

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* delete <where> if it is effectively empty                                  */
/*----------------------------------------------------------------------------*/
void Transforms::vP40_T40_Delete_Empty_where(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("delete <where> if it is effectively empty"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (E_bEachMain(CK_WHERE)) {
    M.p_boolCalc = M.p_where->pSub_boolCalc();

    if (0 == M.p_boolCalc->nChildCards()) {
      E_vRemoveDeleteMain(CK_WHERE);
      break;
    }
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                    Flatten Exists Phase 50 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vP50_T10_Rework_Exists_SubQueries(bool bPrint, bool bTrace)
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

      N_vNewMember(CK_KEYEXISTS);

      M.pDeck->vInsertAfter(M.p_keyExists,M.p_stage);
      RASSERT(E_bEachMain());

      while (E_bSubsequentToMain(C_UPDATE_MEMBER)) {
        M.pDeck->vRemove(M.p_SubsequentToMain);
        C_vAppendChild(CK_KEYEXISTS, M.p_SubsequentToMain);
      }
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
void Transforms::vP50_T20_Copy_Up_SubQueries(bool bPrint, bool bTrace)
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
      m_pCard_temp = M.p_keyExists->pDupCardAndSubCards();
      M_vPopIter();
      M.pDeck->vInsertBefore(m_pCard_temp,M.p_where);
      M.p_tstPred->vMark1(MARK_LEFTWARDS);
      E_vSkipNextMain();
      //vPrintBoard();
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
void Transforms::vP50_T30_Resolve_Parameter_Linkage(bool bPrint, bool bTrace)
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

    M.p_stmColOrd = M.p_stmCol->pSub_stmColOrd();
    M.p_stmColOrd->vValue(M.p_stmColOrd->szValue());
    M.p_param->vAppend(M.p_stmColOrd);
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vP50_T40_Resolve_Parameter_Terms(bool bPrint, bool bTrace)
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

      M.p_stmColOrd = M.p_param->pSub_stmColOrd();
      M.p_term->vValue(M.p_stmColOrd->szValue());
    }
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                      Initial Join Phase 60 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* add an <indexJoin> under all <table>'s which are joinable                  */
/*----------------------------------------------------------------------------*/
void Transforms::vP60_T10_Add_indexJoin_UnderAllJoinable_table(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("add an <indexJoin> under all <table>'s which are joinable"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  M.pDeck->vClearMarksRecursive();

  while (E_bEachMainWithSub(CK_TABLE,CK_FILTER)) {

    M.p_tableNm = M.p_table->pSub_tableNm();
    m_pTable = A_pAccess_Table(M.p_tableNm->sz());

    RASSERT(A_bAccess_IndexListOfTable());

    while (M.p_index = _pCard_Index(M.p_indexList->pPopChild())) {
      M.p_indexJoin = new Card_IndexJoin;
      M.p_indexJoin->vAppend(M.p_index);
      //M.p_table->vInsertBefore(M.p_indexJoin,M.p_filter);
      M.p_comment = new Card_Comment(SEP_SCRATCH);
      M.p_table->vAppend(M.p_comment);
      M.p_table->vAppend(M.p_indexJoin);
    }

    D_vDeleteMember(CK_INDEXLIST);
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* convert the column name in every <idxCol> to a $streamOrdinal              */
/*----------------------------------------------------------------------------*/
void Transforms::vP60_T20_Convert_idxCol_name_Of_table_To_idxCol_ord(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("convert the column name in every <idxCol> to a $streamOrdinal"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  M.pDeck->vClearMarksRecursive();

  while (E_bEachMain(CK_TABLE)) {
    if (! (M.p_indexJoin = M.p_table->pSub_indexJoin()))
      continue;

    M.p_stream = M.p_table->pSub_stream();;
    M.p_stmColList = M.p_table->pSub_stmColList();
    M.p_idxColList = M.p_indexJoin->pSub_idxColList();

    X_vIdxColList_Ordinate(M.p_idxColList, M.p_stream);
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* add a <joinPred> under each <indexJoin>                                    */
/*----------------------------------------------------------------------------*/
void Transforms::vP60_T30_Add_joinPred_UnderAll_indexJoin(bool bPrint, bool bTrace)
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


    // - - - -

    M.p_bounds = new Card_Bounds;
    M.p_indexJoin = M.p_table->pSub_indexJoin();
    M.p_stream = M.p_table->pSub_stream();
    M.p_table->vInsertAfter(M.p_bounds, M.p_stream);

    M.p_term = new Card_Term("BOUND"_Stz);
    M.p_bounds->vAppend(M.p_term);

    // - - - -

    // one of {pCard_term_0,pCard_term_1} is $In

    M.p_stream = M.p_table->pSub_stream();
    i8 iStreamOrdLo = M.p_stream->Y_iVal(P_OrdLo);
    i8 iStreamOrdHi = M.p_stream->Y_iVal(P_OrdHi);

    i8 i_iOrdLo_0 = T_iTerm_StreamColNo(m_pTerm_0);
    i8 i_iOrdLo_1 = T_iTerm_StreamColNo(m_pTerm_1);

    if (i_iOrdLo_0 >= iStreamOrdLo && i_iOrdLo_0 <= iStreamOrdHi) {
      M.p_term = new Card_Term("$I1"_Stz);
      M.p_bounds->vAppend(M.p_term);
      M.p_term = new Card_Term(m_pTerm_1->sz());
      M.p_bounds->vAppend(M.p_term);
    }
    else
    if (i_iOrdLo_1 >= iStreamOrdLo && i_iOrdLo_1 <= iStreamOrdHi) {
      M.p_term = new Card_Term("$I1"_Stz);
      M.p_bounds->vAppend(M.p_term);
      M.p_term = new Card_Term(m_pTerm_1->sz());
      M.p_bounds->vAppend(M.p_term);
    }
    else {
      NEVER_REACH
    }

    M.p_cmpPred->vMark1(MARK_UPWARDS);
    M.p_term = new Card_Term(M.p_cmpPred->sz());
    M.p_bounds->vAppend(M.p_term);
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* remove <table>/<filter>//<pred> that are now <table>/<joinPred>//<pred>    */
/*----------------------------------------------------------------------------*/
void Transforms::vP60_T40_Cull_filter_pred_MovedUpTo_joinPred(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("remove <table>/<filter>//<pred> that are now <table>/<joinPred>//<pred>"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (E_bEachMain(CK_TABLE)) {
    if (! (M.p_filter = M.p_table->pSub_filter()))
      continue;

    M.p_boolCalc = M.p_filter->pSub_boolCalc();

    X_vBoolCalc_CullPred(M.p_boolCalc, MARK_UPWARDS);
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* delete any <table>/<filter> which is effectively empty                     */
/*----------------------------------------------------------------------------*/
void Transforms::vP60_T50_Delete_Empty_filter(bool bPrint, bool bTrace)
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

    M.p_MainCard->vRemoveChild(M.p_filter);
    DELETE_CARD(M.p_filter);
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                     Join Ordering Phase 70 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vP70_T00_Extract_tableList_From_queryBoard(bool bPrint, bool bTrace)
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

    M.p_tableList->vAppend(M.p_table->pDupCardAndSubCards());
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vP70_T10_Extract_joinPredList(bool bPrint, bool bTrace)
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

    X_vJoinPredList_Extract(M.p_joinPredList, M.p_boolCalc);
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vP70_T20_Compute_coPtnPredList(bool bPrint, bool bTrace)
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

    M.p_coPtnPred = new Card_CoPtnPred;
    M.p_coPtnPred->vAppend(M.p_cmpPred->pDupCardAndSubCards());

    M.p_coPtnPredList->vAppend(M.p_coPtnPred);
  }

  m_pCardCtl->vPrintName(CK_COLUMNREF,false);
  m_pCardCtl->vPrintProperties(CK_COLUMNREF,false);

  //m_pCardCtl->vPrintForm(CK_COPTNJOIN,CP_COMPRESSED);
  //m_pCardCtl->vPrintForm(CK_CROSSPTNJOIN,CP_COMPRESSED);
  //m_pCardCtl->vPrintForm(CK_CMPPRED,CP_COMPRESSED);

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vP70_T30_Compute_coPtnJoinList(bool bPrint, bool bTrace)
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
void Transforms::vP70_T40_Compute_crossPtnJoinList(bool bPrint, bool bTrace)
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
void Transforms::vP70_T50_Compute_ptndJoinSet(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  //vSetupPartitionBoard();

  //--nn--------------==
  // ptndJoinSet

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
void Transforms::vP70_T60_Copy_ptndJoinSet(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  //vSetupPermuteBoard();

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
void Transforms::vP70_T70_Generate_ptndPermutations(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

//  PermGen::vTest(4);

  //--nn--------------==
  // x


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
void Transforms::vP70_T80_Remove_invalidPermutations(bool bPrint, bool bTrace)
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
void Transforms::vP70_T90_Setup_costEst(bool bPrint, bool bTrace)
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

    M.pDeck->vRemoveChild(M.p_MainCard);
    delete M.p_MainCard;
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vP70_T92_Estimate_ptndJoins(bool bPrint, bool bTrace)
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
    M.p_ptndJoin = _pCard_PtndJoin(M.p_MainCard);
    X_vPtndJoin_JoinEst(M.p_ptndJoin, M.p_tableList);
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vP70_T94_Estimate_network(bool bPrint, bool bTrace)
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
void Transforms::vP70_T96_Estimate_joinOrder(bool bPrint, bool bTrace)
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
void Transforms::vP70_T98_Extract_joinOrderSeq(bool bPrint, bool bTrace)
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

    M.p_ptndJoin = _pCard_PtndJoin(M.p_MainCard);
    X_vJoinOrderSeq_ExtractJoinOrder(M.p_joinOrderSeq, M.p_ptndJoin);
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* convert all <table> into either <scan> or <indexJoin>                      */
/*----------------------------------------------------------------------------*/
void Transforms::vP70_T99_Convert_table_To_scan_Or_indexJoin(bool bPrint, bool bTrace)
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
/*                                                        Index Scan Phase 80 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* append <indexList> to <scan>                                               */
/*----------------------------------------------------------------------------*/
void Transforms::vP80_T10_Insert_indexList_ToAll_scan(bool bPrint, bool bTrace)
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

    m_pTable = A_pAccess_Table(M.p_tableNm->sz());

    if (A_bAccess_IndexListOfTable()) {
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
void Transforms::vP80_T20_Convert_idxCol_name_Of_scan_To_idxCol_ord(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("convert the column name in every <idxCol> to a $streamOrdinal"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  M.pDeck->vClearMarksRecursive();

  while (E_bEachMainWithSub(CK_SCAN,CK_IDXCOLLIST)) {
    M.p_stream = M.p_scan->pSub_stream();
    X_vIdxColList_Ordinate(M.p_idxColList, M.p_stream);
  }

  vEndTransform();
  vPrintBoard();
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vP80_T30_Mark_scan_pred_bounds(bool bPrint, bool bTrace)
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
    M.p_filter->vApplyRecursive(CK_CMPPRED, bfMarkPredBounds);
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/* mark the <SCAN>//<filter>//cmpPred> which match index ordinals             */
/*----------------------------------------------------------------------------*/
void Transforms::vP80_T40_Mark_scan_filter_term_WhichMatchIndex(bool bPrint, bool bTrace)
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

    M.p_filter->vApplyRecursive(CK_TERM, bfPredBounds);
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vP80_T50_Add_scanPred(bool bPrint, bool bTrace)
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

    M.p_filter->vApplyRecursive(CK_CMPPRED, bfAdd);

  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vP80_T60_Cull_Marked_cmpPred(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (E_bEachMainWithSub(CK_SCAN,CK_FILTER)) {

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
void Transforms::vP80_T70_Promote_scan_To_indexScan(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("((replace with an explanation))"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (E_bEachMainWithSub(CK_SCAN,CK_BOUNDS)) {
    vTracer("Morphing <scan> to <indexScan> %s\n"_Stz,M.p_scan->szBearing());
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
/*                                                   Stream Ordering Phase 90 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* remove <sort> if <sbyElist> is the same as the <gbyElist>                  */
/*----------------------------------------------------------------------------*/
void Transforms::vP90_T10_Remove_sort_IfSameAs_gby(bool bPrint, bool bTrace)
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

    if (M.p_gbyElist->bValuesSameRecursive(M.p_sbyElist)) {
      E_vRemoveDeleteMain(CK_SORT);
      break;
    }
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                 Filter Refinement Phase A0 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* individuate <scan|indexJoin>/<filter> as main <filter>                     */
/*----------------------------------------------------------------------------*/
void Transforms::vPA0_T10_Individuate_filter(bool bPrint, bool bTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  vDescription("individuate <scan|indexJoin>/<filter> as main <filter>"_Stz);
  vTransform(__func__);
  vTracing(bPrint,bTrace);

  while (E_bEachMain(CK_SCAN)) {

    M.p_filter =  M.p_scan->pSub_filter();
    if (! M.p_filter)
      continue;

    M.p_scan->vRemoveChild(M.p_filter);
    M.pDeck->vInsertAfter(M.p_filter, M.p_scan);
  }

  M_vRestart();

  while (E_bEachMain(CK_INDEXJOIN)) {

    M.p_filter = M.p_indexJoin->pSub_filter();
    if (! M.p_filter)
      continue;

    M.p_indexJoin->vRemoveChild(M.p_filter);
    M.pDeck->vInsertAfter(M.p_filter, M.p_indexJoin);
  }

  vEndTransform();
  vPrintBoard();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                  Restate Pipeline Phase Z0 */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* reinstate stages                                                           */
/*----------------------------------------------------------------------------*/
void Transforms::vPZ0_T10_Reinstate_stages(bool bPrint, bool bTrace)
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

    m_pCard_dup = M.p_stage->pDupCardAndSubCards();
    M.pDeck->vInsertAfter(m_pCard_dup, M.p_MainCard);
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
void Transforms::vPZ0_T20_Reinstate_combined(bool bPrint, bool bTrace)
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

    M.p_combined = new Card_Combined;
    M.pDeck->vInsertAfter(M.p_combined, M.p_MainCard);
    E_vSkipNextMain();
  }

  vEndTransform();
  vPrintBoard();
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to assign <predCmp> after <scan>/<absJoin>*/
/*----------------------------------------------------------------------------*/
void Transforms::vMarkPredLeftwards(Card_BoolCalc* p_bool)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_PRED* p_pred = static_cast<Card_PRED*>(p_bool->pGetChild(0));

  vMarkPredDrillDown(p_pred);
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to move <pred> after <scan>/<absJoin>     */
/*----------------------------------------------------------------------------*/
void Transforms::vMarkPredDrillDown(Card_PRED* p_pred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  char cWhichTable = ' ';

  eCK eCard = p_pred->eCard();


  if (CK_ORPRED == eCard) {
    p_pred->vMark1('|');
    return;
  }

  if (CK_CMPPRED == eCard) {
    p_pred->vMark1('?');
    cWhichTable =
        cMovePredLeftwards(p_pred);
    if (C_UNKNOWN != cWhichTable) {
      p_pred->vMark1(MARK_ASSIGNED);
      p_pred->vMark2(cWhichTable);
    }
    return;
  }

  i8 nSubCards = p_pred->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = p_pred->pGetChild(i_Child);

    vMarkPredDrillDown(static_cast<Card_PRED *>(pSubCard));
  }


  if (CK_ANDPRED == eCard) {
    p_pred->vMark1('&');
    Card *pSubCard_0 = p_pred->pGetChild(0);
    i8 iStream_0 = pSubCard_0->cMark2() - '0';

    Card *pSubCard_1 = p_pred->pGetChild(1);
    i8 iStream_1 = pSubCard_1->cMark2() - '0';

    if (iStream_0 == iStream_1) {
      p_pred->vMark1(MARK_ASSIGNED);
      p_pred->vMark2(iStream_0);
    }

    return;
  }
}

/*----------------------------------------------------------------------------*/
/* inspect <PRED>                                                             */
/*----------------------------------------------------------------------------*/
char Transforms::cMovePredLeftwards(Card_PRED* p_pred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 nAbsJoin = 0;
  i8 nTable    = 0;
  char cWhichTable = C_UNKNOWN;

  //vTrace("iAssignPredLeft candidate %p\n"_Stz,p_pred);
  //p_pred->vDump();

  for (i8 i_Main = 0; /* none */ ; i_Main++) {
    Card *pMainCard = M.pDeck->pGetChild(i_Main);

    if (CK_WHERE == pMainCard->eCard())
      return C_UNKNOWN;

    eCK eMainCard = pMainCard->eCard();

    if (CK_TABLE == eMainCard) {
      nTable++;
      cWhichTable = cMovePredToTable(_pCard_Table(pMainCard), p_pred);
      if (C_UNKNOWN != cWhichTable)
        return cWhichTable;
      continue;
    }

    // skip over any other card??
    vTrace("bMovePredLeft skipping mainCard %s\n"_Stz,aCK[eMainCard]);
  }

  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
char Transforms::cMovePredToTable(Card_Table* p_table, Card_PRED* p_pred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_Stream* p_stream = p_table->pSub_stream();

  // are all $n of <pred> in this <stream>?
  if (bStreamContainsPred(p_stream,p_pred)) {
    char cTableSyn = p_table->cValue();
    return cTableSyn;
  }

  return C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* is given <pred> within this stream?                                        */
/*----------------------------------------------------------------------------*/
bool Transforms::bStreamContainsPred(Card_Stream* p_stream, Card_PRED* pCard_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  eCK ePredCard = pCard_PRED->eCard();

  if (CK_TERM == ePredCard) {
    Stz* szTerm = pCard_PRED->sz();
    if (!szTerm->bIsStreamColOrd())
      return true;

    i8 iOrdLo = p_stream->Y_iVal(P_OrdLo);
    i8 iOrdHi = p_stream->Y_iVal(P_OrdHi);

    i8 iOrd = szTerm->iOrdinal();
    if (iOrd > iOrdHi)
      return false;
  }

  i8 nSubCards = pCard_PRED->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = pCard_PRED->pGetChild(i_Child);

    if (! bStreamContainsPred(p_stream,static_cast<Card_PRED*>(pSubCard)))
      return false;
  }

  return true;
}


/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to assign <predCmp> after <scan>/<absJoin>*/
/*----------------------------------------------------------------------------*/
void Transforms::vCopyPredLeftwards(Card_BoolCalc* p_calcBool)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  while (p_calcBool->nChildCards()) {
    Card_PRED* p_pred = static_cast<Card_PRED*>(p_calcBool->pGetChild(0));
    bool bSubRemoved = bCopyPredDrillDown(p_pred,p_calcBool);
    if (! bSubRemoved)
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to move <pred> after <scan>/<absJoin>     */
/*----------------------------------------------------------------------------*/
bool Transforms::bCopyPredDrillDown(Card_PRED* pCard_pred, Card_BoolCalc* p_calcBool)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  if (MARK_ASSIGNED == pCard_pred->cMark1()) {
    Card* pMainCard;
//    i8 iStream = pCard_pred->iMark2();
//    Card_Stream* pCard_stream = m_pBoard->pFindStreamCard(iStream,pMainCard);
    char cTableSyn = pCard_pred->cMark2();
    Card_Table* pCard_table = M.pDeck->pFindTableCard(cTableSyn, pMainCard);

    Card_Filter* pCard_filter_existing = pMainCard->pSub_filter();

    if (pCard_filter_existing) {
      pCard_filter_existing->vMark1(pCard_table->cMark1());
      pCard_filter_existing->vMark2(pCard_table->cMark2());
      vCopyPredToExistingFilter(pCard_filter_existing,pCard_pred);
    }
    else {
      Card_Filter* pCard_filter_new = new Card_Filter;
      pCard_filter_new->vMark1(pCard_table->cMark1());
      pCard_filter_new->vMark2(pCard_table->cMark2());
      pMainCard->vAppend(pCard_filter_new);
      vCopyPredToNewFilter(pCard_filter_new,pCard_pred);
    }

    //pCard_stream->vMark1('#');


    X_vBoolCalc_RemovePred(p_calcBool, pCard_pred);

    return true;
  }

  i8 nSubCards = pCard_pred->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = pCard_pred->pGetChild(i_Child);

    bool bRemoved = bCopyPredDrillDown(static_cast<Card_PRED *>(pSubCard), p_calcBool);
    if (bRemoved)
      return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to move <pred> after <scan>/<absJoin>     */
/*----------------------------------------------------------------------------*/
void Transforms::vCopyPredToExistingFilter(Card_Filter* pCard_filter_existing, Card_PRED* pCard_pred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_BoolCalc* pCard_calcBool = pCard_filter_existing->pSub_boolCalc();

  Card* pCard_top = pCard_calcBool->pPop();

  Card_AndPred* pCard_predAnd = new Card_AndPred;
  pCard_predAnd->vAppend(pCard_top);
  pCard_predAnd->vAppend(pCard_pred);

  pCard_calcBool->vAppend(pCard_predAnd);
}

/*----------------------------------------------------------------------------*/
/* inspect all <filter> and attempt to move <pred> after <scan>/<absJoin>     */
/*----------------------------------------------------------------------------*/
void Transforms::vCopyPredToNewFilter(Card_Filter* p_filter_new, Card_PRED* p_pred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Card_BoolCalc* p_calcBool = new Card_BoolCalc;
  p_filter_new->vAppend(p_calcBool);

  p_calcBool->vAppend(p_pred);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Transforms::vMarkPredAsMoved(Card* pPredCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 nSubCards = pPredCard->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = pPredCard->pGetChild(i_Child);

    //eCK eSubCard = pSubCard->eCard();

    // mark the

    vMarkPredAsMoved(pSubCard);
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
