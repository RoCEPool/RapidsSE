/*============================================================================*/
/* Card_JoinNext.cpp -- Join Head (head of join sequence)                     */
/*============================================================================*/

#include "Card_JoinNext.hpp"

#include "Card_BoolCalc.hpp"
#include "Card_Cast.hpp"
#include "Card_CmpPred.hpp"
#include "Card_LoadJoin.hpp"
#include "Card_LoadNetwork.hpp"
#include "Card_LoadUnicast.hpp"
#include "Card_Table.hpp"
#include "Card_TableList.hpp"
#include "Card_Term.hpp"

#include "eXK.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_JoinNext, CK_JOINNEXT)
__aNewFunction(Card_JoinNext, CN_CARD)
__vDeleteFunction(Card_JoinNext, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_JoinNext::Card_JoinNext(void)
: Card(CK_JOINNEXT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_JoinNext::~Card_JoinNext(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* calculate join load                                                        */
/*----------------------------------------------------------------------------*/
i8 Card_JoinNext::nCalulateLoad(eXK eCast,
                                  i8 nBuffersIn,
                                  Card_TableList* p_tableList,
                                  char cTblSyn_l,
                                  char cTblSyn_r)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  Card_LoadJoin*    p_loadJoin    = new Card_LoadJoin;
  Card_Cast*        p_cast        = new Card_Cast;
  Card_LoadNetwork* p_loadNetwork = new Card_LoadNetwork;
  Card_LoadUnicast* p_loadUnicast = new Card_LoadUnicast;

//  Card_TablePtn* p_tablePtn_l = p_tableList->pGetTablePtn(cTblSyn_l);
//  Card_TablePtn* p_tablePtn_r = p_tableList->pGetTablePtn(cTblSyn_r);
//  Card_CmpPred*  p_cmpPred    = pSub_cmpPred();

  p_cast->vValue(aXK[eCast]);

  p_loadNetwork->vCalculateLoad(eCast,nBuffersIn,p_tableList,cTblSyn_l,cTblSyn_r);
  p_loadUnicast->vCalculateLoad(eCast,nBuffersIn,p_tableList,cTblSyn_l,cTblSyn_r);
  i8 iLoadJoin = p_loadJoin->nCalculateLoad(p_loadNetwork,p_loadUnicast);

  p_loadJoin->vAppend(p_cast);
  p_loadJoin->vAppend(p_loadNetwork);
  p_loadJoin->vAppend(p_loadUnicast);
  vAppend(p_loadJoin);

  return iLoadJoin;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
