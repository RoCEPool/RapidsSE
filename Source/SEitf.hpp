/*============================================================================*/
/* SEitf.hpp - se interface                                                   */
/*============================================================================*/
#pragma once

#include "Interface.h"

namespace Interface
{
  class SEitf : public Object,
                public ListNode_t<SEitf>
  {
    static const i8 C_INPUT_ARRAY = 4;
    
  public:
    i8     m_iPid;             // process which created this seitf
    i8     m_iSlots;           // number of partition slots in each node
    i8     m_iPartition;       // which partition this seitf connects to
    
    DicPath m_xPathStack;      // path stack
    
    Queue* m_pRequestQueue;    // request queue
    Queue* m_pResponseQueue;   // response queue
    s48K   m_sResponse;        // response
    
    eEC    m_eErrorCode;       //
    s256   m_sErrorMsg;
    
    Table*       m_pCurrentTable;
    Index*       m_pCurrentIndex;
    Str*         m_aRowPtr;
    EntryBuffer* m_pIndexBuffer;
    AccessPath*  m_pAccessPath;
  
    Entry* m_pEntry;
    TxRef* m_pTxRef;
    
    bool   m_bDisplay;
    
    s256   m_sMetadata;
    i8     m_iRowsProcessed;
    i8     m_iCurrentRequestId;
    i8     m_iNextRequestId;
    i8     m_iRowPartition;
    
    EntryBuffer* m_pRowBuffer[C_SLOTS_PER_NODE]; // working row buffers
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    SEitf(i8 iPartition);
    ~SEitf(void);
    
    static void vDump(eCN eClass, Object* pObject, Str* aTab);
    
    DicPath* pPathStack(void);
    
    void   vCurrentIndex(Index* pIndex);
    Index* pCurrentIndex(void);
    
    void   vCurrentTable(Table* pTable);
    Table* pCurrentTable(void);
    
    void   vContentDump(void);
    
    void   vInit(void);
    i8     iPartition(void);
    void   vRequestQueue(Queue* pRequestQueue);
    Queue* pRequestQueue(void);
    
    void   vResponseQueue(Queue* pResponseQueue);
    Queue* pResponseQueue(void);

    //--------------------------------------------------------------------------
    
    bool bBeginTx(void);
    void vCommitTx(void);
    void vEndTx(void);
    
    bool bDecode(TokenVec& xTokenSet);
    
    bool bConnect(TokenVec& xTokenSet);
    bool bConnectCluster(TokenVec& xTokenSet);
    bool bDisconnect(TokenVec& xTokenSet);
    
    bool bGet(TokenVec& xTokenSet);
    bool bSet(TokenVec& xTokenSet);
    
    bool bRemoveFolder(TokenVec& xTokenSet);
    
    bool bCreateTenant(TokenVec& xTokenSet);
    bool bListTenants(TokenVec& xTokenSet);
    bool bDropTenant(TokenVec& xTokenSet);

    bool bCreateCatalog(TokenVec& xTokenSet);
    bool bListCatalogs(TokenVec& xTokenSet);
    bool bDropCatalog(TokenVec& xTokenSet);
    
    bool bCreateSchema(TokenVec& xTokenSet);
    bool bListSchemas(TokenVec& xTokenSet);
    bool bDropSchema(TokenVec& xTokenSet);
    bool bUseSchema(TokenVec& xTokenSet);
    
    bool bCreateIndex(TokenVec& xTokenSet);
    bool bDropIndex(TokenVec& xTokenSet);
    bool bDumpIndex(TokenVec& xTokenSet);
    bool bEndCreateIndex(TokenVec& xTokenSet);
    bool bListIndices(TokenVec& xTokenSet);
    bool bIndexSetUnique(TokenVec& xTokenSet);
    bool bVisualizeIndex(TokenVec& xTokenSet);
  
    bool bCreateTable(TokenVec& xTokenSet);
    bool bEndCreateTable(TokenVec& xTokenSet);
  
    bool bListTables(TokenVec& xTokenSet);
    bool bDisplayOff(TokenVec& xTokenSet);
    bool bDisplayOn(TokenVec& xTokenSet);
    bool bDocol(TokenVec& xTokenSet);
    bool bDropTable(TokenVec& xTokenSet);
    bool bDumpTable(TokenVec& xTokenSet);
   
    bool bAddColumn(TokenVec& xTokenSet);
    bool bAddIndexColumn(TokenVec& xTokenSet);
    bool bAddPartitionColumn(TokenVec& xTokenSet);
    bool bListColumns(TokenVec& xTokenSet);
    bool bListIndexColumns(TokenVec& xTokenSet);
    bool bListPartitionColumns(TokenVec& xTokenSet);
    
    bool bAnalyzeIndex(TokenVec& xTokenSet);
    bool bDanalyzeIndex(TokenVec& xTokenSet);
    bool bGetIndex(TokenVec& xTokenSet);
    bool bKeySelectIndex(TokenVec& xTokenSet);
    bool bLowerSelectIndex(TokenVec& xTokenSet);
    bool bRetIndex(TokenVec& xTokenSet);
    bool bRangeSelectIndex(TokenVec& xTokenSet);
    bool bSizeIndex(TokenVec& xTokenSet);
    bool bTruncateIndex(TokenVec& xTokenSet);
    bool bUpperSelectIndex(TokenVec& xTokenSet);

    bool bAnalyzeTable(TokenVec& xTokenSet);
    bool bCopyTable(TokenVec& xTokenSet);
    bool bExportTable(TokenVec& xTokenSet);
    bool bDanalyzeTable(TokenVec& xTokenSet);
    bool bDistExportTable(TokenVec& xTokenSet);
    bool bLoadTable(TokenVec& xTokenSet);
    bool bLoadTableStream(TokenVec& xTokenSet);
    bool bLowerSelectTable(TokenVec& xTokenSet);
    bool bSelectTable(TokenVec& xTokenSet);
    bool bRangeSelectTable(TokenVec& xTokenSet);
    bool bGetTable(TokenVec& xTokenSet);
    bool bInUseTable(TokenVec& xTokenSet);
    bool bRetTable(TokenVec& xTokenSet);
    bool bNodeStatus(TokenVec& xTokenSet);
    bool bPoolSpace(TokenVec& xTokenSet);
    bool bSizeTable(TokenVec& xTokenSet);
    bool bSizeTables(TokenVec& xTokenSet);
    bool bTruncateTable(TokenVec& xTokenSet);
    
    bool bFetchRows(TokenVec& xTokenSet);
  
    bool bGetTask(TokenVec& xTokenSet);
    bool bRetTask(TokenVec& xTokenSet);
  
    bool bInsertRow(Str* pRow);
    bool bInsertBuffer(void);
    bool bInsertBuffer(EntryBuffer* pRowBuffer, i8 iPartition);
    
    bool bPushDic(TokenVec& xTokenSet);
    bool bGetDicPath(TokenVec& xTokenSet);
    bool bGetDicContents(TokenVec& xTokenSet);
    bool bGetDicContentsG(TokenVec& xTokenSet);

    bool bGetMonitor(TokenVec& xTokenSet);
    bool bGetFreeSpace(TokenVec& xTokenSet);
    bool bGetInUseSpace(TokenVec& xTokenSet);
    bool bGetTotalSpace(TokenVec& xTokenSet);
    bool bGetVersion(TokenVec& xTokenSet);
    bool bSeitfCount(TokenVec& xTokenSet);
    
    bool bObject(TokenVec& xTokenSet);
    bool bPing(TokenVec& xTokenSet);
    bool bPrintRowBuffer(TokenVec& xTokenSet);
    
    bool bX(TokenVec& xTokenSet);
    bool bPrint(TokenVec& xTokenSet);
    
    //--------------------------------------------------------------------------
    bool bAppendRequest(StrVal& sRequest);
    
    eEC  eGetResponse(void);
    bool bPopResponse(eEC& eResult, StrVal& sResponse);
    
    //--------------------------------------------------------------------------
    void vClearLastError(void);
    eEC  eGetLastError(void);
    Str* pGetLastError(void);
    void vSetLastError(eEC eInterfaceError);
    
    Footer* aFooter(void);
  };
  
  SEitf* _pSEitf(void* aAddr);
  SEitf* _pSEitf(i8 iAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
