/*============================================================================*/
/* Card_Cast.cpp -- Next                                                      */
/*============================================================================*/

#include "Card_Cast.hpp"

#include "Card_Bound.hpp"
#include "Card_COLUMN.hpp"
#include "Card_StreamPtn.hpp"
#include "Card_TablePtn.hpp"

#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Cast, CK_o_CAST)
__aNewFunction(Card_Cast, CN_CARD)
__vDeleteFunction(Card_Cast, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Cast::Card_Cast(void)
: Card_symbol(CK_o_CAST, C_UNKNOWN)
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
eXK Card_Cast::eStageCast(CardIter* pCI,
                          Card_StreamPtn* p_streamPtn,
                          Symbol* p_Y_tblCol_prior,
                          Symbol* p_Y_tablePtn_join,
                          Symbol* p_Y_idxCol_join)
{
  _static_

  UNUSED(pCI);

  RASSERT(p_streamPtn && p_streamPtn->nChildCards());
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

  // -- -- -- -- -- -- -- -- -- --
  // ? stream joining to a replicated table
  if (Card_COLUMN::bIsReplicated(p_Y_idxCol_join)) {
    // + join table is replicated
    //     stream is unicast mapping to partitions of join table
    eCast = XK_UNICAST;
  }

  // -- -- -- -- -- -- -- -- -- --
  // ? no prior column
  else
  if (! p_Y_tblCol_prior)
    // + no prior column (probably cross join)
    //     stream requires a broadcast mapping to table partitions
    eCast = XK_BROADCAST;

  // -- -- -- -- -- -- -- -- -- --
  // ? stream joining to table partition
  else
  if (Card_TablePtn::bColumnMatchesPtn(p_Y_tablePtn_join, p_Y_idxCol_join)) {
    // + stream joining to column which is table partition

    // ? prior column is key in stream ptn
   if (p_streamPtn->bHasSymbolRecursive(CK_STREAMKEY,p_Y_tblCol_prior)) {
      // + prior column is a key in stream partition
      //     stream is a unicast mapping to table partitions
      eCast = XK_UNICAST;
    }
    else {
      // - prior table column is not a key in stream partition
      //     stream is a hashcast mapping to table partitions
      //       as prior column can be used to identify table partition
      eCast = XK_HASHCAST;
    }
  }

  // -- -- -- -- -- -- -- -- -- --
  // not joining to table partition
  else {
    if (Card_COLUMN::bIsReplicated(p_Y_tblCol_prior))
      eCast = XK_UNICAST;
    else
    // stream requires a broadcast mapping to table partitions
      eCast = XK_BROADCAST;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // update the key set of running stream partition

  switch (eCast) {

    case XK_BROADCAST:
      p_streamPtn->vDeleteAllChildren();
       pCI->vAppendNewChild(p_streamPtn,CK_STREAMKEY,p_Y_tablePtn_join);
      break;

    case XK_HASHCAST:
      p_streamPtn->vDeleteAllChildren();
      pCI->vAppendNewChild(p_streamPtn,CK_STREAMKEY,p_Y_tablePtn_join);
      break;

    case XK_UNICAST:
      pCI->vAppendNewChild(p_streamPtn,CK_STREAMKEY,p_Y_tablePtn_join);
      break;

    default:
      NEVER_REACH
  }
  return eCast;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Cast::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  vValidate_notHave(CK_s_LEVEL, CK_NONE);

  Card::vValidate(pCI,iDepth,pParent);
  pCI->vValidateChildren(this,iDepth+1);

  vValidationPassed();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
