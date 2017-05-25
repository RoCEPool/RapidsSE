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
    
    bool bDecode(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    
    bool bConnect(TokenAry& xTokenAry, Str* aWords[]);
    bool bConnectCluster(TokenAry& xTokenAry, Str* aWords[]);
    bool bDisconnect(TokenAry& xTokenAry, Str* aWords[]);
    
    bool bRemoveFolder(TokenAry& xTokenAry, Str* aWords[], i8 iWords);

    bool bCreateTenant(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bListTenants(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bDropTenant(TokenAry& xTokenAry, Str* aWords[], i8 iWords);

    bool bCreateCatalog(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bListCatalogs(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bDropCatalog(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    
    bool bCreateSchema(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bListSchemas(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bDropSchema(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bUseSchema(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    
    bool bCreateIndex(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bDropIndex(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bDumpIndex(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bEndCreateIndex(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bListIndices(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bIndexSetUnique(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
  
    bool bCreateTable(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bEndCreateTable(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
  
    bool bListTables(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bDisplayOff(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bDisplayOn(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bDocol(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bDropTable(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bDumpTable(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
   
    bool bAddColumn(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bAddIndexColumn(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bListColumns(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bListIndexColumns(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    
    bool bAnalyzeIndex(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bDanalyzeIndex(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bKeySelectIndex(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bLowerSelectIndex(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bRangeSelectIndex(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bSizeIndex(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bTruncateIndex(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bUpperSelectIndex(TokenAry& xTokenAry, Str* aWords[], i8 iWords);

    bool bAnalyzeTable(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bCopyTable(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bExportTable(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bDanalyzeTable(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bDistExportTable(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bLoadTable(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bLoadTableStream(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bLowerSelectTable(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bSelectTable(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bRangeSelectTable(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bGetTable(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bInUseTable(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bRetTable(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bNodeStatus(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bPoolSpace(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bSizeTable(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bSizeTables(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bTruncateTable(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    
    bool bGetTask(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
    bool bRetTask(TokenAry& xTokenAry, Str* aWords[], i8 iWords);
  
    bool bInsertRow(Str* pRow);
    bool bInsertBuffer(void);
    bool bInsertBuffer(RowBuffer* pRowBuffer, i8 iPartition);
    
    bool bPushDic(TokenAry& xTokenAry, Str* aWords[]);
    bool bGetDicPath(TokenAry& xTokenAry, Str* aWords[]);
    bool bGetDicContents(TokenAry& xTokenAry, Str* aWords[]);
    bool bGetDicContentsG(TokenAry& xTokenAry, Str* aWords[]);

    bool bGetMonitor(TokenAry& xTokenAry, Str* aWords[]);
    bool bGetFreeSpace(TokenAry& xTokenAry, Str* aWords[]);
    bool bGetInUseSpace(TokenAry& xTokenAry, Str* aWords[]);
    bool bGetTotalSpace(TokenAry& xTokenAry, Str* aWords[]);
    bool bGetVersion(TokenAry& xTokenAry, Str* aWords[]);
    bool bSeitfCount(TokenAry& xTokenAry, Str* aWords[]);
    
    bool bObject(TokenAry& xTokenAry, Str* aWords[]);
    bool bPing(TokenAry& xTokenAry, Str* aWords[]);
    bool bPrintRowBuffer(TokenAry& xTokenAry, Str* aWords[]);
    
    bool bX(TokenAry& xTokenAry, Str* aWords[]);
    bool bPrint(TokenAry& xTokenAry, Str* aWords[]);
    
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
