/*============================================================================*/
/* Filtrate.hpp --                                                            */
/*============================================================================*/
#pragma once

#include "TransformerTool.hpp"

namespace STORE
{
  class Filtrate : public TransformerTool
  {
  private:

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Filtrate(Bench* pBench);
  
   ~Filtrate(void) noexcept(false);

    void vBurstLeftwards(void);
    void vCalc_to_calcBool(void);
    void vDeleteEmpty_filter(void);
    void vRemove_predCmp(void);

  private:
    void vBurstFilterLeftwards(Card_Filter* pCard_filter);
    void vPredDrillDown(Card_PRED*, Card_Filter* pCard_filter);
    i8   iMovePredLeft(Card_PRED* pCard_PRED, Card_Filter* pCard_filter);

    i8   iMovePredToAbsJoin(Card_AbsJoin* pCard_absJoin, Card_PRED* pCard_PRED);
    i8   iMovePredToScan(Card_Scan* pCard_Scan, Card_PRED* pCard_PRED);

    i8   iIsPredMovableToAbsJoin(Card_AbsJoin* pCard_absJoin, Card_PRED* pCard_PRED);
    i8   iIsPredMovableToScan(Card_Scan* pCard_scan, Card_PRED* pCard_PRED);

    void vPredMovesToAbsJoin(Card* pPredCard, Card_AbsJoin* pCard_absJoin);
    void vPredMovesToScan(Card* pPredCard, Card_Scan* pCard_scan);

    void vMarkPredCardsAsMoved(Card* pPredCard);

    void vCalc_to_calcBool(Card* pCard_Filter);
    Card* pTo_CalcBool(Card* pCard_Calc);

    Footer* aFooter(void);
  };

  Filtrate* _pFiltrate(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
