/*============================================================================*/
/* Card_Sends.hpp -- Sends                                                    */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Sends : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Sends(i8 iSends);
   ~Card_Sends(void) noexcept(false);
  };

  Card_Sends* _pCard_Sends(void* aAddr);
  bool _bIsCard_Sends(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
