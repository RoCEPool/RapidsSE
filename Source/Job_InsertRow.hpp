/*============================================================================*/
/* Job_InsertRow.hpp -- Insert Row Job                                        */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_InsertRow : public Job
  {
  public:
    Table*         m_pTable;                           // table pointer
    Formatters     m_xFormatters;                      // record formatters
    WorkingBuffer* m_pWorkingBuffer[C_SLOTS_PER_NODE]; // working buffers
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_InsertRow(Interface::SEitf* pSEitf);
    ~Job_InsertRow(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bInsertRow(Stz* pRow, i8 iLength, i8 iRowLength, eEC& eErrorCode);
    eEC eInsertLastBuffer(i8 iPartition);
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
  Job_InsertRow* _pJob_InsertRow(void* aAddr);
  
  Job* pJob_InsertRow_pre(Interface::SEitf* pSEitf);
  void vJob_InsertRow_post(Interface::SEitf* pSEitf, Job* pJob);
  Job* pJob_InsertRow_new(Job* pArg);
  void vJob_InsertRow_delete(Interface::SEitf* pSEitf, Job* pJob);
  void vJob_InsertRow_show(Job* pJob);
  
  JobClass* pJob_InsertRow_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
