/*============================================================================*/
/* Card_CardPtr.hpp -- Card                                                     */
/*============================================================================*/
#pragma once

#include "Card.hpp"
//#include "Ribbon.hpp"

namespace STORE
{
  class Card_CardPtr : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CardPtr(Card* pCard);
   ~Card_CardPtr(void) noexcept(false);

    void vCardPtrPrint(Ribbon* pRibbon, i8 iDepth);
  };

  Card_CardPtr* _pCard_CardPtr(void* aAddr);
  bool _bIsCard_CardPtr(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
