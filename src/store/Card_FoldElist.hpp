/*============================================================================*/
/* Card_FoldElist.hpp -- Fold Typed Expression                                 */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_FoldElist : public Card
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_FoldElist(void);
   ~Card_FoldElist(void) noexcept(false);
  };

  Card_FoldElist* _pCard_FoldElist(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
