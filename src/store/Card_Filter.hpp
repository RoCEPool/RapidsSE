/*============================================================================*/
/* Card_Filter.hpp -- Filter                                                 */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card_UNIT.hpp"
#include "Card_PRED.hpp"

namespace STORE
{
  class Card_Filter : public Card_UNIT
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Filter(void);
   ~Card_Filter(void) noexcept(false);

    void vToExpr(Stz* szExpr);
  
    Card_ComparePred* pFindBound(Index* pIndex, eBK eBound);
    bool bHasBounds(Index* pIndex);
    void vReduceFilter(void);
    bool bRemoveBound(Card_ComparePred* pCard_ComparePred);
  };

  Card_Filter* _pCard_Filter(void* aAddr);
  bool _bIsCard_Filter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
