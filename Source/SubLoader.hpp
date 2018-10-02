/*============================================================================*/
/* SubLoader.hpp -- Subordinate Parallel Table LoadTable                      */
/*============================================================================*/
#pragma once

namespace Utilities
{
  class SubLoader : public Object
  {
  private:
    Job_SubLoader* m_pJob_SubLoader; // job
  
    LoadTable* m_pLoadTable; // loader parent
    LoadChunk* m_pLoadChunk; // chunk loading buffer
    LoadMap*   m_pLoadMap;   // load map

    Table*   m_pTable;     // table being loaded
    s48K     m_aRecord;    // formatted row
    i8       m_iLength;    // length of record
    i8       m_iPartition; // partition of record
  
    i8       m_iValidRecords;   // valid records
    i8       m_iInvalidRecords; // invalid records
    
    bool     m_bMoreChunks; // continue while there are more chunks
  
    WorkingBuffer* m_pWorkingBuffer[C_MAX_PARTITIONS]; // working row buffers
    i8 m_nBuffersLoadable[C_MAX_PARTITIONS]; // how many buffers are loadable

    Formatters  m_xFormatters;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, SubLoader* pSubLoader);
    void  operator delete(void* pLoaderAddress);
 
    SubLoader(Job_SubLoader* pJob_SubLoader, LoadTable* pLoadTable);
    ~SubLoader(void);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void vBegin(void);
    i8   nBuffersLoadable(void);
    void vEnd(void);
    void vEnd_Original(void);
    bool bFetchRecord(void);
    bool bFetchRecordDist(void);
    void vHashRecord(void);
    bool bInsertRow(void);
    bool bInsertRowDist(void);
    i8   iInvalidRecords(void);
    Job_SubLoader* pJob_SubLoader(void);
    void vNewWorkingBuffer(void);
    i8   iPartition(void);
    void vPartition(i8 iPartition);
    bool bProcessRecords(void);
    bool bReadChunk(void);
    Table* pTable(void);
    i8   iValidRecords(void);
    WorkingBuffer* pWorkingBuffer(void);
    void vWorkingBuffer(WorkingBuffer* pWorkingBuffer);
  
    Footer* aFooter(void);
  };
  
  SubLoader* _pLoaderSub(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
