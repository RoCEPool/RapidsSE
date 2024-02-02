/*============================================================================*/
/* Card_Scan.hpp -- Scan                                                      */
/*============================================================================*/
#pragma once

#include "Card_OP.hpp"

namespace STORE
{
  class Card_Scan : public Card_OP
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Scan(void);
   ~Card_Scan(void) noexcept(false);

    static void vIndividuateFilter(CardIter* pCI);
    static void vMorph_table_To_scan(CardIter* pCI);

    void vEarlyProject_leaf(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Scan* _pCard_Scan(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
