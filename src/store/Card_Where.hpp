/*============================================================================*/
/* Card_Where.hpp -- Where                                                    */
/*============================================================================*/
#pragma once

#include "Card_symbol.hpp"
#include "Card_CONDITION.hpp"

namespace STORE
{
  class CarryForward;

  class Card_Where : public Card_CONDITION // todo [Ray A 147]
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Where(void);
   ~Card_Where(void) noexcept(false);

    static void vCullMarkedCmpConj(CardIter* pCI);
    static void vDeleteEmpyWhere(CardIter* pCI);
    static void vMarkWherePredWrtTable(CardIter* pCI);
    static void vMoveWherePredsToTableFilter(CardIter* pCI);

    static Card_Where* pNewWhereAssembly(void);

   void vEarlyProject_add_transients(CardIter* pCI);
    void vEarlyProject_recurse_update(CardIter* pCI);
    void vSmooth(CardIter* pCI, CarryForward* pCarryForward);

    void vUpdate_emitted(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Where* _pCard_Where(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
