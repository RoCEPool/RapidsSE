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
    DeckSpec*    m_pDeckSpec;
    StageSpec*   m_pStageSpec;
    OpSpec*      m_pOpSpec;

    bool m_bSawAggOp;
    bool m_bSawProjectOp;

  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Dissector(Bench* pBench);
   ~Dissector(void) noexcept(false);
  
    void vDissectSpec(VertexSpec* pPipeSpec);
  
  private:
    void vSubPipe(i8 i_SubPipeSpec);
    void vStage(i8 i_StageSpec);
    void vOp(i8 i_OpSpec);

    Card* pOp_AbsJoin     (TagSet* pArgTags);
    Card* pOp_Aggreg      (TagSet* pArgTags);
    Card* pOp_Call        (TagSet* pArgTags);
    Card* pOp_Combined    (TagSet* pArgTags);
    Card* pOp_Consume     (TagSet* pArgTags);
    Card* pOp_Debug       (TagSet* pArgTags);
    Card* pOp_Filter      (TagSet* pArgTags);
    Card* pOp_GbyAgg      (TagSet* pArgTags);
    Card* pOp_Hash        (TagSet* pArgTags);
    Card* pOp_IndexJoin   (TagSet* pArgTags);
    Card* pOp_IndexScan   (TagSet* pArgTags);
    Card* pOp_KeyExists   (TagSet* pArgTags);
    Card* pOp_KeyNotExists(TagSet* pArgTags);
    Card* pOp_Limit       (TagSet* pArgTags);
    Card* pOp_OrdAgg      (TagSet* pArgTags);
    Card* pOp_Output      (TagSet* pArgTags);
    Card* pOp_Pulse       (TagSet* pArgTags);
    Card* pOp_Project     (TagSet* pArgTags);
    Card* pOp_Scan        (TagSet* pArgTags);
    Card* pOp_Send        (TagSet* pArgTags);
    Card* pOp_Sink        (TagSet* pArgTags);
    Card* pOp_Sort        (TagSet* pArgTags);
    Card* pOp_Start       (TagSet* pArgTags);

    Card_As*         pPopAs         (Card_RpnTerms* pCard_RpnTerms);
    Card_Direction*  pPopDirection  (Card_RpnTerms* pCard_RpnTerms);
    Card_Expr*       pPopExpr       (Card_RpnTerms* pCard_RpnTerms, bool bIsKeyExpr, i8& ri_Expr);
    Card_Key*        pPopKey        (Card_RpnTerms* pCard_RpnTerms, bool bAsc);
    Card_Projection* pPopProjection (Card_RpnTerms* pCard_RpnTerms);
    Card_RpnCalc*    pPopRpnCalc    (Card_RpnTerms* pCard_RpnTerms);
    Stz*             szPopType      (Card_RpnTerms* pCard_RpnTerms);

    void             vCheckArgs     (TagSet* pArgTags, i8 nArgs);

    i8               iGetInt        (TagSet* pArgTags, i8 i_Arg);
    Card_RpnTerms*   pGetRpnTerms   (TagSet* pArgTags, i8 i_Arg);
    Stz*             szGetStz       (TagSet* pArgTags, i8 i_Arg);

    Card_RpnTerms*   pToRpnTerms    (Stz* szRpnExpr);

  private:
    Footer* aFooter(void);
  };

  Dissector* _pDissector(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
