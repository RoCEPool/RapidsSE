/*============================================================================*/
/* LoaderSub.hpp -- Subordinate Parallel Table Loader                         */
/*============================================================================*/
#pragma once

namespace DBobjects
{
  static const i8 C_MAX_NODES = 32;
  static const i8 C_MAX_PARTITIONS = C_SLOTS_PER_NODE * C_MAX_NODES;
  static const i8 C_DATA_LENGTH = C_256K;
  
  class LoaderSub : public Object
  {
  private:
    Loader*  m_pLoader;    // loader parent
    Table*   m_pTable;     // table being load
    TxRef*   m_pTxRef;     // tx
    LoadMap* m_pLoadMap;   // load map
    s256     m_aRecord;    // formatted row
    i8       m_iLength;    // length of record
    i8       m_iPartition; // partition of record
    
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
    void vHashRecord(void);
    bool bInsertRow(void);
    void vLoader(Loader* pLoader);
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
