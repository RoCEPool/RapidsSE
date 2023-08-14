/*============================================================================*/
/* Card_StreamJoin.hpp -- Stream Join (join in stream sequence)               */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_StreamJoin : public Card_symbol<Card_StreamJoin>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_StreamJoin(void);
   ~Card_StreamJoin(void) noexcept(false);

    i8 nEstJoinCost(i8 i_Permutation,
                    i8 i_Join,
                    i8 nUnfiltBuff_prev_join,
                    i8 nFiltPred_prev,
                    eXK eJoinCast,
                    i8 nBuff_joining_with);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_StreamJoin* _pCard_StreamJoin(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
