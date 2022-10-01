/*============================================================================*/
/* Card_CrossPtnList.cpp -- Cross-Partition List                              */
/*============================================================================*/

#include "Card_CrossPtnList.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_CrossPtnList,CK_CROSSPTNLIST)
__aNewFunction(Card_CrossPtnList,CN_CARD)
__vDeleteFunction(Card_CrossPtnList,CN_CARD)

__bIsCard(STORE,Card_CrossPtnList,CK_CROSSPTNLIST)
__pGetSubFunction_Card(STORE,CrossPtnList,CK_CROSSPTNLIST)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CrossPtnList::Card_CrossPtnList(void)
: Card(CK_CROSSPTNLIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CrossPtnList::~Card_CrossPtnList(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
