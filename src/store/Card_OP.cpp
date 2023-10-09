/*============================================================================*/
/* Card_OP.cpp -- Abstract OP                                                 */
/*============================================================================*/

#include "Card_OP.hpp"

#include "Card_Level.hpp"
#include "Card_Table.hpp"
#include "CardIter.hpp"
#include "Table.hpp"
#include "Index.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_OP::Card_OP(eCK eCard, i8 iFixedDepth)
: Card_MAIN(eCard, iFixedDepth)
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vAdd_Pred_Under_filter(Card_CONJUNCTION *p_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Filter* p_filter = this->pSub_filter();
  if (! p_filter) {
    p_filter = _pCard_Filter(this->pAppendNewCard(CK_FILTER));
    Card_BoolCalc* p_boolCalc = _pCard_BoolCalc(p_filter->pAppendNewCard(CK_BOOLCALC));
    UNUSED(p_boolCalc);
  }

  p_filter->vAddPred(p_PRED);
}

/*----------------------------------------------------------------------------*/
/* analyse from for injected columns                                          */
/*----------------------------------------------------------------------------*/
void Card_OP::vAnalyse_injected(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  UNUSED(pCI);

  pCI->G.p_injected = Card_Injected::pNew_injected(pCI);

  vAppendChild(pCI->G.p_injected);
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

  //Card_OP* p_OP_leaf_maybe = pCI->G.p_OP;

  if (! pCI->G_bEachMain_reverse_NONE())
    return;

  // which card did we arrive at?
  switch (pCI->G.e_MainCard) {

    // -- -- -- -- -- -- --
    // don't go any further

    case CK_GBYAGG:
      pCI->Early.m_pOP_leaf = pCI->G.p_OP;
      break;

    case CK_ORDAGG:
      pCI->Early.m_pOP_leaf = pCI->G.p_OP;
      break;

    // -- -- -- -- -- -- --
    // recurse into...

    case CK_CROSSJOIN:
      pCI->vSave();
      pCI->G.p_crossJoin->vEarlyProject_recurse_update(pCI);
      pCI->vRestore();
      break;

    case CK_FILTER:
      pCI->vSave();
      pCI->G.p_filter->vEarlyProject_recurse_update(pCI);
      pCI->vRestore();
      break;

    case CK_INDEXJOIN:
      pCI->vSave();
      pCI->G.p_indexJoin->vEarlyProject_recurse_add(pCI);
      pCI->vRestore();
      break;

    case CK_INDEXSCAN:
      pCI->vSave();
      pCI->G.p_indexScan->vEarlyProject_leaf(pCI);
      pCI->vRestore();
      break;

    case CK_KEYEXISTS:
      pCI->vSave();
      pCI->G.p_keyExists->vEarlyProject_recurse_add(pCI);
      pCI->vRestore();
      break;

    case CK_KEYNOTEXISTS:
      pCI->vSave();
      pCI->G.p_keyNotExists->vEarlyProject_recurse_add(pCI);
      pCI->vRestore();
      break;

    case CK_PROJECT:
      if (pCI->G.p_project->pSub_needed()) {
        pCI->vSave();
        pCI->G.p_project->vEarlyProject_recurse_needed(pCI);
        pCI->vRestore();
      }
      else {
        pCI->G.p_project->vEarlyProject_given_leaf(pCI);
      }
      break;

    case CK_SCAN:
      pCI->vSave();
      pCI->G.p_scan->vEarlyProject_leaf(pCI);
      pCI->vRestore();
      break;

    case CK_WHERE:
      pCI->vSave();
      pCI->G.p_where->vEarlyProject_recurse_update(pCI);
      pCI->vRestore();
      break;

    // -- -- -- -- -- -- --
    // missing??

    default:
      pCI->G.p_MainCard->vDump(__func__);
      NEVER_REACH
  }

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vEnsure_stream(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! (pCI->G.p_stream = pSub_stream()) ) {
    pCI->G_vAppendNewChild(this,CK_STREAM);
  }

  RASSERT(pCI->G.p_stream);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_OP::bIsEarlyLeaf(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

//  eCK eCard = this->eCard();

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
/* column mapping                                                             */
/*----------------------------------------------------------------------------*/
void Card_OP::vMapColumns(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_key = pSub_key();
  pCI->G.p_projection = pSub_projection();

  pCI->G_vAppendNewChild(CK_SYNTHESIS, CK_MAPPING);
  pCI->G_vAppendNewChild(pCI->G.p_mapping,CK_KEYCELLS);
  pCI->G_vAppendNewChild(pCI->G.p_mapping, CK_PAYLOAD);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // map the columns of the key

  pCI->G.p_key->vMapColumns(pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // map the columns of the projection

  pCI->G.p_projection->vMapColumns(pCI);
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

  if (bIsCard(CK_FROM)         ||
      bIsCard(CK_KEYEXISTS)    ||
      bIsCard(CK_KEYNOTEXISTS) ||
      bIsCard(CK_INDEXSCAN)    ||
      bIsCard(CK_INDEXJOIN)    ||
//    bIsCard(CK_PROJECT)      ||
      bIsCard(CK_SCAN)         ||
      bIsCard(CK_SORT)         )
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
/* update constraint analysis                                                 */
/*----------------------------------------------------------------------------*/
void Card_OP::vUpdate_constraint(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  UNUSED(pCI);

  Card_Constraint* p_constraint;

  if ((p_constraint = pSub_constraint())) {
    vDebugPoint();
  }
}

/*----------------------------------------------------------------------------*/
/* update projection                                                          */
/*----------------------------------------------------------------------------*/
void Card_OP::vUpdate_projection(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Projection* p_projection;

  if (CK_PROJECT == eCard() && pCI->Prior.m_pProjection) {
    p_projection = pSub_projection();
    p_projection->vEnsureTypes_For_stmColOdn(pCI);
    return;
  }

  if (! bNeedsProjectionAnalysis())
    return;

  vEnsure_stream(pCI);


  if ((p_projection = pSub_projection()))
    pCI->G.p_stream->vDeleteChild(p_projection);


  Card_Projection* p_projection_needed;
  if ((p_projection_needed = pSub_projection()))
    vRemoveChild(p_projection_needed);

  if (pSub_keyExists() || pSub_keyNotExists()) {
    p_projection = _pCard_Projection(pCI->Prior.m_pProjection->pDupCardRecursive());
    pCI->G.p_stream->vAppendChild(p_projection);
    pCI->Prior.m_pProjection = p_projection;
    if (p_projection_needed)
      vAppendChild(p_projection_needed);
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->G.p_tableNm = pCI->G.p_table->pSub_tableNm();

  if (! pCI->G.p_tableNm) {
    vDump(__func__);
    vDebugPoint();
  }

  Table* pTable = Card_Table::pAccess_Table(pCI->G.p_tableNm->szSymbol(SF_NONE));
  StreamDef* pStreamDef = pTable->pStreamDef();

  if (pCI->Prior.m_pProjection) {
//    p_projection = pCI->Prior.m_pProjection->pClone();
    p_projection = pCI->Prior.m_pProjection->pReducedCopy(pCI);
    //p_projection->vDump(__func__ );
  }
  else {
    p_projection = new Card_Projection(pCI->iXform());
  }

  //  --  --  --  --  --  --  --  --  --  --  --  --  --

  // todo: [ray A 145]
  char cTblSyn = bIsCard(CK_TABLE) ? pSymbol()->cSF_char() : pSub_table()->pSymbol()->cSF_char();

  i8 i_Expr = p_projection->nExpr() + 1;

  i8 nCols = pStreamDef->nColumns();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    Column *pColumn = pStreamDef->pGetColumn(i_Col);

    Card_Expr* p_expr = new Card_Expr;
    Symbol xSymbol(SF_COL_NO,i_Expr++);
    p_expr->pSymbol()->vFacets(&xSymbol);
    p_projection->vAppendChild(p_expr);

    eCT eColumnType = pColumn->eColumnType();

    Card_As *p_as = (new Card_As) ->SYM(SF_AS_NM,pColumn->szColNm());
    p_expr->vAppendChild(p_as);

    Card_RpnCalc *p_rpnCalc = new Card_RpnCalc;
    p_expr->vAppendChild(p_rpnCalc);
    p_rpnCalc->pSymbol()->Y_vColType(szCT_char[eColumnType]);
    p_expr->pSymbol()->Y_vColType(szCT_char[eColumnType]);

    Card_Term *p_term = (new Card_Term) -> SYM(SF_COL_TC,cTblSyn, i_Col + 1);
    p_rpnCalc->vAppendChild(p_term);

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    p_term->pSymbol()->Y_vSet(P_Column);
    p_term->pSymbol()->Y_vColType(szCT_char[eColumnType]);
  }

  pCI->G.p_stream->vAppendChild(p_projection);

  pCI->Prior.m_pProjection = p_projection;

  if (p_projection_needed)
    vAppendChild(p_projection_needed);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for aggreg op                                               */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnAggreg(String* psRpnExpr, Card_Aggreg* pCard_Aggreg)
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
      psRpnExpr->vConcat("\n                 "_Stz);
      psRpnExpr->vConcat(" ,"_Stz);
    }
    else {
      psRpnExpr->vConcat("  "_Stz);
    }

    Card_Expr* pCard_Expr = _pCard_Expr(pCard_SUB);
    vExpr(psRpnExpr, pCard_Expr);
  }


}

/*----------------------------------------------------------------------------*/
/* expr for combine                                                           */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnCombined(String* psRpnExpr, Card_Combined* pCard_Combined)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  psRpnExpr->vCatSprint("%s"_Stz, pCard_Combined->szSymbol(SF_NONE));
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for crossJoin op                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnCrossJoin(String* psRpnExpr, Card_CrossJoin* pCard_CrossJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Bounds* pCard_bounds = pCard_CrossJoin->pSub_bounds();
  RASSERT(pCard_bounds);
  Card_Bound* pCard_Bound = pCard_bounds->pSub_bound();
  RASSERT(pCard_Bound);

  psRpnExpr->vCatSprint(" %s"_Stz, "BOUND"_Stz);

  i8 nSubs = pCard_Bound->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubs; i_Child++) {
    Card* pChildCard = pCard_Bound->pGet(i_Child);

    if (! pChildCard->bIsCard(CK_TERM))
      continue;

    Card_Term* p_term = _pCard_Term(pChildCard);

    if (p_term->pSymbol()->bIs_Col_No() ||
        p_term->pSymbol()->bIs_Col_Tc()) {

      if (!p_term->pSymbol()->Y_bHas(P_Type)) {
        p_term->vDump(__func__);
        pChildCard->vDump(__func__);
        RASSERT(p_term->pSymbol()->Y_bHas(P_Type));
      }
    }

    psRpnExpr->vCatSprint(" %s"_Stz, p_term->szSymbol(SF_NONE));
  }
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for filter op                                               */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnFilter(String* psRpnExpr, Card_Filter* pCard_Filter)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  psRpnExpr->vCatSprint(" type BOOLEAN"_Stz);

  Card* pCard_test = pCard_Filter->pSub_boolCalc();
  if (CK_BOOLCALC == pCard_test->eCard()) {
    Card_BoolCalc *pCard_CalcBool = _pCard_BoolCalc(pCard_test);
    vBoolCalc(psRpnExpr, pCard_CalcBool);
  }
  else {
    Card_RpnCalc *pCard_Calc = pCard_Filter->pSub_rpnCalc();
    vRpnCalc(psRpnExpr, pCard_Calc);
  }
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for aggGby op                                               */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnGbyAgg(String* psRpnExpr, Card_GbyAgg* pCard_GbyAgg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nSubCards;

  Card_Key* pCard_Key =
      _pCard_Key(pCard_GbyAgg->pFindSubCard(CK_KEY));

  if (! pCard_Key)
    return;

  psRpnExpr->vConcat(" groupBy"_Stz);

  nSubCards = pCard_Key->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card* pCard_SUB = pCard_Key->pGet(i_Child);
    if (! pCard_SUB->bIsCard(CK_EXPR))
      return;

    if (i_Child) {
      psRpnExpr->vConcat("\n                 "_Stz);
      psRpnExpr->vConcat(" ,"_Stz);
    }
    else {
      psRpnExpr->vConcat("\n                 "_Stz);
      psRpnExpr->vConcat("  "_Stz);
    }

    Card_Expr* pCard_Expr = _pCard_Expr(pCard_SUB);
    vExpr(psRpnExpr, pCard_Expr);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  Card_Projection* pCard_Projection =
      _pCard_Projection(pCard_GbyAgg->pFindSubCard(CK_PROJECTION));

  if (! pCard_Projection)
    return;

  psRpnExpr->vConcat("\n                 "_Stz);
  psRpnExpr->vConcat(" aggregates"_Stz);

  nSubCards = pCard_Projection->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card* pCard_SUB = pCard_Projection->pGet(i_Child);
    if (! pCard_SUB->bIsCard(CK_EXPR))
      break;

    if (i_Child) {
      psRpnExpr->vConcat("\n                 "_Stz);
      psRpnExpr->vConcat(" ,"_Stz);
    }
    else {
      psRpnExpr->vConcat("\n                 "_Stz);
      psRpnExpr->vConcat("  "_Stz);
    }

    Card_Expr* pCard_Expr = _pCard_Expr(pCard_SUB);
    vExpr(psRpnExpr, pCard_Expr);
  }

//  vTrace("%s\n"_Stz,szRpnExpr);
//  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* weave : hash  <expr>                                                       */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnHash(String* psOpArgs, Card_Hash* pCard_Hash)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

//  Card_HshCol* p_hshCol = pCard_Hash->pSub_hshCol();
  Card_Term* p_term = pCard_Hash->pSub_term();

  psOpArgs->vCatSprint("type %s %s"_Stz,
                       aCT[Symbol::eTo_eCT_char(p_term->pSymbol()->Y_szColType())],
                       p_term->szSymbol(SF_NONE));
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for having op                                               */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnHaving(String* psRpnExpr, Card_Having* pCard_Having)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  psRpnExpr->vCatSprint(" type BOOLEAN"_Stz);

  Card* pCard_test = pCard_Having->pSub_boolCalc();
  if (pCard_test && CK_BOOLCALC == pCard_test->eCard()) {
    Card_BoolCalc *pCard_CalcBool = _pCard_BoolCalc(pCard_test);
    vBoolCalc(psRpnExpr, pCard_CalcBool);
  }
  else {
    Card_RpnCalc *pCard_Calc = pCard_Having->pSub_rpnCalc();
    vRpnCalc(psRpnExpr, pCard_Calc);
  }
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for indexJoin op                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnIndexJoin(String* psRpnExpr, Card_IndexJoin* pCard_IndexJoin)
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
      psRpnExpr->vCatSprint(" %s"_Stz, "LEFT_OUTER"_Stz);
    }
  }

  psRpnExpr->vCatSprint(" %s"_Stz, "BOUND"_Stz);

  i8 nSubs = p_bound->nChildCards();
  for (i8 i_Sub = 0; i_Sub < nSubs; i_Sub++) {
    Card* pSubCard = p_bound->pGet(i_Sub);

    if (! pSubCard->bIsCard(CK_TERM))
      continue;

    Card_Term* p_term = _pCard_Term(pSubCard);

    if (p_term->pSymbol()->bIs_Col_No() ||
        p_term->pSymbol()->bIs_Col_Tc()) {

      if (!p_term->pSymbol()->Y_bHas(P_Type)) {
        p_term->vDump(__func__);
        pSubCard->vDump(__func__);
        RASSERT(p_term->pSymbol()->Y_bHas(P_Type));
      }
    }

    psRpnExpr->vCatSprint(" %s"_Stz, p_term->szSymbol(SF_NONE));
  }

  psRpnExpr->vCatSprint(" %s"_Stz, p_bound->szSymbol(SF_NONE));
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for indexJoin op                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnIndexJoin_constraint(String* psRpnExpr, Card_IndexJoin* pCard_IndexJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Index* pCard_Index = _pCard_Index(pCard_IndexJoin->pFindPpyRecursive(P_ChosenIndex, CK_INDEX));
  if (! pCard_Index) {
    psRpnExpr->vConcat(" <<no:chosen index>>"_Stz);
    return;
  }

  Card_Constraint *pCard_constraint = pCard_Index->pSub_constraint();
  if (! pCard_constraint) {
    pCard_constraint = pCard_IndexJoin->pSub_constraint();
    if (!pCard_constraint)
      return;
  }

  psRpnExpr->vCatSprint(" type BOOLEAN"_Stz);

  Card* pCard_test = pCard_constraint->pSub_boolCalc();
  if (pCard_test && CK_BOOLCALC == pCard_test->eCard()) {
    Card_BoolCalc *pCard_CalcBool = _pCard_BoolCalc(pCard_test);
    vBoolCalc(psRpnExpr, pCard_CalcBool);
  }
  else {
    Card_RpnCalc *pCard_Calc = pCard_constraint->pSub_rpnCalc();
    vRpnCalc(psRpnExpr, pCard_Calc);
  }

}

