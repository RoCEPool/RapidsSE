/*============================================================================*/
/* SePrompt.hpp -- se> prompt                                                 */
/*============================================================================*/
#pragma once

#include "Interface.h"
#include "SeHistory.hpp"
#include "Error.hpp"

using namespace CorrectnessAlgebra;

class SePrompt : public Object
{
private:
  i8         m_iPartition; // which partition
  SEitf*     m_pSEitf;     // interface into rapidsse
  bool       m_bContinue;  // continue processing commands?
  bool       m_bEcho;      // echo response?
  s32        m_sPrompt;    // prompt
  s48K       m_sResponse;  // response from seitf
  SeHistory  m_xHistory;   // command history
  TokenVec   m_xTokenVec;  // token vector
  TokenVec   m_xTokenVec2; // another token vector
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

private:
  void vGetCommand(void);
  void vGetCommandArgument(void);
  void vGetCommandInput(void);
  
  void vMetaFormat(void);
  
  //----------------------------------------------------------------------------
  bool bProcessCommand(bool bPostProcess = true);
  
  void vSetEcho(void);
  void vClearEcho(void);
  
  bool bRunFile(void);
  
  bool bExecuteRequest(void);
  bool bTestFile(Stz* aPath);
  void vPushFile(Stz* aPath);

  bool bDocol(void);
  
  bool bIndexReturnTable(Stz* aTableName, Stz* aIndexName);
  void vIterateBuffers(void);
  
  bool bPartitionTable(void);
  bool bReturnTable(void);
  
  //----------------------------------------------------------------------------
  void vClipTokens(i8 i_FromToken);
  void vCollectStatement(void);
  bool bTokenCheck(i8 nTokens);
  bool bCmdIs(Stz* aArg);
  bool bOneIs(Stz* aArg);
  
public:
  bool bCmd_AnalyzeTable(void);
  bool bCmd_BeginInsert(void);
  bool bCmd_Bye(void);
  bool bCmd_ChangeDir(void);
  bool bCmd_CreateCatalog(void);
  bool bCmd_CreateIndex(void);
  bool bCmd_CreateSchema(void);
  bool bCmd_CreateTable(void);
  bool bCmd_CreateTenant(void);
  bool bCmd_CreateUnique(void);
  bool bCmd_DanalyzeTable(void);
  bool bCmd_Docol(void);
  bool bCmd_DropTenant(void);
  bool bCmd_DropCatalog(void);
  bool bCmd_DropIndex(void);
  bool bCmd_DropSchema(void);
  bool bCmd_DropTable(void);
  bool bCmd_EndInsert(void);
  bool bCmd_GetFirstRow(void);
  bool bCmd_Help(void);
  bool bCmd_History(void);
  bool bCmd_InsertRow(void);
  bool bCmd_LoadTable(void);
  bool bCmd_ListDir(void);
  bool bCmd_GetNextRow(void);
  bool bCmd_PrintDir(void);
  bool bCmd_RecallCommand(void);
  bool bCmd_Sleep(void);
  bool bCmd_UseSchema(void);
  
  Footer* aFooter(void);
  
private:
  bool bAddTableColumns(i8& i_Token, i8 iNumTokens);
  bool bAddPartitionColumn(Stz* pTable, Stz* pColumn);
  i8   iCopyTokens(TokenVec& xFromTokenVec, TokenVec& xToTokenVec);
  bool bDropReturnTable(Stz* aTableName);
  bool bEndMakeTable(void);
  bool bMakeTable(void);
  bool bPartitionTable(i8 i_Token, i8 iNumTokens);
  bool bPartitionTable(Stz* pColumnName);
  bool bRemoveTable(Stz* pTableName);
  bool bValidateCreateIndex(i8 iNumTokens);
  bool bValidateCreateTable(i8 iNumTokens);  
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
