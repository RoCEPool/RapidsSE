/*============================================================================*/
/* Card_IndexScan.hpp -- Index Scan                                           */
/*============================================================================*/
#pragma once

#include "Card.hpp"

namespace STORE
{
  class Card_IndexScan : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_IndexScan(void);
   ~Card_IndexScan(void) noexcept(false);

    void vApplyStream(CardIter* pCI); // prototype
  };

  Card_IndexScan* _pCard_IndexScan(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
