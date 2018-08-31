/*============================================================================*/
/* Job_Cf.hpp -- Change Folder Job                                            */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_Cf : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_Cf(Interface::SEitf* pSEitf);
    ~Job_Cf(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_Cf* _pJob_Cf(void* aAddr);
  
  Job* pJob_Cf_pre(Interface::SEitf* pSEitf);
  void vJob_Cf_post(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_Cf_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_Cf_show(Job* pJob);
  
  Job* pJob_Cf_next(void** aaArg);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
