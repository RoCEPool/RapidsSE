/*============================================================================*/
/* Card_Load.cpp -- Load of Join                                         */
/*============================================================================*/

#include "Card_Load.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_CmpPred.hpp"
#include "Card_LoadNetwork.hpp"
#include "Card_LoadUnicast.hpp"
#include "Card_Table.hpp"
#include "Card_Term.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Load, CK_LOAD)
__aNewFunction(Card_Load, CN_CARD)
__vDeleteFunction(Card_Load, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Load::Card_Load(void)
: Card(CK_LOAD)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Load::~Card_Load(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* calculate join load                                                        */
/*----------------------------------------------------------------------------*/
i8 Card_Load::nCalculateLoad(Card_LoadNetwork* p_loadNetwork,
                             Card_LoadUnicast* p_loadUnicast)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);


  i8 iLoadJoin = p_loadNetwork->iValue() + p_loadUnicast->iValue();
  //vValue((i8)iLoadJoin);

  return iLoadJoin;
}


//*============================================================================*/
/* end                                                                        */
/*============================================================================*/
