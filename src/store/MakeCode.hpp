/*============================================================================*/
/* MakeCode.hpp -- Call Synthesiser                                           */
/*============================================================================*/
#pragma once

#include "Bench.hpp"
#include "CardIter.hpp"

#include "PipelineDef.hpp"

namespace STORE
{
  class MakeCode : public Object
  {
  private:

    CardIter*    m_pCI;

    PipelineDef*  m_pPipelineDef;
    StageDef*     m_pStageDef;
    OpDef*        m_pOpDef;
    ArgDef*       m_pArgDef;

    OpDef*        m_pOpDef_leading;
    ColsReq*      m_pColsReq;

  protected:
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    MakeCode(CardIter* pCI);
   ~MakeCode(void) noexcept(false);

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

    void vUpdateColsReq_filter(ColsReq* pColsReq, Card_Filter* p_filter);
    void vUpdateColsReq_project(ColsReq* pColsReq, Card_Project* p_project);

    void vUpdateColsReq_terms(ColsReq* pColsReq, Card* pSubCard);

  protected:
    Footer* aFooter(void);
  };

  MakeCode* _pMakeCode(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
