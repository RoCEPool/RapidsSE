/*============================================================================*/
/* Stream.hpp -- Stream (of Stage of Deck of Vertex of Pipeline)              */
/*============================================================================*/
#pragma once


namespace STORE { class Deck; class Op; class StageSpec; }

namespace STORE
{
  class Stream : public VectorNL_t<CN_STREAM,Op,C_MAX_OPS>
  {
  private:
    Stage*     m_p_Stage;      // which stage of pipeline
    StageSpec* m_p_StageSpec;  // stage specification for this stream

    i8    m_i_Stream;         // [stream] in stage
 
    PtnId m_tPtnId;           // this stream processes m_tPtnId in grid
                              //   will be C_UNKNOWN for stage with node topology

    i8    m_iNodeId;          // this stream processes NodeId in grid
                              //   is PtnMap::i_PtnNoToNode(m_tPtnId);

    // -- -- -- -- -- --

    s32  m_sStreamNm;         // syntehised name of this stream

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Stream(Stage*     p_Stage,
           StageSpec* p_StageSpec,
           i8         i_Stream,
           PtnId      tPtnId,
           i8         iNodeId);
   ~Stream(void) noexcept(false);
    
    Deck* pDeck(void);
  
    i8   iNodeId(void);
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
