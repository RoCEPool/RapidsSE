/*============================================================================*/
/* LoaderSub.hpp -- Subordinate Parallel Table Loader                         */
/*============================================================================*/
#pragma once

namespace Utilities
{
  class LoaderSub : public Object
  {
  private:
    Job_SubLoader* m_pJob_SubLoader; // job
    Loader*  m_pLoader;    // loader parent
    LoadChunk* m_pLoadChunk;  // chunk loading buffer
    LoadMap* m_pLoadMap;   // load map

    Table*   m_pTable;     // table being loaded
    s48K     m_aRecord;    // formatted row
    i8       m_iLength;    // length of record
    i8       m_iPartition; // partition of record
    i8       m_iValidRecords;   // valid records
    i8       m_iInvalidRecords; // invalid records
    
    bool       m_bMoreChunks; // continue while there are more chunks
  
    WorkingBuffer* m_pWorkingBuffer[C_MAX_PARTITIONS]; // working row buffers
    i8 m_nBuffersLoadable[C_MAX_PARTITIONS]; // how many buffers are loadable

    Formatters  m_xFormatters;
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, LoaderSub* pLoaderSub);
    void  operator delete(void* pLoaderAddress);
 
    LoaderSub(Job_SubLoader* pJob_SubLoader, Loader* pLoader);
    ~LoaderSub(void);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void vBegin(void);
    void vBegin_Original(void);
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
    Loader* pLoader(void);
    void vLoader(Loader* pLoader);
    void vNewWorkingBuffer(void);
    i8   iPartition(void);
    void vPartition(i8 iPartition);
    bool bReadChunk(void);
    void vSetDistFormatter(char cFieldSeparator);
    WorkingBuffer* pWorkingBuffer(void);
    void vWorkingBuffer(WorkingBuffer* pWorkingBuffer);
    Table* pTable(void);
    i8   iValidRecords(void);
  
    //void vTruncateRecord(i8 iField);
  
    void vDiscardBuffer(void);
  
    Footer* aFooter(void);
  };
  
  LoaderSub* _pLoaderSub(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
