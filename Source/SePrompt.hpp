/*============================================================================*/
/* SePrompt.hpp -- se> prompt                                                 */
/*============================================================================*/
#pragma once

#include "Prompt.h"


static Stz* C_DATE_FORMAT          = "date_format"_Stz;
static Stz* C_ENCLOSED_BY_LEADING  = "enclosed_by_leading"_Stz;
static Stz* C_ENCLOSED_BY_TRAILING = "enclosed_by_trailing"_Stz;
static Stz* C_ESCAPE_CHAR          = "escape_char"_Stz;
static Stz* C_FIELD_DELIMITER      = "field_delimiter"_Stz;
static Stz* C_RECORD_ENDS          = "record_ends"_Stz;
static Stz* C_RECORD_TERMINATOR    = "record_terminator"_Stz;
static Stz* C_SAME_MACHINE         = "same_machine"_Stz;

namespace Prompt
{
  enum eCK // command kind
  {
  /*  0 */ CK_NONE=0,
  /*  1 */ CK_BLANK_LINE,
  /*  2 */ CK_COMMAND_FILE,
  /*  3 */ CK_PROMPT_FUNCTION,
  /*  4 */ CK_SE_JOB,
  /*  5 */ CK_ENUMS
  };

  class SePrompt : public Object
  {
  private:
    i8         m_iPartition; // which partition
    SEitf*     m_pSEitf;     // interface into rapidsse
    bool       m_bContinue;  // continue processing commands?
    bool       m_bEcho;      // echo response?
    s48K       m_sResponse;  // response from seitf
    Tokeniser* m_pTokeniser; // input stream tokeniser
    Timer      m_xTimer;     // timer
    eCK        m_eCommand;   // kind of last command
    JobClass*  m_pJobClass;  // job class
    Job*       m_pJob;       // job
  
    CommandProcessor* m_pCommandProcessor;

    PosixThread m_xPosixThread; // posix thread structure
  
    // must be last
    Footer m_xFooter;
  
  public:
    void* operator new(size_t);
    void  operator delete(void*);

    SePrompt(void);
    ~SePrompt(void);
  
    static void vDump(eCN eClass, Object* pObject, Stz* aTab);

    // these are the prompt.xyz task primitives
    bool bPrepareJob(void);
    void vEndJob(void);
    void vExecuteCommand(void);
    void vGetCommand(void);
    JobClass* pJobClass(void);
    void vNewJob(void);
  
    // other
    eCK  eCommand(void);
    void vContinue(bool bContinue);
    Job* pJob(void);
    SEitf* pSEitf(void);
    Timer* pTimer(void);
    TokenVec* pTokenVec(void);
  

  private:
  
    bool bBlankLine(void);
    void vClearEcho(void);
    void vClipTokens(i8 i_FromToken);
    bool bCmdIs(Stz* aArg);
    void vCollectStatement(void);
    bool bDispatchJob(Job* pJob);
    bool bExecuteFile(void);
    bool bExecuteFunction(void);
    bool bFindJobClass(void);
    void vGetCommandArgument(void);
    void vGetCommandInput(void);
    void vMetaFormat(void);
    bool bOneIs(Stz* aArg);
    void vPushFile(Stz* aPath);
    bool bRunFile(void);
    void vSetEcho(void);
    bool bTestFile(Stz* aPath);
    bool bTokenCheck(i8 nTokens);
  
    TokenVec* pTknVec(void);
    TokenVec* pTknVec2(void);
  
    Token* pTkn(i8 i_Token);
    Token* pTkn2(i8 i_Token);

    Footer* aFooter(void);
  
  //----------------------------------------------------------------------------
  // to be removed
  public:
    bool bCmd_AnalyzeTable(void);
    bool bCmd_BeginInsert(void);
    bool bCmd_CreateCatalog(void);
    bool bCmd_CreateIndex(void);
    bool bCmd_CreateSchema(void);
    bool bCmd_CreateTenant(void);
    bool bCmd_CreateUnique(void);
    bool bCmd_DanalyzeTable(void);
    bool bCmd_Docol(void);
    bool bCmd_DistParLoadTable(void);
    bool bCmd_DropTenant(void);
    bool bCmd_DropCatalog(void);
    bool bCmd_DropIndex(void);
    bool bCmd_DropSchema(void);
    bool bCmd_EndInsert(void);
    bool bCmd_Help(void);
    bool bCmd_InsertRow(void);
    bool bCmd_LoadTable(void);
    bool bCmd_PrintDir(void);
    bool bCmd_UseSchema(void);
  
    bool bCmd_CreateCatalogR35(void);
    bool bCmd_CreateIndexR35(void);
    bool bCmd_CreateSchemaR35(void);
    bool bCmd_CreateTenantR35(void);
  
    bool bCmd_DropCatalogR35(void);
    bool bCmd_DropIndexR35(void);
    bool bCmd_DropSchemaR35(void);
    bool bCmd_DropTenantR35(void);
 
  private:
    i8   iCopyTokens(TokenVec* pFromTokenVec, TokenVec* pToTokenVec);
    bool bDropReturnTable(Stz* aTableName);

    bool bRemoveTable(Stz* pTableName);
    bool bValidateCreateIndex(i8 iNumTokens);
  
    bool bValidateCreateIndexR35(i8 iNumTokens);
  
    bool bReturnTable(void);
    bool bIndexReturnTable(Stz* aTableName, Stz* aIndexName);
    void vIterateBuffers(void);
    bool bProcessCommand(bool bPostProcess = true);
    bool bExecuteRequest(void);

  };

  SePrompt* _pSePrompt(void* aAddr);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
