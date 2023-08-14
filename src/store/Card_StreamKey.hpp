/*============================================================================*/
/* Card_StreamKey.hpp -- Stream Key                                           */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_StreamKey : public Card_symbol<Card_StreamKey>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_StreamKey(void);
   ~Card_StreamKey(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_StreamKey* _pCard_StreamKey(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
