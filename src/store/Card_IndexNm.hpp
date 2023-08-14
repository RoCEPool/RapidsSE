/*============================================================================*/
/* Card_IndexNm.hpp -- Index Name                                             */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"

namespace STORE
{
  class Card_IndexNm : public Card_symbol<Card_IndexNm>
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_IndexNm(void);
   ~Card_IndexNm(void) noexcept(false);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_IndexNm* _pCard_IndexNm(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
