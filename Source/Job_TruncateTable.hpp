/*============================================================================*/
/* pJob_TruncateTable.hpp -- Truncate Table Job                               */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_TruncateTable : public Job
  {
  public:
    
    Tasks::Task* m_pTask; // task running this job
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_TruncateTable(Interface::SEitf* pSEitf);
    ~Job_TruncateTable(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    void vShow(void);
  
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_TruncateTable* _pJob_TruncateTable(void* aAddr);
  
  Job* pJob_TruncateTable_pre(Interface::SEitf* pSEitf);
  void vJob_TruncateTable_post(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_TruncateTable_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_TruncateTable_show(Job* pJob);
  
  pNEXTJOBFUN* pJob_TruncateTable_next(JobClass_s** aaJobClass);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
