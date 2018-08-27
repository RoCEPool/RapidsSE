/*============================================================================*/
/* SEitf.hpp - se interface                                                   */
/*============================================================================*/
#pragma once

#include "Interface.h"

namespace Interface
{
  class SEitf : public Object
  {
  public:
    i8     m_iPid;             // process which created this seitf
    i8     m_iSlots;           // number of partition slots in each node
    i8     m_iPartition;       // which partition this seitf connects to
    
    DicStack m_xPathStack;      // dictionary stack
    
    Queue* m_pBufferQueue;     // buffer queue
    s48K   m_sResponse;        // response
    
    eEC    m_eErrorCode;       //
    s256   m_sErrorMsg;
    
    Table*        m_pCurrentTable;
    Index*        m_pCurrentIndex;
    Stz*          m_aRowPtr;
    EntryBuffer*  m_pEntryBuffer;
    Planner*      m_pPlanner;
    AccessPath*   m_pAccessPath;
  
    WorkingBuffer* m_pResultBuffer;
    FetchBuffer*  m_pFetchBuffer;
    
    Entry*        m_pEntry;
    WorkingRow*   m_pResultRow;
    Row*          m_aRow;
    Row**         m_ppRow;
  
    Tx*    m_pTx;
  
    bool   m_bDisplay;
    
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
    
    GroupBy*     m_pGroupBy[C_SLOTS_PER_NODE];   // working group by
  
    WorkingBuffer* m_pWorkingBuffer[C_SLOTS_PER_NODE]; // working row buffers
    
    RowOnce* m_pRowOnce; // has row been seen before?
  
    Formatters m_xFormatters; // record formatters
  
    PerformanceTrace* m_pPerformanceTrace;
  
    CommandProcessor* m_pCommandProcessor;
  
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    SEitf(i8 iPartition);
    ~SEitf(void);
    
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);
    
    void   vBufferQueue(Queue* pBufferQueue);
    Queue* pBufferQueue(void);

    void   vCurrentIndex(Index* pIndex);
    Index* pCurrentIndex(void);
    
    void   vCurrentTable(Table* pTable);
    Table* pCurrentTable(void);
    
    void   vContentDump(void);
    
    void   vInit(Stz* pCatalogName, Stz* pSchemaName);
    
    i8     iPartition(void);
    i8     iSlot(void);
    i8     nLastBuffers(void);
    void   vLastBuffers(i8 nLastBuffer);
  
    DicStack* pPathStack(void);
  
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
    
    bool bCopyTable(TokenVec& xTokenVec, i8 iReqNo);
    bool bExportTable(TokenVec& xTokenVec, i8 iReqNo);
    bool bDistExportTable(TokenVec& xTokenVec, i8 iReqNo);
    bool bDistFetchRowsIndex(TokenVec& xTokenVec, i8 iReqNo);
    bool bLoadTable(TokenVec& xTokenVec, i8 iReqNo);
    bool bLoadTableStream(TokenVec& xTokenVec, i8 iReqNo);
    bool bDistParallelLoadTable(TokenVec& xTokenVec, i8 iReqNo);
    
    bool bFetchRows(TokenVec& xTokenVec, i8 iReqNo);
    bool bSpawnFetchRows(TokenVec& xTokenVec, i8 iReqNo);
  
    bool bGetTask(TokenVec& xTokenVec, i8 iReqNo);
    bool bRetTask(TokenVec& xTokenVec, i8 iReqNo);
  
    bool bInsertRow(Stz* pRow, i8 iLength);
    bool bInsertRow(Stz* pRow, i8 iLength, i8 iPartition);
    bool bInsertBuffer(void);
    bool bInsertBuffer(i8 iPartition);
    bool bInsertLastBuffer(i8 iPartition);

    bool bInsertEntriesTable(WorkingBuffer* pWorkingBuffer, i8 iPartition);
    bool bInsertEntriesTableSlot(WorkingBuffer* pWorkingBuffer, i8 iSlot);

    bool bGetMonitor(TokenVec& xTokenVec, i8 iReqNo);
    bool bSeitfCount(TokenVec& xTokenVec, i8 iReqNo);
    
    bool bPing(TokenVec& xTokenVec, i8 iReqNo);
 
    bool bTxBegin(TokenVec& xTokenVec, i8 iReqNo);
    bool bTxCommit(TokenVec& xTokenVec, i8 iReqNo);
    bool bTxDumpAll(TokenVec& xTokenVec, i8 iReqNo);
    bool bTxEnd(TokenVec& xTokenVec, i8 iReqNo);
    bool bTxRollback(TokenVec& xTokenVec, i8 iReqNo);
  
    //--------------------------------------------------------------------------
    void vEndFetch(void);
    bool bExtractDateFormat(Stz* dateFormat);
    bool bNextBuffer(void);
    bool bNextRow(void);
    bool bNextRow2(i8& nPartition);
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
