/*============================================================================*/
/* Card_StmOrdOdn.hpp -- Stream Ordered Ordinal                               */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_StmOrdOdn : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_StmOrdOdn(void);
    ~Card_StmOrdOdn(void) noexcept(false);
  };

  Card_StmOrdOdn* _pCard_StmOrdOdn(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
