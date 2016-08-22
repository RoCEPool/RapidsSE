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
    i8         m_iPid;             // process which created this seitf
    Dictionary m_xDictionary;      // private dictionary
    DictionaryStack m_xDicStack;   // dictionary stack

    Queue*     m_pRequestQueue;    // request queue
    Queue*     m_pResponseQueue;   // reponse queue
    
    s8K         m_sResponse;        // response
    
    i8          m_iInput;
    InputStream m_xInputStream[C_INPUT_ARRAY]; // input stream handler
    s256        m_sPrompt;         // prompt
    
    char        m_wHistory[C_4KB];
    File        m_xHistoryFile;
    i8          m_iCommand;
    bool        m_bSkipHistory;
    
    
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
    void   vPrompt2(void);
    void   vParse(s& sRequest, char* pWords[]);
    void   vExecute(char* pWords[]);
    
    void   vIterateRows(void);
    
    bool   bTestFile(char* pWord);
    void   vPushFile(char* pWord);
    void   vPopFile(void);
    
    InputStream* pInput(void);
    
    //--------------------------------------------------------------------------

    void   vHistoryAppend(char* pWords[]);
    void   vHistoryClose(void);
    bool   bHistoryGet(i8 iCommand, char* pWords[]);
    void   vHistoryOpen(void);
    void   vHistoryPrint(void);
    bool   bHistoryRead(char* wCommand);
    bool   bHistoryRecall(char* pWords[]);
    void   vHistoryStart(void);
    void   vHistoryEnd(void);
    
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
    bool bPrintHistory(char* pWords[]);
    
    bool bPing(char* pWords[]);
    
    bool bDo(char* pWords[]);
    bool bTpcH(char* pWords[]);
    bool bX(char* pWords[]);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/