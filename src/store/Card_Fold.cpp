/*============================================================================*/
/* Card_Fold.cpp -- old (delineates the two halves during flattening)         */
/*============================================================================*/

#include "Card_Fold.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Fold,CK_FLAT)
__aNewFunction(Card_Fold,CN_CARD)
__vDeleteFunction(Card_Fold,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Fold::Card_Fold(void)
: Card(CK_FLAT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Fold::~Card_Fold(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Fold::vBump_where_BaseOn_foldExpr(CardIter* pCI)
{
  _static_

  pCI->G.p_boolCalc = pCI->G.p_where->pSub_boolCalc();

  i8 nBumpRight = pCI->G.p_fold->Y_iGet(P_BumpRight);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  auto bLambdaFn = [pCI,nBumpRight](Card *pCard) {

    if (MARK_AT != pCard->cMark1())
      return C_CONTINUE_RECURSION;

    pCI->G.p_roPred = _pCard_RoPred(pCard);

//    m_pTerm_0 = _pCard_Term(pCI->G.p_roPred->pGet(0));
//    m_pTerm_0->vBump_StreamColOrdinal(nBumpRight);

    pCI->m_pTerm_1 = _pCard_Term(pCI->G.p_roPred->pGet(1));
    pCI->G.p_roPred->vRemoveChild(pCI->m_pTerm_1);
    DELETE_CARD(pCI->m_pTerm_1);

    pCI->m_pTerm_1 = _pCard_Term(pCI->G.p_roPred->pGet(1));
    pCI->G.p_roPred->vRemoveChild(pCI->m_pTerm_1);
    DELETE_CARD(pCI->m_pTerm_1);

    pCI->G_vAppendNewChild(pCI->G.p_roPred, CK_TERM, "$2"_Stz);

    pCI->G_vMorphCard(pCI->G.p_roPred, CK_CMPPRED);

    return C_CONTINUE_RECURSION;
  };

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  pCI->G.p_boolCalc->bApplyRecursive(CK_ROPRED, bLambdaFn);

  pCI->G.p_fold->Y_vSet(P_BumpedRight,nBumpRight);
  pCI->G.p_fold->Y_vCut(P_BumpRight);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Fold::vMorph_to_flat(CardIter* pCI)
{
  _static_

  pCI->vRestart();
  while (pCI->G_bEachMain(CK_FOLD))
    pCI->G.p_fold->vCard(CK_FLAT);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Fold::vBump_Rightwards(CardIter* pCI, i8 nBumpBy)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  while (pCI->G_bEachMain()) {

    if (pCI->G.p_MainCard->bIsCard(CK_TABLE)) {
      continue;
    }

    if (pCI->G.p_MainCard->bIsCard(CK_WHERE)) {
      pCI->G.p_where->vBump_Rightwards(nBumpBy);
      continue;
    }

    if (pCI->G.p_MainCard->bIsCard(CK_AGGREG)) {
      pCI->G.p_aggreg->vBump_Rightwards(nBumpBy);
      break;
    }
  }

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
