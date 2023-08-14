/*============================================================================*/
/* Card_Count.hpp -- Count                                                    */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_Count : public Card_symbol<Card_Count>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Count(void);
   ~Card_Count(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Count* _pCard_Count(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
