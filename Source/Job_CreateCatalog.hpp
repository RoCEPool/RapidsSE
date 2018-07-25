/*============================================================================*/
/* Job_CreateCatalog.hpp -- Create Catalog Job                                */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_CreateCatalog : public Job,
                           public JobDicPath,
                           public JobObjectName,
                           public JobResult
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_CreateCatalog(Interface::SEitf* pSEitf);
    ~Job_CreateCatalog(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_CreateCatalog* _pJob_CreateCatalog(void* aAddr);
  
  Job* pJob_CreateCatalog_pre(Interface::SEitf* pSEitf);
  void vJob_CreateCatalog_post(Interface::SEitf* pSEitf, Prompt::SePrompt* pPrompt, Job* pJob);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
