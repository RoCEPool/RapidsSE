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
    Str*         m_aRowPtr;
    IndexBuffer* m_pIndexBuffer;
    
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
    
    bool bDecode(TokenSet& xTokenSet);
    
    bool bConnect(TokenSet& xTokenSet);
    bool bConnectCluster(TokenSet& xTokenSet);
    bool bDisconnect(TokenSet& xTokenSet);
    
    bool bGet(TokenSet& xTokenSet);
    bool bSet(TokenSet& xTokenSet);
    
    bool bRemoveFolder(TokenSet& xTokenSet);
    
    bool bCreateTenant(TokenSet& xTokenSet);
    bool bListTenants(TokenSet& xTokenSet);
    bool bDropTenant(TokenSet& xTokenSet);

    bool bCreateCatalog(TokenSet& xTokenSet);
    bool bListCatalogs(TokenSet& xTokenSet);
    bool bDropCatalog(TokenSet& xTokenSet);
    
    bool bCreateSchema(TokenSet& xTokenSet);
    bool bListSchemas(TokenSet& xTokenSet);
    bool bDropSchema(TokenSet& xTokenSet);
    bool bUseSchema(TokenSet& xTokenSet);
    
    bool bCreateIndex(TokenSet& xTokenSet);
    bool bDropIndex(TokenSet& xTokenSet);
    bool bDumpIndex(TokenSet& xTokenSet);
    bool bEndCreateIndex(TokenSet& xTokenSet);
    bool bListIndices(TokenSet& xTokenSet);
    bool bIndexSetUnique(TokenSet& xTokenSet);
    bool bVisualizeIndex(TokenSet& xTokenSet);
  
    bool bCreateTable(TokenSet& xTokenSet);
    bool bEndCreateTable(TokenSet& xTokenSet);
  
    bool bListTables(TokenSet& xTokenSet);
    bool bDisplayOff(TokenSet& xTokenSet);
    bool bDisplayOn(TokenSet& xTokenSet);
    bool bDocol(TokenSet& xTokenSet);
    bool bDropTable(TokenSet& xTokenSet);
    bool bDumpTable(TokenSet& xTokenSet);
   
    bool bAddColumn(TokenSet& xTokenSet);
    bool bAddIndexColumn(TokenSet& xTokenSet);
    bool bAddPartitionColumn(TokenSet& xTokenSet);
    bool bListColumns(TokenSet& xTokenSet);
    bool bListIndexColumns(TokenSet& xTokenSet);
    bool bListPartitionColumns(TokenSet& xTokenSet);
    
    bool bAnalyzeIndex(TokenSet& xTokenSet);
    bool bDanalyzeIndex(TokenSet& xTokenSet);
    bool bKeySelectIndex(TokenSet& xTokenSet);
    bool bLowerSelectIndex(TokenSet& xTokenSet);
    bool bRangeSelectIndex(TokenSet& xTokenSet);
    bool bSizeIndex(TokenSet& xTokenSet);
    bool bTruncateIndex(TokenSet& xTokenSet);
    bool bUpperSelectIndex(TokenSet& xTokenSet);

    bool bAnalyzeTable(TokenSet& xTokenSet);
    bool bCopyTable(TokenSet& xTokenSet);
    bool bExportTable(TokenSet& xTokenSet);
    bool bDanalyzeTable(TokenSet& xTokenSet);
    bool bDistExportTable(TokenSet& xTokenSet);
    bool bLoadTable(TokenSet& xTokenSet);
    bool bLoadTableStream(TokenSet& xTokenSet);
    bool bLowerSelectTable(TokenSet& xTokenSet);
    bool bSelectTable(TokenSet& xTokenSet);
    bool bRangeSelectTable(TokenSet& xTokenSet);
    bool bGetTable(TokenSet& xTokenSet);
    bool bInUseTable(TokenSet& xTokenSet);
    bool bRetTable(TokenSet& xTokenSet);
    bool bNodeStatus(TokenSet& xTokenSet);
    bool bPoolSpace(TokenSet& xTokenSet);
    bool bSizeTable(TokenSet& xTokenSet);
    bool bSizeTables(TokenSet& xTokenSet);
    bool bTruncateTable(TokenSet& xTokenSet);
    
    bool bGetTask(TokenSet& xTokenSet);
    bool bRetTask(TokenSet& xTokenSet);
  
    bool bInsertRow(Str* pRow);
    bool bInsertBuffer(void);
    bool bInsertBuffer(EntryBuffer* pRowBuffer, i8 iPartition);
    
    bool bPushDic(TokenSet& xTokenSet);
    bool bGetDicPath(TokenSet& xTokenSet);
    bool bGetDicContents(TokenSet& xTokenSet);
    bool bGetDicContentsG(TokenSet& xTokenSet);

    bool bGetMonitor(TokenSet& xTokenSet);
    bool bGetFreeSpace(TokenSet& xTokenSet);
    bool bGetInUseSpace(TokenSet& xTokenSet);
    bool bGetTotalSpace(TokenSet& xTokenSet);
    bool bGetVersion(TokenSet& xTokenSet);
    bool bSeitfCount(TokenSet& xTokenSet);
    
    bool bObject(TokenSet& xTokenSet);
    bool bPing(TokenSet& xTokenSet);
    bool bPrintRowBuffer(TokenSet& xTokenSet);
    
    bool bX(TokenSet& xTokenSet);
    bool bPrint(TokenSet& xTokenSet);
    
    //--------------------------------------------------------------------------
    bool bAppendRequest(StrVal& sRequest);
    
    eEC  eGetResponse(void);
    bool bPopResponse(eEC& eResult, StrVal& sResponse);
    
    //--------------------------------------------------------------------------
    void vClearLastError(void);
    eEC  eGetLastError(void);
    Str* pGetLastError(void);
    void vSetLastError(eEC eInterfaceError);
    
    Footer* pFooter(void);
  };
  
  SEitf* _pSEitf(void* aAddr);
  SEitf* _pSEitf(i8 iAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
