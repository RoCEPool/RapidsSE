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
: Card_symbol<Card_OP>(eCard, iFixedDepth)
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
/* test if eCard is a op                                                      */
/*----------------------------------------------------------------------------*/
bool Card_OP::bIsOp(eCK eCard)
{
  _static_

  switch (eCard) {

    case CK_o_AGGREG:
    case CK_o_CALL:
    case CK_o_CAST:
    case CK_o_CORRELATION:
    case CK_o_CROSSJOIN:
    case CK_o_FILTER:
    case CK_o_FROM:
    case CK_o_GBYAGG:
    case CK_o_HASH:
    case CK_o_HAVING:
    case CK_o_INDEXSCAN:
    case CK_o_INDEXJOIN:
    case CK_o_INFLATE:
    case CK_o_KEYEXISTS:
    case CK_o_KEYNOTEXISTS:
    case CK_o_LIMIT:
    case CK_o_ORDAGG:
    case CK_o_PDSROW:
    case CK_o_PROJECT:
    case CK_o_SCAN:
    case CK_o_SORT:
    case CK_o_TOP:
    case CK_o_WHERE:
      return true;

    case CK_o_ABSJOIN:
    case CK_o_FLAT:
    case CK_o_FOLD:
    case CK_o_SEND:
    case CK_o_STAGE:
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

  pCI->E.p_injectable = Card_Injectable::pNew_injectable(pCI);

  vAppendChild(pCI->E.p_injectable);
}

/*----------------------------------------------------------------------------*/
/* convert given lambda function to all terms                                 */
/*----------------------------------------------------------------------------*/
void Card_OP::vApply_lambdaFn_ToAll_term(CardIter* pCI,
                                         std::function<bool(Card* pCard)> bfLambdaFn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pCI->E.p_Op->bLambdaWalk(CK_TERM, bfLambdaFn);
}

/*----------------------------------------------------------------------------*/
/* correct stmOd                                                              */
/*----------------------------------------------------------------------------*/
void Card_OP::vUpdate_expCol(CardIter* pCI, Card_Emit* p_arriving, bool bAlt)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! p_arriving)
    return;

  if (Card_Filter* p_filter = pSub_filter())
    p_filter->vUpdate_expCol(pCI,p_arriving);

  if (Card_Emit* p_emit = pSub_emit())
    p_emit->vUpdate_expCol(pCI,p_arriving,bAlt);
}

/*----------------------------------------------------------------------------*/
/* remove and delete filter if it's empty                                     */
/*----------------------------------------------------------------------------*/
void Card_OP::vCull_filter_IfEmpty(Card_Filter* p_filter)
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
Card_Filter* Card_OP::pEnsure_filter(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Filter* p_filter = this->pSub_filter();
  if (p_filter)
    return p_filter;

  p_filter = _pCard_Filter(this->pAppendNewCard(CK_o_FILTER));
  Card_Predicate* p_predicate = _pCard_Predicate(p_filter->pAppendNewCard(CK_PREDICATE));
  UNUSED(p_predicate);

  return p_filter;
}

