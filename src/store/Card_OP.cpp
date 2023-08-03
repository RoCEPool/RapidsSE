/*============================================================================*/
/* Card_OP.cpp -- Abstract OP                                                 */
/*============================================================================*/

#include "Card_OP.hpp"

#include "Card_Level.hpp"
#include "Card_Table.hpp"
#include "CardIter.hpp"
#include "Table.hpp"
#include "Index.hpp"
#include "ColsReq.hpp"

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
    Card_BoolCalc* p_boolCalc = _pCard_BoolCalc(p_filter->pAppendNewCard(CK_BOOLCALC));
  }

  p_filter->vAddPred(p_PRED);
}

/*----------------------------------------------------------------------------*/
/* convert given lambda function to all terms                                 */
/*----------------------------------------------------------------------------*/
void Card_OP::vApply_lambdaFn_ToAll_term(CardIter* pCI,
                                         std::function<bool(Card* pCard)> bfLambdaFn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pCI->G.p_OP->bApplyRecursive(CK_TERM, bfLambdaFn);
}

/*----------------------------------------------------------------------------*/
/* convert colNm to stmCol                                                    */
/*----------------------------------------------------------------------------*/
void Card_OP::vConvert_colNm_To_stmCol(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (pSymbol()->Y_bHas(P_BreakPt))
    vDebugPoint();

  RASSERT(pCI->Prior.m_pProjection);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto b_colNm_To_stmCol = [pCI,this](Card* pCard)
  {
    Card_Term* p_term = _pCard_Term(pCard);
    if (! p_term->pSymbol()->bIsColNm())
      return C_CONTINUE_RECURSION;

    //p_term->vDump(__func__);

//    // todo: delete
//    if (Card_Expr* p_expr_col =
//        pCI->Prior.m_pProjection->pFindExpr(p_term->pSymbol())) {
//      //p_expr_col->vDump();
//      p_term->pSymbol()->vSet(p_expr_col->pSymbol());
//    }
//    else
    if (Card_Expr* p_expr_col =
        pCI->Prior.m_pProjection->pFind_expr_By_as(p_term->pSymbol())) {
//      p_expr_col->vDump(__func__);
      p_term->pSymbol()->vSet(p_expr_col->pSymbol());
    }

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vApply_lambdaFn_ToAll_term(pCI, b_colNm_To_stmCol);
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
void Card_OP:: vConvert_stmColOdn_To_tblColOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! (      CK_AGGREG  == pCI->G.e_MainCard
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
/* convert tblColOdn to stmColOdn                                             */
/*----------------------------------------------------------------------------*/
void Card_OP::vConvert_tblColOdn_To_stmColOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (pSymbol()->Y_bHas(P_BreakPt)) {
    //vDump();
    vDebugPoint();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bf_tblColOdn_To_stmColOdn = [pCI,this](Card* pCard)
  {
    Card_Term* p_term = _pCard_Term(pCard);

    if (pSymbol()->Y_bHas(P_BreakPt)) {
      vDump(__func__);
      vDebugPoint();
    }

    if (! p_term->pSymbol()->bIsColumnOrdinal())
      return C_CONTINUE_RECURSION;

    if (pCI->Prior.m_pProjection) {
      if (Card_Expr* p_expr_col = pCI->Prior.m_pProjection->pFindExpr(p_term->pSymbol())) {
        // record what $c.n it was
        p_term->pSymbol()->Y_vSet(P_TblCol,p_term->szSymbol());
        p_term->vSymbol(p_expr_col->szSymbol());
        p_term->vMark1(MARK_CHANGED);

        return C_CONTINUE_RECURSION;
      }
    }

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

    if (! (pCI->G.p_MainCard->bIsCard(CK_KEYEXISTS) || pCI->G.p_MainCard->bIsCard(CK_KEYNOTEXISTS)))
      return C_CONTINUE_RECURSION;

    // column of this table

    // record what $c.n it was
    p_term->pSymbol()->Y_vSet(P_TblCol,p_term->szSymbol());

    pCI->G.p_projection = pCI->G.p_MainCard->pSub_projection();
    i8 nExpr_prior = pCI->G.p_projection->nChildCards();
    i8 i_Col = p_term->iGetOdn();

    Symbol xSymbol_stmColOdn;
    xSymbol_stmColOdn.vSetStreamOrdinal(nExpr_prior + i_Col);
    p_term->vSymbol(&xSymbol_stmColOdn);
    p_term->vMark1(MARK_CHANGED);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  vApply_lambdaFn_ToAll_term(pCI, bf_tblColOdn_To_stmColOdn);

  if (pSymbol()->Y_bHas(P_BreakPt)) {
    //vDump();
    vDebugPoint();
  }
}

/*----------------------------------------------------------------------------*/
/* correct stmOd                                                              */
/*----------------------------------------------------------------------------*/
void Card_OP::vCorrectStmOdn(Card_Projection* p_projection_prior)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! p_projection_prior)
    return;

  if (Card_Filter* p_filter = pSub_filter())
    p_filter->vCorrectStmOdn(p_projection_prior);

  if (Card_Projection* p_projection = pSub_projection())
    p_projection->vCorrectStmOdn(p_projection_prior);
}

/*----------------------------------------------------------------------------*/
/* delete filter if it's empty                                                */
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

      case CK_FILTER:
        pCI->vSave();
        pCI->G.p_filter->vEarlyProject_recurse_update(pCI);
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
        pCI->G.p_where->vEarlyProject_recurse_update(pCI);
        return;

      // -- -- -- -- -- -- --
      // missing??

      default:
        pCI->G.p_MainCard->vDump(__func__);
        NEVER_REACH
    }
  }

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* does this op bring data into the pipeline                                  */
/*----------------------------------------------------------------------------*/
bool Card_OP::bIsDataInclusion(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  eCK eCard = this->eCard();

  switch (eCard) {
    case CK_INDEXJOIN:
    case CK_INDEXSCAN:
    case CK_SCAN:
    case CK_TABLE:
      return true;
    default:
      return false;
  }
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
      bIsCard(CK_SORT)         ||
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
    p_projection = _pCard_Projection(pCI->Prior.m_pProjection->pDupCardRecursive());
    vAppend(p_projection);
    pCI->Prior.m_pProjection = p_projection;
    if (p_projection_needed)
      vAppend(p_projection_needed);
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->G.p_tableNm = pSub_tableNm();

  Table* pTable = Card_Table::pAccess_Table(pCI->G.p_tableNm->szSymbol());
  StreamDef* pStreamDef = pTable->pStreamDef();

  if (pCI->Prior.m_pProjection)
//    p_projection = pCI->Prior.m_pProjection->pClone();
    p_projection = pCI->Prior.m_pProjection->pReducedCopy(pCI);
  else
    p_projection = new Card_Projection(pCI->iXform());

  //  --  --  --  --  --  --  --  --  --  --  --  --  --

  char cTblSyn = bIsCard(CK_TABLE) ? cSymbol() : pSub_table()->cSymbol();

  i8 i_Expr = p_projection->nExpr() + 1;

  i8 nCols = pStreamDef->nColumns();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    Column *pColumn = pStreamDef->pGetColumn(i_Col);

    Card_Expr* p_expr = new Card_Expr;
    Symbol xSymbol;
    xSymbol.vSetStreamOrdinal(i_Expr++);
    p_expr->pSymbol()->vSet(&xSymbol);
    p_projection->vAppend(p_expr);

    eCT eColumnType = pColumn->eColumnType();

    Card_As *p_as = new Card_As(pColumn->szColNm());
    p_expr->vAppend(p_as);

    Card_RpnCalc *p_rpnCalc = new Card_RpnCalc;
    p_expr->vAppend(p_rpnCalc);
    p_rpnCalc->pSymbol()->Y_vSet(P_Type, szCT_char[eColumnType]);
    p_expr->pSymbol()->Y_vSet(P_Type, szCT_char[eColumnType]);

    Symbol xSymbol_tblCol;
    xSymbol_tblCol.vSetTableColOrdinal(cTblSyn, i_Col + 1);
    Card_Term *p_term = new Card_Term(xSymbol_tblCol.szSF_symbol());
    p_rpnCalc->vAppend(p_term);

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    p_term->pSymbol()->Y_vSet(P_Column);
    p_term->pSymbol()->Y_vSet(P_Type, szCT_char[eColumnType]);
  }

  vAppend(p_projection);

  pCI->Prior.m_pProjection = p_projection;

  if (p_projection_needed)
    vAppend(p_projection_needed);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vCraftArg(BASE::String *pString)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for aggreg op                                               */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnAggreg(Stz* szRpnExpr, Card_Aggreg* pCard_Aggreg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nSubCards;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_Projection* pCard_Projection =
      _pCard_Projection(pCard_Aggreg->pFindSubCard(CK_PROJECTION));

  if (! pCard_Projection)
    return;

  nSubCards = pCard_Projection->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card* pCard_SUB = pCard_Projection->pGet(i_Child);
    if (! pCard_SUB->bIsCard(CK_EXPR))
      return;

    if (i_Child) {
      szRpnExpr->vConcat("\n                 "_Stz);
      szRpnExpr->vConcat(" ,"_Stz);
    }
    else {
      szRpnExpr->vConcat("  "_Stz);
    }

    Card_Expr* pCard_Expr = _pCard_Expr(pCard_SUB);
    vExpr(szRpnExpr,pCard_Expr);
  }


}

