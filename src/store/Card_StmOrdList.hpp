/*============================================================================*/
/* Card_StmOrdList.hpp -- Stream Ordered List                                 */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_StmOrdList : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_StmOrdList(void);
    ~Card_StmOrdList(void) noexcept(false);
  };

  Card_StmOrdList* _pCard_StmOrdList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
