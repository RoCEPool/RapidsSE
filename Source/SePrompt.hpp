/*============================================================================*/
/* SePrompt.hpp -- se> prompt                                                 */
/*============================================================================*/

#pragma once

#include "Api.hpp"

class SePrompt
{
private:
  static const i8 C_INPUT_ARRAY = 4;

  i8     m_iPartition; // which partition
  SEitf* m_pSEitf;     // interface into rapidsse
  s256   m_sPrompt;    // the prompt
  
  s256        m_sInputLine;
  i8          m_iInput;
  InputStream m_xInputStream[C_INPUT_ARRAY]; // input stream handler
  
  s8K         m_sResponse;       // response
  
  char        m_wHistory[C_4KB];
  File        m_xHistoryFile;
  i8          m_iCommand;
  bool        m_bSkipHistory;
  
  eIE         m_eInterfaceError;
  s256        m_sInterfaceError;
  
public:
  SePrompt(i8 iPartition);
  ~SePrompt(void);
  
  void vPrompt(void);
  
  InputStream* pInput(void);
  
  //--------------------------------------------------------------------------
  void   vParse(s& sRequest, char* pWords[]);
  void   vExecute(char* pWords[]);
  void   vExecute2(char* pWords[]);
  
  void   vIterateRows(void);
  
  bool   bTestFile(char* pWord);
  void   vPushFile(char* pWord);
  void   vPopFile(void);
  
  
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
  i8     iGetResponse(void);
  bool   bPopResponse(s& sResponse);
  bool   bPopResponse(i8& iResult, s& sResponse);
  
  
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
  bool bPrint(char* pWords[]);
  
  //--------------------------------------------------------------------------
  void         vClearLastError(void);
  const char*  pGetLastError(void);
  void         vSetLastError(eIE eInterfaceError);
  
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/