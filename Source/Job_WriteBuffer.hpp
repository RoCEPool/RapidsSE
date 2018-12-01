/*============================================================================*/
/* Job_WriteBuffer.hpp -- Write Buffer Job                                    */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_WriteBuffer : public Job
  {
  public:
    eDS    m_eDataSource;    // data source
    Table* m_pTable;         // table pointer
    i8     m_iPartitionId;   // partition id
    i8     m_iParentJobId;   // parent (EndInsert/LoadTable) job id
    i8     m_nWriteBehindBuffers; // write behind buffers
    
    i8     m_nRows; // how many rows in working buffer  WON"T WORK RAY
    
    bool   m_bLowMem; // was received under low memory conditions
    
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_WriteBuffer(Interface::Session* pSession);
    ~Job_WriteBuffer(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static Job_WriteBuffer* pNewJobWriteBuffer(Interface::Session* pSession,
                                              Stz* sTableName, eDS eDataSource,
                                              WorkingBuffer* pWorkingBuffer,
                                              i8 iParentJobId,
                                              i8 iPartition,
                                              bool bFinal,
                                              i8 nWriteBehindBuffers);
    
  public:
    bool bInsertRows(void);
    bool bInsertRows_LowMem(void);
    void vShow(void);
    
  public:
    Footer* aFooter(void);
  
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_WriteBuffer* _pJob_WriteBuffer(void* aAddr);
  
  Job* pJob_WriteBuffer_pre(Interface::Session* pSession);
  Job* pJob_WriteBuffer_new(Job* pArg);
  void vJob_WriteBuffer_delete(Interface::Session* pSession, Job* pJob);
  void vJob_WriteBuffer_show(Job* pJob);
  
  JobClass* pJob_WriteBuffer_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
