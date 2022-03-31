/*============================================================================*/
/* PipeSpec.hpp -- Pipe Specification                                         */
/*============================================================================*/
#pragma once

#include "eYS.h"


namespace STORE { class Dynamo;
                  class StageSpec; class DeckSpec;
                  class OpSpec; class SubPipeSpecs; }

namespace STORE
{
  class VertexSpec : public ListNL_t<VertexSpec>
                   , public ListItem_t<VertexSpec>
  {
    friend class STORE::Dynamo;

  private:
  
    VertexSpec*  m_pParentVertexSpec; // parent of this vertex
    s32 m_sVertexNm;  // aka sub-pipe name
    
    DeckSpec* m_pDeckSpec;
 
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  
    Scanner* m_p_Scanner; // statement token scanner
    TagSet*  m_p_JobTags; // job tag set
 
    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    VertexSpec(VertexSpec* pParentVertexSpec,
               Scanner*    p_Scanner,
               TagSet*     p_JobTags);
   ~VertexSpec(void) noexcept(false);
  
    bool bCompile(void);
  
    void vAddStageSpec(StageSpec* pStageSpec);
    DeckSpec* pDeckSpec(void);
    StageSpec* pFindStageSpec(Stz* szFindNm);
    TagSet* pJobTags(void);
    Stz* szVertexNm(void);
    i8 nStageSpecs(void);
    StageSpec* pStageSpec(i8 i_Stage);

    Footer* aFooter(void);
  
  
#ifndef WITH_DYNAMO_COMPILER

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    //                            PIPELINE COMPILER
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  private:
  
    //
    // Pass 1 : Collect
    //
    bool bPass1_collect(void);

    bool bCollectPipeline(void);
    bool bCollectTag(void);
    bool bCollectBeginEnd(void);
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

  };

  VertexSpec* _pVertexSpec(void* aAddr);
}

#endif

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
