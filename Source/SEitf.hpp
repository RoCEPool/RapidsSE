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
    
    bool bDecode(TokenSet& xTokenSet, i8 nTokens);
    
    bool bConnect(TokenSet& xTokenSet);
    bool bConnectCluster(TokenSet& xTokenSet);
    bool bDisconnect(TokenSet& xTokenSet);
    
    bool bRemoveFolder(TokenSet& xTokenSet, i8 nTokens);

    bool bCreateTenant(TokenSet& xTokenSet, i8 nTokens);
    bool bListTenants(TokenSet& xTokenSet, i8 nTokens);
    bool bDropTenant(TokenSet& xTokenSet, i8 nTokens);

    bool bCreateCatalog(TokenSet& xTokenSet, i8 nTokens);
    bool bListCatalogs(TokenSet& xTokenSet, i8 nTokens);
    bool bDropCatalog(TokenSet& xTokenSet, i8 nTokens);
    
    bool bCreateSchema(TokenSet& xTokenSet, i8 nTokens);
    bool bListSchemas(TokenSet& xTokenSet, i8 nTokens);
    bool bDropSchema(TokenSet& xTokenSet, i8 nTokens);
    bool bUseSchema(TokenSet& xTokenSet, i8 nTokens);
    
    bool bCreateIndex(TokenSet& xTokenSet, i8 nTokens);
    bool bDropIndex(TokenSet& xTokenSet, i8 nTokens);
    bool bDumpIndex(TokenSet& xTokenSet, i8 nTokens);
    bool bEndCreateIndex(TokenSet& xTokenSet, i8 nTokens);
    bool bListIndices(TokenSet& xTokenSet, i8 nTokens);
    bool bIndexSetUnique(TokenSet& xTokenSet, i8 nTokens);
  
    bool bCreateTable(TokenSet& xTokenSet, i8 nTokens);
    bool bEndCreateTable(TokenSet& xTokenSet, i8 nTokens);
  
    bool bListTables(TokenSet& xTokenSet, i8 nTokens);
    bool bDisplayOff(TokenSet& xTokenSet, i8 nTokens);
    bool bDisplayOn(TokenSet& xTokenSet, i8 nTokens);
    bool bDocol(TokenSet& xTokenSet, i8 nTokens);
    bool bDropTable(TokenSet& xTokenSet, i8 nTokens);
    bool bDumpTable(TokenSet& xTokenSet, i8 nTokens);
   
    bool bAddColumn(TokenSet& xTokenSet, i8 nTokens);
    bool bAddIndexColumn(TokenSet& xTokenSet, i8 nTokens);
    bool bListColumns(TokenSet& xTokenSet, i8 nTokens);
    bool bListIndexColumns(TokenSet& xTokenSet, i8 nTokens);
    
    bool bAnalyzeIndex(TokenSet& xTokenSet, i8 nTokens);
    bool bDanalyzeIndex(TokenSet& xTokenSet, i8 nTokens);
    bool bKeySelectIndex(TokenSet& xTokenSet, i8 nTokens);
    bool bLowerSelectIndex(TokenSet& xTokenSet, i8 nTokens);
    bool bRangeSelectIndex(TokenSet& xTokenSet, i8 nTokens);
    bool bSizeIndex(TokenSet& xTokenSet, i8 nTokens);
    bool bTruncateIndex(TokenSet& xTokenSet, i8 nTokens);
    bool bUpperSelectIndex(TokenSet& xTokenSet, i8 nTokens);

    bool bAnalyzeTable(TokenSet& xTokenSet, i8 nTokens);
    bool bCopyTable(TokenSet& xTokenSet, i8 nTokens);
    bool bExportTable(TokenSet& xTokenSet, i8 nTokens);
    bool bDanalyzeTable(TokenSet& xTokenSet, i8 nTokens);
    bool bDistExportTable(TokenSet& xTokenSet, i8 nTokens);
    bool bLoadTable(TokenSet& xTokenSet, i8 nTokens);
    bool bLoadTableStream(TokenSet& xTokenSet, i8 nTokens);
    bool bLowerSelectTable(TokenSet& xTokenSet, i8 nTokens);
    bool bSelectTable(TokenSet& xTokenSet, i8 nTokens);
    bool bRangeSelectTable(TokenSet& xTokenSet, i8 nTokens);
    bool bGetTable(TokenSet& xTokenSet, i8 nTokens);
    bool bInUseTable(TokenSet& xTokenSet, i8 nTokens);
    bool bRetTable(TokenSet& xTokenSet, i8 nTokens);
    bool bNodeStatus(TokenSet& xTokenSet, i8 nTokens);
    bool bPoolSpace(TokenSet& xTokenSet, i8 nTokens);
    bool bSizeTable(TokenSet& xTokenSet, i8 nTokens);
    bool bSizeTables(TokenSet& xTokenSet, i8 nTokens);
    bool bTruncateTable(TokenSet& xTokenSet, i8 nTokens);
    
    bool bGetTask(TokenSet& xTokenSet, i8 nTokens);
    bool bRetTask(TokenSet& xTokenSet, i8 nTokens);
  
    bool bInsertRow(Str* pRow);
    bool bInsertBuffer(void);
    bool bInsertBuffer(RowBuffer* pRowBuffer, i8 iPartition);
    
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
