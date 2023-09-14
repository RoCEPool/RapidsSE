/*============================================================================*/
/* Card_TableSize.hpp -- Size (of table)                                           */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_TableSize : public Card_symbol<Card_TableSize>
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_TableSize(void);
   ~Card_TableSize(void) noexcept(false);

   static Card_TableSize* pDerive(Table* pTable);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_TableSize* _pCard_TableSize(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
