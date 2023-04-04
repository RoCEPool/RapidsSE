/*============================================================================*/
/* Card_TableNm.hpp -- Unique                                                  */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_TableNm : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_TableNm(void);
    Card_TableNm(Stz* szTableNm);
   ~Card_TableNm(void) noexcept(false);
  };

  Card_TableNm* _pCard_TableNm(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
