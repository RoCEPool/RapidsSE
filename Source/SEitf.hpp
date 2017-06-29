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
    
    void   vBufferQueue(Queue* pBufferQueue);
    Queue* pBufferQueue(void);

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
    
    bool bDecode(TokenVec& xTokenVec);
    
    bool bConnect(TokenVec& xTokenVec);
    bool bConnectCluster(TokenVec& xTokenVec);
    bool bDisconnect(TokenVec& xTokenVec);
    
    bool bGet(TokenVec& xTokenVec);
    bool bSet(TokenVec& xTokenVec);
    
    bool bRemoveFolder(TokenVec& xTokenVec);
    
    bool bCreateTenant(TokenVec& xTokenVec);
    bool bListTenants(TokenVec& xTokenVec);
    bool bDropTenant(TokenVec& xTokenVec);

    bool bCreateCatalog(TokenVec& xTokenVec);
    bool bListCatalogs(TokenVec& xTokenVec);
    bool bDropCatalog(TokenVec& xTokenVec);
    
    bool bCreateSchema(TokenVec& xTokenVec);
    bool bListSchemas(TokenVec& xTokenVec);
    bool bDropSchema(TokenVec& xTokenVec);
    bool bUseSchema(TokenVec& xTokenVec);
    
    bool bCreateIndex(TokenVec& xTokenVec);
    bool bDropIndex(TokenVec& xTokenVec);
    bool bDumpIndex(TokenVec& xTokenVec);
    bool bEndCreateIndex(TokenVec& xTokenVec);
    bool bListIndices(TokenVec& xTokenVec);
    bool bIndexSetUnique(TokenVec& xTokenVec);
    bool bVisualizeIndex(TokenVec& xTokenVec);
  
    bool bCreateTable(TokenVec& xTokenVec);
    bool bEndCreateTable(TokenVec& xTokenVec);
  
    bool bListTables(TokenVec& xTokenVec);
    bool bDisplayOff(TokenVec& xTokenVec);
    bool bDisplayOn(TokenVec& xTokenVec);
    bool bDocol(TokenVec& xTokenVec);
    bool bDropTable(TokenVec& xTokenVec);
    bool bDumpTable(TokenVec& xTokenVec);
   
    bool bAddColumn(TokenVec& xTokenVec);
    bool bAddIndexColumn(TokenVec& xTokenVec);
    bool bAddPartitionColumn(TokenVec& xTokenVec);
    bool bListColumns(TokenVec& xTokenVec);
    bool bListIndexColumns(TokenVec& xTokenVec);
    bool bListPartitionColumns(TokenVec& xTokenVec);
    
    bool bAnalyzeIndex(TokenVec& xTokenVec);
    bool bDanalyzeIndex(TokenVec& xTokenVec);
    bool bGetIndex(TokenVec& xTokenVec);
    bool bKeySelectIndex(TokenVec& xTokenVec);
    bool bLowerSelectIndex(TokenVec& xTokenVec);
    bool bRetIndex(TokenVec& xTokenVec);
    bool bRangeSelectIndex(TokenVec& xTokenVec);
    bool bScanSelectIndex(TokenVec& xTokenVec);
    bool bSizeIndex(TokenVec& xTokenVec);
    bool bTruncateIndex(TokenVec& xTokenVec);
    bool bUpperSelectIndex(TokenVec& xTokenVec);

    bool bAnalyzeTable(TokenVec& xTokenVec);
    bool bCopyTable(TokenVec& xTokenVec);
    bool bExportTable(TokenVec& xTokenVec);
    bool bDanalyzeTable(TokenVec& xTokenVec);
    bool bDistExportTable(TokenVec& xTokenVec);
    bool bLoadTable(TokenVec& xTokenVec);
    bool bLoadTableStream(TokenVec& xTokenVec);
    bool bLowerSelectTable(TokenVec& xTokenVec);
    bool bSelectTable(TokenVec& xTokenVec);
    bool bRangeSelectTable(TokenVec& xTokenVec);
    bool bGetTable(TokenVec& xTokenVec);
    bool bInUseTable(TokenVec& xTokenVec);
    bool bRetTable(TokenVec& xTokenVec);
    bool bNodeStatus(TokenVec& xTokenVec);
    bool bPoolSpace(TokenVec& xTokenVec);
    bool bSizeTable(TokenVec& xTokenVec);
    bool bSizeTables(TokenVec& xTokenVec);
    bool bTruncateTable(TokenVec& xTokenVec);
    
    bool bFetchRows(TokenVec& xTokenVec);
  
    bool bGetTask(TokenVec& xTokenVec);
    bool bRetTask(TokenVec& xTokenVec);
  
    bool bInsertRow(Str* pRow);
    bool bInsertBuffer(void);
    bool bInsertBuffer(EntryBuffer* pRowBuffer, i8 iPartition);
    
    bool bPushDic(TokenVec& xTokenVec);
    bool bGetDicPath(TokenVec& xTokenVec);
    bool bGetDicContents(TokenVec& xTokenVec);
    bool bGetDicContentsG(TokenVec& xTokenVec);

    bool bGetMonitor(TokenVec& xTokenVec);
    bool bGetFreeSpace(TokenVec& xTokenVec);
    bool bGetInUseSpace(TokenVec& xTokenVec);
    bool bGetTotalSpace(TokenVec& xTokenVec);
    bool bGetVersion(TokenVec& xTokenVec);
    bool bSeitfCount(TokenVec& xTokenVec);
    
    bool bObject(TokenVec& xTokenVec);
    bool bPing(TokenVec& xTokenVec);
    bool bPrintRowBuffer(TokenVec& xTokenVec);
    
    bool bX(TokenVec& xTokenVec);
    bool bPrint(TokenVec& xTokenVec);
    
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