/*----------------------------------------------------------------------------*/
/* fuse next projection into this op                                          */
/*----------------------------------------------------------------------------*/
void Card_OP::vFuse_project_Viable(CardIter *pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  RASSERT(pCI->E.p_Op->bEither(CK_o_SCAN, CK_o_INDEXSCAN, CK_o_INDEXJOIN, CK_o_CROSSJOIN, CK_NONE));
  pCI->Next.m_pEmitted = pCI->Next.m_pProject->pSub_emit();

  if ((pCI->E.p_emit = pCI->E.p_Op->pSub_emit()))
    pCI->E.p_Op->vDeleteChild(pCI->E.p_emit);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // no arrived columns (only injected), emitted is next emitted

  // <scan> or <indexScan>

  if (pCI->E.p_Op->bEither(CK_o_SCAN, CK_o_INDEXSCAN, CK_NONE)) {
    pCI->E.p_Op->vAppendChild(pCI->Next.m_pEmitted->pClone());
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // arrived (and injected) columns, emitted is next emitted

  // <indexJoin> or <crossJoin>

  //
  pCI->E.p_Op->vAppendChild(pCI->Next.m_pEmitted->pClone());

  // delete any SF_EXP_COL facet with SF_TBL_COL of this op's table
  pCI->E.p_emit = pCI->E.p_Op->pSub_emit();
  pCI->E.p_table = pCI->E.p_Op->pSub_table();
  char cTblSyn = pCI->E.p_table->pSymbol()->cTblSyn();
  pCI->E.p_emit->vDeleteIf_facet(pCI, SF_EXP_COL, SF_TBL_COL, cTblSyn);
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

  pCI->E.p_key = pSub_key();
  pCI->E.p_emit = pSub_emit();

  pCI->vAppendNewChild(pCI->E.p_Op, CK_p_MAPPING);
  pCI->vAppendNewChild(pCI->E.p_mapping, CK_KEYCELLS);
  pCI->vAppendNewChild(pCI->E.p_mapping, CK_PAYLOADCELLS);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // map the columns of the key

  pCI->E.p_key->vMapColumns(pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // map the columns of the projection

  pCI->E.p_emit->vMapColumns(pCI);
  //pCI->G.p_payloadCells->vDump(__func__);
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

  pCI->E.p_indexList = nullptr;
  pCI->E.p_index = nullptr;

  if (! (pCI->E.p_indexList = pSub_indexList()))
    return false;

  if (! (pCI->E.p_index = pCI->E.p_indexList->pFindChosenIndex()))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_OP::bPeg_Key(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! (pCI->E.p_key = pSub_key()))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_OP::bPeg_Projection(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! (pCI->E.p_emit = pSub_emit()))
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* create/recreate emit                                                       */
/*----------------------------------------------------------------------------*/
void Card_OP::vRestate_emit(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pCI->E.p_injectable = pCI->E.p_Op->pSub_injectable();
  pCI->E.p_key        = pCI->E.p_Op->pSub_key();
  pCI->E.p_needed     = pCI->E.p_Op->pSub_needed();
  pCI->E.p_emit       = pCI->E.p_Op->pSub_emit();

  eCK eCard_OP = pCI->E.p_Op->eCard();
  switch (eCard_OP) {

    case CK_o_SEND:
    case CK_o_STAGE:
    case CK_o_CALL:
    case CK_o_CORRELATION:
      RASSERT(! pCI->E.p_Op->pSub_emit());
      break;

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_AGGREG:
    case CK_o_CROSSJOIN:
    case CK_o_GBYAGG:
    case CK_o_ORDAGG:
      pCI->E.p_emit = pCI->E.p_Op->pSub_emit(C_NOT_NULL);
      pCI->E.p_emit->vEnsureAll_expr_Have_nmdCol();
      if (pCI->Arriving.m_pEmit) {
        pCI->Dic.m_p_expr = pCI->Arriving.m_pEmit;
        pCI->E.p_key = pCI->E.p_Op->pSub_key();
        if (pCI->E.p_key)
          pCI->E.p_key->vApplyCast(pCI,/*??*/pCI->Dic.m_p_expr);
        pCI->E.p_emit->vApplyCast(pCI,/*??*/pCI->Dic.m_p_expr);
        pCI->E.p_key = nullptr;
      }
      break;

    case CK_o_PROJECT:
      if (pCI->m_pBench->eXform() == XF_PROLOG_ANALYSE_EMIT) {
        vDebugPoint();
      }

      pCI->E.p_emit = pCI->E.p_Op->pSub_emit(C_NOT_NULL);
      pCI->E.p_emit->vEnsureAll_expr_Have_nmdCol();
      if (pCI->Arriving.m_pEmit) {
        pCI->Dic.m_p_expr = pCI->Arriving.m_pEmit;
        pCI->E.p_emit->vApplyCast(pCI,/*??*/pCI->Dic.m_p_expr);
      }
      break;

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_FROM:
      if (pCI->E.p_Op->pSub_keyExists() || pCI->E.p_Op->pSub_keyNotExists() )
        goto L_o_KEYEXISTS;
      [[clang::fallthrough]];
    case CK_o_INDEXJOIN:
    case CK_o_INDEXSCAN:
    case CK_o_SCAN:
      if ((pCI->E.p_emit = pCI->E.p_Op->pSub_emit()))
        pCI->E.p_emit->vDeleteAllChildren();
      else {
        pCI->E.p_emit = new Card_Emit;
        pCI->E.p_Op->vAppendChild(pCI->E.p_emit);
      }

      if (pCI->Arriving.m_pEmit)
        pCI->Arriving.m_pEmit->vInduct(pCI);

      pCI->E.p_table = pCI->E.p_Op->pSub_table(C_NOT_NULL);
      pCI->E.p_emit->vInject_expr_nmdCol_table(pCI);

      if ((pCI->E.p_filter = pCI->E.p_Op->pSub_filter()) &&
          (pCI->E.p_predicate = pCI->E.p_filter->pSub_predicate())) {
        pCI->Dic.m_p_expr = pCI->Arriving.m_pEmit;
        pCI->E.p_predicate->vApplyCast(pCI,/*??*/pCI->Dic.m_p_expr);
      }

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

      if ((pCI->E.p_emit = pCI->E.p_Op->pSub_emit()))
        pCI->E.p_emit->vDeleteAllChildren();
      else {
        pCI->E.p_emit = new Card_Emit;
        pCI->E.p_Op->vAppendChild(pCI->E.p_emit);
      }

      pCI->Arriving.m_pEmit->vInduct(pCI);

      break;

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
    case CK_o_CAST:
    case CK_o_FILTER:
    case CK_o_INFLATE:
      pCI->E.p_Op->vDump(__func__);
      NEVER_REACH
      break;

    case CK_o_FOLD:
      return;

    default:
      pCI->E.p_Op->vDump(__func__);
      NEVER_REACH
      break;

  } // switch(pCI->G.e_Op)


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if ((pCI->E.p_filter = pCI->E.p_Op->pSub_filter())) {
    RASSERT(pCI->E.p_Op->bEither(CK_o_CROSSJOIN,
                                 CK_o_FROM,
                                 CK_o_HAVING,
                                 CK_o_INDEXJOIN,
                                 CK_o_INDEXSCAN,
                                 CK_o_KEYEXISTS,
                                 CK_o_KEYNOTEXISTS,
                                 CK_o_SCAN,
                                 CK_o_WHERE,
                                 CK_NONE));
    if (pCI->E.p_Op->bEither(CK_o_KEYEXISTS, CK_o_KEYNOTEXISTS, CK_NONE)) {
      if (pCI->Arriving.m_pEmit) {
        pCI->Dic.m_p_expr = _pCard_Emit(pCI->E.p_Op->pSub_injectable());
        pCI->E.p_filter->vUpdate_expCol(pCI,pCI->Dic.m_p_expr, true);
      }
    }
    else {
      if (pCI->Arriving.m_pEmit) {
        pCI->Dic.m_p_expr = pCI->Arriving.m_pEmit;
        if (pCI->E.p_Op->bIsCard(CK_o_HAVING))
          pCI->E.p_filter->vUpdate_expCol(pCI,pCI->Dic.m_p_expr, true);
        else
          pCI->E.p_filter->vUpdate_expCol(pCI,pCI->Dic.m_p_expr, true);
      }
    }
  }

  if (pCI->E.p_emit && pCI->E.p_Op->pSub_key()) {
    if (AM::JOB::bHasTag(C_OPTIMISER_TRACE))
      vDebugPoint();
    pCI->E.p_key = pCI->E.p_Op->pSub_key();
    pCI->E.p_emit->vApplyFacets(pCI,pCI->E.p_key);

    for (Kid xEmit(pCI->E.p_emit); xEmit.bEach(CK_EXPR); /**/) {
      for (pCI->vBeginTarget(xEmit.p_expr(),CK_TERM); pCI->bEachTarget(); /**/) {

        Card_Term *p_term_keyCol = pCI->E.xTarget.p_term();
        Symbol *pSym_term_keyCol = p_term_keyCol->pSymbol();
        if (! pSym_term_keyCol->bHasFacet(SF_KEY_COL))
          continue;

        i8 i_Key = pSym_term_keyCol->iTo_ordinal(SF_KEY_COL) - 1;
        Card_Expr *p_expr_key = pCI->E.p_key->pGet_expr(i_Key);
        if (p_expr_key->bIsScalarCol()) {
          p_expr_key->pSymbol()->vApplyFacets(pSym_term_keyCol);
          vDebugPoint();
        }
      }
    }

  }


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (CK_o_SORT== eCard_OP) {
    pCI->Dic.m_p_expr = pCI->E.p_emit;
    pCI->E.p_key->vApplyCast(pCI,/*??*/pCI->Dic.m_p_expr);
  }
  pCI->vSnapIf_arriving();

  RASSERT(pCI->E.p_Op->pSub_emit() == pCI->E.p_emit);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for aggreg op                                               */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_aggreg(CardIter* pCI, String* psRpnExpr, Card_Aggreg* p_aggreg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);

  i8 nSubCards;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_Emit* pCard_Projection =
      _pCard_Emit(p_aggreg->pFindSubCard(CK_p_EMIT));

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
    vRpn_expr(pCI, psRpnExpr, pCard_Expr);
  }


}

/*----------------------------------------------------------------------------*/
/* produce <expr> for crossJoin op                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_crossJoin(CardIter* pCI, String* psRpnExpr, Card_CrossJoin* p_crossJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);

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

      if (!p_term->pSymbol()->bValidType()) {
        p_term->vDump(__func__);
        pChildCard->vDump(__func__);
        RASSERT(p_term->pSymbol()->bValidType());
      }
    }
    psRpnExpr->vCatSprint(" %s"_Stz, p_term->szFacet(SF_IDX_COL));
  }
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for filter op                                               */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_filter(CardIter* pCI, String* psRpnExpr, Card_Filter* p_filter)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);

  psRpnExpr->vCatSprint(" type BOOLEAN"_Stz);

  Card_Predicate *pCard_CalcBool = p_filter->pSub_predicate();
  pCard_CalcBool->vOntoRpnExpr(psRpnExpr);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for aggGby op                                               */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_gbyAgg(CardIter* pCI, String* psRpnExpr, Card_GbyAgg* p_gbyAgg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);

  i8 nSubCards;

  Card_Key* p_key = p_gbyAgg->pSub_key();
  if (! p_key)
    return;

  psRpnExpr->vConcat(" groupBy"_Stz);

//Z_KEY_DIRECTION
  for (pCI->vBeginTarget(p_key,CK_EXPR); pCI->bEachTarget(); /**/) {

    if (pCI->i_Target()) {
      psRpnExpr->vConcat("\n                 "_Stz);
      psRpnExpr->vConcat(" ,"_Stz);
    }
    else {
      psRpnExpr->vConcat("\n                 "_Stz);
      psRpnExpr->vConcat("  "_Stz);
    }

    vRpn_expr(pCI, psRpnExpr, pCI->E.p_expr);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  Card_Emit* pCard_Projection =
      _pCard_Emit(p_gbyAgg->pFindSubCard(CK_p_EMIT));

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
    vRpn_expr(pCI, psRpnExpr, pCard_Expr);
  }

//  vTrace("%s\n"_Stz,szRpnExpr);
//  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/* weave : hash  <expr>                                                       */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_hash(CardIter* pCI, String* psRpnExpr, Card_Hash* p_hash)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);

  Card_Term* p_term = p_hash->pSub_term();

  psRpnExpr->vCatSprint("type %s %s"_Stz,
                        aCT[p_term->eCTVal()],
                        p_term->szFacet(SF_EXP_COL));
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for having op                                               */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_having(CardIter* pCI, String* psRpnExpr, Card_Having* p_having)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);

  psRpnExpr->vCatSprint(" type BOOLEAN"_Stz);

  Card_Predicate *pCard_CalcBool = p_having->pSub_predicate();
  pCard_CalcBool->vOntoRpnExpr(psRpnExpr);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for indexJoin op                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_indexJoin(CardIter* pCI, String* psRpnExpr, Card_IndexJoin* p_indexJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);

  Card_IndexList* pCard_IndexList = p_indexJoin->pSub_indexList();

  Card_Index* pCard_Index =
    _pCard_Index(pCard_IndexList->pFindPpyRecursive(P_ChosenIndex,CK_INDEX));

  Card_Bounds* pCard_bounds = pCard_Index->pSub_bounds();
  if (! pCard_bounds)
    return;

  //Card_Bound* p_bound = (Card_Bound*)pCard_bounds->pChild(0);

  Card_Bound* p_bound = nullptr;
  for (Kid xBound(pCard_bounds); xBound.bEach(CK_BOUND); /**/) {

    if (xBound.pChild()->pSymbol()->Y_bHas(P_ChosenBound)) {
      p_bound = _pCard_Bound(xBound.pChild());
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

      if (!p_term->pSymbol()->bValidType()) {
        p_term->vDump(__func__);
        pSubCard->vDump(__func__);
        RASSERT(p_term->pSymbol()->bValidType());
      }
    }

    psRpnExpr->vCatSprint(" %s"_Stz, p_term->pSymbol()->szFacet_rpnTerm());
  }

  psRpnExpr->vCatSprint(" %s"_Stz, p_bound->pSymbol()->szFacet0());
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for indexJoin op                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_indexJoin_constraint(CardIter* pCI, String* psRpnExpr, Card_IndexJoin* p_indexJoin)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);

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

  Card_Predicate *pCard_CalcBool = pCard_constraint->pSub_predicate();
  pCard_CalcBool->vOntoRpnExpr(psRpnExpr);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for indexScan op                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_indexScan(CardIter* pCI, String* psRpnExpr, Card_IndexScan* p_indexScan)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);

  Card_IndexList* pCard_IndexList = p_indexScan->pSub_indexList();

  Card_Index* pCard_Index =
      _pCard_Index(pCard_IndexList->pFindPpyRecursive(P_ChosenIndex,CK_INDEX));


  Card_Bounds *pCard_bounds = pCard_Index->pSub_bounds();
  if (! pCard_bounds) {
    pCard_bounds = pCard_Index->pSub_bounds();
    if (! pCard_bounds)
      return;
  }

  Kid xBound(pCard_bounds);
  while (xBound.bEach(CK_BOUND)) {

    Card_Bound* p_bound = xBound.p_bound();

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

          if (!p_term->pSymbol()->bValidType()) {
            p_term->vDump(__func__);
            pSubCard->vDump(__func__);
            RASSERT(p_term->pSymbol()->bValidType());
          }
        }

        psRpnExpr->vCatSprint(" %s"_Stz, p_term->pSymbol()->szFacet_rpnTerm());
      }

    }

    psRpnExpr->vCatSprint(" %s"_Stz, p_bound->pSymbol()->szFacet0());
  }
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for inflate op                                              */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_inflate(CardIter* pCI, String* psRpnExpr, Card_Inflate* p_inflate)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);
  UNUSED(psRpnExpr);
  UNUSED(p_inflate);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for indexJoin op                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_keyExists(CardIter* pCI, String* psRpnExpr, Card_KeyExists* p_keyExists)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);

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

  Kid xBound(pCard_bounds);
  while (xBound.bEach(CK_BOUND)) {

    Card_Bound* p_bound = xBound.p_bound();

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

        if (!p_term->pSymbol()->bValidType()) {
          p_term->vDump(__func__);
          pSubCard->vDump(__func__);
          RASSERT(p_term->pSymbol()->bValidType());
        }
      }

      psRpnExpr->vCatSprint(" %s"_Stz, p_term->pSymbol()->szFacet_rpnTerm());
    }

    psRpnExpr->vCatSprint(" %s"_Stz, p_bound->pSymbol()->szFacet0());

  }
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for indexJoin op                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_keyExists_constraint(CardIter* pCI, String* psRpnExpr, Card_KeyExists* p_keyExists)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);

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

  Card_Predicate *pCard_CalcBool = p_keyExists->pSub_predicate();
  pCard_CalcBool->vOntoRpnExpr(psRpnExpr);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for indexJoin op                                            */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_keyNotExists(CardIter* pCI, String* psRpnExpr, Card_KeyNotExists* p_keyNotExists)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);

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

  Kid xBound(pCard_bounds);
  while (xBound.bEach(CK_BOUND)) {

    Card_Bound* p_bound = xBound.p_bound();

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

        if (!p_term->pSymbol()->bValidType()) {
          p_term->vDump(__func__);
          pSubCard->vDump(__func__);
          RASSERT(p_term->pSymbol()->bValidType());
        }
      }

      psRpnExpr->vCatSprint(" %s"_Stz, p_term->pSymbol()->szFacet_rpnTerm());
    }

    psRpnExpr->vCatSprint(" %s"_Stz, p_bound->pSymbol()->szFacet0());
  }

}