/*----------------------------------------------------------------------------*/
/* produce <expr> for indexScan op                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnIndexScan(String* psRpnExpr, Card_IndexScan* pCard_IndexScan)
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

  Kid xKid(pCard_bounds);
  while (xKid.bEach(CK_BOUND)) {

    Card_Bound* p_bound = xKid.p_bound();

    if (p_bound->bIsLBound() || p_bound->bIsUBound()) {

      if (p_bound->bIsLBound())
        psRpnExpr->vCatSprint(" %s"_Stz, "LBOUND"_Stz);
      else
      if (p_bound->bIsUBound())
        psRpnExpr->vCatSprint(" %s"_Stz, "UBOUND"_Stz);
      else
        psRpnExpr->vCatSprint(" %s"_Stz, "BOUND"_Stz);

      i8 nSubs = p_bound->nChildCards();
      for (i8 i_Sub = 0; i_Sub < nSubs; i_Sub++) {
        Card* pSubCard = p_bound->pGet(i_Sub);

        if (! pSubCard->bIsCard(CK_TERM))
          continue;

        Card_Term* p_term = _pCard_Term(pSubCard);

        if (p_term->pSymbol()->bIs_Col_No() ||
            p_term->pSymbol()->bIs_Col_Tc()) {

          if (!p_term->pSymbol()->Y_bHas(P_Type)) {
            p_term->vDump(__func__);
            pSubCard->vDump(__func__);
            RASSERT(p_term->pSymbol()->Y_bHas(P_Type));
          }
        }

        psRpnExpr->vCatSprint(" %s"_Stz, p_term->szSymbol(SF_NONE));
      }

    }

    psRpnExpr->vCatSprint(" %s"_Stz, p_bound->szSymbol(SF_NONE));
  }
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for inflate op                                              */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnInflate(String* psRpnExpr, Card_Inflate* pCard_Inflate)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(psRpnExpr);
  UNUSED(pCard_Inflate);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for indexJoin op                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnKeyExists(String* psRpnExpr, Card_KeyExists* pCard_KeyExists)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Index* pCard_Index = _pCard_Index(pCard_KeyExists->pFindPpyRecursive(P_ChosenIndex,CK_INDEX));
  if (! pCard_Index) {
    psRpnExpr->vConcat(" <<no:chosen index>>"_Stz);
    return;
  }

  Card_Bounds *pCard_bounds = pCard_Index->pSub_bounds();
  if (! pCard_bounds) {
    pCard_bounds = pCard_KeyExists->pSub_bounds();
    if (! pCard_bounds)
      return;
  }

  Kid xKid(pCard_bounds);
  while (xKid.bEach(CK_BOUND)) {

    Card_Bound* p_bound = xKid.p_bound();

    if (! p_bound->bIsEBound())
      continue;

    psRpnExpr->vCatSprint(" %s"_Stz, "BOUND"_Stz);

    i8 nSubs = p_bound->nChildCards();
    for (i8 i_Sub = 0; i_Sub < nSubs; i_Sub++) {
      Card *pSubCard = p_bound->pGet(i_Sub);

      if (!pSubCard->bIsCard(CK_TERM))
        continue;

      Card_Term *p_term = _pCard_Term(pSubCard);

      if (p_term->pSymbol()->bIs_Col_No() ||
          p_term->pSymbol()->bIs_Col_Tc()) {

        if (!p_term->pSymbol()->Y_bHas(P_Type)) {
          p_term->vDump(__func__);
          pSubCard->vDump(__func__);
          RASSERT(p_term->pSymbol()->Y_bHas(P_Type));
        }
      }

      psRpnExpr->vCatSprint(" %s"_Stz, p_term->szSymbol(SF_NONE));
    }

    psRpnExpr->vCatSprint(" %s"_Stz, p_bound->szSymbol(SF_NONE));

  }
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for indexJoin op                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnKeyExists_constraint(String* psRpnExpr, Card_KeyExists* pCard_KeyExists)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Index* pCard_Index = _pCard_Index(pCard_KeyExists->pFindPpyRecursive(P_ChosenIndex,CK_INDEX));
  if (! pCard_Index) {
    psRpnExpr->vConcat(" <<no:chosen index>>"_Stz);
    return;
  }

  Card_Constraint *pCard_constraint = pCard_Index->pSub_constraint();
  if (! pCard_constraint) {
    pCard_constraint = pCard_KeyExists->pSub_constraint();
    if (!pCard_constraint)
      return;
  }

  psRpnExpr->vCatSprint(" type BOOLEAN"_Stz);

  Card* pCard_test = pCard_constraint->pSub_boolCalc();
  if (pCard_test && CK_BOOLCALC == pCard_test->eCard()) {
    Card_BoolCalc *pCard_CalcBool = _pCard_BoolCalc(pCard_test);
    vBoolCalc(psRpnExpr, pCard_CalcBool);
  }
  else {
    Card_RpnCalc *pCard_Calc = pCard_constraint->pSub_rpnCalc();
    vRpnCalc(psRpnExpr, pCard_Calc);
  }

}

