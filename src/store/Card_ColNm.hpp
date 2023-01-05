/*============================================================================*/
/* Card_ColNm.hpp -- Column Name                                              */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_ColNm : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_ColNm(void);
   ~Card_ColNm(void) noexcept(false);
  };

  Card_ColNm* _pCard_ColNm(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
