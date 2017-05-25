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
    
    bool bDecode(TokenAry& xTokenAry, i8 nTokens);
    
    bool bConnect(TokenAry& xTokenAry);
    bool bConnectCluster(TokenAry& xTokenAry);
    bool bDisconnect(TokenAry& xTokenAry);
    
    bool bRemoveFolder(TokenAry& xTokenAry, i8 nTokens);

    bool bCreateTenant(TokenAry& xTokenAry, i8 nTokens);
    bool bListTenants(TokenAry& xTokenAry, i8 nTokens);
    bool bDropTenant(TokenAry& xTokenAry, i8 nTokens);

    bool bCreateCatalog(TokenAry& xTokenAry, i8 nTokens);
    bool bListCatalogs(TokenAry& xTokenAry, i8 nTokens);
    bool bDropCatalog(TokenAry& xTokenAry, i8 nTokens);
    
    bool bCreateSchema(TokenAry& xTokenAry, i8 nTokens);
    bool bListSchemas(TokenAry& xTokenAry, i8 nTokens);
    bool bDropSchema(TokenAry& xTokenAry, i8 nTokens);
    bool bUseSchema(TokenAry& xTokenAry, i8 nTokens);
    
    bool bCreateIndex(TokenAry& xTokenAry, i8 nTokens);
    bool bDropIndex(TokenAry& xTokenAry, i8 nTokens);
    bool bDumpIndex(TokenAry& xTokenAry, i8 nTokens);
    bool bEndCreateIndex(TokenAry& xTokenAry, i8 nTokens);
    bool bListIndices(TokenAry& xTokenAry, i8 nTokens);
    bool bIndexSetUnique(TokenAry& xTokenAry, i8 nTokens);
  
    bool bCreateTable(TokenAry& xTokenAry, i8 nTokens);
    bool bEndCreateTable(TokenAry& xTokenAry, i8 nTokens);
  
    bool bListTables(TokenAry& xTokenAry, i8 nTokens);
    bool bDisplayOff(TokenAry& xTokenAry, i8 nTokens);
    bool bDisplayOn(TokenAry& xTokenAry, i8 nTokens);
    bool bDocol(TokenAry& xTokenAry, i8 nTokens);
    bool bDropTable(TokenAry& xTokenAry, i8 nTokens);
    bool bDumpTable(TokenAry& xTokenAry, i8 nTokens);
   
    bool bAddColumn(TokenAry& xTokenAry, i8 nTokens);
    bool bAddIndexColumn(TokenAry& xTokenAry, i8 nTokens);
    bool bListColumns(TokenAry& xTokenAry, i8 nTokens);
    bool bListIndexColumns(TokenAry& xTokenAry, i8 nTokens);
    
    bool bAnalyzeIndex(TokenAry& xTokenAry, i8 nTokens);
    bool bDanalyzeIndex(TokenAry& xTokenAry, i8 nTokens);
    bool bKeySelectIndex(TokenAry& xTokenAry, i8 nTokens);
    bool bLowerSelectIndex(TokenAry& xTokenAry, i8 nTokens);
    bool bRangeSelectIndex(TokenAry& xTokenAry, i8 nTokens);
    bool bSizeIndex(TokenAry& xTokenAry, i8 nTokens);
    bool bTruncateIndex(TokenAry& xTokenAry, i8 nTokens);
    bool bUpperSelectIndex(TokenAry& xTokenAry, i8 nTokens);

    bool bAnalyzeTable(TokenAry& xTokenAry, i8 nTokens);
    bool bCopyTable(TokenAry& xTokenAry, i8 nTokens);
    bool bExportTable(TokenAry& xTokenAry, i8 nTokens);
    bool bDanalyzeTable(TokenAry& xTokenAry, i8 nTokens);
    bool bDistExportTable(TokenAry& xTokenAry, i8 nTokens);
    bool bLoadTable(TokenAry& xTokenAry, i8 nTokens);
    bool bLoadTableStream(TokenAry& xTokenAry, i8 nTokens);
    bool bLowerSelectTable(TokenAry& xTokenAry, i8 nTokens);
    bool bSelectTable(TokenAry& xTokenAry, i8 nTokens);
    bool bRangeSelectTable(TokenAry& xTokenAry, i8 nTokens);
    bool bGetTable(TokenAry& xTokenAry, i8 nTokens);
    bool bInUseTable(TokenAry& xTokenAry, i8 nTokens);
    bool bRetTable(TokenAry& xTokenAry, i8 nTokens);
    bool bNodeStatus(TokenAry& xTokenAry, i8 nTokens);
    bool bPoolSpace(TokenAry& xTokenAry, i8 nTokens);
    bool bSizeTable(TokenAry& xTokenAry, i8 nTokens);
    bool bSizeTables(TokenAry& xTokenAry, i8 nTokens);
    bool bTruncateTable(TokenAry& xTokenAry, i8 nTokens);
    
    bool bGetTask(TokenAry& xTokenAry, i8 nTokens);
    bool bRetTask(TokenAry& xTokenAry, i8 nTokens);
  
    bool bInsertRow(Str* pRow);
    bool bInsertBuffer(void);
    bool bInsertBuffer(RowBuffer* pRowBuffer, i8 iPartition);
    
    bool bPushDic(TokenAry& xTokenAry);
    bool bGetDicPath(TokenAry& xTokenAry);
    bool bGetDicContents(TokenAry& xTokenAry);
    bool bGetDicContentsG(TokenAry& xTokenAry);

    bool bGetMonitor(TokenAry& xTokenAry);
    bool bGetFreeSpace(TokenAry& xTokenAry);
    bool bGetInUseSpace(TokenAry& xTokenAry);
    bool bGetTotalSpace(TokenAry& xTokenAry);
    bool bGetVersion(TokenAry& xTokenAry);
    bool bSeitfCount(TokenAry& xTokenAry);
    
    bool bObject(TokenAry& xTokenAry);
    bool bPing(TokenAry& xTokenAry);
    bool bPrintRowBuffer(TokenAry& xTokenAry);
    
    bool bX(TokenAry& xTokenAry);
    bool bPrint(TokenAry& xTokenAry);
    
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
