/*============================================================================*/
/* Job_EndInsert.hpp -- End Insert Job                                        */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_EndInsert : public Job
  {
  public:
    i8             m_iInsertRowJobId;                  // insert row job
    Tasks::Task*   m_pTask;                            // task running this job
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_EndInsert(Interface::SEitf* pSEitf);
    ~Job_EndInsert(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bAllLoaded(void);
    void vNewWorkingBuffer(void);
    void vShow(void);
    
  private:
    bool bExtractDateFormat(Stz* dateFormat);
    
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_EndInsert* _pJob_EndInsert(void* aAddr);
  
  Job* pJob_EndInsert_pre(Interface::SEitf* pSEitf);
  void vJob_EndInsert_post(Interface::SEitf* pSEitf, Job* pJob);
  Job* pJob_EndInsert_new(Job* pArg);
  void vJob_EndInsert_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_EndInsert_show(Job* pJob);
  
  JobClass* pJob_EndInsert_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
