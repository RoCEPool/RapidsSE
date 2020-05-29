/*============================================================================*/
/* SePromptR35.hpp -- se> prompt                                              */
/*============================================================================*/
#pragma once

#include "Interface.h"
#include "SeHistory.hpp"
#include "Error.hpp"

using namespace CorrectnessAlgebra;


static Stz* C_DATE_FORMAT          = "date_format"_Stz;
static Stz* C_ENCLOSED_BY_LEADING  = "enclosed_by_leading"_Stz;
static Stz* C_ENCLOSED_BY_TRAILING = "enclosed_by_trailing"_Stz;
static Stz* C_ESCAPE_CHAR          = "escape_char"_Stz;
static Stz* C_FIELD_DELIMITER      = "field_delimiter"_Stz;
static Stz* C_RECORD_ENDS          = "record_ends"_Stz;
static Stz* C_RECORD_TERMINATOR    = "record_terminator"_Stz;

static Stz* C_SAME_MACHINE         = "same_machine"_Stz;

class SePromptR35 : public Object
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
  Timer      m_xTimer;     // timer
  
  PosixThread m_xPosixThread; // posix thread structure
  
  // must be last
  Footer m_xFooter;
  
public:
  void* operator new(size_t);
  void  operator delete(void*);

  SePromptR35(void);
  ~SePromptR35(void);
  
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
  bool bCmd_DistParLoadTable(void);
  bool bCmd_DropTenant(void);
  bool bCmd_DropCatalog(void);
  bool bCmd_DropIndex(void);
  bool bCmd_DropSchema(void);
  bool bCmd_DropTable(void);
  bool bCmd_EndInsert(void);
  bool bCmd_Help(void);
  bool bCmd_History(void);
  bool bCmd_InsertRow(void);
  bool bCmd_LoadTable(void);
  bool bCmd_ListDir(void);
  bool bCmd_PrintDir(void);
  bool bCmd_RecallCommand(void);
  bool bCmd_Sleep(void);
  bool bCmd_UseSchema(void);
  bool bCmd_TimerStart(void);
  bool bCmd_TimerEnd(void);
  
  bool bCmd_CreateCatalogR35(void);
  bool bCmd_CreateIndexR35(void);
  bool bCmd_CreateSchemaR35(void);
  bool bCmd_CreateTableR35(void);
  bool bCmd_CreateTenantR35(void);
  
  bool bCmd_DropCatalogR35(void);
  bool bCmd_DropIndexR35(void);
  bool bCmd_DropSchemaR35(void);
  bool bCmd_DropTableR35(void);
  bool bCmd_DropTenantR35(void);
  
  Footer* aFooter(void);
  
private:
  bool bAddTableColumns(i8& i_Token, i8 iNumTokens);
  bool bAddTableColumnsR35(i8& i_Token, i8 iNumTokens);
  bool bAddPartitionColumn(Stz* pTable, Stz* pColumn);
  i8   iCopyTokens(TokenVec& xFromTokenVec, TokenVec& xToTokenVec);
  bool bDropReturnTable(Stz* aTableName);
  bool bEndMakeTable(void);
  bool bMakeTable(void);
  bool bPartitionTable(i8 i_Token, i8 iNumTokens);
  bool bPartitionTableR35(i8 i_Token, i8 iNumTokens);
  bool bPartitionTable(Stz* pColumnName);
  bool bRemoveTable(Stz* pTableName);
  bool bValidateCreateIndex(i8 iNumTokens);
  bool bValidateCreateTable(i8 iNumTokens);
  
  bool bValidateCreateIndexR35(i8 iNumTokens);
  bool bValidateCreateTableR35(i8 iNumTokens);
};

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
