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
Card_Projection::Card_Projection(void)
: Card_ELIST(CK_PROJECTION)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
          
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Projection::Card_Projection(eCK eCard)
: Card_ELIST(eCard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

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

  CardIter xCI(pCI);
  xCI.vSet(pCI->G.i_MainCard);

  while (xCI.G_bEachMain_reverse()) {
    if (Card_Projection* p_projection = xCI.G.p_MainCard->pSub_projection())
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
void Card_Projection::vAdjust(Card_Key* p_key)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //p_key->vDump();

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/) {

    //xKid.p_expr()->vDump();

    if (! xKid.p_expr()->bIsSoleCol())
      continue;

    Symbol* pSymbol_expr_sole = xKid.p_expr()->pGetSymbol(0);

    Card_Expr* p_expr_sole_key = p_key->pFindExpr(pSymbol_expr_sole);
    if (! p_expr_sole_key)
      continue;

    pSymbol_expr_sole->vSet(p_expr_sole_key->pSymbol());
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

  pCI->G.p_key = pSub_key();

  Card_Needed* p_needed = new Card_Needed;




  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 1) append the referenced col's in rpnCalc of this projection into needed projection

  CardIter xCI_projection(this);
  while (xCI_projection.G_bEachMainWithSub(CK_EXPR,CK_RPNCALC)) {

    xCI_projection.G.p_rpnCalc->vDump();
    CardIter xCI_rpnCalc(xCI_projection.G.p_rpnCalc);
    while (xCI_rpnCalc.G_bEachMain(CK_TERM)) {
      // only interested in columns

      if (xCI_rpnCalc.G.p_term->bIsKeyOdn()) {
        RASSERT(pCI->G.p_key);

        Card_Expr* p_expr_key = pCI->G.p_key->pDecodeExpr(xCI_rpnCalc.G.p_term->pSymbol());
        p_expr_key->vDump();
        Symbol* pSymbol_tblCol = p_expr_key->pFirstTblCol();

        vDebugPoint();

        if (p_needed->pFindExpr(pSymbol_tblCol))
          continue;

        p_needed->vAppend(p_expr_key->pClone());
      }
      else
      if (xCI_rpnCalc.G.p_term->bIsValOdn()) {

        // which we haven't already added
        if (p_needed->pFindExpr(xCI_rpnCalc.G.p_term->pSymbol()))
          continue;

        if (Card_Expr *p_expr_col = pCI->pExpr_For_col(xCI_rpnCalc.G.p_term))
          p_needed->vAppend(p_expr_col->pClone());
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

      CardIter xCI_key(pCI->G.p_key);
      while (xCI_key.G_bEachMainWithSub(CK_EXPR,CK_RPNCALC)) {
        CardIter xCI_rpnCalc(xCI_key.G.p_rpnCalc);
        while (xCI_rpnCalc.G_bEachMain(CK_TERM)) {
          // only interested in columns
          if (!xCI_rpnCalc.G.p_term->bIsValOdn())
            continue;
          // which we haven't already added
          if (p_needed->pFindExpr(xCI_rpnCalc.G.p_term->pSymbol()))
            continue;

          if (Card_Expr *p_expr_col = pCI->pExpr_For_col(xCI_rpnCalc.G.p_term))
            p_needed->vAppend(p_expr_col->pClone());
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
//Card_Needed* Card_Projection::pCraftRoot_needed(CardIter* pCI)
//{
//  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
//
//  // this projection will be in an aggreg, gbyAgg or ordAgg
//  RASSERT(pCI->G.p_OP->bIsEarlyRoot());
//
//  Card_Needed* p_needed = new Card_Needed;
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // 1) get factored columns of key as start of needed projection
//
//  if (pCI->G.p_OP->bIsCard(CK_GBYAGG) || pCI->G.p_OP->bIsCard(CK_ORDAGG)) {
//    if ((pCI->G.p_key = pSub_key())) {
//      // + projection of gbyAgg/ordAgg has a key
//
//      pCI->G.p_key->vFactorAll_col_IntoDistinct_expr(pCI);
//      p_needed->vTransferChildren(pCI->G.p_subSet);
//      DELETE_CARD(pCI->G.p_subSet);
//    }
//  }
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // 2) append the referenced col's in rpnCalc of this projection into needed projection
//
//  CardIter xCI_projection(this);
//  while (xCI_projection.G_bEachMainWithSub(CK_EXPR,CK_RPNCALC)) {
//
//    CardIter xCI_rpnCalc(xCI_projection.G.p_rpnCalc);
//    while (xCI_rpnCalc.G_bEachMain(CK_TERM)) {
//      // only interested in columns
//      if (!xCI_rpnCalc.G.p_term->bIsValOdn())
//        continue;
//      // which we havent already added
//      if (p_needed->pFindExpr(xCI_rpnCalc.G.p_term->pSymbol()))
//        continue;
//
//      if (Card_Expr *p_expr_col = pCI->pExpr_For_col(xCI_rpnCalc.G.p_term))
//        p_needed->vAppend(p_expr_col->pClone());
//    }
//  }
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//
//  p_needed->vRenumberAll_expr();
//  //p_needed->vDump();
//
//  return p_needed;
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Projection::vDedupe_as(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  for (Kid xProjection(this); xProjection.bEach(CK_EXPR); /**/) {
    Card_As* p_as = xProjection.p_expr()->pSub_as();

    Symbol xDeduped;
    xDeduped.vSprint("C%lld_%s"_Stz,
                     xProjection.i_Child(),
                     p_as->pSymbol()->szSymbol());
    p_as->pSymbol()->vSet(&xDeduped);


    //StrLib::vRemoveHead(xDeduped.szSymbol()->azChar(),'_');
    //vTrace("%s\n"_Stz,xDeduped.szSymbol());

  }

  vDebugPoint();
}
/*----------------------------------------------------------------------------*/
//void Card_Projection::vDedupe_as(void)
//{
//  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
//
//  for (Kid xProjection(this); xProjection.bEach(CK_EXPR); /**/) {
//    Card_As* p_as = xProjection.p_expr()->pSub_as();
//
//    i8 nBefore = nAsBefore(p_as);
//
//    if (! nBefore)
//      continue;
//
//    Symbol xDeduped;
//    xDeduped.vSprint("%s__%lld"_Stz,p_as->pSymbol()->szSymbol(),nBefore+1);
//    p_as->pSymbol()->vSet(&xDeduped);
//  }
//}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Projection::vDeletePrjKey(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if ((pCI->G.p_key = pSub_key()))
    vDeleteChild(pCI->G.p_key);

  pCI->G.p_key = nullptr;
}

/*----------------------------------------------------------------------------*/
/* derive the key of this projection (of running key)                         */
/*----------------------------------------------------------------------------*/
void Card_Projection::vDerivePrjKey(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Key* p_key_projection = pCI->G.p_projection->pSub_key();

  if (! pCI->StreamOrder.m_pKey_running)
    return;

  i8 nKeyExpr = pCI->StreamOrder.m_pKey_running->nChildCards();
  for (i8 i_RunKeyExpr = 0; i_RunKeyExpr < nKeyExpr; i_RunKeyExpr++) {
    Card_Expr* p_expr_runkey = pCI->StreamOrder.m_pKey_running->pGetExpr(i_RunKeyExpr);

    Card_Term* p_term_runkey = p_expr_runkey->pGetTerm(0);
    Card_Expr* p_expr_prjCol = pFindExpr(p_term_runkey->pSymbol());

    if (p_expr_prjCol) {

      // annotation or direction in passing
      if (p_expr_runkey->pSub_direction() && ! p_expr_prjCol->pSub_direction()) {
        p_expr_prjCol->vAppend(p_expr_runkey->pSub_direction()->pDupCardRecursive());
      }

      pCI->G.p_key->vAdd_key_expr_BasedOn_expr(p_expr_prjCol,false);
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
Card_Projection* Card_Projection::pPropagate(void) {
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_Projection *p_projection_propagated = new Card_Projection;

  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/) {

    Card_Expr *p_expr_propagated = xKid.p_expr()->pPropagate();
    p_projection_propagated->vAppend(p_expr_propagated);
  }

  return p_projection_propagated;
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
Card_Projection* Card_Projection::pReducedCopy(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Projection* p_projection_reduced = new Card_Projection;

  i8 i_Expr = 0;
  for (Kid xKid(this); xKid.bEach(CK_EXPR); /**/) {
    Card_Expr* p_expr_copy = xKid.p_expr()->pReducedCopy(i_Expr++);
    p_projection_reduced->vAppend(p_expr_copy);
  }

  return p_projection_reduced;
}

/*----------------------------------------------------------------------------*/
/* as review                                                                  */
/*----------------------------------------------------------------------------*/
void Card_Projection::vReviewAs(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nPrjExpr = nChildCards();
  for (i8 i_PrjExpr = 0; i_PrjExpr < nPrjExpr; i_PrjExpr++) {

    // get <gbyAgg>/<projection>/<expr>
    Card_Expr *p_expr_prj = pGetExpr(i_PrjExpr);

    //Stz* szPrjCol_prj = p_expr_prj->szGetOdn();
    Card_Term* p_term_prj = p_expr_prj->pGetTerm(0);
    if ( (! p_term_prj || (!p_term_prj->pSymbol()->bIsKeyOrdinal())) )
      continue;

    // the <gbyAgg>/<projection>/<expr> is a single projected column

    // get corresponding <gbyAgg>/<key>/<expr>
    Card_Term* pTerm_prj = p_expr_prj->pGetTerm(0);
    Card_Expr* p_expr_key = pCI->G.p_key->pDecodeExpr(pTerm_prj->pSymbol());

    if (! p_expr_key)
      continue;
    else
    if (p_expr_key->pSub_as()) {

      if (p_expr_prj->pSub_as()) {
        p_expr_key->vDeleteChild(pCI->G.p_as);
        p_expr_key->vAppend(p_expr_prj->pSub_as()->pDupCardRecursive());
      }
      else {
        p_expr_prj->vAppend(p_expr_key->pSub_as()->pDupCardRecursive());
      }
    }
    else
    if (p_expr_prj->pSub_as()) {
      p_expr_key->vAppend(p_expr_prj->pSub_as()->pDupCardRecursive());
    }

    vDebugPoint();
  }

}

/*----------------------------------------------------------------------------*/
/* convert to streamDef                                                       */
/*----------------------------------------------------------------------------*/
StreamDef* Card_Projection::pToStreamDef(ColMap* pColMap,
                                         Card_Key* p_key,
                                         eCK eOP,
                                         Card* p_OP)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  StreamDef* pStreamDef = new StreamDef;

  s32 sColNm;
  s32 sColNm_mapped;
  eCT eColType;
  //Symbol* pSymbol;



  i8 i_EmittedCol = 1;


  if (CK_GBYAGG == eOP) {

    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

    i8 i_KeyCol = C_UNKNOWN;
    i8 nKeyCol = 0;

    bool bFound_key = true;
    while (bFound_key) {
      bFound_key = false;
      for (Kid xProjection(this); xProjection.bEach(CK_EXPR); /**/) {
        Card_Expr *p_expr = xProjection.p_expr();

        if (! p_expr->bIsExpressionOfKey())
          continue;
        if (!p_expr->pSymbol()->Y_bHas(P_EmittedCol))
          continue;
        if (p_expr->pSymbol()->Y_iGet(P_EmittedCol) != i_EmittedCol)
          continue;

        bFound_key = true;

        if (p_expr->pSub_as())
          sColNm = p_expr->pSub_as()->szSymbol();
        else
          sColNm = p_expr->pGetTerm(0)->szSymbol();

        p_key->vX();

        vDebugPoint();

        s32 sNm_modified;
        sNm_modified.vSprint("C%lld_%s"_Stz,
                             xProjection.i_Child(),
                             sColNm.azChar());
        sColNm = sNm_modified;
        sColNm_mapped = sColNm;

        p_expr->vDump();

        if (Symbol *pSymbol_key = p_expr->pFirstKeyCol()) {
          vTrace("%s\n"_Stz, pSymbol_key->szSymbol());

          i_KeyCol = StrLib::iGleanInt(pSymbol_key->szSymbol()->azChar());
          if (i_KeyCol) {
            vTrace("%s %lld\n"_Stz, pSymbol_key->szSymbol(), i_KeyCol);


            sNm_modified.vSprint("C%lld_COL%lld"_Stz,
                                 xProjection.i_Child(),
                                 i_KeyCol);
            sColNm_mapped = sNm_modified;
          }
        } else if (Symbol *pSymbol_stm = p_expr->pFirstStmCol()) {
          vTrace("%s\n"_Stz, pSymbol_stm->szSymbol());


          i_KeyCol = StrLib::iGleanInt(pSymbol_stm->szSymbol()->azChar());
          if (i_KeyCol) {
            vTrace("%s %lld\n"_Stz, pSymbol_stm->szSymbol(), i_KeyCol);


            sNm_modified.vSprint("C%lld_COL%lld"_Stz,
                                 xProjection.i_Child(),
                                 i_KeyCol);
            sColNm_mapped = sNm_modified;
          }
        } else {
          i_KeyCol = i_EmittedCol;
        }

        vDebugPoint();

        eColType = p_expr->eColumnType();
        pStreamDef->vAddColumn(sColNm, eColType, C_UNKNOWN, C_UNKNOWN);

        pColMap->vAdd(i_KeyCol, false, i_EmittedCol);

        i_EmittedCol++;
        nKeyCol++;
        break;
      }
    }


    // -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

    i8 i_PayLoadCol = 1;
    bool bFound_payload = true;
    while (bFound_payload) {
      bFound_payload = false;
      for (Kid xProjection(this); xProjection.bEach(CK_EXPR); /**/) {
        Card_Expr *p_expr = xProjection.p_expr();

        if (p_expr->bIsExpressionOfKey())
          continue;
        if (!p_expr->pSymbol()->Y_bHas(P_EmittedCol))
          continue;
        if (p_expr->pSymbol()->Y_iGet(P_EmittedCol) != i_EmittedCol)
          continue;

        bFound_payload = true;

        if (p_expr->pSub_as())
          sColNm = p_expr->pSub_as()->szSymbol();
        else
          sColNm = p_expr->pGetTerm(0)->szSymbol();

        p_key->vX();

        vDebugPoint();

        s32 sNm_modified;
        sNm_modified.vSprint("C%lld_%s"_Stz,
                             xProjection.i_Child(),
                             sColNm.azChar());
        sColNm = sNm_modified;
        sColNm_mapped = sColNm;

        p_expr->vDump();

        if (Symbol *pSymbol_stm = p_expr->pFirstStmCol()) {
          vTrace("%s\n"_Stz, pSymbol_stm->szSymbol());


          i_PayLoadCol = StrLib::iGleanInt(pSymbol_stm->szSymbol()->azChar());
          if (i_PayLoadCol) {
            i_PayLoadCol = i_PayLoadCol - nKeyCol;

            vTrace("%s %lld\n"_Stz, pSymbol_stm->szSymbol(), i_PayLoadCol);


            sNm_modified.vSprint("C%lld_COL%lld"_Stz,
                                 xProjection.i_Child(),
                                 i_PayLoadCol);
            sColNm_mapped = sNm_modified;
          }
        } else {
          i_PayLoadCol = i_EmittedCol - nKeyCol;
        }


        vDebugPoint();

        eColType = p_expr->eColumnType();
        pStreamDef->vAddColumn(sColNm, eColType, C_UNKNOWN, C_UNKNOWN);

        pColMap->vAdd(i_EmittedCol, true, i_PayLoadCol);
        //pColMap->vDump();
        i_EmittedCol++;

        break;
      }
    }


    return pStreamDef;
  }



  i8 i_Cell;

  for (Kid xProjection(this); xProjection.bEach(CK_EXPR); i_EmittedCol++) {
    Card_Expr *p_expr = xProjection.p_expr();
    i_Cell = C_UNKNOWN;

    if (CK_PROJECT == eOP) {
      if (p_expr->pSub_as())
        sColNm = p_expr->pSub_as()->szSymbol();
      else
        sColNm = p_expr->pGetTerm(0)->szSymbol();

      sColNm_mapped = sColNm;

    }
    else
    if (CK_GBYAGG == eOP) {

      if (p_expr->pSub_as())
        sColNm = p_expr->pSub_as()->szSymbol();
      else
        sColNm = p_expr->pGetTerm(0)->szSymbol();

      p_key->vX();

      vDebugPoint();

      s32 sNm_modified;
      sNm_modified.vSprint("C%lld_%s"_Stz,
                           xProjection.i_Child(),
                           sColNm.azChar());
      sColNm = sNm_modified;
      sColNm_mapped = sColNm;

      p_expr->vDump();

      if (Symbol* pSymbol_key = p_expr->pFirstKeyCol()) {
        vTrace("%s\n"_Stz, pSymbol_key->szSymbol());

        i8 i_Child;
        Card_Expr* p_expr_key =
          _pCard_Expr(p_key->pFindChild(CK_EXPR, pSymbol_key->szSymbol(), i_Child));

        Symbol* pSymbol_stm = p_expr_key->pFirstStmCol();

        i_Cell = StrLib::iGleanInt(pSymbol_stm->szSymbol()->azChar());
        if (i_Cell) {
          vTrace("%s %lld\n"_Stz, pSymbol_stm->szSymbol(), i_Cell);


          sNm_modified.vSprint("C%lld_COL%lld"_Stz,
                               xProjection.i_Child(),
                               i_Cell);
          sColNm_mapped = sNm_modified;
        }
      }
      else
      if (Symbol* pSymbol_stm = p_expr->pFirstStmCol()) {
        vTrace("%s\n"_Stz, pSymbol_stm->szSymbol());


        i_Cell = StrLib::iGleanInt(pSymbol_stm->szSymbol()->azChar());
        if (i_Cell) {
          vTrace("%s %lld\n"_Stz, pSymbol_stm->szSymbol(), i_Cell);


          sNm_modified.vSprint("C%lld_COL%lld"_Stz,
                               xProjection.i_Child(),
                               i_Cell);
          sColNm_mapped = sNm_modified;
        }
      }
      else {
        i_Cell = i_EmittedCol;
      }

      vDebugPoint();

    }
    else
    if (CK_SORT == eOP) {
      if (p_expr->pSub_as())
        sColNm = p_expr->pSub_as()->szSymbol();
      else
        sColNm = p_expr->pGetTerm(0)->szSymbol();

      s32 sNm_modified;
      sNm_modified.vSprint("C%lld_%s"_Stz,
                           xProjection.i_Child(),
                           sColNm.azChar());
      sColNm = sNm_modified;
      sColNm_mapped = sColNm;

      i_Cell = i_EmittedCol;
    }
    else {
      if (p_expr->pSub_as())
        sColNm = p_expr->pSub_as()->szSymbol();
      else
        sColNm = p_expr->pGetTerm(0)->szSymbol();

      s32 sNm_modified;
      sNm_modified.vSprint("C%lld_%s"_Stz,
                           xProjection.i_Child(),
                           sColNm.azChar());
      sColNm = sNm_modified;
      sColNm_mapped = sColNm;
    }


    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // stream def column

    eColType = p_expr->eColumnType();

    //vTrace("pToStreamDef: %s %s\n"_Stz,sColNm.szStz(),aCT[eColType]);

    pStreamDef->vAddColumn(sColNm, eColType, C_UNKNOWN, C_UNKNOWN);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // col map (gbyagg) column

    if (pColMap) {
      RASSERT(p_key);

//      bool bPayload = false;
//      if (CK_TOP != eOP && ! p_expr->bIsExpressionOfKey())
//        bPayload = true;
//
//      vTrace("ColMap addCol: %lld <- %s %lld [%s]\n"_Stz,
//             i_EmittedCol,
//             bPayload ? "payload cell" : "key cell",
//             i_Cell,
//             p_OP->szBearing());
//
//      pColMap->vAddCol(i_EmittedCol, bPayload, i_Cell);

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

  vValidate_mustHave(CK_EXPR,CK_NONE);

  pSub_expr()->vValidate(pCI,iDepth+1,this);
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* number of similar as's befor given as (by symbol)                          */
/*----------------------------------------------------------------------------*/
i8 Card_Projection::nAsBefore(Card_As* p_as)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Stz* szSep = "__"_Stz;
  i8 nBefore = 0;

  for (Kid xProjection(this); xProjection.bEach(CK_EXPR); /**/) {
    Card_As *p_as_test = xProjection.p_expr()->pSub_as();

    if (p_as_test == p_as)
      return nBefore;

    Stz* szArg  = p_as->szSymbol();
    Stz* szTest = p_as_test->szSymbol();

    if (StrLib::bSimilar(szArg->azChar(),szTest->azChar(),szSep->azChar()))
      nBefore++;
  }

  NEVER_REACH
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
