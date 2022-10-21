/*============================================================================*/
/* Card_PtnColNm.hpp -- Stream Column Name                                    */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_PtnColNm : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_PtnColNm(void);
   ~Card_PtnColNm(void) noexcept(false);
  };

  Card_PtnColNm* _pCard_PtnColNm(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
