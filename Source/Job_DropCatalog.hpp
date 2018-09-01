/*============================================================================*/
/* Job_DropCatalog.hpp -- Drop Catalog Job                                    */
/*============================================================================*/
#pragma once

namespace JobsModel
{

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_DropCatalog : public Job
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
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  Job_DropCatalog* _pJob_DropCatalog(void* aAddr);
  
  Job* pJob_DropCatalog_pre(Interface::SEitf* pSEitf);
  void vJob_DropCatalog_post(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_DropCatalog_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_DropCatalog_show(Job* pJob);
  
  JobClass* pJob_DropCatalog_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
