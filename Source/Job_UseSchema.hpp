/*============================================================================*/
/* Job_UseSchema.hpp -- Use Schema Job                                        */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_UseSchema : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_UseSchema(Interface::SEitf* pSEitf);
    ~Job_UseSchema(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_UseSchema* _pJob_UseSchema(void* aAddr);
  
  Job* pJob_UseSchema_pre(Interface::SEitf* pSEitf);
  void vJob_UseSchema_post(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_UseSchema_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_UseSchema_show(Job* pJob);
  
  JobClass_s* pJob_UseSchema_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
