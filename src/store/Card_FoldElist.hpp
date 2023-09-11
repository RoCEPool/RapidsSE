/*============================================================================*/
/* Card_FoldElist.hpp -- Fold Typed Expression                                 */
/*============================================================================*/
#pragma once

#include "Card_EXPRLIST.hpp"

namespace STORE
{
  class Card_FoldElist : public Card_EXPRLIST
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_FoldElist(void);
   ~Card_FoldElist(void) noexcept(false);


    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_FoldElist* _pCard_FoldElist(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
