/*============================================================================*/
/* Job_Pipeline.hpp -- Pipeline Job                                           */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class Job_Pipeline : public JobMark2
  {
  public:
    Pipeline*      m_pPipeline;
    PipelineSpec*  m_pPipelineSpec;
  
    StreamDef*     m_pStreamDef;
    Queue*         m_pBufferQueue;
    
    s4K            m_sRequest;
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Pipeline(JobClass* aJobClass, ReqTuple* aReqTuple);
   ~Job_Pipeline(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(Session* pSession, TokenVec* pTokenVec);
    void vShow(void);

    void vBegin(void);
    void vCancel(void);
    void vEnd(void);
    void vExecute(void);

  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Pipeline* _pJob_Pipeline(void* aAddr);
  
  Job* pJob_Pipeline_pre(Session* pSession, JobClass* aJobClass, ReqTuple* aReqTuple);
  void vJob_Pipeline_post(Job* pJob);
  void vJob_Pipeline_delete(Job* pJob);
  void vJob_Pipeline_show(Job* pJob);
  
  JobClass* pJob_Pipeline_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
