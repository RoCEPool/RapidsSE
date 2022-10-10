/*============================================================================*/
/* Card_CrossPtnJoin.cpp -- Cross-Partition                                       */
/*============================================================================*/

#include "Card_CrossPtnJoin.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_CrossPtnJoin, CK_CROSSPTNJOIN)
__aNewFunction(Card_CrossPtnJoin, CN_CARD)
__vDeleteFunction(Card_CrossPtnJoin, CN_CARD)

__bIsCard(STORE, Card_CrossPtnJoin, CK_CROSSPTNJOIN)
__pSubFunction_Card(STORE, CrossPtnJoin, CK_CROSSPTNJOIN)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CrossPtnJoin::Card_CrossPtnJoin(void)
: Card(CK_CROSSPTNJOIN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CrossPtnJoin::~Card_CrossPtnJoin(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
