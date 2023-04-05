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
: Card(CK_FILTPREDLIST)
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
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* find the first cmpPred which uses given table                              */
/*----------------------------------------------------------------------------*/
Card_FiltPred* Card_FiltPredList::pFindFiltPred(char cTblSyn)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

//  CardIter xCI(this);
//  while (xCI.G_bEachMain(CK_CMPPRED)) {
//    if (xCI.G.p_cmpPred->bUsesTable(cTblSyn))
//      return xCI.G.p_cmpPred;
//  }

  return nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
