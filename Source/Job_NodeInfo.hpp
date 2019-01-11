/*============================================================================*/
/* Job_NodeInfo.hpp -- Node Information Job                                   */
/*============================================================================*/
#pragma once

namespace JobModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_NodeInfo : public Job
  {
  public:
    s32 m_sNodeName;  // the node that sent this job
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_NodeInfo(Interface::Session* pSession);
    ~Job_NodeInfo(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_NodeInfo* _pJob_NodeInfo(void* aAddr);
  
  Job* pJob_NodeInfo_new(Job* pArg);
  void vJob_NodeInfo_delete(Interface::Session* pSession, Job* pJob);
  void vJob_NodeInfo_show(Job* pJob);
  
  JobClass* pJob_NodeInfo_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
