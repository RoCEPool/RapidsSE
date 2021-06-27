/*============================================================================*/
/* Dynamo.hpp -- Dynamo (pipeline plan compiler)                              */
/*============================================================================*/
#pragma once

#include "RootNode.hpp"
#include "GraphNav.hpp"
#include "QueryNode.hpp"

using namespace Parser;

namespace STORE
{
  class Dynamo : public Object
  {
  private:
    Job*          m_p_PipelineJob;  // job this dynamo is associated with
    
    TokenVec*     m_pTknVec;        // tokenised for of the pipeline statement
    i8            m_i_Tkn;          // current token
    Token*        m_pTkn;           // current token

    PipelineSpec* m_pPipelineSpec;  // pipeline spec being formed
    StageSpec*    m_pStageSpec;     // current stage spec being formed
  
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Dynamo(Stz* szPipelineStmt, Job* p_PipelineJob);
   ~Dynamo(void) noexcept(false);

    static bool bCmdToPipeline(String* pAsPipelineStmt, TokenVec* pFromTokenVec);
    static ePX eTopoloyX(StageSpec* pFromStageSpec, StageSpec* pToStageSpec);

    bool bCompile(void);
    PipelineSpec* pPipelineSpec(void);
    void vShow(void);

  private:

    //
    // phase 1
    //
    bool bPhase1_collect(void);
  
    bool bCollectBegin(void);
    bool bCollectName(void);
    bool bCollectTag(void);
    bool bCollectStage(void);
    bool bCollectTopology(void);
    bool bCollectPipe(void);
    bool bCollectOp(void);
    bool bCollectEnd(void);
  
    void vAcceptToken(void);
    void vBeginToken(i8 i_Tkn);
    bool bIsOperator(Token* pTkn);

    bool bParseOnAll(void);
    bool bParseOnAllNodes(void);
    bool bParseOnAllPartitions(void);
    bool bParseOnNode(void);
    bool bParseOnOrigin(void);
    bool bParseOnPartition(void);
    
    //
    // phase 2
    //
    bool bPhase2_staging(void);
  
    void vAddTopologyParams(OpSpec* pOpSpec, ePX eCrossTopo, i8 i_Stage, i8 i_Op);
    void vAddConsumeParams(OpSpec* pOpSpec, ePX eCrossTopo);
    void vAddToConsumeParams(OpSpec* pOpSpec, ePX eCrossTopo);
    void vReplaceTopologyParams(OpSpec* pOpSpec, ePX eCrossTopo, i8 i_Stage, i8 i_Op);
    void vReplaceWithSend(OpSpec* pOpSpec, ePX eCrossTopo, i8 i_Stage);

    Footer* aFooter(void);
  };

  Dynamo* _pDynamo(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
