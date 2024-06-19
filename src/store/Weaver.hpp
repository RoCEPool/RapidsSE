/*============================================================================*/
/* Weaver.hpp -- Weaver of Cards                                              */
/*============================================================================*/
#pragma once

#include "Board.hpp"
#include "OptimiserTool.hpp"
#include "PipelineDef.hpp"
#include "ExtendedText.hpp"

namespace STORE
{
  class Weaver : public OptimiserTool
  {
  protected:

    CardIter* m_pCI;

    PipelineDef* m_pPipelineDef;

    Card_Deck* m_pDeck;

    i8 m_i_Stage;
    i8 m_i_Op_stage;
    bool m_bOpPrinted;

    ExtendedText* m_pOptStmt;
    ExtendedText* m_pCodeBlock;

    bool m_bSawHash;
  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Weaver(CardIter* pCI, Bench* pBench, PipelineDef* pPipelineDef);
   ~Weaver(void) noexcept(false);
  
    void vWeaveOptStmt(Board* pBoard);
     ExtendedText* pOptStmt(void);
    void vOptStmt(ExtendedText* pOptSmt);

    ExtendedText* pCodeBlock(void);
    void vCodeBlock(ExtendedText* pCodeBlock);

  private:
  
    void vPipeline    (void);
		void vPtnConsume  (i8 iThreads);
		void vPtnSend     (void);
    void vTags        (void);
    void vBegin       (void);
    void vEnd         (void);
    void vCppBlock    (void);
    void vSemicolon   (void);

    void vCard        (Card* pCard);
  
    void vStageFirst  (void);
    void vStageNext   (Card_Stage* p_stage);
    void vStageLast   (void);

    void vDynamic     (StageDef* pStageDef);

    void vOp          (Card* pCard, bool bCommented=false);

    void vArg_absJoin     (String* psOpArgs, Card_AbsJoin*   p_absJoin);
    void vArg_aggreg      (String* psOpArgs, Card_Aggreg*    p_aggreg);
    void vArg_cast        (String* psOpArgs, Card_Cast*      p_next);
    void vArg_crossJoin   (String* psOpArgs, Card_CrossJoin* p_crossJoin);
    void vArg_filter      (String* psOpArgs, Card_Filter*    p_filter);
    void vArg_gbyAgg      (String* psOpArgs, Card_GbyAgg*    p_aggGby);
    void vArg_globalHash  (String* psOpArgs, Card_GlobalHash*p_globalHash);
    void vArg_hash        (String* psOpArgs, Card_Hash*      p_hash);
    void vArg_having      (String* psOpArgs, Card_Having*    p_having);
    void vArg_indexJoin   (String* psOpArgs, Card_IndexJoin* p_indexJoin);
    void vArg_indexScan   (String* psOpArgs, Card_IndexScan* p_indexScan);
    void vArg_inflate     (String* psOpArgs, Card_Inflate*   p_inflate);
    void vArg_keyExists   (String* psOpArgs, Card_KeyExists* p_keyExists);
    void vArg_keyNotExists(String* psOpArgs, Card_KeyNotExists* p_keyNotExists);
    void vArg_limit       (String* psOpArgs, Card_Limit*     p_limit);
    void vArg_ordAgg      (String* psOpArgs, Card_OrdAgg*    p_ordAgg);
    void vArg_pdsRow      (String* psOpArgs, Card_PdsRow*    p_pdsRow);
    void vArg_project     (String* psOpArgs, Card_Project*   p_project);
    void vArg_rrfJoin     (String* psOpArgs, Card_RrfJoin*   p_rrfJoin);
    void vArg_scan        (String* psOpArgs, Card_Scan*      p_scan);
    void vArg_send        (String* psOpArgs, Card_Send*      p_send);
    void vArg_sort        (String* psOpArgs, Card_Sort*      p_sort);
    void vArg_table       (String* psOpArgs, Card_Table*     p_table);
    void vArg_top         (String* psOpArgs, Card_Top*       p_op);
    void vArg_where       (String* psOpArgs, Card_Where*     p_where);

    void vConcat_cmd(Stz* aFormat, ...);
    void vConcat_cmd_vArg_s(Stz* aFormat, va_list xArgs);

    void vEolOp(void);
    void vEolStage(void);

    void vPreStage(void);
  
    void vTabOp(bool bCommented=false);
    void vTabStage(void);

  private:
    Footer* aFooter(void);
  };

  Weaver* _pWeaver(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
