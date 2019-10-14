/*============================================================================*/
/* Pipeline.hpp -- Operator Set Pipeline                                      */
/*============================================================================*/
#pragma once

namespace Operators
{
  class Pipeline : public Object
  {
  private:
    Session*    m_pSession;
    Job*        m_pJob;
    bool        m_bPulsed;
    Op*         m_pOpToPulse;
    bool        m_bCancelling;
    
    PipelineSpec* m_pPipelineSpec; // the dynamic job specification
    StageVec      m_oStageVec;     // vector of stages
  
  private:
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    Pipeline(Job* pJob);
   ~Pipeline(void) noexcept(false);
  
    void      vCancelling(bool bCancelling);
    bool      bCancelling(void);
    Job*      pJob(void);
    i8        nOpsOnNode(i8 iNode);
    Op*       pOpToPulse(void);
    void      vPopulateOpVec(PipelineSpec* pPipelineSpec);
    bool      bPulsed(void);
    void      vSession(Session* pSession);
    Session*  pSession(void);
    void      vSetupJob(void);
    void      vSpawnOperators(i8 iNode);
    StageVec* pStageVec(void);

    Footer* aFooter(void);
  };
  
  Pipeline* _pPipeline(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
