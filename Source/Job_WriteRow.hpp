/*============================================================================*/
/* Job_WriteRow.hpp -- Write Row Job                                          */
/*============================================================================*/
#pragma once

namespace JobModel
{
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_WriteRow : public Job
  {
  public:
    Table* m_pTable;          // table pointer
    i8     m_iPartitionId;    // partition id
    i8     m_iParentJobId;    // parent (InsertRow) job id
    s48K   m_aRecord;         // formatted row
    i8     m_nRecordBytes;    // length of record
    Formatters m_xFormatters; // formatters
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_WriteRow(Interface::Session* pSession);
    ~Job_WriteRow(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    static Job_WriteRow* pNewJobWriteRow(Interface::Session* pSession,
                                         Stz* sTableName,
                                         i8 iParentJobId);
    
  public:
    bool bFormatRow(Table* pTable, Stz* aRawData, i8 nRawDataBytes);
    void vInsertRow(void);
    void vShow(void);
    
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_WriteRow* _pJob_WriteRow(void* aAddr);
  
  Job* pJob_WriteRow_pre(Interface::Session* pSession);
  Job* pJob_WriteRow_new(Job* pArg);
  void vJob_WriteRow_delete(Interface::Session* pSession, Job* pJob);
  void vJob_WriteRow_show(Job* pJob);
  
  JobClass* pJob_WriteRow_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
