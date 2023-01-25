/*============================================================================*/
/* Card_LoadPermSeq.cpp -- JLoad of join Permutation Sequence                 */
/*============================================================================*/

#include "Card_LoadPermSeq.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_CmpPred.hpp"
#include "Card_LoadNetwork.hpp"
#include "Card_LoadUnicast.hpp"
#include "Card_Table.hpp"
#include "Card_Term.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_LoadPermSeq, CK_LOADPERMSEQ)
__aNewFunction(Card_LoadPermSeq, CN_CARD)
__vDeleteFunction(Card_LoadPermSeq, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_LoadPermSeq::Card_LoadPermSeq(void)
: Card(CK_LOADPERMSEQ)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_LoadPermSeq::~Card_LoadPermSeq(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


//*============================================================================*/
/* end                                                                        */
/*============================================================================*/
