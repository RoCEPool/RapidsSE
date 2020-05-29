/*============================================================================*/
/* Job_SubIngester.hpp -- Table SubIngester Job                               */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_SubIngester : public Job,
                          public JobWriteBehind
  {
  public:
    Job_Ingest* m_pJob_Ingest;
 
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_SubIngester(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_SubIngester(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    Job_Ingest* pJob_Ingest(void);
    void vJob_Ingest(Job_Ingest* pJob_Ingest);

    bool bBegin(void);
    bool bChunk(void);
    void vEnd(void);
  
    void vShow(void);
    Footer* aFooter(void);

  private:
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_SubIngester* _pJob_SubIngester(void* aAddr);
  
  Job* vJob_SubIngester_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  Job* pJob_SubIngester_new(JobClass* aJobClass, ReqTuple* aReqTuple);
  void vJob_SubIngester_delete(Job* pJob);
  void vJob_SubIngester_show(Job* pJob);
  
  JobClass* pJob_SubIngester_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
