/*============================================================================*/
/* Job_ListSchemas.hpp -- List Schemas Job                                    */
/*============================================================================*/
#pragma once

namespace Jobs
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_ListSchemas : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_ListSchemas(ReqTuple* aReqTuple, i8 iForId, Session* pSession);//=C_UNKNOWN);
    ~Job_ListSchemas(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_ListSchemas* _pJob_ListSchemas(void* aAddr);
  
  Job* pJob_ListSchemas_pre(ReqTuple* aReqTuple, i8 iForId, Session* pSession);
  void vJob_ListSchemas_delete(Session* pSession, Job* pJob);
  void vJob_ListSchemas_show(Job* pJob);
  
  JobClass* pJob_ListSchemas_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
