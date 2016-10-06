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
    
    Dictionary m_xDictionary;    // private dictionary
    DictionaryStack m_xDicStack; // dictionary stack
    
    Queue* m_pRequestQueue;    // request queue
    Queue* m_pResponseQueue;   // reponse queue
    
    eEC    m_eErrorCode;       //
    s256   m_sErrorMsg;
    
    s8K    m_sResponse;        // response
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    SEitf(i8 iPartition);
    ~SEitf(void);
    
    static void vDump(eCN eClass, Object* pObject, const char* pTab);
    
    DictionaryStack* pDicStack(void);
    void   vPushDic(Dictionary* pDictionary);
    void   vPopDic(void);
    Word*  pFindWord(s& sName);
    Word*  pFindWordInCurrent(s& sName);
    
    void   vDump(void);
    
    void   vInit(void);
    i8     iPartition(void);
    void   vRequestQueue(Queue* pRequestQueue);
    Queue* pRequestQueue(void);
    
    void   vResponseQueue(Queue* pResponseQueue);
    Queue* pResponseQueue(void);

    //--------------------------------------------------------------------------
    
    bool bDecode(char* pWords[]);
    
    bool bConnect(char* pWords[]);
    bool bDisconnect(char* pWords[]);
    
    bool bDropDictionary(char* pWords[]);

    bool bCreateTenant(char* pWords[]);
    bool bListTenants(char* pWords[]);
    bool bDropTenant(char* pWords[]);

    bool bCreateCatalog(char* pWords[]);
    bool bListCatalogs(char* pWords[]);
    bool bDropCatalog(char* pWords[]);
    
    bool bCreateSchema(char* pWords[]);
    bool bListSchemas(char* pWords[]);
    bool bDropSchema(char* pWords[]);
    
    bool bCreateTable(char* pWords[]);
    bool bListTables(char* pWords[]);
    bool bDocol(char* pWords[]);
    bool bDropTable(char* pWords[]);
    bool bDumpTable(char* pWords[]);
   
    bool bAddColumn(char* pWords[]);
    bool bListColumns(char* pWords[]);
    
    bool bCopyTable(char* pWords[]);
    bool bExportTable(char* pWords[]);
    bool bLoadTable(char* pWords[]);
    bool bSelectTable(char* pWords[]);
    bool bAddressTable(char* pWords[]);
    bool bSizeTable(char* pWords[]);
    bool bTruncateTable(char* pWords[]);
    
    bool bPushDic(char* pWords[]);
    bool bGetDicPath(char* pWords[]);
    bool bGetDicContents(char* pWords[]);

    bool bGetMonitor(char* pWords[]);
    bool bGetFreeSpace(char* pWords[]);
    bool bGetInUseSpace(char* pWords[]);
    bool bGetTotalSpace(char* pWords[]);
    
    bool bPing(char* pWords[]);
    
    bool bX(char* pWords[]);
    bool bPrint(char* pWords[]);
    
    //--------------------------------------------------------------------------
    void vAppendRequest(s& sRequest);
    
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