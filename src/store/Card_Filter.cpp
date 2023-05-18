/*============================================================================*/
/* Card_Filter.cpp -- Filter                                                  */
/*============================================================================*/

#include "Card_Filter.hpp"

#include "Card_Expr.hpp"
#include "Card_RpnCalc.hpp"
#include "Card_BoolCalc.hpp"
#include "Card_CmpPred.hpp"
#include "Card_Term.hpp"
#include "Card_SubSet.hpp"
#include "Card_AndPred.hpp"
#include "Card_OrPred.hpp"
#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Filter,CK_FILTER)
__aNewFunction(Card_Filter,CN_CARD)
__vDeleteFunction(Card_Filter,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Filter::Card_Filter(void)
: Card_OP(CK_FILTER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Filter::~Card_Filter(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* cull non-used bounds                                                        */
/*----------------------------------------------------------------------------*/
void Card_Filter::vCullUsedBounds(CardIter* pCI) {

  _static_

  while (pCI->G_bEachMain()) {
    if ((pCI->G.p_filter = pCI->G.p_MainCard->pSub_filter()))
      pCI->G.p_filter->vCullPred(MARK_AT);
  }
}

/*----------------------------------------------------------------------------*/
/* delete empty filters                                                       */
/*----------------------------------------------------------------------------*/
void Card_Filter::vDeleteEmptyFilters(CardIter* pCI) {

  _static_

  while (pCI->G_bEachMain()) {
    if ((pCI->G.p_filter = pCI->G.p_MainCard->pSub_filter()))
      pCI->G.p_OP->vDeleteFilterIfEmpty(pCI->G.p_filter);
  }
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Filter::vIndividuate_filter(CardIter* pCI)
{
  _static_

  pCI->G.p_MainCard->vRemoveChild(pCI->G.p_filter);
  pCI->G.pDeck->vInsertAfter(pCI->G.p_filter, pCI->G.p_MainCard);

  if (! pCI->G.p_filter->pSub_level()) {
    pCI->G.p_filter->vPush(pCI->G.p_MainCard->pSub_level()->pDupCardRecursive());
  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Filter* Card_Filter::pNew(Card_Constraint* p_constraint)
{
  _static_

  Card_Filter* p_filter = new Card_Filter;

  p_filter->vAppend(p_constraint);
  return p_filter;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Filter::vAddPred(Card_PRED *p_PRED)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_BoolCalc* p_calcBool = this->pSub_boolCalc();

  if (0 == p_calcBool->nChildCards()) {
    p_calcBool->vAppend(p_PRED);
    return;
  }

  Card* p_top = p_calcBool->pPop();

  Card_AndPred* pCard_predAnd = new Card_AndPred;
  pCard_predAnd->vAppend(p_top);
  pCard_predAnd->vAppend(p_PRED);

  p_calcBool->vAppend(pCard_predAnd);
}

/*----------------------------------------------------------------------------*/
/* apply stream                                                               */
/*----------------------------------------------------------------------------*/
void Card_Filter::vComputeProjection(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* convert terms which are stmCol to tblCol                                   */
/*----------------------------------------------------------------------------*/
void Card_Filter::vConvert_stmColOdn_terms_To_tblColOdn(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if ((pCI->G.p_projection = pSub_projection()))
    pCI->G.p_projection->vConvertAll_stmOdn_To_colOdn(pCI);

//  if ((pCI->G.p_constraint = pSub_constraint()))
//    pCI->G.p_constraint->vConvertAll_stmOdn_To_colOdn(pCI);
  if (Card_BoolCalc* p_boolCalc = pSub_boolCalc())
    p_boolCalc->vConvertAll_stmOdn_To_colOdn(pCI);

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Filter::vCullPred(char cWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_BoolCalc* p_boolCalc = pSub_boolCalc();
  p_boolCalc->vCullPred(cWithMark);

//  i8 nChildren = this->nChildCards();
//  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
//    Card* pChildCard = this->pGetChild(i_Child);
//
//    eCK eCard = pChildCard->eCard();
//    if (eCard != CK_BOOLCALC)
//      continue;
//
//    Card_BoolCalc* p_boolCalc = _pCard_BoolCalc(pChildCard);
//
//    p_boolCalc->vCullPred(cWithMark);
//  }
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_Filter::bIsEmpty(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_BoolCalc* pCard_calcBool =
      _pCard_BoolCalc(this->pFindSubCard(CK_BOOLCALC));

  if (0 == pCard_calcBool->nChildCards())
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Filter::vValidate(i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
