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
/* estimate possible buffers arriving at this locus via unicast               */
/*----------------------------------------------------------------------------*/
void Card_CostUnicast::vEstUnicastBufferVolume(i8 nLocus,
                                               i8 nBuff_emitted_prior,
                                               eXK eLocusCast)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 nBuff_unicast = 0;

  switch (eLocusCast) {

    case XK_UNICAST:
      // buffers arriving to this locus via a unicast
      nBuff_unicast = (i8)(1.0 * (nBuff_emitted_prior));
      break;

    case XK_HASHCAST:
      // no unicast buffers arriving to this locus via hashcast
      break;

    case XK_BROADCAST:
      // no unicast buffers arriving to this locus via broadcast
      break;

    default:
      NEVER_REACH
  }

  vValue(nBuff_unicast);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
