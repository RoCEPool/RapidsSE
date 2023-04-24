/*============================================================================*/
/* Card_Cast.cpp -- Next                                                      */
/*============================================================================*/

#include "Card_Cast.hpp"

#include "Card_Bound.hpp"
#include "Card_COL.hpp"
#include "Card_Index.hpp"
#include "Card_IdxCol.hpp"
#include "Card_IdxColList.hpp"
#include "Card_StreamPtn.hpp"
#include "Card_TablePtn.hpp"
#include "Card_Term.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Cast, CK_CAST)
__aNewFunction(Card_Cast, CN_CARD)
__vDeleteFunction(Card_Cast, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Cast::Card_Cast(void)
: Card(CK_CAST)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Cast::~Card_Cast(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* determine the stage cast from prev to curr (assuming cmpPred ==)           */
/*----------------------------------------------------------------------------*/
eXK Card_Cast::eStageCast(Symbol* p_Y_streamPtn_running,
                          Card_StreamPtn* p_streamPtn,
                          Symbol* p_Y_tblCol_prior,
                          Symbol* p_Y_tablePtn_curr,
                          Symbol* p_Y_idxCol_curr)
{
  _static_

  RASSERT(p_streamPtn && ! p_streamPtn->bIsEmpty());
  /*
   *           Prev     Curr
   *   tblPtn  $a.1     $b.1
   *                 ==
   *   col     $a.1     $b.1  : unicast
   *           $a.2     $b.1  : hashcast
   *           $a.1     $b.2  : broadcast
   *
   *           $a.2     $B.any: unicast
   */

  eXK eCast = XK_NONE;

//  //
//  if (Card_COL::bIsReplicated(p_Y_streamPtn_running))
//    eCast = XK_UNICAST;
//
//  else
  if (Card_COL::bIsReplicated(p_Y_idxCol_curr)) {
    // curr table is replicated so always unicast
    eCast = XK_UNICAST;
  }

  else
  if (! p_Y_tblCol_prior)
    // no prior table column (probably cross join)
    eCast = XK_BROADCAST;

  else
  if (Card_TablePtn::bColumnMatchesPtn(p_Y_tablePtn_curr, p_Y_idxCol_curr)) {
    // + curr indexed column is curr table ptn
    //   the curr join-to column is an indexed column of the curr table ptn
    //   so the curr join-to column identifies target key partition
    //if (Card_TablePtn::bColumnMatchesPtn(p_Y_tablePtn_prior, p_Y_tblCol_prior)) {
//  if (Card_TablePtn::bColumnMatchesPtn(p_Y_streamPtn_running, p_Y_tblCol_prior)) {
    //p_streamPtn->vDump();
    if (p_streamPtn->bContainsKey(p_Y_tblCol_prior)) {
      // + prev table column is prior table ptn
      //   likewise the prev data-from column is an indexed column of prev table ptn
      //   noting the cmpPred is ==, the source matches target, so we can unicast
      eCast = XK_UNICAST;
    }
    else {
      // - prev table column is not prev table ptn
      //     but the prev table column can be used for hash cast to this stage
      //     because curr join-to column is an indexed column of the curr table ptn
      //     the prev data-from column can be hashed to identify target key partition
      eCast = XK_HASHCAST;
    }
  }

  else {
    // - curr indexed column is not curr table ptn
    //     curr join-to column is not an indexed column of the curr table ptn
    //     cannot use the curr/target column to identify partition of target key
    //     so irrespective of prev data-from column we must broadcast
    eCast = XK_BROADCAST;
  }

  switch (eCast) {

    case XK_BROADCAST:
      p_streamPtn->vResetPtn();
      p_streamPtn->vAddKey(p_Y_idxCol_curr);
      break;

    case XK_HASHCAST:
      p_streamPtn->vResetPtn();
      p_streamPtn->vAddKey(p_Y_idxCol_curr);
      break;

    case XK_UNICAST:
      p_streamPtn->vAddKey(p_Y_idxCol_curr);
      break;

    default:
      NEVER_REACH
  }
  return eCast;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
