/*============================================================================*/
/* Card_KeyExists.hpp -- Key Exists                                           */
/*============================================================================*/
#pragma once

#include "Card_OP.hpp"

namespace STORE
{
  class Card_KeyExists : public Card_OP
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_KeyExists(void);
   ~Card_KeyExists(void) noexcept(false);

    void vComputeProjection(CardIter* pCI); // prototype

    static void vTableMorphsToKeyExists(CardIter* pCI);
  };

  Card_KeyExists* _pCard_KeyExists(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
