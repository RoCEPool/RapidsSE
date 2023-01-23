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
  while (Card* pMainCard = xCI.pEachChild()) {
    if (Card_Filter *p_filter = pMainCard->pSub_filter()) {
      Card_BoolCalc *p_boolCalc = p_filter->pSub_boolCalc();
      p_joinPredList->vConcat(p_boolCalc);
    }
  }

  return p_joinPredList;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_JoinPredList::vConcat(Card_BoolCalc* p_boolCalc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  // --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

  auto bConcat = [this](Card *pCard) {
    Card_CmpPred* p_cmpPred = _pCard_CmpPred(pCard);
    if (! p_cmpPred->sz()->bSame("=="_Stz)) //RAYMOND CAN DO BETTER!
      return true;

    Card_Term* p_term_0 = _pCard_Term(p_cmpPred->pGetChild(0));
    Card_Term* p_term_1 = _pCard_Term(p_cmpPred->pGetChild(1));

    if ( ! ( (*p_term_0->szValue()->azChar() == '$') &&
             (*p_term_1->szValue()->azChar() == '$') ) )
      return true;

    Card_CmpPred* p_cmpPred_new = _pCard_CmpPred(p_cmpPred->pDupCardAndSubCards());
    this->vAppend(p_cmpPred_new);

    return true;
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
      return (xCI.G.p_cmpPred;
  }

  return nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
