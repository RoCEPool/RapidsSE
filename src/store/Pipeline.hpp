/*============================================================================*/
/* Pipeline.hpp -- Pipeline                                                   */
/*============================================================================*/
#pragma once

#include "LifeGuard_t.hpp"

namespace STORE { class Formwork; class Stage; class PipelineSpec; }

namespace STORE
{
  class Pipeline : public Object
                   //public VectorNL_t<CN_PIPELINE,Stage,C_MAX_STAGES>,
                 , public LifeGuard_t<Pipeline>

  {
  private:

    // -- id, name and kind
    Gid_t<CN_PIPELINE> m_xPipelineId;  // pipeline id in grid
    s32  m_sPipelineNm;  // name of this pipeline

    // -- substructure
    Formwork* m_pFormWork; // formwork of this pipeline

    // -- belongs to
    Job* m_pPipelineJob;  // which job
  
    // -- refers to
    PipelineSpec* m_pPipelineSpec; // specification of this pipeline
  
    i8   m_i_Origin;    // originating node
  

  
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
    
    Pipeline(Job* pPipelineJob, PipelineSpec* pPipelineSpec, i8 i_Origin);
   ~Pipeline(void) noexcept(false);
  
    void vAddAccum(i8 i_Accum, i8 iValue);
    bool bCancel(Task* pTriggeringTask);
    void vCancelled(bool bCancelled);
    bool bCancelled(void);
    void vDropDebugLock(void);
    bool bEarlyExit(void);
    void vEarlyExit(bool bEarlyExit);
    bool bErrored(void);
    void vErrored(bool bErrored);
    Formwork* pFormwork(void);
    i8   iGetAccum(i8 i_Accum);
    void vGrabDebugLock(void);
    i8   i_Origin(void);
    Op*  pOp0(void);
    i8   nOpsOnNode(i8 iNode);
    Op*  pOpToPulse(void);
    void vOpToPulse(Op* pOpToPulse);
    Job* pPipelineJob(void);
    Stz* szPipelineNm(void);
    PipelineSpec* pPipelineSpec(void);
    bool bPulsed(void);
    void vPulsed(bool bPulsed);
    bool bPulsedLimit(void);
    void vPulsedLimit(bool bPulsedLimit);
    void vSetupJob(void);
    void vSpawnOpTasks(i8 iNode);
    Stage* pStage0(void);
    Stream* pStream0(void);

  private:
    void vCreateFormwork();

    Footer* aFooter(void);
  };
  
  Pipeline* _pPipeline(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
