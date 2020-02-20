/*============================================================================*/
/* Dynamo.hpp -- Dynamo (pipeline plan compiler)                               */
/*============================================================================*/
#pragma once

namespace Pipelines
{
  class Dynamo : public Object
  {
  private:
  
    Job*          m_p_Job;
    TokenVec*     m_pTknVec;
  
    PipelineSpec* m_pPipelineSpec;
    StageSpec*    m_pStageSpec;
  
    i8            m_i_Tkn;
    Token*        m_pTkn;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Dynamo(Stz* sCmd, Job* pJob);
   ~Dynamo(void) noexcept(false);
  
    static ePX eTopoloyX(StageSpec*  pFromStageSpec, StageSpec* pToStageSpec);
    
    void vAcceptToken(void);
    void vBeginToken(i8 i_Tkn);
    bool bCompile(void);
    bool bCollectBegin(void);
    bool bCollectName(void);
    bool bCollectTag(void);
    bool bCollectStage(void);
    bool bCollectTopology(void);
    bool bCollectPipe(void);
    bool bCollectOp(void);
    bool bCollectEnd(void);
    bool bIsOperator(Token* pTkn);
    bool bPhase1_collect(void);
    bool bPhase2_staging(void);

    PipelineSpec* pPipelineSpec(void);
    void vShow(void);

  private:
    void vAddStageParams(OpSpec* pOpSpec, ePX eTopologyX, i8 i_Stage, i8 i_Op);
    void vReplaceStageParams(OpSpec* pOpSpec, ePX eTopologyX, i8 i_Stage, i8 i_Op);
    void vReplaceWithXmit(OpSpec* pOpSpec, ePX eTopologyX, i8 i_Stage);
    
    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
