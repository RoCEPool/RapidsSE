/*============================================================================*/
/* Card_StreamJoin.hpp -- Stream Join (join in stream sequence)               */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_StreamJoin : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_StreamJoin(void);
   ~Card_StreamJoin(void) noexcept(false);

    i8 nEstJoinCosts(i8 i_JoinPermSeq,
                     i8 i_Locus,
                     i8 nBuff_prev_locus,
                     i8 nFiltPred_prev,
                     eXK eLocusCast,
                     i8 nBuff_joining_with
                      );
  };

  Card_StreamJoin* _pCard_StreamJoin(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
