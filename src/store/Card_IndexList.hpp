/*============================================================================*/
/* Card_IndexList.hpp -- Index List                                           */
/*============================================================================*/
#pragma once

#include "Card_SUB.hpp"

namespace STORE
{
  class Card_Stream;

  class Card_IndexList : public Card_SUB
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_IndexList(Index* pIndex);
   ~Card_IndexList(void) noexcept(false);
  };

  Card_IndexList* _pCard_IndexList(void* aAddr);
  bool _bIsCard_IndexList(void* aAddr);
  Card_IndexList* _pGetSub_Card_IndexList(Card* pParent);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
