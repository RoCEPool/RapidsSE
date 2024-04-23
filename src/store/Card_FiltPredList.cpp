/*============================================================================*/
/* Card_FiltPredList.cpp -- Table List                                        */
/*============================================================================*/

#include "Card_FiltPredList.hpp"

#include "CardIter.hpp"

#include "Card_Predicate.hpp"
#include "Card_RlnConj.hpp"
#include "Card_Deck.hpp"
#include "Card_Filter.hpp"
#include "Card_Term.hpp"

#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_FiltPredList,CK_FILTPREDLIST)
__aNewFunction(Card_FiltPredList,CN_CARD)
__vDeleteFunction(Card_FiltPredList,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_FiltPredList::Card_FiltPredList(void)
: Card_symbol(CK_FILTPREDLIST, C_UNKNOWN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_FiltPredList::~Card_FiltPredList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_FiltPredList* Card_FiltPredList::pExtractList(Card_Deck* p_deck)
{
  _static_

  i8 nFilters = 0;

  auto bLambda = [&nFilters] (Card* p_card) {

    if (!_pCard_RlnConj(p_card)->bIsJoinConj())
      nFilters++;

    return C_CONTINUE_RECURSION;
  };

  Card_FiltPredList* p_filtPredList = new Card_FiltPredList;

  Kid xDeck(p_deck);
  while (xDeck.bEach(CK_s_TABLE, CK_o_FILTER)) {

    Card_Filter* p_filter = _pCard_Filter(xDeck.pSub1());

    Card_Predicate* p_predicate = p_filter->pSub_predicate();

    nFilters = 0;
    p_predicate->bLambdaWalk(CK_RLNCONJ, bLambda);

    if (nFilters) {
      Card_FiltPred* p_filtPred = new Card_FiltPred;
      p_filtPred->SYM(SF_TBL_SYN, xDeck.p_table()->szFacet(SF_TBL_NM));
      p_filtPred->pSymbol()->Y_vSet(P_Count,nFilters);
      p_filtPredList->vAppendChild(p_filtPred);
    }
  }

  return p_filtPredList;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* find the first cmpPred which uses given table                              */
/*----------------------------------------------------------------------------*/
i8 Card_FiltPredList::nFiltersFor(char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Kid xCI_filtPred(this);
  while (xCI_filtPred.bEach()) {
    Card_FiltPred* p_filtPred = _pCard_FiltPred(xCI_filtPred.pChild());

    if (p_filtPred->pSymbol()->cTblSyn() == cTblSyn) {
      i8 nFilter = p_filtPred->pSymbol()->Y_iGet(P_Count);
      return nFilter;
    }
  }

  return 0;

}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_FiltPredList::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  UNUSED(pParent);
  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);
  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
