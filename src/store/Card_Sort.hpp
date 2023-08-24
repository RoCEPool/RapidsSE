/*============================================================================*/
/* Card_Sort.hpp -- Sort                                                      */
/*============================================================================*/
#pragma once

#include "Card_OP.hpp"
#include "CarryForward.hpp"

namespace STORE
{
  class Card_Sort : public Card_OP
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Sort(void);
   ~Card_Sort(void) noexcept(false);

    void vColumn_Mapping(CardIter* pCI);

    bool bCut_sort(CardIter* pCI);

    void vDerive_streamKey(CardIter* pCI);

    void vSmooth(CardIter* pCI, CarryForward* pCarryForward);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Sort* _pCard_Sort(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
