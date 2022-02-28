/*============================================================================*/
/* Card_Board.hpp -- Board                                                    */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_MAIN.hpp"

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
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
