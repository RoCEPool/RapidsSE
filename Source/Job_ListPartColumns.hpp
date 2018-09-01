/*============================================================================*/
/* Job_ListPartColumns.hpp -- List Partition Columns Job                      */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_ListPartColumns : public Job
  {
  public:
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_ListPartColumns(Interface::SEitf* pSEitf);
    ~Job_ListPartColumns(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_ListPartColumns* _pJob_ListPartColumns(void* aAddr);
  
  Job* pJob_ListPartColumns_pre(Interface::SEitf* pSEitf);
  void vJob_ListPartColumns_post(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_ListPartColumns_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_ListPartColumns_show(Job* pJob);
  
  JobClass* pJob_ListPartColumns_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