/*----------------------------------------------------------------------------*/
/* produce <expr> for indexJoin op                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnKeyNotExists(String* psRpnExpr, Card_KeyNotExists* p_keyNotExists)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Index* pCard_Index = _pCard_Index(p_keyNotExists->pFindPpyRecursive(P_ChosenIndex,CK_INDEX));
  if (! pCard_Index) {
    psRpnExpr->vConcat(" <<no:chosen index>>"_Stz);
    return;
  }

  Card_Bounds *pCard_bounds = pCard_Index->pSub_bounds();
  if (! pCard_bounds) {
    pCard_bounds = p_keyNotExists->pSub_bounds();
    if (! pCard_bounds)
      return;
  }

  Kid xKid(pCard_bounds);
  while (xKid.bEach(CK_BOUND)) {

    Card_Bound* p_bound = xKid.p_bound();

    if (! p_bound->bIsEBound())
      continue;

    psRpnExpr->vCatSprint(" %s"_Stz, "BOUND"_Stz);

    i8 nSubs = p_bound->nChildCards();
    for (i8 i_Sub = 0; i_Sub < nSubs; i_Sub++) {
      Card *pSubCard = p_bound->pGet(i_Sub);

      if (!pSubCard->bIsCard(CK_TERM))
        continue;

      Card_Term *p_term = _pCard_Term(pSubCard);

      if (p_term->pSymbol()->bIs_Col_No() ||
          p_term->pSymbol()->bIs_Col_Tc()) {

        if (!p_term->pSymbol()->Y_bHas(P_Type)) {
          p_term->vDump(__func__);
          pSubCard->vDump(__func__);
          RASSERT(p_term->pSymbol()->Y_bHas(P_Type));
        }
      }

      psRpnExpr->vCatSprint(" %s"_Stz, p_term->szSymbol(SF_NONE));
    }

    psRpnExpr->vCatSprint(" %s"_Stz, p_bound->szSymbol(SF_NONE));
  }

}

/*----------------------------------------------------------------------------*/
/* produce <expr> for keyNotExists op                                         */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnKeyNotExists_constraint(String* psRpnExpr, Card_KeyNotExists* p_keyNotExists)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Index* pCard_Index = _pCard_Index(p_keyNotExists->pFindPpyRecursive(P_ChosenIndex,CK_INDEX));
  if (! pCard_Index) {
    psRpnExpr->vConcat(" <<no:chosen index>>"_Stz);
    return;
  }

  Card_Constraint *pCard_constraint = pCard_Index->pSub_constraint();
  if (! pCard_constraint) {
    pCard_constraint = p_keyNotExists->pSub_constraint();
    if (!pCard_constraint)
      return;
  }

  psRpnExpr->vCatSprint(" type BOOLEAN"_Stz);

  Card* pCard_test = pCard_constraint->pSub_boolCalc();
  if (pCard_test && CK_BOOLCALC == pCard_test->eCard()) {
    Card_BoolCalc *pCard_CalcBool = _pCard_BoolCalc(pCard_test);
    vBoolCalc(psRpnExpr, pCard_CalcBool);
  }
  else {
    Card_RpnCalc *pCard_Calc = pCard_constraint->pSub_rpnCalc();
    vRpnCalc(psRpnExpr, pCard_Calc);
  }

}

