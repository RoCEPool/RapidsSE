/*============================================================================*/
/* Job_Query.hpp -- Query Job                                                 */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class Job_Query : public JobMark2
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Query(Interface::Session* pSession);
    ~Job_Query(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
    void vBegin(void);
    void vEnd(void);
    void vExecute(void);

  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Query* _pJob_Query(void* aAddr);
  
  Job* pJob_Query_pre(Interface::Session* pSession);
  void vJob_Query_delete(Interface::Session* pSession, Job* pJob);
  void vJob_Query_show(Job* pJob);
  
  JobClass* pJob_Query_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
