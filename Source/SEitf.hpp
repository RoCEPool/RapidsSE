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
    char*  m_pRowPtr;
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
    
    bool bDecode(char* aWords[], i8 iWords);
    
    bool bConnect(char* aWords[]);
    bool bConnectCluster(char* aWords[]);
    bool bDisconnect(char* aWords[]);
    
    bool bRemoveFolder(char* aWords[], i8 iWords);

    bool bCreateTenant(char* aWords[], i8 iWords);
    bool bListTenants(char* aWords[], i8 iWords);
    bool bDropTenant(char* aWords[], i8 iWords);

    bool bCreateCatalog(char* aWords[], i8 iWords);
    bool bListCatalogs(char* aWords[], i8 iWords);
    bool bDropCatalog(char* aWords[], i8 iWords);
    
    bool bCreateSchema(char* aWords[], i8 iWords);
    bool bListSchemas(char* aWords[], i8 iWords);
    bool bDropSchema(char* aWords[], i8 iWords);
    bool bUseSchema(char* aWords[], i8 iWords);
    
    bool bCreateIndex(char* aWords[], i8 iWords);
    bool bDumpIndex(char* aWords[], i8 iWords);
    bool bListIndices(char* aWords[], i8 iWords);
    
    bool bCreateTable(char* aWords[], i8 iWords);
    
    bool bListTables(char* aWords[], i8 iWords);
    bool bDocol(char* aWords[]);
    bool bDropTable(char* aWords[], i8 iWords);
    bool bDumpTable(char* aWords[], i8 iWords);
   
    bool bAddColumn(char* aWords[], i8 iWords);
    bool bListColumns(char* aWords[], i8 iWords);
    
    bool bAnalyzeTable(char* aWords[], i8 iWords);
    bool bCopyTable(char* aWords[], i8 iWords);
    bool bExportTable(char* aWords[], i8 iWords);
    bool bDanalyzeTable(char* aWords[], i8 iWords);
    bool bDistExportTable(char* aWords[], i8 iWords);
    bool bLoadTable(char* aWords[], i8 iWords);
    bool bLoadTableStream(char* aWords[], i8 iWords);
    bool bSelectTable(char* aWords[], i8 iWords);
    bool bGetTable(char* aWords[], i8 iWords);
    bool bInUseTable(char* aWords[], i8 iWords);
    bool bRetTable(char* aWords[], i8 iWords);
    bool bNodeStatus(char* aWords[], i8 iWords);
    bool bPoolSpace(char* aWords[], i8 iWords);
    bool bSizeTable(char* aWords[], i8 iWords);
    bool bTruncateTable(char* aWords[], i8 iWords);
    
    bool bInsertRow(char* pRow);
    bool bInsertBuffer(void);
    bool bInsertBuffer(RowBuffer* pRowBuffer, i8 iPartition);
    
    bool bPushDic(char* aWords[]);
    bool bGetDicPath(char* aWords[]);
    bool bGetDicContents(char* aWords[]);
    bool bGetDicContentsG(char* aWords[]);

    bool bGetMonitor(char* aWords[]);
    bool bGetFreeSpace(char* aWords[]);
    bool bGetInUseSpace(char* aWords[]);
    bool bGetTotalSpace(char* aWords[]);
    bool bGetVersion(char* aWords[]);
    bool bSeitfCount(char* aWords[]);
    
    bool bObject(char* aWords[]);
    bool bPing(char* aWords[]);
    bool bPrintRowBuffer(char* aWords[]);
    
    bool bX(char* aWords[]);
    bool bPrint(char* aWords[]);
    
    //--------------------------------------------------------------------------
    bool bAppendRequest(StrVal& sRequest);
    
    eEC  eGetResponse(void);
    bool bPopResponse(eEC& eResult, StrVal& sResponse);
    
    //--------------------------------------------------------------------------
    void         vClearLastError(void);
    eEC          eGetLastError(void);
    const char*  pGetLastError(void);
    void         vSetLastError(eEC eInterfaceError);
    
    Footer* pFooter(void);
  };
  
  SEitf* _pSEitf(void* aAddr);
  SEitf* _pSEitf(i8 iAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
