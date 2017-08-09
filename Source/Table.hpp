/*============================================================================*/
/* Table.hpp - Table                                                          */
/*============================================================================*/
#pragma once

namespace DBobjects
{  
  enum eTT // Table Type
  {
    /* 0 */ TT_NONE=0,
    /* 1 */ TT_NORMAL
  };

  static Stz* aTT[]
  {
    /* 0 */ "NONE"_Stz,
    /* 1 */ "NORMAL"_Stz
  };
  
  typedef Vector<CN_IDXVEC,Index,CN_INDEX,C_INDEXES_PER_TABLE,u8> IdxVec;
  
  class Table : public Object
  {
  private:
    s32    m_sTableName;   // table name
    eTT    m_eTableType;   // table type
    Dic    m_xSubDic;      // sub dictionary for this table
    Slots  m_xSlots;       // slots
    Atomic m_zIndexNo;
    Atomic m_zRefCount;    // reference count
    Index* m_pHiddenIndex; // hidden index
    
    IdxVec*    m_pIdxVec;    // index vector
    ColumnVec* m_pColumnVec; // colummn vector
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Table* pTable);
    void  operator delete(void*);
    
    Table(void);
    Table(Stz* aTableName);
    ~Table(void);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static void vFinalize(eCN eClass, Object* pObject);
    
    void vAddIndex(Index* pIndex);
    Stz* pAnalyze(bool bDetailed);
    Stz* pAnalyzeIndex(Index* pIndex, bool bDetailed);
    void vAppendBuffer(i8 iPartition, EntryBuffer* pRowBuffer);    
    ColumnVec* pColumnVec(void);
    void vContentDump(void);
    void vCreateColumnVec(s32& sName, s48K& sDefinition);
    Index* pHiddenIndex(void);
    void vHiddenIndex(Index* pHiddenIndex);
    IdxVec* pIdxVec(void);
    bool bInsertRow(EntryBuffer* pRowBuffer, i8 iPartition, TxRef* pTxRef, Stz* pRecord, i8 iLength);
    Partition* pPartition(i8 iSlot);
    void vPopulateIndex(i8 iIndexNo);
    void vRefDec(void);
    void vRefInc(void);
    bool bRefOut(void);
    i8   nRowBuffers(void);
    i8   nIndexBuffers(i8 iIndexNo);
    i8   iRowPartition(Stz* pRow);

    void vShowColumnVec(void);
    Dic* pSubDic(void);
    Slots* pSlots(void);
    Stz* aTableName(void);
    void vTidyUp(void);
    void vTidyUpIndex(i8 iIndexNo);
    void vTruncate(void);
    void vTruncateIndex(i8 iIndexNo);
  
    Footer* aFooter(void);
  };
  
  Table* _pTable(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
