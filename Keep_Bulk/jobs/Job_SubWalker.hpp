/*============================================================================*/
/* pJob_SubWalker.hpp -- Sub Walker Job                                       */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class Job_Walk;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_SubWalker : public Job
  {
  public:
   
    Job_Walk* m_pJobWalk;
  
    IndexTree*   m_pIndexTree;
    EntBuffer* m_pEntBuffer;
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_SubWalker(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_SubWalker(void) noexcept(false);
  
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
  Job_SubWalker* _pJob_SubWalker(void* aAddr);
  
  Job* pJob_SubWalker_pre(JobClass* aJobClass, ReqTuple* aReqTuple, Session* pSession);
  void vJob_SubWalker_delete(Job* pJob);
  void vJob_SubWalker_show(Job* pJob);
  
  JobClass* pJob_SubWalker_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
