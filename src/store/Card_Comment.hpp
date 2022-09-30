/*============================================================================*/
/* Card_Comment.hpp -- Comment                                                */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_Comment : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Card_Comment(void);
    Card_Comment(Stz* szComment);
   ~Card_Comment(void) noexcept(false);
  };

  Card_Comment* _pCard_Comment(void* aAddr);
  bool _bIsCard_Comment(void* aAddr);
  Card_Comment* _pGetSub_Card_Comment(Card* pParent);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
