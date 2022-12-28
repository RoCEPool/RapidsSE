/*============================================================================*/
/* Card_Bounds.hpp -- Join Predicate                                        */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Bounds : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Bounds(void);
   ~Card_Bounds(void) noexcept(false);

    static Card_Bounds*     pBounds_CreateFrom_PRED(Card_PRED* p_PRED,
                                             char cTblSyn);
    static Card_Bounds*     pBounds_Create_Eq_ForGivenCol(Stz* szTblColOdn);
    static Card_Bounds*     pBounds_CreateFrom_cmpPred(Card_CmpPred* p_cmpPred,
                                                  char cTblSyn);
    eXK eCastKind(Card_Index* p_index_r, Card_Table* p_table_l, Card_Table* p_table_r,
                  Card_Term*& rp_term_hash);
  };

  Card_Bounds* _pCard_Bounds(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
