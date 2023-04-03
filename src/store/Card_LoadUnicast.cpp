/*============================================================================*/
/* Card_LoadUnicast.cpp -- Join Head (head of join sequence)                     */
/*============================================================================*/

#include "Card_LoadUnicast.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_CmpPred.hpp"
#include "Card_Size.hpp"
#include "Card_Table.hpp"
#include "Card_TableList.hpp"
#include "Card_Term.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_LoadUnicast, CK_LOADUNICAST)
__aNewFunction(Card_LoadUnicast, CN_CARD)
__vDeleteFunction(Card_LoadUnicast, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_LoadUnicast::Card_LoadUnicast(void)
: Card(CK_LOADUNICAST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_LoadUnicast::~Card_LoadUnicast(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* estimate unicast volume (as part of cost estimate of next join)            */
/*----------------------------------------------------------------------------*/
void Card_LoadUnicast::vEstUnicastVolume(i8 nNodes,
                                       eXK eNextCast,
                                       i8 nBuff_arriving,
                                       Card_TableList* p_tableList,
                                       char cTblSyn_r,
                                         i8 iSize_r)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //i8 iSize_r = p_tableList->pGetTableSize(cTblSyn_r)->iValue();

  i8 iLoadUnicast = 0;
  i8 nBuffersOut = 0;

  switch (eNextCast) {

    case XK_UNICAST:
      iLoadUnicast = (i8)(1.0 * (nBuff_arriving));
      nBuffersOut = (i8)(1.0 * (nBuff_arriving * iSize_r));
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
