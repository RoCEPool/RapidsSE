/*============================================================================*/
/* Job_DropCatalog.hpp -- Drop Catalog Job                                    */
/*============================================================================*/
#pragma once

namespace JobsModel
{

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_DropCatalog : public Job,
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
  
    Job_DropCatalog(Interface::SEitf* pSEitf);
    ~Job_DropCatalog(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  Job_DropCatalog* _pJob_DropCatalog(void* aAddr);
  
  Job* pJob_DropCatalog_pre(Prompt::SePrompt* pPrompt);
  void vJob_DropCatalog_post(Prompt::SePrompt* pPrompt);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
