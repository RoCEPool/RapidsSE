/*============================================================================*/
/* Job_Repeater.hpp -- Repeater Job                                           */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Repeater : public Job,
                       public JobFilePath
  {
  public:
    s256  m_sOutputPath;  // pipe path
  
    IoCopy*   m_pIoCopy;
    ByteRate* m_pByteRate;
    bool      m_bStatistics;
    i8        m_nRepeats;

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Repeater(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_Repeater(void) noexcept(false);
  
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
  Job_Repeater* _pJob_Repeater(void* aAddr);
  
  Job* pJob_Repeater_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  Job* pJob_Repeater_new(JobClass* aJobClass, ReqTuple* aReqTuple);
  void vJob_Repeater_delete(Job* pJob);
  void vJob_Repeater_show(Job* pJob);
  
  JobClass* pJob_Repeater_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
