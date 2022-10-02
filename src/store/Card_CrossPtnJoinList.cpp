/*============================================================================*/
/* Card_CrossPtnJoinList.cpp -- Cross-Partition List                              */
/*============================================================================*/

#include "Card_CrossPtnJoinList.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_CrossPtnJoinList, CK_CROSSPTNJOINLIST)
__aNewFunction(Card_CrossPtnJoinList, CN_CARD)
__vDeleteFunction(Card_CrossPtnJoinList, CN_CARD)

__bIsCard(STORE, Card_CrossPtnJoinList, CK_CROSSPTNJOINLIST)
__pGetSubFunction_Card(STORE, CrossPtnJoinList, CK_CROSSPTNJOINLIST)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CrossPtnJoinList::Card_CrossPtnJoinList(void)
: Card(CK_CROSSPTNJOINLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CrossPtnJoinList::~Card_CrossPtnJoinList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
