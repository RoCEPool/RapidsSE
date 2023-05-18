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
    static void vScanBecomesTable(CardIter* pCI);

    void vComputeProjection(CardIter* pCI); // prototype

    void vEarlyProject_leaf(CardIter* pCI);

    void vValidate(i8 iDepth, Card* pParent);
  };

  Card_Scan* _pCard_Scan(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
