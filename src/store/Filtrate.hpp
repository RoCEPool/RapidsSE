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

    void vAssignLeftwards(void);
    void vCalc_to_calcBool(void);
    void vCopyLeftwards(void);
    void vCull_indexJoin_filterPred(void);
    void vDeleteEmpty_filter(void);
    void vDeleteEmpty_sub_filter(void);
    void vIndividuate_filter(void);

  private:
    void vAssignFilterLeftwards(Card_Filter* pCard_filter);
    void vAssignPredDrillDown(Card_PRED*, Card_Filter* pCard_filter);

    i8   iAssignPredLeft(Card_PRED* pCard_PRED, Card_Filter* pCard_filter);
    i8   iAssignPredToAbsJoin(Card_AbsJoin* pCard_absJoin, Card_PRED* pCard_PRED);
    i8   iAssignPredToScan(Card_Scan* pCard_scan, Card_PRED* pCard_PRED);

    void vCopyFilterLeftwards(Card_Filter* pCard_filter);
    bool bCopyPredDrillDown(Card_PRED*, Card_Filter* pCard_filter);
    void vCopyPredToExistingFilter(Card_Filter* pCard_filter_existing, Card_PRED* pCard_pred);
    void vCopyPredToNewFilter(Card_Filter* pCard_filter_new, Card_PRED* pCard_pred);

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
