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
    Stz*         m_szOpNm;
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

    Card* pOp_absJoin     (void);
    Card* pOp_aggreg      (void);
    Card* pOp_call        (void);
    Card* pOp_combined    (void);
    Card* pOp_consume     (void);
    Card* pOp_debug       (void);
    Card* pOp_filter      (void);
    Card* pOp_gbyAgg      (void);
    Card* pOp_hash        (void);
    Card* pOp_indexJoin   (void);
    Card* pOp_indexScan   (void);
    Card* pOp_keyExists   (void);
    Card* pOp_keyNotExists(void);
    Card* pOp_limit       (void);
    Card* pOp_ordAgg      (void);
    Card* pOp_output      (void);
    Card* pOp_pulse       (void);
    Card* pOp_project     (void);
    Card* pOp_scan        (void);
    Card* pOp_send        (void);
    Card* pOp_sink        (void);
    Card* pOp_sort        (void);
    Card* pOp_start       (void);
    Card* pOp_top         (void);

    Card_IndexList*  pFormIndex(Stz* szIndexNm);
    Card_Table*      pFormTable(Stz* szTableNm);

    Card_As*         pPopAs         (Card_RpnExpr* pCard_RpnTerms);
    Card_Bounds*     pPopBounds     (Card_RpnExpr* pCard_RpnTerms);
    Card_Bound*      pPopBound      (Card_RpnExpr* p_rpnExpr);
    Card_Constraint* pPopConstraint (Card_RpnExpr* pCard_RpnTerms);
    Card_Direction*  pPopDirection  (Card_RpnExpr* pCard_RpnTerms);
    Card_Expr*       pPop_expr       (Card_RpnExpr* p_rpnTerms,
                                      bool bIsKeyExpr, i8& ri_Expr,
                                      bool bEnsureDirection);
    Card_HshCol*     pPopHshCol     (Card_RpnExpr* pCard_RpnTerms);
    Card_HshColList* pPopHshColList (Card_RpnExpr* pCard_RpnTerms);
    Card_Key*        pPopKey        (Card_RpnExpr* pCard_RpnTerms,
                                     bool bEnsureDirection);
    Card_Emit*       pPopEmit       (Card_RpnExpr* p_rpnTerms);
    Card_RpnCalc*    pPopRpnCalc    (Card_RpnExpr* pCard_RpnTerms);
    eCT              ePopType       (Card_RpnExpr* pCard_RpnTerms);

    void             vCheckArgs     (i8 nArgs);

    i8               iGetInt        (i8 i_Arg);
    Card_RpnExpr*    pGet_rpnExpr   (i8 i_Arg);
    Stz*             szGetStz       (i8 i_Arg);

    Card_RpnExpr*   pToRpnExpr    (Stz* szRpnExpr);

  private:
    Footer* aFooter(void);
  };

  Dissector* _pDissector(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
