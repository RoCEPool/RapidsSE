/*============================================================================*/
/* Card_Projection.cpp -- Stream                                                  */
/*============================================================================*/

#include "Card_Projection.hpp"

#include "Card_RpnCalc.hpp"

#include "CardIter.hpp"

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
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Projection* Card_Projection::pClone(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return _pCard_Projection(pDupCardRecursive());
}

/*----------------------------------------------------------------------------*/
/* craft factored-column-set this projection needs for early-projects         */
/*----------------------------------------------------------------------------*/
Card_Needed* Card_Projection::pCraftRoot_needed(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  // this projection will be in an aggreg, gbyAgg or ordAgg
  RASSERT(pCI->G.p_OP->bIsEarlyRoot());

  Card_Needed* p_needed = new Card_Needed;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 1) get factored columns of key as start of needed projection

  if (pCI->G.p_OP->bIsCard(CK_GBYAGG) || pCI->G.p_OP->bIsCard(CK_ORDAGG)) {
    if ((pCI->G.p_key = pSub_key())) {
      // + projection of gbyAgg/ordAgg has a key

      pCI->G.p_key->vFactorAll_col_IntoDistinct_expr(pCI);
      p_needed->vTransferChildren(pCI->G.p_subSet);
      DELETE_CARD(pCI->G.p_subSet);
    }
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 2) append the referenced col's in rpnCalc of this projection into needed projection

  CardIter xCI_projection(this);
  while (xCI_projection.G_bEachMainWithSub(CK_EXPR,CK_RPNCALC)) {

    CardIter xCI_rpnCalc(xCI_projection.G.p_rpnCalc);
    while (xCI_rpnCalc.G_bEachMain(CK_TERM)) {
      // only interested in columns
      if (!xCI_rpnCalc.G.p_term->bIsValOdn())
        continue;
      // which we havent already added
      if (p_needed->pFindExpr(xCI_rpnCalc.G.p_term->pSymbol()))
        continue;

      if (Card_Expr *p_expr_col = pCI->pExpr_For_col(xCI_rpnCalc.G.p_term))
        p_needed->vAppend(p_expr_col->pClone());
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
StreamDef* Card_Projection::pToStreamDef(ColsReq* pColMap_gbyAgg, Card_Key* p_key)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  StreamDef* pStreamDef = new StreamDef;

  i8 i_Col = 1;
  s32 sColNm;
  eCT eColumnType;

  Kid xExpr(this);
  while (xExpr.bEach(CK_EXPR)) {
    Card_Expr* p_expr = xExpr.p_expr();

    if (p_expr->pSub_as())
      sColNm = p_expr->pSub_as()->szSymbol();
    else
      sColNm = p_expr->pGetTerm(0)->szSymbol();


    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // stream def column

    eColumnType = p_expr->eColumnType();

    //vTrace("pToStreamDef: %s %s\n"_Stz,sColNm.szStz(),aCT[eColumnType]);

    pStreamDef->vAddColumn(sColNm,eColumnType,C_UNKNOWN,C_UNKNOWN);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    // col map (gbyagg) column

    if (pColMap_gbyAgg) {
      RASSERT(p_key);

      bool bPayload = false;
//    if (! p_expr->pSymbol()->bIsKeyOrdinal())
      if (! p_expr->bIsExpressionOfKey())
        bPayload = true;

      pColMap_gbyAgg->vAddCol(i_Col++,sColNm,bPayload);
    }
  }

  return pStreamDef;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Projection::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValidate_mustHave(CK_EXPR,CK_NONE);

  pSub_expr()->vValidate(pCI,iDepth+1,this);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