/*----------------------------------------------------------------------------*/
/* expr for combine                                                           */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnCombined(Stz* szRpnExpr, Card_Combined* pCard_Combined)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  szRpnExpr->vCatSprint("%s"_Stz, pCard_Combined->szSymbol());
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for crossJoin op                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnCrossJoin(Stz* szRpnExpr, Card_CrossJoin* pCard_CrossJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Bounds* pCard_bounds = pCard_CrossJoin->pSub_bounds();
  RASSERT(pCard_bounds);
  Card_Bound* pCard_Bound = pCard_bounds->pSub_bound();
  RASSERT(pCard_Bound);

  szRpnExpr->vCatSprint(" %s"_Stz,"BOUND"_Stz);

  i8 nSubs = pCard_Bound->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubs; i_Child++) {
    Card* pChildCard = pCard_Bound->pGet(i_Child);

    if (! pChildCard->bIsCard(CK_TERM))
      continue;

    Card_Term* p_term = _pCard_Term(pChildCard);

    if (p_term->pSymbol()->bIsStreamOrdinal() ||
        p_term->pSymbol()->bIsColumnOrdinal()) {

      if (!p_term->pSymbol()->Y_bHas(P_Type)) {
        p_term->vDump(__func__);
        pChildCard->vDump(__func__);
        RASSERT(p_term->pSymbol()->Y_bHas(P_Type));
      }
    }

    szRpnExpr->vCatSprint(" %s"_Stz, p_term->szSymbol());
  }
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for filter op                                               */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnFilter(Stz* szRpnExpr, Card_Filter* pCard_Filter)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  szRpnExpr->vCatSprint(" type BOOLEAN"_Stz);

  Card* pCard_test = pCard_Filter->pSub_boolCalc();
  if (CK_BOOLCALC == pCard_test->eCard()) {
    Card_BoolCalc *pCard_CalcBool = _pCard_BoolCalc(pCard_test);
    vBoolCalc(szRpnExpr, pCard_CalcBool);
  }
  else {
    Card_RpnCalc *pCard_Calc = pCard_Filter->pSub_rpnCalc();
    vRpnCalc(szRpnExpr, pCard_Calc);
  }
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for aggGby op                                               */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnGbyAgg(Stz* szRpnExpr, Card_GbyAgg* pCard_GbyAgg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nSubCards;

  Card_Key* pCard_Key =
      _pCard_Key(pCard_GbyAgg->pFindSubCard(CK_KEY));

  if (! pCard_Key)
    return;

  szRpnExpr->vConcat(" groupBy"_Stz);

  nSubCards = pCard_Key->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card* pCard_SUB = pCard_Key->pGet(i_Child);
    if (! pCard_SUB->bIsCard(CK_EXPR))
      return;

    if (i_Child) {
      szRpnExpr->vConcat("\n                 "_Stz);
      szRpnExpr->vConcat(" ,"_Stz);
    }
    else {
      szRpnExpr->vConcat("\n                 "_Stz);
      szRpnExpr->vConcat("  "_Stz);
    }

    Card_Expr* pCard_Expr = _pCard_Expr(pCard_SUB);
    vExpr(szRpnExpr,pCard_Expr);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  Card_Projection* pCard_Projection =
      _pCard_Projection(pCard_GbyAgg->pFindSubCard(CK_PROJECTION));

  if (! pCard_Projection)
    return;

  szRpnExpr->vConcat("\n                 "_Stz);
  szRpnExpr->vConcat(" aggregates"_Stz);

  nSubCards = pCard_Projection->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card* pCard_SUB = pCard_Projection->pGet(i_Child);
    if (! pCard_SUB->bIsCard(CK_EXPR))
      break;

    if (i_Child) {
      szRpnExpr->vConcat("\n                 "_Stz);
      szRpnExpr->vConcat(" ,"_Stz);
    }
    else {
      szRpnExpr->vConcat("\n                 "_Stz);
      szRpnExpr->vConcat("  "_Stz);
    }

    Card_Expr* pCard_Expr = _pCard_Expr(pCard_SUB);
    vExpr(szRpnExpr,pCard_Expr);
  }