/*----------------------------------------------------------------------------*/
/* produce <expr> for limit op                                                */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnLimit(String* psRpnExpr, Card_Limit*  pCard_Limit)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  psRpnExpr->vCatSprint(" %s"_Stz, pCard_Limit->szSymbol(SF_NONE));
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for ordagg op                                               */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnOrdAgg(String* psRpnExpr, Card_OrdAgg* pCard_OrdAgg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nSubCards;

  Card_Key* pCard_Key =
      _pCard_Key(pCard_OrdAgg->pFindSubCard(CK_KEY));

  if (! pCard_Key)
    return;

  psRpnExpr->vConcat(" ordGroupBy"_Stz);

  nSubCards = pCard_Key->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card* pCard_SUB = pCard_Key->pGet(i_Child);
    if (! pCard_SUB->bIsCard(CK_EXPR))
      return;

    if (i_Child) {
      psRpnExpr->vConcat("\n                 "_Stz);
      psRpnExpr->vConcat(" ,"_Stz);
    }
    else {
      psRpnExpr->vConcat("\n                 "_Stz);
      psRpnExpr->vConcat("  "_Stz);
    }

    Card_Expr* pCard_Expr = _pCard_Expr(pCard_SUB);
    vExpr(psRpnExpr, pCard_Expr);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  Card_Projection* pCard_Projection =
      _pCard_Projection(pCard_OrdAgg->pFindSubCard(CK_PROJECTION));

  if (! pCard_Projection)
    return;

  psRpnExpr->vConcat("\n                 "_Stz);
  psRpnExpr->vConcat(" aggregates"_Stz);

  nSubCards = pCard_Projection->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card* pCard_SUB = pCard_Projection->pGet(i_Child);
    if (! pCard_SUB->bIsCard(CK_EXPR))
      return;

    if (i_Child) {
      psRpnExpr->vConcat("\n                 "_Stz);
      psRpnExpr->vConcat(" ,"_Stz);
    }
    else {
      psRpnExpr->vConcat("\n                 "_Stz);
      psRpnExpr->vConcat("  "_Stz);
    }

    Card_Expr* pCard_Expr = _pCard_Expr(pCard_SUB);
    vExpr(psRpnExpr, pCard_Expr);
  }
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for project op                                              */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnProject(String* psRpnExpr, Card_Project* pCard_Project)
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
      psRpnExpr->vConcat("\n                 "_Stz);
      psRpnExpr->vConcat(" ,"_Stz);
    }
    else if (nSubCards > 1) {
      //szRpnExpr->vConcat("\n                 "_Stz);
      psRpnExpr->vConcat("  "_Stz);
    }

    vExpr(psRpnExpr, pCard_Expr);
  }

}

