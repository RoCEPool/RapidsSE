/*============================================================================*/
/* Card_Cast.cpp -- Next                                                      */
/*============================================================================*/

#include "Card_Cast.hpp"

#include "Card_Bound.hpp"
#include "Card_Col.hpp"
#include "Card_Index.hpp"
#include "Card_IdxCol.hpp"
#include "Card_IdxColList.hpp"
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
                          Symbol* p_Y_tablePtn_prev,
                          Symbol* p_Y_tblCol_prev,
                          Symbol* p_Y_tablePtn_curr,
                          Symbol* p_Y_idxCol_curr)
{
  _static_

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


  // todo: this is naive, needs to be fixed
  if (Card_COL::bIsReplicated(p_Y_streamPtn_running))
    return XK_UNICAST;


  if (Card_COL::bIsReplicated(p_Y_idxCol_curr)) {
    // curr table is replicated so always unicast
    return XK_UNICAST;
  }

  if (! p_Y_tblCol_prev)
    // no prev table column (probably cross join)
    return XK_BROADCAST;

  if (Card_TablePtn::bColumnMatchesPtn(p_Y_tablePtn_curr, p_Y_idxCol_curr)) {
    // + curr indexed column is curr table ptn
    //   the curr join-to column is an indexed column of the curr table ptn
    //   so the curr join-to column identifies target key partition
    if (Card_TablePtn::bColumnMatchesPtn(p_Y_tablePtn_prev, p_Y_tblCol_prev)) {
      // + prev table column is prior table ptn
      //   likewise the prev data-from column is an indexed column of prev table ptn
      //   noting the cmpPred is ==, the source matches target, so we can unicast
      return XK_UNICAST;
    }
    else {
      // - prev table column is not prev table ptn
      //     but the prev table column can be used for hash cast to this stage
      //     because curr join-to column is an indexed column of the curr table ptn
      //     the prev data-from column can be hashed to identify target key partition
      return XK_HASHCAST;
    }
  }
  else {
    // - curr indexed column is not curr table ptn
    //     curr join-to column is not an indexed column of the curr table ptn
    //     cannot use the curr/target column to identify partition of target key
    //     so irrespective of prev data-from column we must broadcast
    return XK_BROADCAST;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
