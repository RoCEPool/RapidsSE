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
    Table*         m_pTable;                             // table pointer
    Formatters     m_xFormatters;                        // record formatters
    i8             m_iPartition;                         // current partition
    WorkingBuffer* m_pWorkingBuffer[C_MAX_PARTITIONS];   // working buffers
    i8             m_nBuffersToBeInserted[C_MAX_PARTITIONS]; // buffers insertable
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_InsertRow(Interface::Session* pSession);
    ~Job_InsertRow(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

  public:
    bool bInsertRow(Stz* pRow, i8 iLength, i8 iRowLength, eEC& eErrorCode);
    void vNewWorkingBuffer(void);
    bool bPopulateJob(TokenVec* pTokenVec);

    void vShow(void);
    
  private:
    bool bExtractDateFormat(Stz* dateFormat);
    
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_InsertRow* _pJob_InsertRow(void* aAddr);
  
  Job* pJob_InsertRow_pre(Interface::Session* pSession);
  void vJob_InsertRow_post(Interface::Session* pSession, Job* pJob);
  Job* pJob_InsertRow_new(Job* pArg);
  void vJob_InsertRow_delete(Interface::Session* pSession, Job* pJob);
  void vJob_InsertRow_show(Job* pJob);
  
  JobClass* pJob_InsertRow_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
