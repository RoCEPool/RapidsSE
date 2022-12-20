/*============================================================================*/
/* Card_TableList.hpp -- Table List                                           */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_TableList : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_TableList(void);
   ~Card_TableList(void) noexcept(false);

    static Card_TableList* pExtract(Card_Deck* p_deck);
  };

  Card_TableList* _pCard_TableList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
