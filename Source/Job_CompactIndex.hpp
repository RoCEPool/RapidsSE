/*============================================================================*/
/* Job_CompactIndex.hpp -- Compact Index Job                                  */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_CompactIndex : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_CompactIndex(Interface::Session* pSession);
    ~Job_CompactIndex(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_CompactIndex* _pJob_CompactIndex(void* aAddr);
  
  Job* pJob_CompactIndex_pre(Interface::Session* pSession);
  void vJob_CompactIndex_delete(Interface::Session* pSession, Job* pJob);
  void vJob_CompactIndex_show(Job* pJob);
  
  JobClass* pJob_CompactIndex_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
