/*============================================================================*/
/* Card_Aggreg.hpp -- Aggreg                                                  */
/*============================================================================*/
#pragma once

#include "Card_AGGREGATE.hpp"

namespace STORE
{
  class Card_Aggreg : public Card_AGGREGATE
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Aggreg(void);
   ~Card_Aggreg(void) noexcept(false);


    void vDerive_streamKey(CardIter* pCI);

    void vSmooth(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);

  };

  Card_Aggreg* _pCard_Aggreg(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
