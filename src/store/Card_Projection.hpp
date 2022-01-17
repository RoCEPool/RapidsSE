/*============================================================================*/
/* Card_Projection.hpp -- Project Card                                           */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"
#include "Table.hpp"

namespace STORE
{
  class Card_Projection : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Projection(void);
   ~Card_Projection(void) noexcept(false);
  };

  Card_Projection* _pCard_Projection(void* aAddr);
  bool _bIsCard_Projection(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
