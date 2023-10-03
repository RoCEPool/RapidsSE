/*============================================================================*/
/* Card_TableInfo.hpp -- Unique                                                  */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_TableInfo : public Card_symbol<Card_TableInfo>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_TableInfo(void);
   ~Card_TableInfo(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_TableInfo* _pCard_TableInfo(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
