/*============================================================================*/
/* Table.hpp - Table                                                          */
/*============================================================================*/
#pragma once

namespace MetaData
{
  class Index;
  class EntBuffer;
  
  static const char* C_SORT_REGION_BASE  = (char*)0x409600000000;
  static const i8    C_SORT_REGION_DELTA = C_1GB * 128;

  typedef VectorNL_t<CN_IDXVEC,Index,C_MAX_INDEXES> IdxVec;
  
  class Table : public Object
  {
  private:
    TableDef* m_pTableDef;   // definition of this table
  
    s32 m_sTableName;        // table name
  
    Dic m_xSubDic;           // sub dictionary for this table
  
    //ColumnVec* m_pColumnVec; // column vector
    IdxVec*    m_pIdxVec;    // index vector
    Atomic     m_ixIndexNo;  // next index no

    BucketMap* m_pBucketMap; // bucket (to partition) map
    Slots      m_xSlots;     // slots
  
    Atomic     m_ixSubLoaders[C_MAX_SLOTS];  // subloader*slot count
    bool       m_bIsLoading; // data is loading
  
    // - - - - - - - - - - - - -
    // in-progress
    StreamDef* m_pStreamDef;   // definition
  

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
    static Index* pFindIndex(DicStack* pDicStack, Stz* aTableNm, Stz* aIndexNm);
    static Table* pFindTable(DicStack* pDicStack, Stz* aTableNm);
    static void  vMonitorPortrait(s48K& sPortrait);
    
    void vAddIndex(Index* pIndex);
    Stz* pAnalyzeIndex(Index* pIndex, bool bDetailed, i8& nRows);
    Stz* pAnalyzeTable(bool bDetailed, i8& nRows);
    void vCompactIndex(Index* pIndex);
    i8   nCrwBuffers(void);
    ColumnVec* pColumnVec(void);
    void vContentDump(void);
    void vCreateColumnVec(s32& sName, s48K& sDefinition);
    void vCreateColumnVec(Jobs::Job_CreateTable* pJob);
    IdxVec* pIdxVec(void);
    i8   nIndexBuffers(i8 iIndexNo);
    void vInsertEntries(i8 iTxId, i8 iPartition,
                        Rows::CrwBuffer* pCrwBuffer);
    void vIsLoading(bool bIsLoading);
    bool bIsLoading(void);
    bool bIsValidIndexColumns(Jobs::Job_CreateIndex* pJob);
    void vLazyBuildIndexes(i8 iSlot);
    void vMarkTable(void);
    Partitioning::TablePtn* pPartition(i8 iSlot);
    bool bPopulateIndex(i8 iIndexNo);
    void vRemoveIndex(Index* pIndex);
    PtnColVec*  pPtnColVec(void);
    void vPublish(i8 iPartitionNo);
    bool bPurge(i8 iPartitionNo, i8 iOlderThanSeconds);
    EntBuffer* pRootBuffer(i8 i_Slot, i8 i_Index);
    void vShowColumnVec(void);
    Dic* pSubDic(void);
    void vSubLoaders(i8 iSubLoaders);
    Slots* pSlots(void);
    StreamDef* pStreamDef(void);
    TableDef* pTableDef(void);
    void vTableDef(TableDef* pTableDef);
    Stz* aTableName(void);
    void vTidyUp(void);
    void vTidyUpIndex(i8 iIndexNo);
    i8   iToPartition(i8 iRowHash);
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
