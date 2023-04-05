/*============================================================================*/
/* Card_JoinPredList.cpp -- Table List                                        */
/*============================================================================*/

#include "Card_JoinPredList.hpp"

#include "CardIter.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_CmpPred.hpp"
#include "Card_Deck.hpp"
#include "Card_Filter.hpp"
#include "Card_Term.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_JoinPredList,CK_JOINPREDLIST)
__aNewFunction(Card_JoinPredList,CN_CARD)
__vDeleteFunction(Card_JoinPredList,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_JoinPredList::Card_JoinPredList(void)
: Card(CK_JOINPREDLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_JoinPredList::~Card_JoinPredList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_JoinPredList* Card_JoinPredList::pExtract(Card_Deck* p_deck)
{
  _static_

  Card_JoinPredList* p_joinPredList = new Card_JoinPredList;

  CardIter xCI(p_deck);
  while (xCI.G_bEachMain()) {
    if (! (xCI.G.p_indexList = xCI.G.p_MainCard->pSub_indexList()) )
      continue;
    if (! (xCI.G.p_bounds = xCI.G.p_MainCard->pSub_bounds()) )
      continue;
    if (! (xCI.G.p_filter = xCI.G.p_MainCard->pSub_filter()) )
      continue;

    xCI.G.p_boolCalc = xCI.G.p_filter->pSub_boolCalc();
    p_joinPredList->vConcat(xCI.G.p_boolCalc,xCI.G.p_indexList, xCI.G.p_bounds);
  }

  return p_joinPredList;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_JoinPredList::vConcat(Card_BoolCalc* p_boolCalc,
                                Card_IndexList* p_indexList, Card_Bounds* p_bounds)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

  auto bConcat = [this, p_indexList, p_bounds](Card *pCard) {
    Card_CmpPred* p_cmpPred = _pCard_CmpPred(pCard);
    if (! p_cmpPred->bIsEqOp())
      return C_CONTINUE_RECURSION;

    if (! p_indexList->bHas_cmpPred_In_bounds(p_cmpPred))
      return C_CONTINUE_RECURSION;

    Card_Term* p_term_0 = p_cmpPred->pGetTerm(0);
    Card_Term* p_term_1 = p_cmpPred->pGetTerm(1);

    if ( ! (p_term_0->bIsTblColOdn() && p_term_1->bIsTblColOdn()))
      return C_CONTINUE_RECURSION;

    Card_CmpPred* p_cmpPred_new = _pCard_CmpPred(p_cmpPred->pDupCardRecursive());
    this->vAppend(p_cmpPred_new);

    return C_CONTINUE_RECURSION;
  };

  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

  p_boolCalc->bApplyRecursive(CK_CMPPRED, bConcat);
}

/*----------------------------------------------------------------------------*/
/* find the first cmpPred which uses given table                              */
/*----------------------------------------------------------------------------*/
Card_CmpPred* Card_JoinPredList::pFindCmpPred(char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI(this);
  while (xCI.G_bEachMain(CK_CMPPRED)) {
    if (xCI.G.p_cmpPred->bUsesTable(cTblSyn))
      return xCI.G.p_cmpPred;
  }

  return nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
