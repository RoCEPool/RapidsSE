/*============================================================================*/
/* Card_IdxCol.hpp -- Index Column                                            */
/*============================================================================*/
#pragma once

#include "Card_SUB.hpp"

namespace STORE
{
  class Card_IdxCol : public Card_SUB
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_IdxCol(Column* pColumn);
   ~Card_IdxCol(void) noexcept(false);
  };

  Card_IdxCol* _pCard_IdxCol(void* aAddr);
  bool _bIsCard_IdxCol(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
