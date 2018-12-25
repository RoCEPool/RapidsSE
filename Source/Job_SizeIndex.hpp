/*============================================================================*/
/* Job_SizeIndex.hpp -- Size Index Job                                        */
/*============================================================================*/
#pragma once

namespace JobModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_SizeIndex : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_SizeIndex(Interface::Session* pSession);
    ~Job_SizeIndex(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_SizeIndex* _pJob_SizeIndex(void* aAddr);
  
  Job* pJob_SizeIndex_pre(Interface::Session* pSession);
  void vJob_SizeIndex_post(Interface::Session* pSession, Job* pJob);
  void vJob_SizeIndex_delete(Interface::Session* pSession, Job* pJob);
  void vJob_SizeIndex_show(Job* pJob);
  
  JobClass* pJob_SizeIndex_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
