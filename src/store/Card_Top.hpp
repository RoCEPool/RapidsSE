/*============================================================================*/
/* Card_Top.hpp -- Top                                                        */
/*============================================================================*/
#pragma once

#include "Card_OP.hpp"

namespace STORE
{
  class Card_Top : public Card_OP
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Top(Stz* szRows);
   ~Card_Top(void) noexcept(false);

    void vComputeProjection(CardIter* pCI); // prototype

    void vValidate(i8 iDepth);
  };

  Card_Top* _pCard_Top(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
