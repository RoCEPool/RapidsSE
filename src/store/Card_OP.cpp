/*============================================================================*/
/* Card_OP.cpp -- Abstract OP                                                 */
/*============================================================================*/

#include "Card_OP.hpp"

#include "Card_Level.hpp"
#include "Card_Table.hpp"
#include "CardIter.hpp"
#include "Table.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_OP::Card_OP(eCK eCard)
: Card(eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_OP::~Card_OP(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_OP::bIsOp(Card* pCard)
{
  _static_

  eCK eCard = pCard->eCard();

  switch (eCard) {

    case CK_AGGREG:
    case CK_CROSSJOIN:
    case CK_FILTER:
    case CK_GBYAGG:
    case CK_HAVING:
    case CK_INDEXJOIN:
    case CK_INDEXSCAN:
    case CK_KEYEXISTS:
    case CK_KEYNOTEXISTS:
    case CK_LIMIT:
    case CK_ORDAGG:
    case CK_PROJECT:
    case CK_WHERE:
    case CK_SCAN:
    case CK_SORT:
    case CK_TABLE:
    case CK_TOP:
      return true;

    default:
      return false;
  }

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add missing level                                                          */
/*----------------------------------------------------------------------------*/
void Card_OP::vAddLevel(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Level* p_level = pSub_level();
  if (p_level)
    return;

  p_level = new Card_Level(AL_PARTITION);
  vPush(p_level);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vAdd_Pred_Under_filter(Card_PRED *p_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Filter* p_filter = this->pSub_filter();
  if (! p_filter) {
    p_filter = _pCard_Filter(this->pAppendNewCard(CK_FILTER));
    p_filter->vAppendNewCard(CK_BOOLCALC);
  }

  p_filter->vAddPred(p_PRED);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vConvert_rpnCalc_To_boolCalc(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pCI->G.p_boolCalc = pCI->G.p_rpnCalc->pToBoolCalc(pCI);

  vReplaceSubCard(pCI->G.p_rpnCalc,pCI->G.p_boolCalc);

  pCI->G_vDeleteMember(pCI->G.p_rpnCalc);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vConvert_stmColOdn_To_tblColOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! (   CK_AGGREG  == pCI->G.e_MainCard
            || CK_FILTER  == pCI->G.e_MainCard
            || CK_GBYAGG  == pCI->G.e_MainCard
            || CK_PROJECT == pCI->G.e_MainCard
            || CK_WHERE   == pCI->G.e_MainCard))
    return;

  if (CK_AGGREG  == pCI->G.e_MainCard ||
      CK_FILTER  == pCI->G.e_MainCard ||
      CK_GBYAGG  == pCI->G.e_MainCard ||
      CK_PROJECT == pCI->G.e_MainCard) {
    pCI->G.p_level = pCI->G.p_MainCard->pSub_level();
    if (pCI->G.p_level && AL_PARTITION != pCI->G.p_level->pSymbol()->Y_iGet(P_Level))
      return;
  }

  if (CK_WHERE   == pCI->G.e_MainCard)
    vDebugPoint();

  if (CK_AGGREG  == pCI->G.e_MainCard) pCI->G.p_aggreg->vConvert_stmColOdn_terms_To_tblColOdn(pCI);
  else if (CK_FILTER  == pCI->G.e_MainCard) pCI->G.p_filter->vConvert_stmColOdn_terms_To_tblColOdn(pCI);
  else if (CK_GBYAGG  == pCI->G.e_MainCard) pCI->G.p_gbyAgg->vConvert_stmColOdn_terms_To_tblColOdn(pCI);
  else if (CK_PROJECT == pCI->G.e_MainCard) pCI->G.p_project->vConvert_stmColOdn_terms_To_tblColOdn(pCI);
  else if (CK_WHERE   == pCI->G.e_MainCard) pCI->G.p_where->vConvert_stmColOdn_terms_To_tblColOdn(pCI);
  else NEVER_REACH

}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vConvert_tblColOdn_To_stmColOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (pSymbol()->Y_bHas(P_BreakPt)) {
    //vDump();
    vDebugPoint();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pCI,this](Card* pCard)
  {
    Card_Term* p_term = _pCard_Term(pCard);

//    vTrace("CONVERT %s %s (%s %s)\n"_Stz,
//           p_term->szSymbol(),p_term->szBearing(),
//           szCardNm(),szBearing());

    if (pSymbol()->Y_bHas(P_BreakPt)) {
      vDebugPoint();
    }

    if (!p_term->bIsColOdn())
      return C_CONTINUE_RECURSION;

    if (pCI->Analysis.m_pProjection_prior) {
      if (Card_Expr* p_expr_col =
          pCI->Analysis.m_pProjection_prior->pFindExpr(p_term->pSymbol())) {
        p_term->vSymbolMark1(p_expr_col->szSymbol(), MARK_CHANGED);
        //vTrace("  TO %s\n"_Stz,p_expr_col->szSymbol());
        return C_CONTINUE_RECURSION;
      }
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    if (! (pCI->G.p_MainCard->bIsCard(CK_KEYEXISTS) || pCI->G.p_MainCard->bIsCard(CK_KEYNOTEXISTS)))
      return C_CONTINUE_RECURSION;

    // column of this table

    pCI->G.p_projection = pCI->G.p_MainCard->pSub_projection();
    i8 nExpr_prior = pCI->G.p_projection->nChildCards();
    i8 i_Col = p_term->iGetOdn();

    Symbol xSymbol_stmColOdn;
    xSymbol_stmColOdn.vSetStreamOrdinal(nExpr_prior + i_Col);
    p_term->vSymbolMark1(xSymbol_stmColOdn, MARK_CHANGED);

    //vTrace("  TO %s\n"_Stz,xSymbol_stmColOdn.szSymbol());
    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (bIsCard(CK_INDEXSCAN) || bIsCard(CK_SCAN)) {

  }

  if (bIsCard(CK_PROJECT)) {
    if ((pCI->G.p_key = pSub_key()))
      pCI->G.p_key->bApplyRecursive(CK_TERM, bLambdaFn);
    pCI->G.p_projection = pSub_projection();
    pCI->G.p_projection->bApplyRecursive(CK_TERM, bLambdaFn);
  }

  if (bIsCard(CK_FILTER) || bIsCard(CK_WHERE) || bIsCard(CK_HAVING)) {
    pCI->G.p_boolCalc = pSub_boolCalc();
    pCI->G.p_boolCalc->bApplyRecursive(CK_TERM, bLambdaFn);
  }

  if (bIsCard(CK_AGGREG) && bIsPartitionLevel()) {
    pCI->G.p_projection = pSub_projection();
    pCI->G.p_projection->bApplyRecursive(CK_TERM, bLambdaFn);
  }

  if ((bIsCard(CK_GBYAGG) || bIsCard(CK_ORDAGG)) && bIsPartitionLevel()) {
    if ((pCI->G.p_key = pSub_key()))
      pCI->G.p_key->bApplyRecursive(CK_TERM, bLambdaFn);
    pCI->G.p_projection = pSub_projection();
    pCI->G.p_projection->bApplyRecursive(CK_TERM, bLambdaFn);
  }

  if (bIsCard(CK_HASH)) {
    Card_HshCol *p_hshCol = pSub_hshCol();
    if (Card_Expr *p_expr_col =
        pCI->Analysis.m_pProjection_prior->pFindExpr(p_hshCol->pSymbol())) {
      p_hshCol->vSymbolMark1(p_expr_col->szSymbol(), MARK_CHANGED);
    }
  }

  if (bIsCard(CK_INDEXJOIN)) {
    if ((pCI->G.p_key = pSub_key()))
      pCI->G.p_key->bApplyRecursive(CK_TERM, bLambdaFn);

    if (Card_IndexList* p_indexList_curr = pSub_indexList()) {
      if (Card_Index *p_index_chosen = p_indexList_curr->pFindChosenIndex()) {
        p_index_chosen->bApplyRecursive(CK_TERM, bLambdaFn);
      }
    }

    pCI->vPrintBoard(__LINE__);
    vDebugPoint();
  }

  if (bIsCard(CK_KEYEXISTS) || bIsCard(CK_KEYNOTEXISTS)) {
    if (Card_IndexList* p_indexList_curr = pSub_indexList()) {
      if (Card_Index *p_index_chosen = p_indexList_curr->pFindChosenIndex()) {
        p_index_chosen->bApplyRecursive(CK_TERM, bLambdaFn);
      }
    }

    pCI->G.p_constraint = pSub_constraint();
    pCI->G.p_boolCalc = pCI->G.p_constraint->pSub_boolCalc();
    pCI->G.p_boolCalc->bApplyRecursive(CK_TERM, bLambdaFn);
  }

  if (bIsCard(CK_SORT)) {
    if ((pCI->G.p_key = pSub_key()))
      pCI->G.p_key->bApplyRecursive(CK_TERM, bLambdaFn);
  }

  if (pSymbol()->Y_bHas(P_BreakPt)) {
    //vDump();
    vDebugPoint();
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vDeleteFilterIfEmpty(Card_Filter* p_filter)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! p_filter->bIsEmpty())
    return;

  this->vRemoveChild(p_filter);
  DELETE_CARD(p_filter);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vEarlyProject_root(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  switch (pCI->G.e_MainCard) {

    // -- -- -- -- -- -- --
    case CK_AGGREG:
      pCI->G.p_aggreg->vEarlyProject_root(pCI);
      break;

    case CK_GBYAGG:
      pCI->G.p_gbyAgg->vEarlyProject_root(pCI);
      break;

    case CK_ORDAGG:
      pCI->G.p_ordAgg->vEarlyProject_root(pCI);
      break;

    // -- -- -- -- -- -- --
    case CK_PROJECT:
      pCI->G.p_project->vEarlyProject_root(pCI);
      break;

    case CK_INDEXJOIN:
      pCI->G.p_indexJoin->vEarlyProject_root(pCI);
      break;

    // -- -- -- -- -- -- --
    case CK_CROSSJOIN:
    case CK_INDEXSCAN:
    case CK_LIMIT:
    case CK_SCAN:
    case CK_TOP:
      break;

    default:
      NEVER_REACH;
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vEarlyProject_backstep(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  RASSERT(pCI->Early.m_pNeeded_later);

  Card_OP* p_OP_leaf_maybe = pCI->G.p_OP;

//  if (bIsEarlyLeaf(pCI)) {
//    pCI->Early.m_pOP_leaf = pCI->G.p_OP;
//    return;
//  }

  while (pCI->G_bEachMain_reverse()) {

    // which card did we arrive at?
    switch (pCI->G.e_MainCard) {

      // -- -- -- -- -- -- --
      // don't go any further

      case CK_GBYAGG:
        pCI->Early.m_pOP_leaf = p_OP_leaf_maybe;
        return;


      case CK_ORDAGG:
        pCI->Early.m_pOP_leaf = p_OP_leaf_maybe;
        return;

      // -- -- -- -- -- -- --
      // recurse into...

      case CK_CROSSJOIN:
        pCI->vSave();
        pCI->G.p_crossJoin->vEarlyProject_recurse_update(pCI);
        pCI->vRestore();
        return;

      case CK_INDEXJOIN:
        pCI->vSave();
        pCI->G.p_indexJoin->vEarlyProject_recurse_add(pCI);
        pCI->vRestore();
        return;

      case CK_INDEXSCAN:
        pCI->vSave();
        pCI->G.p_indexScan->vEarlyProject_leaf(pCI);
        pCI->vRestore();
        return;

      case CK_KEYEXISTS:
        pCI->vSave();
        pCI->G.p_keyExists->vEarlyProject_recurse_add(pCI);
        pCI->vRestore();
        return;

      case CK_KEYNOTEXISTS:
        pCI->vSave();
        pCI->G.p_keyNotExists->vEarlyProject_recurse_add(pCI);
        pCI->vRestore();
        return;

      case CK_PROJECT:
        if (pCI->G.p_project->pSub_needed()) {
          pCI->vSave();
          pCI->G.p_project->vEarlyProject_needed_recurse(pCI);
          pCI->vRestore();
        }
        else {
          pCI->G.p_project->vEarlyProject_given_leaf(pCI);
        }
        return;

      case CK_SCAN:
        pCI->vSave();
        pCI->G.p_scan->vEarlyProject_leaf(pCI);
        pCI->vRestore();
        return;

      case CK_WHERE:
        pCI->G.p_where->vEarlyProject_recurse_passthru(pCI);
        return;

      // -- -- -- -- -- -- --
      // missing??

      default:
        pCI->G.p_MainCard->vDump();
        NEVER_REACH
    }
  }

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_OP::bIsEarlyLeaf(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  eCK eCard = this->eCard();

  Card* pChild_prev = pCI->pPrevChild();
  eCK   eChild_prev = pChild_prev ? pChild_prev->eCard() : CK_NONE;

  if (CK_NONE      == eChild_prev ||
      CK_CROSSJOIN == eChild_prev ||
      CK_GBYAGG    == eChild_prev ||
      CK_ORDAGG    == eChild_prev ||
      (CK_PROJECT  == eChild_prev && ! pChild_prev->pSub_needed()))
    return true;

  return false;
//  switch (eCard) {
//
//    case CK_INDEXJOIN:
//      if (CK_INDEXSCAN == eChild_prev || CK_SCAN == eChild_prev)
//        return true;
//      return false;
//
//    case CK_INDEXSCAN:
//    case CK_SCAN:
//      return true;
//
//
//
//    default:
//      return false;
//  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_OP::bIsEarlyRoot(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  eCK eCard = this->eCard();

  switch (eCard) {

    case CK_AGGREG:
      if (bIsPartitionLevel())
        return true;
      return false;

    case CK_GBYAGG:
      if (bIsPartitionLevel())
        return true;
      return false;

    case CK_ORDAGG:
      return true;

    case CK_PROJECT:
      if (bIsPartitionLevel())
        return true;
      return false;

    case CK_INDEXJOIN:
      return true;

      // -- -- -- -- -- --
    default:
      return false;
  }

}

/*----------------------------------------------------------------------------*/
/* is this op at node level                                                   */
/*----------------------------------------------------------------------------*/
bool Card_OP::bIsNodeLevel(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Card_Level* p_level = pSub_level())
    if (AL_NODE == p_level->pSymbol()->Y_iGet(P_Level))
      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this op at origin level                                                 */
/*----------------------------------------------------------------------------*/
bool Card_OP::bIsOriginLevel(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Card_Level* p_level = pSub_level())
    if (AL_ORIGIN == p_level->pSymbol()->Y_iGet(P_Level))
      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this op at partition level                                              */
/*----------------------------------------------------------------------------*/
bool Card_OP::bIsPartitionLevel(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (Card_Level* p_level = pSub_level())
    if (AL_PARTITION == p_level->pSymbol()->Y_iGet(P_Level))
      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* does op needs projection analysis?                                         */
/*----------------------------------------------------------------------------*/
bool Card_OP::bNeedsProjectionAnalysis(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (bIsCard(CK_TABLE)        ||
      bIsCard(CK_SCAN)         ||
      bIsCard(CK_KEYEXISTS)    ||
      bIsCard(CK_KEYNOTEXISTS) ||
      bIsCard(CK_INDEXSCAN)    ||
      bIsCard(CK_INDEXJOIN))
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* does op needs projection correction?                                       */
/*----------------------------------------------------------------------------*/
bool Card_OP::bNeedsProjectionCorrection(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (bIsCard(CK_GBYAGG)    ||
      bIsCard(CK_ORDAGG))
    return true;

  return false;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_OP::bPeg_Chosen_index(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_indexList = nullptr;
  pCI->G.p_index = nullptr;

  if (! (pCI->G.p_indexList = pSub_indexList()))
    return false;

  if (! (pCI->G.p_index = pCI->G.p_indexList->pFindChosenIndex()))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_OP::bPeg_Key(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! (pCI->G.p_key = pSub_key()))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_OP::bPeg_Projection(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! (pCI->G.p_projection = pSub_projection()))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vRelegate_table(CardIter* pCI, Card_OP* p_under)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_table->vRemoveSub(p_under);
  p_under->vTransferChildren(pCI->G.p_table);
  p_under->vInsert(pCI->G.p_table);

  pCI->G.pBoard->pDeck()->vReplace(pCI->G.p_table, p_under);

  pCI->G.p_MainCard = p_under;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // restructure

  if ((pCI->G.p_tableNm = pCI->G.p_MainCard->pSub_tableNm())) {
    pCI->G.p_MainCard->vRemoveSub(pCI->G.p_tableNm);
    pCI->G.p_table->vAppend(pCI->G.p_tableNm);
  }
  if ((pCI->G.p_tablePtn = pCI->G.p_MainCard->pSub_tablePtn())) {
    pCI->G.p_MainCard->vRemoveSub(pCI->G.p_tablePtn);
    pCI->G.p_table->vAppend(pCI->G.p_tablePtn);
  }
  if ((pCI->G.p_tableSize = pCI->G.p_MainCard->pSub_tableSize())) {
    pCI->G.p_MainCard->vRemoveSub(pCI->G.p_tableSize);
    pCI->G.p_table->vAppend(pCI->G.p_tableSize);
  }
  if ((pCI->G.p_rowSize = pCI->G.p_MainCard->pSub_rowSize())) {
    pCI->G.p_MainCard->vRemoveSub(pCI->G.p_rowSize);
    pCI->G.p_table->vAppend(pCI->G.p_rowSize);
  }
  if ((pCI->G.p_rowCount = pCI->G.p_MainCard->pSub_rowCount())) {
    pCI->G.p_MainCard->vRemoveSub(pCI->G.p_rowCount);
    pCI->G.p_table->vAppend(pCI->G.p_rowCount);
  }
}

/*----------------------------------------------------------------------------*/
/* table projection analysis                                                  */
/*----------------------------------------------------------------------------*/
void Card_OP::vUpdate_projection(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (!bNeedsProjectionAnalysis() )
    return;

  Card_Projection* p_projection;

  if ((p_projection = pSub_projection()))
    vDeleteChild(p_projection);

  Card_Projection* p_projection_needed;
  if ((p_projection_needed = pSub_projection()))
    vRemoveChild(p_projection_needed);

  if (pSub_keyExists() || pSub_keyNotExists()) {
    p_projection = _pCard_Projection(pCI->Analysis.m_pProjection_prior->pDupCardRecursive());
    vAppend(p_projection);
    pCI->Analysis.m_pProjection_prior = p_projection;
    if (p_projection_needed)
      vAppend(p_projection_needed);
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->G.p_tableNm = pSub_tableNm();

  Table* pTable = Card_Table::pAccess_Table(pCI->G.p_tableNm->szSymbol());
  StreamDef* pStreamDef = pTable->pStreamDef();

  if (pCI->Analysis.m_pProjection_prior)
    p_projection = pCI->Analysis.m_pProjection_prior->pClone();
  else
    p_projection = new Card_Projection;

  //  --  --  --  --  --  --  --  --  --  --  --  --  --

  char cTblSyn = bIsCard(CK_TABLE) ? cSymbol() : pSub_table()->cSymbol();

  i8 i_Expr = p_projection->nExpr() + 1;

  i8 nCols = pStreamDef->nColumns();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    Column *pColumn = pStreamDef->pGetColumn(i_Col);

    Card_Expr* p_expr = new Card_Expr;
    Symbol xSymbol;
    xSymbol.vSetStreamOrdinal(i_Expr++);
    p_expr->vSymbol(xSymbol);
    p_projection->vAppend(p_expr);

    eCT eColumnType = pColumn->eColumnType();

    Card_As *p_as = new Card_As(pColumn->szColNm());
    p_expr->vAppend(p_as);

    Card_RpnCalc *p_rpnCalc = new Card_RpnCalc;
    p_expr->vAppend(p_rpnCalc);
    p_rpnCalc->pSymbol()->Y_vSet(P_Type, szCT_char[eColumnType]);

    Symbol xSymbol_tblCol;
    xSymbol_tblCol.vSetTableColOrdinal(cTblSyn, i_Col + 1);
    Card_Term *p_term = new Card_Term(xSymbol_tblCol);
    p_rpnCalc->vAppend(p_term);

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    p_term->pSymbol()->Y_vSet(P_Column);
    p_term->pSymbol()->Y_vSet(P_Type, szCT_char[eColumnType]);
  }

  vAppend(p_projection);

  pCI->Analysis.m_pProjection_prior = p_projection;

  if (p_projection_needed)
    vAppend(p_projection_needed);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
