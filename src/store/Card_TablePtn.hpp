/*============================================================================*/
/* Card_TablePtn.hpp -- Table Partition                                       */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_TablePtn : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_TablePtn(void);
   ~Card_TablePtn(void) noexcept(false);
  };

  Card_TablePtn* _pCard_TablePtn(void* aAddr);
  bool _bIsCard_TablePtn(void* aAddr);
  Card_TablePtn* _pGetSub_Card_TablePtn(Card* pParent);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
