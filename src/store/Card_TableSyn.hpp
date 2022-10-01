/*============================================================================*/
/* Card_TableSyn.hpp -- Table Synonym                                         */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_TableSyn : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_TableSyn(void);
   ~Card_TableSyn(void) noexcept(false);
  };

  Card_TableSyn* _pCard_TableSyn(void* aAddr);
  bool _bIsCard_TableSyn(void* aAddr);
  Card_TableSyn* _pGetSub_TableSyn(Card * pParent);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
