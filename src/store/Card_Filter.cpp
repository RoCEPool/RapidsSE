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
: Card_COND(CK_FILTER,C_UNKNOWN)
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
/* cull non-used bounds                                                       */
/*----------------------------------------------------------------------------*/
void Card_Filter::vCullUsedBounds(CardIter* pCI) {

  _static_

  while (pCI->G_bEachMain()) {
    if ((pCI->G.p_filter = pCI->G.p_MainCard->pSub_filter()))
      pCI->G.p_filter->nCullPred(MARK_AT);
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

  if (! pCI->G.p_MainCard->pSub_projection())
    pCI->G.p_MainCard->vDump(__func__);

  RASSERT(pCI->G.p_MainCard->pSub_projection());

  pCI->G.p_MainCard->vRemoveChild(pCI->G.p_filter);

  pCI->G.p_filter->vAppend(pCI->G.p_MainCard->pSub_projection()->pClone());

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
  UNUSED(pCI);
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
void Card_Filter::vCorrectStmOdn(Card_Projection* p_projection_prior)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  if (Card_BoolCalc* p_boolCalc = pSub_boolCalc())
    p_boolCalc->vCorrectStmOdn(p_projection_prior);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
i8 Card_Filter::nCullPred(Stz* szWithMark)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_BoolCalc* p_boolCalc = pSub_boolCalc();
  i8 nCulled = p_boolCalc->nCullPred(szWithMark);

  return nCulled;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Filter::vEarlyProject_add_transients(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vAddTransients(pCI);
}


/*----------------------------------------------------------------------------*/
/* early project                                                              */
/*----------------------------------------------------------------------------*/
void Card_Filter::vEarlyProject_recurse_update(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

 // pCI->vPrintBoard();

  // save the current projection needed
  //   we can restore to this save if projection needed get extended
  Card_Needed* p_needed_save = pCI->Early.m_pNeeded_later->pClone();

  // check for additional/temporary transients
  vEarlyProject_add_transients(pCI);

  Card_OP::vEarlyProject_backstep(pCI);

  /// ... arrived back ... ... ... ... ... ... ... ... ... ... ... ... ... ...

 // pCI->vPrintBoard();

  vUpdate_projection(pCI);

 // pCI->vPrintBoard();

  // restore needed
  delete pCI->Early.m_pNeeded_later;
  pCI->Early.m_pNeeded_later = p_needed_save;

 // pCI->vPrintBoard();

  vDebugPoint();
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
void Card_Filter::vUpdate_projection(CardIter* pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  if ((pCI->G.p_projection = pCI->G.p_filter->pSub_projection())) {

    pCI->G.p_filter->vDeleteChild(pCI->G.p_projection);
    pCI->G.p_filter->vAppend(pCI->Prior.m_pProjection->pClone());

  }
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Filter::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValidate_mustHave(CK_BOOLCALC,CK_NONE);

  if (iDepth > 2) {
   // pCI->vPrintBoard();
    vValidationFailed("Filter is at depth %lld"_Stz, iDepth);
  }

  if (CK_DECK == pParent->eCard()) {
    vValidate_mustHave(CK_LEVEL, CK_PROJECTION, CK_NONE);
  }

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
