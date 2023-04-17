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

    i8 nEstLocusCosts(i8 i_JoinPermSeq,
                      i8 i_Locus,
                      i8 nBuff_prev_locus,
                      i8 nFiltPred_prev,
                      eXK eLocusCast,
                      i8 nBuff_joining_with
                      );
  };

  Card_JoinLocus* _pCard_JoinLocus(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