/*----------------------------------------------------------------------------*/
/* produce <expr> for keyNotExists op                                         */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_keyNotExists_constraint(CardIter* pCI, String* psRpnExpr, Card_KeyNotExists* p_keyNotExists)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);

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

  Card_Predicate *pCard_CalcBool = p_keyNotExists->pSub_predicate();
  pCard_CalcBool->vOntoRpnExpr(psRpnExpr);

}

/*----------------------------------------------------------------------------*/
/* produce <expr> for limit op                                                */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_limit(CardIter* pCI, String* psRpnExpr, Card_Limit*  p_limit)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);

  psRpnExpr->vCatSprint(" %s"_Stz, p_limit->szFacet(SF_INT_LIT));
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for ordagg op                                               */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_ordAgg(CardIter* pCI, String* psRpnExpr, Card_OrdAgg* p_ordAgg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);

  i8 nSubCards;

  Card_Key* pCard_Key =
      _pCard_Key(p_ordAgg->pFindSubCard(CK_p_KEY));

  if (! pCard_Key)
    return;

  psRpnExpr->vConcat(" ordGroupBy"_Stz);

  for (pCI->vBeginTarget(pCard_Key,CK_EXPR); pCI->bEachTarget(); /**/) {

    if (pCI->E.xTarget.i_Child()) {
      psRpnExpr->vConcat("\n                 "_Stz);
      psRpnExpr->vConcat(" ,"_Stz);
    }
    else {
      psRpnExpr->vConcat("\n                 "_Stz);
      psRpnExpr->vConcat("  "_Stz);
    }

    vRpn_expr(pCI, psRpnExpr, pCI->E.p_expr);
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  Card_Emit* pCard_Emit =
      _pCard_Emit(p_ordAgg->pFindSubCard(CK_p_EMIT));

  if (! pCard_Emit)
    return;

  psRpnExpr->vConcat("\n                 "_Stz);
  psRpnExpr->vConcat(" aggregates"_Stz);

  nSubCards = pCard_Emit->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card* pCard_SUB = pCard_Emit->pGet(i_Child);
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
    vRpn_expr(pCI, psRpnExpr, pCard_Expr);
  }
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for pdsRow                                                  */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_pdsRow(CardIter* pCI, String* psRpnExpr, Card_PdsRow* p_pdsRow)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);
  UNUSED(psRpnExpr);
  UNUSED(p_pdsRow);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for project op                                              */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_project(CardIter* pCI, String* psRpnExpr, Card_Project* p_project)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);

  Card_Emit* pCard_Emit =
      _pCard_Emit(p_project->pFindSubCard(CK_p_EMIT));

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

    vRpn_expr(pCI, psRpnExpr, pCard_Expr);
  }

}

