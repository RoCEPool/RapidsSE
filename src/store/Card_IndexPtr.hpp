/*============================================================================*/
/* Card_IndexPtr.hpp -- Index                                                    */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_IndexPtr : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_IndexPtr(Index* pIndex);
   ~Card_IndexPtr(void) noexcept(false);

    Card_IdxCol*     pFind_indexCol(Stz* szColNm, i8& i_IdxCol);
  };

  Card_IndexPtr* _pCard_IndexPtr(void* aAddr);
  bool _bIsCard_IndexPtr(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
