/*============================================================================*/
/* Stream.hpp -- Stream (instance of StageSpec within a Pipeline)             */
/*============================================================================*/
#pragma once

#include "DeleteHelper.hpp"


namespace STORE { class Op; class StageSpec; }

namespace STORE
{
  class Stream : public VectorNL_t<CN_STREAM,Op,C_MAX_OPS>,
                 public DeleteHelper
  {
  private:
    s32  m_sPipelineNm; // pipeline name
    s32  m_sStageNm;    // stage name within pipeline
    s32  m_sStreamNm;   // name of this stream
    i8   m_i_Stream;    // [stream] in stage

    StageSpec* m_pStageSpec;  // stage specification for this stream
    Stage*     m_pStage;      // which stage of pipeline
    Pipeline*  m_pPipeline;   // which pipeline

    tPN  m_tPtnNo; // this stream processes m_tPtnNo in grid
                   //   will be C_UNKNOWN for stage with node topology
    i8   m_i_Node; // this stream processes m_i_Node in grid
                   //   is PtnMap::i_PtnNoToNode(m_tPtnNo);
 
    // -- -- -- -- -- --

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Stream(StageSpec* pStageSpec, Stage* pStage,
           i8 i_Stream, tPN tPtnNo, i8 i_Node);
   ~Stream(void) noexcept(false);
    
    i8   i_Node(void);
    i8   i_Stream(void);

    void vDeleteThis(Object* pDeleter, Stz* szFunc);
    Pipeline* pPipeline(void);
    tPN  tPtnNo(void);
  
    Stage* pStage(void);
    StageSpec* pStageSpec(void);
    void vSpawnOpTasks(i8 iSessionId, i8 iNode);
    Stz* szStreamNm(void);

  private:
    void vCreateOps();
    void vSynthesiseName(void);
    Footer* aFooter(void);
  };
  
  Stream* _pStream(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