//  vTrace("%s\n"_Stz,szRpnExpr);
//  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* weave : hash  <expr>                                                       */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnHash(Stz* szOpArgs, Card_Hash* pCard_Hash)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_HshCol* p_hshCol = pCard_Hash->pSub_hshCol();
  Card_Term* p_term = pCard_Hash->pSub_term();

  szOpArgs->vCatSprint("type %s %s"_Stz,
                       aCT[Symbol::eTo_eCT_char(p_term->pSymbol()->Y_szGet(P_Type))],
                       p_term->szSymbol());
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for having op                                               */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnHaving(Stz* szRpnExpr, Card_Having* pCard_Having)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  szRpnExpr->vCatSprint(" type BOOLEAN"_Stz);

  Card* pCard_test = pCard_Having->pSub_boolCalc();
  if (pCard_test && CK_BOOLCALC == pCard_test->eCard()) {
    Card_BoolCalc *pCard_CalcBool = _pCard_BoolCalc(pCard_test);
    vBoolCalc(szRpnExpr, pCard_CalcBool);
  }
  else {
    Card_RpnCalc *pCard_Calc = pCard_Having->pSub_rpnCalc();
    vRpnCalc(szRpnExpr, pCard_Calc);
  }
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for indexJoin op                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnIndexJoin(Stz* szRpnExpr, Card_IndexJoin* pCard_IndexJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_IndexList* pCard_IndexList = pCard_IndexJoin->pSub_indexList();

  Card_Index* pCard_Index =
    _pCard_Index(pCard_IndexList->pFindPpyRecursive(P_ChosenIndex,CK_INDEX));

  Card_Bounds* pCard_bounds = pCard_Index->pSub_bounds();
  if (! pCard_bounds)
    return;

  //Card_Bound* p_bound = (Card_Bound*)pCard_bounds->pGetChild(0);

  Card_Bound* p_bound = nullptr;
  for (Kid xKid(pCard_bounds); xKid.bEach(CK_BOUND); /**/) {

    if (xKid.pChild()->pSymbol()->Y_bHas(P_ChosenBound)) {
      p_bound = _pCard_Bound(xKid.pChild());
      break;
    }
  }

  if (pCard_IndexJoin->pSub_outerJoin()) {
    if (pCard_IndexJoin->pSub_outerJoin()->pSymbol()->bSame("LEFT_OUTER"_Stz)) {
      szRpnExpr->vCatSprint(" %s"_Stz,"LEFT_OUTER"_Stz);
    }
  }

  szRpnExpr->vCatSprint(" %s"_Stz,"BOUND"_Stz);

  i8 nSubs = p_bound->nChildCards();
  for (i8 i_Sub = 0; i_Sub < nSubs; i_Sub++) {
    Card* pSubCard = p_bound->pGet(i_Sub);

    if (! pSubCard->bIsCard(CK_TERM))
      continue;

    Card_Term* p_term = _pCard_Term(pSubCard);

    if (p_term->pSymbol()->bIsStreamOrdinal() ||
        p_term->pSymbol()->bIsColumnOrdinal()) {

      if (!p_term->pSymbol()->Y_bHas(P_Type)) {
        p_term->vDump(__func__);
        pSubCard->vDump(__func__);
        RASSERT(p_term->pSymbol()->Y_bHas(P_Type));
      }
    }

    szRpnExpr->vCatSprint(" %s"_Stz, p_term->szSymbol());
  }

  szRpnExpr->vCatSprint(" %s"_Stz, p_bound->szSymbol());
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for indexJoin op                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnIndexJoin_constraint(Stz* szRpnExpr, Card_IndexJoin* pCard_IndexJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Index* pCard_Index = _pCard_Index(pCard_IndexJoin->pFindPpyRecursive(P_ChosenIndex, CK_INDEX));
  if (! pCard_Index) {
    szRpnExpr->vConcat(" <<no:chosen index>>"_Stz);
    return;
  }

  Card_Constraint *pCard_constraint = pCard_Index->pSub_constraint();
  if (! pCard_constraint) {
    pCard_constraint = pCard_IndexJoin->pSub_constraint();
    if (!pCard_constraint)
      return;
  }

  szRpnExpr->vCatSprint(" type BOOLEAN"_Stz);

  Card* pCard_test = pCard_constraint->pSub_boolCalc();
  if (pCard_test && CK_BOOLCALC == pCard_test->eCard()) {
    Card_BoolCalc *pCard_CalcBool = _pCard_BoolCalc(pCard_test);
    vBoolCalc(szRpnExpr, pCard_CalcBool);
  }
  else {
    Card_RpnCalc *pCard_Calc = pCard_constraint->pSub_rpnCalc();
    vRpnCalc(szRpnExpr, pCard_Calc);
  }

}

/*----------------------------------------------------------------------------*/
/* produce <expr> for indexScan op                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnIndexScan(Stz* szRpnExpr, Card_IndexScan* pCard_IndexScan)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_IndexList* pCard_IndexList = pCard_IndexScan->pSub_indexList();

  Card_Index* pCard_Index =
      _pCard_Index(pCard_IndexList->pFindPpyRecursive(P_ChosenIndex,CK_INDEX));


  Card_Bounds *pCard_bounds = pCard_Index->pSub_bounds();
  if (! pCard_bounds) {
    pCard_bounds = pCard_Index->pSub_bounds();
    if (! pCard_bounds)
      return;
  }

  CardIter xCI(pCard_bounds);
  while (xCI.G_bEachMain(CK_BOUND)) {

    if (xCI.G.p_bound->bIsLBound() || xCI.G.p_bound->bIsUBound()) {

      if (xCI.G.p_bound->bIsLBound())
        szRpnExpr->vCatSprint(" %s"_Stz,"LBOUND"_Stz);
      else
      if (xCI.G.p_bound->bIsUBound())
        szRpnExpr->vCatSprint(" %s"_Stz,"UBOUND"_Stz);
      else
        szRpnExpr->vCatSprint(" %s"_Stz,"BOUND"_Stz);

      i8 nSubs = xCI.G.p_bound->nChildCards();
      for (i8 i_Sub = 0; i_Sub < nSubs; i_Sub++) {
        Card* pSubCard = xCI.G.p_bound->pGet(i_Sub);

        if (! pSubCard->bIsCard(CK_TERM))
          continue;

        Card_Term* p_term = _pCard_Term(pSubCard);

        if (p_term->pSymbol()->bIsStreamOrdinal() ||
            p_term->pSymbol()->bIsColumnOrdinal()) {

          if (!p_term->pSymbol()->Y_bHas(P_Type)) {
            p_term->vDump(__func__);
            pSubCard->vDump(__func__);
            RASSERT(p_term->pSymbol()->Y_bHas(P_Type));
          }
        }

        szRpnExpr->vCatSprint(" %s"_Stz, p_term->szSymbol());
      }

    }

    szRpnExpr->vCatSprint(" %s"_Stz, xCI.G.p_bound->szSymbol());
  }
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for indexJoin op                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnKeyExists(Stz* szRpnExpr, Card_KeyExists* pCard_KeyExists)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Index* pCard_Index = _pCard_Index(pCard_KeyExists->pFindPpyRecursive(P_ChosenIndex,CK_INDEX));
  if (! pCard_Index) {
    szRpnExpr->vConcat(" <<no:chosen index>>"_Stz);
    return;
  }

  Card_Bounds *pCard_bounds = pCard_Index->pSub_bounds();
  if (! pCard_bounds) {
    pCard_bounds = pCard_KeyExists->pSub_bounds();
    if (! pCard_bounds)
      return;
  }

  CardIter xCI(pCard_bounds);
  while (xCI.G_bEachMain(CK_BOUND)) {

    if (! xCI.G.p_bound->bIsEBound())
      continue;

    szRpnExpr->vCatSprint(" %s"_Stz,"BOUND"_Stz);

    i8 nSubs = xCI.G.p_bound->nChildCards();
    for (i8 i_Sub = 0; i_Sub < nSubs; i_Sub++) {
      Card *pSubCard = xCI.G.p_bound->pGet(i_Sub);

      if (!pSubCard->bIsCard(CK_TERM))
        continue;

      Card_Term *p_term = _pCard_Term(pSubCard);

      if (p_term->pSymbol()->bIsStreamOrdinal() ||
          p_term->pSymbol()->bIsColumnOrdinal()) {

        if (!p_term->pSymbol()->Y_bHas(P_Type)) {
          p_term->vDump(__func__);
          pSubCard->vDump(__func__);
          RASSERT(p_term->pSymbol()->Y_bHas(P_Type));
        }
      }

      szRpnExpr->vCatSprint(" %s"_Stz, p_term->szSymbol());
    }

    szRpnExpr->vCatSprint(" %s"_Stz, xCI.G.p_bound->szSymbol());

  }
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for indexJoin op                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnKeyExists_constraint(Stz* szRpnExpr, Card_KeyExists* pCard_KeyExists)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Index* pCard_Index = _pCard_Index(pCard_KeyExists->pFindPpyRecursive(P_ChosenIndex,CK_INDEX));
  if (! pCard_Index) {
    szRpnExpr->vConcat(" <<no:chosen index>>"_Stz);
    return;
  }

  Card_Constraint *pCard_constraint = pCard_Index->pSub_constraint();
  if (! pCard_constraint) {
    pCard_constraint = pCard_KeyExists->pSub_constraint();
    if (!pCard_constraint)
      return;
  }

  szRpnExpr->vCatSprint(" type BOOLEAN"_Stz);

  Card* pCard_test = pCard_constraint->pSub_boolCalc();
  if (pCard_test && CK_BOOLCALC == pCard_test->eCard()) {
    Card_BoolCalc *pCard_CalcBool = _pCard_BoolCalc(pCard_test);
    vBoolCalc(szRpnExpr, pCard_CalcBool);
  }
  else {
    Card_RpnCalc *pCard_Calc = pCard_constraint->pSub_rpnCalc();
    vRpnCalc(szRpnExpr, pCard_Calc);
  }

}

/*----------------------------------------------------------------------------*/
/* produce <expr> for indexJoin op                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnKeyNotExists(Stz* szRpnExpr, Card_KeyNotExists* p_keyNotExists)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Index* pCard_Index = _pCard_Index(p_keyNotExists->pFindPpyRecursive(P_ChosenIndex,CK_INDEX));
  if (! pCard_Index) {
    szRpnExpr->vConcat(" <<no:chosen index>>"_Stz);
    return;
  }

  Card_Bounds *pCard_bounds = pCard_Index->pSub_bounds();
  if (! pCard_bounds) {
    pCard_bounds = p_keyNotExists->pSub_bounds();
    if (! pCard_bounds)
      return;
  }

  CardIter xCI(pCard_bounds);
  while (xCI.G_bEachMain(CK_BOUND)) {

    if (! xCI.G.p_bound->bIsEBound())
      continue;

    szRpnExpr->vCatSprint(" %s"_Stz,"BOUND"_Stz);

    i8 nSubs = xCI.G.p_bound->nChildCards();
    for (i8 i_Sub = 0; i_Sub < nSubs; i_Sub++) {
      Card *pSubCard = xCI.G.p_bound->pGet(i_Sub);

      if (!pSubCard->bIsCard(CK_TERM))
        continue;

      Card_Term *p_term = _pCard_Term(pSubCard);

      if (p_term->pSymbol()->bIsStreamOrdinal() ||
          p_term->pSymbol()->bIsColumnOrdinal()) {

        if (!p_term->pSymbol()->Y_bHas(P_Type)) {
          p_term->vDump(__func__);
          pSubCard->vDump(__func__);
          RASSERT(p_term->pSymbol()->Y_bHas(P_Type));
        }
      }

      szRpnExpr->vCatSprint(" %s"_Stz, p_term->szSymbol());
    }

    szRpnExpr->vCatSprint(" %s"_Stz, xCI.G.p_bound->szSymbol());
  }

}

/*----------------------------------------------------------------------------*/
/* produce <expr> for keyNotExists op                                         */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnKeyNotExists_constraint(Stz* szRpnExpr, Card_KeyNotExists* p_keyNotExists)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Index* pCard_Index = _pCard_Index(p_keyNotExists->pFindPpyRecursive(P_ChosenIndex,CK_INDEX));
  if (! pCard_Index) {
    szRpnExpr->vConcat(" <<no:chosen index>>"_Stz);
    return;
  }

  Card_Constraint *pCard_constraint = pCard_Index->pSub_constraint();
  if (! pCard_constraint) {
    pCard_constraint = p_keyNotExists->pSub_constraint();
    if (!pCard_constraint)
      return;
  }

  szRpnExpr->vCatSprint(" type BOOLEAN"_Stz);

  Card* pCard_test = pCard_constraint->pSub_boolCalc();
  if (pCard_test && CK_BOOLCALC == pCard_test->eCard()) {
    Card_BoolCalc *pCard_CalcBool = _pCard_BoolCalc(pCard_test);
    vBoolCalc(szRpnExpr, pCard_CalcBool);
  }
  else {
    Card_RpnCalc *pCard_Calc = pCard_constraint->pSub_rpnCalc();
    vRpnCalc(szRpnExpr, pCard_Calc);
  }

}

