/*============================================================================*/
/* Card_Projection.cpp -- Projection                                          */
/*============================================================================*/

#include "Card_Projection.hpp"

#include "Card_RpnCalc.hpp"

#include "CardIter.hpp"
#include "Card_GbyAgg.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Projection, CK_PROJECTION)
__aNewFunction(Card_Projection, CN_CARD)
__vDeleteFunction(Card_Projection, CN_CARD)



/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Projection::Card_Projection(i8 iCrossRef)
: Card_ELIST(CK_PROJECTION,3)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
          
  _checkInitialised_(CN_CARD);

  SYM(SF_XRF_NO,iCrossRef);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Projection::~Card_Projection(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* resolve (right-left) projection prior to main card                         */
/*----------------------------------------------------------------------------*/
Card_Projection* Card_Projection::pResolvePriorProjection(CardIter* pCI)
{
  _static_

  Kid xKid(pCI->G.pDeck);

  xKid.vBeginEach_reverse();
  xKid.vResynch(pCI->G.i_MainCard);

  while (xKid.bEach_reverse()) {
    if (Card_Projection* p_projection = xKid.pChild()->pSub_projection())
      return p_projection;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* adjust this projection given key                                           */
/*----------------------------------------------------------------------------*/
void Card_Projection::vAdjust_Wrt_key(Card_Key* p_key)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //p_key->vDump();

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/) {

    //xKid.p_expr()->vDump(__func__ );

    if (! xKid.p_expr()->bIsScalarCol())
      continue;

    Symbol* pSymbol_expr_sole = xKid.p_expr()->pGetSymbol(0);

    Card_Expr* p_expr_sole_key = p_key->pFind_expr_Of_symbol(pSymbol_expr_sole);
    if (! p_expr_sole_key)
      continue;

    pSymbol_expr_sole->vSym(p_expr_sole_key->pSymbol());
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Projection* Card_Projection::pClone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return _pCard_Projection(pDupCardRecursive());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Projection::vCorrectStmOdn(Card_Projection* p_projection_prior)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/) {

    if (Card_BoolCalc* p_boolCalc = xKid.p_expr()->pSub_boolCalc())
      p_boolCalc->vCorrectStmOdn(p_projection_prior);
    else
    if (Card_RpnCalc* p_rpnCalc = xKid.p_expr()->pSub_rpnCalc())
      p_rpnCalc->vCorrectStmOdn(p_projection_prior);
  }

}

/*----------------------------------------------------------------------------*/
/* craft factored-column-set this projection needs for early-projects         */
/*----------------------------------------------------------------------------*/
Card_Needed* Card_Projection::pCraftRoot_needed(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  // this projection will be in an aggreg, gbyAgg or ordAgg
  RASSERT(pCI->G.p_OP->bIsEarlyRoot());

  pCI->G.p_streamKey = pSub_streamKey();

  Card_Needed* p_needed = new Card_Needed;
  p_needed->vAddedCorrectly();



  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 1) append the referenced col's in rpnCalc of this projection into needed projection

  Kid xCI_projection(this);
  while (xCI_projection.bEach(CK_EXPR,CK_RPNCALC)) {

    Kid xCI_rpnCalc(xCI_projection.pSub1());
    while (xCI_rpnCalc.bEach(CK_TERM)) {
      // only interested in columns

      if (xCI_rpnCalc.p_term()->bIsKeyOdn()) {
        RASSERT(pCI->G.p_streamKey);

        Card_Expr* p_expr_key = pCI->G.p_streamKey->pDecodeExpr(xCI_rpnCalc.p_term()->pSymbol());
        Symbol* pSymbol_tblCol = p_expr_key->pFirstTblCol();

        vDebugPoint();

        if (p_needed->pFind_expr_Of_symbol(pSymbol_tblCol))
          continue;

        p_needed->vAppendChild(p_expr_key->pClone());
      }
      else
      if (xCI_rpnCalc.p_term()->bIsValOdn()) {

        // which we haven't already added
        if (p_needed->pFind_expr_Of_symbol(xCI_rpnCalc.p_term()->pSymbol()))
          continue;

        if (Card_Expr *p_expr_col = pCI->pExpr_For_col(xCI_rpnCalc.p_term()))
          p_needed->vAppendChild(p_expr_col->pClone());
      }
      else {

      }
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 2) get factored columns of key as start of needed projection

  if (pCI->G.p_OP->bIsCard(CK_GBYAGG) || pCI->G.p_OP->bIsCard(CK_ORDAGG)) {
    if ((pCI->G.p_key = pSub_key())) {
      // + projection of gbyAgg/ordAgg has a key

      Kid xCI_key(pCI->G.p_key);
      while (xCI_key.bEach(CK_EXPR,CK_RPNCALC)) {
        Kid xCI_rpnCalc(xCI_key.pSub1());
        while (xCI_rpnCalc.bEach(CK_TERM)) {
          // only interested in columns
          if (!xCI_rpnCalc.p_term()->bIsValOdn())
            continue;
          // which we haven't already added
          if (p_needed->pFind_expr_Of_symbol(xCI_rpnCalc.p_term()->pSymbol()))
            continue;

          if (Card_Expr *p_expr_col = pCI->pExpr_For_col(xCI_rpnCalc.p_term()))
            p_needed->vAppendChild(p_expr_col->pClone());
        }
      }
    }
  }


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  p_needed->vRenumberAll_expr();
  //p_needed->vDump();

  return p_needed;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Projection::vDedupe_as(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  for (Kid xProjection(this); xProjection.bEach(CK_EXPR); /**/) {
    Card_As* p_as = xProjection.p_expr()->pSub_as();

    p_as->SYM(SF_COL_NM_dis,xProjection.i_Child(), p_as->pSymbol());
  }

  vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Projection::vDelete_streamKey(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if ((pCI->G.p_streamKey = pSub_streamKey()))
    vDeleteChild(pCI->G.p_streamKey);

  pCI->G.p_streamKey = nullptr;
}

/*----------------------------------------------------------------------------*/
/* derive the key of this projection (of running key)                         */
/*----------------------------------------------------------------------------*/
void Card_Projection::vDerive_streamKey(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! pCI->StreamOrder.m_pStreamKey_running)
    return;

  i8 nKeyExpr = pCI->StreamOrder.m_pStreamKey_running->nChildCards();
  for (i8 i_RunKeyExpr = 0; i_RunKeyExpr < nKeyExpr; i_RunKeyExpr++) {
    Card_Expr* p_expr_runkey = pCI->StreamOrder.m_pStreamKey_running->pGetExpr(i_RunKeyExpr);

    Card_Term* p_term_runkey = p_expr_runkey->pGetTerm(0);
    Card_Expr* p_expr_prjCol = pFind_expr_Of_symbol(p_term_runkey->pSymbol());

    if (p_expr_prjCol) {

      // annotation or direction in passing
      if (p_expr_runkey->pSub_direction() && ! p_expr_prjCol->pSub_direction()) {
        p_expr_prjCol->vAppendChild(p_expr_runkey->pSub_direction()->pDupCardRecursive());
      }

      pCI->G.p_streamKey->vAdd_key_expr_BasedOn_expr(p_expr_prjCol,false);
    }

  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Projection::vEnsureTypes_For_stmColOdn(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/)
    xKid.p_expr()->vEnsureTypes_For_stmOdn(pCI);
}

/*----------------------------------------------------------------------------*/
/* propagate this projection                                                  */
/*----------------------------------------------------------------------------*/
Card_Projection* Card_Projection::pPropagate(CardIter* pCI) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Projection *p_projection_propagated = new Card_Projection(pCI->iXform());

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/) {

    Card_Expr *p_expr_propagated = xKid.p_expr()->pPropagate();
    p_projection_propagated->vAppendChild(p_expr_propagated);
  }

  p_projection_propagated->vRenumberAll_expr();

  return p_projection_propagated;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Projection::pFindExprOfAs(Symbol* pSymbol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Expr* p_expr = nullptr;

  Card* pParent;
  Card_As* p_as = _pCard_As(pFindSymbolRecursive(CK_AS,pSymbol,pParent));

  if (p_as) {
    p_expr = _pCard_Expr(pParent);
  }

  return p_expr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Projection::pFindExprOfSymbol(Symbol* pSymbol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/)
    if (xKid.p_expr()->bHasTerm(pSymbol))
      return xKid.p_expr();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Projection::bHasTerm(Symbol* pSymbol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/)
    if (xKid.p_expr()->bHasTerm(pSymbol))
      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* return a reduced copy of this projection                                   */
/*----------------------------------------------------------------------------*/
Card_Projection* Card_Projection::pReducedCopy(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Projection* p_projection_reduced = new Card_Projection(pCI->iXform());

  i8 i_Expr = 0;
  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/) {
    Card_Expr* p_expr_copy = xKid.p_expr()->pReducedCopy(i_Expr++);
    p_projection_reduced->vAppendChild(p_expr_copy);
  }

  return p_projection_reduced;
}

/*----------------------------------------------------------------------------*/
/* resolve prior expr of given term (find right-to-left)                      */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Projection::pResolve_expr(Card_Term* p_term)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if (p_term->pSymbol()->bIs_Col_No()) {
    Card_Expr *p_expr_prior = this->pDecodeExpr(p_term->pSymbol());
    return p_expr_prior;
  }
  else
  if (p_term->pSymbol()->bIs_Col_Tc()) {
    Card_Expr *p_expr_prior = this->pFind_expr_Of_symbol(p_term->pSymbol());
    return p_expr_prior;
  }

  return nullptr;
}
/*----------------------------------------------------------------------------*/
/* as review                                                                  */
/*----------------------------------------------------------------------------*/
void Card_Projection::vAs_Review(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xProjection(this); xProjection.bEach(); /**/) {
    Card_Expr *p_expr_prj = xProjection.p_expr();

    if (p_expr_prj->bIsScalarKey()) {
      Symbol* pSymbol_key_term = p_expr_prj->pGetTerm(0)->pSymbol();

      if (p_expr_prj->pSub_as()->pSymbol()->Y_bHas(P_Synthetic)) {
        Card_Expr *p_expr_key = pCI->G.p_key->pDecodeExpr(pSymbol_key_term);

        p_expr_prj->pSub_as()->SYM(SF_AS_NM,p_expr_key->pSub_as()->szSymbol(SF_NONE));
        p_expr_prj->pSub_as()->pSymbol()->Y_vCut(P_Synthetic);
      }
    }
  }

}

