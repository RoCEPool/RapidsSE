/*============================================================================*/
/* Dissect.hpp -- Dissect                                                     */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "eTE.h"

#include "Card.hpp"
#include "Card_Aggreg.hpp"
#include "Card_AggPred.hpp"
#include "Card_AndPred.hpp"
#include "Card_As.hpp"
#include "Card_By.hpp"
#include "Card_ByList.hpp"
#include "Card_ByPred.hpp"
#include "Card_CalcPred.hpp"
#include "Card_CandidateIndex.hpp"
#include "Card_Combine.hpp"
#include "Card_ComparePred.hpp"
#include "Card_CorrSubquery.hpp"
#include "Card_CreateTable.hpp"
#include "Card_Debug.hpp"
#include "Card_DropTable.hpp"
#include "Card_EBound.hpp"
#include "Card_Exists.hpp"
#include "Card_Expr.hpp"
#include "Card_Filter.hpp"
#include "Card_BoolPred.hpp"
#include "Card_Hash.hpp"
#include "Card_HashCast.hpp"
#include "Card_Having.hpp"
#include "Card_Index.hpp"
#include "Card_IndexJoin.hpp"
#include "Card_IndexPred.hpp"
#include "Card_IndexScan.hpp"
#include "Card_LBound.hpp"
#include "Card_Limit.hpp"
#include "Card_MainSubquery.hpp"
#include "Card_NodeCast.hpp"
#include "Card_NoncSubquery.hpp"
#include "Card_On.hpp"
#include "Card_OrdAgg.hpp"
#include "Card_OriginCast.hpp"
#include "Card_OrPred.hpp"
#include "Card_Predicate.hpp"
#include "Card_UBound.hpp"
#include "Card_Project.hpp"
#include "Card_RBound.hpp"
#include "Card_Scan.hpp"
#include "Card_Send.hpp"
#include "Card_Sort.hpp"
#include "Card_Stage.hpp"
#include "Card_StreamDef.hpp"
#include "Card_Table.hpp"
#include "Card_Term.hpp"
#include "Card_TermList.hpp"
#include "Card_Type.hpp"
#include "Card_TypedExpr.hpp"
#include "Card_UBound.hpp"

#include "PipelineSpec.hpp"
#include "StageSpec.hpp"
#include "OpSpec.hpp"


namespace STORE { class Bench; class Board; class Optimiser; }

namespace STORE
{
  class Dissect : public Object
  {
  protected:
  
    Bench*     m_pBench;
    Board*     m_pBoard;

    PipelineSpec* m_pPipelineSpec;
    StageSpec*    m_pStageSpec;
    OpSpec*       m_pOpSpec;

    bool m_bSawAggOp;

  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Dissect(void);
   ~Dissect(void) noexcept(false);
  
    void vDissectSpec(Bench* pBench, PipelineSpec* pPipelineSpec);
  
  private:
    void vDissectPipeline(void);
    void vDissectStage(i8 i_StageSpec);
    void vDissectOp(i8 i_OpSpec);

    void vDissectAggreg   (OpSpec* pOpSpec);
    void vDissectCombined (OpSpec* pOpSpec);
    void vDissectConsume  (OpSpec* pOpSpec);
    void vDissectDebug    (OpSpec* pOpSpec);
    void vDissectFilter   (OpSpec* pOpSpec);
    void vDissectHash     (OpSpec* pOpSpec);
    void vDissectIndexJoin(OpSpec* pOpSpec);
    void vDissectIndexScan(OpSpec* pOpSpec);
    void vDissectLimit    (OpSpec* pOpSpec);
    void vDissectOrdAgg   (OpSpec* pOpSpec);
    void vDissectOutput   (OpSpec* pOpSpec);
    void vDissectPulse    (OpSpec* pOpSpec);
    void vDissectProject  (OpSpec* pOpSpec);
    void vDissectScan     (OpSpec* pOpSpec);
    void vDissectSend     (OpSpec* pOpSpec);
    void vDissectSink     (OpSpec* pOpSpec);
    void vDissectSort     (OpSpec* pOpSpec);
    void vDissectStart    (OpSpec* pOpSpec);

    Card_Aggreg*    pConvertAggregExpr (Card_TermList* pCard_TermList);
    Card_Hash*      pConvertHashExpr   (Card_TermList* pCard_TermList);
    Card_OrdAgg*    pConvertOrdAggExpr (Card_TermList* pCard_TermList);
    Card_Project*   pConvertProjectExpr(Card_TermList* pCard_TermList);

    Card_AggPred*   pCollectAggPred    (Card_TermList* pCard_TermList);
    Card_As*        pCollectAs         (Card_TermList* pCard_TermList);
    Card_BoolPred*  pCollectBoolPred   (Card_TermList* pCard_TermList);
    Card_CalcPred*  pCollectCalcPred   (Card_TermList* pCard_TermList);
    Card_ByList*    pCollectGroupBy    (Card_TermList* pCard_TermList);
    Card_Type*      pCollectType       (Card_TermList* pCard_TermList);
    Card_TypedExpr* pCollectTypedExpr  (Card_TermList* pCard_TermList);

    void            vEvalOrPred        (Card_BoolPred*   pCard_BoolPred,
                                        Card_TermList*   pCard_TermList);
    void            vEvalAndPred       (Card_BoolPred*   pCard_BoolPred,
                                        Card_TermList*   pCard_TermList);
    void            vEvalComparePred   (Card_BoolPred*   pCard_BoolPred,
                                        Card_TermList*   pCard_TermList);
  
    Card_TermList*  pToTermList(Stz* szExpr);

  private:
    Footer* aFooter(void);
  };

  Dissect* _pDissect(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
