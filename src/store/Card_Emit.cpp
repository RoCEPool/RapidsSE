/*============================================================================*/
/* Card_Emit.cpp -- Emit                                                      */
/*============================================================================*/

#include "Card_Emit.hpp"

#include "Card_RpnCalc.hpp"

#include "CardIter.hpp"
#include "Card_GbyAgg.hpp"

#include "Table.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Emit, CK_t_EMIT)
__aNewFunction(Card_Emit, CN_CARD)
__vDeleteFunction(Card_Emit, CN_CARD)



/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Emit::Card_Emit(void)
: Card_EXPRLIST(CK_t_EMIT, 3)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
          
  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Emit::~Card_Emit(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (pSymbol()->Y_bHas(P_BreakPt))
    vDebugPoint();
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* resolve (right-left) projection prior to main card                         */
/*----------------------------------------------------------------------------*/
Card_Emit* Card_Emit::pResolvePriorProjection(CardIter* pCI)
{
  _static_

  Kid xKid(pCI->G.pDeck);

  xKid.vBegin_reverse();
  xKid.vReposition(pCI->G.i_MainCard);

  while (xKid.bEach_reverse()) {
    if (Card_Emit* p_emit = xKid.pChild()->pSub_emit())
      return p_emit;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* concat facets from given key                                               */
/*----------------------------------------------------------------------------*/
void Card_Emit::vConcatFacets_From_key(Card_Key* p_key)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //p_key->vDump();

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/) {

    //xKid.p_expr()->vDump(__func__ );

    if (! xKid.p_expr()->bIsScalarCol())
      continue;

    Symbol* pSymbol_expr_sole = xKid.p_expr()->pGet_Symbol(0);

    Card_Expr* p_expr_sole_key = p_key->pFind_expr_Via_term0(pSymbol_expr_sole);
    if (! p_expr_sole_key)
      continue;

    //pSymbol_expr_sole->vCastFacets(p_expr_sole_key->pSymbol());
    pSymbol_expr_sole->vConcatFacets(p_expr_sole_key->pSymbol());
  }
}

/*----------------------------------------------------------------------------*/
/* append streamDef of table to this emit                                     */
/*----------------------------------------------------------------------------*/
void Card_Emit::vAppend_streamDef_table(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pCI->G.p_tableNm = pCI->G.p_table->pSub_tableNm(C_NOT_NULL);

  Table* pTable = Card_Table::pAccess_Table(pCI->G.p_tableNm->szFacet(SF_NONE));
  StreamDef* pStreamDef = pTable->pStreamDef();

  char cTblSyn = pCI->G.p_table->pSymbol()->cSF_char();

  i8 iExpCol = pCI->G.p_emit->nExpr() + 1;

  i8 nCols = pStreamDef->nColumns();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    Column *pColumn = pStreamDef->pGetColumn(i_Col);

    Card_Expr* p_expr_Column =
        Card_Expr::pCast_expr_Column(pColumn, iExpCol++, cTblSyn, i_Col + 1);

    pCI->G.p_emit->vAppendChild(p_expr_Column);
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Emit* Card_Emit::pClone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return _pCard_Emit(pDupCardRecursive());
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Emit::vCorrect_expCol(Card_Emit* p_arriving)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/) {

    if (Card_BoolPred* p_boolPred = xKid.p_expr()->pSub_boolPred())
      p_boolPred->vCorrect_expCol(p_arriving);
    else
    if (Card_RpnCalc* p_rpnCalc = xKid.p_expr()->pSub_rpnCalc())
      p_rpnCalc->vCorrect_expCol(p_arriving);
  }
}

/*----------------------------------------------------------------------------*/
/* craft factored-column-set this projection needs for early-projects         */
/*----------------------------------------------------------------------------*/
Card_Needed* Card_Emit::pCraftRoot_needed(CardIter* pCI)
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

      if (xCI_rpnCalc.p_term()->bIsKeyCol()) {
        RASSERT(pCI->G.p_streamKey);

        Card_Expr* p_expr_key = pCI->G.p_streamKey->pGet_expr(xCI_rpnCalc.p_term()->pSymbol());
        Symbol* pSymbol_tblCol = p_expr_key->pFirst_tblCol();

        vDebugPoint();

        if (p_needed->pFind_expr_Via_term0(pSymbol_tblCol))
          continue;

        Card_Expr* p_expr_cloned = p_expr_key->pClone();
        p_expr_cloned->pSymbol()->vDeleteFacet(SF_KEY_COL);

        p_needed->vAppendChild(p_expr_cloned);
      }
      else
      if (xCI_rpnCalc.p_term()->bIsColSym()) {

        // which we haven't already added
        if (p_needed->pFind_expr_Via_term0(xCI_rpnCalc.p_term()->pSymbol()))
          continue;

        if (Card_Expr *p_expr_col = pCI->pExpr_For_col(xCI_rpnCalc.p_term(),pCI->i_Child()+1))
          p_needed->vAppendChild(p_expr_col->pClone());
      }
      else {

      }
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 2) get factored columns of key as start of needed projection

  if (pCI->G.p_OP->bIsCard(CK_o_GBYAGG) || pCI->G.p_OP->bIsCard(CK_o_ORDAGG)) {
    if ((pCI->G.p_key = pSub_key())) {
      // + projection of gbyAgg/ordAgg has a key

      Kid xCI_key(pCI->G.p_key);
      while (xCI_key.bEach(CK_EXPR,CK_RPNCALC)) {
        Kid xCI_rpnCalc(xCI_key.pSub1());
        while (xCI_rpnCalc.bEach(CK_TERM)) {
          // only interested in columns
          if (!xCI_rpnCalc.p_term()->bIsColSym())
            continue;
          // which we haven't already added
          if (p_needed->pFind_expr_Via_term0(xCI_rpnCalc.p_term()->pSymbol()))
            continue;

          if (Card_Expr *p_expr_col = pCI->pExpr_For_col(xCI_rpnCalc.p_term())) {
            Card_Expr *p_expr_cloned = p_expr_col->pClone();
            p_expr_cloned->pSymbol()->vDeleteFacet(SF_KEY_COL);
            p_expr_cloned->vDump(__func__,"CLONED"_Stz);
            p_needed->vAppendChild(p_expr_cloned);
          }
        }
      }
    }
  }


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  p_needed->vRenumberAll_expr();
  p_needed->vDump(__func__);

  return p_needed;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Emit::vDelete_streamKey(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if ((pCI->G.p_streamKey = pSub_streamKey()))
    vDeleteChild(pCI->G.p_streamKey);

  pCI->G.p_streamKey = nullptr;
}

