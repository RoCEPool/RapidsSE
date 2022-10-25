/*============================================================================*/
/* Card_PtnColList.hpp -- Partition Column List                               */
/*============================================================================*/
#pragma once

#include "Card_StmCol.hpp"

namespace STORE
{
  class Card_PtnColList : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_PtnColList(void);
    ~Card_PtnColList(void) noexcept(false);
  };

  Card_PtnColList* _pCard_PtnColList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
