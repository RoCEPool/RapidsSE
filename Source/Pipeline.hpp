/*============================================================================*/
/* Pipeline.hpp -- Operator Set Pipeline                                      */
/*============================================================================*/
#pragma once

namespace Pipelines
{
  class Pipeline : public Object
  {
  private:
    Job*          m_pJob;
    PipelineSpec* m_pPipelineSepc;
    i8            m_iOriginatingNode;

    bool          m_bPulsed;
    Op*           m_pOpToPulse;
    bool          m_bCancelling;
    
    StageVec      m_oStageVec;         // vector of stages
  
  private:
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Pipeline(Job* pJob, PipelineSpec* pPipelineSepc, i8 iOriginatingNode);
   ~Pipeline(void) noexcept(false);
  
    void     vDump(void);
    
    void      vCancelling(bool bCancelling);
    bool      bCancelling(void);
    i8        iOriginatingNode(void);
    Job*      pJob(void);
    i8        nOpsOnNode(i8 iNode);
    Op*       pOpToPulse(void);
    void      vPopulateOpVec(PipelineSpec* pPipelineSpec);
    bool      bPulsed(void);
    void      vSetupJob(void);
    void      vSpawnOperatorJobs(i8 iSessionId, i8 iNode);
    StageVec* pStageVec(void);

    Footer* aFooter(void);
  };
  
  Pipeline* _pPipeline(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
