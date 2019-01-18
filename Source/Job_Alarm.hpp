/*============================================================================*/
/* Job_Alarm.hpp -- Alarm Job                                                 */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Alarm : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Alarm(Interface::Session* pSession);
    ~Job_Alarm(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Alarm* _pJob_Alarm(void* aAddr);
  
  Job* pJob_Alarm_new(Job* pArg);
  void vJob_Alarm_delete(Interface::Session* pSession, Job* pJob);
  void vJob_Alarm_show(Job* pJob);
  
  JobClass* pJob_Alarm_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
