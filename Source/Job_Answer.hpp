/*============================================================================*/
/* Job_Answer.hpp -- Socket Answer Job                                        */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Answer : public Job
  {
  private:

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Answer(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_Answer(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:

    void vShow(void);
    Footer* aFooter(void);

  private:
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Answer* _pJob_Answer(void* aAddr);
  
  Job* pJob_Answer_new(JobClass* aJobClass, ReqTuple* aReqTuple);
  void vJob_Answer_delete(Job* pJob);
  void vJob_Answer_show(Job* pJob);
  
  JobClass* pJob_Answer_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
