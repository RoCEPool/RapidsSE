/*============================================================================*/
/* Dynamo.hpp -- Dynamo (dynamic plan compiler)                               */
/*============================================================================*/
#pragma once

namespace Pipelines
{
  class Dynamo : public Object
  {
  private:
  
    TokenVec*    m_p_TknVec;
  
    PipelineSpec* m_pPipelineSpec;
    StageSpec*   m_pStageSpec;
    OpSpec*      m_pOpSpec;
  
    i8           m_i_Tkn;
    Token*       m_pTkn;

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Dynamo(TokenVec* pTknVec);
   ~Dynamo(void) noexcept(false);
  
    static ePX eTopoloyX(StageSpec*  pFromStageSpec, StageSpec* pToStageSpec);
    
    void vAcceptToken(void);
    void vBeginToken(i8 i_Tkn);
    bool bCompile(void);
    bool bCollectName(void);
    bool bCollectBegin(void);
    bool bCollectStage(void);
    bool bCollectTopology(void);
    bool bCollectPipe(void);
    bool bCollectOp();
    bool bCollectEnd(void);
    void vDynamicSpec(PipelineSpec* pPipelineSpec);
    bool bIsOperator(Token* pTkn);
    bool bPhase1_collect(void);
    bool bPhase2_staging(void);
    bool bPhase3_flatten(void);
    bool bPhase4_opdefn(void);

    PipelineSpec* pPipelineSpec(void);
    void vReplaceWithXmit(OpSpec* pOpSpec, ePX eTopologyX, i8 i_Stage);

    Footer* aFooter(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
