/*============================================================================*/
/* Stage.hpp -- Pipe Stage                                                    */
/*============================================================================*/
#pragma once

namespace Pipelines
{
  class Stage : public VectorNL_t<CN_STAGE,Division,C_MAX_STAGES>
  {
  private:
    s32         m_sStageNm;   // stage name
    StageSpec*  m_pStageSpec; // specification of this stage
    Pipeline*   m_pPipeline;  // which pipeline
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Stage(StageSpec* pStageSpec, Pipeline* pPipeline);
   ~Stage(void) noexcept(false);
  
    void vDump(Pipeline* pPipeline);
    
    void      vCancelOperators(i8 iThisNode);
    Division* pGetNodeDivision(i8 i_Nde);
    i8        nOpsOnNode(i8 iNode);
    Op*       pPipeOutOp(ePX eTopologyX, i8 iNde, i8 iPtn, i8 iOp);
    Stz*      szStageNm(void);
    void      vStartPipeline(void);
  
  private:
    void vCreateDivisions(void);

    Footer* aFooter(void);
  };
  
  Stage* _pStage(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