/*----------------------------------------------------------------------------*/
/* produce <expr> for limit op                                                */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnLimit(Stz* szRpnExpr, Card_Limit*  pCard_Limit)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  szRpnExpr->vCatSprint(" %s"_Stz, pCard_Limit->szSymbol());
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for ordagg op                                               */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnOrdAgg(Stz* szRpnExpr, Card_OrdAgg* pCard_OrdAgg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nSubCards;

  Card_Key* pCard_Key =
      _pCard_Key(pCard_OrdAgg->pFindSubCard(CK_KEY));

  if (! pCard_Key)
    return;

  szRpnExpr->vConcat(" ordGroupBy"_Stz);

  nSubCards = pCard_Key->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card* pCard_SUB = pCard_Key->pGet(i_Child);
    if (! pCard_SUB->bIsCard(CK_EXPR))
      return;

    if (i_Child) {
      szRpnExpr->vConcat("\n                 "_Stz);
      szRpnExpr->vConcat(" ,"_Stz);
    }
    else {
      szRpnExpr->vConcat("\n                 "_Stz);
      szRpnExpr->vConcat("  "_Stz);
    }

    Card_Expr* pCard_Expr = _pCard_Expr(pCard_SUB);
    vExpr(szRpnExpr,pCard_Expr);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  Card_Projection* pCard_Projection =
      _pCard_Projection(pCard_OrdAgg->pFindSubCard(CK_PROJECTION));

  if (! pCard_Projection)
    return;

  szRpnExpr->vConcat("\n                 "_Stz);
  szRpnExpr->vConcat(" aggregates"_Stz);

  nSubCards = pCard_Projection->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card* pCard_SUB = pCard_Projection->pGet(i_Child);
    if (! pCard_SUB->bIsCard(CK_EXPR))
      return;

    if (i_Child) {
      szRpnExpr->vConcat("\n                 "_Stz);
      szRpnExpr->vConcat(" ,"_Stz);
    }
    else {
      szRpnExpr->vConcat("\n                 "_Stz);
      szRpnExpr->vConcat("  "_Stz);
    }

    Card_Expr* pCard_Expr = _pCard_Expr(pCard_SUB);
    vExpr(szRpnExpr,pCard_Expr);
  }
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for project op                                              */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnProject(Stz* szRpnExpr, Card_Project* pCard_Project)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Projection* pCard_Projection =
      _pCard_Projection(pCard_Project->pFindSubCard(CK_PROJECTION));

  i8 nSubCards = pCard_Projection->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {

    Card* pSubCard = pCard_Projection->pGet(i_Child);
    eCK eSubCard = pSubCard->eCard();

    if (CK_EXPR != eSubCard)
      continue;

    Card_Expr* pCard_Expr = _pCard_Expr(pSubCard);

    if (i_Child) {
      szRpnExpr->vConcat("\n                 "_Stz);
      szRpnExpr->vConcat(" ,"_Stz);
    }
    else if (nSubCards > 1) {
      //szRpnExpr->vConcat("\n                 "_Stz);
      szRpnExpr->vConcat("  "_Stz);
    }

    vExpr(szRpnExpr,pCard_Expr);
  }

}

