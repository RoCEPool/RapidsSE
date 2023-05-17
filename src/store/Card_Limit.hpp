/*============================================================================*/
/* Card_Limit.hpp -- Limit                                                    */
/*============================================================================*/
#pragma once

#include "Card_OP.hpp"

namespace STORE
{
  class Card_Limit : public Card_OP
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Limit(Stz* szRows);
   ~Card_Limit(void) noexcept(false);

    void vComputeProjection(CardIter* pCI); // prototype

    void vValidate(void);
  };

  Card_Limit* _pCard_Limit(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
