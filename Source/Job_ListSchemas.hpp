/*============================================================================*/
/* Job_ListSchemas.hpp -- List Schemas Job                                    */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_ListSchemas : public Job,
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
  
    Job_ListSchemas(Interface::SEitf* pSEitf);
    ~Job_ListSchemas(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_ListSchemas* _pJob_ListSchemas(void* aAddr);
  
  Job* pJob_ListSchemas_pre(Prompt::SePrompt* pPrompt);
  void vJob_ListSchemas_post(Prompt::SePrompt* pPrompt, Job* pJob);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
