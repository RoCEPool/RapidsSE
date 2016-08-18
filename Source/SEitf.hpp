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
  public:
    i8         m_iPid;           // process which created this seitf
    Dictionary m_xDictionary;    // private dictionary
    DictionaryStack m_xDicStack; // dictionary stack

    Queue*     m_pRequestQueue;  // request queue
    Queue*     m_pResponseQueue; // reponse queue
    
    s256       m_sResponse;      // response
    
    // must be last
    Footer m_xFooter;
    
  public:
    void* operator new(size_t);
    void  operator delete(void*);
    
    SEitf(void);
    ~SEitf(void);
    
    DictionaryStack* pDicStack(void);
    void   vPushDic(Dictionary* pDictionary);
    void   vPopDic(void);
    Word*   pFindWord(s& sName);
    
    void   vDump(void);
    void   vInit(void);
    void   vPrompt(void);
    void   vParse(s& sRequest, char* pWords[]);
    void   vExecute(char* pWords[]);
    
    //--------------------------------------------------------------------------
    
    void   vAppendRequest(s& sRequest);
    char*  pGetResponse(void);
    bool   bPopResponse(s& sResponse);
    bool   bPopResponse(i8& iResult, s& sResponse);
    
    void   vRequestQueue(Queue* pRequestQueue);
    Queue* pRequestQueue(void);
    
    void   vResponseQueue(Queue* pResponseQueue);
    Queue* pResponseQueue(void);
    
    //--------------------------------------------------------------------------
    
    void   vDo(const char* aCommand);
    
    //--------------------------------------------------------------------------
    
    bool bDecode(char* pWords[]);
    bool bHelp(char* pWords[]);
    
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
    
    bool bDo(char* pWords[]);
    bool bTpcH(char* pWords[]);
    bool bX(char* pWords[]);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/