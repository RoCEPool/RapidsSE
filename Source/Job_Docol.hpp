/*============================================================================*/
/* Job_Docol.hpp -- Docol Job                                                 */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Docol : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Docol(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_Docol(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Docol* _pJob_Docol(void* aAddr);
  
  Job* pJob_Docol_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  void vJob_Docol_delete(Job* pJob);
  void vJob_Docol_show(Job* pJob);
  
  JobClass* pJob_Docol_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
