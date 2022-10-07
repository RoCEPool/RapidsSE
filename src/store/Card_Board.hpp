/*============================================================================*/
/* Card_Board.hpp -- Board                                                    */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Board : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Board(void);
   ~Card_Board(void) noexcept(false);
  };

  Card_Board* _pCard_Board(void* aAddr);
  bool _bIsCard_Board(void* aAddr);
  Card_Board* _pGetSub_Board(Card* pParent);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
