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
    
    DicPath m_xPathStack;     // path stack
    
    Queue* m_pRequestQueue;    // request queue
    Queue* m_pResponseQueue;   // response queue
    s48K   m_sResponse;        // response
    
    eEC    m_eErrorCode;       //
    s256   m_sErrorMsg;
    
    Table* m_pCurrentTable;
    Str*   m_aRowPtr;
    TxRef* m_pTxRef;
    
    i8     m_iCurrentRequestId;
    i8     m_iNextRequestId;
    i8     m_iRowPartition;
    
    RowBuffer* m_pRowBuffer[C_SLOTS_PER_NODE]; // working row buffers
    
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
    
    bool bDecode(Str* aWords[], i8 iWords);
    
    bool bConnect(Str* aWords[]);
    bool bConnectCluster(Str* aWords[]);
    bool bDisconnect(Str* aWords[]);
    
    bool bRemoveFolder(Str* aWords[], i8 iWords);

    bool bCreateTenant(Str* aWords[], i8 iWords);
    bool bListTenants(Str* aWords[], i8 iWords);
    bool bDropTenant(Str* aWords[], i8 iWords);

    bool bCreateCatalog(Str* aWords[], i8 iWords);
    bool bListCatalogs(Str* aWords[], i8 iWords);
    bool bDropCatalog(Str* aWords[], i8 iWords);
    
    bool bCreateSchema(Str* aWords[], i8 iWords);
    bool bListSchemas(Str* aWords[], i8 iWords);
    bool bDropSchema(Str* aWords[], i8 iWords);
    bool bUseSchema(Str* aWords[], i8 iWords);
    
    bool bCreateIndex(Str* aWords[], i8 iWords);
    bool bDropIndex(Str* aWords[], i8 iWords);
    bool bDumpIndex(Str* aWords[], i8 iWords);
    bool bListIndices(Str* aWords[], i8 iWords);
    
    bool bCreateTable(Str* aWords[], i8 iWords);
    
    bool bListTables(Str* aWords[], i8 iWords);
    bool bDocol(Str* aWords[]);
    bool bDropTable(Str* aWords[], i8 iWords);
    bool bDumpTable(Str* aWords[], i8 iWords);
   
    bool bAddColumn(Str* aWords[], i8 iWords);
    bool bEndCreateTable(Str* aWords[], i8 iWords);
    bool bListColumns(Str* aWords[], i8 iWords);
    
    bool bAnalyzeTable(Str* aWords[], i8 iWords);
    bool bCopyTable(Str* aWords[], i8 iWords);
    bool bExportTable(Str* aWords[], i8 iWords);
    bool bDanalyzeTable(Str* aWords[], i8 iWords);
    bool bDistExportTable(Str* aWords[], i8 iWords);
    bool bLoadTable(Str* aWords[], i8 iWords);
    bool bLoadTableStream(Str* aWords[], i8 iWords);
    bool bSelectTable(Str* aWords[], i8 iWords);
    bool bGetTable(Str* aWords[], i8 iWords);
    bool bInUseTable(Str* aWords[], i8 iWords);
    bool bRetTable(Str* aWords[], i8 iWords);
    bool bNodeStatus(Str* aWords[], i8 iWords);
    bool bPoolSpace(Str* aWords[], i8 iWords);
    bool bSizeTable(Str* aWords[], i8 iWords);
    bool bTruncateTable(Str* aWords[], i8 iWords);
    
    bool bInsertRow(Str* pRow);
    bool bInsertBuffer(void);
    bool bInsertBuffer(RowBuffer* pRowBuffer, i8 iPartition);
    
    bool bPushDic(Str* aWords[]);
    bool bGetDicPath(Str* aWords[]);
    bool bGetDicContents(Str* aWords[]);
    bool bGetDicContentsG(Str* aWords[]);

    bool bGetMonitor(Str* aWords[]);
    bool bGetFreeSpace(Str* aWords[]);
    bool bGetInUseSpace(Str* aWords[]);
    bool bGetTotalSpace(Str* aWords[]);
    bool bGetVersion(Str* aWords[]);
    bool bSeitfCount(Str* aWords[]);
    
    bool bObject(Str* aWords[]);
    bool bPing(Str* aWords[]);
    bool bPrintRowBuffer(Str* aWords[]);
    
    bool bX(Str* aWords[]);
    bool bPrint(Str* aWords[]);
    
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