/*----------------------------------------------------------------------------*/
/* produce <expr> for scan op                                                 */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_scan(CardIter* pCI, String* psRpnExpr, Card_Scan* p_scan)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);

  UNUSED(psRpnExpr);
  UNUSED(p_scan);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for sort op                                                 */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_sort(CardIter* pCI, String* psRpnExpr, Card_Sort* p_sort)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);

  Card_Key* p_key = p_sort->pSub_key();

//Z_KEY_DIRECTION
   for (pCI->vBeginTarget(p_key,CK_EXPR); pCI->bEachTarget(); /**/) {

    if (pCI->i_Target())
      psRpnExpr->vConcat(" ,"_Stz);
    vRpn_expr(pCI, psRpnExpr, pCI->E.p_expr);

    psRpnExpr->vCatSprint(" %s"_Stz, p_key->szDirection(pCI->i_Target()));
  }

}

/*----------------------------------------------------------------------------*/
/* produce <expr> for top op                                                  */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_top(CardIter* pCI, String* psRpnExpr, Card_Top* p_top)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);

  Card_Key *pCard_key = p_top->pSub_key();
  Card_Expr *pCard_Expr = pCard_key->pSub_expr(C_NOT_NULL);
  TypeId xTypeId = pCard_Expr->xTypeId();


	psRpnExpr->vCatSprint(" type %s "_Stz, xTypeId.szTypeName());
  vRpn_termSeq(pCI, psRpnExpr, pCard_Expr);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for filter op                                               */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_where(CardIter* pCI, String* psRpnExpr, Card_Where* p_where)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);

  psRpnExpr->vCatSprint(" type BOOLEAN"_Stz);

  Card_Predicate *pCard_CalcBool = p_where->pSub_predicate();
  pCard_CalcBool->vOntoRpnExpr(psRpnExpr);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* produce <expr> for p_expr                                                  */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_expr(CardIter* pCI, String* psRpnExpr, Card_Expr* p_expr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Symbol* pSymbol = p_expr->pSymbol();
  Cast xCast = pSymbol->xCast();

  psRpnExpr->vCatSprint(" type %s"_Stz, aCT[xCast.m_xTypeId.eCTVal()]);

  if (xCast.m_nTypeLength || xCast.m_nTypeScale) {
    psRpnExpr->vCatSprint(" ( %lld"_Stz, xCast.m_nTypeLength);
    if (xCast.m_nTypeScale)
      psRpnExpr->vCatSprint(" %lld"_Stz, xCast.m_nTypeScale);
    psRpnExpr->vCatSprint(" )"_Stz);
  }

  if (p_expr->pSymbol()->bHasFacet(SF_ITM_COL)) {
    RASSERT(p_expr->pSymbol()->bHasFacet(SF_NMD_COL));
    psRpnExpr->vCatSprint(" as %s"_Stz, p_expr->szFacet(SF_ITM_COL));
  }

  if (p_expr->bNeedsBrace(pCI))
    psRpnExpr->vConcat(" }"_Stz);

  vRpn_termSeq(pCI, psRpnExpr, p_expr);
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for p_term                                                  */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_term(CardIter* pCI, String* psRpnExpr, Card_Term* p_term)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);

  if (p_term->pSymbol()->bHasFacet(SF_EXP_COL) ||
      p_term->pSymbol()->bHasFacet(SF_TBL_COL)) {

    if (!p_term->pSymbol()->bValidType()) {
      p_term->vDump(__func__);
      RASSERT(p_term->pSymbol()->bValidType());
    }
  }

  psRpnExpr->vCatSprint(" %s"_Stz, p_term->pSymbol()->szFacet_rpnTerm());
}