/*----------------------------------------------------------------------------*/
/* produce <expr> for scan op                                                 */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnScan(Stz* szRpnExpr, Card_Scan* pCard_Scan)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for sort op                                                 */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnSort(Stz* szRpnExpr, Card_Sort* pCard_Sort)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Key* pCard_Key =
      _pCard_Key(pCard_Sort->pFindSubCard(CK_KEY));

  i8 nSubCards = pCard_Key->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {

    Card* pSubCard = pCard_Key->pGet(i_Child);
    eCK eSubCard = pSubCard->eCard();

    if (CK_EXPR != eSubCard)
      continue;

    Card_Expr* pCard_Expr = _pCard_Expr(pSubCard);

    if (i_Child)
      szRpnExpr->vConcat(" ,"_Stz);

    vExpr(szRpnExpr,pCard_Expr);

    Card* p_direction = pCard_Expr->pSub_direction();

    szRpnExpr->vCatSprint(" %s"_Stz, p_direction->szSymbol());

  }

}

/*----------------------------------------------------------------------------*/
/* produce <expr> for table op                                                */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnTable(Stz* szRpnExpr, Card_Table* pCard_Table)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for top op                                                  */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnTop(Stz* szRpnExpr, Card_Top* pCard_Top)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Key *pCard_key = pCard_Top->pSub_key();
  Card_RpnCalc *pCard_Calc = pCard_key->pSub_rpnCalc();

  eCT eColumnType = Symbol::eTo_eCT_char(pCard_Calc->szP_Type());

  Stz* szColType = aCT[eColumnType];

  szRpnExpr->vCatSprint(" type %s "_Stz,szColType->azChar());

  vRpnCalc(szRpnExpr, pCard_Calc);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for filter op                                               */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnWhere(Stz* szRpnExpr, Card_Where* pCard_Where)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  szRpnExpr->vCatSprint(" type BOOLEAN"_Stz);

  Card* pCard_test = pCard_Where->pSub_boolCalc();
  if (pCard_test && CK_BOOLCALC == pCard_test->eCard()) {
    Card_BoolCalc *pCard_CalcBool = _pCard_BoolCalc(pCard_test);
    vBoolCalc(szRpnExpr, pCard_CalcBool);
  }
  else {
    Card_RpnCalc *pCard_Calc = pCard_Where->pSub_rpnCalc();
    vRpnCalc(szRpnExpr, pCard_Calc);
  }
}


