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

    Weaver(Bench* pBench, PipelineDef* pPipelineDef);
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

    void vArgAbsJoin     (String* psOpArgs, Card_AbsJoin*   pCard_AbsJoin);
    void vArgAggreg      (String* psOpArgs, Card_Aggreg*    pCard_Aggreg);
    void vArgCast        (String* psOpArgs, Card_Cast*      pCard_Next);
    void vArgCrossJoin   (String* psOpArgs, Card_CrossJoin* pCard_CrossJoin);
    void vArgFilter      (String* psOpArgs, Card_Filter*    pCard_Filter);
    void vArgGbyAgg      (String* psOpArgs, Card_GbyAgg*    pCard_AggGby);
    void vArgHash        (String* psOpArgs, Card_Hash*      pCard_Hash);
    void vArgHaving      (String* psOpArgs, Card_Having*    pCard_Having);
    void vArgIndexJoin   (String* psOpArgs, Card_IndexJoin* pCard_IndexJoin);
    void vArgIndexScan   (String* psOpArgs, Card_IndexScan* pCard_IndexScan);
    void vArgKeyExists   (String* psOpArgs, Card_KeyExists* pCard_KeyExists);
    void vArgKeyNotExists(String* psOpArgs, Card_KeyNotExists* pCard_KeyNotExists);
    void vArgLimit       (String* psOpArgs, Card_Limit*     pCard_Limit);
    void vArgOrdAgg      (String* psOpArgs, Card_OrdAgg*    pCard_OrdAgg);
    void vArgProject     (String* psOpArgs, Card_Project*   pCard_Project);
    void vArgScan        (String* psOpArgs, Card_Scan*      pCard_Scan);
    void vArgSend        (String* psOpArgs, Card_Send*      pCard_send);
    void vArgSort        (String* psOpArgs, Card_Sort*      pCard_Sort);
    void vArgTable       (String* psOpArgs, Card_Table*     pCard_Table);
    void vArgTop         (String* psOpArgs, Card_Top*       pCard_op);
    void vArgWhere       (String* psOpArgs, Card_Where*     pCard_Where);

    void vConcat_cmd(Stz* aFormat, ...);
    void vConcat_cmd_vargs(Stz* aFormat, va_list xArgs);

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
