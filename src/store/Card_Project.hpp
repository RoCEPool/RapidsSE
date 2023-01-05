/*============================================================================*/
/* Card_Project.hpp -- Project                                                */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Project : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Project(eCK eCard);
   ~Card_Project(void) noexcept(false);

    void vComputeProjection(CardIter* pCI); // prototype

  };

  Card_Project* _pCard_Project(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