/*----------------------------------------------------------------------------*/
/* append <rpnExpr> for <calcBool>                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vBoolCalc(Stz* szRpnExpr, Card_BoolCalc* pCard_BoolCalc)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (pCard_BoolCalc->bIsBraced())
    szRpnExpr->vConcat(" }"_Stz);

  vBoolPart(szRpnExpr, pCard_BoolCalc, 0);
}

/*----------------------------------------------------------------------------*/
/* append part                                                                */
/*----------------------------------------------------------------------------*/
void Card_OP::vBoolPart(Stz* szRpnExpr, Card* p_part, i8 i)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nSubCards = p_part->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = p_part->pGet(i_Child);

    eCK eSubCard = pSubCard->eCard();

    switch (eSubCard) {
      case CK_ANDPRED:
        vBoolPart(szRpnExpr, _pCard_AndPred(pSubCard), 0);
        szRpnExpr->vCatSprint(" and"_Stz);
        break;
      case CK_CMPPRED:
        vBoolPart(szRpnExpr, _pCard_CmpPred(pSubCard), 0);
        szRpnExpr->vCatSprint(" %s"_Stz, pSubCard->szSymbol());
        break;
      case CK_NOTPRED:
        vBoolPart(szRpnExpr, _pCard_NotPred(pSubCard), 0);
        szRpnExpr->vCatSprint(" not"_Stz);
        break;
      case CK_ORPRED:
        vBoolPart(szRpnExpr, _pCard_OrPred(pSubCard), 0);
        szRpnExpr->vCatSprint(" or"_Stz);
        break;
      case CK_TERM:
        if (pSubCard->pSymbol()->bIsStreamOrdinal() ||
            pSubCard->pSymbol()->bIsColumnOrdinal()) {

          if (!pSubCard->pSymbol()->Y_bHas(P_Type)) {
            //p_part->vDump();
            //pSubCard->vDump();
            RASSERT(pSubCard->pSymbol()->Y_bHas(P_Type));
          }
        }
        szRpnExpr->vCatSprint(" %s"_Stz, pSubCard->szSymbol());
        break;
      default:
        NEVER_REACH
    }
  }

}

