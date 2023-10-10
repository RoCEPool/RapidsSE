/*============================================================================*/
/* Card_Emitted.cpp -- Projection                                          */
/*============================================================================*/

#include "Card_Emitted.hpp"

#include "Card_RpnCalc.hpp"

#include "CardIter.hpp"
#include "Card_GbyAgg.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Emitted, CK_EMITTED)
__aNewFunction(Card_Emitted, CN_CARD)
__vDeleteFunction(Card_Emitted, CN_CARD)



/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Emitted::Card_Emitted(i8 iCrossRef)
: Card_EXPRLIST(CK_EMITTED, 3)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
          
  _checkInitialised_(CN_CARD);

  SYM(SF_XRF_NO,iCrossRef);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Emitted::~Card_Emitted(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* resolve (right-left) projection prior to main card                         */
/*----------------------------------------------------------------------------*/
Card_Emitted* Card_Emitted::pResolvePriorProjection(CardIter* pCI)
{
  _static_

  Kid xKid(pCI->G.pDeck);

  xKid.vBegin_reverse();
  xKid.vReposition(pCI->G.i_MainCard);

  while (xKid.bEach_reverse()) {
    if (Card_Emitted* p_emitted = xKid.pChild()->pSub_emitted())
      return p_emitted;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* adjust this projection given key                                           */
/*----------------------------------------------------------------------------*/
void Card_Emitted::vAdjust_Wrt_key(Card_Key* p_key)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //p_key->vDump();

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/) {

    //xKid.p_expr()->vDump(__func__ );

    if (! xKid.p_expr()->bIsScalarCol())
      continue;

    Symbol* pSymbol_expr_sole = xKid.p_expr()->pGetSymbol(0);

    Card_Expr* p_expr_sole_key = p_key->pFind_expr_By_symbol(pSymbol_expr_sole);
    if (! p_expr_sole_key)
      continue;

    pSymbol_expr_sole->vFacets(p_expr_sole_key->pSymbol());
  }
}

/*----------------------------------------------------------------------------*/
/* review synthetic as -- e.g. can COL1/$K1 -> l_returnFlag/$K1               */
/*----------------------------------------------------------------------------*/
void Card_Emitted::vAs_Review(CardIter* pCI)
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Emitted* Card_Emitted::pClone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return _pCard_Emitted(pDupCardRecursive());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Emitted::vCorrectStmOdn(Card_Emitted* p_emitted_prior)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/) {

    if (Card_BoolCalc* p_boolCalc = xKid.p_expr()->pSub_boolCalc())
      p_boolCalc->vCorrectStmOdn(p_emitted_prior);
    else
    if (Card_RpnCalc* p_rpnCalc = xKid.p_expr()->pSub_rpnCalc())
      p_rpnCalc->vCorrectStmOdn(p_emitted_prior);
  }

}

