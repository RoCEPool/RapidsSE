/*============================================================================*/
/* Job_Cat.hpp -- Cat Job                                                     */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Cat : public Job,
                  public JobFilePath
  {
  public:
    s256 m_sOutputPath; // output file name

    IoCopy*   m_pIoCopy;
    ByteRate* m_pByteRate;
    bool      m_bStatistics;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Cat(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_Cat(void) noexcept(false);
  
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
  Job_Cat* _pJob_Cat(void* aAddr);
  
  Job* pJob_Cat_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  Job* pJob_Cat_new(JobClass* aJobClass, ReqTuple* aReqTuple);
  void vJob_Cat_delete(Job* pJob);
  void vJob_Cat_show(Job* pJob);
  
  JobClass* pJob_Cat_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
