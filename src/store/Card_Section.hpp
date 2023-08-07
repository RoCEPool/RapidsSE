/*============================================================================*/
/* Card_Section.hpp -- Section break                                          */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_Section : public Card_symbol<Card_Section>
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Section(void);
   ~Card_Section(void) noexcept(false);
  };

  Card_Section* _pCard_Section(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
