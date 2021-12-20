/*============================================================================*/
/* Stream.hpp -- Stream (instance of StageSpec within a Pipeline)             */
/*============================================================================*/
#pragma once

#include "LifeGuard_t.hpp"


namespace STORE { class Op; class StageSpec; }

namespace STORE
{
  class Stream : public VectorNL_t<CN_STREAM,Op,C_MAX_OPS>,
                 public LifeGuard_t<Stream>
  {
  private:
    s32  m_sPipelineNm; // pipeline name
    s32  m_sStageNm;    // stage name within pipeline
    s32  m_sStreamNm;   // name of this stream
    i8   m_i_Stream;    // [stream] in stage

    StageSpec* m_pStageSpec;  // stage specification for this stream
    Stage*     m_pStage;      // which stage of pipeline
    Pipeline*  m_pPipeline;   // which pipeline
    PtnId m_tPtnId;           // this stream processes m_tPtnId in grid
                              //   will be C_UNKNOWN for stage with node topology

    i8   m_i_Node;            // this stream processes m_i_Node in grid
                              //   is PtnMap::i_PtnNoToNode(m_tPtnId);

    // -- -- -- -- -- --

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
   Stream(StageSpec* pStageSpec, Stage* pStage,
           i8 i_Stream, PtnId tPtnId, i8 i_Node);

   ~Stream(void) noexcept(false);
    
    i8   i_Node(void);
    i8   i_Stream(void);

    Pipeline* pPipeline(void);

    PtnId  tPtnId(void);

    Stage* pStage(void);
    StageSpec* pStageSpec(void);
    void vSpawnOpTasks(i8 iSessionId, i8 iNode);
    Stz* szStreamNm(void);
    Stz* szStageNm(void);
    void vWalk(void* aArg);
    void vWalkLambda(
           i8 iDepth,
           std::function<void(Object* pObject, i8 iDepth)> f_vClosure);

    void vCreateOps();
  private:
    void vSynthesiseName(void);
    Footer* aFooter(void);
  };
  
  Stream* _pStream(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
