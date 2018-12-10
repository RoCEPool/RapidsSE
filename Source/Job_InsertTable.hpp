/*============================================================================*/
/* Job_InsertTable.hpp -- Insert Table Job                                    */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  class Job_InsertTable;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_InsertTable : public Job
  {
  public:
    Job_InsertRows* m_pJobInsertRows;                  // insert row job
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_InsertTable(Interface::Session* pSession);
    ~Job_InsertTable(void) noexcept(false);
  
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
  Job_InsertTable* _pJob_InsertTable(void* aAddr);
  
  Job* pJob_InsertTable_pre(Interface::Session* pSession);
  void vJob_InsertTable_post(Interface::Session* pSession, Job* pJob);
  Job* pJob_InsertTable_new(Job* pArg);
  void vJob_InsertTable_delete(Interface::Session* pSession, Job* pJob);
  void vJob_InsertTable_show(Job* pJob);
  
  JobClass* pJob_InsertTable_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
