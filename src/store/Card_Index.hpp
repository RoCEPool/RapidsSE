/*============================================================================*/
/* Card_Index.hpp -- Index                                                    */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_OBJECT.hpp"

namespace STORE
{
  class Card_Index : public Card_OBJECT
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Index(Index* pIndex);
   ~Card_Index(void) noexcept(false);
  };

  Card_Index* _pCard_Index(void* aAddr);
  bool _bIsCard_Index(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
