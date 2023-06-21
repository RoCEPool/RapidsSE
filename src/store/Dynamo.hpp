/*============================================================================*/
/* Dynamo.hpp -- Dynamo (pipeline plan compiler)                              */
/*============================================================================*/
#pragma once

#include "RootNode.hpp"
#include "GraphNav.hpp"
#include "QueryNode.hpp"
#include "ExtendedText.hpp"

namespace STORE
{
  class ParamsSpec;

  class Dynamo : public ListNL_t<VertexSpec>
  {  
    friend class STORE::VertexSpec;

  private:
    TagSet*      m_p_JobTags;        // tag set from job
    Stz*         m_szPipelineStmt;   // statement being compiled
    
    VertexSpec*  m_p_VertexSpecRoot; // the resulting spec
    
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    
    Scanner*     m_pScanner;         // statement token scanner
 
    VertexSpec*  m_pCurrVertexSpec;  // current/working spec

    ParamsSpec*  m_pParamsSpec;      // set of parameters

    bool         m_bHasCode;
    ExtendedText* m_pCode;
    
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Dynamo(TagSet* p_JobTags);
   ~Dynamo(void) noexcept(false);

    static void vRepair(Object* pObject);
    static VertexSpec* pToVertexSpec(Stz* szPipelineStmt, TagSet* p_JobTags);

    ExtendedText* pCodeBlock(void);

    VertexSpec* pCompile(Stz* szPipelineStmt);

    Stz* szCode(void);
    Stz* szName(void);

  private:
    Footer* aFooter(void);
    
    
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    //                            PIPELINE COMPILER
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

    bool bCompile(void);

  private:
  
    //
    // Pass 1 : Collect
    //
    bool bPass1_collect(void);

    bool bCollectPipeline(void);
    bool bCollectTag(void);
    bool bCollectParams(void);
    bool bCollectBeginEnd(void);
    bool bCollectCode(void);
    bool bCollectSubPipeline(void);
    bool bCollectStage(void);
    bool bCollectTopology(StageSpec* p_StageSpec);
    bool bCollectOpSequence(StageSpec* p_StageSpec);
    bool bCollectOp(StageSpec* p_StageSpec);
    bool bCollectOpArgs(StageSpec* p_StageSpec, OpSpec* pOpSpec);
    bool bCollectOpArgsExpr(StageSpec* p_StageSpec, OpSpec* pOpSpec,
                            String* pArgVal);

    bool bRecogniseOnAll(StageSpec* p_StageSpec);
    bool bRecogniseOnAllNodes(StageSpec* p_StageSpec);
    bool bRecogniseOnAllPartitions(StageSpec* p_StageSpec);
    bool bRecogniseOnNextNode(StageSpec* p_StageSpec);
    bool bRecogniseOnOrigin(StageSpec* p_StageSpec);

    bool bTokenMaybeOp(Token* pTkn);
    bool bTokenMaybeOpArg(Token* pTkn);


    //
    // Pass 2 : Piping
    //
    bool bPass2_piping(void);

    void vNameIsStage(StageSpec* pStageSpec,
                      OpSpec*    pOpSpec_nameOfStage,
                      StageSpec* pNextStageSpec);
    void vNameIsOp(StageSpec* pStageSpec,
                   OpSpec*    pOpSpec_nameOfOp);

    void vFirstSend(StageSpec* pSrcStageSpec,
                    OpSpec*    pSrcOpSpec_nameOfStage,
                    StageSpec* pDstStageSpec);
    void vMorphToSend(OpSpec* pOpSpec,
                      eYX eCrossTopology,
                      eYS eIntraTopology,
                      i8 i_NextStage);
    void vOtherSend(StageSpec* pSrcStageSpec,
                    OpSpec*    pSrcOpSpec_nameOfStage,
                    StageSpec* pOtherStageSpec,
                    OpSpec*    pOtherOpSpec);

    void vArgAdd_sends(OpSpec* pConsumeOpSpec, eYX eCrossTopology);
    void vArgUpdate_sends(OpSpec* pConsumeOpSpec, eYX eCrossTopology);


    void vAddConsumeOp(StageSpec* pStageSpec, eYX eCrossTopology);

    Stz* szStageNm(i8 i_Stage);
  
    //
    // Pass 3 : Includes
    //
    bool bPass3_includes(void);
  
    bool bPass3_include(Stz* szIncludePipNm);
    bool bPass3_includeFirst(void);
    bool bPass3_includeLast(void);

    //
    // token scanning
    //
    Token*     pTkn(void);
    TokenVec*  pTknVec(void);

    void vTokenAccept(void);



    bool bHasCode(void);
    void vHasCode(bool bHasCode);

    void vCode(Stz* szCode);


  };

  Dynamo* _pDynamo(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
