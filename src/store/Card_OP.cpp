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
    p_filter = _pCard_Filter(this->pAppendNewCardTail(CK_FILTER));
    p_filter->vAppendNewCardTail(CK_BOOLCALC);
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
    if (pCI->G.p_level && AL_PARTITION != pCI->G.p_level->Y_iGet(P_Level))
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

  if (Y_bHas(P_BreakPt)) {
    vDump();
    vDebugPoint();
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pCI,this](Card* pCard)
  {
    Card_Term* p_term = _pCard_Term(pCard);

    vTrace("CONVERT %s %s (%s %s)\n"_Stz,
           p_term->szValue(),p_term->szBearing(),
           szCardNm(),szBearing());

    if (Y_bHas(P_BreakPt)) {
      vDebugPoint();
    }

    if (! p_term->bIsTblColOdn())
      return C_CONTINUE_RECURSION;

    if (pCI->ProjectionAnalysis.m_pProjection_running) {
      if (Card_Expr* p_expr_col =
          pCI->ProjectionAnalysis.m_pProjection_running->pFind_expr_By_projectedCol(p_term->szValue())) {
        p_term->vSetValueMark1(p_expr_col->szValue(), MARK_CHANGED);
        vTrace("  TO %s\n"_Stz,p_expr_col->szValue());
        return C_CONTINUE_RECURSION;
      }
    }

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    if (! (pCI->G.p_MainCard->bIsCard(CK_KEYEXISTS) || pCI->G.p_MainCard->bIsCard(CK_KEYNOTEXISTS)))
      return C_CONTINUE_RECURSION;

    // column of this table

    Stz* szStmColOdn;
    s32 sColOfThisTable;

    pCI->G.p_projection = pCI->G.p_MainCard->pSub_projection();
    i8 nExpr_prior = pCI->G.p_projection->nChildCards();
    i8 i_Col = p_term->iGetOdnNo();
    sColOfThisTable.vSprint("$%lld"_Stz,nExpr_prior+i_Col);
    szStmColOdn = sColOfThisTable.szStz();
    p_term->vSetValueMark1(szStmColOdn, MARK_CHANGED);
    vTrace("  TO %s\n"_Stz,szStmColOdn);
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

  if (bIsCard(CK_INDEXJOIN)) {
    if ((pCI->G.p_key = pSub_key()))
      pCI->G.p_key->bApplyRecursive(CK_TERM, bLambdaFn);

    if (Card_IndexList* p_indexList_curr = pSub_indexList()) {
      if (Card_Index *p_index_chosen = p_indexList_curr->pFindChosenIndex()) {
        p_index_chosen->bApplyRecursive(CK_TERM, bLambdaFn);
      }
    }

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

  if (Y_bHas(P_BreakPt)) {
    vDump();
    vDebugPoint();
  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vEarly_project_root(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  switch (pCI->G.e_MainCard) {

    // -- -- -- -- -- -- --
    case CK_AGGREG:
      // todo: temp...
      break;

    case CK_GBYAGG:
      pCI->G.p_gbyAgg->vEarly_project_root(pCI);
      break;

    case CK_ORDAGG:
      pCI->G.p_ordAgg->vEarly_project_root(pCI);
      break;

      // -- -- -- -- -- -- --

    case CK_CROSSJOIN:
    case CK_INDEXJOIN:
    case CK_INDEXSCAN:
    case CK_LIMIT:
    case CK_PROJECT:
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
void Card_OP::vEarly_project_backstep(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  RASSERT(pCI->EarlyProject.m_pProjection_needed);

  if (bIsEarlyLeaf(pCI))
    return;

  while (pCI->G_bEachMain_reverse()) {

    switch (pCI->G.e_MainCard) {

      // -- -- -- -- -- -- --
      case CK_GBYAGG:
        // temp: don't go any further
        return;

      case CK_ORDAGG:
        // temp: don't go any further
        return;

      // -- -- -- -- -- -- --
      case CK_PROJECT:
        // recurse into...
        pCI->vSave();
        pCI->G.p_project->vEarly_project_recurse(pCI);
        pCI->vRestore();
        return;

      case CK_INDEXJOIN:
        // recurse into...
        pCI->vSave();
        pCI->G.p_indexJoin->vEarly_project_recurse_add(pCI);
        pCI->vRestore();
        return;

      case CK_INDEXSCAN:
        // recurse into...
        pCI->vSave();
        pCI->G.p_indexScan->vEarly_project_leaf_add(pCI);
        pCI->vRestore();
        return;

      case CK_SCAN:
        // recurse into...
        pCI->vSave();
        pCI->G.p_scan->vEarly_project_leaf_add(pCI);
        pCI->vRestore();
        return;

      // -- -- -- -- -- -- --
      default:
        // repeat while so as to move to previous card
        break;
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

  if (CK_NONE    == eChild_prev ||
      CK_GBYAGG  == eChild_prev ||
      CK_ORDAGG  == eChild_prev ||
      CK_PROJECT == eChild_prev)
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
    if (AL_NODE == p_level->Y_iGet(P_Level))
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
    if (AL_ORIGIN == p_level->Y_iGet(P_Level))
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
    if (AL_PARTITION == p_level->Y_iGet(P_Level))
      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* does op needs projection analysis?                                         */
/*----------------------------------------------------------------------------*/
bool Card_OP::bNeedsProjectionAnalysis(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (bIsCard(CK_TABLE)     ||
      bIsCard(CK_SCAN)      ||
      bIsCard(CK_INDEXSCAN) ||
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
/* table projection analysis                                                  */
/*----------------------------------------------------------------------------*/
void Card_OP::vUpdate_projection_Off_running(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (!bNeedsProjectionAnalysis() )
    return;

  Card_Projection* p_projection;

  if ((p_projection = pSub_projection()))
    vRemoveDeleteChild(p_projection);

  Card_Projection* p_projection_needed;
  if ((p_projection_needed = pSub_projection()))
    vRemoveChild(p_projection_needed);

  if (pSub_keyExists() || pSub_keyNotExists()) {
    p_projection = _pCard_Projection(pCI->ProjectionAnalysis.m_pProjection_running->pDupCardAndSubCards());
    vAppend(p_projection);
    pCI->ProjectionAnalysis.m_pProjection_running = p_projection;
    if (p_projection_needed)
      vAppend(p_projection_needed);
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->G.p_tableNm = pSub_tableNm();

  Table* pTable = Card_Table::pAccess_Table(pCI->G.p_tableNm->sz());
  StreamDef* pStreamDef = pTable->pStreamDef();

  if (pCI->ProjectionAnalysis.m_pProjection_running)
    p_projection = pCI->ProjectionAnalysis.m_pProjection_running->pClone();
  else
    p_projection = new Card_Projection;

  //  --  --  --  --  --  --  --  --  --  --  --  --  --

  char cTblSyn = bIsCard(CK_TABLE) ? cValue() : pSub_table()->cValue();

  i8 i_Expr = p_projection->nExpr() + 1;

  i8 nCols = pStreamDef->nColumns();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    Column *pColumn = pStreamDef->pGetColumn(i_Col);

    Card_Expr* p_expr = new Card_Expr;
    p_expr->vValueSprint("$%lld"_Stz,i_Expr++);
    p_projection->vAppend(p_expr);

    eCT eColumnType = pColumn->eColumnType();
    Stz *szType = aCT[eColumnType];
    Card_Type *pCard_Type = new Card_Type(szType);
    p_expr->vAppend(pCard_Type);

    Card_As *p_as = new Card_As(pColumn->szColNm());
    p_expr->vAppend(p_as);

    Card_RpnCalc *p_rpnCalc = new Card_RpnCalc;
    p_expr->vAppend(p_rpnCalc);
    p_rpnCalc->Y_vSet(P_Type,szCT_char[eColumnType]);

    s32 sTblCol_from_SEmetadata;
    sTblCol_from_SEmetadata.vSprint("$%c.%lld"_Stz, cTblSyn, i_Col + 1);

    Card_Term *p_term = new Card_Term(sTblCol_from_SEmetadata);
    p_rpnCalc->vAppend(p_term);

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    p_term->Y_vSet(P_Column);
    p_term->Y_vSet(P_Type,szCT_char[eColumnType]);
  }

  vAppend(p_projection);

  pCI->ProjectionAnalysis.m_pProjection_running = p_projection;

  if (p_projection_needed)
    vAppend(p_projection_needed);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
