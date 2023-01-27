/*============================================================================*/
/* Card_LoadNetwork.cpp -- Load of Network (in load of join)                  */
/*============================================================================*/

#include "Card_LoadNetwork.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_CmpPred.hpp"
#include "Card_Size.hpp"
#include "Card_Table.hpp"
#include "Card_TableList.hpp"
#include "Card_Term.hpp"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_LoadNetwork, CK_LOADNETWORK)
__aNewFunction(Card_LoadNetwork, CN_CARD)
__vDeleteFunction(Card_LoadNetwork, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_LoadNetwork::Card_LoadNetwork(void)
: Card(CK_LOADNETWORK)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_LoadNetwork::~Card_LoadNetwork(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* calculate network load                                                     */
/*----------------------------------------------------------------------------*/
i8 Card_LoadNetwork::nCalculateLoad(eXK eCast,
                                      i8 nBuffersIn,
                                      Card_TableList* p_tableList,
                                      char cTblSyn_l,
                                      char cTblSyn_r)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  // todo: complete...

  i8 nPtnsPerNode = AM::PTNMAP::nPtnsPerNode();

//  i8 iSize_l = p_tableList->pGetTableSize(cTblSyn_l)->iValue();
  i8 iSize_r = p_tableList->pGetTableSize(cTblSyn_r)->iValue();

  i8 iLoad = 0;
  i8 nBuffersOut = 0;
  switch (eCast) {
    case XK_UNICAST:
      break;
    case XK_HASHCAST:
      iLoad = (i8)(1.0 * nBuffersIn);
      nBuffersOut = (i8)(1.0 * (nBuffersIn * iSize_r));
      break;
    case XK_BROADCAST:
      iLoad = (i8)(1.0 * (nBuffersIn * nPtnsPerNode));
      nBuffersOut = (i8)(1.0 * (nBuffersIn * iSize_r * nPtnsPerNode));
      break;
    default:
      NEVER_REACH
  }

  vValue((i8)iLoad);

  return nBuffersOut;
}

//*============================================================================*/
/* end                                                                        */
/*============================================================================*/
