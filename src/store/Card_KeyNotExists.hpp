/*============================================================================*/
/* Card_KeyNotExists.hpp -- Key Not Exists                                    */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_KeyNotExists : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_KeyNotExists(void);
   ~Card_KeyNotExists(void) noexcept(false);

    void vComputeProjection(CardIter* pCI); // prototype

  };

  Card_KeyNotExists* _pCard_KeyNotExists(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
