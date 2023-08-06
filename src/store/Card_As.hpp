/*============================================================================*/
/* Card_As.hpp -- As                                                          */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_As : public Card_symbol<Card_As>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_As(i8 iAsNo);
    Card_As(Stz* szAs);
    Card_As(Card_As* p_as_arg);
   ~Card_As(void) noexcept(false);

    Card_As* pClone(void);
    bool bIsSame(Card_As* p_as_arg);
  };

  Card_As* _pCard_As(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
