/*============================================================================*/
/* Job_Status.hpp -- Status Job                                               */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Status : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Status(Interface::SEitf* pSEitf);
    ~Job_Status(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Status* _pJob_Status(void* aAddr);
  
  Job* pJob_Status_pre(Interface::SEitf* pSEitf);
  void vJob_Status_post(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_Status_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_Status_show(Job* pJob);
  
  pNEXTJOBFUN* pJob_Status_next(JobClass_s** aaJobClass);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
