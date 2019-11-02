/*============================================================================*/
/* Job_Boot.hpp -- Library Boot Job                                            */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Boot : public Job
  {
  private:

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Boot(JobClass* aJobClass, ReqTuple* aReqTuple);//=C_UNKNOWN);
   ~Job_Boot(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:

    void vShow(void);
    Footer* aFooter(void);

  private:
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Boot* _pJob_Boot(void* aAddr);
  
  Job* pJob_Boot_new(JobClass* aJobClass);
  void vJob_Boot_delete(Job* pJob);
  void vJob_Boot_show(Job* pJob);
  
  JobClass* pJob_Boot_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
