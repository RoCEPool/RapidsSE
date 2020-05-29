/*============================================================================*/
/* Job_Connector.hpp -- Socket/Node Cross Connector Job                       */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Connector : public Job
  {
  private:

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Connector(JobClass* aJobClass, ReqTuple* aReqTuple);
    ~Job_Connector(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:

    void vShow(void);
    Footer* aFooter(void);

  private:
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Connector* _pJob_Connector(void* aAddr);
  
  Job* pJob_Connector_new(JobClass* aJobClass, ReqTuple* aReqTuple);
  void vJob_Connector_delete(Job* pJob);
  void vJob_Connector_show(Job* pJob);
  
  JobClass* pJob_Connector_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
