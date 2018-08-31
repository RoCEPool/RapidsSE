/*============================================================================*/
/* Job_ListCatalogs.hpp -- List Catalogs Job                                  */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_ListCatalogs : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_ListCatalogs(Interface::SEitf* pSEitf);
    ~Job_ListCatalogs(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_ListCatalogs* _pJob_ListCatalogs(void* aAddr);
  
  Job* pJob_ListCatalogs_pre(Interface::SEitf* pSEitf);
  void vJob_ListCatalogs_post(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_ListCatalogs_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_ListCatalogs_show(Job* pJob);
  
  pNEXTJOBFUN* pJob_ListCatalogs_next(JobClass_s** aaJobClass);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
