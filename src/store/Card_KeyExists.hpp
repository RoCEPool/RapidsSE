/*============================================================================*/
/* Card_KeyExists.hpp -- Key Exists                                           */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_KeyExists : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_KeyExists(void);
   ~Card_KeyExists(void) noexcept(false);

    void vApplyStream(Card_StreamModel* pStreamModel); // prototype

  };

  Card_KeyExists* _pCard_KeyExists(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
