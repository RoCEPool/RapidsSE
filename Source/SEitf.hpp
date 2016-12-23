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
    i8     m_iPartitions;      // number of partitions in each node
    i8     m_iPartition;       // which partition this seitf connects to
    
    Dic      m_xDic;           // private dictionary
    DicStack m_xDicStack;      // dictionary stack
    
    Queue* m_pRequestQueue;    // request queue
    Queue* m_pResponseQueue;   // response queue
    
    eEC    m_eErrorCode;       //
    s256   m_sErrorMsg;
    
    Table* m_pCurrentTable;
    s8K    m_sResponse;        // response
    char*  m_pRowPtr;
    
    TxRef* m_pTxRef;
    i8     m_iCurrentRequestId;
    i8     m_iNextRequestId;
    
    RowBuffer* m_pRowBuffer[C_TABLE_PARTITIONS]; // working row buffers
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    SEitf(i8 iPartition);
    ~SEitf(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
    DicStack* pDicStack(void);
    void   vPushDic(Dic* pDic);
    void   vPopDic(void);
    Word*  pFindWord(s& sName);
    Word*  pFindWordInCurrent(s& sName);
    
    void   vCurrentTable(Table* pTable);
    Table* pCurrentTable(void);
    
    void   vDump(void);
    
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
    
    bool bDecode(char* pWords[], i8 iWords);
    
    bool bConnect(char* pWords[]);
    bool bDisconnect(char* pWords[]);
    
    bool bDropDictionary(char* pWords[], i8 iWords);

    bool bCreateTenant(char* pWords[], i8 iWords);
    bool bListTenants(char* pWords[], i8 iWords);
    bool bDropTenant(char* pWords[], i8 iWords);

    bool bCreateCatalog(char* pWords[], i8 iWords);
    bool bListCatalogs(char* pWords[], i8 iWords);
    bool bDropCatalog(char* pWords[], i8 iWords);
    
    bool bCreateSchema(char* pWords[], i8 iWords);
    bool bListSchemas(char* pWords[], i8 iWords);
    bool bDropSchema(char* pWords[], i8 iWords);
    bool bUseSchema(char* pWords[], i8 iWords);
    
    bool bCreateTable(char* pWords[], i8 iWords);
    
    bool bListTables(char* pWords[], i8 iWords);
    bool bDocol(char* pWords[]);
    bool bDropTable(char* pWords[], i8 iWords);
    bool bDumpTable(char* pWords[], i8 iWords);
   
    bool bAddColumn(char* pWords[], i8 iWords);
    bool bListColumns(char* pWords[], i8 iWords);
    
    bool bCopyTable(char* pWords[], i8 iWords);
    bool bExportTable(char* pWords[], i8 iWords);
    bool bLoadTable(char* pWords[], i8 iWords);
    bool bLoadTableStream(char* pWords[], i8 iWords);
    bool bSelectTable(char* pWords[], i8 iWords);
    bool bGetTable(char* pWords[], i8 iWords);
    bool bInUseTable(char* pWords[], i8 iWords);
    bool bRetTable(char* pWords[], i8 iWords);
    bool bSizeTable(char* pWords[], i8 iWords);
    bool bTruncateTable(char* pWords[], i8 iWords);
    
    bool bInsertRow(char* pRow);
    bool bInsertBuffer(void);
    bool bInsertBuffer(RowBuffer* pRowBuffer, i8 iPartition);
    
    bool bPushDic(char* pWords[]);
    bool bGetDicPath(char* pWords[]);
    bool bGetDicContents(char* pWords[]);
    bool bGetDicContentsG(char* pWords[]);

    bool bGetMonitor(char* pWords[]);
    bool bGetFreeSpace(char* pWords[]);
    bool bGetInUseSpace(char* pWords[]);
    bool bGetTotalSpace(char* pWords[]);
    bool bGetVersion(char* pWords[]);
    bool bSeitfCount(char* pWords[]);
    
    bool bPing(char* pWords[]);
    bool bPrintRowBuffer(char* pWords[]);
    
    bool bX(char* pWords[]);
    void vPrint(char* pWords[]);
    
    //--------------------------------------------------------------------------
    bool bAppendRequest(s& sRequest);
    
    eEC  eGetResponse(void);
    bool bPopResponse(eEC& eResult, s& sResponse);
    
    //--------------------------------------------------------------------------
    void         vClearLastError(void);
    eEC          eGetLastError(void);
    const char*  pGetLastError(void);
    void         vSetLastError(eEC eInterfaceError);
    
    Footer* pFooter(void);
  };
  
  SEitf* _pSEitf(void* pAddr);
  SEitf* _pSEitf(i8 iAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
