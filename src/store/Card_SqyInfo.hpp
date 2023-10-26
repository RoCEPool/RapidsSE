/*============================================================================*/
/* Card_SqyInfo.hpp -- SubQuery Information                                   */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_SqyInfo : public Card_symbol<Card_SqyInfo>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_SqyInfo(void);
   ~Card_SqyInfo(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_SqyInfo* _pCard_SqyInfo(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
