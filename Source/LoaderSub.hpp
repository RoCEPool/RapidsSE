/*============================================================================*/
/* LoaderSub.hpp -- Subordinate Parallel Table Loader                         */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  class LoaderSub : public Object
  {
  private:
    Loader*  m_pLoader;    // loader parent
    Table*   m_pTable;     // table being loaded
    LoadMap* m_pLoadMap;   // load map
    s256     m_aRecord;    // formatted row
    i8       m_iLength;    // length of record
    i8       m_iPartition; // partition of record
    i8       m_iValidRecords;   // valid records
    i8       m_iInvalidRecords; // invalid records
    
    bool       m_bMoreChunks; // continue while there are more chunks
    LoadChunk* m_pLoadChunk;  // chunk loading buffer
    
    WorkingBuffer* m_pWorkingBuffer[C_MAX_PARTITIONS]; // working row buffers
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, LoaderSub* pLoaderSub);
    void  operator delete(void* pLoaderAddress);
 
    LoaderSub(void);
    ~LoaderSub(void);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void vBegin(void);
    void vEnd(void);
    bool bFetchRecord(void);
    void vHashRecord(void);
    bool bInsertRow(void);
    i8   iInvalidRecords(void);
    Loader* pLoader(void);
    void vLoader(Loader* pLoader);
    void vNewWorkingBuffer(void);
    i8   iPartition(void);
    void vPartition(i8 iPartition);
    bool bReadChunk(void);
    WorkingBuffer* pWorkingBuffer(void);
    void vWorkingBuffer(WorkingBuffer* pWorkingBuffer);
    Table* pTable(void);
    i8   iValidRecords(void);
  
    //void vTruncateRecord(i8 iField);
  
    Footer* aFooter(void);
  };
  
  LoaderSub* _pLoaderSub(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
