/*============================================================================*/
/* Job_VerifyIndex.hpp -- Verify Index Job                                    */
/*============================================================================*/
#pragma once

namespace JobModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_VerifyIndex : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_VerifyIndex(Interface::Session* pSession);
    ~Job_VerifyIndex(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_VerifyIndex* _pJob_VerifyIndex(void* aAddr);
  
  Job* pJob_VerifyIndex_pre(Interface::Session* pSession);
  void vJob_VerifyIndex_post(Interface::Session* pSession, Job* pJob);
  void vJob_VerifyIndex_delete(Interface::Session* pSession, Job* pJob);
  void vJob_VerifyIndex_show(Job* pJob);
  
  JobClass* pJob_VerifyIndex_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
