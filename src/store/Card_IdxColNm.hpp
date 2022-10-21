/*============================================================================*/
/* Card_IdxColNm.hpp -- Index Column Name                                     */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_IdxColNm : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_IdxColNm(void);
   ~Card_IdxColNm(void) noexcept(false);
  };

  Card_IdxColNm* _pCard_IdxColNm(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
