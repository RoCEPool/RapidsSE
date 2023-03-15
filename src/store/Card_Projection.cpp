/*============================================================================*/
/* Card_Projection.cpp -- Stream                                                  */
/*============================================================================*/

#include "Card_Projection.hpp"

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
/* projection analysis (post fold)                                            */
/*----------------------------------------------------------------------------*/
void Card_Projection::vProjection_Analysis_v2(CardIter* pCI)
{
  _static_

  pCI->vRestart();
  while (pCI->G_bEachMain(CK_TABLE))
    if ((pCI->G.p_projection = pCI->G.p_table->pSub_projection()))
      pCI->G.p_table->vRemoveDeleteChild(pCI->G.p_projection);

  bool bPostFold = false;

  pCI->vRestart();
  pCI->G.p_projection = nullptr;
  while (pCI->G_bEachMain()) {
    if (CK_FOLD == pCI->G.e_MainCard) {
      bPostFold = true;
      continue;
    }

    if (CK_TABLE == pCI->G.e_MainCard) {
      pCI->G.p_table->vProjectionAnalysis(pCI);
      continue;
    }

    if ((pCI->G.p_projection = pCI->G.p_MainCard->pSub_projection()))
      pCI->G.p_projection->vRenumberAllExpr();
  }
}

/*----------------------------------------------------------------------------*/
/* resolve (right-left) projection prior to main card                         */
/*----------------------------------------------------------------------------*/
Card_Projection* Card_Projection::pResolvePriorProjection(CardIter* pCI)
{
  _static_

  CardIter xCI(pCI);
  xCI.vSet(pCI->G.i_MainCard,pCI->G.p_MainCard);

  while (Card* p_card = xCI.pEachChild_reverse()) {
    if (Card_Projection* p_projection = p_card->pSub_projection())
      return p_projection;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* as review                                                                  */
/*----------------------------------------------------------------------------*/
void Card_Projection::vAs_Review(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nPrjExpr = nChildCards();
  for (i8 i_PrjExpr = 0; i_PrjExpr < nPrjExpr; i_PrjExpr++) {
    Card_Expr *p_expr_prj = pGetExpr(i_PrjExpr);

//    if (p_expr_prj->pSub_as())
//      continue;

    Stz* szPrjCol_prj = p_expr_prj->szProjectedCol();
    if ((! szPrjCol_prj || (! Card::bIsKeyOrdinal(szPrjCol_prj))))
      continue;

    if ( (pCI->G.p_as = p_expr_prj->pSub_as()) ) {
      p_expr_prj->vRemoveDeleteChild(pCI->G.p_as);
      pCI->G.p_as = nullptr;
    }

    Card_Term* pTerm_prj = p_expr_prj->szProjectedTerm();
    Card_Expr* p_expr_key = pCI->G.p_key->pGetExpr(pTerm_prj);

    if (p_expr_key && (pCI->G.p_as = p_expr_key->pSub_as())) {
      p_expr_prj->vAppend(pCI->G.p_as->pDupCardAndSubCards());
    }

    vDebugPoint();

  }

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Projection::vDelete_key(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if ((pCI->G.p_key = pSub_key()))
    vRemoveDeleteChild(pCI->G.p_key);

  pCI->G.p_key = nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Projection::pFind_expr_By_colNm(CardIter* pCI, Stz* szColNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(this == pCI->G.p_projection);

  // within projection find the expr which has given colNm
  if (pCI->G_bFindSubCard(CK_PROJECTION, CK_EXPR, CK_COLNM, szColNm)) {
    return _pCard_Expr(pCI->m_pCard_found);
  }

  return nullptr;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Projection::pFind_expr_By_stmColOdn(CardIter* pCI, Stz* szStmColOdn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(this == pCI->G.p_projection);

  // within projection find the expr which has given colNm
  if (pCI->G_bFindSubCard(CK_PROJECTION, CK_EXPR, CK_COLNM, szStmColOdn)) {
    return _pCard_Expr(pCI->m_pCard_found);
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* Card_Projection::pFind_expr_By_tblColOdn(CardIter* pCI, Stz* szTblColOdn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  RASSERT(this == pCI->G.p_projection);

  // within projection find the expr which has given colNm
  if (pCI->G_bFindSubCard(CK_PROJECTION, CK_EXPR, CK_COLNM, szTblColOdn)) {
    return _pCard_Expr(pCI->m_pCard_found);
  }

  return nullptr;
}


/*----------------------------------------------------------------------------*/
/* derive the key projection of this projection (of running key)              */
/*----------------------------------------------------------------------------*/
void Card_Projection::vKeyProjection(CardIter *pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_Key* p_key_projection = pCI->G.p_projection->pSub_key();

  i8 nKeyExpr = pCI->m_pKey_running->nChildCards();
  for (i8 i_RunKeyExpr = 0; i_RunKeyExpr < nKeyExpr; i_RunKeyExpr++) {
    Card_Expr* p_expr_runkey = pCI->m_pKey_running->pGetExpr(i_RunKeyExpr);

    Stz* szPrjCol_runkey = p_expr_runkey->szProjectedCol();

    Card_Expr* p_expr_prjCol = pFindExpr_BasedOn_projectedCol(pCI,szPrjCol_runkey);
    if (p_expr_prjCol) {

      // annotation or direction in passing
      if (p_expr_runkey->pSub_direction() && ! p_expr_prjCol->pSub_direction()) {
        p_expr_prjCol->vAppend(p_expr_runkey->pSub_direction()->pDupCardAndSubCards());
      }

      pCI->G.p_key->vAdd_key_expr_BasedOn_expr(p_expr_prjCol,false);
    }


//    if (Card::bIsStreamColOrdinal(szPrjCol_runkey)) {
//      Card_Expr* p_expr_Projection = pFind_expr_By_stmColOdn(pCI, szPrjCol_runkey);
//      if (p_expr_Projection) {
//
//      }
//    }
//    else
//    if (Card::bIsTableColOrdinal(szPrjCol_runkey)) {
//      Card_Expr* p_expr_Projection = pFind_expr_By_tblColOdn(pCI, szPrjCol_runkey);
//      if (p_expr_Projection) {
//
//      }
//    }
  }
}

/*----------------------------------------------------------------------------*/
/* re-projection                                                              */
/*----------------------------------------------------------------------------*/
Card_Projection* Card_Projection::pReProjection(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
