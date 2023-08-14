/*============================================================================*/
/* Card_StreamHead.hpp -- Stream Head (head of stream sequence)               */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
   class Card_StreamHead : public Card_symbol<Card_StreamHead>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_StreamHead(void);
   ~Card_StreamHead(void) noexcept(false);

     void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
   };

  Card_StreamHead* _pCard_StreamHead(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
