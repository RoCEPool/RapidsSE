/*============================================================================*/
/* Table.hpp - Table                                                          */
/*============================================================================*/
#pragma once

namespace MetaData
{
  static const char* C_SORT_REGION_BASE  = (char*)0x409600000000;
  static const i8    C_SORT_REGION_DELTA = C_1GB * 128;

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
    
    static void       vDump(eCN eClass, Object* pObject, Stz* aTab);
    static Table*     pFindTable(DicStack* pDicStack, Stz* aTableNm);
    static IndexDef*  pIndexDef(DicStack* pDicStack, Stz* aTableNm, i8 iIndexNo);
    static void       vMonitorPortrait(s48K& sPortrait);
    static StreamDef* pStreamDef(DicStack* pDicStack, Stz* aTableNm);
    
    i8      iPtnNoToTablePtn(tPN tPtnNo);
    tPN     tTablePtnToPtnNo(i8 i_TablePtn);

    void    vAddIndex(Index* pIndex);
    void    vAnalyze(i8 i_TablePtn, i8& nBuffers, i8& nRows);
    BucketMap* pBucketMap(void);
    void    vCompactIndex(Index* pIndex);
    void    vCreatePtnDef(void);
    i8      nCrwBuffers(void);
    void    vContentDump(void);
    Column* pFindColumn(Stz* szColNm);
    IdxVec* pIdxVec(void);
    i8      nIndexBuffers(i8 i_Index);
    bool    bIsDefinitionOnly(void);
    void    vIsDefinitionOnly(bool bIsDefinitionOnly);
    void    vIsLoading(bool bIsLoading);
    bool    bIsLoading(void);
    void    vLazyBuildIndexes(i8 i_TablePtn);
    void    vMarkTable(void);
    bool    bPopulateIndex(i8 iIndexNo);
    void    vRemoveIndex(Index* pIndex);
    StreamDef* pPtnStreamDef(void);
    bool    bPurge(i8 i_TablePtn, i8 iOlderThanSeconds);
    EntBuffer* pRootBuffer(i8 i_TablePtn, i8 i_Index);
    StreamDef* pStreamDef(void);
    void       vStreamDef(StreamDef* pStreamDef);
    Stz*    szTableNm(void);
    TablePtn* pTablePtn(i8 i_TablePtn);
    i8      iToPartition(i8 iRowHash);
    
    //void vInsdelBuffer(i8 iTxId, i8 i_TablePtn, CrwBuffer* pCrwBuffer);
    //void vInsdelBuffer(i8 iTxId, i8 i_TablePtn, RowBuffer* pRsbBuffer);
    //void vUpsertBuffer(i8 iTxId, i8 i_TablePtn, CrwBuffer* pCrwBuffer);
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
