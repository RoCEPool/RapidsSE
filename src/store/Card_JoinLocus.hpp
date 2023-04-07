/*============================================================================*/
/* Card_JoinLocus.hpp -- Join Head (head of join sequence)                     */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_JoinLocus : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_JoinLocus(void);
   ~Card_JoinLocus(void) noexcept(false);

    i8 nEstLocusCosts(i8 nLocus,
                      i8 nBuff_emitted_prior,
                      eXK eLocusCast,
                      i8 nBuff_joining_with,
                      i8 nFilters);

    i8 nEstLocusCosts_v2(i8 nLocus,
                         i8 nBuff_emitted_prior,
                         eXK eLocusCast,
                         i8 nBuff_joining_with,
                         i8 nFilters);
  };

  Card_JoinLocus* _pCard_JoinLocus(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
