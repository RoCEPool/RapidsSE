/*============================================================================*/
/* Job_Report.hpp -- Report Job                                               */
/*============================================================================*/
#pragma once

namespace JobModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Report : public Job,
                     public JobFileName
  {
  public:
    File* m_pInFile;     // output file

    bool  m_bNamedOut;   // named output file or stdout?
    s256  m_sOutputPath; // output file name
    File* m_pOutFile;    // output file
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Report(Interface::Session* pSession);
    ~Job_Report(void) noexcept(false);
  
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
  Job_Report* _pJob_Report(void* aAddr);
  
  Job* pJob_Report_pre(Interface::Session* pSession);
  void vJob_Report_post(Interface::Session* pSession, Job* pJob);
  Job* pJob_Report_new(Job* pArg);
  void vJob_Report_delete(Interface::Session* pSession, Job* pJob);
  void vJob_Report_show(Job* pJob);
  
  JobClass* pJob_Report_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
