/*============================================================================*/
/* Pipeline.hpp -- Pipeline                                                   */
/*============================================================================*/
#pragma once

namespace Pipelines
{
  class Pipeline : public VectorNL_t<CN_PIPELINE,Stage,C_MAX_STAGES>
  {
  private:
    s32           m_sPipelineNm;
  
    PipelineSpec* m_pPipelineSpec;
  
    Job*          m_pJob;
  
    i8            m_iOriginatingNode;

    bool          m_bPulsed;
    bool          m_bErrored;
    
    bool          m_bPulsedLimit;
    Op*           m_pOpToPulse;
    bool          m_bCancelled;
      
  private:
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Pipeline(Job* pJob, PipelineSpec* pPipelineSpec, i8 iOriginatingNode);
   ~Pipeline(void) noexcept(false);
  
    void     vDump(void);
    bool     bCancel(Task* pTriggeringTask);
    void     vCancelled(bool bCancelled);
    bool     bCancelled(void);
    bool     bErrored(void);
    void     vErrored(bool bErrored);
    i8       iOriginatingNode(void);
    Job*     pJob(void);
    i8       nOpsOnNode(i8 iNode);
    Op*      pOpToPulse(void);
    Stz*     szPipelineNm(void);
    PipelineSpec* pPipelineSpec(void);
    void     vPopulateOpVec(PipelineSpec* pPipelineSpec);
    bool     bPulsed(void);
    void     vPulsed(bool bPulsed);
    bool     bPulsedLimit(void);
    void     vSetupJob(void);
    void     vSpawnOperatorJobs(i8 iSessionId, i8 iNode);

    Footer* aFooter(void);
  };
  
  Pipeline* _pPipeline(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