/*----------------------------------------------------------------------------*/
/* produce <expr> for scan op                                                 */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnScan(String* psRpnExpr, Card_Scan* pCard_Scan)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(psRpnExpr);
  UNUSED(pCard_Scan);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for sort op                                                 */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnSort(String* psRpnExpr, Card_Sort* pCard_Sort)
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
      psRpnExpr->vConcat(" ,"_Stz);

    vExpr(psRpnExpr, pCard_Expr);

    Card* p_direction = pCard_Expr->pSub_direction();

    psRpnExpr->vCatSprint(" %s"_Stz, p_direction->szSymbol(SF_NONE));

  }

}

/*----------------------------------------------------------------------------*/
/* produce <expr> for top op                                                  */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnTop(String* psRpnExpr, Card_Top* pCard_Top)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Key *pCard_key = pCard_Top->pSub_key();
  Card_RpnCalc *pCard_Calc = pCard_key->pSub_rpnCalc();

  eCT eColumnType = Symbol::eTo_eCT_char(pCard_Calc->szP_Type());

  Stz* szColType = aCT[eColumnType];

  psRpnExpr->vCatSprint(" type %s "_Stz, szColType->azChar());

  vRpnCalc(psRpnExpr, pCard_Calc);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for filter op                                               */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnWhere(String* psRpnExpr, Card_Where* pCard_Where)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  psRpnExpr->vCatSprint(" type BOOLEAN"_Stz);

  Card* pCard_test = pCard_Where->pSub_boolCalc();
  if (pCard_test && CK_BOOLCALC == pCard_test->eCard()) {
    Card_BoolCalc *pCard_CalcBool = _pCard_BoolCalc(pCard_test);
    vBoolCalc(psRpnExpr, pCard_CalcBool);
  }
  else {
    Card_RpnCalc *pCard_Calc = pCard_Where->pSub_rpnCalc();
    vRpnCalc(psRpnExpr, pCard_Calc);
  }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* produce <expr> for pCard_As                                                */
