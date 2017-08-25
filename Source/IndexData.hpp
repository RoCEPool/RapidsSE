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
    void         vAppendBuffer(EntryBuffer* pEntryBuffer);
    void         vAppendEntry(KeyWrk& xKeyWrk, Id xId);
    void         vDump(bool bSameLevel);
    EntryBuffer* pFirstBuffer();
    Index*       pIndex(void);
    i8           nIndexBuffers(void);
    Entry*       pInsertEntry(i8 iTxId, Stz* pRow, i8 iLength);
    void         vInsertEntry(Entry* pRow);
    EntryBuffer* pPositionLevel(KeyWrk& xKeyWrk, eJC eJuxtaCompare, i8 iLevel);
    EntryBuffer* pRoot(void);
    void         vTidyUp(void);
    void         vTruncate(void);
    bool         bVerify(void);
    void         vVisualize(void);
    
    Footer*      aFooter(void);
    
  private:
    EntryBuffer* pAppendEntry(EntryBuffer* pRoot,KeyWrk& xKeyWrk, Id xId, i8 iLevel);
    void         vFixHigherBuffer(EntryBuffer* pIndexBuffer, i8 iLevel);
    void         vInsertEntry(EntryBuffer* pRoot,KeyWrk& xKeyWrk, Id xId, i8 iLevel);
    void         vInsertFirstEntry(EntryBuffer* pToBuffer, EntryBuffer* pFromBuffer);
    EntryBuffer* pSplitBuffer(EntryBuffer* pIndexBuffer);
    void         vDumpLevel(EntryBuffer* pIndexBuffer);
  };
  
  IndexData* _pIndexData(void* pAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
