/*============================================================================*/
/* Card_CandIndex.hpp -- Candidate Index                                      */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_MAIN.hpp"

namespace STORE
{
  class Card_CandIndex : public Card_MAIN
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CandIndex(void);
   ~Card_CandIndex(void) noexcept(false);
  };

  Card_CandIndex* _pCard_CandIndex(void* aAddr);
  bool _bIsCard_CandIndex(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
