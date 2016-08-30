/*============================================================================*/
/* SEitf.hpp - se interface                                                   */
/*============================================================================*/

#pragma once

#include "Interface.h"

namespace Interface
{
  enum eIE// Interface Error
  {
    /* 0 */ IE_NONE=0,
    /* 1 */ IE_COMMAND_FAILED,
    /* 2 */ IE_INVALID_COMMAND,
    /* 3 */ IE_INVALID_PARAMETER,
    /* 4 */ IE_ENUMS
  };
  
  static const char *pIE[]
  {
    /* 0 */ "NONE",
    /* 1 */ "COMMAND FAILED",
    /* 2 */ "INVALID COMMAND",
    /* 3 */ "INVALID PARAMETER",
    /* 4 */ "ENUMS",
  };
  
  class SEitf : public Object,
                       public ListNode_t<SEitf>
  {
    static const i8 C_INPUT_ARRAY = 4;
    
  public:
    i8         m_iPid;             // process which created this seitf
    i8         m_iPartitions;      // number of partitions in each node
    i8         m_iPartition;       // which partition this seitf connects to
    
    Dictionary m_xDictionary;      // private dictionary
    DictionaryStack m_xDicStack;   // dictionary stack

    Queue*     m_pRequestQueue;    // request queue
    Queue*     m_pResponseQueue;   // reponse queue
    s8K        m_sResponse;        // response
    
    eIE         m_eInterfaceError; // error code
    s256        m_sInterfaceError; // error string
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    SEitf(i8 iPartition);
    ~SEitf(void);
    
    DictionaryStack* pDicStack(void);
    void   vPushDic(Dictionary* pDictionary);
    void   vPopDic(void);
    Word*  pFindWord(s& sName);
    
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
    bool bDumpTable(char* pWords[]);
    bool bDropTable(char* pWords[]);

    bool bAddColumn(char* pWords[]);
    bool bListColumns(char* pWords[]);
    
    bool bCopyTable(char* pWords[]);
    bool bExportTable(char* pWords[]);
    bool bLoadTable(char* pWords[]);
    bool bSelectTable(char* pWords[]);
    bool bAddressTable(char* pWords[]);
    bool bSizeTable(char* pWords[]);
    bool bTruncateTable(char* pWords[]);
    
    bool bPopDic(char* pWords[]);
    bool bPushDic(char* pWords[]);
    bool bGetDicPath(char* pWords[]);
    bool bSetDicPath(char* pWords[]);
    bool bGetDicContents(char* pWords[]);

    bool bGetMonitor(char* pWords[]);
    
    bool bPing(char* pWords[]);
    
    bool bX(char* pWords[]);
    bool bPrint(char* pWords[]);
    
    //--------------------------------------------------------------------------
    void vAppendRequest(s& sRequest);
    
    i8   iGetResponse(void);
    bool bPopResponse(i8& iResult, s& sResponse);
    
    //--------------------------------------------------------------------------
    void         vClearLastError(void);
    const char*  pGetLastError(void);
    void         vSetLastError(eIE eInterfaceError);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/