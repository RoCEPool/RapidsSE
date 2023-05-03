/*============================================================================*/
/* Card_Filter.hpp -- Filter                                                 */
/*============================================================================*/
#pragma once

#include "Card_OP.hpp"
#include "Card_PRED.hpp"

namespace STORE
{
  class Card_Filter : public Card_OP
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Filter(void);
   ~Card_Filter(void) noexcept(false);

    static void vCullUsedBounds(CardIter* pCI);
    static void vDeleteEmptyFilters(CardIter* pCI);
    static void vIndividuate_filter(CardIter* pCI);
    static Card_Filter* pNew(Card_Constraint* p_constraint);

    void vAddPred(Card_PRED *p_PRED);

    void vComputeProjection(CardIter* pCI); // prototype
    void vConvert_stmColOdn_terms_To_tblColOdn(CardIter* pCI);

    void vCullPred(char cWithMark);
    bool bIsEmpty(void);

  };

  Card_Filter* _pCard_Filter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
