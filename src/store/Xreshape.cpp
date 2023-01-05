/*============================================================================*/
/* Xreshape.cpp --                                                       */
/*============================================================================*/

#include "Xreshape.hpp"

#include "Table.hpp"
#include "PtnDef.hpp"

#include "RelOp.hpp"
#include "Error.hpp"
#include "Xute.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Xreshape)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Xreshape::Xreshape(eCN eClass, Bench* pBench)
: Xgearing(eClass,pBench)
{
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Xreshape::~Xreshape(void) noexcept(false)
{
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* visit each stream and adjust it's lo-hi range                              */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Stream_AdjustLoHi(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  i8 iOdn_shift = 0;

  i8 iLoOdn_running = 0;
  i8 iHiOdn_running = 0;

  // TODO: G_bEachSub(CK_STREAM)

  G_vRestart();
  while (G_bEachMain()) {

//    if (! (G.p_MainCard->bIsCard(CK_TABLE) || G.p_MainCard->bIsCard(CK_PROJECT)))
//      continue;

    G.p_stream = G.p_MainCard->pSub_stream();
    if (! G.p_stream)
      continue;

    if (G.p_MainCard->bIsCard(CK_AGGREG) ||
        G.p_MainCard->bIsCard(CK_GBYAGG) ||
        G.p_MainCard->bIsCard(CK_PROJECT)) {
      iOdn_shift = 0;
      iLoOdn_running = 0;
      iHiOdn_running = 0;
    }

    G.p_stmColList = G.p_stream->pSub_stmColList();
    i8 nStmCols = G.p_stmColList->nChildCards();

    i8 iLoOdn_stream = G.p_stream->iGetOdnLow();
    if (C_UNKNOWN == iLoOdn_stream) {
      i8 iLoOdn_adjusted = iHiOdn_running + 1;
      i8 iHiOdn_adjusted = iLoOdn_adjusted + nStmCols - 1;
      G.p_stream->vAdjust_OrdinalRange(iLoOdn_adjusted, iHiOdn_adjusted);

      iOdn_shift = iHiOdn_adjusted;
      iLoOdn_running = iLoOdn_adjusted;
      iHiOdn_running = iHiOdn_adjusted;

    }
    else {
      i8 iLoOdn_adjusted = iOdn_shift + 1;
      i8 iHiOdn_adjusted = iLoOdn_adjusted + nStmCols - 1;

      G.p_stream->vAdjust_OrdinalRange(iLoOdn_adjusted, iHiOdn_adjusted);
      iOdn_shift = iHiOdn_adjusted;

      iLoOdn_running = iLoOdn_adjusted;
      iHiOdn_running = iHiOdn_adjusted;
    }

  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Resolve_SubqueryCall_In_mq_boolCalc_Into_cmpPred(Card_BoolCalc *p_boolCalc)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //

  auto vResolve = [this,p_boolCalc](Card_PRED* p_pred,
                                    auto&& vResolve /*oddity permits recursive lambda*/)
  {
    if (! p_pred->bIsPRED())
      return; /* this effectively declares resolve as void */

    if (p_pred->bIsCard(CK_CMPPRED)) {

      if (! p_pred->Y_bHas(P_CorrPred))
        return;

      CardIter xCI(p_pred);
      while (Card* p_child = xCI.pEachChild()) {
        if (p_child->bIsPRED()) {
          vResolve(static_cast<Card_PRED*>(p_child), vResolve);
          continue;
        }
        // TODO: test for: == $d.2 %1 => == $1 [TRANSLATE PARAM %1]

        if (Card::bIsParamOrdinal(p_child->szValue())) {
          //R_vUTE_Resolve_Param_Ordinal(_pCard_Term(p_child));
          p_child->vValue("$1"_Stz);  // TODO: hack!!
        }
        else {
          //p_child->vValue("$1"_Stz);
          char cTblSyn = Card::cTableColSyn(p_child->szValue());
          cTblSyn = 'a'; // TODO: hack!!
          p_child->vSetTableColSyn(cTblSyn);
        }
      }
      return;
    }
    else
    if (p_pred->bIsCard(CK_ROPRED)) {
      p_pred->vCard(CK_CMPPRED);
      CardIter xCI(p_pred);
      while (Card* p_child = xCI.pEachChild()) {
        if (p_child->bIsPRED()) {
          vResolve(static_cast<Card_PRED*>(p_child), vResolve);
          continue;
        }
        // TODO: test for: < $a.5 $b.1 __subquery2 => < $a.5 $2

        if (Card::bIsSubQueryName(p_child->szValue())) {
          Card* p_arg = xCI.pPrevChild();

          Card_Term* p_term_new = new Card_Term("$2"_Stz);
          p_pred->vInsertAfter(p_term_new,p_child);

          p_pred->vRemoveChild(p_arg);
          DELETE_CARD(p_arg);

          p_pred->vRemoveChild(p_child);
          DELETE_CARD(p_child);
        }
      }
      return;
    }
    else
    if (p_pred->bIsCard(CK_BOLPRED)) {
      CardIter xCI(p_pred);
      while (Card* p_child = xCI.pEachChild()) {
        if (p_child->bIsPRED()) {
          vResolve(static_cast<Card_PRED *>(p_child), vResolve);
          continue;
        }
      }
      return;
    }
    else
    if (p_pred->bIsCard(CK_ANDPRED) || p_pred->bIsCard(CK_ORPRED)) {
      CardIter xCI(p_pred);
      while (Card_PRED* p_PRED_child = static_cast<Card_PRED*>(xCI.pEachChild())) {
        RASSERT(p_PRED_child->bIsPRED());
        vResolve(p_PRED_child, vResolve);
      }
    }
    else {
      NEVER_REACH;
    }

  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_PRED* p_PRED = static_cast<Card_PRED*>(p_boolCalc->pGetChild(0));

  vResolve(p_PRED, vResolve);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Xreshape::R_iUTE_Stage_Partitioning(Card* p_main_prior,
                                         Card_Combined* p_combined_prior,
                                         Card_Next* p_next_prior,
                                         Card* p_main_curr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card_Table*      p_table_prior      = p_main_prior->pSub_table();
  Card_Stream*     p_stream_prior     = p_main_prior->pSub_stream();
  Card_StmColList* p_stmColList_prior = p_stream_prior->pSub_stmColList();

  Card_Table*      p_table_curr     = p_main_curr->pSub_table();
  Card_IndexList*  p_indexList_curr = p_main_curr->pSub_indexList();
  Card_Index*      p_index_curr     = p_indexList_curr->pFindChosenIndex();
  Card_Bounds*     p_bounds_curr    = p_main_curr->pSub_bounds();

  Card_Hash* p_hash;
  Card_Term* p_term_hash;
  Stz* szTblColOdn;
  Card_StmCol* p_stmCol_hash;
  i8 i_StmCol;

  eXK eCastKind = p_bounds_curr->eCastKind_v2(p_index_curr,
                                              p_table_prior, p_table_curr,
                                              p_term_hash);

  switch (eCastKind) {
    case XK_BROADCAST:
      p_next_prior->vValue("broadcast"_Stz);
      return 0;

    case XK_HASHCAST:
      p_next_prior->vValue("hashcast"_Stz);

      szTblColOdn = p_term_hash->szValue();
      //p_stmCol_hash = p_stmColList_prior->pStmCol_FindBy_tblColOdn(szTblColOdn, i_StmCol);
      //p_hash = Card_Hash::pDerive(p_stmCol_hash);

      RASSERT(G_bResolveSubCard(CK_STMCOL,CK_TBLCOLODN,szTblColOdn,G.i_MainCard));

      p_hash = Card_Hash::pDerive(G.p_stmCol);


      if (p_combined_prior) {
        G_vInsertBefore(p_hash, p_combined_prior);
        return 1;
      }
      else
      if (p_next_prior) {
        G_vInsertBefore(p_hash, p_next_prior);
        Card_Combined* p_combined = new Card_Combined;
        G_vInsertBefore(p_combined, p_next_prior);
        return 2;
      }
      else
        NEVER_REACH

    case XK_UNICAST:
      return 0;

    default:
      NEVER_REACH
  }

  NEVER_REACH
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Unproject_foldExpr_After_having(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);


  if (! G_bResolveCardWithSub(CK_GBYAGG, CK_AGGELIST, G.i_MainCard))
    return;

  G_vNewMember(CK_PROJECT);
  G.pDeck->vAppend(G.p_project);

  G_vNewMemberOf(CK_LEVEL,""_Stz,CK_PROJECT);
  G.p_level->Y_vSet(P_Level, (i8)AL_ORIGIN);

  G_vNewMemberOf(CK_PROJELIST,""_Stz,CK_PROJECT);

  i8 i_Col = 0;
  i8 nChildren_aggElist = G.p_aggElist->nChildCards();
  for (i8 i_expr_aggElist = 0;
       i_expr_aggElist < nChildren_aggElist;
       i_expr_aggElist++) {

    i_Col++;

    G.p_expr = _pCard_Expr(G.p_aggElist->pGetChild(i_expr_aggElist));
    G.p_as = G.p_expr->pSub_as();

    if (Card::bIsSqyColOrdinal(G.p_as->szValue()))
      continue;

    Card_Expr* p_expr_simple = Card_Expr::pProj_Simple_expr_Of(G.p_expr,i_Col);

    G_vAppendChild(CK_PROJELIST,p_expr_simple);
  }


  G.p_stream = Card_Stream::pDerive_Of_project(G.p_projElist);

  G.p_project->vAppend(G.p_stream);

  G_vNewMember(CK_COMMENT,"unproject of Sq1"_Stz);
  G.p_project->vAppend(G.p_comment);


}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::R_vUTE_Unproject_foldExpr_After_where(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  // todo: remove? It is unused


  G_vNewMember(CK_PROJECT);
  G.pDeck->vInsertAfter(G.p_project,G.p_where);

  G_vNewMemberOf(CK_LEVEL,""_Stz,CK_PROJECT);
  G.p_level->Y_vSet(P_Level, (i8)AL_ORIGIN);

  G_vNewMemberOf(CK_PROJELIST,""_Stz,CK_PROJECT);

  // TODO : unproject $1

}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                               vSeries_26_SubQuery_Analysis */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S26_T10d_Main_Query_Analysis_Of_where_rpnCalc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  SubQuery* pSubQuery = m_pBench->pGetBoard((i8)1)->pSubQuery();

  if (pSubQuery->bViaFrom())
    return;

  pSubQuery->vCaller(0);

  // there is a subQuery, how is it called?

  pSubQuery->vClause(QC_WHERE_CLAUSE);

  CardIter xCI(G.p_rpnCalc);
  while ((G.p_term = _pCard_Term(xCI.pEachChild()))) {
    Stz* szValue_term = G.p_term->szValue();

    if (!G_bIsBoard(szValue_term))
      continue;

    if (! (G.p_term = _pCard_Term(xCI.pEachChild())))
      break;

    szValue_term = G.p_term->szValue();

    if (szValue_term->bSame("exists"_Stz)) {
      if ((G.p_term = _pCard_Term(xCI.pEachChild()))) {
        szValue_term = G.p_term->szValue();
        if (szValue_term->bSame("not"_Stz))
          pSubQuery->vCalcOp(QO_NOT_EXISTS_OP);
        else
          pSubQuery->vCalcOp(QO_EXISTS_OP);
      }
      else {
        pSubQuery->vCalcOp(QO_EXISTS_OP);
      }
      break;
    }
    else
    if (szValue_term->bSame("in"_Stz)) {
      if ((G.p_term = _pCard_Term(xCI.pEachChild()))) {
        szValue_term = G.p_term->szValue();
        if (szValue_term->bSame("not"_Stz))
          pSubQuery->vCalcOp(QO_NOT_IN_OP);
        else
          pSubQuery->vCalcOp(QO_IN_OP);
      }
      else {
        pSubQuery->vCalcOp(QO_IN_OP);
      }
      break;
    }
    else
    if (Card::bIsResultOrdinal(szValue_term)) {
      if ((G.p_term = _pCard_Term(xCI.pEachChild()))) {
        szValue_term = G.p_term->szValue();
        if (Card::bIsStreamColOrdinal(szValue_term)) {
          if ((G.p_term = _pCard_Term(xCI.pEachChild()))) {
            szValue_term = G.p_term->szValue();
            if (szValue_term->bSame("in"_Stz)) {
              if ((G.p_term = _pCard_Term(xCI.pEachChild()))) {
                szValue_term = G.p_term->szValue();
                if (szValue_term->bSame("not"_Stz))
                  pSubQuery->vCalcOp(QO_NOT_IN_OP);
                else
                  pSubQuery->vCalcOp(QO_IN_OP);
              }
              else {
                pSubQuery->vCalcOp(QO_IN_OP);
              }
            }
          }
        }
      }
      break;
    }
    else
    {
      eRO eRelnOp = RelOp::eDecode(szValue_term);
      if (RO_NONE != eRelnOp) {
        pSubQuery->vCalcOp(QO_COMPARE_OP);
        pSubQuery->vRelnOp(eRelnOp);
      }
      break;
    }

  }

  SubQuery* pMainQuery = m_pBench->pGetBoard((i8)0)->pSubQuery();
  pMainQuery->vSubQueries(1);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S26_T10e_Main_Query_Analysis_Of_having_rpnCalc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  SubQuery* pSubQuery = m_pBench->pGetBoard((i8)1)->pSubQuery();

  pSubQuery->vCaller(0);

  // there is a subQuery, how is it called?

  pSubQuery->vClause(QC_HAVING_CLAUSE);

  CardIter xCI(G.p_rpnCalc);
  while ((G.p_term = _pCard_Term(xCI.pEachChild()))) {
    Stz* szValue_term = G.p_term->szValue();

    if (!G_bIsBoard(szValue_term))
      continue;

    if (! (G.p_term = _pCard_Term(xCI.pEachChild())))
      break;

    szValue_term = G.p_term->szValue();

    if (szValue_term->bSame("exists"_Stz)) {
      if ((G.p_term = _pCard_Term(xCI.pEachChild()))) {
        szValue_term = G.p_term->szValue();
        if (szValue_term->bSame("not"_Stz))
          pSubQuery->vCalcOp(QO_NOT_EXISTS_OP);
        else
          pSubQuery->vCalcOp(QO_EXISTS_OP);
      }
      else {
        pSubQuery->vCalcOp(QO_EXISTS_OP);
      }
    }
    else
    if (szValue_term->bSame("in"_Stz)) {
      if ((G.p_term = _pCard_Term(xCI.pEachChild()))) {
        szValue_term = G.p_term->szValue();
        if (szValue_term->bSame("not"_Stz))
          pSubQuery->vCalcOp(QO_NOT_IN_OP);
        else
          pSubQuery->vCalcOp(QO_IN_OP);
      }
      else {
        pSubQuery->vCalcOp(QO_IN_OP);
      }
    }
    else
    {
      eRO eRelnOp = RelOp::eDecode(szValue_term);
      if (RO_NONE != eRelnOp) {
        pSubQuery->vCalcOp(QO_COMPARE_OP);
        pSubQuery->vRelnOp(eRelnOp);
      }
    }

    break;
  }

  SubQuery* pMainQuery = m_pBench->pGetBoard((i8)0)->pSubQuery();
  pMainQuery->vSubQueries(1);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                    vSeries_33_To_TblColOdn */
/*============================================================================*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S33_T10_Convert_stmColOdn_To_tblColOdn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this](Card* pCard)
  {
    Card_Term* p_term = _pCard_Term(pCard);
    if (! Card::bIsStreamColOrdinal(p_term->szValue()))
      return C_CONTINUE_RECURSION;

    Stz* szStmColOdn = p_term->szValue();
    if (! G_bResolveSubCard(CK_STREAM,CK_STMCOLODN,szStmColOdn,G.i_MainCard))
      return C_CONTINUE_RECURSION;

    RASSERT(m_pCard_parent->bIsCard(CK_STMCOL));

    G.p_tblColOdn = m_pCard_parent->pSub_tblColOdn();
    p_term->vSetValueMark1(G.p_tblColOdn->szValue(), MARK_CHANGED);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  G.p_MainCard->bApplyRecursive(CK_TERM, bLambdaFn);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                    vSeries_75_EE_operators */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S75_T10_Morph_table_To_EEop_v2(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  Card* pMain = nullptr;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // SCAN
  if (0 == G.i_MainCard)  {
    G.p_scan = new Card_Scan;
    G.p_scan->vTransferChildren(G.p_table);

    G.pBoard->pDeck()->vReplace(G.p_table,G.p_scan);
    G.p_scan->vInsert(G.p_table);

    if (G.p_indexJoin) {
      NEVER_REACH
      G.p_scan->vRemoveChild(G.p_indexJoin);
      DELETE_CARD(G.p_indexJoin);
    }

    pMain = G.p_scan;
  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // KEYEXISTS
  else if (G.p_keyExists) {
    Xute::vChoose_index_For_keyExists(static_cast<CardIter*>(this));

    G.p_table->vRemoveSubCard(G.p_keyExists);

    G.p_keyExists->vTransferChildren(G.p_table);

    G.pBoard->pDeck()->vReplace(G.p_table, G.p_keyExists);
    G.p_keyExists->vInsert(G.p_table);

    G.p_keyExists->vRemoveSubCard(G.p_index);
    G.p_keyExists->vAppend(G.p_index);

    pMain = G.p_keyExists;
  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // KEYNOTEXISTS
  else if (G.p_keyNotExists) {
    Xute::vChoose_index_For_keyNotExists(static_cast<CardIter*>(this));

    G.p_table->vRemoveSubCard(G.p_keyNotExists);

    G.p_keyNotExists->vTransferChildren(G.p_table);

    G.pBoard->pDeck()->vReplace(G.p_table, G.p_keyNotExists);
    G.p_keyNotExists->vInsert(G.p_table);

    G.p_keyNotExists->vRemoveSubCard(G.p_index);
    G.p_keyNotExists->vAppend(G.p_index);

    pMain = G.p_keyNotExists;
  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // CROSSJOIN
//  else if (! G.p_bounds) {
//    Xute::vChoose_index_For_crossJoin(static_cast<CardIter*>(this));
//
//    G.p_crossJoin = new Card_CrossJoin;
//    G.p_crossJoin->vTransferChildren(G.p_table);
//
//    G.pBoard->pDeck()->vReplace(G.p_table,G.p_crossJoin);
//    G.p_crossJoin->vInsert(G.p_table);
//
//    G_vNewMember(CK_BOUNDS);
//    G.p_crossJoin->vAppend(G.p_bounds);
//
//    G_vNewMember(CK_XBOUND);
//    G.p_bounds->vAppend(G.p_xBound);
//
//    G_vNewMember(CK_TERM,"bound"_Stz);
//    G.p_xBound->vAppend(G.p_term);
//
//    G_vNewMember(CK_TERM,"$I1"_Stz);
//    G.p_xBound->vAppend(G.p_term);
//
//    pMain = G.p_crossJoin;
//  }
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // INDEXJOIN
  else {

    if (! G.p_bounds) {
      s256 sErrorMsg;
      sErrorMsg.vSprint("No index for table %s"_Stz,G.p_table->pSub_tableNm()->szValue());
      Error xError(EC_MISSING_JOIN_INDEX,sErrorMsg,(Stz*)__FILE__,__LINE__);
      throw xError;
    }

    Xute::vChoose_index_For_indexJoin(static_cast<CardIter*>(this));

    G_vNewMember(CK_INDEXJOIN);

    G.p_indexJoin->vTransferChildren(G.p_table);

    G.pBoard->pDeck()->vReplace(G.p_table, G.p_indexJoin);
    G.p_indexJoin->vInsert(G.p_table);

    //G.p_indexJoin->vRemoveSubCard(G.p_index);
    //G.p_indexJoin->vAppend(G.p_index);

    pMain = G.p_indexJoin;
  }

  if ((G.p_tableNm = pMain->pSub_tableNm())) {
    pMain->vRemoveSubCard(G.p_tableNm);
    G.p_table->vAppend(G.p_tableNm);
  }
  if ((G.p_tablePtn = pMain->pSub_tablePtn())) {
    pMain->vRemoveSubCard(G.p_tablePtn);
    G.p_table->vAppend(G.p_tablePtn);
  }
  if ((G.p_size = pMain->pSub_size())) {
    pMain->vRemoveSubCard(G.p_size);
    G.p_table->vAppend(G.p_size);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_S75_T40_Convert_stmColOdn_Of_constraint_To_stmColNm(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  i8 i_Main = G.pDeck->iFindChild(G.p_MainCard);

  Card_BoolCalc* p_boolCalc = G.p_constraint->pSub_boolCalc();
  RASSERT(1 == p_boolCalc->nChildCards());

  Card_PRED* p_pred_ROOT = static_cast<Card_PRED*>(p_boolCalc->pGetChild(0));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [this,i_Main](Card_PRED* p_pred, bool bUpwards)
  {
    if (bUpwards)
      return C_CONTINUE_RECURSION;

    if (! p_pred->bIsCard(CK_TERM))
      return C_CONTINUE_RECURSION;

    Card_Term* p_term = _pCard_Term(p_pred);
    if (! Card::bIsStreamColOrdinal(p_term->szValue()))
      return C_CONTINUE_RECURSION;

    G_bResolveSubCard(CK_STMCOL, CK_STMCOLODN, p_term->szValue(), i_Main + 1);

    Card_StmColNm* p_stmColNm = G.p_stmCol->pSub_stmColNm();

    p_term->vSetValueMark1(p_stmColNm->szValue(), MARK_CHANGED);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  bool bIgnore = p_pred_ROOT->bPredRecursive(bLambdaFn);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*                                                vSeries_Z0_Restate_Pipeline */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Xreshape::vR_SZ0_T20_Reinstate_stagesV3(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_TRANSFORMS, _FOOTER_);

  i8 nStages = 0;

  Card_Stage* p_stage = new Card_Stage;
  p_stage->vValueSprint("S%02lld"_Stz,++nStages);
  Card_Topology* p_topology = new Card_Topology(YG_ALL_PARTITIONS);
  p_stage->vAppend(p_topology);
  G.pDeck->vPush(p_stage);

  eAL e_level_running = Card::eStageTopologyToAggrLevel(YG_ALL_PARTITIONS);

  bool bSawAggOp = false;
  bool bSawIndexJoinOp = false;
  bool bSawProjectOp = false;
  bool bSawSortOp = false;

  auto vAddCombined = [this] (Card* p_main, i8& ri_main) {
    Card_Combined* p_combined = new Card_Combined;
    if (C_UNKNOWN == ri_main) {
      G.pDeck->vAppend(p_combined);
      return;
    }
    G.pDeck->vInsertBefore(p_combined, p_main);
    ri_main++;
    RASSERT(G.pDeck->pGetChild(ri_main) == p_main);
  };

  auto vAddNext = [this] (Card* p_main, i8& ri_main, Stz* szNextValue) {
    Card_Next* p_next = new Card_Next;
    p_next->vValue(szNextValue);
    if (C_UNKNOWN == ri_main) {
      G.pDeck->vAppend(p_next);
      return;
    }
    G.pDeck->vInsertBefore(p_next, p_main);
    ri_main++;
    RASSERT(G.pDeck->pGetChild(ri_main) == p_main);
  };

  auto vAddStage = [this] (Card* p_main, i8& ri_main, eAL e_level_running, i8& nStages) {
    Card_Stage *p_stage = new Card_Stage;
    p_stage->vValueSprint("S%02lld"_Stz,++nStages);
    eYG eStageTopo = Card::eAggrLevelToStageTopology(e_level_running);
    Card_Topology *p_topology = new Card_Topology(eStageTopo);
    p_stage->vAppend(p_topology);
    G.pDeck->vInsertBefore(p_stage, p_main);
    ri_main++;
    RASSERT(G.pDeck->pGetChild(ri_main) == p_main);
  };

  G_vRestart();
  if (G_bEachMain(CK_FOLD))
    G_vRemoveDeleteMain(CK_FOLD);

  Card* p_prev = nullptr;
  eCK   e_prev = CK_NONE;
  Card* p_main = p_stage;
  eCK   e_main = p_main->eCard();

  for (i8 i_Main = 1; i_Main < G.pDeck->nChildCards(); i_Main++) {

    p_prev = p_main;
    e_prev = e_main;

    p_main = G.pDeck->pGetChild(i_Main);
    e_main = p_main->eCard();
    eAL e_level_main = (eAL) p_main->pSub_level()->Y_iGet(P_Level);

    // change of level
    if (e_level_main != e_level_running) {
      if (CK_SCAN != e_prev)
        vAddCombined(p_main, i_Main);
      if (e_level_running == AL_ORIGIN &&
          e_level_main == AL_PARTITION)
        vAddNext(p_main, i_Main,"broadcast"_Stz);
      else
        vAddNext(p_main, i_Main,""_Stz);
      e_level_running = e_level_main;
      vAddStage(p_main, i_Main, e_level_running,nStages);
      if (CK_GBYAGG == e_main || CK_AGGREG == e_main || CK_ORDAGG == e_main)
        bSawAggOp = true;
      else
        bSawAggOp = false;
      if (CK_PROJECT == e_main)
        bSawProjectOp = true;
      else
        bSawProjectOp = false;
      continue;
    }

    if (CK_CROSSJOIN == e_main || CK_INDEXJOIN == e_main
                               || CK_KEYEXISTS == e_main
                               || CK_KEYNOTEXISTS == e_main) {
      if (CK_SCAN != e_prev)
        vAddCombined(p_main, i_Main);
      vAddNext(p_main, i_Main,""_Stz);
      vAddStage(p_main, i_Main, e_level_running,nStages);
      bSawAggOp = false;
      bSawIndexJoinOp = true;
      bSawProjectOp = false;
      bSawSortOp = false;
      continue;
    }

    if ((CK_PROJECT == e_main || CK_AGGREG == e_main)
        && (bSawAggOp || bSawIndexJoinOp || bSawProjectOp) ) {
      if (CK_SCAN != e_prev)
        vAddCombined(p_main, i_Main);
      vAddNext(p_main, i_Main,""_Stz);
      vAddStage(p_main, i_Main, e_level_running,nStages);
      bSawAggOp = false;
      bSawIndexJoinOp = false;
      bSawProjectOp = true;
      bSawSortOp = false;
      continue;
    }

    if (CK_AGGREG == e_main && (bSawAggOp || bSawIndexJoinOp || bSawProjectOp)) {
      vAddNext(p_main, i_Main,""_Stz);
      vAddStage(p_main, i_Main, e_level_running,nStages);
      bSawAggOp = false;
      bSawIndexJoinOp = false;
      bSawProjectOp = false;
      bSawSortOp = false;
      continue;
    }

    if ((CK_GBYAGG == e_main || CK_ORDAGG == e_main) &&
        (bSawAggOp || bSawIndexJoinOp || bSawProjectOp)) {
      if (CK_SCAN != e_prev)
        vAddCombined(p_main, i_Main);
      vAddNext(p_main, i_Main,""_Stz);
      vAddStage(p_main, i_Main, e_level_running,nStages);
      bSawAggOp = false;
      bSawIndexJoinOp = false;
      bSawProjectOp = false;
      bSawSortOp = false;
      continue;
    }

    if (CK_SORT == e_main) {
      if (bSawAggOp || bSawIndexJoinOp || bSawProjectOp || bSawSortOp) {
        vAddCombined(p_main, i_Main);
      }

      vAddNext(p_main, i_Main,""_Stz);
      vAddStage(p_main, i_Main, e_level_running,nStages);
      bSawAggOp = false;
      bSawIndexJoinOp = false;
      bSawProjectOp = false;
      bSawSortOp = true;
      continue;
    }

    // if filter then continue?
    if (CK_FILTER == e_main) {
      continue;
    }

    if (bSawAggOp || bSawIndexJoinOp) {
      vAddCombined(p_main, i_Main);
      vAddNext(p_main, i_Main,""_Stz);
      vAddStage(p_main, i_Main, e_level_running,nStages);
      bSawAggOp = false;
      bSawIndexJoinOp = false;
      bSawProjectOp = false;
      bSawSortOp = false;
      continue;
    }

    if (CK_GBYAGG == e_main|| CK_ORDAGG == e_main)
      bSawAggOp = true;
    if (CK_PROJECT == e_main || CK_AGGREG == e_main)
      bSawProjectOp = true;
  }

  e_prev = e_main;

  i8 i_End = C_UNKNOWN;
  if (CK_SCAN != e_prev)
    vAddCombined(p_main, i_End);
  vAddNext(p_main, i_End,""_Stz);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
