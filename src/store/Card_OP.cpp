/*============================================================================*/
/* Card_OP.cpp -- Abstract OP                                                 */
/*============================================================================*/

#include "Card_OP.hpp"

#include "Card_Level.hpp"
#include "Card_Table.hpp"
#include "CardIter.hpp"
#include "Table.hpp"
#include "Index.hpp"
//#include "Expr.hpp"

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

    case CK_o_AGGREG:
    case CK_o_CROSSJOIN:
    case CK_o_FILTER:
    case CK_o_GBYAGG:
    case CK_o_HAVING:
    case CK_o_INDEXJOIN:
    case CK_o_INDEXSCAN:
    case CK_o_KEYEXISTS:
    case CK_o_KEYNOTEXISTS:
    case CK_o_LIMIT:
    case CK_o_ORDAGG:
    case CK_o_PROJECT:
    case CK_o_WHERE:
    case CK_o_SCAN:
    case CK_o_SORT:
    case CK_o_TOP:
      return true;

    default:
      return false;
  }

}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* analyse from for injected columns                                          */
/*----------------------------------------------------------------------------*/
void Card_OP::vAnalyse_injectable(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  UNUSED(pCI);

  pCI->G.p_injectable = Card_Injectable::pNew_injectable(pCI);

  vAppendChild(pCI->G.p_injectable);
}

/*----------------------------------------------------------------------------*/
/* convert given lambda function to all terms                                 */
/*----------------------------------------------------------------------------*/
void Card_OP::vApply_lambdaFn_ToAll_term(CardIter* pCI,
                                         std::function<bool(Card* pCard)> bfLambdaFn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pCI->G.p_OP->bLambdaWalk(CK_TERM, bfLambdaFn);
}

/*----------------------------------------------------------------------------*/
/* correct stmOd                                                              */
/*----------------------------------------------------------------------------*/
void Card_OP::vUpdate_expCol(Card_Emit* p_arriving)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! p_arriving)
    return;

  if (Card_Filter* p_filter = pSub_filter())
    p_filter->vUpdate_expCol(p_arriving);

  if (Card_Emit* p_emit = pSub_emit())
    p_emit->vUpdate_expCol(p_arriving);
}

