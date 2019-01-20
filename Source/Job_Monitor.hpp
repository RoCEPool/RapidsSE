/*============================================================================*/
/* Job_Monitor.hpp -- Monitor Job                                             */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Monitor : public Job,
                      public JobFilePath
  {
  public:
    i8     m_iInterval;  // time between monitor takes
    bool   m_bNamedFile; // named output file or stdout?
    File*  m_pOutFile;   // output file
  
    i8     m_iTakeNo;    // take number
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Monitor(Interface::Session* pSession);
    ~Job_Monitor(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
    bool bBegin(void);
    bool bTake(void);
    void vEnd(void);

  public:
    Footer* aFooter(void);
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Monitor* _pJob_Monitor(void* aAddr);
  
  Job* pJob_Monitor_pre(Interface::Session* pSession);
  void vJob_Monitor_post(Interface::Session* pSession, Job* pJob);
  Job* pJob_Monitor_new(Job* pArg);
  void vJob_Monitor_delete(Interface::Session* pSession, Job* pJob);
  void vJob_Monitor_show(Job* pJob);
  
  JobClass* pJob_Monitor_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
