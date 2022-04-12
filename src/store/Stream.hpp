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

    LglPtn m_tLglPtn;  // this stream represents this logical ptn in grid
                       //   will be C_UNKNOWN for stage with node topology

    i8     m_iNodeId;  // this stream processes NodeId in grid
                       //   is PtnMap::i_PtnNoToNode(m_tLglPtn);

    i8     m_i_PtnOfNode; // this stream processes PtnNo (in tables)
  
    // -- -- -- -- -- --

    s32  m_sStreamNm;  // synthesised name of this stream

    // must be last
    Footer m_xFooter;

  public:
    void* operator new(size_t);
    void  operator delete(void*);
  
    Stream(Stage*     p_Stage,
           StageSpec* p_StageSpec,
           LglPtn     tLglPtn,
           i8         iNodeId,
           i8         i_PtnOfNode
           );
   ~Stream(void) noexcept(false);
    
    Deck* pDeck(void);

    Op*  pFindOp(Stz* szOpNm);

    LglPtn tLglPtn(void);
  
    i8   iNodeId(void);

    Op*  pOp0(void);

    Pipeline* pPipeline(void);
    i8   i_PtnOfNode(void);

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