/*----------------------------------------------------------------------------*/
/* craft factored-column-set this projection needs for early-projects         */
/*----------------------------------------------------------------------------*/
Card_Needed* Card_Emitted::pCraftRoot_needed(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  // this projection will be in an aggreg, gbyAgg or ordAgg
  RASSERT(pCI->G.p_OP->bIsEarlyRoot());

  pCI->G.p_streamKey = pSub_streamKey();

  Card_Needed* p_needed = new Card_Needed;

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

        if (p_needed->pFind_expr_By_symbol(pSymbol_tblCol))
          continue;

        p_needed->vAppendChild(p_expr_key->pClone());
      }
      else
      if (xCI_rpnCalc.p_term()->bIsValOdn()) {

        // which we haven't already added
        if (p_needed->pFind_expr_By_symbol(xCI_rpnCalc.p_term()->pSymbol()))
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
          if (p_needed->pFind_expr_By_symbol(xCI_rpnCalc.p_term()->pSymbol()))
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
void Card_Emitted::vDedupe_as(void)
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
void Card_Emitted::vDelete_streamKey(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if ((pCI->G.p_streamKey = pSub_streamKey()))
    vDeleteChild(pCI->G.p_streamKey);

  pCI->G.p_streamKey = nullptr;
}

/*----------------------------------------------------------------------------*/
/* derive the key of this projection (of running key)                         */
/*----------------------------------------------------------------------------*/
void Card_Emitted::vDerive_streamKey(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! pCI->StreamOrder.m_pStreamKey)
    return;

  i8 nKeyExpr = pCI->StreamOrder.m_pStreamKey->nChildCards();
  for (i8 i_RunKeyExpr = 0; i_RunKeyExpr < nKeyExpr; i_RunKeyExpr++) {
    Card_Expr* p_expr_runkey = pCI->StreamOrder.m_pStreamKey->pGetExpr(i_RunKeyExpr);

    Card_Term* p_term_runkey = p_expr_runkey->pGetTerm(0);
    Card_Expr* p_expr_prjCol = pFind_expr_By_symbol(p_term_runkey->pSymbol());

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
void Card_Emitted::vEnsureTypes_For_stmCol(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/)
    xKid.p_expr()->vEnsureTypes_For_stmCol(pCI);
}

/*----------------------------------------------------------------------------*/
/* map columns of projection onto cells                                       */
/*----------------------------------------------------------------------------*/
void Card_Emitted::vMapColumns(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 i_Cell = 0;
  i8 i_Cell_key;
  i8 i_Cell_payload = 1;
  i8 i_EmittedCol = 0;
  i8 nExpr_key = 0;

  for (Kid xProjection(pCI->G.p_emitted); xProjection.bEach(CK_EXPR); /**/) {
    Card_Expr *p_expr_projection = xProjection.p_expr();

    i_EmittedCol++;

    if (p_expr_projection->bIsExpressionOfKey()) {

      nExpr_key++;

      Symbol *pSymbol_keyCol = p_expr_projection->pFirstKeyCol();
      Card_Map* p_map_keyCell = pCI->G.p_keyCells->pDecodeMap(pSymbol_keyCol);
      p_map_keyCell->pSymbol()->Y_vSet(P_EmittedCol, i_EmittedCol);

      i_Cell_key = pSymbol_keyCol->iSF_ordinal();
      p_expr_projection->pSymbol()->Y_vSet(P_EmittedCol, i_EmittedCol);
      p_expr_projection->pSymbol()->Y_vSet(P_Key);
      p_expr_projection->pSymbol()->Y_vSet(P_Cell, i_Cell_key);

      continue;
    }

    i_Cell++;

    pCI->G_vAppendNewChild(pCI->G.p_payloadCells, CK_MAP,p_expr_projection->pSymbol());
    pCI->G.p_map->pSymbol()->Y_vSet(P_Cell, i_Cell);
    pCI->G.p_map->pSymbol()->Y_vSet(P_EmittedCol, i_EmittedCol);

    if (Symbol* pSymbol_stmCol_projection = p_expr_projection->pFirstStmCol()) {
      i_Cell_payload = pSymbol_stmCol_projection->iSF_ordinal() - nExpr_key;
      p_expr_projection->pSymbol()->Y_vSet(P_EmittedCol, i_EmittedCol);
      p_expr_projection->pSymbol()->Y_vSet(P_Payload);
      p_expr_projection->pSymbol()->Y_vSet(P_Cell, i_Cell_payload++);
    }
    else {
      p_expr_projection->pSymbol()->Y_vSet(P_EmittedCol, i_EmittedCol);
      p_expr_projection->pSymbol()->Y_vSet(P_Payload);
      p_expr_projection->pSymbol()->Y_vSet(P_Cell, i_Cell_payload++);
    }
  }
}

/*----------------------------------------------------------------------------*/
/* propagate this projection                                                  */
/*----------------------------------------------------------------------------*/
Card_Emitted* Card_Emitted::pPropagate(CardIter* pCI) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Emitted *p_emitted_propagated = new Card_Emitted(pCI->iXform());

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/) {

    Card_Expr *p_expr_propagated = xKid.p_expr()->pPropagate();
    p_emitted_propagated->vAppendChild(p_expr_propagated);
  }

  p_emitted_propagated->vRenumberAll_expr();

  return p_emitted_propagated;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Emitted::pFindExprOfAs(Symbol* pSymbol)
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
Card_Expr* Card_Emitted::pFindExprOfSymbol(Symbol* pSymbol)
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
bool Card_Emitted::bHasTerm(Symbol* pSymbol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/)
    if (xKid.p_expr()->bHasTerm(pSymbol))
      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a compound projection?                                             */
/*----------------------------------------------------------------------------*/
bool Card_Emitted::bIsCompound(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/)
    if (xKid.p_expr()->bIsCompound())
      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a literals projection?                                             */
/*----------------------------------------------------------------------------*/
bool Card_Emitted::bIsLiterals(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool bLiterals = true;

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/)
    if (! xKid.p_expr()->bIsLiterals()) {
      bLiterals = false;
      break;
    }

  return bLiterals;
}

/*----------------------------------------------------------------------------*/
/* return a reduced copy of this projection                                   */
/*----------------------------------------------------------------------------*/
Card_Emitted* Card_Emitted::pReducedCopy(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Emitted* p_emitted_reduced = new Card_Emitted(pCI->iXform());

  i8 i_Expr = 0;
  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/) {
    Card_Expr* p_expr_copy = xKid.p_expr()->pReducedCopy(i_Expr++);
    p_emitted_reduced->vAppendChild(p_expr_copy);
  }

  return p_emitted_reduced;
}

/*----------------------------------------------------------------------------*/
/* resolve prior expr of given term (find right-to-left)                      */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Emitted::pResolve_expr(Card_Term* p_term)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if (p_term->pSymbol()->bIs_Stm_Col()) {
    Card_Expr *p_expr_prior = this->pDecodeExpr(p_term->pSymbol());
    return p_expr_prior;
  }
  else
  if (p_term->pSymbol()->bIs_Tbl_Col()) {
    Card_Expr *p_expr_prior = this->pFind_expr_By_symbol(p_term->pSymbol());
    return p_expr_prior;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* short form                                                                 */
/*----------------------------------------------------------------------------*/
void Card_Emitted::vShortForm(String* pString)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pString->vCopy("e:"_Stz);
  Card_EXPRLIST::vShortForm(pString);
  //pString->vConcat("}"_Stz);

}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void Card_Emitted::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  RASSERT(CK_STREAM == pParent->eCard());

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
