/*============================================================================*/
/* SePrompt.hpp -- se> prompt                                                 */
/*============================================================================*/
#pragma once

#include "Interface.h"
#include "SeHistory.hpp"
#include "Error.hpp"

using namespace Magic;

class SePrompt : public Buffer
{
  static const i8 C_TOKENS      = 1024;
  static const i8 C_TOKEN_SPACE = 256;
  static const i8 C_TOKEN_AREA  = C_TOKEN_SPACE * C_TOKENS;
  
private:
  i8        m_iPartition; // which partition
  SEitf*    m_pSEitf;     // interface into rapidsse
  bool      m_bContinue;  // continue processing commands?
  s32       m_sPrompt;    // prompt
  bool      m_bUseString; // use string

  bool      m_bEcho;      // echo response?
  
  bool      m_bExpect;    // is there an expected response?
  s48K       m_sExpect;    // the expected response;
  
  s48K       m_sResponse;  // response from seitf
  SeHistory m_xHistory;   // command history
  
  Str*       m_aToken[C_TOKENS];
  Str*       m_aToken2[C_TOKENS];
  Tokeniser* m_pTokeniser; // input stream tokeniser

  WspFxd<C_TOKEN_AREA> m_xTokenArea;
  WspFxd<C_TOKEN_AREA> m_xTokenArea2;
  
  // must be last
  Footer m_xFooter;
  
public:
  void* operator new(size_t);
  void  operator delete(void*);

  SePrompt(void);
  ~SePrompt(void);
  
  static void vDump(eCN eClass, Object* pObject, Str* aTab);
  
  void vLoop(void);
  bool bExecuteString(StrVal& sCommand, eEC& eErrorCode);
  bool bExecuteString(StrVal& sCommand, eEC& eErrorCode, StrVal& sResponse);
  bool bExecuteString(StrVal& sCommand, eEC& eErrorCode, Str** ppRowPtr);

private:
  void vGetCommand(void);
  void vGetCommandArgument(void);
  void vGetCommandInput(void);
  
  void vMetaFormat(void);
  bool bProcessCommand(bool bPostProcess = true);  
  
  void vSetEcho(void);
  void vClearEcho(void);
  void vSetExpect(void);
  void vClearExpect(void);
  void vClipTokens(i8 nTokens);
  
  bool bExecuteRequest(void);
  bool bGetFirstRow(void);
  bool bGetNextRow(void);
  void vIterateRows(void);
  void vHelp(void);
  bool bTestFile(Str* aPath);
  void vPushFile(Str* aPath);
  
  bool bBeginInsert(void);
  bool bChangeWorkingDir(void);
  
  bool bCreateCatalog(void);
  bool bCreateIndex(void);
  bool bCreateSchema(void);
  bool bCreateTable(void);
  bool bCreateTenant(void);
  
  void vCollectStatement(void);
  void vStripSemicolon(void);
  bool bDocol(void);
  
  bool bConnectCluster(void);
  
  bool bDropCatalog(void);
  bool bDropIndex(void);
  bool bDropSchema(void);
  bool bDropTable(void);
  bool bDropReturnTable(Str* aTableName);
  bool bIndexReturnTable(Str* aTableName, Str* aIndexName);
  bool bDropTenant(void);
  
  bool bEndInsert(void);
  bool bInsertRow(void);
  bool bAnalyzeTable(void);
  bool bDanalyzeTable(void);
  bool bListDir(void);
  bool bNodeStatus(void);
  bool bObject(void);
  bool bPoolSpace(void);
  bool bPrintWorkingDir(void);
  bool bReturnTable(void);

  bool bUseSchema(void);
  
  bool bSleep(void);
  
  Footer* pFooter(void);
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
