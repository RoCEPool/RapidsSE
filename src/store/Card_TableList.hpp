/*============================================================================*/
/* Card_TableList.hpp -- Table List                                           */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Stream;

  class Card_TableList : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_TableList(void);
   ~Card_TableList(void) noexcept(false);
  };

  Card_TableList* _pCard_TableList(void* aAddr);
  bool _bIsCard_TableList(void* aAddr);
  Card_TableList* _pGetSub_Card_TableList(Card* pParent);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
