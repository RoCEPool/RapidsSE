/*============================================================================*/
/* Card_FoldElist.cpp -- Fold Typed Expression                                 */
/*============================================================================*/

#include "Card_FoldElist.hpp"

#include "Card_Type.hpp"
#include "Card_As.hpp"
#include "Card_RpnCalc.hpp"
#include "Card_BoolCalc.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_FoldElist, CK_FOLDELIST)
__aNewFunction(Card_FoldElist, CN_CARD)
__vDeleteFunction(Card_FoldElist, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_FoldElist::Card_FoldElist(void)
: Card_ELIST(CK_FOLDELIST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_FoldElist::~Card_FoldElist(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

//*============================================================================*/
/* end                                                                        */
/*============================================================================*/
