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
/* calculate unicast load                                                     */
/*----------------------------------------------------------------------------*/
i8 Card_LoadUnicast::nCalculateLoad(eXK eCast,
                                      i8 nBuffersIn,
                                      Card_TableList* p_tableList,
                                      char cTblSyn_l,
                                      char cTblSyn_r)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  i8 iSize_l = p_tableList->pGetTableSize(cTblSyn_l)->iValue();
  i8 iSize_r = p_tableList->pGetTableSize(cTblSyn_r)->iValue();

  i8 iLoad = 0;
  i8 nBuffersOut = 0;
  switch (eCast) {
    case XK_UNICAST:
      iLoad = (i8)(1.0 * (nBuffersIn));
      nBuffersOut = (i8)(1.0 * (nBuffersIn * iSize_r));
      break;
    case XK_HASHCAST:
      break;
    case XK_BROADCAST:
      break;
    default:
      NEVER_REACH
  }

  vValue((i8)iLoad);

  return nBuffersOut;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
