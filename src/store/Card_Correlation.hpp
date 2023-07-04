/*============================================================================*/
/* Card_Correlation.hpp -- Correlation information                            */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Correlation : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Correlation(void);
   ~Card_Correlation(void) noexcept(false);
  };

  Card_Correlation* _pCard_Correlation(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
