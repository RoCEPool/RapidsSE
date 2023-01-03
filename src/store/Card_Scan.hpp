/*============================================================================*/
/* Card_Scan.hpp -- Scan                                                      */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "Card_PRED.hpp"

namespace STORE
{
  class Card_Scan : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Scan(void);
   ~Card_Scan(void) noexcept(false);

    void vApplyStream(Card_StreamModel* pStreamModel); // prototype
  };

  Card_Scan* _pCard_Scan(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
