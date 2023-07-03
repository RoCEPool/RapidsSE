/*============================================================================*/
/* XF_Make_Code.hpp -- Call Synthesiser                                           */
/*============================================================================*/
#pragma once

#include "Bench.hpp"
#include "CardIter.hpp"

#include "PipelineDef.hpp"

namespace STORE
{
  class XF_Make_Code : public Object
  {
  private:

    CardIter*    m_pCI;

    PipelineDef*  m_pPipelineDef;
    StageDef*     m_pStageDef;
    OpDef*        m_pOpDef;

    StreamDef*    m_pStreamDef_prior;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    XF_Make_Code(CardIter* pCI);
   ~XF_Make_Code(void) noexcept(false);

    PipelineDef* pPipelineDef(void);
    void vPipelineDef(PipelineDef* pPipelineDef);

    void vSynthesise(void);

  private:
    void vCraftPipelineDef(Board* pBoard);

    void vCraftStageDef(Card_Stage*p_stage);
    void vCraftOpDef(Card_OP* p_OP);
    void vCraftArgDefs(Card_OP* p_OP);

    void vCraftIndexes(Card_OP* p_OP);

    void vCraftIndexDefs(Card_OP* p_OP);
    void vCraftStreamDefs(Card_OP* p_OP);

    void vUpdateColsReq(Card_OP* p_OP);

    void vUpdateColsReq_filter(Card_Table* p_table, ColsReq* pColsReq,
                               Card_Filter* p_filter);
    void vUpdateColsReq_project(Card_Table* p_table, ColsReq* pColsReq,
                                Card_Project* p_project);

    void vUpdateColsReq_terms(Card_Table* p_table,
                              ColsReq* pColsReq, Card* pSubCard);

  protected:
    Footer* aFooter(void);
  };

  XF_Make_Code* _pXF_Make_Code(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
