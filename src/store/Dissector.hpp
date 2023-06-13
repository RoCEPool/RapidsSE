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
    TagSet*      m_pOpArgs;

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

    Card* pOp_AbsJoin     (void);
    Card* pOp_Aggreg      (void);
    Card* pOp_Call        (void);
    Card* pOp_Combined    (void);
    Card* pOp_Consume     (void);
    Card* pOp_Debug       (void);
    Card* pOp_Filter      (void);
    Card* pOp_GbyAgg      (void);
    Card* pOp_Hash        (void);
    Card* pOp_IndexJoin   (void);
    Card* pOp_IndexScan   (void);
    Card* pOp_KeyExists   (void);
    Card* pOp_KeyNotExists(void);
    Card* pOp_Limit       (void);
    Card* pOp_OrdAgg      (void);
    Card* pOp_Output      (void);
    Card* pOp_Pulse       (void);
    Card* pOp_Project     (void);
    Card* pOp_Scan        (void);
    Card* pOp_Send        (void);
    Card* pOp_Sink        (void);
    Card* pOp_Sort        (void);
    Card* pOp_Start       (void);

    Card_Index*      pFormIndex(i8 i_Arg);
    Card_Table*      pFormTable(i8 i_Arg);

    Card_As*         pPopAs         (Card_RpnTerms* pCard_RpnTerms);
    Card_Direction*  pPopDirection  (Card_RpnTerms* pCard_RpnTerms);
    Card_Expr*       pPopExpr       (Card_RpnTerms* pCard_RpnTerms, bool bIsKeyExpr, i8& ri_Expr);
    Card_Key*        pPopKey        (Card_RpnTerms* pCard_RpnTerms, bool bAsc);
    Card_Projection* pPopProjection (Card_RpnTerms* pCard_RpnTerms);
    Card_RpnCalc*    pPopRpnCalc    (Card_RpnTerms* pCard_RpnTerms);
    Stz*             szPopType      (Card_RpnTerms* pCard_RpnTerms);

    void             vCheckArgs     (i8 nArgs);

    i8               iGetInt        (i8 i_Arg);
    Card_RpnTerms*   pGetRpnTerms   (i8 i_Arg);
    Stz*             szGetStz       (i8 i_Arg);

    Card_RpnTerms*   pToRpnTerms    (Stz* szRpnExpr);

  private:
    Footer* aFooter(void);
  };

  Dissector* _pDissector(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
