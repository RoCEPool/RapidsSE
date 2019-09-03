/*============================================================================*/
/* Job_DumpIndex.hpp -- Dump Index Job                                        */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_DumpIndex : public Job
  {
  public:
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_DumpIndex(ReqTuple* aReqTuple, Session* pSession);//=C_UNKNOWN);
    ~Job_DumpIndex(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_DumpIndex* _pJob_DumpIndex(void* aAddr);
  
  Job* pJob_DumpIndex_pre(ReqTuple* aReqTuple, Session* pSession);
  void vJob_DumpIndex_delete(Session* pSession, Job* pJob);
  void vJob_DumpIndex_show(Job* pJob);
  
  JobClass* pJob_DumpIndex_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
