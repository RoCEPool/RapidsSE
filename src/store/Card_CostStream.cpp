/*============================================================================*/
/* Card_CostStream.cpp -- JLoad of join Permutation Sequence                 */
/*============================================================================*/

#include "Card_CostStream.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_CmpPred.hpp"
#include "Card_CostNetwork.hpp"
#include "Card_CostUnicast.hpp"
#include "Card_Table.hpp"
#include "Card_Term.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_CostStream, CK_COSTSTREAM)
__aNewFunction(Card_CostStream, CN_CARD)
__vDeleteFunction(Card_CostStream, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CostStream::Card_CostStream(void)
: Card_symbol(CK_COSTSTREAM)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CostStream::~Card_CostStream(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is this permutation sequence load greater than given arg?                  */
/*----------------------------------------------------------------------------*/
bool Card_CostStream::bIsHigherCost(Card_CostStream* p_costQuery_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CostNetwork* p_costNetwork_this = pSub_costNetwork();
  Card_CostNetwork* p_costNetwork_arg  = p_costQuery_arg->pSub_costNetwork();

  i8 iCostNetwork_this = p_costNetwork_this->iSymbol();
  i8 iCostNetwork_arg  = p_costNetwork_arg->iSymbol();

  if (iCostNetwork_this > iCostNetwork_arg)
    return true;

  if (iCostNetwork_this < iCostNetwork_arg)
    return false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_CostUnicast* p_costUnicast_this = pSub_costUnicast();
  Card_CostUnicast* p_costUnicast_arg  = p_costQuery_arg->pSub_costUnicast();

  i8 iloadUnicast_this = p_costUnicast_this->iSymbol();
  i8 iloadUnicast_arg  = p_costUnicast_arg->iSymbol();

  if (iloadUnicast_this > iloadUnicast_arg)
    return true;

  return false;
}

/*----------------------------------------------------------------------------*/
/* is this permutation sequence load less than given arg?                     */
/*----------------------------------------------------------------------------*/
bool Card_CostStream::bIsLowerCost(Card_CostStream* p_costQuery_arg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_CostNetwork* p_costNetwork_this = pSub_costNetwork();
  Card_CostNetwork* p_costNetwork_arg  = p_costQuery_arg->pSub_costNetwork();

  i8 iCostNetwork_this = p_costNetwork_this->iSymbol();
  i8 iCostNetwork_arg  = p_costNetwork_arg->iSymbol();

  if (iCostNetwork_this < iCostNetwork_arg)
    return true;

  if (iCostNetwork_this > iCostNetwork_arg)
    return false;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Card_CostUnicast* p_costUnicast_this = pSub_costUnicast();
  Card_CostUnicast* p_costUnicast_arg  = p_costQuery_arg->pSub_costUnicast();

  i8 iloadUnicast_this = p_costUnicast_this->iSymbol();
  i8 iloadUnicast_arg  = p_costUnicast_arg->iSymbol();

  if (iloadUnicast_this < iloadUnicast_arg)
    return true;

  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
