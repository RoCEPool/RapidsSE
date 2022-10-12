/*============================================================================*/
/* Card_Filter.hpp -- Filter                                                 */
/*============================================================================*/
#pragma once

#include "Card.hpp"
#include "Card_PRED.hpp"

namespace STORE
{
  class Card_Filter : public Card
  {
  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Card_Filter(void);
   ~Card_Filter(void) noexcept(false);

    void vToExpr(Stz* szExpr);
  
    bool bCull(void);
    void vCullPred(char cWithMark);
    void vCalc_to_calcBool(void);

    Card_CmpPred* pFindBound(Index* pIndex, ePB eBound);

    bool bHasBounds(Index* pIndex);

    bool bIsEmptyFilter(void);

    void vReduceFilter(void);
    bool bRemoveBound(Card_CmpPred* pCard_PredCmp);

    Card* pTo_CalcBool(Card* pCard_Calc);

  };

  Card_Filter* _pCard_Filter(void* aAddr);
  bool _bIsCard_Filter(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
