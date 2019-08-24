/*============================================================================*/
/* Job_Dynamic.hpp -- Dynamic Job                                             */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class Job_Dynamic : public JobMark2
  {
  public:
    Digest* m_pDigest;
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Dynamic(Session* pSession, i8 iCommandId=C_UNKNOWN);
   ~Job_Dynamic(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(Session* pSession, TokenVec* pTokenVec);
    void vShow(void);
  
    void vBegin(void);
    void vEnd(void);
    void vExecute(void);

  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Dynamic* _pJob_Dynamic(void* aAddr);
  
  Job* pJob_Dynamic_pre(Session* pSession, i8 iCommandId);
  void vJob_Dynamic_delete(Session* pSession, Job* pJob);
  void vJob_Dynamic_show(Job* pJob);
  
  JobClass* pJob_Dynamic_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
