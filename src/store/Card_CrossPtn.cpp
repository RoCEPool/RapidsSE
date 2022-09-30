/*============================================================================*/
/* Card_CrossPtn.cpp -- Cross-Partition                                       */
/*============================================================================*/

#include "Card_CrossPtn.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_CrossPtn,CK_COPTN)
__aNewFunction(Card_CrossPtn,CN_CARD)
__vDeleteFunction(Card_CrossPtn,CN_CARD)

__bIsCard(STORE,Card_CrossPtn,CK_COPTN)
__pGetSubFunction_Card(STORE,Card_CrossPtn,CK_COPTN)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CrossPtn::Card_CrossPtn(void)
: Card(CK_COPTN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CrossPtn::~Card_CrossPtn(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
