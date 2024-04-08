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

    CardIter*    m_pCI;

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
  
    Dissector(CardIter* pCI, Bench* pBench);
   ~Dissector(void) noexcept(false);
  
    void vDissectSpec(VertexSpec* pPipeSpec);
  
  private:
    void vSubPipe(i8 i_SubPipe);
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

    Card_Bounds*     pPop_bounds    (Card_TermSeq* p_termSeq);
    Card_Bound*      pPop_bound     (Card_TermSeq* p_termSeq);
    Cast             xPop_cast      (Card_TermSeq* p_termSeq);
    Card_Constraint* pPop_constraint(Card_TermSeq* p_termSeq);
    Card_Emit*       pPop_emit      (Card_TermSeq* p_termSeq);
    Card_Expr*       pPop_expr4     (Card_TermSeq* p_termSeq);
    Card_HshCol*     pPop_hshCol    (Card_TermSeq* p_termSeq);
    Card_HshColList* pPop_hshColList(Card_TermSeq* p_termSeq);
    Card_Key*        pPop_key       (Card_TermSeq* p_termSeq);
    Card_RpnCalc*    pPop_rpnCalc   (Card_TermSeq* p_termSeq);
    Card_On*         pPop_on        (Card_TermSeq* p_termSeq);

    Card_TermSeq*    pPop_exprSeq   (Card_TermSeq* p_termSeq);

    void             vCheckArgs     (i8 nArgs);

    Card_TermSeq*    pArg_termSeq   (i8 i_Arg);
    i8               iArg_value      (i8 i_Arg);
    Stz*             szArg_value     (i8 i_Arg);

    Card_TermSeq*    pTo_termSeq    (Stz* szTermSeq);

  private:
    Footer* aFooter(void);
  };

  Dissector* _pDissector(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
