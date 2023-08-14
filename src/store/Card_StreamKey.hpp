/*============================================================================*/
/* Card_StreamKey.hpp -- Stream Key                                           */
/*============================================================================*/
#pragma once

#include "Card_Key.hpp"

namespace STORE
{
  class Card_StreamKey : public Card_Key
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_StreamKey(void);
   ~Card_StreamKey(void) noexcept(false);

    static Card_StreamKey* pTo_streamKey(Card_Key* p_key);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_StreamKey* _pCard_StreamKey(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
