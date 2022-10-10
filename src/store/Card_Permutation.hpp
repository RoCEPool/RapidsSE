/*============================================================================*/
/* Card_Permutation.hpp -- Filter                                                 */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Permutation : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Permutation(void);
   ~Card_Permutation(void) noexcept(false);
  };

  Card_Permutation* _pCard_Permutation(void* aAddr);
  bool _bIsCard_Permutation(void* aAddr);
  Card_Permutation* _pSub_Permutation(Card* pParent);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
