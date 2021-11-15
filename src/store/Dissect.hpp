/*============================================================================*/
/* Dissect.hpp -- Dissect                                                     */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "Card.hpp"
#include "Card_Aggreg.hpp"
#include "Card_AggregList.hpp"
#include "Card_AggregPred.hpp"
#include "Card_AndPred.hpp"
#include "Card_As.hpp"
#include "Card_Atom.hpp"
#include "Card_AtomList.hpp"
#include "Card_By.hpp"
#include "Card_ByList.hpp"
#include "Card_ComparePred.hpp"
#include "Card_CorrSubquery.hpp"
#include "Card_CreateTable.hpp"
#include "Card_DropTable.hpp"
#include "Card_Expr.hpp"
#include "Card_ExprList.hpp"
#include "Card_Filter.hpp"
#include "Card_FilterList.hpp"
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
#include "Card_OriginCast.hpp"
#include "Card_OrPred.hpp"
#include "Card_Project.hpp"
#include "Card_ProjectList.hpp"
#include "Card_Scan.hpp"
#include "Card_Sort.hpp"
#include "Card_SortList.hpp"
#include "Card_Stage.hpp"
#include "Card_StreamDef.hpp"
#include "Card_Table.hpp"
#include "Card_TableList.hpp"
#include "Card_Type.hpp"

#include "PipelineSpec.hpp"
#include "StageSpec.hpp"
#include "OpSpec.hpp"


#include "ShardDef.hpp"

#include "Eval_Filter.hpp"

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

    bool m_bSeenAgg;

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
    void vDissectConsume(OpSpec* pOpSpec);
    void vDissectFilter(OpSpec* pOpSpec);
    void vDissectIndexJoin(OpSpec* pOpSpec);
    void vDissectIndexScan(OpSpec* pOpSpec);
    void vDissectLimit(OpSpec* pOpSpec);
    void vDissectOutput(OpSpec* pOpSpec);
    void vDissectPulse(OpSpec* pOpSpec);
    void vDissectProject(OpSpec* pOpSpec);
    void vDissectScan(OpSpec* pOpSpec);
    void vDissectSend(OpSpec* pOpSpec);
    void vDissectSink(OpSpec* pOpSpec);
    void vDissectSort(OpSpec* pOpSpec);
    void vDissectStart(OpSpec* pOpSpec);

    Card_AggregList*  pAnalyseAggreg(Card_AtomList* pCard_Expr);
    Card_ByList*      pAnalyseBy(Card_AtomList* pCard_Expr);
    Card_Filter*      pAnalyseFilter(Card_AtomList* pCard_Expr);
    Card_ProjectList* pAnalyseProject(Card_AtomList* pCard_Expr);
  
    Card_AtomList* pAtomiseSeq(Stz* szExpr);
    void           vEvaluateExpr(Card_Expr* pCard_Expr);
    void           vEvaluateFilter(Card_Filter* pCard_Filter);
    Index*         pFindIndex(Table* pTable, Stz* szTableNm);
    Table*         pFindTable(Stz* szTableNm);
    void           vSeparateAtoms(Card* pCard, Stz* szExpr);

  private:
    Footer* aFooter(void);
  };

  Dissect* _pDissect(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