/*----------------------------------------------------------------------------*/
/* remove and delete filter if it's empty                                     */
/*----------------------------------------------------------------------------*/
void Card_OP::vCullIfEmpty_filter(Card_Filter* p_filter)
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
    case CK_o_AGGREG:
      pCI->G.p_aggreg->vEarlyProject_root(pCI);
      break;

    case CK_o_GBYAGG:
      pCI->G.p_gbyAgg->vEarlyProject_root(pCI);
      break;

    case CK_o_ORDAGG:
      pCI->G.p_ordAgg->vEarlyProject_root(pCI);
      break;

    // -- -- -- -- -- -- --
    case CK_o_PROJECT:
      pCI->G.p_project->vEarlyProject_root(pCI);
      break;

    case CK_o_INDEXJOIN:
      pCI->G.p_indexJoin->vEarlyProject_root(pCI);
      break;

    // -- -- -- -- -- -- --
    case CK_o_CROSSJOIN:
    case CK_o_INDEXSCAN:
    case CK_o_LIMIT:
    case CK_o_SCAN:
    case CK_o_TOP:
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

    case CK_o_GBYAGG:
      pCI->Early.m_pOP_leaf = pCI->G.p_OP;
      break;

    case CK_o_ORDAGG:
      pCI->Early.m_pOP_leaf = pCI->G.p_OP;
      break;

    // -- -- -- -- -- -- --
    // recurse into...

    case CK_o_CROSSJOIN:
      pCI->vSave();
      pCI->G.p_crossJoin->vEarlyProject_recurse_update(pCI);
      pCI->vRestore();
      break;

    case CK_o_FILTER:
      pCI->vSave();
      pCI->G.p_filter->vEarlyProject_recurse_update(pCI);
      pCI->vRestore();
      break;

    case CK_o_INDEXJOIN:
      pCI->vSave();
      pCI->G.p_indexJoin->vEarlyProject_recurse_add(pCI);
      pCI->vRestore();
      break;

    case CK_o_INDEXSCAN:
      pCI->vSave();
      pCI->G.p_indexScan->vEarlyProject_leaf(pCI);
      pCI->vRestore();
      break;

    case CK_o_KEYEXISTS:
      pCI->vSave();
      pCI->G.p_keyExists->vEarlyProject_recurse_add(pCI);
      pCI->vRestore();
      break;

    case CK_o_KEYNOTEXISTS:
      pCI->vSave();
      pCI->G.p_keyNotExists->vEarlyProject_recurse_add(pCI);
      pCI->vRestore();
      break;

    case CK_o_PROJECT:
      if (pCI->G.p_project->pSub_needed()) {
        pCI->vSave();
        pCI->G.p_project->vEarlyProject_recurse_needed(pCI);
        pCI->vRestore();
      }
      else {
        pCI->G.p_project->vEarlyProject_given_leaf(pCI);
      }
      break;

    case CK_o_SCAN:
      pCI->vSave();
      pCI->G.p_scan->vEarlyProject_leaf(pCI);
      pCI->vRestore();
      break;

    case CK_o_WHERE:
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
Card_Filter* Card_OP::pEnsure_filter(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Filter* p_filter = this->pSub_filter();
  if (p_filter)
    return p_filter;

  p_filter = _pCard_Filter(this->pAppendNewCard(CK_o_FILTER));
  Card_BoolPred* p_boolPred = _pCard_BoolPred(p_filter->pAppendNewCard(CK_BOOLPRED));
  UNUSED(p_boolPred);

  return p_filter;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vEnsure_stream(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! (pCI->G.p_stream = pSub_stream()) ) {
    pCI->G_vAppendNewChild(this, CK_s_STREAM);
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
      CK_o_CROSSJOIN == eChild_prev ||
      CK_o_GBYAGG == eChild_prev ||
      CK_o_ORDAGG == eChild_prev ||
      (CK_o_PROJECT == eChild_prev && ! pChild_prev->pSub_needed()))
    return true;

  return false;
//  switch (eCard) {
//
//    case CK_o_INDEXJOIN:
//      if (CK_o_INDEXSCAN == eChild_prev || CK_o_SCAN == eChild_prev)
//        return true;
//      return false;
//
//    case CK_o_INDEXSCAN:
//    case CK_o_SCAN:
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

    case CK_o_AGGREG:
      if (bIsPartitionLevel())
        return true;
      return false;

    case CK_o_GBYAGG:
      if (bIsPartitionLevel())
        return true;
      return false;

    case CK_o_ORDAGG:
      return true;

    case CK_o_PROJECT:
      if (bIsPartitionLevel())
        return true;
      return false;

    case CK_o_INDEXJOIN:
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
  pCI->G.p_emit = pSub_emit();

  pCI->G_vAppendNewChild(CK_SYNTHESIS, CK_s_MAPPING);
  pCI->G_vAppendNewChild(pCI->G.p_mapping,CK_KEYCELLS);
  pCI->G_vAppendNewChild(pCI->G.p_mapping,CK_PAYLOADCELLS);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // map the columns of the key

  pCI->G.p_key->vMapColumns(pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // map the columns of the projection

  pCI->G.p_emit->vMapColumns(pCI);
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

  if (! (pCI->G.p_emit = pSub_emit()))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* create/recreate emit                                                       */
/*----------------------------------------------------------------------------*/
void Card_OP::vRecreate_emit(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pCI->G.p_injectable = pCI->G.p_OP->pSub_injectable();
  pCI->G.p_key        = pCI->G.p_OP->pSub_key();
  pCI->G.p_needed     = pCI->G.p_OP->pSub_needed();
  pCI->G.p_stream     = pCI->G.p_OP->pSub_stream();

  eCK eCard_OP = pCI->G.p_OP->eCard();
  switch (eCard_OP) {

    case CK_c_COMBINED:
    case CK_c_SEND:
    case CK_c_STAGE:
    case CK_o_CALL:
    case CK_o_CORRELATION:
      RASSERT(! pCI->G.p_OP->pSub_emit());
      break;

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_AGGREG:
    case CK_o_CROSSJOIN:
    case CK_o_GBYAGG:
    case CK_o_ORDAGG:
      pCI->G.p_emit = pCI->G.p_OP->pSub_emit(C_NOT_NULL);
      pCI->G.p_emit->vEnsure_expr_Has_nmdCol();
      if (pCI->Flux.m_pArriving) {
        pCI->G.p_key = pCI->G.p_OP->pSub_key();
        pCI->G.p_emit->vApplyCast(pCI);
        pCI->G.p_key = nullptr;
      }
      pCI->G.p_emit->vIncFunctionCount();
      break;

    case CK_o_PROJECT:
      if (pCI->m_pBench->eXform() == XF_PROLOG_ANALYSE_EMIT) {
        vDebugPoint();
      }

      pCI->G.p_emit = pCI->G.p_OP->pSub_emit(C_NOT_NULL);
      pCI->G.p_emit->vEnsure_expr_Has_nmdCol();
      if (pCI->Flux.m_pArriving)
        pCI->G.p_emit->vApplyCast(pCI);
      pCI->G.p_emit->vIncFunctionCount();
      break;

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_FROM:
      if (pCI->G.p_OP->pSub_keyExists() || pCI->G.p_OP->pSub_keyNotExists() )
        goto L_o_KEYEXISTS;
      [[clang::fallthrough]];
    case CK_o_INDEXJOIN:
    case CK_o_INDEXSCAN:
    case CK_o_SCAN:
      if ((pCI->G.p_emit = pCI->G.p_OP->pSub_emit()))
        pCI->G.p_stream->vDeleteChild(pCI->G.p_emit);

      if (pCI->Flux.m_pArriving)
        pCI->G.p_emit = pCI->Flux.m_pArriving->pInduct(pCI);
      else
        pCI->G.p_emit = new Card_Emit;

      pCI->G.p_table = pCI->G.p_OP->pSub_table(C_NOT_NULL);
      pCI->G.p_emit->vInject_expr_nmdCol_table(pCI);


      pCI->G.p_OP->vEnsure_stream(pCI);
      pCI->G.p_stream->vAppendChild(pCI->G.p_emit);

      if ((pCI->G.p_filter = pCI->G.p_OP->pSub_filter()) &&
          (pCI->G.p_boolPred = pCI->G.p_filter->pSub_boolPred()))
        pCI->G.p_boolPred->vApplyCast(pCI);

      pCI->G.p_emit->vIncFunctionCount();
      break;

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_HAVING:
      vDebugPoint();
      [[clang::fallthrough]];

    case CK_o_HASH:
    case CK_o_KEYEXISTS:
    case CK_o_KEYNOTEXISTS:
    case CK_o_LIMIT:
    case CK_o_SORT:
    case CK_o_TOP:
    case CK_o_WHERE:
    L_o_KEYEXISTS:
      if ((pCI->G.p_emit = pCI->G.p_OP->pSub_emit()))
        pCI->G.p_stream->vDeleteChild(pCI->G.p_emit);

      pCI->G.p_emit = pCI->Flux.m_pArriving->pInduct(pCI);
      pCI->G.p_OP->vEnsure_stream(pCI);
      pCI->G.p_stream->vAppendChild(pCI->G.p_emit);
      pCI->G.p_emit->vIncFunctionCount();

      pCI->G.p_OP->vDump(__func__);

      break;

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_CAST:
    case CK_o_FILTER:
    case CK_o_INFLATE:
      pCI->G.p_OP->vDump(__func__);
      NEVER_REACH
      break;

    default:
      pCI->G.p_OP->vDump(__func__);
      NEVER_REACH
      break;

  } // switch(pCI->G.e_MainCard)

  if (   CK_o_HAVING == eCard_OP
      || CK_o_WHERE  == eCard_OP) {
    if ((pCI->G.p_boolPred = pCI->G.p_OP->pSub_boolPred()))
      pCI->G.p_boolPred->vApplyCast(pCI);
    else
    if ((pCI->G.p_rpnCalc = pCI->G.p_OP->pSub_rpnCalc()))
      pCI->G.p_rpnCalc->vApplyCast(pCI);
  }

  pCI->G.p_OP->vDump(__func__);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if ((pCI->G.p_filter = pCI->G.p_OP->pSub_filter())) {
    RASSERT(pCI->G.p_OP->bEither(CK_o_CROSSJOIN,
                                 CK_o_FROM,
                                 CK_o_HAVING,
                                 CK_o_INDEXJOIN,
                                 CK_o_KEYEXISTS,
                                 CK_o_KEYNOTEXISTS,
                                 CK_o_WHERE,
                                 CK_NONE));
    if (pCI->Flux.m_pArriving) {
      pCI->Flux.m_pArriving->vDump(__func__);
      pCI->G.p_filter->vUpdate_expCol(pCI->Flux.m_pArriving);
    }
  }
  pCI->G.p_OP->vDump(__func__);

  if (pCI->G.p_emit && pCI->G.p_key)
    pCI->G.p_emit->vApplyFacets(pCI->G.p_key);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->Flux.m_pArriving = pCI->G.p_emit;

  if (CK_o_KEYEXISTS    == eCard_OP ||
      CK_o_KEYNOTEXISTS == eCard_OP ||
      CK_o_WHERE        == eCard_OP)
    vUpdate_expCol(pCI->Flux.m_pArriving);

  if (CK_o_SORT== eCard_OP) {
    pCI->G.p_key->vApplyCast(pCI);
  }
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for aggreg op                                               */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_aggreg(String* psRpnExpr, Card_Aggreg* p_aggreg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nSubCards;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_Emit* pCard_Projection =
      _pCard_Emit(p_aggreg->pFindSubCard(CK_t_EMIT));

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
    vRpn_expr(psRpnExpr, pCard_Expr);
  }


}

/*----------------------------------------------------------------------------*/
/* produce <expr> for crossJoin op                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_crossJoin(String* psRpnExpr, Card_CrossJoin* p_crossJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Bounds* pCard_bounds = p_crossJoin->pSub_bounds();
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

    if (p_term->pSymbol()->bHasFacet(SF_EXP_COL) ||
        p_term->pSymbol()->bHasFacet(SF_TBL_COL)) {

      if (!p_term->pSymbol()->bHasType()) {
        p_term->vDump(__func__);
        pChildCard->vDump(__func__);
        RASSERT(p_term->pSymbol()->bHasType());
      }
    }
    psRpnExpr->vCatSprint(" %s"_Stz, p_term->szFacet(SF_IDX_COL));
  }
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for filter op                                               */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_filter(String* psRpnExpr, Card_Filter* p_filter)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  psRpnExpr->vCatSprint(" type BOOLEAN"_Stz);

  Card* pCard_test = p_filter->pSub_boolPred();
  if (CK_BOOLPRED == pCard_test->eCard()) {
    Card_BoolPred *pCard_CalcBool = _pCard_BoolPred(pCard_test);
    pCard_CalcBool->vOntoRpnExpr(psRpnExpr);
  }
  else {
    Card_RpnCalc *pCard_Calc = p_filter->pSub_rpnCalc();
    vRpn_rpnCalc(psRpnExpr, pCard_Calc);
  }
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for aggGby op                                               */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_gbyAgg(String* psRpnExpr, Card_GbyAgg* p_gbyAgg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nSubCards;

  Card_Key* pCard_Key =
      _pCard_Key(p_gbyAgg->pFindSubCard(CK_s_KEY));

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
    vRpn_expr(psRpnExpr, pCard_Expr);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  Card_Emit* pCard_Projection =
      _pCard_Emit(p_gbyAgg->pFindSubCard(CK_t_EMIT));

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
    vRpn_expr(psRpnExpr, pCard_Expr);
  }

//  vTrace("%s\n"_Stz,szRpnExpr);
//  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* weave : hash  <expr>                                                       */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_hash(String* psRpnExpr, Card_Hash* p_hash)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Term* p_term = p_hash->pSub_term();

  psRpnExpr->vCatSprint("type %s %s"_Stz,
                        aCT[p_term->eCTVal()],
                        p_term->szFacet(SF_EXP_COL));
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for having op                                               */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_having(String* psRpnExpr, Card_Having* p_having)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  psRpnExpr->vCatSprint(" type BOOLEAN"_Stz);

  Card* pCard_test = p_having->pSub_boolPred();
  if (pCard_test && CK_BOOLPRED == pCard_test->eCard()) {
    Card_BoolPred *pCard_CalcBool = _pCard_BoolPred(pCard_test);
    pCard_CalcBool->vOntoRpnExpr(psRpnExpr);
  }
  else {
    Card_RpnCalc *pCard_Calc = p_having->pSub_rpnCalc();
    vRpn_rpnCalc(psRpnExpr, pCard_Calc);
  }
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for indexJoin op                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_indexJoin(String* psRpnExpr, Card_IndexJoin* p_indexJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_IndexList* pCard_IndexList = p_indexJoin->pSub_indexList();

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

  if (p_indexJoin->pSub_outerJoin()) {
    if (p_indexJoin->pSub_outerJoin()->pSymbol()->Symbol::bHasFacet("LEFT_OUTER"_Stz)) {
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

    if (p_term->pSymbol()->bHasFacet(SF_EXP_COL) ||
        p_term->pSymbol()->bHasFacet(SF_TBL_COL)) {

      if (!p_term->pSymbol()->bHasType()) {
        p_term->vDump(__func__);
        pSubCard->vDump(__func__);
        RASSERT(p_term->pSymbol()->bHasType());
      }
    }

    psRpnExpr->vCatSprint(" %s"_Stz, p_term->pSymbol()->szFacet_term());
  }

  psRpnExpr->vCatSprint(" %s"_Stz, p_bound->pSymbol()->szFacet_bound());
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for indexJoin op                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_indexJoin_constraint(String* psRpnExpr, Card_IndexJoin* p_indexJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Index* pCard_Index = _pCard_Index(p_indexJoin->pFindPpyRecursive(P_ChosenIndex, CK_INDEX));
  if (! pCard_Index) {
    psRpnExpr->vConcat(" <<no:chosen index>>"_Stz);
    return;
  }

  Card_Constraint *pCard_constraint = pCard_Index->pSub_constraint();
  if (! pCard_constraint) {
    pCard_constraint = p_indexJoin->pSub_constraint();
    if (!pCard_constraint)
      return;
  }

  psRpnExpr->vCatSprint(" type BOOLEAN"_Stz);

  Card* pCard_test = pCard_constraint->pSub_boolPred();
  if (pCard_test && CK_BOOLPRED == pCard_test->eCard()) {
    Card_BoolPred *pCard_CalcBool = _pCard_BoolPred(pCard_test);
    pCard_CalcBool->vOntoRpnExpr(psRpnExpr);
  }
  else {
    Card_RpnCalc *pCard_Calc = pCard_constraint->pSub_rpnCalc();
    vRpn_rpnCalc(psRpnExpr, pCard_Calc);
  }

}

/*----------------------------------------------------------------------------*/
/* produce <expr> for indexScan op                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_indexScan(String* psRpnExpr, Card_IndexScan* p_indexScan)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_IndexList* pCard_IndexList = p_indexScan->pSub_indexList();

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

        if (p_term->pSymbol()->bHasFacet(SF_EXP_COL) ||
            p_term->pSymbol()->bHasFacet(SF_TBL_COL)) {

          if (!p_term->pSymbol()->bHasType()) {
            p_term->vDump(__func__);
            pSubCard->vDump(__func__);
            RASSERT(p_term->pSymbol()->bHasType());
          }
        }

        psRpnExpr->vCatSprint(" %s"_Stz, p_term->pSymbol()->szFacet_term());
      }

    }

    psRpnExpr->vCatSprint(" %s"_Stz, p_bound->pSymbol()->szFacet_bound());
  }
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for inflate op                                              */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_inflate(String* psRpnExpr, Card_Inflate* p_inflate)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(psRpnExpr);
  UNUSED(p_inflate);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for indexJoin op                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_keyExists(String* psRpnExpr, Card_KeyExists* p_keyExists)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Index* pCard_Index = _pCard_Index(p_keyExists->pFindPpyRecursive(P_ChosenIndex, CK_INDEX));
  if (! pCard_Index) {
    psRpnExpr->vConcat(" <<no:chosen index>>"_Stz);
    return;
  }

  Card_Bounds *pCard_bounds = pCard_Index->pSub_bounds();
  if (! pCard_bounds) {
    pCard_bounds = p_keyExists->pSub_bounds();
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

      if (p_term->pSymbol()->bHasFacet(SF_EXP_COL) ||
          p_term->pSymbol()->bHasFacet(SF_TBL_COL)) {

        if (!p_term->pSymbol()->bHasType()) {
          p_term->vDump(__func__);
          pSubCard->vDump(__func__);
          RASSERT(p_term->pSymbol()->bHasType());
        }
      }

      psRpnExpr->vCatSprint(" %s"_Stz, p_term->pSymbol()->szFacet_term());
    }

    psRpnExpr->vCatSprint(" %s"_Stz, p_bound->pSymbol()->szFacet_bound());

  }
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for indexJoin op                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_keyExists_constraint(String* psRpnExpr, Card_KeyExists* p_keyExists)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Index* pCard_Index = _pCard_Index(p_keyExists->pFindPpyRecursive(P_ChosenIndex, CK_INDEX));
  if (! pCard_Index) {
    psRpnExpr->vConcat(" <<no:chosen index>>"_Stz);
    return;
  }

  Card_Constraint *pCard_constraint = pCard_Index->pSub_constraint();
  if (! pCard_constraint) {
    pCard_constraint = p_keyExists->pSub_constraint();
    if (!pCard_constraint)
      return;
  }

  psRpnExpr->vCatSprint(" type BOOLEAN"_Stz);

  Card* pCard_test = pCard_constraint->pSub_boolPred();
  if (pCard_test && CK_BOOLPRED == pCard_test->eCard()) {
    Card_BoolPred *pCard_CalcBool = _pCard_BoolPred(pCard_test);
    pCard_CalcBool->vOntoRpnExpr(psRpnExpr);
  }
  else {
    Card_RpnCalc *pCard_Calc = pCard_constraint->pSub_rpnCalc();
    vRpn_rpnCalc(psRpnExpr, pCard_Calc);
  }

}

/*----------------------------------------------------------------------------*/
/* produce <expr> for indexJoin op                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_keyNotExists(String* psRpnExpr, Card_KeyNotExists* p_keyNotExists)
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

      if (p_term->pSymbol()->bHasFacet(SF_EXP_COL) ||
          p_term->pSymbol()->bHasFacet(SF_TBL_COL)) {

        if (!p_term->pSymbol()->bHasType()) {
          p_term->vDump(__func__);
          pSubCard->vDump(__func__);
          RASSERT(p_term->pSymbol()->bHasType());
        }
      }

      psRpnExpr->vCatSprint(" %s"_Stz, p_term->pSymbol()->szFacet_term());
    }

    psRpnExpr->vCatSprint(" %s"_Stz, p_bound->pSymbol()->szFacet_bound());
  }

}

/*----------------------------------------------------------------------------*/
/* produce <expr> for keyNotExists op                                         */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_keyNotExists_constraint(String* psRpnExpr, Card_KeyNotExists* p_keyNotExists)
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

  Card* pCard_test = pCard_constraint->pSub_boolPred();
  if (pCard_test && CK_BOOLPRED == pCard_test->eCard()) {
    Card_BoolPred *pCard_CalcBool = _pCard_BoolPred(pCard_test);
    pCard_CalcBool->vOntoRpnExpr(psRpnExpr);
  }
  else {
    Card_RpnCalc *pCard_Calc = pCard_constraint->pSub_rpnCalc();
    vRpn_rpnCalc(psRpnExpr, pCard_Calc);
  }

}

/*----------------------------------------------------------------------------*/
/* produce <expr> for limit op                                                */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_limit(String* psRpnExpr, Card_Limit*  p_limit)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  psRpnExpr->vCatSprint(" %s"_Stz, p_limit->szFacet(SF_INT_LIT));
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for ordagg op                                               */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_ordAgg(String* psRpnExpr, Card_OrdAgg* p_ordAgg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nSubCards;

  Card_Key* pCard_Key =
      _pCard_Key(p_ordAgg->pFindSubCard(CK_s_KEY));

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
    vRpn_expr(psRpnExpr, pCard_Expr);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  Card_Emit* pCard_Projection =
      _pCard_Emit(p_ordAgg->pFindSubCard(CK_t_EMIT));

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
    vRpn_expr(psRpnExpr, pCard_Expr);
  }
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for project op                                              */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_project(String* psRpnExpr, Card_Project* p_project)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Emit* pCard_Emit =
      _pCard_Emit(p_project->pFindSubCard(CK_t_EMIT));

  i8 nSubCards = pCard_Emit->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {

    Card* pSubCard = pCard_Emit->pGet(i_Child);
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

    vRpn_expr(psRpnExpr, pCard_Expr);
  }

}

