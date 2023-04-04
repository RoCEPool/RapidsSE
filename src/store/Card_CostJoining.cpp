/*============================================================================*/
/* Card_LoadJoined.cpp -- Load of the actual Join                              */
/*============================================================================*/

#include "Card_CostJoining.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_CmpPred.hpp"
#include "Card_CostNetwork.hpp"
#include "Card_CostUnicast.hpp"
#include "Card_Size.hpp"
#include "Card_Table.hpp"
#include "Card_TableList.hpp"
#include "Card_Term.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_CostJoining, CK_COSTJOINING)
__aNewFunction(Card_CostJoining, CN_CARD)
__vDeleteFunction(Card_CostJoining, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_CostJoining::Card_CostJoining(void)
: Card(CK_COSTJOINING)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_CostJoining::~Card_CostJoining(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* estimate join volume (as part of cost estimate of next join)               */
/*----------------------------------------------------------------------------*/
i8 Card_CostJoining::nEstJoinVolume(eXK eCastLocus,
                                    i8 nBuff_arriving,
                                    i8 nBuff_joiningTo)

{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 iLoadJoined = C_UNKNOWN;
  if (nBuff_arriving > nBuff_joiningTo)
    iLoadJoined = ((d8)nBuff_arriving / (d8)nBuff_joiningTo) * (nBuff_arriving + nBuff_joiningTo);
  else
    iLoadJoined = ((d8)nBuff_joiningTo / (d8)nBuff_arriving) * (nBuff_arriving + nBuff_joiningTo);

  vValue((i8)iLoadJoined);

  return iLoadJoined;
}


//*============================================================================*/
/* end                                                                        */
/*============================================================================*/
