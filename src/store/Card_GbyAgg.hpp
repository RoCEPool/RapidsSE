/*============================================================================*/
/* Card_GbyAgg.hpp -- Aggreg-GroupBy                                          */
/*============================================================================*/
#pragma once

#include "Card_AGGREGATE.hpp"

namespace STORE
{
  class CarryForward;

  class Card_GbyAgg : public Card_AGGREGATE
  {  
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_GbyAgg(void);
   ~Card_GbyAgg(void) noexcept(false);

    void vDerive_streamKey(CardIter* pCI);


    void vInject_foldExpr(CardIter* pCI);

    bool bMorph_gbyAgg_To_ordAgg(CardIter* pCI);

    void vSmooth(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_GbyAgg* _pCard_GbyAgg(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
