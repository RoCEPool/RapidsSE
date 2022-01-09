/*============================================================================*/
/* Dissect.hpp -- Dissect                                                     */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"
#include "Card_Aggreg.hpp"
#include "Card_AggregPred.hpp"
#include "Card_AndPred.hpp"
#include "Card_As.hpp"
#include "Card_Term.hpp"
#include "Card_TermList.hpp"
#include "Card_By.hpp"
#include "Card_ByList.hpp"
#include "Card_ByPred.hpp"
#include "Card_Combine.hpp"
#include "Card_ComparePred.hpp"
#include "Card_CorrSubquery.hpp"
#include "Card_CreateTable.hpp"
#include "Card_Debug.hpp"
#include "Card_DropTable.hpp"
#include "Card_Expr.hpp"
#include "Card_ExprList.hpp"
#include "Card_Filter.hpp"
#include "Card_FilterList.hpp"
#include "Card_Hash.hpp"
#include "Card_HashCast.hpp"
#include "Card_Having.hpp"
#include "Card_HavingList.hpp"
#include "Card_Index.hpp"
#include "Card_IndexJoin.hpp"
#include "Card_IndexJoinList.hpp"
#include "Card_IndexPred.hpp"
#include "Card_IndexScan.hpp"
#include "Card_IndexScanList.hpp"
#include "Card_Limit.hpp"
#include "Card_MainSubquery.hpp"
#include "Card_NodeCast.hpp"
#include "Card_NoncSubquery.hpp"
#include "Card_On.hpp"
#include "Card_OnList.hpp"
#include "Card_OrdAgg.hpp"
#include "Card_OriginCast.hpp"
#include "Card_OrPred.hpp"
#include "Card_Predicate.hpp"
#include "Card_Projection.hpp"
#include "Card_Project.hpp"
#include "Card_ProjectPred.hpp"
#include "Card_Properties.hpp"
#include "Card_Scan.hpp"
#include "Card_Send.hpp"
#include "Card_Sort.hpp"
#include "Card_SortList.hpp"
#include "Card_Stage.hpp"
#include "Card_StreamDef.hpp"
#include "Card_Table.hpp"
#include "Card_TableList.hpp"
#include "Card_Term.hpp"
#include "Card_TermList.hpp"
#include "Card_Type.hpp"

#include "PipelineSpec.hpp"
#include "StageSpec.hpp"
#include "OpSpec.hpp"


#include "ShardDef.hpp"


#include "Ribbon.hpp"

namespace STORE { class Bench; class Board;}

namespace STORE
{
  class Dissect : public Object
  {
  protected:
  
    Bench* m_pBench;
    Board* m_pBoard;
  
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
  
    Dissect(Bench* pBench);
   ~Dissect(void) noexcept(false);
  
    void vDissectSpec(PipelineSpec* pPipelineSpec);
    void vDissectPipeline(void);
    void vDissectStage(i8 i_StageSpec);
    void vDissectOp(i8 i_OpSpec);

    void vDissectAggreg(OpSpec* pOpSpec);
    void vDissectCombined(OpSpec* pOpSpec);
    void vDissectConsume(OpSpec* pOpSpec);
    void vDissectDebug(OpSpec* pOpSpec);
    void vDissectFilter(OpSpec* pOpSpec);
    void vDissectHash(OpSpec* pOpSpec);
    void vDissectIndexJoin(OpSpec* pOpSpec);
    void vDissectIndexScan(OpSpec* pOpSpec);
    void vDissectLimit(OpSpec* pOpSpec);
    void vDissectOrdAgg(OpSpec* pOpSpec);
    void vDissectOutput(OpSpec* pOpSpec);
    void vDissectPulse(OpSpec* pOpSpec);
    void vDissectProject(OpSpec* pOpSpec);
    void vDissectScan(OpSpec* pOpSpec);
    void vDissectSend(OpSpec* pOpSpec);
    void vDissectSink(OpSpec* pOpSpec);
    void vDissectSort(OpSpec* pOpSpec);
    void vDissectStart(OpSpec* pOpSpec);

    Card_Aggreg*  pAnalyseAggregExpr(Card_TermList* pCard_TermList);
    Card_Filter*      pAnalyseFilterExpr(Card_TermList* pCard_TermList);
    Card_Hash*        pAnalyseHashExpr(Card_TermList* pCard_TermList);
    Card_OrdAgg*  pAnalyseOrdAggExpr(Card_TermList* pCard_TermList);
    Card_Project* pAnalyseProjectExpr(Card_TermList* pCard_TermList);

    Card_As*          pCollectAs(Card_TermList* pCard_TermList);
    Card_ByList*      pCollectBy(Card_TermList* pCard_TermList);
    Card_Predicate*   pCollectPredicate(Card_TermList* pCard_TermList);
    Card_Type*        pCollectType(Card_TermList* pCard_TermList);

    eAL               eAggegLevel(void);
    void              vEvaluateFilter(Card_Filter* pCard_Filter);
    Card_TermList*    pToTermList(Stz* szExpr);

  private:
    Footer* aFooter(void);
  };

  Dissect* _pDissect(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