/*----------------------------------------------------------------------------*/
/* convert to streamDef                                                       */
/*----------------------------------------------------------------------------*/
StreamDef* Card_Projection::pToStreamDef(ColMap* pColMap,
                                         Card_Key* p_key,
                                         Card* p_OP)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  StreamDef* pStreamDef = new StreamDef;

  s32 sColNm;
  s32 sColNm_mapped;
  eCT eColType;



  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (CK_GBYAGG == p_OP->eCard()) {

    for (Kid xProjection(this); xProjection.bEach(CK_EXPR); /**/) {
      Card_Expr *p_expr = xProjection.p_expr();

      if (p_expr->pSub_as())
        sColNm = p_expr->pSub_as()->szSymbol(SF_NONE);
      else
        sColNm = p_expr->pGetTerm(0)->szSymbol(SF_NONE);

      p_key->vX();

#ifndef Z_ITEMISED_NAMES
      s32 sNm_modified;
      sNm_modified.vSprint("C%lld_%s"_Stz,
                           xProjection.i_Child(),
                           sColNm.azChar());
      sColNm = sNm_modified;
#endif

      eColType = p_expr->eColumnType();
      pStreamDef->vAddColumn(sColNm, eColType, C_UNKNOWN, C_UNKNOWN);
    }

    return pStreamDef;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (CK_SORT == p_OP->eCard()) {

    for (Kid xProjection(this); xProjection.bEach(CK_EXPR); /**/) {
      Card_Expr *p_expr = xProjection.p_expr();

      if (p_expr->pSub_as())
        sColNm = p_expr->pSub_as()->szSymbol(SF_NONE);
      else
        sColNm = p_expr->pGetTerm(0)->szSymbol(SF_NONE);

      p_key->vX();

#ifndef Z_ITEMISED_NAMES
      s32 sNm_modified;
      sNm_modified.vSprint("C%lld_%s"_Stz,
                           xProjection.i_Child(),
                           sColNm.azChar());
      sColNm = sNm_modified;
#endif

      eColType = p_expr->eColumnType();
      pStreamDef->vAddColumn(sColNm, eColType, C_UNKNOWN, C_UNKNOWN);
    }

    return pStreamDef;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  if (CK_TOP == p_OP->eCard()) {

    for (Kid xProjection(this); xProjection.bEach(CK_EXPR); /**/) {
      Card_Expr *p_expr = xProjection.p_expr();

      if (p_expr->pSub_as())
        sColNm = p_expr->pSub_as()->szSymbol(SF_NONE);
      else
        sColNm = p_expr->pGetTerm(0)->szSymbol(SF_NONE);

      p_key->vX();

#ifndef Z_ITEMISED_NAMES
      s32 sNm_modified;
      sNm_modified.vSprint("C%lld_%s"_Stz,
                           xProjection.i_Child(),
                           sColNm.azChar());
      sColNm = sNm_modified;
#endif

      eColType = p_expr->eColumnType();
      pStreamDef->vAddColumn(sColNm, eColType, C_UNKNOWN, C_UNKNOWN);
    }

    return pStreamDef;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  i8 i_EmittedCol = 1;
  UNUSED(i_EmittedCol);
  if (CK_PROJECT == p_OP->eCard()) {
    for (Kid xProjection(this); xProjection.bEach(CK_EXPR); i_EmittedCol++) {
      Card_Expr *p_expr = xProjection.p_expr();

      if (p_expr->pSub_as())
        sColNm = p_expr->pSub_as()->szSymbol(SF_NONE);
      else
        sColNm = p_expr->pGetTerm(0)->szSymbol(SF_NONE);

#ifndef Z_ITEMISED_NAMES
      s32 sNm_modified;
      sNm_modified.vSprint("C%lld_%s"_Stz,
                           xProjection.i_Child(),
                           sColNm.azChar());
      sColNm = sNm_modified;
#endif

      eColType = p_expr->eColumnType();
      pStreamDef->vAddColumn(sColNm, eColType, C_UNKNOWN, C_UNKNOWN);

    }

    return pStreamDef;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


  i_EmittedCol = 1;
  if (CK_SCAN == p_OP->eCard()) {
    for (Kid xProjection(this); xProjection.bEach(CK_EXPR); i_EmittedCol++) {
      Card_Expr *p_expr = xProjection.p_expr();

      if (p_expr->pSub_as())
        sColNm = p_expr->pSub_as()->szSymbol(SF_NONE);
      else
        sColNm = p_expr->pGetTerm(0)->szSymbol(SF_NONE);

#ifndef Z_ITEMISED_NAMES
      s32 sNm_modified;
      sNm_modified.vSprint("C%lld_%s"_Stz,
                           xProjection.i_Child(),
                           sColNm.azChar());
      sColNm = sNm_modified;
#endif

      eColType = p_expr->eColumnType();
      pStreamDef->vAddColumn(sColNm, eColType, C_UNKNOWN, C_UNKNOWN);
    }

    return pStreamDef;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  RASSERT(CK_GBYAGG != p_OP->eCard());
  i_EmittedCol = 1;


  for (Kid xProjection(this); xProjection.bEach(CK_EXPR); i_EmittedCol++) {
    Card_Expr *p_expr = xProjection.p_expr();

    if (CK_PROJECT == p_OP->eCard()) {
      if (p_expr->pSub_as())
        sColNm = p_expr->pSub_as()->szSymbol(SF_NONE);
      else
        sColNm = p_expr->pGetTerm(0)->szSymbol(SF_NONE);

      sColNm_mapped = sColNm;

    }
    else
    if (CK_SORT == p_OP->eCard()) {
      if (p_expr->pSub_as())
        sColNm = p_expr->pSub_as()->szSymbol(SF_NONE);
      else
        sColNm = p_expr->pGetTerm(0)->szSymbol(SF_NONE);

#ifndef Z_ITEMISED_NAMES
      s32 sNm_modified;
      sNm_modified.vSprint("C%lld_%s"_Stz,
                           xProjection.i_Child(),
                           sColNm.azChar());
      sColNm = sNm_modified;
#endif

      sColNm_mapped = sColNm;

    }
    else {
      if (p_expr->pSub_as())
        sColNm = p_expr->pSub_as()->szSymbol(SF_NONE);
      else
        sColNm = p_expr->pGetTerm(0)->szSymbol(SF_NONE);

#ifndef Z_ITEMISED_NAMES
      s32 sNm_modified;
      sNm_modified.vSprint("C%lld_%s"_Stz,
                           xProjection.i_Child(),
                           sColNm.azChar());
      sColNm = sNm_modified;
#endif

      sColNm_mapped = sColNm;
    }


    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // stream def column

    eColType = p_expr->eColumnType();

    pStreamDef->vAddColumn(sColNm, eColType, C_UNKNOWN, C_UNKNOWN);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // col map (gbyagg) column

    if (pColMap) {
      RASSERT(p_key);

      if (p_expr->pSymbol()->Y_bHas(P_Key))
        pColMap->vAdd(p_expr->pSymbol()->Y_iGet(P_EmittedCol),
                      false,
                      p_expr->pSymbol()->Y_iGet(P_Cell));
      else
        pColMap->vAdd(p_expr->pSymbol()->Y_iGet(P_EmittedCol),
                      true,
                      p_expr->pSymbol()->Y_iGet(P_Cell));


    }
  }

  return pStreamDef;
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void Card_Projection::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

#ifdef Z_CARD_STREAM
  RASSERT(CK_STREAM == pParent->eCard());
#endif

  vValidate_mustHave(CK_EXPR,CK_NONE);
  vValidate_notHave(CK_KEY,CK_NONE);

  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
