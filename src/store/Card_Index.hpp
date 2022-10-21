/*============================================================================*/
/* Card_Index.hpp -- Index                                                    */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Index : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Index(void);
   ~Card_Index(void) noexcept(false);
  };

  Card_Index* _pCard_Index(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
