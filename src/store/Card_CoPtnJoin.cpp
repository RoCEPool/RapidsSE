/*============================================================================*/
/* Card_CoPtnJoin.cpp -- Co-Partition                                             */
/*============================================================================*/

#include "Card_CoPtnJoin.hpp"

#include "Card_CmpPred.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_CoPtnJoin, CK_COPTNJOIN)
__aNewFunction(Card_CoPtnJoin, CN_CARD)
__vDeleteFunction(Card_CoPtnJoin, CN_CARD)

__bIsCard(STORE, Card_CoPtnJoin, CK_COPTNJOIN)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CoPtnJoin::Card_CoPtnJoin(void)
: Card(CK_COPTNJOIN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CoPtnJoin::~Card_CoPtnJoin(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool Card_CoPtnJoin::bCoPtnWith(Card_CoPtnPred* p_CoPtnPred)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CmpPred* p_predCmp_this = _pCard_CmpPred(this);
  Card_CmpPred* p_predCmp_arg  = _pCard_CmpPred(p_CoPtnPred);

  if (p_predCmp_this->bCoPtnWith(p_predCmp_arg))
    return true;

  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
