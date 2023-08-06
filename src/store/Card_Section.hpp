/*============================================================================*/
/* Card_Section.hpp -- Section break                                          */
/*============================================================================*/
#pragma once

#include "Card_t.hpp"

namespace STORE
{
  class Card_Section : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Section(void);
    Card_Section(Stz* szComment);
   ~Card_Section(void) noexcept(false);
  };

  Card_Section* _pCard_Section(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
