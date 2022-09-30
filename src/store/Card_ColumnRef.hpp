/*============================================================================*/
/* Card_ColumnRef.hpp -- Column Reference                                     */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_ColumnRef : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_ColumnRef(void);
   ~Card_ColumnRef(void) noexcept(false);
  };

  Card_ColumnRef* _pCard_ColumnRef(void* aAddr);
  bool _bIsCard_ColumnRef(void* aAddr);
  Card_ColumnRef* _pGetSub_Card_ColumnRef(Card * pParent);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
