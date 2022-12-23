/*============================================================================*/
/* Card_Bounds.cpp -- Join Predicate                                        */
/*============================================================================*/

#include "Card_Bounds.hpp"

#include "Card_EBound.hpp"
#include "Card_IdxCol.hpp"
#include "Card_IdxColList.hpp"
#include "Card_Index.hpp"
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
eXK Card_Bounds::eCastKind(Card_Index* p_index_r,
                           Card_Table* p_table_l, Card_Table* p_table_r,
                           Card_Term*& rp_term_hash)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  Card_IdxColList* p_idxColList = p_index_r->pSub_idxColList();
  Card_TablePtn* p_tablePtn_l = p_table_l->pSub_tablePtn();
  Card_TablePtn* p_tablePtn_r = p_table_r->pSub_tablePtn();

  rp_term_hash = nullptr;

  CardIter xCI_bounds(this);
  while (Card_EBound* p_ebound = _pCard_EBound(xCI_bounds.pEachChild(CK_EBOUND))) {

    bool bMatch_l = false;
    bool bMatch_r = false;
    CardIter xCI_bound(p_ebound);
    while (Card_Term* p_term_bound = _pCard_Term(xCI_bound.pEachChild(CK_TERM))) {

      if (Card::bIsIndexColOrdinal(p_term_bound->szValue())) {
        Card_IdxCol* p_idxCol = p_idxColList->pIdxCol_byIdxColOdn(p_term_bound->szValue());
        if (! p_idxCol)
          continue;

        vTrace("p_idxCol %s tblptn_l %s tblptn_r %s\n"_Stz,
               p_idxCol->szValue(),p_tablePtn_l->szValue(),p_tablePtn_r->szValue());

        if (p_tablePtn_r->bSame(p_idxCol->szValue())) {
          bMatch_r = true;
        }

      }
      else
      if (Card::bIsTableColOrdinal(p_term_bound->szValue())) {

        if (p_tablePtn_l->bSame(p_term_bound->szValue())) {
          bMatch_l = true;
          rp_term_hash = p_term_bound;
        }

      }

    }

    if (bMatch_l) {
      if (bMatch_r)
        return XK_UNICAST;
      else
        return XK_HASHCAST;
    }
  }

  return XK_BROADCAST;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
