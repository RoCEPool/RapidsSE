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
    Table*   m_pTable;     // table being load
    LoadMap* m_pLoadMap;   // load map
    s256     m_aRecord;    // formatted row
    i8       m_iLength;    // length of record
    i8       m_iPartition; // partition of record
    i8       m_iFetchedRecords; // fetched records
    
    s32      m_sEnclosedBy;       // enclosed by
    s32      m_sEscapeChar;       // escape char
    s32      m_sFieldSeparator;   // field separator
    s32      m_sRecordTerminator; // record terminator
    
    bool     m_bMoreChunks;  // continue while there are more chunks
    LoadChunk* m_pLoadChunk; // chunk loading buffer
    
    EntryBuffer* m_pEntryBuffer[C_MAX_PARTITIONS]; // working row buffers
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, LoaderSub* pLoaderSub);
    void  operator delete(void* pLoaderAddress);
 
    LoaderSub(void);
    ~LoaderSub(void);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void vAppendBuffer(void);
    void vBegin(void);
    void vEnd(void);
    bool bFetchRecord(void);
    i8   iFetchedRecords(void);
    void vHashRecord(void);
    bool bInsertRow(void);
    Loader* pLoader(void);
    void vLoader(Loader* pLoader);
    void vNewRowBuffer(void);
    i8   iPartition(void);
    void vPartition(i8 iPartition);
    bool bReadChunk(void);
    EntryBuffer* pRowBuffer(void);
    void vRowBuffer(EntryBuffer* pRowBuffer);
    Table* pTable(void);
  
    //void vTruncateRecord(i8 iField);
  
    Footer* aFooter(void);
  };
  
  LoaderSub* _pLoaderSub(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
