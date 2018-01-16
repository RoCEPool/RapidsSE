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
    Queue* m_pBufferQueue;     // buffer queue
    s48K   m_sResponse;        // response
    
    eEC    m_eErrorCode;       //
    s256   m_sErrorMsg;
    
    Table*       m_pCurrentTable;
    Index*       m_pCurrentIndex;
    Stz*         m_aRowPtr;
    EntryBuffer* m_pEntryBuffer;
    Planner*     m_pPlanner;
    AccessPath*  m_pAccessPath;
  
    Entry* m_pEntry;
    Tx*    m_pTx;
  
    bool   m_bDisplay;
    
    s4K    m_sMetadata;
    i8     m_iRowsProcessed;
    i8     m_iCurrentRequestId;
    i8     m_iNextRequestId;
    i8     m_iRowPartition;
  
    i8     m_ziLastBuffers;     // number of last buffers for fetch
    i8     m_ziLastBuffersScanned; // number of last buffers scanned
    
    Atomic m_ziFetchTasks;
    bool   m_bSeitfExiting;

    Atomic m_ziAcksRemaining;

    i8     m_iRowsInvalid;
    i8     m_iRowsValid;
    
    EntryBuffer* m_pRowBuffer[C_SLOTS_PER_NODE]; // working row buffers
    GroupBy*     m_pGroupBy[C_SLOTS_PER_NODE];   // working group by
  
    RowOnce* m_pRowOnce; // has row been seen before?
  
    Formatters m_xFormatters; // record formatters
  
    PerformanceTrace* m_pPerformanceTrace;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    SEitf(i8 iPartition);
    ~SEitf(void);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    DicPath* pPathStack(void);
    
    void   vBufferQueue(Queue* pBufferQueue);
    Queue* pBufferQueue(void);

    void   vCurrentIndex(Index* pIndex);
    Index* pCurrentIndex(void);
    
    void   vCurrentTable(Table* pTable);
    Table* pCurrentTable(void);
    
    void   vContentDump(void);
    
    void   vInit(void);
    i8     iPartition(void);
    i8     iSlot(void);
    i8     nLastBuffers(void);
    void   vLastBuffers(i8 nLastBuffer);
  
    void   vRequestQueue(Queue* pRequestQueue);
    Queue* pRequestQueue(void);
    
    void   vResponseQueue(Queue* pResponseQueue);
    Queue* pResponseQueue(void);
  
    void   vSetAcksRemaining(i8 iAcksRemaining);
    i8     iGetAcksRemaining(void);
    void   vDecAcksRemaining(void);
  
    void   vCreateRowOnce(void);
    bool   bRowSeen(Id xRowId);
   
    Tx*    pTx(void);
    void   vTx(Tx* pTx);
  
    PerformanceTrace* pPerformanceTrace(void);
    void   vPerformanceTrace(PerformanceTrace* pPerformanceTrace);
  
    //--------------------------------------------------------------------------
    
    bool bBeginTx(void);
    void vCommitTx(void);
    void vEndTx(void);
    
    bool bDecode(TokenVec& xTokenVec);
    
    bool bCheckpoint(TokenVec& xTokenVec, i8 iReqNo);
    
    bool bConnect(TokenVec& xTokenVec, i8 iReqNo);
    bool bConnectCluster(TokenVec& xTokenVec, i8 iReqNo);
    bool bDisconnect(TokenVec& xTokenVec, i8 iReqNo);
    
    bool bGetAttachMap(TokenVec& xTokenVec, i8 iReqNo);
    bool bSetAttachMap(TokenVec& xTokenVec, i8 iReqNo);
    
    bool bReload(TokenVec& xTokenVec, i8 iReqNo);
    bool bRemoveFolder(TokenVec& xTokenVec, i8 iReqNo);
    
    bool bCreateTenant(TokenVec& xTokenVec, i8 iReqNo);
    bool bListTenants(TokenVec& xTokenVec, i8 iReqNo);
    bool bDropTenant(TokenVec& xTokenVec, i8 iReqNo);

    bool bCreateCatalog(TokenVec& xTokenVec, i8 iReqNo);
    bool bListCatalogs(TokenVec& xTokenVec, i8 iReqNo);
    bool bDropCatalog(TokenVec& xTokenVec, i8 iReqNo);
    
    bool bCreateSchema(TokenVec& xTokenVec, i8 iReqNo);
    bool bListSchemas(TokenVec& xTokenVec, i8 iReqNo);
    bool bDropSchema(TokenVec& xTokenVec, i8 iReqNo);
    bool bUseSchema(TokenVec& xTokenVec, i8 iReqNo);
    
    bool bCreateIndex(TokenVec& xTokenVec, i8 iReqNo);
    bool bDropIndex(TokenVec& xTokenVec, i8 iReqNo);
    bool bDumpIndex(TokenVec& xTokenVec, i8 iReqNo);
    bool bEndCreateIndex(TokenVec& xTokenVec, i8 iReqNo);
    bool bListIndices(TokenVec& xTokenVec, i8 iReqNo);
    bool bIndexSetUnique(TokenVec& xTokenVec, i8 iReqNo);
    bool bVisualizeIndex(TokenVec& xTokenVec, i8 iReqNo);
  
    bool bCreateTable(TokenVec& xTokenVec, i8 iReqNo);
    bool bEndCreateTable(TokenVec& xTokenVec, i8 iReqNo);
  
    bool bListTables(TokenVec& xTokenVec, i8 iReqNo);
    bool bDisplayOff(TokenVec& xTokenVec, i8 iReqNo);
    bool bDisplayOn(TokenVec& xTokenVec, i8 iReqNo);
    bool bDocol(TokenVec& xTokenVec, i8 iReqNo);
    bool bDropTable(TokenVec& xTokenVec, i8 iReqNo);
    bool bDumpTable(TokenVec& xTokenVec, i8 iReqNo);
   
    bool bAddColumn(TokenVec& xTokenVec, i8 iReqNo);
    bool bAddIndexColumn(TokenVec& xTokenVec, i8 iReqNo);
    bool bAddPartitionColumn(TokenVec& xTokenVec, i8 iReqNo);
    bool bListColumns(TokenVec& xTokenVec, i8 iReqNo);
    bool bListIndexColumns(TokenVec& xTokenVec, i8 iReqNo);
    bool bListPartitionColumns(TokenVec& xTokenVec, i8 iReqNo);
    
    bool bAnalyzeIndex(TokenVec& xTokenVec, i8 iReqNo);
    bool bDanalyzeIndex(TokenVec& xTokenVec, i8 iReqNo);
    bool bGetIndex(TokenVec& xTokenVec, i8 iReqNo);
    bool bRetIndex(TokenVec& xTokenVec, i8 iReqNo);
    bool bSizeIndex(TokenVec& xTokenVec, i8 iReqNo);
    bool bTruncateIndex(TokenVec& xTokenVec, i8 iReqNo);
    bool bVerifyIndex(TokenVec& xTokenVec, i8 iReqNo);

    bool bAnalyzeTable(TokenVec& xTokenVec, i8 iReqNo);
    bool bCopyTable(TokenVec& xTokenVec, i8 iReqNo);
    bool bExportTable(TokenVec& xTokenVec, i8 iReqNo);
    bool bDanalyzeTable(TokenVec& xTokenVec, i8 iReqNo);
    bool bDistExportTable(TokenVec& xTokenVec, i8 iReqNo);
    bool bDistFetchRowsIndex(TokenVec& xTokenVec, i8 iReqNo);
    bool bLoadTable(TokenVec& xTokenVec, i8 iReqNo);
    bool bLoadTableStream(TokenVec& xTokenVec, i8 iReqNo);
    bool bGetTable(TokenVec& xTokenVec, i8 iReqNo);
    bool bGetTableNew(TokenVec& xTokenVec, i8 iReqNo);
    bool bInUseTable(TokenVec& xTokenVec, i8 iReqNo);
    bool bRetTable(TokenVec& xTokenVec, i8 iReqNo);
    bool bNodeSanity(TokenVec& xTokenVec, i8 iReqNo);
    bool bNodeStatus(TokenVec& xTokenVec, i8 iReqNo);
    bool bPoolSpace(TokenVec& xTokenVec, i8 iReqNo);
    bool bSizeTable(TokenVec& xTokenVec, i8 iReqNo);
    bool bSizeTables(TokenVec& xTokenVec, i8 iReqNo);
    bool bTruncateTable(TokenVec& xTokenVec, i8 iReqNo);
    
    bool bFetchRows(TokenVec& xTokenVec, i8 iReqNo);
    bool bSpawnFetchRows(TokenVec& xTokenVec, i8 iReqNo);
  
    bool bGetTask(TokenVec& xTokenVec, i8 iReqNo);
    bool bRetTask(TokenVec& xTokenVec, i8 iReqNo);
  
    bool bInsertRow(Stz* pRow, i8 iLength);
    bool bInsertRow(Stz* pRow, i8 iLength, i8 iPartition);
    bool bInsertBuffer(void);
    bool bInsertBuffer(i8 iPartition);
    bool bInsertLastBuffer(i8 iPartition);

    bool bInsertEntriesTable(EntryBuffer* pRowBuffer, i8 iPartition);
    bool bInsertEntriesTableSlot(EntryBuffer* pRowBuffer, i8 iSlot);
    
    bool bChangeFolder(TokenVec& xTokenVec, i8 iReqNo);
    bool bPrintWorkingFolder(TokenVec& xTokenVec, i8 iReqNo);
    bool bListFolder(TokenVec& xTokenVec, i8 iReqNo);

    bool bGetMonitor(TokenVec& xTokenVec, i8 iReqNo);
    bool bGetFreeSpace(TokenVec& xTokenVec, i8 iReqNo);
    bool bGetInUseSpace(TokenVec& xTokenVec, i8 iReqNo);
    bool bGetTotalSpace(TokenVec& xTokenVec, i8 iReqNo);
    bool bGetVersion(TokenVec& xTokenVec, i8 iReqNo);
    bool bSeitfCount(TokenVec& xTokenVec, i8 iReqNo);
    
    bool bPing(TokenVec& xTokenVec, i8 iReqNo);

    bool bPrint(TokenVec& xTokenVec, i8 iReqNo);
 
    bool bTxBegin(TokenVec& xTokenVec, i8 iReqNo);
    bool bTxCommit(TokenVec& xTokenVec, i8 iReqNo);
    bool bTxDumpAll(TokenVec& xTokenVec, i8 iReqNo);
    bool bTxEnd(TokenVec& xTokenVec, i8 iReqNo);
    bool bTxRollback(TokenVec& xTokenVec, i8 iReqNo);
  
    //--------------------------------------------------------------------------
    void vEndFetch(void);
    bool bNextBuffer(void);
    bool bNextRow(void);
    void vPopBufferQueue(void);
    
    //--------------------------------------------------------------------------
    bool bAppendRequest(StrVal& sRequest);
    
    eEC  eGetResponse(void);
    
    bool bPopResponse(eEC& eResult, StrVal& sResponse);
    
    //--------------------------------------------------------------------------
    void vClearLastError(void);
    eEC  eGetLastError(void);
    Stz* pGetLastError(void);
    void vSetLastError(eEC eInterfaceError);
    
    Footer* aFooter(void);
  };
  
  SEitf* _pSEitf(void* aAddr);
  SEitf* _pSEitf(i8 iAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
