/*============================================================================*/
/* Job_StartNode.hpp -- Start Node Job                                        */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class Job_StartNode : public Job
  {
  public:
      
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_StartNode(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_StartNode(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    eEC ePopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  private:
    bool bValidateStartNode(TokenVec* pTokenVec);
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_StartNode* _pJob_StartNode(void* aAddr);
  
  Job* pJob_StartNode_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  void vJob_StartNode_post(Job* pJob);
  Job* pJob_StartNode_new(JobClass* aJobClass, ReqTuple* aReqTuple);
  void vJob_StartNode_delete(Job* pJob);
  void vJob_StartNode_show(Job* pJob);
  
  JobClass* pJob_StartNode_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