/*----------------------------------------------------------------------------*/
void Card_OP::vAs(String* psRpnExpr, Card_As* pCard_As)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  psRpnExpr->vCatSprint(" as %s"_Stz, pCard_As->szSymbol(SF_NONE));
}

/*----------------------------------------------------------------------------*/
/* append <rpnExpr> for <calcBool>                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vBoolCalc(String* psRpnExpr, Card_BoolCalc* pCard_BoolCalc)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (pCard_BoolCalc->bIsBraced())
    psRpnExpr->vConcat(" }"_Stz);

  vBoolPart(psRpnExpr, pCard_BoolCalc, 0);
}

/*----------------------------------------------------------------------------*/
/* append part                                                                */
/*----------------------------------------------------------------------------*/
void Card_OP::vBoolPart(String* psRpnExpr, Card* p_part, i8 i)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(i);

  i8 nSubCards = p_part->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = p_part->pGet(i_Child);

    eCK eSubCard = pSubCard->eCard();

    switch (eSubCard) {
      case CK_ANDCONJ:
        vBoolPart(psRpnExpr, _pCard_AndConj(pSubCard), 0);
        psRpnExpr->vCatSprint(" and"_Stz);
        break;
      case CK_CMPCONJ:
        vBoolPart(psRpnExpr, _pCard_CmpConj(pSubCard), 0);
        psRpnExpr->vCatSprint(" %s"_Stz, pSubCard->szSymbol(SF_NONE));
        break;
      case CK_NOTCONJ:
        vBoolPart(psRpnExpr, _pCard_NotConj(pSubCard), 0);
        psRpnExpr->vCatSprint(" not"_Stz);
        break;
      case CK_ORCONJ:
        vBoolPart(psRpnExpr, _pCard_OrConj(pSubCard), 0);
        psRpnExpr->vCatSprint(" or"_Stz);
        break;
      case CK_TERM:
        if (pSubCard->pSymbol()->bIs_Col_No() ||
            pSubCard->pSymbol()->bIs_Col_Tc()) {

          if (!pSubCard->pSymbol()->Y_bHas(P_Type)) {
            //p_part->vDump();
            //pSubCard->vDump();
            RASSERT(pSubCard->pSymbol()->Y_bHas(P_Type));
          }
        }
        psRpnExpr->vCatSprint(" %s"_Stz, pSubCard->szSymbol(SF_NONE));
        break;
      default:
        NEVER_REACH
    }
  }

}

