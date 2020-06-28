/*============================================================================*/
/* Table.hpp - Table                                                          */
/*============================================================================*/
#pragma once

namespace MetaData
{
  static const char* C_SORT_REGION_BASE  = (char*)0x409600000000;
  static const i8    C_SORT_REGION_DELTA = C_1GB * 128;

  typedef VectorNL_t<CN_IDXVEC,Index,C_MAX_INDEXES> IdxVec;
  typedef VectorNL_t<CN_TABLEPTNVEC,Partitioning::TablePtn,C_MAX_SLOTS>TablePtnVec;

  class Table : public Object
  {
  private:
    s32        m_sTableNm;    // table name (leave for the moment)
    IdxVec*    m_pIdxVec;     // index vector
    Atomic     m_ixIndexNo;   // next index no
    BucketMap* m_pBucketMap;  // bucket (to partition) map
    bool       m_bIsLoading;  // data is loading
    bool       m_bDefinitionOnly; // table is a definition only
    
    StreamDef*    m_pStreamDef;    // table stream def
    TablePtnVec*  m_pTablePtnVec;  // table partition vector
    StreamDef*    m_pPtnStreamDef; // partition stream def
        
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void* operator new(size_t, Table* pTable);
    void  operator delete(void*);
    
    Table(Stz* szTableNm);
   ~Table(void) noexcept(false);
    
    static void   vDump(eCN eClass, Object* pObject, Stz* aTab);
    static Table* pFindTable(DicStack* pDicStack, Stz* aTableNm);
    static void   vMonitorPortrait(s48K& sPortrait);
    
    void    vAddIndex(Index* pIndex);
    void    vAnalyze(i8 iSlot, i8& nBuffers, i8& nRows);
    void    vCompactIndex(Index* pIndex);
    void    vCreatePtnDef(void);
    i8      nCrwBuffers(void);
    void    vContentDump(void);
    Column* pFindColumn(Stz* szColNm);
    IdxVec* pIdxVec(void);
    i8      nIndexBuffers(i8 iIndexNo);
    bool    bIsDefinitionOnly(void);
    void    vIsDefinitionOnly(bool bIsDefinitionOnly);
    void    vIsLoading(bool bIsLoading);
    bool    bIsLoading(void);
    void    vLazyBuildIndexes(i8 iSlot);
    void    vMarkTable(void);
    TablePtn* pPartition(i8 iSlot);
    bool    bPopulateIndex(i8 iIndexNo);
    void    vRemoveIndex(Index* pIndex);
    StreamDef* pPtnStreamDef(void);
    void    vPublish(i8 iPartitionNo);
    bool    bPurge(i8 iPartitionNo, i8 iOlderThanSeconds);
    EntBuffer* pRootBuffer(i8 i_Slot, i8 i_Index);
    StreamDef* pStreamDef(void);
    void       vStreamDef(StreamDef* pStreamDef);
    Stz*    szTableNm(void);
    void    vTidyUp(void);
    void    vTidyUpIndex(i8 iIndexNo);
    i8      iToPartition(i8 iRowHash);
    
    void vInsdelBuffer(i8 iTxId, i8 iPartition, CrwBuffer* pCrwBuffer);
    void vUpsertBuffer(i8 iTxId, i8 iPartition, CrwBuffer* pCrwBuffer);
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
