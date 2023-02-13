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
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Projection::vPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);


  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_EXPR)) {
    StzList* pStzList = new StzList;

    pStzList->vAppend(xCI.G.p_expr->szValue());

    CardIter xCI_expr(xCI.G.p_expr);
    xCI_expr.G.p_as        = nullptr;
    xCI_expr.G.p_boolCalc  = nullptr;
    xCI_expr.G.p_colNm     = nullptr;
    xCI_expr.G.p_rpnCalc   = nullptr;
    xCI_expr.G.p_tblColOdn = nullptr;
    xCI_expr.G.p_type      = nullptr;

    while (xCI_expr.G_bEachMain()) {

      if ( ! (
          xCI_expr.G.p_MainCard->bIsCard(CK_AS)        ||
          xCI_expr.G.p_MainCard->bIsCard(CK_BOOLCALC)  ||
          xCI_expr.G.p_MainCard->bIsCard(CK_COLNM)     ||
          xCI_expr.G.p_MainCard->bIsCard(CK_RPNCALC)   ||
          xCI_expr.G.p_MainCard->bIsCard(CK_TBLCOLODN) ||
          xCI_expr.G.p_MainCard->bIsCard(CK_TYPE))) {
        vTrace("Unexpected %s"_Stz,xCI_expr.G.p_MainCard->szCardNm());
        NEVER_REACH
      }
    }

    if (xCI_expr.G.p_as) {
      pStzList->vAppend(xCI_expr.G.p_as->szValue());
    }
    if (xCI_expr.G.p_colNm) {
      if (! xCI_expr.G.p_as)
        pStzList->vAppend(xCI_expr.G.p_colNm->szValue());
    }
//    pRibbon->vSprintList(pStzList, 20);
//    pStzList->vRemoveAll();
//
//    pStzList->vAppend(" "_Stz);
    if (xCI_expr.G.p_tblColOdn) {
      pStzList->vAppend(xCI_expr.G.p_tblColOdn->szValue());
    }
    if (xCI_expr.G.p_type) {
      pStzList->vAppend(xCI_expr.G.p_type->szValue());
    }
    pRibbon->vSprintList(pStzList, 20);
    pStzList->vRemoveAll();

    if (xCI_expr.G.p_rpnCalc) {
      xCI_expr.G.p_rpnCalc->vOntoStzList(pStzList);
      pRibbon->vSprintList(pStzList, 20);
      pStzList->vRemoveAll();
    }

    delete pStzList;
  }




  //vPrintCard_bySettings(pRibbon, iDepth);
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