/*----------------------------------------------------------------------------*/
/* produce <expr> for p_rpnCalc                                               */
/*----------------------------------------------------------------------------*/
void Card_OP::vRpn_termSeq(CardIter* pCI, String* psRpnExpr, Card_Expr* p_expr)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 nSubCards = p_expr->nItems();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card_Term* pCard = p_expr->pGet_term(i_Child);
    vRpn_term(pCI, psRpnExpr, _pCard_Term(pCard));
  }
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
IndexDef* Card_OP::pCraftIndexDef_key(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Key* p_key = pSub_key();
  if (! p_key)
    return nullptr;

  IndexDef* pIndexDef = p_key->pTo_IndexDef(pCI);

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
  if (bEither(CK_o_GBYAGG, CK_o_SORT, CK_o_TOP, CK_NONE)) {
    *ppColMap = new ColMap(__FILE__,__LINE__);
    p_key = pSub_key();
    RASSERT(p_key);
  }
  else {
    vDebugPoint();
  }

  ColMap* pColMap = ppColMap? *ppColMap : nullptr;

  if (CK_o_GBYAGG == eCard()) {
//    pColMap->vDump();
//    p_key->vDump(__func__);
    vDebugPoint();
  }

  StreamDef* pStreamDef_emitted =
      p_emit->pTo_StreamDef(pColMap, p_key, this);

  if (CK_o_GBYAGG == eCard()) {
//    pColMap->vDump();
//    pStreamDef_emitted->vDump();
    vDebugPoint();
  }

  if (CK_o_GBYAGG == eCard()) {
    Card_Mapping* p_mapping = pSub_mapping();
    p_mapping->vTo_ColMap(pColMap);
  }
  else
  if (CK_o_SORT == eCard()) {
    Card_Mapping* p_mapping = pSub_mapping();
    p_mapping->vTo_ColMap(pColMap);
  }
  else
  if (CK_o_TOP == eCard()) {
    Card_Mapping* p_mapping = pSub_mapping();
    p_mapping->vTo_ColMap(pColMap);
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

  RASSERT(1 == iDepth);

  if (eCard() == CK_o_INFLATE || eCard() == CK_o_PDSROW)
    return;

  if (pCI->pBoard()->pState()->bHasName(aOH[OH_PROLOG]))
    vValidate_child_mustBe(CK_p_LEVEL, CK_NONE);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
