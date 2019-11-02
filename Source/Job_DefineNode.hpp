/*============================================================================*/
/* Job_DefineNode.hpp -- Define Node Job                                      */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class Job_DefineNode : public Job
  {
  public:
      
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_DefineNode(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_DefineNode(void) noexcept(false);
    
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
  Job_DefineNode* _pJob_CreateNode(void* aAddr);
  
  Job* pJob_DefineNode_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  void vJob_DefineNode_post(Job* pJob);
  Job* pJob_DefineNode_new(JobClass* aJobClass, ReqTuple* aReqTuple);
  void vJob_DefineNode_delete(Job* pJob);
  void vJob_DefineNode_show(Job* pJob);
  
  JobClass* pJob_DefineNode_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