/*----------------------------------------------------------------------------*/
/* produce <expr> for pCard_Expr                                              */
/*----------------------------------------------------------------------------*/
void Card_OP::vExpr(String* psRpnExpr, Card_Expr* pCard_Expr)
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

  psRpnExpr->vCatSprint(" type %s"_Stz,
                        aCT[Symbol::eTo_eCT_char(p_rpnCalc->pSymbol()->Y_szColType())]);

  if (p_as)
    vAs(psRpnExpr, p_as);

  if (p_rpnCalc->bNeedsBrace()) {
    psRpnExpr->vConcat(" }"_Stz);
  }
  vRpnCalc(psRpnExpr, p_rpnCalc);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for pCard_RpnCalc                                          */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpnCalc(String* psRpnExpr, Card_RpnCalc* pCard_RpnCalc)
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
        vTerm(psRpnExpr, _pCard_Term(pCard));
        break;

      default:
        //{;}
        //vTrace("  skipping %s\n"_Stz,aCK[eCard]);
        break;
    }

  }

}

/*----------------------------------------------------------------------------*/
/* produce <expr> for pCard_Term                                              */
/*----------------------------------------------------------------------------*/
void Card_OP::vTerm(String* psRpnExpr, Card_Term* pCard_Term)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (pCard_Term->pSymbol()->bIs_Col_No() ||
      pCard_Term->pSymbol()->bIs_Col_Tc()) {

    if (!pCard_Term->pSymbol()->Y_bHas(P_Type)) {
      pCard_Term->vDump(__func__);
      RASSERT(pCard_Term->pSymbol()->Y_bHas(P_Type));
    }
  }

  psRpnExpr->vCatSprint(" %s"_Stz, pCard_Term->szSymbol(SF_NONE));
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* craft index for indexJoin                                                  */
/*----------------------------------------------------------------------------*/
Index* Card_OP::pGetIndex_chosenIndex(void)
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

  Index* pIndex = p_index->pGetIndex(p_table->pSub_tableNm());

  return pIndex;
}