/*----------------------------------------------------------------------------*/
/* produce <expr> for pCard_RpnCalc                                          */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnCalc(Stz* szRpnExpr, Card_RpnCalc* pCard_RpnCalc)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (!pCard_RpnCalc->pSymbol()->Y_bHas(P_Type)) {
    pCard_RpnCalc->vDump(__func__);
    RASSERT(pCard_RpnCalc->pSymbol()->Y_bHas(P_Type));
  }

  i8 nSubCards = pCard_RpnCalc->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card* pCard = pCard_RpnCalc->pGet(i_Child);

    eCK eCard = pCard->eCard();

    switch (eCard) {

      case CK_TERM:
        vTerm(szRpnExpr,_pCard_Term(pCard));
        break;

      default:
        //{;}
        //vTrace("  skipping %s\n"_Stz,aCK[eCard]);
        break;
    }

  }

}

/*----------------------------------------------------------------------------*/
/* produce <expr> for pCard_Expr                                              */
/*----------------------------------------------------------------------------*/
void Card_OP::vExpr(Stz* szRpnExpr, Card_Expr* pCard_Expr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_As*      p_as      = nullptr;
  Card_RpnCalc* p_rpnCalc = nullptr;

  i8 nSubCards = pCard_Expr->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pCard = pCard_Expr->pGet(i_Child);

    eCK eCard = pCard->eCard();

    switch (eCard) {
      case CK_AS:
        p_as = _pCard_As(pCard);
        break;

      case CK_RPNCALC:
        p_rpnCalc = _pCard_RpnCalc(pCard);
        break;

      default:
        break;
    }
  }

  szRpnExpr->vCatSprint(" type %s"_Stz,
                        aCT[Symbol::eTo_eCT_char(p_rpnCalc->pSymbol()->Y_szGet(P_Type))]);

  if (p_as)
    vAs(szRpnExpr, p_as);

  if (p_rpnCalc->bNeedsBrace()) {
    szRpnExpr->vConcat(" }"_Stz);
  }
  vRpnCalc(szRpnExpr, p_rpnCalc);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for pCard_As                                                */
/*----------------------------------------------------------------------------*/
void Card_OP::vAs(Stz* szRpnExpr, Card_As* pCard_As)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  szRpnExpr->vCatSprint(" as %s"_Stz, pCard_As->szSymbol());
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for pCard_Term                                              */
/*----------------------------------------------------------------------------*/
void Card_OP::vTerm(Stz* szRpnExpr, Card_Term* pCard_Term)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (pCard_Term->pSymbol()->bIsStreamOrdinal() ||
      pCard_Term->pSymbol()->bIsColumnOrdinal()) {

    if (!pCard_Term->pSymbol()->Y_bHas(P_Type)) {
      pCard_Term->vDump(__func__);
      RASSERT(pCard_Term->pSymbol()->Y_bHas(P_Type));
    }
  }

  szRpnExpr->vCatSprint(" %s"_Stz, pCard_Term->szSymbol());
}

