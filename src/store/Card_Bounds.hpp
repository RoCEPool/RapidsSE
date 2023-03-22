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

    static Card_Bounds*  pBounds_Create_Eq_ForGivenCol(Stz* szTblColOdn);

    eXK eCastKind_v2(Card_Index* p_index_r, Card_Table* p_table_l, Card_Table* p_table_r,
                     Card_Term*& rp_left_column_to_be_hashed);

    void vCullBound(char cWithMark);

    void vTermsBecome(Card_Projection* p_projection_reduced);
  };

  Card_Bounds* _pCard_Bounds(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
