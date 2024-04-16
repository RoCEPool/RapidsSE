/*============================================================================*/
/* Card_Emit.cpp -- Emit                                                      */
/*============================================================================*/

#include "Card_Emit.hpp"


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
: Card_EXPRLIST(CK_t_EMIT, 2)
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
Card_Emit* Card_Emit::pResolvePrior_emit(CardIter* pCI)
{
  _static_

  Kid xDeck(pCI->E.p_deck);

  xDeck.vBegin_reverse();
  xDeck.vReposition(pCI->E.i_Op);

  while (xDeck.bEach_reverse()) {
    if (Card_Emit* p_emit = xDeck.pChild()->pSub_emit())
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

  pCI->E.p_key = pCI->E.p_Op->pSub_key();

  for (Kid xEmit(this); xEmit.bEach(CK_EXPR); /**/) {
    pCI->E.p_expr = xEmit.p_expr();
    pCI->E.p_expr->vApplyCast(pCI,/*??*/p_Dic_expr);
  }

  pCI->E.p_expr = nullptr;
}

/*----------------------------------------------------------------------------*/
/* apply facets from given key                                                */
/*----------------------------------------------------------------------------*/
void Card_Emit::vApplyFacets(CardIter *pCI, Card_Key* p_key)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xEmit(this); xEmit.bEach(CK_EXPR); /**/) {
    if (! xEmit.p_expr()->bIsScalarCol())
      continue;

    Symbol* pSym_expr_emit = xEmit.p_expr()->pGet_Symbol(0);
    Card_Expr* p_expr_key = p_key->pDecode_scalar_expr_Via_symbol(pCI,pSym_expr_emit);
    if (! p_expr_key)
      continue;

    pSym_expr_emit->vApplyFacets(p_expr_key->pSymbol());

#ifdef Z_NO_EXPR_TBLCOL
    pSym_expr_emit->vDeleteFacet(SF_TBL_COL);
#endif
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
  RASSERT(((Card_OP*)pCI->E.p_Op)->bIsEarlyRoot());

  if (8 == pCI->m_pBench->iBenchNo() && 54 == pCI->m_pBench->nPrintBoard()) {
//    pCI->E.p_Op->vDump(__func__);
//    this->vDump(__func__);
    vDebugPoint();
  }

  pCI->E.p_key = pCI->E.p_Op->pSub_key();

  Card_Needed* p_needed = new Card_Needed;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 1) append the referenced col's in rpnCalc of this projection into needed projection

  for (Kid xEmit(this); xEmit.bEach(CK_EXPR); /**/) {
    for (pCI->vBeginTarget(xEmit.p_expr(),CK_TERM); pCI->bEachTarget(); /**/) {
      Card_Term *p_term = pCI->E.xTarget.p_term();
      Symbol *pSym_term = p_term->pSymbol();

      // only interested in columns
      if (! pSym_term->bIs_Col())
        continue;

      // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

      if (pSym_term->bHasFacet(SF_KEY_COL)) {
        RASSERT(pCI->E.p_key);

        i8 i_Key = pSym_term->iTo_ordinal(SF_KEY_COL) - 1;
        Card_Expr *p_expr_key = pCI->E.p_key->pGet_expr(i_Key);
        Symbol *pSym_tblCol = p_expr_key->pFirst_tblCol(pCI);
        //p_needed->vDump(__func__);
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
        else
        if (p_expr_key->pHead_symbol()->bHasFacet(SF_TBL_COL)) {
          vDebugPoint();
        }
        else {
          pCI->E.p_Op->vDump(__func__);
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

  RASSERT(((Card_OP*)pCI->E.p_Op)->bIsEarlyRoot());

  if (8 == pCI->m_pBench->iBenchNo() && 56 == pCI->m_pBench->nPrintBoard()) {
    pCI->E.p_Op->vDump(__func__);
    //this->vDump(__func__);
    vDebugPoint();
  }

  Card_Needed *p_needed = new Card_Needed;

  for (Kid xEmit(this); xEmit.bEach(CK_EXPR); /**/) {
    for (pCI->vBeginTarget(xEmit.p_expr(),CK_TERM); pCI->bEachTarget(); /**/) {
      Card_Term *p_term = pCI->E.xTarget.p_term();
      Symbol *pSym_term = p_term->pSymbol();

      if (! pSym_term->bIs_Col())
        continue;

      if (pSym_term->bHasFacet(SF_KEY_COL)) {
        RASSERT(!pSym_term->bHasFacet(SF_NMD_COL));
        RASSERT(!pSym_term->bHasFacet(SF_ITM_COL));
        if (pSym_term->bHasFacet(SF_EXP_COL)) {
          RASSERT(pCI->E.p_key->pFind_expr_Matches(SF_KEY_COL, pSym_term) ==
                  pCI->E.p_key->pFind_expr_Matches_ignoring(pSym_term, SF_EXP_COL));
          vDebugPoint();
        }
        else
        if (pSym_term->bHasFacet(SF_TBL_COL)) {
          RASSERT(pCI->E.p_key->pFind_expr_Matches(SF_KEY_COL, pSym_term) ==
                  pCI->E.p_key->pFind_expr_Matches_ignoring(pSym_term, SF_TBL_COL));
          vDebugPoint();
        }
        else {
          RASSERT(pSym_term->bSingleFaceted(SF_KEY_COL));
          RASSERT(pCI->E.p_key->pFind_expr_key_Matches(pCI,pSym_term));
        }
        RASSERT(pCI->E.p_key->pFind_expr_key_Matches(pCI,SF_KEY_COL, pSym_term));
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
/* derive the key of this projection (of running key)                         */
/*----------------------------------------------------------------------------*/
void Card_Emit::vDerive_streamKey(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (pCI->E.p_Op->bIsCard(CK_o_PROJECT)) {
    vDebugPoint();
  }
  if (! pCI->StreamOrder.m_pStreamKey)
    return;

  //pCI->StreamOrder.m_pStreamKey->vDump(__func__);

//Z_KEY_DIRECTION
   for (pCI->vBeginTarget(pCI->StreamOrder.m_pStreamKey,CK_EXPR); pCI->bEachTarget(); /**/) {
     Card_Expr* p_expr_runkey = pCI->E.p_expr;
     Card_Term* p_term_runkey = p_expr_runkey->pGet_term(0);
     Card_Expr* p_expr_prjCol = pFind_expr_Matches(p_term_runkey->pSymbol());
     if (p_expr_prjCol && 1 == p_expr_prjCol->nChildCards()) {
       pCI->E.p_streamKey->vAdd_key_expr_BasedOn_expr(pCI, p_expr_prjCol, true, true);
     }
   }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Emit::pFindExprOfSymbol(CardIter* pCI, Symbol* pSymbol)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xEmit(this); xEmit.bEach(CK_EXPR); /**/)
    if (xEmit.p_expr()->bHasTerm(pCI,pSymbol))
      return xEmit.p_expr();

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get a particular expr                                                      */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Emit::pGet_expr(i8 i_Element)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Expr* p_expr = pGet_expr_z(i_Element);

  return p_expr;
}

/*----------------------------------------------------------------------------*/
/* induct this arriving emit                                                  */
/*----------------------------------------------------------------------------*/
Card_Emit* Card_Emit::pInduct(CardIter* pCI) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Emit *p_emit_inducted = new Card_Emit;

  for (Kid xEmit(this); xEmit.bEach(CK_EXPR); /**/) {
    Card_Expr *p_expr_arriving = xEmit.p_expr();

    // exclude not needed expressions
    if (pCI->E.p_needed)
      if (! pCI->E.p_needed->bNeeded_expr(p_expr_arriving))
        continue;

    Card_Expr *p_expr_inducted = p_expr_arriving->pInduct(pCI,xEmit.i_Child()+1);
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

  pCI->E.p_tableNm = pCI->E.p_table->pSub_tableNm(C_NOT_NULL);

  Table* pTable = Card_Table::pAccess_Table(pCI->E.p_tableNm->szFacet(SF_TBL_NM));
  StreamDef* pStreamDef = pTable->pStreamDef();

  char cTblSyn = pCI->E.p_table->pSymbol()->cTblSyn();

  i8 iExpCol = pCI->E.p_emit->nExpr() + 1;

  i8 nCols = pStreamDef->nColumns();
  for (i8 i_Col = 0; i_Col < nCols; i_Col++) {
    Column *pColumn = pStreamDef->pGetColumn(i_Col);

    Card_Expr* p_expr_nmdCol =
        Card_Expr::pCraft_expr_of_nmdCol(pColumn, iExpCol++, cTblSyn, i_Col + 1);

    pCI->E.p_emit->vAppendChild(p_expr_nmdCol);
  }
}
/*----------------------------------------------------------------------------*/
/* is this a compound projection?                                             */
/*----------------------------------------------------------------------------*/
bool Card_Emit::bIsCompound(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  for (Kid xEmit(this); xEmit.bEach(CK_EXPR); /**/)
    if (xEmit.p_expr()->bIsCompound())
      return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this a literals projection?                                             */
/*----------------------------------------------------------------------------*/
bool Card_Emit::bIsLiterals(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  bool bLiterals = true;

  for (Kid xEmit(this); xEmit.bEach(CK_EXPR); /**/)
    if (! xEmit.p_expr()->bIsLiterals(pCI)) {
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

  for (Kid xEmit(pCI->E.p_emit); xEmit.bEach(CK_EXPR); /**/) {
    Card_Expr *p_expr_emit = xEmit.p_expr();

    i_EmittedCol++;

    if (p_expr_emit->bIsKeyCol()) {

      nExpr_key++;

      Symbol *pSymbol_keyCol = p_expr_emit->pFirst_keyCol(pCI);
      Card_Map* p_map_keyCell = pCI->E.p_keyCells->pGet_map(pSymbol_keyCol);
      p_map_keyCell->pSymbol()->Y_vSet(P_EmittedCol, i_EmittedCol);

      i_Cell_key = pSymbol_keyCol->iTo_ordinal(SF_KEY_COL);
      p_expr_emit->pSymbol()->Y_vSet(P_EmittedCol, i_EmittedCol);
      p_expr_emit->pSymbol()->Y_vSet(P_Key);
      p_expr_emit->pSymbol()->Y_vSet(P_Cell, i_Cell_key);

      continue;
    }

    i_Cell++;

    pCI->vAppendNewChild(pCI->E.p_payloadCells, CK_MAP, p_expr_emit->pSymbol());
    pCI->E.p_map->pSymbol()->Y_vSet(P_Cell, i_Cell);
    pCI->E.p_map->pSymbol()->Y_vSet(P_EmittedCol, i_EmittedCol);

    if (Symbol* pSymbol_expCol_projection = p_expr_emit->pFirst_expCol(pCI)) {
      i_Cell_payload = pSymbol_expCol_projection->iTo_ordinal(SF_EXP_COL) - nExpr_key;
      p_expr_emit->pSymbol()->Y_vSet(P_EmittedCol, i_EmittedCol);
      p_expr_emit->pSymbol()->Y_vSet(P_Payload);
      p_expr_emit->pSymbol()->Y_vSet(P_Cell, i_Cell_payload++);
    }
    else {
      p_expr_emit->pSymbol()->Y_vSet(P_EmittedCol, i_EmittedCol);
      p_expr_emit->pSymbol()->Y_vSet(P_Payload);
      p_expr_emit->pSymbol()->Y_vSet(P_Cell, i_Cell_payload++);
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

  sName.vSprint("%s%lld]emit"_Stz,
                Stz::szDots(iDepth - 1),
                m_i_Child);

  pRibbon->vSprintSnippet(false,"%c%c%s #%lld"_Stz,
                          cMark1(),
                          cMark2(),
                          sName.szStz(),
                          m_iSequence);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Emit::vPush_foldExpr_Into_emit(CardIter* pCI, Card_Expr* p_expr_fold)
{
  _static_

  RASSERT(CT_NONE != p_expr_fold->eCTVal());

  pCI->E.p_expr = Card_Expr::pCraft_expr_Of_term(p_expr_fold->pSub_term());
  pCI->E.p_expr->pSymbol()->vApplyFacet(p_expr_fold->pSymbol()->aFacet(SF_SQY_RSL));
  pCI->E.p_expr->pSymbol()->Y_vSet(P_Injected);

  Symbol xSymbol_KeyCol(SF_KEY_COL,1);
  pCI->E.p_expr->pGet_term0()->pSymbol()->vApplyFacets(&xSymbol_KeyCol);

  vPushChild(pCI->E.p_expr);
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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Emit::vUpdate_expCol(CardIter* pCI, Card_Emit* p_arriving, bool bAlt)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  UNUSED(bAlt);

  for (Kid xEmit(this); xEmit.bEach(CK_EXPR); /**/) {
    Card_Expr *p_expr = xEmit.p_expr();
    p_expr->vUpdate_expCol(pCI, p_arriving);
  }
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void Card_Emit::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if (! pSub_expr())
    pCI->PRINTBOARD_dbg();

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
