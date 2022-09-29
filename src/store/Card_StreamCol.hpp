/*============================================================================*/
/* Card_StreamCol.hpp -- Stream Column                                        */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_StreamCol : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_StreamCol(void);
   ~Card_StreamCol(void) noexcept(false);

    Stz* szStreamColNm(void);
  };

  Card_StreamCol* _pCard_StreamCol(void* aAddr);
  bool _bIsCard_StreamCol(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
