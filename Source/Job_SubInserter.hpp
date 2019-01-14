/*============================================================================*/
/* Job_SubInserter.hpp -- Sub Insert Row Job                                  */
/*============================================================================*/
#pragma once

namespace JobModel
{
  class Job_InsertTable;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_SubInserter : public Job,
                          public JobWriteBehind
  {
  public:
    Job_InsertTable* m_pJob_InsertTable;                   // insert table job
    RecordBuffer*    m_pRecordBuffer;
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_SubInserter(Interface::Session* pSession);
    ~Job_SubInserter(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    void vBegin(Table* pTable);
    void vEnd(void);
    bool bPopulateJob(TokenVec* pTokenVec);

    void vShow(void);
    
  private:
    bool bExtractDateFormat(Stz* dateFormat);
    
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_SubInserter* _pJob_SubInserter(void* aAddr);
  
  Job* pJob_SubInserter_pre(Interface::Session* pSession);
  void vJob_SubInserter_post(Interface::Session* pSession, Job* pJob);
  Job* pJob_SubInserter_new(Job* pArg);
  void vJob_SubInserter_delete(Interface::Session* pSession, Job* pJob);
  void vJob_SubInserter_show(Job* pJob);
  
  JobClass* pJob_SubInserter_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
