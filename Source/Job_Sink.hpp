/*============================================================================*/
/* Job_Sink.hpp -- Sink Job                                                   */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Sink : public Job,
                   public JobFilePath
  {
  public:
    IoChunk*  m_pIoChunk;
    ByteRate* m_pByteRate;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Sink(ReqTuple* aReqTuple, i8 iForId, Session* pSession, JobClass* aJobClass);//=C_UNKNOWN);
    ~Job_Sink(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
    bool bBegin(void);
    bool bChunk(void);
    void vEnd(void);

  public:
    Footer* aFooter(void);
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Sink* _pJob_Sink(void* aAddr);
  
  Job* pJob_Sink_pre(ReqTuple* aReqTuple, i8 iForId, Session* pSession, JobClass* aJobClass);
  Job* pJob_Sink_new(JobClass* aJobClass);
  void vJob_Sink_delete(Session* pSession, Job* pJob);
  void vJob_Sink_show(Job* pJob);
  
  JobClass* pJob_Sink_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
