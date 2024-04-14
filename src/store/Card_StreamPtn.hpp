/*============================================================================*/
/* Card_StreamPtn.hpp -- Stream Partition                                     */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_StreamPtn : public Card_symbol<Card_StreamPtn>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_StreamPtn(void);
   ~Card_StreamPtn(void) noexcept(false);

    bool bContainsKey(Symbol* p_Y_tblCol);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_StreamPtn* _pCard_StreamPtn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
