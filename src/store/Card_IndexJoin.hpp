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

    static void vMorph_table_To_indexJoin(CardIter* pCI);

    void vDerive_streamKey(CardIter* pCI);

    void vEarlyProject_add_transients(CardIter* pCI);

    void vEarlyProject_recurse_add(CardIter* pCI);
    bool bEarlyProject_root(CardIter* pCI);

    void vValidate(CardIter* pCI, i8 iDepth, Card* pParent);
  };

  Card_IndexJoin* _pCard_IndexJoin(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
