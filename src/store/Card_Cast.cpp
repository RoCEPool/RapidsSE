/*============================================================================*/
/* Card_Cast.cpp -- Next                                                      */
/*============================================================================*/

#include "Card_Cast.hpp"

#include "Card_Bound.hpp"
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
/* determine the stage cast for the previous p_cast                           */
/*----------------------------------------------------------------------------*/
eXK Card_Cast::eStageCast(Card_TablePtn* p_tablePtn_prev,
                          Symbol*        p_Y_tablePtn_curr,
                          Symbol*        p_Y_idxCol_curr,
                          Symbol*        p_Y_tblCol_tbh)
{
  _static_

  //! this is naive
  if (p_tablePtn_prev->pSymbol()->bIsReplicated())
    return XK_UNICAST;

  if (p_Y_idxCol_curr && p_Y_idxCol_curr->bIsReplicated()) {
    // + later table is replicated so always unicast
    //p_idxCol_curr->vDump();
    vDebugPoint();
    return XK_UNICAST;
  }



  if (! p_Y_tblCol_tbh)
    // - no prior table column (referenced in this bound) -- probably cross join
    return XK_BROADCAST;

  if (p_Y_idxCol_curr->bSameValue(p_Y_tablePtn_curr)) {
    // + later indexed column (referenced in this bound) is later table ptn
    if (p_Y_tblCol_tbh->bSameValue(p_tablePtn_prev->pSymbol())) {
      // + prior table column (referenced in this bound) is prior table ptn
      return XK_UNICAST;
    }
    else {
      // - prior table column (referenced in this bound) is not prior table ptn
      //     but the prior table column can be used for hash cast
      return XK_HASHCAST;
    }
  }
  else {
    // - later indexed column in this bound is not later table ptn
    return XK_BROADCAST;
  }
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
