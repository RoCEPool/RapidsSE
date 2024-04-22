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
    void vTags        (void);
    void vBegin       (void);
    void vEnd         (void);
    void vCppBlock    (void);
    void vSemicolon   (void);

    void vCard        (Card* pCard);
  
    void vStageFirst  (void);
    void vStageNext   (Card_Stage* pCard_Stage);
    void vStageLast   (void);

    void vDynamic     (StageDef* pStageDef);

    void vOp          (Card* pCard, bool bCommented=false);

    void vArg_absJoin     (String* psOpArgs, Card_AbsJoin*   pCard_AbsJoin);
    void vArg_aggreg      (String* psOpArgs, Card_Aggreg*    pCard_Aggreg);
    void vArg_cast        (String* psOpArgs, Card_Cast*      pCard_Next);
    void vArg_crossJoin   (String* psOpArgs, Card_CrossJoin* pCard_CrossJoin);
    void vArg_filter      (String* psOpArgs, Card_Filter*    pCard_Filter);
    void vArg_gbyAgg      (String* psOpArgs, Card_GbyAgg*    pCard_AggGby);
    void vArg_hash        (String* psOpArgs, Card_Hash*      pCard_Hash);
    void vArg_having      (String* psOpArgs, Card_Having*    pCard_Having);
    void vArg_indexJoin   (String* psOpArgs, Card_IndexJoin* pCard_IndexJoin);
    void vArg_indexScan   (String* psOpArgs, Card_IndexScan* pCard_IndexScan);
    void vArg_inflate     (String* psOpArgs, Card_Inflate*   pCard_Inflate);
    void vArg_keyExists   (String* psOpArgs, Card_KeyExists* pCard_KeyExists);
    void vArg_keyNotExists(String* psOpArgs, Card_KeyNotExists* pCard_KeyNotExists);
    void vArg_limit       (String* psOpArgs, Card_Limit*     pCard_Limit);
    void vArg_ordAgg      (String* psOpArgs, Card_OrdAgg*    pCard_OrdAgg);
    void vArg_pdsRow      (String* psOpArgs, Card_PdsRow*    pCard_pdsRow);
    void vArg_project     (String* psOpArgs, Card_Project*   pCard_Project);
    void vArg_scan        (String* psOpArgs, Card_Scan*      pCard_Scan);
    void vArg_send        (String* psOpArgs, Card_Send*      pCard_send);
    void vArg_sort        (String* psOpArgs, Card_Sort*      pCard_Sort);
    void vArg_table       (String* psOpArgs, Card_Table*     pCard_Table);
    void vArg_top         (String* psOpArgs, Card_Top*       pCard_op);
    void vArg_where       (String* psOpArgs, Card_Where*     pCard_Where);

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
