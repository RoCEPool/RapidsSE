/*============================================================================*/
/* Job_CreateNode.hpp -- Create Node Job                                      */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class Job_CreateNode : public Job
  {
  public:
      
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_CreateNode(JobClass* aJobClass, ReqTuple* aReqTuple, i8 iForId, Session* pSession);//=C_UNKNOWN);
    ~Job_CreateNode(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    eEC ePopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  private:
    bool bValidateCreateNode(TokenVec* pTokenVec);
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_CreateNode* _pJob_CreateNode(void* aAddr);
  
  Job* pJob_CreateNode_pre(JobClass* aJobClass, ReqTuple* aReqTuple, i8 iForId, Session* pSession);
  void vJob_CreateNode_post(Job* pJob);
  Job* pJob_CreateNode_new(JobClass* aJobClass);
  void vJob_CreateNode_delete(Job* pJob);
  void vJob_CreateNode_show(Job* pJob);
  
  JobClass* pJob_CreateNode_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
