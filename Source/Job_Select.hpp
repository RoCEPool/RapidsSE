/*============================================================================*/
/* pJob_Select.hpp -- Select Statement Job                                    */
/*============================================================================*/
#pragma once

namespace Jobs
{
  class Job_Select : public Job
  {
  public:
        
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Select(Session* pSession);
    ~Job_Select(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);

    void vShow(void);
    
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Job_Select* _pJob_Select(void* aAddr);
  
  Job* pJob_Select_pre(Session* pSession);
  void vJob_Select_delete(Session* pSession, Job* pJob);
  void vJob_Select_show(Job* pJob);
  
  JobClass* pJob_Select_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
