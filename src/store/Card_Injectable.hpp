/*============================================================================*/
/* Card_Injectable.hpp -- Injectable columns                                  */
/*============================================================================*/
#pragma once

#include "Card_EXPRLIST.hpp"
#include "ColMap.hpp"

namespace STORE
{
  class Card_Injectable : public Card_EXPRLIST
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Injectable(void);
   ~Card_Injectable(void) noexcept(false);

    static Card_Injectable* pNew_injectable(CardIter* pCI);

    void vShortForm(String* pString);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Injectable* _pCard_Injectable(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
