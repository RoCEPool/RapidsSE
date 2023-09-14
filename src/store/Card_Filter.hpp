/*============================================================================*/
/* Card_Filter.hpp -- Filter                                                 */
/*============================================================================*/
#pragma once

#include "Card_CONDITION.hpp"
#include "Card_CONJUNCTION.hpp"

namespace STORE
{
  class Card_Filter : public Card_CONDITION
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

    void vAddPred(Card_CONJUNCTION *p_PRED);

    void vComputeProjection(CardIter* pCI); // prototype
    void vCorrectStmOdn(Card_Projection* p_projection_prior);

    i8 nCullPred(Stz* szWithMark);

    void vEarlyProject_add_transients(CardIter* pCI);
    void vEarlyProject_recurse_update(CardIter* pCI);

    bool bIsEmpty(void);

    void vUpdate_projection(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Filter* _pCard_Filter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
