/*============================================================================*/
/* Tools.hpp -- Drafting Tools                                                */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Board.hpp"
#include "eBK.h"

namespace STORE { class Bench; }


namespace STORE
{
  class Tools : public Object
  {
  private:
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Tools(void);
   ~Tools(void) noexcept(false);
  
    Card_Scan* pFind_Scan_Filter(Board* pBoard);

    void vCullIndex(Board* pBoard, Card_CandidateIndex* pExcept);
    void vGenerativeStreamDefs(Bench* pBench);
    void vPickIndex(Board* pBoard);
    void vProjectiveStreamDefs(Bench* pBench);
    i8   iRankIndex(Index* pIndex, Card_ComparePred* pCard_ComparePred);
    void vStreamOrder(Bench* pBench);
    void vSubstituteOrdinals(Bench* pBench);
  
    bool bHasBounds        (Index* pIndex, Card_BoolPred*    pCard_BoolPred);
    bool bHasBounds_pred   (Index* pIndex, Card_PRED*        pCard_PRED);
    bool bHasBounds_or     (Index* pIndex, Card_OrPred*      pCard_OrPred);
    bool bHasBounds_and    (Index* pIndex, Card_AndPred*     pCard_AndPred);
    bool bHasBounds_compare(Index* pIndex, Card_ComparePred* pCard_ComparePred);

    Card_ComparePred* pFindBound(Index* pIndex,
                                 eBK eBound,
                                 Card_BoolPred* pCard_BoolPred
                                 );
    Card_ComparePred* pFindBound_pred   (Index* pIndex,
                                         eBK eBound,
                                         Card_PRED*        pCard_PRED);
    Card_ComparePred* pFindBound_or     (Index* pIndex,
                                         eBK eBound,
                                         Card_OrPred*      pCard_OrPred);
    Card_ComparePred* pFindBound_and    (Index* pIndex,
                                         eBK eBound,
                                         Card_AndPred*     pCard_AndPred);
    Card_ComparePred* pFindBound_compare(Index* pIndex,
                                         eBK eBound,
                                         Card_ComparePred* pCard_ComparePred);

    bool bRemoveBound        (Card_BoolPred*    pCard_BoolPred,
                              Card_ComparePred* pCard_ComparePred);
    bool bRemoveBound_pred   (Card_PRED*        pCard_PRED,
                              Card_ComparePred* pCard_ComparePred);
    bool bRemoveBound_or     (Card_OrPred*      pCard_OrPred,
                              Card_ComparePred* pCard_ComparePred);
    bool bRemoveBound_and    (Card_AndPred*     pCard_AndPred,
                              Card_ComparePred* pCard_ComparePred);
  
    void vDeleteComparePred  (Card_ComparePred* pCard_ComparePred);
    void vDeleteRecursive    (Card* pCard);
  
    void vReduceFilter       (Card_BoolPred*    pCard_BoolPred);
    void vReduceFilter_pred  (Card_PRED*        pCard_PRED);
    bool bReduciblePred      (Card_PRED*        pCard_PRED);
    void vReducePred         (Card_PRED*        pCard_PRED_parent,
                              Card_PRED*        pCard_PRED_child);


  private:
  
    static void vOrderStreamDef(StreamDef* pStreamDef,
                                Card_ByList* pCard_ByList);

    static StreamDef* pProduceStreamDef(Card_Project* pCard_Project);
  
    static void vReplaceOrdinals(Card_PRED* pCard_Pred,
                                 StreamDef* pStreamDef,
                                 Index*     pIndex=nullptr);
  
    Footer* aFooter(void);
  };

  Tools* _pTools(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
