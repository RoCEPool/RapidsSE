/*============================================================================*/
/* Dissect.hpp -- Dissect                                                     */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "eTE.h"

#include "Card.hpp"

#include "Card_AggElist.hpp"
#include "Card_Aggreg.hpp"
#include "Card_AndPred.hpp"
#include "Card_As.hpp"

#include "Card_BoolExpr.hpp"

#include "Card_CalcExpr.hpp"
#include "Card_CandIndex.hpp"
#include "Card_Combine.hpp"
#include "Card_CmpPred.hpp"
#include "Card_CorrSubquery.hpp"
#include "Card_CreateTable.hpp"

#include "Card_Debug.hpp"
#include "Card_DropTable.hpp"

#include "Card_EBound.hpp"
#include "Card_Exists.hpp"

#include "Card_FiltElist.hpp"
#include "Card_Filter.hpp"

#include "Card_GbyElist.hpp"

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

#include "Card_Project.hpp"

#include "Card_RBound.hpp"

#include "Card_Scan.hpp"
#include "Card_Send.hpp"
#include "Card_Sort.hpp"
#include "Card_Stage.hpp"
#include "Card_StreamDef.hpp"

#include "Card_Table.hpp"
#include "Card_Term.hpp"
#include "Card_TermSeq.hpp"
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

    Card_As*        pCollectAs         (Card_TermSeq* pCard_TermSeq);
    Card_BoolExpr*  pCollectBoolExpr   (Card_TermSeq* pCard_TermSeq);
    Card_CalcExpr*  pCollectCalcPred   (Card_TermSeq* pCard_TermSeq);
    Card_GbyElist*   pCollectGbyList    (Card_TermSeq* pCard_TermSeq);
    Card_Type*      pCollectType       (Card_TermSeq* pCard_TermSeq);
    Card_TypedExpr* pCollectTypedExpr  (Card_TermSeq* pCard_TermSeq);

    void            vEvalOrPred        (Card_BoolExpr* pCard_BoolExpr,
                                        Card_TermSeq* pCard_TermSeq);
    void            vEvalAndPred       (Card_BoolExpr* pCard_BoolExpr,
                                        Card_TermSeq* pCard_TermSeq);
    void            vEvalComparePred   (Card_BoolExpr* pCard_BoolExpr,
                                        Card_TermSeq* pCard_TermSeq);
  
    Card_TermSeq*  pToTermList(Stz* szExpr);

  private:
    Footer* aFooter(void);
  };

  Dissect* _pDissect(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
