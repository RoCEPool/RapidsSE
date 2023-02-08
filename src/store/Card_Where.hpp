/*============================================================================*/
/* Card_Where.hpp -- Where                                                    */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "Card_PRED.hpp"

namespace STORE
{
  class Card_Where : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Where(void);
   ~Card_Where(void) noexcept(false);

    static void vCullMarkedCmpPred(CardIter* pCI);
    static void vDeleteEmpyWhere(CardIter* pCI);
    static void vMarkWherePredWrtTable(CardIter* pCI);
    static void vMoveWherePredToTableFilter(CardIter* pCI);
    static void vMove_All_on_To_where(CardIter* pCI);
    static void vMoveWhereToTableAsConstraint(CardIter* pCI);

    static Card_Where* pNewWhereConstruct(void);

    void vBump_Rightwards(i8 nBumpBy);
    void vConvert_stmColOdn_terms_To_tblColOdn(CardIter* pCI);
    void vSmooth(CardIter* pCI, bool bMorph_bolPred_To_cmpPred);
  };

  Card_Where* _pCard_Where(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
