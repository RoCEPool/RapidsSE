/*============================================================================*/
/* Card_IndexJoin.hpp -- Index Join                                           */
/*============================================================================*/
#pragma once

#include "Card_OP.hpp"

namespace STORE
{
  class Card_IndexJoin : public Card_OP
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    static void vIndividuateFilter(CardIter* pCI);

    Card_IndexJoin(void);
   ~Card_IndexJoin(void) noexcept(false);

    static void vTableMorphsToIndexJoin(CardIter* pCI);

    void vDerive_Projection_Key(CardIter* pCI);

    void vEarly_project(CardIter* pCI);

    void vComputeProjection(CardIter* pCI); // prototype
  };

  Card_IndexJoin* _pCard_IndexJoin(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
