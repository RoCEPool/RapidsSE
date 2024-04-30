/*============================================================================*/
/* Card_Filter.hpp -- Filter                                                 */
/*============================================================================*/
#pragma once

#include "Card_CONDITION.hpp"
#include "Card_CONJ.hpp"

namespace STORE
{
  class Card_Filter : public Card_CONDITION
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Filter(void);
   ~Card_Filter(void) noexcept(false);

    static void vDeleteEmptyFilters(CardIter* pCI);
    static void vIndividuate_filter(CardIter* pCI);
    void vUpdate_expCol(CardIter* pCI, Card_Emit* p_Dic_expr, bool bAlt=false);
    i8 nCullMarked_rlnConj(Stz* szWithMark);
    void vEarlyProject_add_transients(CardIter* pCI);
    bool bIsEmpty(void);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_Filter* _pCard_Filter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
