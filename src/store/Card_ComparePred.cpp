/*============================================================================*/
/* Card_ComparePred.cpp -- Compare Predicate                                  */
/*============================================================================*/

#include "Card_ComparePred.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_ComparePred,CK_COMPAREPRED)
__aNewFunction(Card_ComparePred,CN_CARD)
__vDeleteFunction(Card_ComparePred,CN_CARD)

__bIsCard(STORE,Card_ComparePred,CK_COMPAREPRED)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_ComparePred::Card_ComparePred(void)
: Card_PRED(CK_COMPAREPRED)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_ComparePred::~Card_ComparePred(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get bound of this compare op                                               */
/*----------------------------------------------------------------------------*/
eBK Card_ComparePred::eBound(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  i8 i_LastCard = nItems() - 1;
  Card* pLastCard = pGet(i_LastCard);

  eBK eBound = pLastCard->eBound();
  
  return eBound;
}

/*----------------------------------------------------------------------------*/
/* get relational operator of this compare op                                 */
/*----------------------------------------------------------------------------*/
eRO Card_ComparePred::eRelOp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  i8 i_LastCard = nItems() - 1;
  Card* pLastCard = pGet(i_LastCard);

  eRO eRelOp = pLastCard->eRelOp();
  
  return eRelOp;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
