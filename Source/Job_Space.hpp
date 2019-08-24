/*============================================================================*/
/* Job_Space.hpp -- Space Job                                                 */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Space : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Space(Session* pSession, i8 iCommandId=C_UNKNOWN);
    ~Job_Space(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Space* _pJob_Space(void* aAddr);
  
  Job* pJob_Space_pre(Session* pSession, i8 iCommandId);
  void vJob_Space_delete(Session* pSession, Job* pJob);
  void vJob_Space_show(Job* pJob);
  
  JobClass* pJob_Space_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
