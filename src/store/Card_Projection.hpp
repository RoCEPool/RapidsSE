/*============================================================================*/
/* Card_Projection.hpp -- Stream Model                                       */
/*============================================================================*/
#pragma once

#include "Card_ELIST.hpp"

namespace STORE
{
  class Card_Projection : public Card_ELIST
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Projection(void);
   ~Card_Projection(void) noexcept(false);

    Card_Projection* pReProjection(void);
  };

  Card_Projection* _pCard_Projection(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
