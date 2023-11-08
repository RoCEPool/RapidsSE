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

    Card_IndexList*  pForm_indexList(Stz* szIndexNm);
    Card_Table*      pForm_table    (Stz* szTableNm);

    Card_As*         pPop_as        (Card_RpnExpr* p_rpnExpr);
    Card_Bounds*     pPop_bounds    (Card_RpnExpr* p_rpnExpr);
    Card_Bound*      pPop_bound     (Card_RpnExpr* p_rpnExpr);
    Card_Constraint* pPop_constraint(Card_RpnExpr* p_rpnExpr);
    Card_Direction*  pPop_direction (Card_RpnExpr* p_rpnExpr);
    Card_Expr*       pPop_expr      (Card_RpnExpr* p_rpnExpr,
                                     bool bIsKeyExpr, i8& ri_Expr,
                                     bool bEnsureDirection);
    Card_HshCol*     pPop_hshCol    (Card_RpnExpr* p_rpnExpr);
    Card_HshColList* pPop_hshColList(Card_RpnExpr* p_rpnExpr);
    Card_Key*        pPop_key       (Card_RpnExpr* p_rpnExpr,
                                     bool bEnsureDirection);
    Card_Emit*       pPop_emit      (Card_RpnExpr* p_rpnExpr);
    Card_RpnCalc*    pPop_rpnCalc   (Card_RpnExpr* p_rpnExpr);
		TypeId   xPopColTypeId  (Card_RpnExpr* p_rpnExpr);

    void             vCheckArgs     (i8 nArgs);

    Card_RpnExpr*    pArg_rpnExpr   (i8 i_Arg);
    i8               iArgValue      (i8 i_Arg);
    Stz*             szArgValue     (i8 i_Arg);

    Card_RpnExpr*    pTo_rpnExpr    (Stz* szRpnExpr);

  private:
    Footer* aFooter(void);
  };

  Dissector* _pDissector(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
