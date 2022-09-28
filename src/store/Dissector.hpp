/*============================================================================*/
/* Dissector.hpp -- Dissector                                                     */
/*============================================================================*/
#pragma once

#include "OptimiserTool.hpp"

namespace STORE { class Bench; class Board; class Optimiser; }

namespace STORE
{
  class Dissector : public OptimiserTool
  {
  protected:

    VertexSpec*  m_pPipeSpec;
    StageSpec*   m_pStageSpec;
    OpSpec*      m_pOpSpec;

    bool m_bSawAggOp;

  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Dissector(Bench* pBench);
   ~Dissector(void) noexcept(false);
  
    Board* pDissectSpec(VertexSpec* pPipeSpec);
  
  private:
    void vPipeline(void);
    void vStage(i8 i_StageSpec);
    void vOp(i8 i_OpSpec);

    Card* pCard_AbsJoin  (TagSet* pArgTags);
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
    Card_Calc*      pExprCalc   (Card_RpnTerms* pCard_RpnTerms);
  
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

    Stz*           szArgsStz     (TagSet* pArgTags, i8 i_Arg);
    Card_RpnTerms* pArgsRpnTerms (TagSet* pArgTags, i8 i_Arg);

  private:
    Footer* aFooter(void);
  };

  Dissector* _pDissector(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
