/*============================================================================*/
/* Table.hpp - Table                                                          */
/*============================================================================*/
#pragma once

namespace MetaData
{
  class Index;
  class EntBuffer;
  
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
  
  static const char* C_SORT_REGION_BASE  = (char*)0x409600000000;
  static const i8    C_SORT_REGION_DELTA = C_1GB * 128;

  typedef VectorNL_t<CN_IDXVEC,Index,CN_INDEX,C_MAX_INDEXES,u8> IdxVec;
  
  class Table : public Object
  {
  private:
    s32    m_sTableName;   // table name
    eTT    m_eTableType;   // table type
    Dic    m_xSubDic;      // sub dictionary for this table
    Slots  m_xSlots;       // slots
    Atomic m_ixIndexNo;    // index no
    
    IdxVec*    m_pIdxVec;    // index vector
    ColumnVec* m_pColumnVec; // column vector

    Atomic     m_ixSubLoaders[C_MAX_SLOTS];  // subloader*slot count
    bool       m_bIsLoading;   // data is loading
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Table* pTable);
    void  operator delete(void*);
    
    Table(void);
    ~Table(void) noexcept(false);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    static void vFinalize(eCN eClass, Object* pObject);
    static Table* pFindTable(DicStack* pDicStack, Stz* aTableNm);
    static void  vMonitorPortrait(s48K& sPortrait);
    
    void vAddIndex(Index* pIndex);
    Stz* pAnalyzeIndex(Index* pIndex, bool bDetailed, i8& nRows);
    Stz* pAnalyzeTable(bool bDetailed, i8& nRows);
    void vCompactIndex(Index* pIndex);
    void vLazyBuildIndexes(i8 iSlot);
    
    void vInsertEntries(i8 iTxId, i8 iPartition,
                        Data::CrwBuffer* pCrwBuffer);
    ColumnVec* pColumnVec(void);
    void vContentDump(void);
    void vCreateColumnVec(s32& sName, s48K& sDefinition);
    void vCreateColumnVec(Jobs::Job_CreateTable* pJob);
    IdxVec* pIdxVec(void);
    void  vIsLoading(bool bIsLoading);
    bool  bIsLoading(void);
    bool  bIsValidIndexColumns(Jobs::Job_CreateIndex* pJob);
    void  vMarkTable(void);
    Partitioning::TablePtn* pPartition(i8 iSlot);
    bool bPopulateIndex(i8 iIndexNo);
    void vRemoveIndex(Index* pIndex);
    i8   nIndexBuffers(i8 iIndexNo);
    PtnColVec*  pPtnColVec(void);
    void vPublish(i8 iPartitionNo);
    bool bPurge(i8 iPartitionNo, i8 iOlderThanSeconds);
    EntBuffer* pRootBuffer(i8 i_Slot, i8 i_Index);
    i8   iRowPartition(i8 iHash);

    i8   nCrwBuffers(void);
    void vShowColumnVec(void);
    Dic* pSubDic(void);
    void vSubLoaders(i8 iSubLoaders);
    Slots* pSlots(void);
    Stz* aTableName(void);
    void vTidyUp(void);
    void vTidyUpIndex(i8 iIndexNo);
    void vTruncate(i8 iPartitionNo);
    void vTruncateIndex(i8 iPartitionNo, i8 iIndexNo);
    bool bVerifyTable(void);
    bool bVerifyIndexes(void);
    bool bVerifyIndex(Index* pIndex);
   
    Footer* aFooter(void);
  };
  
  Table* _pTable(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
