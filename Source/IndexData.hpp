/*============================================================================*/
/* IndexData.hpp - index portion of table partition                           */
/*============================================================================*/
#pragma once

namespace DBobjects
{

  static const i8 C_HIGHER_LEVEL_ID = 0xFFFF;
  
  class Table;
  class Partition;
  class Index;
  
  class IndexData : public Object
  {
    
  private:
    Lock         m_zLock;
    Index*       m_pIndex;
    EntryBuffer* m_pRoot;
    EntryBuffer* m_pCurrentBuffer;
    i8           m_nPartitionNo;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, IndexData* pIndexData);
    void  operator delete(void*);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static void vFinalize(eCN eClass, Object* pObject);
    
    IndexData(Index* pIndex, i8 nPartitionNo);
    ~IndexData(void);
  
    Stz*         pAnalyze(void);
    void         vAppendEntriesFromBuffer(EntryBuffer* pEntryBuffer,EntryBuffer** ppFirstBuffer, Entry** ppFirstEntry);
    void         vAppendBuffer(EntryBuffer* pEntryBuffer);
    void         vAppendEntry(KeyWrk& xKeyWrk, Id xId, EntryVec& xPriorVec);
    void         vDump(bool bSameLevel);
    EntryBuffer* pFirstBuffer();
    Index*       pIndex(void);
    i8           nEntryBuffers(void);
    Entry*       pInsertEntry(i8 iTxId, Stz* pRow, i8 iLength);
    bool         bInsertEntry(Entry* pRow);
    void         vInsertEntriesFromBuffer(EntryBuffer* pEntryBuffer, Entry* pRowEntry);
    EntryBuffer* pPositionLevel(KeyWrk& xKeyWrk, i8 nKeyCols, eJC eJuxtaCompare, i8 iLevel);
    EntryBuffer* pRoot(void);
    void         vTidyUp(void);
    void         vTruncate(void);
    bool         bVerify(void);
    void         vVisualize(void);
    
    Footer*      aFooter(void);
    
  private:
    EntryBuffer* pAppendL0Entry(EntryBuffer* pRoot,KeyWrk& xKeyWrk, Id xId,
                                EntryVec& xPriorVec);
    void         vFixHigherBuffer(EntryBuffer* pEntryBuffer, i8 iLevel);
    bool         bInsertEntry(EntryBuffer* pRoot,KeyWrk& xKeyWrk, Id xId, i8 iLevel);
    void         vInsertFirstEntry(EntryBuffer* pToBuffer, EntryBuffer* pFromBuffer);
    EntryBuffer* pSplitBuffer(EntryBuffer* pEntryBuffer);
    void         vDumpLevel(EntryBuffer* pEntryBuffer);
  };
  
  IndexData* _pIndexData(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
