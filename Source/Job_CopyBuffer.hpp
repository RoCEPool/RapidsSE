/*============================================================================*/
/* Job_CopyBuffer.hpp -- Copy Buffer Job                                      */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class Job_SubInserter;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_CopyBuffer : public Job,
                         public JobExtractRow
  {
  public:
    Job_SubInserter*   m_pJob_SubInserter;
    RecordBuffer*      m_pRecordBuffer;
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_CopyBuffer(ReqTuple* aReqTuple, i8 iForId, Session* pSession, JobClass* aJobClass);//=C_UNKNOWN);
    ~Job_CopyBuffer(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vCopy(void);
    void vShow(void);
    
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_CopyBuffer* _pJob_CopyBuffer(void* aAddr);
  
  Job* pJob_CopyBuffer_pre(ReqTuple* aReqTuple, i8 iForId, Session* pSession, JobClass* aJobClass);
  Job* pJob_CopyBuffer_new(JobClass* aJobClass);
  void vJob_CopyBuffer_delete(Session* pSession, Job* pJob);
  void vJob_CopyBuffer_show(Job* pJob);
  
  JobClass* pJob_CopyBuffer_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
