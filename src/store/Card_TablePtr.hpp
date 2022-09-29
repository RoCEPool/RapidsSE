/*============================================================================*/
/* Card_TablePtr.hpp -- Table                                                    */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_IndexList;

  class Card_TablePtr : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_TablePtr(Table* pTable);
   ~Card_TablePtr(void) noexcept(false);

    Card_IndexList* pNew_indexList(void);
  };

  Card_TablePtr* _pCard_TablePtr(void* aAddr);
  bool _bIsCard_TablePtr(void* aAddr);
  Card_TablePtr* _pGetSub_Card_TablePtr(Card * pParent);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
