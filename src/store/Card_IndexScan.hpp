/*============================================================================*/
/* Card_IndexScan.hpp -- Index Scan                                           */
/*============================================================================*/
#pragma once

#include "Card_OP.hpp"

namespace STORE
{
  class Card_IndexScan : public Card_OP
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_IndexScan(void);
   ~Card_IndexScan(void) noexcept(false);

    static void vIndividuateFilter(CardIter* pCI);

    void vDerive_Projection_Key(CardIter* pCI);

    void vEarlyProject_leaf(CardIter* pCI);

    void vValidate(void);
  };

  Card_IndexScan* _pCard_IndexScan(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
