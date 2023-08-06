/*============================================================================*/
/* Card_Note.hpp -- Note                                                      */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_Note : public Card_symbol<Card_Note>
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Note(void);
    Card_Note(Stz* szComment);
   ~Card_Note(void) noexcept(false);
  };

  Card_Note* _pCard_Note(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
