/*============================================================================*/
/* Card_TblColOdn.hpp -- Stream Column Ordinal                                */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_TblColOdn : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_TblColOdn(void);
   ~Card_TblColOdn(void) noexcept(false);
  };

  Card_TblColOdn* _pCard_TblColOdn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