/*----------------------------------------------------------------------------*/
/* craft column map (where to find columns of stream def in index entry       */
/*----------------------------------------------------------------------------*/
ColsReq* Card_OP::pCraftColMap_gbyAgg(StreamDef* pStreamDef_emitted,
                                      IndexDef*  pIndexDef_gbyAgg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! bIsCard(CK_GBYAGG))
    return nullptr;

  ColsReq*  pColMap_gbyAgg = new ColsReq;

  i8 nCols = pStreamDef_emitted->nColumns();

  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    Column* pCol = pStreamDef_emitted->pGetColumn(i_Col);

    /// ????
//    if $K
//      key
//    else
//      payload

    pColMap_gbyAgg->vAddCol(i_Col, false, i_Col);
  }

  pColMap_gbyAgg->vValidate();

  return pColMap_gbyAgg;
}

/*----------------------------------------------------------------------------*/
/* craft emitted streamDef                                                    */
/*----------------------------------------------------------------------------*/
StreamDef* Card_OP::pCraftStreamDef_emitted(ColMap** ppColMap)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Projection* p_projection = pSub_projection();
  if (! p_projection)
    return nullptr;

  Card_Key* p_key = nullptr;
  if (bIsOne(CK_GBYAGG,CK_SORT,CK_TOP,CK_NONE)) {
    *ppColMap = new ColMap;
    p_key = pSub_key();
    RASSERT(p_key);
  }

  ColMap* pColMap = ppColMap? *ppColMap : nullptr;

  StreamDef* pStreamDef_emitted =
      p_projection->pToStreamDef(pColMap, p_key, this);


  if (CK_GBYAGG == eCard()) {
    Card_Mapping* p_mapping = pSub_mapping();
    pColMap = p_mapping->pToColMap(pColMap,p_key,this);
  }
  else
  if (CK_SORT == eCard()) {
    Card_Mapping* p_mapping = pSub_mapping();
    pColMap = p_mapping->pToColMap(pColMap,p_key,this);
  }
  else
  if (CK_TOP == eCard()) {
    Card_Mapping* p_mapping = pSub_mapping();
    pColMap = p_mapping->pToColMap(pColMap,p_key,this);
  }

  if (ppColMap)
    (*ppColMap)->vValidate();

  return pStreamDef_emitted;
}

/*----------------------------------------------------------------------------*/
/* craft index for indexJoin                                                  */
/*----------------------------------------------------------------------------*/
Index* Card_OP::pCraftIndex_indexJoin(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Table* p_table = pSub_table();
  if (! p_table)
    return nullptr;

  Card_IndexList* p_indexList = pSub_indexList();
  if (! p_indexList)
    return nullptr;

  Card_Index* p_index = p_indexList->pFindChosenIndex();
  if (! p_index)
    return nullptr;

  Index* pIndex = p_index->pGetIndex(p_table);

  return pIndex;
}

/*----------------------------------------------------------------------------*/
/* craft index for indexScan                                                  */
/*----------------------------------------------------------------------------*/
Index* Card_OP::pCraftIndex_indexScan(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Table* p_table = pSub_table();
  if (! p_table)
    return nullptr;

  Card_IndexList* p_indexList = pSub_indexList();
  if (! p_indexList)
    return nullptr;

  Card_Index* p_index = p_indexList->pFindChosenIndex();
  if (! p_index)
    return nullptr;

  Index* pIndex = p_index->pGetIndex(p_table);

  return pIndex;
}

/*----------------------------------------------------------------------------*/
/* craft index for indexSemi                                                  */
/*----------------------------------------------------------------------------*/
Index* Card_OP::pCraftIndex_indexSemi(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Table* p_table = pSub_table();
  if (! p_table)
    return nullptr;

  Card_IndexList* p_indexList = pSub_indexList();
  if (! p_indexList)
    return nullptr;

  Card_Index* p_index = p_indexList->pFindChosenIndex();
  if (! p_index)
    return nullptr;

  Index* pIndex = p_index->pGetIndex(p_table);

  return pIndex;
}


/*----------------------------------------------------------------------------*/
/* craft indexDef for gbyAgg                                                  */
/*----------------------------------------------------------------------------*/
IndexDef* Card_OP::pCraftIndexDef_gbyAgg(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Key* p_key = pSub_key();
  if (! p_key)
    return nullptr;

  IndexDef* pIndexDef = p_key->pToIndexDef();

  return pIndexDef;
}

/*----------------------------------------------------------------------------*/
/* craft indexDef for sort                                                    */
/*----------------------------------------------------------------------------*/
IndexDef* Card_OP::pCraftIndexDef_sort(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Key* p_key = pSub_key();
  if (! p_key)
    return nullptr;

  IndexDef* pIndexDef = p_key->pToIndexDef();

  return pIndexDef;
}

/*----------------------------------------------------------------------------*/
/* craft indexDef for top                                                     */
/*----------------------------------------------------------------------------*/
IndexDef* Card_OP::pCraftIndexDef_top(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Key* p_key = pSub_key();
  if (! p_key)
    return nullptr;

  IndexDef* pIndexDef = p_key->pToIndexDef();

  return pIndexDef;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
