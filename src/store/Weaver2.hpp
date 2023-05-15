/*============================================================================*/
/* Weaver2.hpp -- Weaver of Cards (for CS2)                                   */
/*============================================================================*/
#pragma once

#include "Board.hpp"
#include "OptimiserTool.hpp"
#include "PipelineDef.hpp"

namespace STORE
{
  class Weaver2 : public OptimiserTool
  {
  protected:

    PipelineDef* m_pPipelineDef;

    i8 m_i_Stage;
    i8 m_i_Op_stage;
    s48K m_sOptStmt;

  private:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);

    Weaver2(Bench* pBench, PipelineDef* pPipelineDef);
   ~Weaver2(void) noexcept(false);
  
    void vWeaveOptStmt(Board* pBoard);
    Stz* szOptStmt(void);

  private:
  
    void vPipeline    (void);
    void vTags        (void);
    void vBegin       (void);
    void vEnd         (void);

    void vCard        (Card* pCard);
  
    void vStageFirst  (void);
    void vStageNext   (Card_Stage* pCard_Stage);
    void vStageLast   (void);

    void vOp          (Card* pCard);

    void vArgAbsJoin     (Stz* szOpArgs,  Card_AbsJoin*   pCard_AbsJoin);
    void vArgAggreg      (Stz* szOpArgs,  Card_Aggreg*    pCard_Aggreg);
    void vArgCast        (Stz* szOpArgs, Card_Cast*      pCard_Next);
    void vArgCombined    (Stz* szOpArgs,  Card_Combined*  pCard_Combine);
    void vArgCrossJoin   (Stz* szOpArgs,  Card_CrossJoin* pCard_CrossJoin);
    void vArgFilter      (Stz* szOpArgs,  Card_Filter*    pCard_Filter);
    void vArgGbyAgg      (Stz* szOpArgs,  Card_GbyAgg*    pCard_AggGby);
    void vArgHash        (Stz* szOpArgs,  Card_Hash*      pCard_Hash);
    void vArgHaving      (Stz* szOpArgs,  Card_Having*    pCard_Having);
    void vArgIndexJoin   (Stz* szOpArgs,  Card_IndexJoin* pCard_IndexJoin);
    void vArgIndexScan   (Stz* szOpArgs,  Card_IndexScan* pCard_IndexScan);
    void vArgKeyExists   (Stz* szOpArgs,  Card_KeyExists* pCard_KeyExists);
    void vArgKeyNotExists(Stz* szOpArgs,  Card_KeyNotExists* pCard_KeyNotExists);
    void vArgLimit       (Stz* szOpArgs,  Card_Limit*     pCard_Limit);
    void vArgOrdAgg      (Stz* szOpArgs,  Card_OrdAgg*    pCard_OrdAgg);
    void vArgProject     (Stz* szOpArgs,  Card_Project*   pCard_Project);
    void vArgScan        (Stz* szOpArgs,  Card_Scan*      pCard_Scan);
    void vArgSort        (Stz* szOpArgs,  Card_Sort*      pCard_Sort);
    void vArgWhere       (Stz* szOpArgs,  Card_Where*     pCard_Where);

    void vConcat(Stz* aFormat, ...);
    void vConcat_vargs(Stz* aFormat, va_list xArgs);

    void vEolOp(void);
    void vEolStage(void);

    void vPreStage(void);
  
    void vTabOp(void);
    void vTabStage(void);

  private:
    Footer* aFooter(void);
  };

  Weaver2* _pWeaver2(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
