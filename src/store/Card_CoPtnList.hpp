/*============================================================================*/
/* Card_CoPtnList.hpp -- Co-Partition List                                    */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_CoPtnList : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_CoPtnList(void);
   ~Card_CoPtnList(void) noexcept(false);
  };

  Card_CoPtnList* _pCard_CoPtnList(void* aAddr);
  bool _bIsCard_CoPtnList(void* aAddr);
  Card_CoPtnList* _pGetSub_CoPtnList(Card* pParent);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
