/*============================================================================*/
/* Card_CandIndex.hpp -- Candidate Index                                      */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_CandIndex : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CandIndex(void);
   ~Card_CandIndex(void) noexcept(false);

    i8   iRank(void);
    void vRank(i8 iRank);
  };

  Card_CandIndex* _pCard_CandIndex(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
