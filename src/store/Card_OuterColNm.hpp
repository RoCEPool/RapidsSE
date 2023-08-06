/*============================================================================*/
/* Card_OuterColNm.hpp -- Parameter                                                */
/*============================================================================*/
#pragma once

#include "Card_t.hpp"

namespace STORE
{
  class Card_OuterColNm : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_OuterColNm(void);
   ~Card_OuterColNm(void) noexcept(false);
  };

  Card_OuterColNm* _pCard_OuterColNm(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
