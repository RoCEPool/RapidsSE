/*============================================================================*/
/* Job_LoadTable.hpp -- Load Table Job                                        */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_LoadTable : public Job,
                        public JobFileName
  {
  public:
    Utilities::Loader* m_pLoader; // table loader
    Formatters m_xFormatters; // record formatters
  
    Tasks::Task* m_pTask; // task running this job
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_LoadTable(Interface::SEitf* pSEitf);
    ~Job_LoadTable(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bPopulateJob(TokenVec* pTokenVec);
    Tasks::Task* pTask(void);
    void vTask(Tasks::Task* pTask);

    void vShow(void);
    
  private:
    bool bExtractDateFormat(Stz* dateFormat);
    
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_LoadTable* _pJob_LoadTable(void* aAddr);
  
  Job* pJob_LoadTable_pre(Interface::SEitf* pSEitf);
  void vJob_LoadTable_post(Interface::SEitf* pSEitf, Job* pJob);
  Job* pJob_LoadTable_new(Job* pArg);
  void vJob_LoadTable_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_LoadTable_show(Job* pJob);
  
  pNEXTJOBFUN* pJob_LoadTable_next(JobClass_s** aaJobClass);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
