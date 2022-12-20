/*============================================================================*/
/* Card_TableList.cpp -- Table List                                           */
/*============================================================================*/

#include "Card_TableList.hpp"

#include "CardIter.hpp"
#include "Card_Deck.hpp"
#include "Card_Table.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_TableList,CK_TABLELIST)
__aNewFunction(Card_TableList,CN_CARD)
__vDeleteFunction(Card_TableList,CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_TableList::Card_TableList(void)
: Card(CK_TABLELIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_TableList::~Card_TableList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_TableList* Card_TableList::pExtract(Card_Deck* p_deck)
{
  _static_

  Card_TableList* p_tableList = new Card_TableList;

  CardIter xCI(p_deck);
  while (Card* pMainCard = xCI.pEachChild())
    if (Card_Table* p_table = pMainCard->pSub_table())
      p_tableList->vAppend(p_table->pDupCardAndSubCards());

  return p_tableList;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Table* Card_TableList::pFind(char cTblSyn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  i8 nChildren = nChildCards();
  for (i8 i_Child = 0; i_Child < nChildren; i_Child++) {
    Card* pChildCard = pGetChild(i_Child);
    if (CK_TABLE != pChildCard->eCard())
      continue;

    Card_Table* p_table = _pCard_Table(pChildCard);
    char cTableSyn = p_table->cValue();
    if (cTableSyn == cTblSyn)
      return p_table;
  }

  return nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
