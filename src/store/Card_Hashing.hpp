/*============================================================================*/
/* Card_Hashing.hpp -- Hashing                                                */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"
#include "ColMap.hpp"

namespace STORE
{
  class Card_Hashing : public Card_symbol<Card_Hashing>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Hashing(void);
   ~Card_Hashing(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Hashing* _pCard_Hashing(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
