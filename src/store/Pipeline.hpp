/*============================================================================*/
/* Pipeline.hpp -- Pipeline                                                   */
/*============================================================================*/
#pragma once

namespace STORE { class Deck; class Stage; class Vertex; class VertexSpec; }

namespace STORE
{
  class Pipeline : public Object
  {
  private:

    // -- belongs to
    Job*        m_pPipelineJob;    // which job
    VertexSpec* m_pVertexSpecRoot; // root specification of this pipeline
    i8          m_iOriginNodeId;   // originating node

    // -- id, name and kind
    Gid_t<CN_PIPELINE> m_xPipelineId;  // pipeline id in grid
    s32  m_sPipelineNm;  // name of this pipeline

    // -- substructure
    Vertex* m_pVertexRoot; // root of vertex tree

  
    // -- refers to
  
  

  
    // -- -- -- -- -- --

    Atomic m_ixAccum0;
    Atomic m_ixAccum1;
    Atomic m_ixAccum2;
    Lock   m_okDebugLock;

    bool m_bPulsed;
    bool m_bEarlyExit;
    bool m_bErrored;
    
    bool m_bPulsedLimit;
    Op*  m_pOpToPulse;
    bool m_bCancelled;
        
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Pipeline(Job* pPipelineJob, VertexSpec* pVertexSpecRoot, i8 iOriginNodeId);
   ~Pipeline(void) noexcept(false);
  
    void vAddAccum(i8 i_Accum, i8 iValue);
    bool bCancel(Task* pTriggeringTask);
    void vCancelled(bool bCancelled);
    bool bCancelled(void);
    Deck* pDeck0(void);
    void vDropDebugLock(void);
    void vDump(bool bRecursive=true);
    bool bEarlyExit(void);
    void vEarlyExit(bool bEarlyExit);
    bool bErrored(void);
    void vErrored(bool bErrored);
    i8   iGetAccum(i8 i_Accum);
    void vGrabDebugLock(void);
    i8   iOriginNodeId(void);
    Op*  pOp0(void);
    i8   nOpsOnNode(i8 iNode);
    Op*  pOpToPulse(void);
    void vOpToPulse(Op* pOpToPulse);
    Job* pPipelineJob(void);
    Stz* szPipelineNm(void);
    VertexSpec* pPipeSpec(void);
    bool bPulsed(void);
    void vPulsed(bool bPulsed);
    bool bPulsedLimit(void);
    void vPulsedLimit(bool bPulsedLimit);
    void vSetInvoker(Task* pInvoker);
    void vSetupJob(void);
    void vSpawnOpTasks(void);
    Stage* pStage0(void);
    Stream* pStream0(void);
    Vertex* pVertexRoot(void);

  private:
    void vCreateVertexes();

    Footer* aFooter(void);
  };
  
  Pipeline* _pPipeline(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
