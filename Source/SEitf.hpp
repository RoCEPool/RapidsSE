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
    
    bool bDecode(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    
    bool bConnect(TokenAry* pTokenAry, Str* aWords[]);
    bool bConnectCluster(TokenAry* pTokenAry, Str* aWords[]);
    bool bDisconnect(TokenAry* pTokenAry, Str* aWords[]);
    
    bool bRemoveFolder(TokenAry* pTokenAry, Str* aWords[], i8 iWords);

    bool bCreateTenant(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bListTenants(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bDropTenant(TokenAry* pTokenAry, Str* aWords[], i8 iWords);

    bool bCreateCatalog(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bListCatalogs(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bDropCatalog(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    
    bool bCreateSchema(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bListSchemas(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bDropSchema(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bUseSchema(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    
    bool bCreateIndex(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bDropIndex(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bDumpIndex(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bEndCreateIndex(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bListIndices(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bIndexSetUnique(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
  
    bool bCreateTable(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bEndCreateTable(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
  
    bool bListTables(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bDisplayOff(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bDisplayOn(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bDocol(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bDropTable(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bDumpTable(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
   
    bool bAddColumn(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bAddIndexColumn(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bListColumns(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bListIndexColumns(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    
    bool bAnalyzeIndex(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bDanalyzeIndex(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bKeySelectIndex(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bLowerSelectIndex(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bRangeSelectIndex(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bSizeIndex(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bTruncateIndex(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bUpperSelectIndex(TokenAry* pTokenAry, Str* aWords[], i8 iWords);

    bool bAnalyzeTable(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bCopyTable(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bExportTable(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bDanalyzeTable(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bDistExportTable(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bLoadTable(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bLoadTableStream(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bLowerSelectTable(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bSelectTable(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bRangeSelectTable(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bGetTable(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bInUseTable(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bRetTable(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bNodeStatus(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bPoolSpace(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bSizeTable(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bSizeTables(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bTruncateTable(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    
    bool bGetTask(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
    bool bRetTask(TokenAry* pTokenAry, Str* aWords[], i8 iWords);
  
    bool bInsertRow(Str* pRow);
    bool bInsertBuffer(void);
    bool bInsertBuffer(RowBuffer* pRowBuffer, i8 iPartition);
    
    bool bPushDic(TokenAry* pTokenAry, Str* aWords[]);
    bool bGetDicPath(TokenAry* pTokenAry, Str* aWords[]);
    bool bGetDicContents(TokenAry* pTokenAry, Str* aWords[]);
    bool bGetDicContentsG(TokenAry* pTokenAry, Str* aWords[]);

    bool bGetMonitor(TokenAry* pTokenAry, Str* aWords[]);
    bool bGetFreeSpace(TokenAry* pTokenAry, Str* aWords[]);
    bool bGetInUseSpace(TokenAry* pTokenAry, Str* aWords[]);
    bool bGetTotalSpace(TokenAry* pTokenAry, Str* aWords[]);
    bool bGetVersion(TokenAry* pTokenAry, Str* aWords[]);
    bool bSeitfCount(TokenAry* pTokenAry, Str* aWords[]);
    
    bool bObject(TokenAry* pTokenAry, Str* aWords[]);
    bool bPing(TokenAry* pTokenAry, Str* aWords[]);
    bool bPrintRowBuffer(TokenAry* pTokenAry, Str* aWords[]);
    
    bool bX(TokenAry* pTokenAry, Str* aWords[]);
    bool bPrint(TokenAry* pTokenAry, Str* aWords[]);
    
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