/*----------------------------------------------------------------------------*/
/* produce <expr> for scan op                                                 */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_scan(String* psRpnExpr, Card_Scan* p_scan)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(psRpnExpr);
  UNUSED(p_scan);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for sort op                                                 */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_sort(String* psRpnExpr, Card_Sort* p_sort)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Key* pCard_Key =
      _pCard_Key(p_sort->pFindSubCard(CK_s_KEY));

  i8 nSubCards = pCard_Key->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {

    Card* pSubCard = pCard_Key->pGet(i_Child);
    eCK eSubCard = pSubCard->eCard();

    if (CK_EXPR != eSubCard)
      continue;

    Card_Expr* pCard_Expr = _pCard_Expr(pSubCard);

    if (i_Child)
      psRpnExpr->vConcat(" ,"_Stz);

    vRpn_expr(psRpnExpr, pCard_Expr);

    Card* p_direction = pCard_Expr->pSub_direction();

    psRpnExpr->vCatSprint(" %s"_Stz, p_direction->pSymbol()->szFacet(SF_DIRECTION));

  }

}

/*----------------------------------------------------------------------------*/
/* produce <expr> for top op                                                  */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_top(String* psRpnExpr, Card_Top* p_top)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Key *pCard_key = p_top->pSub_key();
  Card_RpnCalc *pCard_Calc = pCard_key->pSub_rpnCalc();

  TypeId xTypeId = pCard_Calc->xTypeId();

	psRpnExpr->vCatSprint(" type %s "_Stz, xTypeId.szTypeName());
  vRpn_rpnCalc(psRpnExpr, pCard_Calc);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for filter op                                               */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_where(String* psRpnExpr, Card_Where* p_where)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  psRpnExpr->vCatSprint(" type BOOLEAN"_Stz);

  Card* pCard_test = p_where->pSub_boolPred();
  if (pCard_test && CK_BOOLPRED == pCard_test->eCard()) {
    Card_BoolPred *pCard_CalcBool = _pCard_BoolPred(pCard_test);
    pCard_CalcBool->vOntoRpnExpr(psRpnExpr);
  }
  else {
    Card_RpnCalc *pCard_Calc = p_where->pSub_rpnCalc();
    vRpn_rpnCalc(psRpnExpr, pCard_Calc);
  }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* produce <expr> for p_expr                                                  */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_expr(String* psRpnExpr, Card_Expr* p_expr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_RpnCalc* p_rpnCalc = nullptr;

  i8 nSubCards = p_expr->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pCard = p_expr->pGet(i_Child);

    eCK eCard = pCard->eCard();

    switch (eCard) {
      case CK_RPNCALC:
        p_rpnCalc = _pCard_RpnCalc(pCard);
        break;

      default:
        break;
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Symbol* pSymbol = p_rpnCalc->pSymbol();
  Cast xCast = pSymbol->xCast();

  psRpnExpr->vCatSprint(" type %s"_Stz, aCT[xCast.m_xTypeId.eCTVal()]);

//#ifdef TYPE_DETAILED
  if (xCast.m_nTypeLength || xCast.m_nTypeScale) {
    psRpnExpr->vCatSprint(" ( %lld"_Stz, xCast.m_nTypeLength);
    if (xCast.m_nTypeScale)
      psRpnExpr->vCatSprint(" %lld"_Stz, xCast.m_nTypeScale);
    psRpnExpr->vCatSprint(" )"_Stz);
  }
//#endif

  if (p_expr->pSymbol()->bHasFacet(SF_ITM_COL)) {
    RASSERT(p_expr->pSymbol()->bHasFacet(SF_NMD_COL));
    psRpnExpr->vCatSprint(" as %s"_Stz, p_expr->szFacet(SF_ITM_COL));
  }

  if (p_rpnCalc->bNeedsBrace())
    psRpnExpr->vConcat(" }"_Stz);

  vRpn_rpnCalc(psRpnExpr, p_rpnCalc);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for p_rpnCalc                                               */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_rpnCalc(String* psRpnExpr, Card_RpnCalc* p_rpnCalc)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (!p_rpnCalc->pSymbol()->bHasType()) {
    p_rpnCalc->vDump(__func__);
    RASSERT(p_rpnCalc->pSymbol()->bHasType());
  }

  i8 nSubCards = p_rpnCalc->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card* pCard = p_rpnCalc->pGet(i_Child);

    eCK eCard = pCard->eCard();

    switch (eCard) {

      case CK_TERM:
        vRpn_term(psRpnExpr, _pCard_Term(pCard));
        break;

      default:
        //{;}
        //vTrace("  skipping %s\n"_Stz,aCK[eCard]);
        break;
    }

  }

}

