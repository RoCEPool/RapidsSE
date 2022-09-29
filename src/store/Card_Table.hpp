/*============================================================================*/
/* Card_Table.hpp -- Table                                                    */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_IndexList;

  class Card_Table : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Table(void);
   ~Card_Table(void) noexcept(false);
  };

  Card_Table* _pCard_Table(void* aAddr);
  bool _bIsCard_Table(void* aAddr);
  Card_Table* _pGetSub_Card_Table(Card * pParent);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
