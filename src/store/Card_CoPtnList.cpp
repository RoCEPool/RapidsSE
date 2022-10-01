/*============================================================================*/
/* Card_CoPtnList.cpp -- Co-Partition List                                    */
/*============================================================================*/

#include "Card_CoPtnList.hpp"

#include "Card_CoPtn.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_CoPtnList,CK_COPTNLIST)
__aNewFunction(Card_CoPtnList,CN_CARD)
__vDeleteFunction(Card_CoPtnList,CN_CARD)

__bIsCard(STORE,Card_CoPtnList,CK_COPTNLIST)
__pGetSubFunction_Card(STORE,CoPtnList,CK_COPTNLIST)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CoPtnList::Card_CoPtnList(void)
: Card(CK_COPTNLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CoPtnList::~Card_CoPtnList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_CoPtn* Card_CoPtnList::pFind_coPtn_for(Card_CoPtnPred* p_CoPtnPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  return nullptr;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
