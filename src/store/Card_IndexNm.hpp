/*============================================================================*/
/* Card_IndexNm.hpp -- Index Name                                             */
/*============================================================================*/
#pragma once

#include "Card_t.hpp"

namespace STORE
{
  class Card_IndexNm : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_IndexNm(void);
   ~Card_IndexNm(void) noexcept(false);
  };

  Card_IndexNm* _pCard_IndexNm(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
