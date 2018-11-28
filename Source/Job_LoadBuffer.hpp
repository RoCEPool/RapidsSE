/*============================================================================*/
/* Job_LoadBuffer.hpp -- Load Buffer Job                                      */
/*============================================================================*/
#pragma once

namespace JobsModel
{
  class Job_LoadTable;
  class Job_SubLoader;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  class Job_LoadBuffer : public Job
  {
  public:
    Table* m_pTable; // which table
    i8 m_iPartition; // which partition
    i8 m_iParentJobId; // the job id of the load table job
    i8 m_nWriteBehindBuffers; // number of buffers to be loadeed from sub loader
  
    bool   m_bEndOfBuffers; // no more buffers  WON'T WORK RAY

    i8     m_nRows; // how many rows in working buffer  WON"T WORK RAY

    bool   m_bLowMem; // was received under low memory conditions

    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void* operator new(size_t, void* aAddress);
    void  operator delete(void*);
  
    Job_LoadBuffer(Interface::Session* pSession);
    ~Job_LoadBuffer(void) noexcept(false);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static Job_LoadBuffer* pNewJobLoadBuffer(Job_SubLoader* pJob_SubLoader, bool bFinal);

  public:
    i8   nWriteBehindBuffers(void);
    void vWriteBehindBuffers(i8 nBuffers);
  
    bool bInsertRows(void);
    bool bInsertRows_LowMem(void);

    i8   iPartition(void);
    void vPartition(i8 iPartition);

    Table* pTable(void);
    void vTable(Table* pTable);

  
    void vShow(void);
  
    Footer* aFooter(void);

  private:
  };
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  Job_LoadBuffer* _pJob_LoadBuffer(void* aAddr);
  
  Job* vJob_LoadBuffer_pre(Interface::Session* pSession);
  Job* pJob_LoadBuffer_new(Job* pArg);
  void vJob_LoadBuffer_delete(Interface::Session* pSession, Job* pJob);
  void vJob_LoadBuffer_show(Job* pJob);
  
  JobClass* pJob_LoadBuffer_class(pFUN** aaFun);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
