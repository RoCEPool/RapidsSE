/*============================================================================*/
/* Dissect.hpp -- Dissect                                                     */
/*============================================================================*/
#pragma once

#include "BASE.h"

#include "eTE.h"

#include "Card.hpp"
#include "CardCtl.hpp"

#include "Card_AggElist.hpp"
#include "Card_AggLevel.hpp"
#include "Card_Aggreg.hpp"
#include "Card_As.hpp"

#include "Card_CandIndex.hpp"
#include "Card_Combine.hpp"

#include "Card_Debug.hpp"
#include "Card_Direction.hpp"

#include "Card_EBound.hpp"
#include "Card_Exists.hpp"
#include "Card_ExprBool.hpp"
#include "Card_ExprCalc.hpp"

#include "Card_Filter.hpp"

#include "Card_GbyElist.hpp"
#include "Card_Generate.hpp"

#include "Card_Hash.hpp"
#include "Card_Having.hpp"

#include "Card_Index.hpp"
#include "Card_IndexJoin.hpp"
#include "Card_IndexPred.hpp"
#include "Card_IndexScan.hpp"

#include "Card_LBound.hpp"
#include "Card_Limit.hpp"

#include "Card_Mode.hpp"

#include "Card_Name.hpp"

#include "Card_On.hpp"
#include "Card_Option.hpp"
#include "Card_OrdAgg.hpp"
#include "Card_ObyElist.hpp"

#include "Card_PredAnd.hpp"
#include "Card_PredCmp.hpp"
#include "Card_PredOr.hpp"
#include "Card_Project.hpp"
#include "Card_ProjElist.hpp"

#include "Card_RBound.hpp"
#include "Card_Rows.hpp"
#include "Card_RpnTerms.hpp"

#include "Card_SbyElist.hpp"
#include "Card_Scan.hpp"
#include "Card_Send.hpp"
#include "Card_SendKind.hpp"
#include "Card_Sends.hpp"
#include "Card_Sort.hpp"
#include "Card_Stage.hpp"
#include "Card_StageTopo.hpp"
#include "Card_Stream.hpp"
#include "Card_StreamElist.hpp"
#include "Card_SubSet.hpp"

#include "Card_Table.hpp"
#include "Card_Term.hpp"
#include "Card_Type.hpp"
#include "Card_Expr.hpp"

#include "Card_UBound.hpp"
#include "Card_Unique.hpp"

#include "PipeSpec.hpp"
#include "StageSpec.hpp"
#include "OpSpec.hpp"


namespace STORE { class Bench; class Board; class Optimiser; }

namespace STORE
{
  class Dissect : public Object
  {
  protected:
  
    Optimiser* m_pOptimiser;
    Bench*     m_pBench;
    Board*     m_pBoard;

    PipeSpec*  m_pPipeSpec;
    StageSpec* m_pStageSpec;
    OpSpec*    m_pOpSpec;

    bool m_bSawAggOp;

  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Dissect(Optimiser* pOptimiser);
   ~Dissect(void) noexcept(false);
  
    Board* pDissectSpec(PipeSpec* pPipeSpec);
  
  private:
    void vPipeline(void);
    void vStage(i8 i_StageSpec);
    void vOp(i8 i_OpSpec);

    Card* pCard_Aggreg   (TagSet* pArgTags);
    Card* pCard_Combined (TagSet* pArgTags);
    Card* pCard_Consume  (TagSet* pArgTags);
    Card* pCard_Debug    (TagSet* pArgTags);
    Card* pCard_Filter   (TagSet* pArgTags);
    Card* pCard_Hash     (TagSet* pArgTags);
    Card* pCard_IndexJoin(TagSet* pArgTags);
    Card* pCard_IndexScan(TagSet* pArgTags);
    Card* pCard_Limit    (TagSet* pArgTags);
    Card* pCard_OrdAgg   (TagSet* pArgTags);
    Card* pCard_Output   (TagSet* pArgTags);
    Card* pCard_Pulse    (TagSet* pArgTags);
    Card* pCard_Project  (TagSet* pArgTags);
    Card* pCard_Scan     (TagSet* pArgTags);
    Card* pCard_Send     (TagSet* pArgTags);
    Card* pCard_Sink     (TagSet* pArgTags);
    Card* pCard_Sort     (TagSet* pArgTags);
    Card* pCard_Start    (TagSet* pArgTags);

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
  
    Card_As*        pAs       (Card_RpnTerms* pCard_RpnTerms);
    Card_Direction* pDirection(Card_RpnTerms* pCard_RpnTerms);
    Card_Type*      pType     (Card_RpnTerms* pCard_RpnTerms);

    Card_RpnTerms* pToRpnTerms(Stz* szRpnExpr);
  
    i8             iArgCheckLast (TagSet* pArgTags, i8 i_Last);
    Card_RpnTerms* pArgCheckLast (TagSet* pArgTags, i8 i_Last);
    Card_RpnTerms* pArgCheckLast (TagSet* pArgTags, i8 i_Last,
                                  Stz*& szTableNm, Stz*& szIndexNm);
    Stz*           szArgCheckLast(TagSet* pArgTags, i8 i_Last);
    void           vArgCheck     (TagSet* pArgTags, i8 nArgs);
  
  private:
    Footer* aFooter(void);
  };

  Dissect* _pDissect(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
