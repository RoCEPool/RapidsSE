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
                          Card_TablePtn* p_tablePtn_curr,
                          Card_Index*    p_index_curr,
                          Card_Term*&    rp_term_tblCol_tbh)
{
  _static_

  Card_IdxColList* p_idxColList_curr = p_index_curr->pSub_idxColList();
  Card_Bound*      p_bound_curr      = p_index_curr->pSub_bound();

  Card_Term*       p_term_curr_idxColOdn = p_bound_curr->pGet_idxColOdn();
  Card_IdxCol *    p_idxCol_curr =
      p_idxColList_curr->pGetIdxCol_byIdxColOdn(p_term_curr_idxColOdn->szValue());

  //! this is naive
  if (p_tablePtn_prev->pSymbol()->bIsReplicated())
    return XK_UNICAST;

  if (p_idxCol_curr && p_idxCol_curr->pSymbol()->bIsReplicated()) {
    // + later table is replicated so always unicast
    //p_idxCol_curr->vDump();
    vDebugPoint();
    return XK_UNICAST;
  }

  //
  Card_Term* p_term_tblCol_tbh = p_bound_curr->pGet_tblColOdn();


  if (! p_term_tblCol_tbh)
    // - no prior table column (referenced in this bound) -- probably cross join
    return XK_BROADCAST;

  if (p_idxCol_curr->bSameValue(p_tablePtn_curr)) {
    // + later indexed column (referenced in this bound) is later table ptn
    if (p_term_tblCol_tbh->bSameValue(p_tablePtn_prev)) {
      // + prior table column (referenced in this bound) is prior table ptn
      return XK_UNICAST;
    }
    else {
      // - prior table column (referenced in this bound) is not prior table ptn
      //     but the prior table column can be used for hash cast
      rp_term_tblCol_tbh = p_term_tblCol_tbh;
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
