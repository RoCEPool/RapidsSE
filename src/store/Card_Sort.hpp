/*============================================================================*/
/* Card_Sort.hpp -- Sort                                                      */
/*============================================================================*/
#pragma once

#include "Card_OP.hpp"

namespace STORE
{
  class Card_Sort : public Card_OP
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Sort(void);
   ~Card_Sort(void) noexcept(false);

    void vCut_sort(CardIter* pCI);

    void vDerive_Projection_Key(CardIter* pCI);

    void vValidate(i8 iDepth, Card* pParent);
  };

  Card_Sort* _pCard_Sort(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
