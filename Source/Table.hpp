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
  
  static const char* C_SORT_REGION_BASE  = (char*)0x409600000000;
  static const i8    C_SORT_REGION_DELTA = C_1GB * 128;

  typedef VectorNL_t<CN_IDXVEC,Index,CN_INDEX,C_INDEXES_PER_TABLE,u8> IdxVec;
  
  class Table : public Object
  {
  private:
    s32    m_sTableName;   // table name
    eTT    m_eTableType;   // table type
    Dic    m_xSubDic;      // sub dictionary for this table
    Slots  m_xSlots;       // slots
    Atomic m_ziIndexNo;    // index no
    Index* m_pHiddenIndex; // hidden index
    
    IdxVec*    m_pIdxVec;    // index vector
    ColumnVec* m_pColumnVec; // column vector

    Atomic     m_ziSubLoaders[C_SLOTS_PER_NODE];  // subloader*slot count
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
    void vCompactIndex(Index* pIndex);
    void vLazyBuildIndexes(i8 iSlot);
    
    void vInsertRow(i8 iTxId, i8 iPartition, Stz* m_aRecord, i8 iRecordLength);
    void vInsertEntries(i8 iTxId, i8 iPartition, EntryBuffer* pEntryBuffer);
    ColumnVec* pColumnVec(void);
    void vContentDump(void);
    void vCreateColumnVec(s32& sName, s48K& sDefinition);
    void vCreateColumnVec(Jobs::Job_CreateTable* pJob);
    Index* pHiddenIndex(void);
    void vHiddenIndex(Index* pHiddenIndex);
    IdxVec* pIdxVec(void);
    void  vIsLoading(bool bIsLoading);
    bool  bIsLoading(void);
    bool  bIsValidIndexColumns(Jobs::Job_CreateIndex* pJob);
    TablePtn* pPartition(i8 iSlot);
    bool bPopulateIndex(i8 iIndexNo);
    void vRemoveIndex(Index* pIndex);
    i8   nIndexBuffers(i8 iIndexNo);
    PtnColVec*  pPtnColVec(void);
    void vPublish(i8 iPartitionNo);
    bool bPurge(i8 iPartitionNo, i8 iOlderThanSeconds);
    EntryBuffer* pRootBuffer(i8 i_Slot, i8 i_Index);
    i8   iRowPartition(Stz* pRow);

    void vShowColumnVec(void);
    Dic* pSubDic(void);
    void vSubLoaders(i8 iSubLoaders);
    Slots* pSlots(void);
    Stz* aTableName(void);
    void vTidyUp(void);
    void vTidyUpIndex(i8 iIndexNo);
    void vTruncate(i8 iPartitionNo);
    void vTruncateIndex(i8 iPartitionNo, i8 iIndexNo);
    bool bVerifyIndexes(void);
    bool bVerifyIndex(Index* pIndex);
   
    Footer* aFooter(void);
  
    i8 nHiddenEntryCount(EntryBuffer* pFirstL0Buffer);
    
//  void vNewIndexLoad(i8 iPartition, IndexTree* pIndexTree,
//                     IndexTree* pHiddenIndexTree, i8 nHiddenEntries);
    
//    void vPopulateSortArray(char* aSortArray, i8 nElements, i8 nElmSize,
//                            IndexTree* pIndexTree, EntryBuffer* pFirstL0Buffer);
//    void vSortTheArray(char* aSortArray, i8 nElements, i8 nElmSize,
//                       IndexTree* pIndexTree);
//    void vDumpSortArray(char* aSortArray, i8 nElements, i8 nElmSize,
//                        IndexTree* pIndexTree);
//    void vBuildNewIndex(char* aSortArray, i8 nElements, i8 nElmSize,
//                        IndexTree* pIndexTree);
//    void vBuildKeyWrk(Key* aSortElm, IdxColVec* pIdxColVec, KeyWrk& xKeyWrk);
  };
  
  Table* _pTable(void* aAddr);
  
//#ifdef __APPLE__
//  int iQsort_complex(void* aIndexTree, const void* aArg1, const void* aArg2);
//#else
//  int iQsort_complex(const void* aArg1, const void* aArg2, void* aIndexTree);
//#endif
//  int iQsort_i8(const void* aArg1, const void* aArg2);
//  int iQsort_i8i8(const void* aArg1, const void* aArg2);
//  int iQsort_i8i8i8(const void* aArg1, const void* aArg2);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
