/*============================================================================*/
/* Card_Where.hpp -- Where                                                    */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "CarryForward.hpp"

namespace STORE
{
  class CarryForward;

  class Card_Where : public Card_OP
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Where(void);
   ~Card_Where(void) noexcept(false);

    static void vCullMarkedCmpPred(CardIter* pCI);
    static void vDeleteEmpyWhere(CardIter* pCI);
    static void vMarkWherePredWrtTable(CardIter* pCI);
    static void vMoveWherePredsToTableFilter(CardIter* pCI);
    static void vMove_All_on_To_where(CardIter* pCI);

    static Card_Where* pNewWhereConstruct(void);

    void vConvert_stmColOdn_terms_To_tblColOdn(CardIter* pCI);
    void vEarlyProject_add_transients(CardIter* pCI);
    void vEarlyProject_recurse_passthru(CardIter* pCI);
    void vSmooth(CardIter* pCI, CarryForward* pCarryForward);

    void vValidate(i8 iDepth, Card* pParent);
  };

  Card_Where* _pCard_Where(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
