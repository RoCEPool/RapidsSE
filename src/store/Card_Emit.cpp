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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Emit::vApplyCast(CardIter *pCI, Card_Emit* p_Dic_expr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  pCI->G.p_key = pCI->G.p_OP->pSub_key();

  for (Kid xEmit(this); xEmit.bEach(CK_EXPR); /**/) {
    pCI->G.p_expr = xEmit.p_expr();
    pCI->G.p_expr->vApplyCast(pCI,/*??*/p_Dic_expr);
  }

  pCI->G.p_expr = nullptr;
}

/*----------------------------------------------------------------------------*/
/* apply facets from given key                                                */
/*----------------------------------------------------------------------------*/
void Card_Emit::vApplyFacets(Card_Key* p_key)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xEmit(this); xEmit.bEach(CK_EXPR); /**/) {
    if (! xEmit.p_expr()->bIsScalarCol())
      continue;

    Symbol* pSym_expr_emit = xEmit.p_expr()->pGet_Symbol(0);
    Card_Expr* p_expr_key = p_key->pDecode_scalar_expr_Via_symbol(pSym_expr_emit);
    if (! p_expr_key)
      continue;

    pSym_expr_emit->vApplyFacets(p_expr_key->pSymbol());
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
/* craft factored-column-set this projection needs for early-projects         */
/*----------------------------------------------------------------------------*/
Card_Needed* Card_Emit::pCraftRoot_needed(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  // this projection will be in an aggreg, gbyAgg or ordAgg
  RASSERT(pCI->G.p_OP->bIsEarlyRoot());

  //this->vDump(__func__);

  pCI->G.p_key = pCI->G.p_OP->pSub_key();

  Card_Needed* p_needed = new Card_Needed;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 1) append the referenced col's in rpnCalc of this projection into needed projection

  for (Kid xEmit(this); xEmit.bEach(CK_EXPR, CK_RPNCALC); /**/) {
    for (Kid xRpnCalc(xEmit.pSub1()); xRpnCalc.bEach(CK_TERM); /**/) {
      Card_Term *p_term = xRpnCalc.p_term();
      Symbol *pSym_term = p_term->pSymbol();

      // only interested in columns
      if (! pSym_term->bIs_Col())
        continue;

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

      if (pSym_term->bHasFacet(SF_KEY_COL)) {
        RASSERT(pCI->G.p_key);

        i8 i_Key = pSym_term->iTo_ordinal(SF_KEY_COL) - 1;
        Card_Expr *p_expr_key = pCI->G.p_key->pGet_expr(i_Key);

//        if (p_expr_key->bIsScalarCol()) {
//          p_expr_key->pSymbol()->vDump();
//          p_expr_key->pSymbol()->vApplyFacets(pSym_term);
//          p_expr_key->pSymbol()->vDump();
//          vDebugPoint();
//        }

//        pSym_term->vDump();
//        p_expr_key->vDump(__func__);

//        RASSERT(p_expr_key->pSymbol()->bHasFacet(SF_NMD_COL) ||
//                p_expr_key->pSymbol()->bHasFacet(SF_TBL_COL));


        Symbol *pSym_tblCol = p_expr_key->pFirst_tblCol();
        if (p_needed->pFind_expr_Matches(pSym_tblCol))
          continue;

        if (p_expr_key->pSymbol()->bHasFacet(SF_EXP_COL)) {
          if (p_expr_key->pSymbol()->bHasFacet(SF_TBL_COL)) {
            NEVER_REACH
          }
          else {
            vDebugPoint();
          }
        }
        else
        if (p_expr_key->pSymbol()->bHasFacet(SF_TBL_COL)) {
          vDebugPoint();
        }
        else {
          pCI->G.p_OP->vDump(__func__);
          p_expr_key->pSymbol()->vDump();
          NEVER_REACH
        }


        Card_Expr *p_expr_cloned = p_expr_key->pClone();
        p_expr_cloned->pSymbol()->vDeleteFacet(SF_KEY_COL);
        p_needed->vAppendChild(p_expr_cloned);
        continue;
      }

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

//      if (xEmit.p_expr()->bIsScalarCol())
//        xEmit.p_expr()->pSymbol()->vApplyFacets(pSym_term);

      // which we haven't already added
      if (p_needed->pFind_expr_Matches(pSym_term))
        continue;

      RASSERT(pSym_term->bHasFacet(SF_EXP_COL) ||
              pSym_term->bHasFacet(SF_TBL_COL));

      Card_Expr *p_expr_col = pCI->pBackSearch_expr_For_col(pSym_term,
                                                            pCI->i_Child() + 1);

//      pSym_term->vDump();
//      p_expr_col->vDump(__func__);

      RASSERT(p_expr_col->pSymbol()->bHasFacet(SF_NMD_COL));

      p_needed->vAppendChild(p_expr_col->pClone());
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  p_needed->vRenumberAll_expr();

  return p_needed;
}

/*----------------------------------------------------------------------------*/
/* craft factored-column-set this projection needs for early-projects         */
/*----------------------------------------------------------------------------*/
Card_Needed* Card_Emit::pCraft_needed_root(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  RASSERT(pCI->G.p_OP->bIsEarlyRoot());

  Card_Needed *p_needed = new Card_Needed;

  for (Kid xEmit(this); xEmit.bEach(CK_EXPR, CK_RPNCALC); /**/) {
    for (Kid xRpnCalc(xEmit.pSub1()); xRpnCalc.bEach(CK_TERM); /**/) {
      Card_Term *p_term = xRpnCalc.p_term();
      Symbol *pSym_term = p_term->pSymbol();

      if (! pSym_term->bIs_Col())
        continue;

      if (pSym_term->bHasFacet(SF_KEY_COL)) {
        RASSERT(!pSym_term->bHasFacet(SF_NMD_COL));
        RASSERT(!pSym_term->bHasFacet(SF_ITM_COL));
        if (pSym_term->bHasFacet(SF_EXP_COL)) {
          RASSERT(pCI->G.p_key->pFind_expr_Matches(SF_KEY_COL,pSym_term) ==
                    pCI->G.p_key->pFind_expr_Matches_ignoring(pSym_term,SF_EXP_COL));
          vDebugPoint();
        }
        else
        if (pSym_term->bHasFacet(SF_TBL_COL)) {
          RASSERT(pCI->G.p_key->pFind_expr_Matches(SF_KEY_COL,pSym_term) ==
                  pCI->G.p_key->pFind_expr_Matches_ignoring(pSym_term,SF_TBL_COL));
          vDebugPoint();
        }
        else {
          RASSERT(pSym_term->bSingleFaceted(SF_KEY_COL));
          RASSERT(pCI->G.p_key->pFind_expr_Matches(pSym_term));
          vDebugPoint();
        }
        RASSERT(pCI->G.p_key->pFind_expr_Matches(SF_KEY_COL,pSym_term));
        vDebugPoint();
      }
      else
      if (pSym_term->bHasFacet(SF_EXP_COL)) {
        if (pSym_term->bHasFacet(SF_TBL_COL)) {
          RASSERT(pSym_term->bMultiFaceted(2));
          vDebugPoint();
        }
        else {
          RASSERT(pSym_term->bSingleFaceted(SF_EXP_COL));
          vDebugPoint();
        }
      }
      else
      if (pSym_term->bHasFacet(SF_TBL_COL)) {
        if (pSym_term->bHasFacet(SF_NMD_COL)) {
          RASSERT(pSym_term->bMultiFaceted(2));
          vDebugPoint();
        }
        else {
          RASSERT(pSym_term->bSingleFaceted(SF_TBL_COL));
          vDebugPoint();
        }
      }
      else {
        NEVER_REACH
      }

      // ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ?
      // todo:
      // ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ? ?
    }
  }

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
    Card_Expr* p_expr_prjCol = pFind_expr_Matches(p_term_runkey->pSymbol());
    if (p_expr_prjCol && 1 == p_expr_prjCol->pSub_rpnCalc()->nChildCards()) {

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
Card_Expr* Card_Emit::pFindExprOfSymbol(Symbol* pSymbol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/)
    if (xKid.p_expr()->bHasTerm(pSymbol))
      return xKid.p_expr();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* induct this arriving emit                                                  */
/*----------------------------------------------------------------------------*/
Card_Emit* Card_Emit::pInduct(CardIter* pCI) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Emit *p_emit_inducted = new Card_Emit;

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/) {
    Card_Expr *p_expr_arriving = xKid.p_expr();

    // exclude not needed expressions
    if (pCI->G.p_needed)
      if (! pCI->G.p_needed->bNeeded_expr(p_expr_arriving))
        continue;

    Card_Expr *p_expr_inducted = p_expr_arriving->pInduct(xKid.i_Child()+1);
    p_emit_inducted->vAppendChild(p_expr_inducted);
  }

  p_emit_inducted->vRenumberAll_expr();

  return p_emit_inducted;
}

/*----------------------------------------------------------------------------*/
/* inject streamDef of table into this emit                                   */
/*----------------------------------------------------------------------------*/
void Card_Emit::vInject_expr_nmdCol_table(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  pCI->G.p_tableNm = pCI->G.p_table->pSub_tableNm(C_NOT_NULL);

  Table* pTable = Card_Table::pAccess_Table(pCI->G.p_tableNm->szFacet(SF_TBL_NM));
  StreamDef* pStreamDef = pTable->pStreamDef();

  char cTblSyn = pCI->G.p_table->pSymbol()->cTblSyn();

  i8 iExpCol = pCI->G.p_emit->nExpr() + 1;

  i8 nCols = pStreamDef->nColumns();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    Column *pColumn = pStreamDef->pGetColumn(i_Col);

    Card_Expr* p_expr_nmdCol =
        Card_Expr::pCraft_expr_nmdCol(pColumn, iExpCol++, cTblSyn, i_Col + 1);

    pCI->G.p_emit->vAppendChild(p_expr_nmdCol);
  }
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

      i_Cell_key = pSymbol_keyCol->iTo_ordinal(SF_KEY_COL);
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
      i_Cell_payload = pSymbol_expCol_projection->iTo_ordinal(SF_EXP_COL) - nExpr_key;
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

//  sName.vSprint("%s%lld]"_Stz,
//                Stz::szDots(iDepth - 1),
//                m_i_Child);
//
//  sName.vSprintCat("%s"_Stz,
//                   szCardNm());

  sName.vSprint("%s%lld]out"_Stz,
                Stz::szDots(iDepth - 1),
                m_i_Child);

  pRibbon->vSprintSnippet(false,"%c%c%s %llx %lld"_Stz,
                          cMark1(),
                          cMark2(),
                          sName.szStz(),
                          ((char *) this) - (char *) AM::SYSARGS::aRegionAddr(),
                          m_nFunctionCount);

}

/*----------------------------------------------------------------------------*/
/* resolve prior expr of given term (find right-to-left)                      */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Emit::pResolve_expr(Card_Term* p_term)
{
  _precondition_( _MAGIC_, _FOOTER_);

  if (p_term->pSymbol()->bHasFacet(SF_EXP_COL) || p_term->pSymbol()->bHasFacet(SF_TBL_COL)) {
    Card_Expr *p_expr_prior = this->pFind_expr_Matches(p_term->pSymbol());
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Emit::vUpdate_expCol(Card_Emit* p_arriving, bool bAlt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/) {

    if (Card_BoolPred* p_boolPred = xKid.p_expr()->pSub_boolPred())
      p_boolPred->vUpdate_expCol(p_arriving,bAlt);
    else
    if (Card_RpnCalc* p_rpnCalc = xKid.p_expr()->pSub_rpnCalc())
      p_rpnCalc->vUpdate_expCol(p_arriving);
  }
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void Card_Emit::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  RASSERT(CK_s_STREAM == pParent->eCard());

  if (! pSub_expr())
    pCI->vPrintBoard(__func__,__LINE__,true);

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
