/*============================================================================*/
/* Dissect.hpp -- Dissect                                                     */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "eTE.h"

#include "Card.hpp"

#include "Card_AggElist.hpp"
#include "Card_Aggreg.hpp"
#include "Card_PredAnd.hpp"
#include "Card_As.hpp"

#include "Card_ExprBool.hpp"

#include "Card_ExprCalc.hpp"
#include "Card_CandIndex.hpp"
#include "Card_Combine.hpp"
#include "Card_PredCmp.hpp"

#include "Card_Debug.hpp"

#include "Card_EBound.hpp"
#include "Card_Exists.hpp"

#include "Card_Filter.hpp"

#include "Card_GbyElist.hpp"

#include "Card_Hash.hpp"
#include "Card_Having.hpp"

#include "Card_Index.hpp"
#include "Card_IndexJoin.hpp"
#include "Card_IndexPred.hpp"
#include "Card_IndexScan.hpp"

#include "Card_LBound.hpp"
#include "Card_Limit.hpp"

#include "Card_On.hpp"
#include "Card_OrdAgg.hpp"
#include "Card_PredOr.hpp"

#include "Card_Project.hpp"
#include "Card_ProjElist.hpp"

#include "Card_RBound.hpp"
#include "Card_RpnTerms.hpp"

#include "Card_SbyElist.hpp"
#include "Card_Scan.hpp"
#include "Card_Send.hpp"
#include "Card_Sort.hpp"
#include "Card_SortDirn.hpp"
#include "Card_Stage.hpp"
#include "Card_StreamDef.hpp"

#include "Card_Table.hpp"
#include "Card_Term.hpp"
#include "Card_Type.hpp"
#include "Card_Expr.hpp"

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

    Card_AggElist*  pAggElist   (Card_RpnTerms* pCard_RpnTerms);
    Card_GbyElist*  pGbyElist   (Card_RpnTerms* pCard_RpnTerms);
    Card_ProjElist* pProjElist  (Card_RpnTerms* pCard_RpnTerms);
    Card_SbyElist*  pSbyElist   (Card_RpnTerms* pCard_RpnTerms);

    Card_Expr*      pExpr_b     (Card_RpnTerms* pCard_RpnTerms);
    Card_Expr*      pExpr_c     (Card_RpnTerms* pCard_RpnTerms);
    Card_ExprBool*  pExprBool   (Card_RpnTerms* pCard_RpnTerms);
    Card_ExprCalc*  pExprCalc   (Card_RpnTerms* pCard_RpnTerms);
  
    void vPredOr  (Card_ExprBool* pCard_ExprBool,
                   Card_RpnTerms* pCard_RpnTerms);
    void vPredAnd (Card_ExprBool* pCard_ExprBool,
                   Card_RpnTerms* pCard_RpnTerms);
    void vPredCmp (Card_ExprBool* pCard_ExprBool,
                   Card_RpnTerms* pCard_RpnTerms);
  
    Card_As*       pCollectAs       (Card_RpnTerms* pCard_RpnTerms);
    Card_SortDirn* pCollectSortDirn (Card_RpnTerms* pCard_RpnTerms);
    Card_Type*     pCollectType     (Card_RpnTerms* pCard_RpnTerms);

    Card_RpnTerms* pToRpnTerms(Stz* szRpnExpr);

  private:
    Footer* aFooter(void);
  };

  Dissect* _pDissect(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
