/*============================================================================*/
/* SePrompt.hpp -- se> prompt                                                 */
/*============================================================================*/
#pragma once

#include "Interface.h"
#include "SeHistory.hpp"
#include "Error.hpp"

using namespace CorrectnessAlgebra;

class SePrompt : public Buffer
{
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
  
  TokenVec   m_xTokenVec;
  TokenVec   m_xTokenVec2;
  
  Tokeniser* m_pTokeniser; // input stream tokeniser
  
  // must be last
  Footer m_xFooter;
  
public:
  void* operator new(size_t);
  void  operator delete(void*);

  SePrompt(void);
  ~SePrompt(void);
  
  static void vDump(eCN eClass, Object* pObject, Stz* aTab);
  
  void vLoop(void);
  bool bExecuteString(StrVal& sCommand, eEC& eErrorCode);
  bool bExecuteString(StrVal& sCommand, eEC& eErrorCode, StrVal& sResponse);
  bool bExecuteString(StrVal& sCommand, eEC& eErrorCode, Stz** ppRowPtr);

private:
  void vGetCommand(void);
  void vGetCommandArgument(void);
  void vGetCommandInput(void);
  
  void vMetaFormat(void);
  
  //----------------------------------------------------------------------------
  bool bProcessCommand(bool bPostProcess = true);
  
  void vSetEcho(void);
  void vClearEcho(void);
  void vSetExpect(void);
  void vClearExpect(void);
  
  bool bBye(void);
  bool bHistory(void);
  bool bRunFile(void);
  bool bRecallCommand(void);
  
  bool bExecuteRequest(void);
  bool bGetFirstRow(void);
  bool bGetNextRow(void);
  void vIterateRows(void);
  bool bHelp(void);
  bool bTestFile(Stz* aPath);
  void vPushFile(Stz* aPath);
  
  bool bBeginInsert(void);
  bool bChangeWorkingDir(void);
  
  bool bCreateCatalog(void);
  bool bCreateIndex(bool bUnique);
  bool bCreateIndex(void);
  bool bCreateSchema(void);
  bool bCreateTable(void);
  bool bCreateTenant(void);
  
  bool bDocol(void);
  
  bool bConnectCluster(void);
  
  bool bDropCatalog(void);
  bool bDropIndex(void);
  bool bDropSchema(void);
  bool bDropTable(void);
  bool bIndexReturnTable(Stz* aTableName, Stz* aIndexName);
  bool bDropTenant(void);
  
  bool bEndInsert(void);
  bool bInsertRow(void);
  bool bAnalyzeTable(void);
  bool bDanalyzeTable(void);
  bool bListDir(void);
  bool bNodeStatus(void);
  bool bObject(void);
  bool bPoolSpace(void);
  bool bPartitionTable(void);
  bool bPrintWorkingDir(void);
  bool bReturnTable(void);

  bool bUseSchema(void);
  
  bool bFetchRows(void);
  
  bool bSleep(void);
  
  //----------------------------------------------------------------------------
  void vClipTokens(i8 i_FromToken);
  void vCollectStatement(void);
  bool bTokenCheck(i8 nTokens);
  bool bCmdIs(Stz* aArg);
  bool bOneIs(Stz* aArg);
  
  Footer* aFooter(void);
  
private:
  bool bAddTableColumns(i8& i_Token, i8 iNumTokens);
  bool bAddPartitionColumn(Stz* pTable, Stz* pColumn);
  bool bDropReturnTable(Stz* aTableName);
  bool bEndMakeTable(void);
  bool bMakeTable(void);
  bool bPartitionTable(i8 i_Token, i8 iNumTokens);
  bool bPartitionTable(Stz* pColumnName);
  bool bRemoveTable(Stz* pTableName);
  
};

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