/*----------------------------------------------------------------------------*/
/* craft indexDef for key                                                     */
/*----------------------------------------------------------------------------*/
IndexDef* Card_OP::pCraftIndexDef_key(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Key* p_key = pSub_key();
  if (! p_key)
    return nullptr;

  IndexDef* pIndexDef = p_key->pToIndexDef();

  return pIndexDef;
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
  else {
    vDebugPoint();
  }

  ColMap* pColMap = ppColMap? *ppColMap : nullptr;

  StreamDef* pStreamDef_emitted =
      p_projection->pToStreamDef(pColMap, p_key, this);


  if (CK_GBYAGG == eCard()) {
    Card_Mapping* p_mapping = pSub_mapping();
    p_mapping->vToColMap(pColMap);
  }
  else
  if (CK_SORT == eCard()) {
    Card_Mapping* p_mapping = pSub_mapping();
    p_mapping->vToColMap(pColMap);
  }
  else
  if (CK_TOP == eCard()) {
    Card_Mapping* p_mapping = pSub_mapping();
    p_mapping->vToColMap(pColMap);
  }

  if (pColMap)
    pColMap->vValidate();

  return pStreamDef_emitted;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);
  UNUSED(iDepth);
  UNUSED(pParent);

  Card_MAIN::vValidate(pCI, iDepth, pParent);

  if (eCard() == CK_INFLATE)
    return;

  if (pCI->pBoard()->pState()->bHasName(aOH[OH_PROLOG]))
    vValidate_mustHave(CK_LEVEL,CK_STREAM,CK_NONE);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
