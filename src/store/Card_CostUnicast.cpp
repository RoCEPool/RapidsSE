/*============================================================================*/
/* Card_CostUnicast.cpp -- Join Head (head of join sequence)                     */
/*============================================================================*/

#include "Card_CostUnicast.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_CmpPred.hpp"
#include "Card_Size.hpp"
#include "Card_Table.hpp"
#include "Card_TableList.hpp"
#include "Card_Term.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_CostUnicast, CK_COSTUNICAST)
__aNewFunction(Card_CostUnicast, CN_CARD)
__vDeleteFunction(Card_CostUnicast, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CostUnicast::Card_CostUnicast(void)
: Card(CK_COSTUNICAST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CostUnicast::~Card_CostUnicast(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* estimate unicast volume (as part of cost estimate of next join)            */
/*----------------------------------------------------------------------------*/
void Card_CostUnicast::vEstUnicastVolume(eXK eCastLocus,
                                         i8 nBuff_arriving,
                                         i8 nBuff_joiningTo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 iLoadUnicast = 0;
  i8 nBuffersOut = 0;

  switch (eCastLocus) {

    case XK_UNICAST:
      iLoadUnicast = (i8)(1.0 * (nBuff_arriving));
      nBuffersOut = (i8)(1.0 * (nBuff_arriving * nBuff_joiningTo));
      break;

    case XK_HASHCAST:
      break;

    case XK_BROADCAST:
      break;

    default:
      NEVER_REACH
  }

  vValue((i8)iLoadUnicast);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
