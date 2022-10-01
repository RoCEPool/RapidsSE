/*============================================================================*/
/* Card_CrossPtnPred.cpp -- Permutation                                        */
/*============================================================================*/

#include "Card_CrossPtnPred.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_CrossPtnPred,CK_CROSSPTNPRED)
__aNewFunction(Card_CrossPtnPred,CN_CARD)
__vDeleteFunction(Card_CrossPtnPred,CN_CARD)

__bIsCard(STORE,Card_CrossPtnPred,CK_CROSSPTNPRED)
__pGetSubFunction_Card(STORE,CrossPtnPred,CK_CROSSPTNPRED)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CrossPtnPred::Card_CrossPtnPred(void)
: Card(CK_CROSSPTNPRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CrossPtnPred::~Card_CrossPtnPred(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
