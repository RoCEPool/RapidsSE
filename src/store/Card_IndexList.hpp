/*============================================================================*/
/* Card_IndexList.hpp -- Index List                                           */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_IndexList : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_IndexList(void);
   ~Card_IndexList(void) noexcept(false);

    static Card_IndexList* pDerive(Table* pTable);
    Card_Index* pFindChosenIndex(void);
  };

  Card_IndexList* _pCard_IndexList(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