/*----------------------------------------------------------------------------*/
/* produce <expr> for p_term                                                  */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_term(String* psRpnExpr, Card_Term* p_term)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (p_term->pSymbol()->bHasFacet(SF_EXP_COL) ||
      p_term->pSymbol()->bHasFacet(SF_TBL_COL)) {

    if (!p_term->pSymbol()->bHasType()) {
      p_term->vDump(__func__);
      RASSERT(p_term->pSymbol()->bHasType());
    }
  }

  psRpnExpr->vCatSprint(" %s"_Stz, p_term->pSymbol()->szFacet_term());
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

  Card_Emit* p_emit = pSub_emit();
  if (! p_emit)
    return nullptr;

  Card_Key* p_key = nullptr;
  if (bIsOne(CK_o_GBYAGG, CK_o_SORT, CK_o_TOP, CK_NONE)) {
    *ppColMap = new ColMap;
    p_key = pSub_key();
    RASSERT(p_key);
  }
  else {
    vDebugPoint();
  }

  ColMap* pColMap = ppColMap? *ppColMap : nullptr;

  StreamDef* pStreamDef_emitted =
      p_emit->pToStreamDef(pColMap, p_key, this);


  if (CK_o_GBYAGG == eCard()) {
    Card_Mapping* p_mapping = pSub_mapping();
    p_mapping->vToColMap(pColMap);
  }
  else
  if (CK_o_SORT == eCard()) {
    Card_Mapping* p_mapping = pSub_mapping();
    p_mapping->vToColMap(pColMap);
  }
  else
  if (CK_o_TOP == eCard()) {
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

  if (eCard() == CK_o_INFLATE)
    return;

  if (pCI->pBoard()->pState()->bHasName(aOH[OH_PROLOG]))
    vValidate_mustHave(CK_s_LEVEL, CK_s_STREAM, CK_NONE);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
