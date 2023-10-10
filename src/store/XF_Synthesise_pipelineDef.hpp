/*============================================================================*/
/* XF_Synthesise_pipelineDef.hpp -- Synthesise PipelineDef                    */
/*============================================================================*/
#pragma once

#include "XF_SYNTHESISE.hpp"

#include "Bench.hpp"
#include "CardIter.hpp"

#include "PipelineDef.hpp"

namespace STORE
{
  class XF_Synthesise_pipelineDef : public XF_SYNTHESISE
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
    
    XF_Synthesise_pipelineDef(CardIter* pCI);
   ~XF_Synthesise_pipelineDef(void) noexcept(false);

    static void vSynthesise(CardIter* pCI);

    PipelineDef* pPipelineDef(void);
    void vPipelineDef(PipelineDef* pPipelineDef);

    void vSynthesise(void);

  private:
    void vCraftPipelineDef(void);
    void vCraftStageDef(void);
    void vCraftOpDef(void);

    void vCraftArgDefs(void);
    void vCraftIndexes(void);
    void vCraftIndexDefs(void);
    void vCraftStreamDefs(void);
    void vCraftColInj(void);
    void vCraftColInj(ColInj* pColInj, Card *pSubCard);
    void vCraftColInj_FACETS(ColInj* pColInj, Card *pSubCard);
    void vCraftColPpg(void);
    void vCraftColPpg(ColPpg* pColPpg, Card *pSubCard);

  protected:
    Footer* aFooter(void);
  };

  XF_Synthesise_pipelineDef* _pXF_Synthesise_pipelineDef(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
