/*============================================================================*/
/* Card_FiltPredList.cpp -- Table List                                        */
/*============================================================================*/

#include "Card_FiltPredList.hpp"

#include "CardIter.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_CmpPred.hpp"
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

    if (! _pCard_CmpPred(p_card)->bIsJoinPred())
      nFilters++;

    return C_CONTINUE_RECURSION;
  };

  Card_FiltPredList* p_filtPredList = new Card_FiltPredList;

  CardIter xCI(p_deck);
  while (xCI.G_bEachMainWithSub(CK_TABLE,CK_FILTER)) {

    xCI.G.p_boolCalc = xCI.G.p_filter->pSub_boolCalc();

    nFilters = 0;
    xCI.G.p_boolCalc->bApplyRecursive(CK_CMPPRED,bLambda);

    if (nFilters) {
      Card_FiltPred* p_filtPred = new Card_FiltPred;
      p_filtPred->SYM(SF_TBL_SYN,xCI.G.p_table->szSymbol(SF_NONE));
      p_filtPred->pSymbol()->Y_vSet(P_Count,nFilters);
      p_filtPredList->vAppend(p_filtPred);
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
Card_FiltPred* Card_FiltPredList::pFindFiltPred(char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(cTblSyn);


//  CardIter xCI(this);
//  while (xCI.G_bEachMain(CK_CMPPRED)) {
//    if (xCI.G.p_cmpPred->bUsesTable(cTblSyn))
//      return xCI.G.p_cmpPred;
//  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find the first cmpPred which uses given table                              */
/*----------------------------------------------------------------------------*/
i8 Card_FiltPredList::nFiltersFor(char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  CardIter xCI_filtPred(this);
  while (xCI_filtPred.G_bEachMain()) {
    if (xCI_filtPred.G.p_filtPred->pSymbol()->cSF_char() == cTblSyn) {
      i8 nFilter = xCI_filtPred.G.p_filtPred->pSymbol()->Y_iGet(P_Count);
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
