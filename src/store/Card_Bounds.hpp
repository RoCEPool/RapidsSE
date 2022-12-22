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

    eXK eCastKind(Card_TablePtn* p_tablePtn_l, Card_TablePtn* p_tablePtn_r,
                  Card_Term*& rp_term_hash);
  };

  Card_Bounds* _pCard_Bounds(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
