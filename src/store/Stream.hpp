/*============================================================================*/
/* Stream.hpp -- Stream (of Stage of Deck of Vertex of Pipeline)              */
/*============================================================================*/
#pragma once


namespace STORE { class Op; class StageSpec; }

namespace STORE
{
  class Stream : public VectorNL_t<CN_STREAM,Op,C_MAX_OPS>
  {
  private:
    Stage*     m_pStage;      // which stage of pipeline
    StageSpec* m_pStageSpec;  // stage specification for this stream

    i8    m_i_Stream;         // [stream] in stage
 
    PtnId m_tPtnId;           // this stream processes m_tPtnId in grid
                              //   will be C_UNKNOWN for stage with node topology

    i8    m_iNodeNo;          // this stream processes NodeNo in grid
                              //   is PtnMap::i_PtnNoToNode(m_tPtnId);

    // -- -- -- -- -- --

    s32  m_sStreamNm;         // syntehised name of this stream

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Stream(Stage*     pStage,
           StageSpec* pStageSpec,
           i8         i_Stream,
           PtnId      tPtnId,
           i8         iNodeNo);
   ~Stream(void) noexcept(false);
    
    i8   iNodeNo(void);
    i8   i_Stream(void);

    Op*  pOp0(void);
    Pipeline* pPipeline(void);

    PtnId  tPtnId(void);

    void vSetInvoker(Task* pInvoker);
    Stage* pStage(void);
    StageSpec* pStageSpec(void);
    void vSpawnOpTasks(void);
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
