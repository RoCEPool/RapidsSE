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

    void vAssign_predCmp_leftwards(void);
    void vConvert_calc_to_calcBool(void);
    void vCopy_predCmp_leftwards(void);
    void vCull_indexJoin_filterPred(void);
    void vDelete_empty_filter(void);
    void vDelete_empty_sub_filter(void);
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

    Footer* aFooter(void);
  };

  Filtrate* _pFiltrate(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