/*----------------------------------------------------------------------------*/
/* derive the key of this projection (of running key)                         */
/*----------------------------------------------------------------------------*/
void Card_Emit::vDerive_streamKey(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (! pCI->StreamOrder.m_pStreamKey)
    return;

  i8 nKeyExpr = pCI->StreamOrder.m_pStreamKey->nChildCards();
  for (i8 i_RunKeyExpr = 0; i_RunKeyExpr < nKeyExpr; i_RunKeyExpr++) {
    Card_Expr* p_expr_runkey = pCI->StreamOrder.m_pStreamKey->pGet_expr(i_RunKeyExpr);

    Card_Term* p_term_runkey = p_expr_runkey->pGet_term(0);
    Card_Expr* p_expr_prjCol = pFind_expr_Via_term0(p_term_runkey->pSymbol());

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
void Card_Emit::vEnsureTypeId_expCol(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_key = pCI->G.p_OP->pSub_key();

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/)
    xKid.p_expr()->vEnsureTypeId_expCol(pCI);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Emit::pFindExprOfSymbol(Symbol* pSymbol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/)
    if (xKid.p_expr()->bHasTerm(pSymbol))
      return xKid.p_expr();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* is this a compound projection?                                             */
/*----------------------------------------------------------------------------*/
bool Card_Emit::bIsCompound(void)
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
bool Card_Emit::bIsLiterals(void)
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
/* map columns of projection onto cells                                       */
/*----------------------------------------------------------------------------*/
void Card_Emit::vMapColumns(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  i8 i_Cell = 0;
  i8 i_Cell_key;
  i8 i_Cell_payload = 1;
  i8 i_EmittedCol = 0;
  i8 nExpr_key = 0;

  for (Kid xProjection(pCI->G.p_emit); xProjection.bEach(CK_EXPR); /**/) {
    Card_Expr *p_expr_projection = xProjection.p_expr();

    i_EmittedCol++;

    if (p_expr_projection->bIsExpressionOfKey()) {

      nExpr_key++;

      Symbol *pSymbol_keyCol = p_expr_projection->pFirst_keyCol();
      Card_Map* p_map_keyCell = pCI->G.p_keyCells->pGetMap(pSymbol_keyCol);
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

    if (Symbol* pSymbol_expCol_projection = p_expr_projection->pFirst_expCol()) {
      i_Cell_payload = pSymbol_expCol_projection->iSF_ordinal() - nExpr_key;
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Emit::vPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  UNUSED(iDepth);

  s32 sName;

  sName.vSprint("%s%lld]"_Stz,
                Stz::szDots(iDepth - 1),
                m_i_Child);

  sName.vSprintCat("%s"_Stz,
                   szCardNm());

  pRibbon->vSprintSnippet(false,"%c%c%s %p"_Stz,
                          cMark1(),
                          cMark2(),
                          sName.szStz(),
                          ((char *) this) - (char *) AM::SYSARGS::aRegionAddr());

}

/*----------------------------------------------------------------------------*/
/* propagate this projection                                                  */
/*----------------------------------------------------------------------------*/
Card_Emit* Card_Emit::pPropagate(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Emit *p_emit_propagated = new Card_Emit;

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/) {

    Card_Expr *p_expr_propagated = xKid.p_expr()->pPropagate();
    p_emit_propagated->vAppendChild(p_expr_propagated);
  }

  p_emit_propagated->vRenumberAll_expr();

  return p_emit_propagated;
}


/*----------------------------------------------------------------------------*/
/* return a reduced copy of this projection                                   */
/*----------------------------------------------------------------------------*/
Card_Emit* Card_Emit::pReducedCopy(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Emit* p_emitted_reduced = new Card_Emit;

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
Card_Expr* Card_Emit::pResolve_expr(Card_Term* p_term)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if (p_term->pSymbol()->bIs_Exp_Col()) {
    Card_Expr *p_expr_prior = this->pGet_expr(p_term->pSymbol());
    return p_expr_prior;
  }
  else
  if (p_term->pSymbol()->bIs_Tbl_Col()) {
    Card_Expr *p_expr_prior = this->pFind_expr_Via_term0(p_term->pSymbol());
    return p_expr_prior;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* short form                                                                 */
/*----------------------------------------------------------------------------*/
void Card_Emit::vShortForm(String* pString)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pString->vCopy("e:"_Stz);
  Card_EXPRLIST::vShortForm(pString);
  //pString->vConcat("}"_Stz);

}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void Card_Emit::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  RASSERT(CK_s_STREAM == pParent->eCard());

  vValidate_mustHave(CK_EXPR,CK_NONE);
  vValidate_notHave(CK_s_KEY, CK_NONE);

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
