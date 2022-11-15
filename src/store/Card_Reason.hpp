/*============================================================================*/
/* Card_Reason.hpp -- Reason                                                  */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Reason : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Reason(void);
    Card_Reason(Stz* szComment);
   ~Card_Reason(void) noexcept(false);
  };

  Card_Reason* _pCard_Reason(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
