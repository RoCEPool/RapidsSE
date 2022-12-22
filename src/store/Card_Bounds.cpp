/*============================================================================*/
/* Card_Bounds.cpp -- Join Predicate                                        */
/*============================================================================*/

#include "Card_Bounds.hpp"

#include "Card_EBound.hpp"
#include "Card_Table.hpp"
#include "Card_TablePtn.hpp"
#include "Card_Term.hpp"

#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Bounds, CK_BOUNDS)
__aNewFunction(Card_Bounds, CN_CARD)
__vDeleteFunction(Card_Bounds, CN_CARD)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Bounds::Card_Bounds(void)
: Card(CK_BOUNDS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Bounds::~Card_Bounds(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
eXK Card_Bounds::eCastKind(Card_Table* p_table_l, Card_Table* p_table_r,
                           Card_Term*& rp_term_hash)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_TablePtn* p_tablePtn_l = p_table_l->pSub_tablePtn();
  Card_TablePtn* p_tablePtn_r = p_table_r->pSub_tablePtn();

  rp_term_hash = nullptr;


  CardIter xCI_bounds(this);
  while (Card_EBound* p_ebound = _pCard_EBound(xCI_bounds.pEachChild(CK_EBOUND))) {

    bool bMatch_l = false;
    bool bMatch_r = false;
    CardIter xCI_bound(p_ebound);
    while (Card_Term* p_term_bound = _pCard_Term(xCI_bound.pEachChild(CK_TERM))) {

      if (p_tablePtn_l->bSame(p_term_bound->szValue())) {
        bMatch_l = true;
      }
      else
      if (p_tablePtn_r->bSame(p_term_bound->szValue())) {
        bMatch_r = true;
        rp_term_hash = p_term_bound;
      }
    }

    if (bMatch_l) {
      if (bMatch_r)
        return XK_UNICAST;
    }
    else {
      if (bMatch_r)
        return XK_HASHCAST;
    }
  }

  return XK_BROADCAST;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
