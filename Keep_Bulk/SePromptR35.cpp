/*============================================================================*/
/* SePromptR35.cpp -- se> prompt                                              */
/*============================================================================*/

#include "SePromptR35.hpp"

extern AttachMap*    gpAttachMap;
extern BufferPool*   gpBufferPool;
extern Arguments     gxArguments;
extern pthread_key_t giTLS_ThreadKey;

//
// list of commands which need intercept processing by SePromptR35
//
const Stz* C_CMD_HELP1      = "?"_Stz;
const Stz* C_CMD_HELP2      = "help"_Stz;
const Stz* C_CMD_RECALL     = "!!"_Stz;
const Stz* C_CMD_ANTBL      = "antbl"_Stz;
const Stz* C_CMD_BEGIN      = "begin"_Stz;
const Stz* C_CMD_BYE        = "bye"_Stz;
const Stz* C_CMD_CD         = "cd"_Stz;
const Stz* C_CMD_CREATE     = "create"_Stz;
const Stz* C_CMD_DANTBL     = "dantbl"_Stz;
const Stz* C_CMD_DOCOL      = "docol"_Stz;
const Stz* C_CMD_DISTPARLOAD = "dpld"_Stz;
const Stz* C_CMD_DROP       = "drop"_Stz;
const Stz* C_CMD_END        = "end"_Stz;
const Stz* C_CMD_FIRST      = "first"_Stz;
const Stz* C_CMD_HISTORY    = "history"_Stz;
const Stz* C_CMD_INSERT     = "insert"_Stz;
const Stz* C_CMD_LOAD       = "ld"_Stz;
const Stz* C_CMD_LS         = "ls"_Stz;
const Stz* C_CMD_NEXT       = "next"_Stz;
const Stz* C_CMD_PWD        = "pwd"_Stz;
const Stz* C_CMD_SLEEP      = "sleep"_Stz;
const Stz* C_CMD_USE        = "use"_Stz;
const Stz* C_CMD_TIMERSTART = "timerStart"_Stz;
const Stz* C_CMD_TIMEREND   = "timerEnd"_Stz;

const Stz* C_ONE_CATALOG = "catalog"_Stz;
const Stz* C_ONE_INDEX   = "index"_Stz;
const Stz* C_ONE_INSERT  = "insert"_Stz;
const Stz* C_ONE_ROW     = "row"_Stz;
const Stz* C_ONE_SCHEMA  = "schema"_Stz;
const Stz* C_ONE_TABLE   = "table"_Stz;
const Stz* C_ONE_TENANT  = "tenant"_Stz;
const Stz* C_ONE_UNIQUE  = "unique"_Stz;

const Stz* C_R35_ONE_CATALOG = "R35catalog"_Stz;
const Stz* C_R35_ONE_INDEX   = "R35index"_Stz;
const Stz* C_R35_ONE_INSERT  = "R35insert"_Stz;
const Stz* C_R35_ONE_ROW     = "R35row"_Stz;
const Stz* C_R35_ONE_SCHEMA  = "R35schema"_Stz;
const Stz* C_R35_ONE_TABLE   = "R35table"_Stz;
const Stz* C_R35_ONE_TENANT  = "R35tenant"_Stz;
const Stz* C_R35_ONE_UNIQUE  = "R35unique"_Stz;

struct CmdTable {
  const Stz* aCommand;
  const Stz* aAlternate;
  bool (SePromptR35::*aFunc)(void);
};

CmdTable SePromptR35Commands[] =
{
  { C_CMD_ANTBL,      nullptr,       &SePromptR35::bCmd_AnalyzeTable  },
  { C_CMD_BEGIN,      C_ONE_INSERT,  &SePromptR35::bCmd_BeginInsert   },
  { C_CMD_BYE,        nullptr,       &SePromptR35::bCmd_Bye           },
  { C_CMD_CD,         nullptr,       &SePromptR35::bCmd_ChangeDir     },
  { C_CMD_CREATE,     C_ONE_CATALOG, &SePromptR35::bCmd_CreateCatalog },
  { C_CMD_CREATE,     C_ONE_INDEX,   &SePromptR35::bCmd_CreateIndex   },
  { C_CMD_CREATE,     C_ONE_SCHEMA,  &SePromptR35::bCmd_CreateSchema  },
  { C_CMD_CREATE,     C_ONE_TABLE,   &SePromptR35::bCmd_CreateTable   },
  { C_CMD_CREATE,     C_ONE_TENANT,  &SePromptR35::bCmd_CreateTenant  },
  { C_CMD_CREATE,     C_ONE_UNIQUE,  &SePromptR35::bCmd_CreateUnique  },
  { C_CMD_DANTBL,     nullptr,       &SePromptR35::bCmd_DanalyzeTable },
  { C_CMD_DOCOL,      nullptr,       &SePromptR35::bCmd_Docol         },
  { C_CMD_DISTPARLOAD,nullptr,       &SePromptR35::bCmd_DistParLoadTable},
  { C_CMD_DROP,       C_ONE_CATALOG, &SePromptR35::bCmd_DropCatalog   },
  { C_CMD_DROP,       C_ONE_INDEX,   &SePromptR35::bCmd_DropIndex     },
  { C_CMD_DROP,       C_ONE_SCHEMA,  &SePromptR35::bCmd_DropSchema    },
  { C_CMD_DROP,       C_ONE_TABLE,   &SePromptR35::bCmd_DropTable     },
  { C_CMD_DROP,       C_ONE_TENANT,  &SePromptR35::bCmd_DropTenant    },
  { C_CMD_END,        C_ONE_INSERT,  &SePromptR35::bCmd_EndInsert     },
  { C_CMD_HELP1,      nullptr,       &SePromptR35::bCmd_Help          },
  { C_CMD_HELP2,      nullptr,       &SePromptR35::bCmd_Help          },
  { C_CMD_HISTORY,    nullptr,       &SePromptR35::bCmd_History       },
  { C_CMD_INSERT,     C_ONE_ROW,     &SePromptR35::bCmd_InsertRow     },
  { C_CMD_LOAD,       nullptr,       &SePromptR35::bCmd_LoadTable     },
  { C_CMD_LS,         nullptr,       &SePromptR35::bCmd_ListDir       },
  { C_CMD_PWD,        nullptr,       &SePromptR35::bCmd_PrintDir      },
  { C_CMD_RECALL,     nullptr,       &SePromptR35::bCmd_RecallCommand },
  { C_CMD_SLEEP,      nullptr,       &SePromptR35::bCmd_Sleep         },
  { C_CMD_USE,        nullptr,       &SePromptR35::bCmd_UseSchema     },
  { C_CMD_TIMERSTART, nullptr,       &SePromptR35::bCmd_TimerStart    },
  { C_CMD_TIMEREND,   nullptr,       &SePromptR35::bCmd_TimerEnd      },
  
  { C_CMD_CREATE,     C_R35_ONE_CATALOG, &SePromptR35::bCmd_CreateCatalogR35 },
  { C_CMD_CREATE,     C_R35_ONE_INDEX,   &SePromptR35::bCmd_CreateIndexR35   },
  { C_CMD_CREATE,     C_R35_ONE_SCHEMA,  &SePromptR35::bCmd_CreateSchemaR35  },
  { C_CMD_CREATE,     C_R35_ONE_TABLE,   &SePromptR35::bCmd_CreateTableR35   },
  { C_CMD_CREATE,     C_R35_ONE_TENANT,  &SePromptR35::bCmd_CreateTenantR35  },
  { C_CMD_DROP,       C_R35_ONE_CATALOG, &SePromptR35::bCmd_DropCatalogR35   },
  { C_CMD_DROP,       C_R35_ONE_INDEX,   &SePromptR35::bCmd_DropIndexR35     },
  { C_CMD_DROP,       C_R35_ONE_SCHEMA,  &SePromptR35::bCmd_DropSchemaR35    },
  { C_CMD_DROP,       C_R35_ONE_TABLE,   &SePromptR35::bCmd_DropTableR35     },
  { C_CMD_DROP,       C_R35_ONE_TENANT,  &SePromptR35::bCmd_DropTenantR35    },
  
  { nullptr,          nullptr,       nullptr                              }
};

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* SePromptR35::operator new(size_t iSize)
{
  RASSERT(iSize);
  SePromptR35* pSePromptR35 = (SePromptR35*)Heap::pAllocate(CN_SEPROMPT);
  
  return pPoolCheck<void*>(pSePromptR35);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void SePromptR35::operator delete(void* pSePromptR35Address)
{
  SePromptR35* pSePromptR35 = static_cast<SePromptR35*>(pSePromptR35Address);
  
  Heap::vFree(pSePromptR35);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SePromptR35::SePromptR35(void)
: Object(CN_SEPROMPT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_iPartition = 0;

  eEC eResult = RapidsSE_SEitfNew((void**)&m_pSEitf,m_iPartition, /*trace*/ 0);
  ASSERT(EC_NONE == eResult);
  
  m_bContinue = true;
  m_bEcho   = false;
  
  m_sPrompt.vSprint("[N%lld %lld] se %s%c> "_Stz,
                    gpAttachMap->m_pGrid->iThisNode(),
                    Process::iGetPid(),
                    RAPIDSSE_VERSION_STRING,
    #ifdef _DEBUG
                    'd'
    #else
                    'r'
    #endif
                    );
  
  
    
  m_sResponse  = ""_Stz;
  //m_xHistory
  
  //----
  m_pTokeniser = new Tokeniser;
  m_pTokeniser->vMalloc(false);
  m_pTokeniser->vPush();
  
  if (gxArguments.m_bCommand) {
    m_pTokeniser->vUseString(gxArguments.m_sCommand);
  }
  else if (gxArguments.m_bPrompt || gxArguments.m_bScript) {
    m_pTokeniser->vPrompt(m_sPrompt);
    m_pTokeniser->vUseStdin();
  }
  
  m_pTokeniser->vLineMode(true);
  m_pTokeniser->vNormal(true);
  
  // open the command history file
  m_xHistory.vOpen();

  _checkInitialised_(CN_SEPROMPT);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SePromptR35::~SePromptR35(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // close history file
  //m_xHistory.vClose();
  
  RapidsSE_SEitfDelete(m_pSEitf, /*trace*/ 0);
  
  delete m_pTokeniser;
}

/*----------------------------------------------------------------------------*/
/* dump SePromptR35                                                              */
/*----------------------------------------------------------------------------*/
void SePromptR35::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(eClass == CN_SEPROMPT);
  RASSERT(pObject->eClass() == CN_SEPROMPT);
  
  SePromptR35* pSePromptR35 = reinterpret_cast<SePromptR35*>(pObject);
  
  vTrace("partition %lld seitf %p continue %s\n"_Stz,
         pSePromptR35->m_iPartition,
         pSePromptR35->m_pSEitf,
         pSePromptR35->m_bContinue ? "true"_Stz : "false"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prompt for commands and process them                                       */
/*----------------------------------------------------------------------------*/
void SePromptR35::vLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  // open the command history file
  //m_xHistory.vOpen();
  
  m_xPosixThread.vSetOSThreadId();
  PosixThread::vTlsSet(giTLS_ThreadKey,&m_xPosixThread);
  //ASSERT(PosixThread::pTlsGet(giTLS_ThreadKey) == &m_xPosixThread);

  // get and process commands
  while (m_bContinue) {
    
    vGetCommand();
    
    if (0 == m_xTokenVec[0]->iLength())
      continue;
    
    bool bProcessResult = bProcessCommand();
    
    if (m_bEcho) {
      vTrace("cmd: "_Stz);
      for (i8 i_Token = 0; i_Token < C_TOKENS; i_Token++) {
        if ( ! m_xTokenVec[i_Token]->bHasText() )
          break;
        vTrace("%s "_Stz,m_xTokenVec[i_Token]);
      }
      vTrace(": got: %s\n"_Stz,m_sResponse.aChar());
      continue;
    }
    
    if (bProcessResult) {
      if (gpAttachMap->m_bMetadata ) {
        vMetaFormat();
      }
      else {
        if ( !m_sResponse.bEqual("<ok>"_Stz) ) {
          vTrace("%s\n"_Stz,m_sResponse.aChar());
        }
      }
    }
    else {
      if (m_pSEitf->eGetLastError() != EC_NONE)
        vTrace("error: %s\n"_Stz,m_pSEitf->pGetLastError());
    }
    
    // clear the response
    m_sResponse = ""_Stz;
  }
  
  // close history file
  //m_xHistory.vClose();
  
  // SHOULD REMOVE HISTORY FILE?
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* clear echo result                                                          */
/*----------------------------------------------------------------------------*/
void SePromptR35::vClearEcho(void)
{
  m_bEcho = false;
}

/*----------------------------------------------------------------------------*/
/* clip tokens                                                                */
/*----------------------------------------------------------------------------*/
void SePromptR35::vClipTokens(i8 i_FromToken)
{
  i8 i_ToToken = 0;
  for ( ; i_FromToken < C_TOKENS; i_FromToken++) {
    
    m_xTokenVec[i_ToToken++]->vCopy(m_xTokenVec[i_FromToken]);
    
    if ( ! m_xTokenVec[i_FromToken]->bHasText() )
      break;
  }
  
}

/*----------------------------------------------------------------------------*/
/* collect ';' delimited statement                                            */
/*----------------------------------------------------------------------------*/
void SePromptR35::vCollectStatement(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // we should have a first token (e.g. create or drop)
  ASSERT(m_xTokenVec[0]->bHasText());
  
  // have we already seen the ';'
  i8 i_Token = m_pTokeniser->iFindToken(";"_Stz,m_xTokenVec,C_TOKENS);
  
  if (i_Token >= 0) {
    //m_pTokeniser->vDumpTokens("\nCommand : "_Stz,m_xTokenVec,C_TOKENS);
    return;
  }
  
  i_Token = m_pTokeniser->iCountTokens(m_xTokenVec,C_TOKENS);
  
  // switch to terminal mode
  m_pTokeniser->vLineMode(false);
  
  s32 sContinuation("  > "_Stz);
  m_pTokeniser->vPrompt(sContinuation);
  
  // get tokens delimited by ';'
  for ( ; i_Token < C_TOKENS; i_Token++) {
    
    Token* pToken = m_xTokenVec[i_Token];
    eTN eToken = m_pTokeniser->eNextToken((char*)pToken->aText());
    ASSERT(TN_NONE != eToken);
    pToken->vToken(eToken);
    
    // look for terminal
    if (TN_OTHER == eToken && pToken->bSame(";"_Stz))
      break;
  }
  
  // switch back to line mode
  m_pTokeniser->vLineMode(true);
  m_pTokeniser->vPrompt(m_sPrompt);
  
  //m_pTokeniser->vDumpTokens("Command : "_Stz,m_xTokenVec,C_TOKENS);
}

/*----------------------------------------------------------------------------*/
/* execute an se request                                                      */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bExecuteRequest(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Timer xTimer;
  Stz* aResponse = nullptr;
  
  xTimer.vStart();
  
  m_xTokenVec.vSetTokens();
  
  //! make this part of verbose??
  //m_pTokeniser->vDumpTokens("Command : "_Stz,m_xTokenVec,C_TOKENS);
  
  bool bResult = RapidsSE_ExecuteRequest(m_pSEitf,m_xTokenVec,&aResponse);
  
  if (m_xTokenVec.bDetachedRequest())
    return bResult;
  
  if (bResult)
    m_sResponse = aResponse ? (Stz*)aResponse : ""_Stz;
  else
    m_sResponse = aResponse ? (Stz*)aResponse : ""_Stz;
  
  xTimer.vUpdate();
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* get command                                                                */
/*----------------------------------------------------------------------------*/
void SePromptR35::vGetCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  if (gxArguments.m_bCommand)
    vGetCommandArgument();
  else {
    vGetCommandInput();
    
    // if empty line just return
    if ( ! m_xTokenVec[0]->bHasText() )
      return;
    
    // if its a command file just return
    if (bTestFile(m_xTokenVec[0]->aText()))
      return;
    
    // collect till ;
    vCollectStatement();
    
    // stip off ;
    m_xTokenVec.vStripSemicolon();

    // stip off &
    m_xTokenVec.vStripAmpersand();

  }
}

/*----------------------------------------------------------------------------*/
/* get command from arguments                                                 */
/*----------------------------------------------------------------------------*/
void SePromptR35::vGetCommandArgument(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_xTokenVec.vReset();
  
  while (TN_NONE !=
         m_pTokeniser->eNextToken((char*)m_xTokenVec[m_xTokenVec.nTokens()]->aText())) {
    
    //vTrace("vGetCommand : [%lld] %s\n"_Stz,i_Token,m_xTokenVec[i_Token]);
    
    m_xTokenVec.vIncTokens();
    ASSERT(m_xTokenVec.nTokens() < C_TOKENS);
  }
  
  m_bContinue = false;
}

/*----------------------------------------------------------------------------*/
/* get command from input                                                     */
/*----------------------------------------------------------------------------*/
void SePromptR35::vGetCommandInput(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  while (true) {
    
    m_xTokenVec.vReset();
    m_xTokenVec2.vReset();
    
    for (;;) {

      Token* pToken = m_xTokenVec[m_xTokenVec.nTokens()];
    
      eTN eToken = m_pTokeniser->eNextToken((char*)pToken->aText());
      
      if (TN_NONE == eToken)
        break;

      pToken->vToken(eToken);
                     
//      vTrace("vGetCommand : token %lld ]%s[\n"_Stz,
//             m_xTokenVec.nTokens(),
//             m_xTokenVec[m_xTokenVec.nTokens()]->aText());
      
      m_xTokenVec.vIncTokens();
      ASSERT(m_xTokenVec.nTokens() < C_TOKENS);
    }
    
    // end of file? if so, pop and retry
    if (! m_xTokenVec.nTokens() && m_pTokeniser->bEOF()) {
      bool b = m_pTokeniser->bPop();
      ASSERT(b);
      continue;
    }
    
    break;
  }
  
}

/*----------------------------------------------------------------------------*/
/* print output in metadata format                                            */
/*----------------------------------------------------------------------------*/
void SePromptR35::vMetaFormat(void)
{
  
  if ( !m_sResponse.bEqual("<ok>"_Stz) ) {
    if ( ! m_pSEitf->m_sMetadata.bEqual( ""_Stz))
      vTrace("metadata: %s\n"_Stz, m_pSEitf->m_sMetadata.aStr());
    vTrace("%s"_Stz,m_sResponse.aChar());
    vTrace("success: %lld rows affected\n"_Stz, m_pSEitf->m_iRowsProcessed);
  }
  else {
    vTrace("success: %lld rows affected\n"_Stz, m_pSEitf->m_iRowsProcessed);
  }
  
  // reset counters
  m_pSEitf->m_iRowsProcessed = 0;
  m_pSEitf->m_sMetadata      = ""_Stz;
}

/*----------------------------------------------------------------------------*/
/* process command                                                            */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bProcessCommand(bool bPostProcess)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for echo prefix
  if (bCmdIs("echo"_Stz))
    vSetEcho();
  else
    vClearEcho();
  
  //vTrace("command: %s\n"_Stz,m_xTokenVec[0]->sText().aChar());
  
   for (i8 i_Cmd = 0; SePromptR35Commands[i_Cmd].aCommand; i_Cmd++) {
   
     if ( ! bCmdIs((Stz*)SePromptR35Commands[i_Cmd].aCommand))
       continue;
     
     if (! SePromptR35Commands[i_Cmd].aAlternate)
       return (this->*SePromptR35Commands[i_Cmd].aFunc)();
     
     if (bOneIs((Stz*)SePromptR35Commands[i_Cmd].aAlternate)) {
       return (this->*SePromptR35Commands[i_Cmd].aFunc)();
     }
   }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // append to history
  m_xHistory.vAppend(m_xTokenVec);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for command file
  if (bTestFile(m_xTokenVec[0]->aText()))
    return bRunFile();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // execute seitf request
  if (! bExecuteRequest())
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // post process requests
  if (! bPostProcess)
    return true;
  
  if ( bCmdIs("fetch"_Stz) || bCmdIs("dfetch"_Stz)) {
    vIterateBuffers();
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // the following never return
  
  if (m_xTokenVec[0]->bSame("mon"_Stz)) {
    while (true) {
      PosixThread::vSleepSec(10);
      if (!bExecuteRequest())
        return true;
    }
  }
  
  if (m_xTokenVec[0]->bSame("x"_Stz)) {
    while (true)
      bExecuteRequest();
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* push command file                                                          */
/*----------------------------------------------------------------------------*/
void SePromptR35::vPushFile(Stz* aFileName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  s256 sPath;
  
  sPath = aFileName;
  
  m_pTokeniser->vPush();
  
  bool b = m_pTokeniser->bUseFile(sPath);
  ASSERT(b);
}

/*----------------------------------------------------------------------------*/
/* return table                                                               */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bReturnTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_xTokenVec[0]->vCopy("rettbl"_Stz);
  m_xTokenVec[1]->vCopy(""_Stz);
  
  if (!bExecuteRequest()) {
    vTrace("bReturnTable: (%s) Return Table FAILED!!! : %s\n"_Stz, m_sResponse.aChar());
    return false;
  }
  return true;
}

/*----------------------------------------------------------------------------*/
/* set file to run                                                            */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bRunFile(void)
{
  if ( ! bTokenCheck(1) )
    return false;
  
  vPushFile(m_xTokenVec[0]->aText());
  m_pTokeniser->vLineMode(true);
  m_sResponse = "<ok>"_Stz;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* set echo result                                                            */
/*----------------------------------------------------------------------------*/
void SePromptR35::vSetEcho(void)
{
  m_bEcho = true;
  
  vClipTokens(1);
}

/*----------------------------------------------------------------------------*/
/* test for command file                                                      */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bTestFile(Stz* aFileName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  File xFile;
  
  if (xFile.bExists(aFileName))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* check for a specific number of tokens                                      */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bTokenCheck(i8 nTokens)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  if (m_xTokenVec[nTokens]->bHasText()) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  return true;
}
  
/*----------------------------------------------------------------------------*/
/* check if command token 0 is given argument                                 */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmdIs(Stz* aArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  if (m_xTokenVec[0]->bSame(aArg))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* check if command token 1 is given argument                                 */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bOneIs(Stz* aArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  if (m_xTokenVec[1]->bSame(aArg))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SePromptR35::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*-- Intercepted Commands ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* analyze table                                                              */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_AnalyzeTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  
  s256 sDir;
  Dir xDir;
  
  ASSERT(m_xTokenVec[0]->bSame("antbl"_Stz));
  
  return bExecuteRequest();
}

/*----------------------------------------------------------------------------*/
/* begin insert                                                               */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_BeginInsert(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(m_xTokenVec[0]->bSame("begin"_Stz));
  ASSERT(m_xTokenVec[1]->bSame("insert"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy tokens from m_xTokenVec to m_xTokenVec2
  i8 iNumTokens = iCopyTokens(m_xTokenVec, m_xTokenVec2);
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // get table pointer
  m_xTokenVec[0]->vCopy("gettbl"_Stz);
  m_xTokenVec[1]->vCopy(m_xTokenVec2[2]);
  if (!bExecuteRequest()) {
    vTrace("SePromptR35::bBeginInsert : gettbl FAILED!!!\n"_Stz);
    return false;
  }
  
  // begin a transaction
  m_xTokenVec[0]->vCopy("txBeg"_Stz);
  m_xTokenVec[1]->vCopy(""_Stz);
  if (!bExecuteRequest()) {
     vTrace("SePromptR35::bBeginInsert : txBegin FAILED!!!\n"_Stz);
     m_pSEitf->vSetLastError(EC_TX_BEGIN_FAILED);
    return false;
  }
  
  m_sResponse = "<ok>"_Stz;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* bye                                                                        */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_Bye(void)
{
  if ( ! bTokenCheck(1) )
    return false;
  
  m_bContinue = false;
  
  m_sResponse = "<ok>"_Stz;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* change working directory                                                   */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_ChangeDir(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  s256 sDir;
  Dir xDir;
  
  ASSERT(m_xTokenVec[0]->bSame("cd"_Stz));
  
  sDir = m_xTokenVec[1]->aText();
  
  i8 i_Token;
  for (i_Token = 2; i_Token < C_TOKENS; i_Token++) {
    if (! m_xTokenVec[i_Token]->bHasText())
      break;
    sDir.vConcat(m_xTokenVec[i_Token]->aText());
  }
  
  if ( ! xDir.bChangeCurrent(sDir) ) {
    m_pSEitf->vSetLastError(EC_NO_SUCH_DIRECTORY);
    return false;
  }
  
  m_sResponse = "<ok>"_Stz;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* create catalog                                                             */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_CreateCatalog(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(m_xTokenVec[0]->bSame("create"_Stz));
  ASSERT(m_xTokenVec[1]->bSame("catalog"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy tokens from m_xTokenVec to m_xTokenVec2
  i8 iNumTokens = iCopyTokens(m_xTokenVec, m_xTokenVec2);
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // create a catalog
  m_xTokenVec[0]->vCopy("mkcat"_Stz);
  m_xTokenVec[1]->vCopy(m_xTokenVec2[2]);
  
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Catalog created: %s\n"_Stz, m_xTokenVec2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* create index                                                               */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_CreateIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iIfUnique = 0;
  
  ASSERT(m_xTokenVec[0]->bSame("create"_Stz));
  
  // copy tokens from m_xTokenVec to m_xTokenVec2
  i8 iNumTokens = iCopyTokens(m_xTokenVec, m_xTokenVec2);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // some validations
  bool bValid = bValidateCreateIndex(iNumTokens);
  if ( ! bValid )
    return false;
  
  if (m_xTokenVec2[1]->bSame("unique"_Stz))
    iIfUnique = 1;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get reference to pTable
  m_xTokenVec[0]->vCopy("gettbl"_Stz);
  m_xTokenVec[1]->vCopy(m_xTokenVec2[4 + iIfUnique]);
  
  if (!bExecuteRequest()) {
    vTrace("SePromptR35::bCreateIndex : gettbl FAILED!!!\n"_Stz);
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the index
  
  m_xTokenVec[0]->vCopy("mkidx"_Stz);
  m_xTokenVec[1]->vCopy(m_xTokenVec2[4 + iIfUnique]);
  m_xTokenVec[2]->vCopy(m_xTokenVec2[2 + iIfUnique]);
  
  if (!bExecuteRequest()) {
    // save error returned by addColumn
    eEC eError = m_pSEitf->eGetLastError();
    
    // return the table
    bReturnTable();
    
    m_pSEitf->vSetLastError(eError);
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the index columns
  
  // now collect the column names and their types
  i8 iCurrentToken = 6 + iIfUnique;
  while (iCurrentToken < iNumTokens - 1) {
    
    if (iCurrentToken != 6 + iIfUnique && !m_xTokenVec2[iCurrentToken++]->bSame(","_Stz)) {
      bIndexReturnTable(m_xTokenVec2[4 + iIfUnique]->aText(),
                        m_xTokenVec2[2 + iIfUnique]->aText());
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      return false;
    }
    
    // add columns to the table
    i8 j = 0;
    m_xTokenVec[j++]->vCopy("addidxcol"_Stz);
    m_xTokenVec[j++]->vCopy(m_xTokenVec2[4 + iIfUnique]);
    m_xTokenVec[j++]->vCopy(m_xTokenVec2[2 + iIfUnique]);
    m_xTokenVec[j++]->vCopy(m_xTokenVec2[iCurrentToken++]);
    
    // add the column
    if (!bExecuteRequest()) {
      
      // save error returned by addColumn
      eEC eError = m_pSEitf->eGetLastError();
      
      // drop the table that was created
      bIndexReturnTable(m_xTokenVec2[4 + iIfUnique]->aText(),
                        m_xTokenVec2[2 + iIfUnique]->aText());
      
      // return error returned by addColumn
      m_pSEitf->vSetLastError(eError);
      return false;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // uniqueness
  
  if (iIfUnique) {
    
    m_xTokenVec[0]->vCopy("setidxunique"_Stz);
    m_xTokenVec[1]->vCopy(m_xTokenVec2[4 + iIfUnique]);
    m_xTokenVec[2]->vCopy(m_xTokenVec2[2 + iIfUnique]);
    m_xTokenVec[3]->vCopy("true"_Stz);
    m_xTokenVec[4]->vCopy(""_Stz);
    
    if (!bExecuteRequest()) {
      vTrace("SePromptR35::bCreateIndex : setidxunique FAILED!!!\n"_Stz);
      // save error returned by addColumn
      eEC eError = m_pSEitf->eGetLastError();
      
      // drop the table that was created
      bIndexReturnTable(m_xTokenVec2[4 + iIfUnique]->aText(),
                        m_xTokenVec2[2 + iIfUnique]->aText());
      
      // return error returned by addColumn
      m_pSEitf->vSetLastError(eError);
      return false;
    }
    
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // end of make index
  
  m_xTokenVec[0]->vCopy("endmkidx"_Stz);
  m_xTokenVec[1]->vCopy(m_xTokenVec2[4 + iIfUnique]);
  m_xTokenVec[2]->vCopy(m_xTokenVec2[2 + iIfUnique]);
  m_xTokenVec[3]->vCopy(""_Stz);
  
  if (!bExecuteRequest()) {
    //vTrace("SePromptR35::bCreateIndex : endmkidx FAILED!!!\n"_Stz);
    // save error returned by addColumn
    eEC eError = m_pSEitf->eGetLastError();
    
    // drop the table that was created
    bIndexReturnTable(m_xTokenVec2[4 + iIfUnique]->aText(),
                      m_xTokenVec2[2 + iIfUnique]->aText());
    
    // return error returned by addColumn
    m_pSEitf->vSetLastError(eError);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* create schema                                                              */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_CreateSchema(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(m_xTokenVec[0]->bSame("create"_Stz));
  ASSERT(m_xTokenVec[1]->bSame("schema"_Stz));
  
  i8 i_Token;
  
  // copy tokens from m_xTokenVec to m_xTokenVec2
  // while doing that also validate the create statament
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! m_xTokenVec[i_Token]->bHasText())
      break;
    m_xTokenVec2[i_Token]->vCopy(m_xTokenVec[i_Token]);
    m_xTokenVec[i_Token]->vCopy(""_Stz);
  }
  
  i8 iNumTokens = i_Token;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // create a schema
  m_xTokenVec[0]->vCopy("mksch"_Stz);
  m_xTokenVec[1]->vCopy(m_xTokenVec2[2]);
  
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Schema created: %s\n"_Stz, m_xTokenVec2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* create table                                                               */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_CreateTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(m_xTokenVec[0]->bSame("create"_Stz));
  ASSERT(m_xTokenVec[1]->bSame("table"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy tokens from m_xTokenVec to m_xTokenVec2
  i8 iNumTokens = iCopyTokens(m_xTokenVec, m_xTokenVec2);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // some validations
  bool bValid = bValidateCreateTable(iNumTokens);
  if ( ! bValid )
    return false;
  
  // make the table
  if ( ! bMakeTable() )
    return false;
  
  // mark current toen
  i8 iCurrentToken = 4;
  
  // save current token as the first column of the table
  Stz* pFirstColumn = m_xTokenVec2[iCurrentToken]->aText();
  
  // add columns to the table
  if ( ! bAddTableColumns(iCurrentToken, iNumTokens) )
    return false;
  
  // current token should be at ')'
  if ( ! m_xTokenVec2[iCurrentToken++]->bSame(")"_Stz)) {
    bool bRemove = bRemoveTable(m_xTokenVec2[2]->aText());
    ASSERT(bRemove);
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // check if we have a partition by clause
  if ( m_xTokenVec2[iCurrentToken++]->bSame("partition"_Stz)) {
    
    if ( ! bPartitionTable(iCurrentToken, iNumTokens) )
      return false;
    
  }
  else {
    // no partition by clause
    // we will create partitioning using the first column
    bPartitionTable(pFirstColumn);
  }
  
  if ( ! bEndMakeTable() )
    return false;
  
  //vTrace("Table created: %s\n"_Stz, m_xTokenVec2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* create tenant                                                              */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_CreateTenant(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(m_xTokenVec[0]->bSame("create"_Stz));
  ASSERT(m_xTokenVec[1]->bSame("tenant"_Stz));
  
  i8 i_Token;
  
  // copy tokens from m_xTokenVec to m_xTokenVec2
  // while doing that also validate the create statament
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! m_xTokenVec[i_Token]->bHasText())
      break;
    m_xTokenVec2[i_Token]->vCopy(m_xTokenVec[i_Token]);
    m_xTokenVec[i_Token]->vCopy(""_Stz);
  }
  
  i8 iNumTokens = i_Token;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // create a tenant
  m_xTokenVec[0]->vCopy("mkten"_Stz);
  m_xTokenVec[1]->vCopy(m_xTokenVec2[2]);
  
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Tenant created: %s\n"_Stz, m_xTokenVec2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* create index                                                               */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_CreateUnique(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(m_xTokenVec[0]->bSame("create"_Stz));
  ASSERT(m_xTokenVec[1]->bSame("unique"_Stz));
  
  return bCmd_CreateIndex();
}

/*----------------------------------------------------------------------------*/
/* detailed analyze table                                                     */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_DanalyzeTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  s256 sDir;
  Dir xDir;
  
  ASSERT(m_xTokenVec[0]->bSame("dantbl"_Stz));
  
  return bExecuteRequest();
}

/*----------------------------------------------------------------------------*/
/* docol                                                                      */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_Docol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(m_xTokenVec[0]->bSame("docol"_Stz));
  return bExecuteRequest();
}

/*----------------------------------------------------------------------------*/
/* load a table from a file  (distributed and parallel)                       */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_DistParLoadTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  bool bResult = false;
  i8 iIndex;
  i8 iCurrentIndex;
  bool bSameMachine = false;
  
  ASSERT(m_xTokenVec[0]->bSame("dpld"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy tokens from m_xTokenVec to m_xTokenVec2
  i8 iNumTokens = iCopyTokens(m_xTokenVec, m_xTokenVec2);
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be at least 3 tokens
  if (iNumTokens < 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    goto resetFormatter;
  }
  
  iIndex = 0;
  m_xTokenVec[iIndex++]->vCopy(m_xTokenVec2[0]);
  m_xTokenVec[iIndex++]->vCopy(m_xTokenVec2[1]);
  m_xTokenVec[iIndex++]->vCopy(m_xTokenVec2[2]);
  
  iCurrentIndex = 3;
  
  while (iNumTokens > iCurrentIndex) {
    
    //--------------------------------------------------------------------------
    // C_FIELD_DELIMITER
    if (m_xTokenVec2[iCurrentIndex]->bSame(C_FIELD_DELIMITER)) {
      
      if (iNumTokens < iCurrentIndex + 2) {
        m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
        goto resetFormatter;
      }
      
      m_pSEitf->m_xFormatters.m_cFieldDelimiter = *m_xTokenVec2[++iCurrentIndex]->sText().aChar();
      if (m_pSEitf->m_xFormatters.m_cFieldDelimiter == '0')
        m_pSEitf->m_xFormatters.m_cFieldDelimiter = '\0';
    }
    //--------------------------------------------------------------------------
    // C_SAME_MACHINE
    else if (m_xTokenVec2[iCurrentIndex]->bSame(C_SAME_MACHINE)) {
      bSameMachine = true;
    }
    else {
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      goto resetFormatter;
    }
    iCurrentIndex++;
  }
  
  m_xTokenVec[iIndex++]->vCopy(bSameMachine ? "1"_Stz : "0"_Stz);
  
  //m_pSEitf->m_xFormatters.vShow();
  
  //
  // clear the last error
  m_pSEitf->vClearLastError();
  
  if (!bExecuteRequest())
    goto resetFormatter;
  
  bResult = true;
  
resetFormatter:
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // reset seitf formatters
  
  m_pSEitf->m_xFormatters.vReset(&gpAttachMap->m_xFormatters);
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* drop catalog                                                               */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_DropCatalog(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 i_Token;
  i8 iNumTokens;
  
  ASSERT(m_xTokenVec[0]->bSame("drop"_Stz));
  ASSERT(m_xTokenVec[1]->bSame("catalog"_Stz));
  
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! m_xTokenVec[i_Token]->bHasText())
      break;
    m_xTokenVec2[i_Token]->vCopy(m_xTokenVec[i_Token]);
    m_xTokenVec[i_Token]->vCopy(""_Stz);
  }
  
  iNumTokens = i_Token;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a catalog
  m_xTokenVec[0]->vCopy("rmcat"_Stz);
  m_xTokenVec[1]->vCopy(m_xTokenVec2[2]);
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Catalog dropped: %s\n"_Stz, m_xTokenVec2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop index                                                                 */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_DropIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 i_Token;
  i8 iNumTokens;
  
  ASSERT(m_xTokenVec[0]->bSame("drop"_Stz));
  ASSERT(m_xTokenVec[1]->bSame("index"_Stz));
  
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! m_xTokenVec[i_Token]->bHasText())
      break;
    m_xTokenVec2[i_Token]->vCopy(m_xTokenVec[i_Token]);
    m_xTokenVec[i_Token]->vCopy(""_Stz);
  }
  
  iNumTokens = i_Token;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 5 tokens
  if (iNumTokens != 5) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // 4th token should be '.'
  if (!m_xTokenVec2[3]->bSame("."_Stz)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a table
  m_xTokenVec[0]->vCopy("rmidx"_Stz);
  m_xTokenVec[1]->vCopy(m_xTokenVec2[2]);
  m_xTokenVec[2]->vCopy(m_xTokenVec2[4]);
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Table dropped: %s\n"_Stz, m_xTokenVec2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop schema                                                                */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_DropSchema(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 i_Token;
  i8 iNumTokens;
  
  ASSERT(m_xTokenVec[0]->bSame("drop"_Stz));
  ASSERT(m_xTokenVec[1]->bSame("schema"_Stz));
  
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! m_xTokenVec[i_Token]->bHasText())
      break;
    m_xTokenVec2[i_Token]->vCopy(m_xTokenVec[i_Token]);
    m_xTokenVec[i_Token]->vCopy(""_Stz);
  }
  
  iNumTokens = i_Token;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a schema
  m_xTokenVec[0]->vCopy("rmsch"_Stz);
  m_xTokenVec[1]->vCopy(m_xTokenVec2[2]);
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Schema dropped: %s\n"_Stz, m_xTokenVec2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop table                                                                 */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_DropTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 i_Token;
  i8 iNumTokens;
  
  ASSERT(m_xTokenVec[0]->bSame("drop"_Stz));
  ASSERT(m_xTokenVec[1]->bSame("table"_Stz));
  
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! m_xTokenVec[i_Token]->bHasText())
      break;
    m_xTokenVec2[i_Token]->vCopy(m_xTokenVec[i_Token]);
    m_xTokenVec[i_Token]->vCopy(""_Stz);
  }
  
  iNumTokens = i_Token;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a table
  m_xTokenVec[0]->vCopy("rmtbl"_Stz);
  m_xTokenVec[1]->vCopy(m_xTokenVec2[2]);
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Table dropped: %s\n"_Stz, m_xTokenVec2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop tenant                                                                */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_DropTenant(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 i_Token;
  i8 iNumTokens;
  
  ASSERT(m_xTokenVec[0]->bSame("drop"_Stz));
  ASSERT(m_xTokenVec[1]->bSame("tenant"_Stz));
  
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! m_xTokenVec[i_Token]->bHasText())
      break;
    m_xTokenVec2[i_Token]->vCopy(m_xTokenVec[i_Token]);
    m_xTokenVec[i_Token]->vCopy(""_Stz);
  }
  
  iNumTokens = i_Token;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a tenant
  m_xTokenVec[0]->vCopy("rmten"_Stz);
  m_xTokenVec[1]->vCopy(m_xTokenVec2[2]);
  if (!bExecuteRequest()) {
    return false;
  }
  
  //vTrace("Tenant dropped: %s\n"_Stz, m_xTokenVec2[2]);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* end insert                                                                 */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_EndInsert(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 i_Token;
  i8 iNumTokens;
  
  ASSERT(m_xTokenVec[0]->bSame("end"_Stz));
  ASSERT(m_xTokenVec[1]->bSame("insert"_Stz));
  
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! m_xTokenVec[i_Token]->bHasText())
      break;
    m_xTokenVec2[i_Token]->vCopy(m_xTokenVec[i_Token]);
    m_xTokenVec[i_Token]->vCopy(""_Stz);
  }
  
  iNumTokens = i_Token;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 2 tokens
  if (iNumTokens != 2) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // clear the last error
  m_pSEitf->vClearLastError();
  
  Grid* pGrid = gpAttachMap->m_pGrid;
  
  // before sending buffers set no of remaining acks to no of partitons
  m_pSEitf->vSetAcksRemaining(pGrid->nSlots() * pGrid->nNodes());
  
  for (i8 iPartition = 0; iPartition < pGrid->m_nPartitions; iPartition++) {
    bool bResult = m_pSEitf->bInsertLastBuffer(iPartition);
    
    //vTrace("bInsertLastBuffer returned %lld %lld\n"_Stz, iPartition, bResult);
    
    // if rowBuffer request was appended to requestQ, then wait for resonse
    if (bResult) {
      // get response from se
      eEC eResult = eGetResponse(m_pSEitf);
      
      if (eResult != EC_NONE) {
        m_pSEitf->vSetLastError(eResult);
        bResult = bReturnTable();
        if (!bResult)
          return false;
      }
    }
  }

  // now wait till acks from all partitions have been received
  while (*m_pSEitf->m_ziAcksRemaining != 0) {
    //vTrace("*pSEitf->m_ziAcksRemaining %lld ...\n"_Stz, *pSEitf->m_ziAcksRemaining);
    ;
  }
  
  // now return the table
  bool bResult = bReturnTable();
  if (!bResult)
    return false;
  
  ASSERT(m_pSEitf->m_pCurrentTable == nullptr);
  
  // commit a transaction
  m_xTokenVec[0]->vCopy("txCmt"_Stz);
  if (!bExecuteRequest()) {
    vTrace("SePromptR35::EndInsert : txCmt FAILED!!!\n"_Stz);
    m_pSEitf->vSetLastError(EC_TX_COMMIT_FAILED);
    return false;
  }
  
  // begin a transaction
  m_xTokenVec[0]->vCopy("txEnd"_Stz);
  if (!bExecuteRequest()) {
    vTrace("SePromptR35::EndInsert : txEnd FAILED!!!\n"_Stz);
    m_pSEitf->vSetLastError(EC_TX_END_FAILED);
    return false;
  }
  
  m_sResponse = "<ok>"_Stz;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* help                                                                       */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_Help(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  vTrace("se commands:\n"_Stz);
  vTrace("antbl      -- analyze table\n"_Stz);
  vTrace("bye        -- quit se prompt\n"_Stz);
  vTrace("cf         -- change folder\n"_Stz);
  vTrace("create     -- create tenant|catalog|schema|table|index\n"_Stz);
  vTrace("dex        -- distributed export table t\n"_Stz);
  vTrace("drop       -- drop tenant|catalog|schema|table|index\n"_Stz);
  vTrace("ex         -- export table t\n"_Stz);
  vTrace("fetch      -- fetch t where ...\n"_Stz);
  vTrace("keyselidx  -- exact key select\n"_Stz);
  vTrace("ld         -- load table t\n"_Stz);
  vTrace("lf         -- list folder contents\n"_Stz);
  vTrace("ls         -- list directory contents\n"_Stz);
  vTrace("lscat      -- list catalogs contents\n"_Stz);
  vTrace("lscol      -- list columns\n"_Stz);
  vTrace("lsidx      -- list indices\n"_Stz);
  vTrace("lsidxcol   -- list columns of indiex\n"_Stz);
  vTrace("lssch      -- list schemas contents\n"_Stz);
  vTrace("lstbl      -- list tables contents\n"_Stz);
  vTrace("lsten      -- list tenants contents\n"_Stz);
  vTrace("lwrselidx  -- lower bound select\n"_Stz);
  vTrace("print      -- print string\n"_Stz);
  vTrace("pwf        -- print working folder\n"_Stz);
  vTrace("rgeselidx  -- range select\n"_Stz);
  vTrace("sel        -- select * from table\n"_Stz);
  vTrace("space      -- pool space\n"_Stz);
  vTrace("status     -- grid status\n"_Stz);
  vTrace("sz         -- size of table\n"_Stz);
  vTrace("szidx      -- size of an index\n"_Stz);
  vTrace("tableSizes -- size of all tables\n"_Stz);
  vTrace("tr         -- truncate table\n"_Stz);
  vTrace("uprselidx  -- upper bound select\n"_Stz);
  vTrace("usesch     -- use schema\n"_Stz);
  vTrace("version    -- print SE version\n"_Stz);
  vTrace("?          -- print this help\n"_Stz);
  vTrace("help       -- print this help\n"_Stz);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* print history                                                              */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_History(void)
{
  if ( ! bTokenCheck(1) )
    return false;
  
  m_xHistory.vPrint();
  
  m_sResponse = "<ok>"_Stz;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* insert a row                                                               */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_InsertRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 i_Token;
  i8 iNumTokens;
  bool bBlankLine = false;
  
  ASSERT(m_xTokenVec[0]->bSame("insert"_Stz));
  ASSERT(m_xTokenVec[1]->bSame("row"_Stz));
  
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! m_xTokenVec[i_Token]->bHasText())
      break;
    m_xTokenVec2[i_Token]->vCopy(m_xTokenVec[i_Token]);
    m_xTokenVec[i_Token]->vCopy(""_Stz);
  }
  
  iNumTokens = i_Token;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // clear the last error
  m_pSEitf->vClearLastError();
  
  if (!m_pSEitf->pCurrentTable()) {
    m_pSEitf->vSetLastError(EC_NO_SUCH_TABLE);
    return false;
  }
  
  Stz* pRow = m_xTokenVec2[2]->aText();
  s48K s;
  i8 iRowLength;
  
  // format the record into row format
  bool bValid = Row::bFormatRow(
                  m_pSEitf->m_pCurrentTable->pColumnVec(),
                  &m_pSEitf->m_xFormatters,
                  pRow, StrLib::iSize(pRow->aChar()),
                  s, iRowLength, bBlankLine,
                  false /* allowNonPrintable */);
  
  if (! bValid) {
    m_pSEitf->vSetLastError(EC_INVALID_VALUE);
    return false;
  }
  
  // get the partition this row hashes to
  i8 iRowPartition = m_pSEitf->pCurrentTable()->iRowPartition(s);
  
  bool bResult = m_pSEitf->bInsertRow(s, iRowLength, iRowPartition);
  
  if (!bResult) {
    m_pSEitf->vSetLastError(m_pSEitf->m_eErrorCode);
    return bResult;
  }
  
  m_sResponse = "<ok>"_Stz;
  
  return true;
}
  
/*----------------------------------------------------------------------------*/
/* load a table from a file                                                   */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_LoadTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  bool bResult = false;
  i8 iIndex;
  i8 iCurrentIndex;
  
  ASSERT(m_xTokenVec[0]->bSame("ld"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy tokens from m_xTokenVec to m_xTokenVec2
  i8 iNumTokens = iCopyTokens(m_xTokenVec, m_xTokenVec2);

  // - - - - - - - - - - - - - - - - - - - - -
  // there should be at least 3 tokens
  if (iNumTokens < 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    goto resetFormatter;
  }
  
  iIndex = 0;
  m_xTokenVec[iIndex++]->vCopy(m_xTokenVec2[0]);
  m_xTokenVec[iIndex++]->vCopy(m_xTokenVec2[1]);
  m_xTokenVec[iIndex++]->vCopy(m_xTokenVec2[2]);
  
  iCurrentIndex = 3;
    
  while (iNumTokens > iCurrentIndex) {
    
    if (iNumTokens < iCurrentIndex + 2) {
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      goto resetFormatter;
    }
    
    //--------------------------------------------------------------------------
    // C_RECORD_TERMINATOR
    if (m_xTokenVec2[iCurrentIndex]->bSame(C_RECORD_TERMINATOR)) {
      m_pSEitf->m_xFormatters.m_cRecordTerminator = *m_xTokenVec2[++iCurrentIndex]->sText().aChar();
      if (m_pSEitf->m_xFormatters.m_cRecordTerminator == '0')
        m_pSEitf->m_xFormatters.m_cRecordTerminator = '\0';
    }
    //--------------------------------------------------------------------------
    // C_RECORD_ENDS
    else if (m_xTokenVec2[iCurrentIndex]->bSame(C_RECORD_ENDS)) {
      char cRecordEnds = *m_xTokenVec2[++iCurrentIndex]->sText().aChar();
      
      // is record_ends 1 character ?
      if (m_xTokenVec2[iCurrentIndex]->iLength() == 1) {
        if (cRecordEnds == '0') {
          vWarn("Invalid load : record_ends can only be \\n, \\r or \\r\\n\n"_Stz);
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          goto resetFormatter;
        }
        else if (cRecordEnds == '\n') {
          m_pSEitf->m_xFormatters.m_eRecordEnds = RE_NL;
        }
        else if (cRecordEnds == '\r') {
          m_pSEitf->m_xFormatters.m_eRecordEnds = RE_CR;
        }
      }
      // record_ends is 2 characters
      else if (m_xTokenVec2[iCurrentIndex]->iLength() == 2) {
        
        if (m_xTokenVec2[iCurrentIndex]->bSame("\r\n"_Stz)) {
          m_pSEitf->m_xFormatters.m_eRecordEnds = RE_CRNL;
        }
        else {
          vWarn("Invalid load : record_ends can only be \\n, \\r or \\r\\n\n"_Stz);
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          goto resetFormatter;
        }
      }
      else {
        vWarn("Invalid load : record_ends can only be \\n, \\r or \\r\\n\n"_Stz);
        m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
        goto resetFormatter;
      }
    }
    //--------------------------------------------------------------------------
    // C_FIELD_DELIMITER
    else if (m_xTokenVec2[iCurrentIndex]->bSame(C_FIELD_DELIMITER)) {
      m_pSEitf->m_xFormatters.m_cFieldDelimiter = *m_xTokenVec2[++iCurrentIndex]->sText().aChar();
      if (m_pSEitf->m_xFormatters.m_cFieldDelimiter == '0')
        m_pSEitf->m_xFormatters.m_cFieldDelimiter = '\0';
    }
    //--------------------------------------------------------------------------
    // C_ENCLOSED_BY_LEADING
    else if (m_xTokenVec2[iCurrentIndex]->bSame(C_ENCLOSED_BY_LEADING)) {
      m_pSEitf->m_xFormatters.m_cEnclosedBy_Leading =
        *m_xTokenVec2[++iCurrentIndex]->sText().aChar();
      if (m_pSEitf->m_xFormatters.m_cEnclosedBy_Leading == '0')
        m_pSEitf->m_xFormatters.m_cEnclosedBy_Leading = '\0';
      
      // default set trailing same as leading
      m_pSEitf->m_xFormatters.m_cEnclosedBy_Trailing =
        m_pSEitf->m_xFormatters.m_cEnclosedBy_Leading;
    }
    //--------------------------------------------------------------------------
    // C_ENCLOSED_BY_TRAILING
    else if (m_xTokenVec2[iCurrentIndex]->bSame(C_ENCLOSED_BY_TRAILING)) {
      
      // if trailing is specified, leading should also have been specified
      if (m_pSEitf->m_xFormatters.m_cEnclosedBy_Leading == '\0') {
        if ( ! gpAttachMap->m_bMetadata)
          vWarn("Invalid load : enclosed_by_trailing specified but no enclosed_by_leading\n"_Stz);
        m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
        goto resetFormatter;
      }
      m_pSEitf->m_xFormatters.m_cEnclosedBy_Trailing =
        *m_xTokenVec2[++iCurrentIndex]->sText().aChar();
      if (m_pSEitf->m_xFormatters.m_cEnclosedBy_Trailing == '0')
        m_pSEitf->m_xFormatters.m_cEnclosedBy_Trailing = '\0';
    }
//
//    DATE_FORMAT='YYYY.MM.DD’
//
//
    //--------------------------------------------------------------------------
    // C_DATE_FORMAT
    else if (m_xTokenVec2[iCurrentIndex]->bSame(C_DATE_FORMAT)) {
      // extract date format
      if ( ! m_pSEitf->bExtractDateFormat(m_xTokenVec2[++iCurrentIndex]->sText()) ) {
        if ( ! gpAttachMap->m_bMetadata)
          vWarn("Invalid load : enclosed_by_trailing specified but no enclosed_by_leading\n"_Stz);
        m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
        goto resetFormatter;
      }
    }
    
    //--------------------------------------------------------------------------
    // C_ESCAPE_CHAR
    else if (m_xTokenVec2[iCurrentIndex]->bSame(C_ESCAPE_CHAR)) {
      m_pSEitf->m_xFormatters.m_cEscapeChar = *m_xTokenVec2[++iCurrentIndex]->sText().aChar();
      if (m_pSEitf->m_xFormatters.m_cEscapeChar == '0')
        m_pSEitf->m_xFormatters.m_cEscapeChar = '\0';
    }
    else {
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      goto resetFormatter;
    }
    iCurrentIndex++;
  }
  
  //m_pSEitf->m_xFormatters.vShow();
  
  //
  // clear the last error
  m_pSEitf->vClearLastError();

  if (!bExecuteRequest())
    goto resetFormatter;
  
  bResult = true;
  
resetFormatter:
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // reset seitf formatters
  
  m_pSEitf->m_xFormatters.vReset(&gpAttachMap->m_xFormatters);
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* list directory                                                             */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_ListDir(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Dir  xDir;
  s256 sDir;
  s256 sDirEntry;
  i8 iEntryType;
  
  ASSERT(m_xTokenVec[0]->bSame("ls"_Stz));
  
  if ( ! bTokenCheck(1) )
    return false;
  
  // get the current directory
  if (!xDir.bGetCurrent(sDir)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // open the current directory
  if (!xDir.bOpen()) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // read the current directory
  while (xDir.bRead()) {
    sDirEntry = xDir.sEntryName();
    iEntryType = xDir.iEntryType();
    vTrace("%c %s\n"_Stz, iEntryType == DT_DIR ? 'd' : 'f', sDirEntry.aChar());
  }
  
  // close the current directory
  xDir.vClose();
  
  // clear the response
  m_sResponse = "<ok>"_Stz;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* print working directory                                                    */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_PrintDir(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Dir xDir;
  s256 sDir;
  
  ASSERT(m_xTokenVec[0]->bSame("pwd"_Stz));
  
  if ( ! bTokenCheck(1) )
    return false;
  
  if (xDir.bGetCurrent(sDir)) {
    vTrace("%s"_Stz, sDir.aChar());
  }
  
  // clear the respnse
  m_sResponse = "<ok>"_Stz;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* recall command                                                             */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_RecallCommand(void)
{
  if ( ! bTokenCheck(1) )
    return false;
  
  if (! m_xHistory.bRecall(m_xTokenVec))
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* sleep n seconds                                                            */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_Sleep(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(m_xTokenVec[0]->bSame("sleep"_Stz));
  
  if ( ! bTokenCheck(2) )
    return false;
  
  i8 nSeconds = m_xTokenVec[1]->iToI8();
  
  PosixThread::vSleepSec(nSeconds);
  
  m_sResponse = "<ok>"_Stz;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* use schema                                                                 */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_UseSchema(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 i_Token;
  i8 iNumTokens;
  
  ASSERT(m_xTokenVec[0]->bSame("use"_Stz));
  ASSERT(m_xTokenVec[1]->bSame("schema"_Stz));
  
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! m_xTokenVec[i_Token]->bHasText())
      break;
    m_xTokenVec2[i_Token]->vCopy(m_xTokenVec[i_Token]);
    m_xTokenVec[i_Token]->vCopy(""_Stz);
  }
  
  iNumTokens = i_Token;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a schema
  m_xTokenVec[0]->vCopy("usesch"_Stz);
  m_xTokenVec[1]->vCopy(m_xTokenVec2[2]);
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Schema changed: %s\n"_Stz, m_xTokenVec2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* timer start                                                                */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_TimerStart(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_xTimer.vStart();

  return true;
}

/*----------------------------------------------------------------------------*/
/* timer end                                                                  */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_TimerEnd(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_xTimer.vUpdate();
  vTrace("duration %lldms\n"_Stz,m_xTimer.iMilliseconds());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add a partition column to the table                                        */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bAddPartitionColumn(Stz* pTable, Stz* pColumn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 j = 0;
  m_xTokenVec[j++]->vCopy("addpartcol"_Stz);
  m_xTokenVec[j++]->vCopy(pTable);
  m_xTokenVec[j++]->vCopy(pColumn);
  m_xTokenVec[j++]->vCopy(""_Stz);
  
  // add the column
  if (!bExecuteRequest()) {
    
    // save error returned by addColumn
    eEC eError = m_pSEitf->eGetLastError();
    
    vTrace("SePromptR35::bAddPartitionColumn : addpartcol FAILED!!!\n"_Stz);
    bool bDrop = bDropReturnTable(m_xTokenVec2[2]->aText());
    ASSERT(bDrop);
    
    // return error returned by addColumn
    m_pSEitf->vSetLastError(eError);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* add columns to a table                                                     */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bAddTableColumns(i8& i_Token, i8 iNumTokens)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  bool bDrop;
  
  // now collect the column names and their types
  i8 iCurrentToken = i_Token;
  
  while ( ! m_xTokenVec2[iCurrentToken]->bSame(")"_Stz)) {
    
    if (iCurrentToken != i_Token && ! m_xTokenVec2[iCurrentToken++]->bSame(","_Stz)) {
      bDropReturnTable(m_xTokenVec2[2]->aText());
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      return false;
    }
    
    // add columns to the table
    i8 j = 0;
    m_xTokenVec[j++]->vCopy("addcol"_Stz);
    m_xTokenVec[j++]->vCopy(m_xTokenVec2[2]);
    m_xTokenVec[j++]->vCopy(m_xTokenVec2[iCurrentToken++]);
    
    if (m_xTokenVec2[iCurrentToken]->bSame("varchar"_Stz) ||
        m_xTokenVec2[iCurrentToken]->bSame("varbinary"_Stz) ||
        m_xTokenVec2[iCurrentToken]->bSame("char"_Stz)) {
      
      i8 iSize;
      
      // copy varchar/char
      m_xTokenVec[j]->vCopy(m_xTokenVec2[iCurrentToken++]);
      if (m_xTokenVec2[iCurrentToken]->bSame("("_Stz)) {
        
        // str.cat '('
        m_xTokenVec[j]->vConcat(m_xTokenVec2[iCurrentToken++]->aText());
        
        // str.cat the size
        m_xTokenVec[j]->vConcat(m_xTokenVec2[iCurrentToken]->aText());
        
        // check if it was numeric
        if ( ! m_xTokenVec2[iCurrentToken]->bIsNumeric() ) {
          // drop the table
          bDrop = bDropReturnTable(m_xTokenVec2[2]->aText());
          ASSERT(bDrop);
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        } else {
          iSize = m_xTokenVec2[iCurrentToken++]->iToI8();
        }
        
        if (!m_xTokenVec2[iCurrentToken]->bSame(")"_Stz)) {
          // drop the table
          bDrop = bDropReturnTable(m_xTokenVec2[2]->aText());
          ASSERT(bDrop);
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        }
        
        // str.cat ')'
        m_xTokenVec[j]->vConcat(m_xTokenVec2[iCurrentToken++]->aText());
        
        if (!Vch::bIsValidSize(iSize)) {
          bDropReturnTable(m_xTokenVec2[2]->aText());
          m_pSEitf->vSetLastError(EC_INVALID_SIZE);
          return false;
        }
      }
      
    }
    else if (m_xTokenVec2[iCurrentToken]->bSame("float"_Stz)) {
      
      i8 iSize;
      
      // copy varchar/char
      m_xTokenVec[j]->vCopy(m_xTokenVec2[iCurrentToken++]);
      if (m_xTokenVec2[iCurrentToken]->bSame("("_Stz)) {
        
        // str.cat '('
        m_xTokenVec[j]->vConcat(m_xTokenVec2[iCurrentToken++]->aText());
        
        // str.cat the size
        m_xTokenVec[j]->vConcat(m_xTokenVec2[iCurrentToken]->aText());
        
        // check if it was numeric
        if ( ! m_xTokenVec2[iCurrentToken]->bIsNumeric() ) {
          // drop the table
          bDrop = bDropReturnTable(m_xTokenVec2[2]->aText());
          ASSERT(bDrop);
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        } else {
          iSize = m_xTokenVec2[iCurrentToken++]->iToI8();
        }
        
        if (!m_xTokenVec2[iCurrentToken]->bSame(")"_Stz)) {
          // drop the table
          bDrop = bDropReturnTable(m_xTokenVec2[2]->aText());
          ASSERT(bDrop);
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        }
        
        // str.cat ')'
        m_xTokenVec[j]->vConcat(m_xTokenVec2[iCurrentToken++]->aText());
        
        if (!Flt::bIsValidSize(iSize)) {
          bDropReturnTable(m_xTokenVec2[2]->aText());
          m_pSEitf->vSetLastError(EC_INVALID_SIZE);
          return false;
        }
      }
      
    }
    else if (m_xTokenVec2[iCurrentToken]->bSame("decimal"_Stz)) {
      
      i8 iPrecision;
      i8 iScale;
      
      // copy varchar/char
      m_xTokenVec[j]->vCopy(m_xTokenVec2[iCurrentToken++]);
      
      if (m_xTokenVec2[iCurrentToken]->bSame("("_Stz)) {
        
        // str.cat '('
        m_xTokenVec[j]->vConcat(m_xTokenVec2[iCurrentToken++]->aText());
        
        // str.cat the precision
        m_xTokenVec[j]->vConcat(m_xTokenVec2[iCurrentToken]->aText());
        
        // check if it was numeric
        if ( ! m_xTokenVec2[iCurrentToken]->bIsNumeric() ) {
          bDropReturnTable(m_xTokenVec2[2]->aText());
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        } else {
          iPrecision = m_xTokenVec2[iCurrentToken++]->iToI8();
        }
        
        if (!m_xTokenVec2[iCurrentToken]->bSame(","_Stz)) {
          bDropReturnTable(m_xTokenVec2[2]->aText());
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        }
        
        // str.cat ','
        m_xTokenVec[j]->vConcat(m_xTokenVec2[iCurrentToken++]->aText());
        
        // str.cat the size
        m_xTokenVec[j]->vConcat(m_xTokenVec2[iCurrentToken]->aText());
        
        // check if it was numeric
        if ( ! m_xTokenVec2[iCurrentToken]->bIsNumeric() ) {
          bDropReturnTable(m_xTokenVec2[2]->aText());
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        } else {
          iScale = m_xTokenVec2[iCurrentToken++]->iToI8();
        }
        
        if (!m_xTokenVec2[iCurrentToken]->bSame(")"_Stz)) {
          bDropReturnTable(m_xTokenVec2[2]->aText());
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        }
        
        // str.cat ')'
        m_xTokenVec[j]->vConcat(m_xTokenVec2[iCurrentToken++]->aText());
        
        if (!Bcd::bIsValidSize(iPrecision, iScale)) {
          bDropReturnTable(m_xTokenVec2[2]->aText());
          m_pSEitf->vSetLastError(EC_INVALID_SIZE);
          return false;
        }
      }
    }
    else if (m_xTokenVec2[iCurrentToken]->bSame("integer"_Stz) ||
             m_xTokenVec2[iCurrentToken]->bSame("date"_Stz) ||
             m_xTokenVec2[iCurrentToken]->bSame("time"_Stz) ||
             m_xTokenVec2[iCurrentToken]->bSame("timestamp"_Stz) ||
             m_xTokenVec2[iCurrentToken]->bSame("interval"_Stz) ||
             m_xTokenVec2[iCurrentToken]->bSame("boolean"_Stz)) {
      m_xTokenVec[j]->vCopy(m_xTokenVec2[iCurrentToken++]);
    }
    else {
      // drop the table that was created
      bDropReturnTable(m_xTokenVec2[2]->aText());
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      return false;
    }
    
    // check for NOT NULL
    if (m_xTokenVec2[iCurrentToken]->bSame("not"_Stz)) {
      
      m_xTokenVec[j]->vConcat(" "_Stz);
      m_xTokenVec[j]->vConcat(m_xTokenVec2[iCurrentToken++]->aText());
      
      if (!m_xTokenVec2[iCurrentToken]->bSame("null"_Stz)) {
        bDropReturnTable(m_xTokenVec2[2]->aText());
        m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
        return false;
      }
      m_xTokenVec[j]->vConcat(" "_Stz);
      m_xTokenVec[j]->vConcat(m_xTokenVec2[iCurrentToken++]->aText());
    }
    
    // check for PRIMARY KEY
    if (m_xTokenVec2[iCurrentToken]->bSame("primary"_Stz)) {
      
      m_xTokenVec[j]->vConcat(" "_Stz);
      m_xTokenVec[j]->vConcat(m_xTokenVec2[iCurrentToken++]->aText());
      
      if (!m_xTokenVec2[iCurrentToken]->bSame("key"_Stz)) {
        bDropReturnTable(m_xTokenVec2[2]->aText());
        m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
        return false;
      }
      m_xTokenVec[j]->vConcat(" "_Stz);
      m_xTokenVec[j]->vConcat(m_xTokenVec2[iCurrentToken++]->aText());
    }
    
    // check for UNIQUE
    if (m_xTokenVec2[iCurrentToken]->bSame("unique"_Stz)) {
      
      m_xTokenVec[j]->vConcat(" "_Stz);
      m_xTokenVec[j]->vConcat(m_xTokenVec2[iCurrentToken++]->aText());
    }
    
    
    // now we should see a column
    m_xTokenVec[j]->aText()->vToLower();
    
    // add the column
    if (!bExecuteRequest()) {
      
      // save error returned by addColumn
      eEC eError = m_pSEitf->eGetLastError();
      
      // drop the table that was created
      bDropReturnTable(m_xTokenVec2[2]->aText());
      
      // return error returned by addColumn
      m_pSEitf->vSetLastError(eError);
      return false;
    }
  }
  
  i_Token = iCurrentToken;
  return true;
}

/*----------------------------------------------------------------------------*/
/* add columns of the table to the job                                        */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bAddTableColumnsR35(i8& i_Token, i8 iNumTokens)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  bool bDrop;
  Job_CreateTable* pJob = (Job_CreateTable*)m_pSEitf->m_pJob;
  
  // now collect the column names and their types
  i8 iCurrentToken = i_Token;
  i8 iColumn = 0;
  
  while ( ! m_xTokenVec[iCurrentToken]->bSame(")"_Stz)) {
    
    if (iCurrentToken != i_Token && ! m_xTokenVec[iCurrentToken++]->bSame(","_Stz)) {
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      return false;
    }
    
    // add columns to the table
  
    pJob->m_xColumns[iColumn].m_iColumnNo = iColumn;
    pJob->m_xColumns[iColumn].m_sColumnName= m_xTokenVec[iCurrentToken++]->aText();
    
    if (m_xTokenVec[iCurrentToken]->bSame("varchar"_Stz) ||
        m_xTokenVec[iCurrentToken]->bSame("varbinary"_Stz) ||
        m_xTokenVec[iCurrentToken]->bSame("char"_Stz)) {
      
      pJob->m_xColumns[iColumn].m_eColumnType = CT_VARCHAR;
      pJob->m_xColumns[iColumn].m_eDatumType = DM_VCH;
      iCurrentToken++;
      
      i8 iSize = C_64K;

      if (m_xTokenVec[iCurrentToken]->bSame("("_Stz)) {
        
        pJob->m_xColumns[iColumn].m_iColumnLength = m_xTokenVec[iCurrentToken++]->iToI8();
        
        
        // check if it was numeric
        if ( ! m_xTokenVec[iCurrentToken]->bIsNumeric() ) {
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        } else {
          iSize = m_xTokenVec[iCurrentToken++]->iToI8();
        }
        
        if (!m_xTokenVec[iCurrentToken++]->bSame(")"_Stz)) {
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        }
        
        if (!Vch::bIsValidSize(iSize)) {
          m_pSEitf->vSetLastError(EC_INVALID_SIZE);
          return false;
        }
      }
      pJob->m_xColumns[iColumn].m_iColumnLength = iSize;
      
    }
    else if (m_xTokenVec[iCurrentToken]->bSame("float"_Stz)) {
      
      i8 iSize = C_MAX_PRECISION_FLT;
    
      pJob->m_xColumns[iColumn].m_eColumnType = CT_FLOAT;
      pJob->m_xColumns[iColumn].m_eDatumType = DM_FLT;
      iCurrentToken++;
      
      if (m_xTokenVec[iCurrentToken]->bSame("("_Stz)) {
        
        // check if it was numeric
        if ( ! m_xTokenVec[iCurrentToken++]->bIsNumeric() ) {
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        } else {
          iSize = m_xTokenVec[iCurrentToken++]->iToI8();
        }
        
        if (!m_xTokenVec[iCurrentToken++]->bSame(")"_Stz)) {
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        }
        
        if (!Flt::bIsValidSize(iSize)) {
          m_pSEitf->vSetLastError(EC_INVALID_SIZE);
          return false;
        }
      }
      pJob->m_xColumns[iColumn].m_iColumnLength = iSize;
    }
    else if (m_xTokenVec[iCurrentToken]->bSame("decimal"_Stz)) {
      
      i8 iPrecision = C_MAX_PRECISION_BCD;
      i8 iScale = 0;
      
      pJob->m_xColumns[iColumn].m_eColumnType = CT_DECIMAL;
      pJob->m_xColumns[iColumn].m_eDatumType = DM_BCD;
      iCurrentToken++;
      
      if (m_xTokenVec[iCurrentToken++]->bSame("("_Stz)) {
        
        // check if it was numeric
        if ( ! m_xTokenVec[iCurrentToken]->bIsNumeric() ) {
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        } else {
          iPrecision = m_xTokenVec[iCurrentToken++]->iToI8();
        }
        
        if (!m_xTokenVec[iCurrentToken++]->bSame(","_Stz)) {
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        }
        
        // check if it was numeric
        if ( ! m_xTokenVec[iCurrentToken]->bIsNumeric() ) {
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        } else {
          iScale = m_xTokenVec[iCurrentToken++]->iToI8();
        }
        
        if (!m_xTokenVec[iCurrentToken++]->bSame(")"_Stz)) {
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        }
        
        if (!Bcd::bIsValidSize(iPrecision, iScale)) {
          m_pSEitf->vSetLastError(EC_INVALID_SIZE);
          return false;
        }
      }
      pJob->m_xColumns[iColumn].m_iColumnLength = iPrecision;
      pJob->m_xColumns[iColumn].m_iColumnScale  = iScale;
    }
    else if (m_xTokenVec[iCurrentToken]->bSame("integer"_Stz)) {
      pJob->m_xColumns[iColumn].m_eColumnType = CT_INTEGER;
      pJob->m_xColumns[iColumn].m_eDatumType = DM_INT;
      pJob->m_xColumns[iColumn].m_iColumnLength = C_MAX_PRECISION_INT;
      iCurrentToken++;
    }
    else if (m_xTokenVec[iCurrentToken]->bSame("date"_Stz)) {
      pJob->m_xColumns[iColumn].m_eColumnType = CT_DATE;
      pJob->m_xColumns[iColumn].m_eDatumType = DM_DTE;
      iCurrentToken++;
    }
    else if (m_xTokenVec[iCurrentToken]->bSame("time"_Stz)) {
      pJob->m_xColumns[iColumn].m_eColumnType = CT_TIME;
      pJob->m_xColumns[iColumn].m_eDatumType = DM_TME;
      iCurrentToken++;
    }
    else if (m_xTokenVec[iCurrentToken]->bSame("timestamp"_Stz)) {
      pJob->m_xColumns[iColumn].m_eColumnType = CT_TIMESTAMP;
      pJob->m_xColumns[iColumn].m_eDatumType = DM_TSP;
      iCurrentToken++;
    }
    else if (m_xTokenVec[iCurrentToken]->bSame("interval"_Stz)) {
      pJob->m_xColumns[iColumn].m_eColumnType = CT_INTERVAL;
      pJob->m_xColumns[iColumn].m_eDatumType = DM_IVL;
      iCurrentToken++;
    }
    else if (m_xTokenVec[iCurrentToken]->bSame("boolean"_Stz)) {
      pJob->m_xColumns[iColumn].m_eColumnType = CT_BOOLEAN;
      pJob->m_xColumns[iColumn].m_eDatumType = DM_BOL;
      iCurrentToken++;
    }
    else {
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      return false;
    }
    
    // check for NOT NULL
    if (m_xTokenVec[iCurrentToken]->bSame("not"_Stz)) {
      iCurrentToken++;
      if (!m_xTokenVec[iCurrentToken++]->bSame("null"_Stz)) {
        m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
        return false;
      }
      pJob->m_xColumns[iColumn].m_bIsNullable = false;
    }
    
    // check for PRIMARY KEY
//    if (m_xTokenVec2[iCurrentToken]->bSame("primary"_Stz)) {
//
//      pJob->m_xColumns[iColumn].m_sPrimaryKey = m_xTokenVec[iCurrentToken++]->aText();
//
//      if (!m_xTokenVec2[iCurrentToken]->bSame("key"_Stz)) {
//        m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
//        return false;
//      }
//    }
    
    // check for UNIQUE
    if (m_xTokenVec[iCurrentToken]->bSame("unique"_Stz)) {
      iCurrentToken++;
      pJob->m_xColumns[iColumn].m_bIsUnique = true;
    }
    
    iColumn++;
    
  }
  pJob->m_iColumns = iColumn;
  
  i_Token = iCurrentToken;
  return true;
}

/*----------------------------------------------------------------------------*/
/* copy tokens from a token vector to another                                 */
/*----------------------------------------------------------------------------*/
i8 SePromptR35::iCopyTokens(TokenVec& xFromTokenVec, TokenVec& xToTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // copy tokens from m_xTokenVec to m_xTokenVec2
  // while doing that also validate the create statament
  i8 i_Token;
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! xFromTokenVec[i_Token]->bHasText())
      break;
    xToTokenVec[i_Token]->vCopy(xFromTokenVec[i_Token]);
    xFromTokenVec[i_Token]->vCopy(""_Stz);
  }
  
  return i_Token;
}

/*----------------------------------------------------------------------------*/
/* drop return table                                                          */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bDropReturnTable(Stz* aTableName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // derefernce and return the table first
  bool bResult = bReturnTable();
  ASSERT(bResult);
  
  bool bRemove = bRemoveTable(aTableName);
  ASSERT(bRemove);
  
  if (!bRemove) {
    return false;
  }
  return true;
}

/*----------------------------------------------------------------------------*/
/* end a make table                                                           */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bEndMakeTable()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // end of make table
  m_xTokenVec[0]->vCopy("endmktbl"_Stz);
  m_xTokenVec[1]->vCopy(m_xTokenVec2[2]);
  m_xTokenVec[2]->vCopy(""_Stz);
  
  if (!bExecuteRequest()) {
    vTrace("SePromptR35::bEndMakeTable : endmktbl FAILED!!!\n"_Stz);
    bool bRemove = bRemoveTable(m_xTokenVec2[2]->aText());
    //ASSERT(bRemove);
    return false;
  }
  
  // derefernce and return the table first
  bool bResult = bReturnTable();
  ASSERT(bResult);
  
  return true;
}
  
/*----------------------------------------------------------------------------*/
/* index return table                                                         */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bIndexReturnTable(Stz* aTableName, Stz* aIndexName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // derefernce and return the table first
  bool bResult = bReturnTable();
  ASSERT(bResult);
  
  m_xTokenVec[0]->vCopy("rmidx"_Stz);
  m_xTokenVec[1]->vCopy(aTableName);
  m_xTokenVec[2]->vCopy(aIndexName);
  m_xTokenVec[3]->vCopy(""_Stz);
  
  if (!bExecuteRequest()) {
    //vFatal("Error: (%s) Drop Table %s\n"_Stz, m_sResponse.aChar(), pTableName);
    return false;
  }
  return true;
}

/*----------------------------------------------------------------------------*/
/* iterate entry buffers frim the seitf queue                                 */
/*----------------------------------------------------------------------------*/
void SePromptR35::vIterateBuffers(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 nPartition;
  m_pSEitf->m_iRowsProcessed = 0;
  
  if (gpAttachMap->m_bMetadata ) {
    vTrace("metadata: %s\n"_Stz, m_pSEitf->m_sMetadata.aStr());
  }
  
  if (m_pSEitf->m_bDisplay) {
    
    while (m_pSEitf->bNextRow2(nPartition)) {
      
      m_pSEitf->m_iRowsProcessed++;
      
      //Entry* pEntry = m_pSEitf->m_pEntry;
      //Row* aRow = pEntry->aRow();
      Row* aRow = m_pSEitf->m_aRow;
      aRow->vAsBinaryString(m_pSEitf->m_sResponse,
                            m_pSEitf->m_pCurrentTable->pColumnVec());
      if (gpAttachMap->m_bMetadata )
        vTrace("row: (%lld, %s)\n"_Stz, nPartition, m_pSEitf->m_sResponse.aChar());
      else
        vTrace("row: (%s)\n"_Stz, m_pSEitf->m_sResponse.aChar());
    }
    if (gpAttachMap->m_bMetadata )
      vTrace("success: %lld rows affected\n"_Stz, m_pSEitf->m_iRowsProcessed);
    else
      vTrace("rows selected %lld\n"_Stz, m_pSEitf->m_iRowsProcessed);
  }
  else {
    // pop the buffers
    m_pSEitf->vPopBufferQueue();
    vTrace("rows selected %lld\n"_Stz, m_pSEitf->m_iRowsProcessed);
  }
  m_pSEitf->m_iRowsProcessed = 0;
  
  m_pSEitf->vEndFetch();
  
  bReturnTable();
  
}

/*----------------------------------------------------------------------------*/
/* make a table                                                               */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bMakeTable()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // create the table
  m_xTokenVec[0]->vCopy("mktbl"_Stz);
  m_xTokenVec[1]->vCopy(m_xTokenVec2[2]);
  
  if (!bExecuteRequest())
    return false;
  
  // change folder to table
//  m_xTokenVec[0]->vCopy("cf"_Stz);
//  m_xTokenVec[1]->vCopy(m_xTokenVec2[2]);
//  
//  if (!bExecuteRequest()) {
//    vTrace("SePromptR35::bCreateTable : cf FAILED!!!\n"_Stz);
//    bool bRemove = bRemoveTable(m_xTokenVec2[2]->aText());
//    //ASSERT(bRemove);
//    return false;
//  }
  
  // get reference to pTable
  m_xTokenVec[0]->vCopy("gettbl"_Stz);
  m_xTokenVec[1]->vCopy(m_xTokenVec2[2]);
  
  if (!bExecuteRequest()) {
    vTrace("SePromptR35::bCreateTable : gettbl FAILED!!!\n"_Stz);
    bool bRemove = bRemoveTable(m_xTokenVec2[2]->aText());
    
    if ( ! bRemove )
      vTrace("SePromptR35::bCreateTable : remtbl FAILED!!!\n"_Stz);
    
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* partition the table                                                        */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bPartitionTable(i8 i_Token, i8 iNumTokens)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // now collect the column names and their types
  i8 iCurrentToken = i_Token;
  
  // next token should be 'by'
  if (!m_xTokenVec2[iCurrentToken++]->bSame("by"_Stz)) {
    bool bDrop = bDropReturnTable(m_xTokenVec2[2]->aText());
    ASSERT(bDrop);
    
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // next token should be '('
  if (!m_xTokenVec2[iCurrentToken++]->bSame("("_Stz)) {
    bool bDrop = bDropReturnTable(m_xTokenVec2[2]->aText());
    ASSERT(bDrop);
    
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the partition columns
  i_Token = iCurrentToken;
  i8 iPartKeys = 0;
  while (iCurrentToken < iNumTokens - 1) {
    
    if ( (iCurrentToken != i_Token) &&
        !m_xTokenVec2[iCurrentToken++]->bSame(","_Stz)) {
      
      bool bDrop = bDropReturnTable(m_xTokenVec2[2]->aText());
      ASSERT(bDrop);
      
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      return false;
    }
    iPartKeys++;
    
    // add columns to the table
    bool bAdd = bAddPartitionColumn(m_xTokenVec2[2]->aText(),
                                    m_xTokenVec2[iCurrentToken++]->aText());
    if ( ! bAdd)
      return false;
  }
  
  if ( ! iPartKeys ) {
    
    bool bDrop = bDropReturnTable(m_xTokenVec2[2]->aText());
    ASSERT(bDrop);
    
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* partition the table                                                        */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bPartitionTableR35(i8 i_Token, i8 iNumTokens)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // now collect the column names and their types
  i8 iCurrentToken = i_Token;
  
  // next token should be 'by'
  if (!m_xTokenVec[iCurrentToken++]->bSame("by"_Stz)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // next token should be '('
  if (!m_xTokenVec[iCurrentToken++]->bSame("("_Stz)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the partition columns
  Job_CreateTable* pJob = (Job_CreateTable*)m_pSEitf->m_pJob;
  i_Token = iCurrentToken;
  i8 iPartKeys = 0;
  bool bFound = false;
  while (iCurrentToken < iNumTokens - 1) {
    
    if ( (iCurrentToken != i_Token) &&
        !m_xTokenVec[iCurrentToken++]->bSame(","_Stz)) {
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      return false;
    }
    iPartKeys++;
    
    // find column and set as partition column
    for (i8 i_Column=0; i_Column< pJob->m_iColumns; i_Column++) {
      if (pJob->m_xColumns[i_Column].m_sColumnName.bSame(m_xTokenVec[iCurrentToken]->aText())) {
        pJob->m_xColumns[i_Column].m_bIsPartitionKey = true;
        iCurrentToken++;
        bFound = true;
        break;
      }
    }
    if ( ! bFound) {
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      return false;
    }
    bFound = false;
  }
  
  if ( ! iPartKeys ) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* partition the table                                                        */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bPartitionTable(Stz* pColumnName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // add columns to the table
  bool bAdd = bAddPartitionColumn(m_xTokenVec2[2]->aText(), pColumnName);
  if ( ! bAdd)
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* remove a table                                                             */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bRemoveTable(Stz* pTableName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // create the table
  m_xTokenVec[0]->vCopy("rmtbl"_Stz);
  m_xTokenVec[1]->vCopy(pTableName);
  m_xTokenVec[2]->vCopy(""_Stz);
  
  if (!bExecuteRequest()) {
    return false;
  }
  
  return true;
}
  
/*----------------------------------------------------------------------------*/
/* validate a create index request                                            */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bValidateCreateIndex(i8 iNumTokens)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iIfUnique = 0;
  
  if (m_xTokenVec2[1]->bSame("unique"_Stz)) {
    iIfUnique = 1;
    if ( ! m_xTokenVec2[2]->bSame("index"_Stz)) {
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      return false;
    }
  }
  else {
    if ( ! m_xTokenVec2[1]->bSame("index"_Stz)) {
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      return false;
    }
  }
  
  // there should be a minumum of 8 tokens
  if (iNumTokens < 8 + iIfUnique) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }

  // token[3] should be 'on'
  if (!m_xTokenVec2[3 + iIfUnique]->bSame("on"_Stz)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }

  // token[5] should be '('
  if (!m_xTokenVec2[5 + iIfUnique]->bSame("("_Stz)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }

  // last-1 token should be ')'
  if (!m_xTokenVec2[iNumTokens - 1]->bSame(")"_Stz)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* validate a create index request                                            */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bValidateCreateIndexR35(i8 iNumTokens)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iIfUnique = 0;
  
  if (m_xTokenVec[1]->bSame("unique"_Stz)) {
    iIfUnique = 1;
    if ( ! m_xTokenVec[2]->bSame("R35index"_Stz)) {
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      return false;
    }
  }
  else {
    if ( ! m_xTokenVec[1]->bSame("R35index"_Stz)) {
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      return false;
    }
  }
  
  // there should be a minumum of 8 tokens
  if (iNumTokens < 8 + iIfUnique) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // token[3] should be 'on'
  if (!m_xTokenVec[3 + iIfUnique]->bSame("on"_Stz)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // token[5] should be '('
  if (!m_xTokenVec[5 + iIfUnique]->bSame("("_Stz)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last-1 token should be ')'
  if (!m_xTokenVec[iNumTokens - 1]->bSame(")"_Stz)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* validate a create table request                                            */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bValidateCreateTable(i8 iNumTokens)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // there should be a minumum of 7 tokens
  if (iNumTokens < 7) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }

  // token[3] should be '('
  if ( ! m_xTokenVec2[3]->bSame("("_Stz)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last-1 token should be ')'
  if ( ! m_xTokenVec2[iNumTokens-1]->bSame(")"_Stz)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* validate a create table request                                            */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bValidateCreateTableR35(i8 iNumTokens)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // there should be a minumum of 7 tokens
  if (iNumTokens < 7) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // token[3] should be '('
  if ( ! m_xTokenVec[3]->bSame("("_Stz)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last-1 token should be ')'
  if ( ! m_xTokenVec[iNumTokens-1]->bSame(")"_Stz)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

/*----------------------------------------------------------------------------*/
/* create catalog                                                             */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_CreateCatalogR35(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(m_xTokenVec[0]->bSame("create"_Stz));
  ASSERT(m_xTokenVec[1]->bSame("R35catalog"_Stz));
  
  i8 i_Token;
  
  // validate the create statament
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! m_xTokenVec[i_Token]->bHasText())
      break;
  }
  
  i8 iNumTokens = i_Token;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // create the job
  Job_CreateCatalog* pJob_CreateCatalog = new Job_CreateCatalog(m_pSEitf);
  pJob_CreateCatalog->m_sObjectName = m_xTokenVec[2]->aText();
  
  // set the job in SEitf
  m_pSEitf->m_pJob = pJob_CreateCatalog;
  
  // create a schema
  m_xTokenVec[0]->vCopy("mkcatR35"_Stz);
  m_xTokenVec[1]->vCopy(""_Stz);
  
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Schema created: %s\n"_Stz, m_xTokenVec2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* create index                                                               */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_CreateIndexR35(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iIfUnique = 0;
  
  ASSERT(m_xTokenVec[0]->bSame("create"_Stz));
  
  // validate the create statament
  i8 i_Token;
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! m_xTokenVec[i_Token]->bHasText())
      break;
  }
  
  i8 iNumTokens = i_Token;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // some validations
  bool bValid = bValidateCreateIndexR35(iNumTokens);
  if ( ! bValid )
    return false;
  
  if (m_xTokenVec[1]->bSame("unique"_Stz))
    iIfUnique = 1;
  
  
  // create the job
  Job_CreateIndex* pJob_CreateIndex = new Job_CreateIndex(m_pSEitf);
  
  // set the index name
  pJob_CreateIndex->m_sObjectName = m_xTokenVec[2 + iIfUnique]->aText();
  
  // set the table name
  pJob_CreateIndex->m_sTableName = m_xTokenVec[4 + iIfUnique]->aText();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the index columns
  
  // now collect the column names and their types
  i8 iCurrentToken = 6 + iIfUnique;
  i8 iColumn = 0;
  while (iCurrentToken < iNumTokens - 1) {
    
    if (iCurrentToken != 6 + iIfUnique && !m_xTokenVec2[iCurrentToken++]->bSame(","_Stz)) {
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      return false;
    }
    pJob_CreateIndex->m_sColumnName[iColumn] = m_xTokenVec[iCurrentToken++]->aText();
  }
  pJob_CreateIndex->m_iColumns = iColumn;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // uniqueness
  
  if (iIfUnique) {
    pJob_CreateIndex->m_bIsUnique = true;
  }
  
  // create a schema
  m_xTokenVec[0]->vCopy("mkidxR35"_Stz);
  m_xTokenVec[1]->vCopy(""_Stz);
  
  if (!bExecuteRequest())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* create schema                                                              */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_CreateSchemaR35(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(m_xTokenVec[0]->bSame("create"_Stz));
  ASSERT(m_xTokenVec[1]->bSame("R35schema"_Stz));
  
  i8 i_Token;
  
  // validate the create statament
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! m_xTokenVec[i_Token]->bHasText())
      break;
  }
  
  i8 iNumTokens = i_Token;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // create the job
  Job_CreateSchema* pJob_CreateSchema = new Job_CreateSchema(m_pSEitf);
  pJob_CreateSchema->m_sObjectName = m_xTokenVec[2]->aText();
  
  // set the job in SEitf
  m_pSEitf->m_pJob = pJob_CreateSchema;
  
  // create a schema
  m_xTokenVec[0]->vCopy("mkschR35"_Stz);
  m_xTokenVec[1]->vCopy(""_Stz);
  
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Schema created: %s\n"_Stz, m_xTokenVec2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* create table                                                               */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_CreateTableR35(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(m_xTokenVec[0]->bSame("create"_Stz));
  ASSERT(m_xTokenVec[1]->bSame("R35table"_Stz));
  
  i8 i_Tokens;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // validate the create statament
  for (i_Tokens = 0; i_Tokens < C_TOKENS; i_Tokens++) {
    if (! m_xTokenVec[i_Tokens]->bHasText())
      break;
  }
  i8 iNumTokens = i_Tokens;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // some validations
  bool bValid = bValidateCreateTableR35(iNumTokens);
  if ( ! bValid )
    return false;
  
  // create the job
  Job_CreateTable* pJob_CreateTable = new Job_CreateTable(m_pSEitf);
  
  // set the table name
  pJob_CreateTable->m_sObjectName = m_xTokenVec[2]->aText();
  
  // set the job in SEitf
  m_pSEitf->m_pJob = pJob_CreateTable;
  
  // mark current token
  i8 iCurrentToken = 4;
  
  // add columns to the table
  if ( ! bAddTableColumnsR35(iCurrentToken, iNumTokens) ) {
    delete pJob_CreateTable;
    return false;
  }
  
  // current token should be at ')'
  if ( ! m_xTokenVec[iCurrentToken++]->bSame(")"_Stz)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // check if we have a partition by clause
  if ( m_xTokenVec[iCurrentToken++]->bSame("partition"_Stz)) {
    
    if ( ! bPartitionTableR35(iCurrentToken, iNumTokens) )
      return false;
  }
  else {
    // no partition by clause
    // we will create partitioning using the first column
    pJob_CreateTable->m_xColumns[0].m_bIsPartitionKey = true;
  }
  
  pJob_CreateTable->vShow();
  
  m_xTokenVec[0]->vCopy("mktblR35"_Stz);
  m_xTokenVec[1]->vCopy(""_Stz);
  
  // excute mktbl request
  if (!bExecuteRequest())
    return false;

  return true;
}

/*----------------------------------------------------------------------------*/
/* create tenant                                                              */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_CreateTenantR35(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(m_xTokenVec[0]->bSame("create"_Stz));
  ASSERT(m_xTokenVec[1]->bSame("R35tenant"_Stz));
  
  i8 i_Token;
  
  // validate the create statament
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! m_xTokenVec[i_Token]->bHasText())
      break;
  }
  
  i8 iNumTokens = i_Token;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // create the job
  Job_CreateTenant* pJob_CreateTenant = new Job_CreateTenant(m_pSEitf);
  pJob_CreateTenant->m_sObjectName = m_xTokenVec[2]->aText();
  
  // set the job in SEitf
  m_pSEitf->m_pJob = pJob_CreateTenant;
  
  // create a schema
  m_xTokenVec[0]->vCopy("mktenR35"_Stz);
  m_xTokenVec[1]->vCopy(""_Stz);
  
  if (!bExecuteRequest())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop catalog                                                               */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_DropCatalogR35(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 i_Token;
  i8 iNumTokens;
  
  ASSERT(m_xTokenVec[0]->bSame("drop"_Stz));
  ASSERT(m_xTokenVec[1]->bSame("R35catalog"_Stz));
  
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! m_xTokenVec[i_Token]->bHasText())
      break;
  }
  
  iNumTokens = i_Token;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // create the job
  Job_DropCatalog* pJob_DropCatalog = new Job_DropCatalog(m_pSEitf);
  
  // set the table name
  pJob_DropCatalog->m_sObjectName = m_xTokenVec[2]->aText();
  
  // set the job in SEitf
  m_pSEitf->m_pJob = pJob_DropCatalog;
  
  // drop a schema
  m_xTokenVec[0]->vCopy("rmcatR35"_Stz);
  m_xTokenVec[1]->vCopy(""_Stz"");
  if (!bExecuteRequest())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop index                                                                 */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_DropIndexR35(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 i_Token;
  i8 iNumTokens;
  
  ASSERT(m_xTokenVec[0]->bSame("drop"_Stz));
  ASSERT(m_xTokenVec[1]->bSame("R35index"_Stz));
  
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! m_xTokenVec[i_Token]->bHasText())
      break;
  }
  
  iNumTokens = i_Token;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 5 tokens
  if (iNumTokens != 5) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // 4th token should be '.'
  if (!m_xTokenVec[3]->bSame("."_Stz)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // create the job
  Job_DropIndex* pJob_DropIndex = new Job_DropIndex(m_pSEitf);
  
  // set the table name
  pJob_DropIndex->m_sObjectName = m_xTokenVec[2]->aText();
  
  // set the job in SEitf
  m_pSEitf->m_pJob = pJob_DropIndex;
  
  // drop a table
  m_xTokenVec[0]->vCopy("rmidxR35"_Stz);
  m_xTokenVec[1]->vCopy(""_Stz);
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Table dropped: %s\n"_Stz, m_xTokenVec2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop schema                                                                */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_DropSchemaR35(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 i_Token;
  i8 iNumTokens;
  
  ASSERT(m_xTokenVec[0]->bSame("drop"_Stz));
  ASSERT(m_xTokenVec[1]->bSame("R35schema"_Stz));
  
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! m_xTokenVec[i_Token]->bHasText())
      break;
  }
  
  iNumTokens = i_Token;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // create the job
  Job_DropSchema* pJob_DropSchema = new Job_DropSchema(m_pSEitf);
  
  // set the table name
  pJob_DropSchema->m_sObjectName = m_xTokenVec[2]->aText();
  
  // set the job in SEitf
  m_pSEitf->m_pJob = pJob_DropSchema;
  
  // drop a schema
  m_xTokenVec[0]->vCopy("rmschR35"_Stz);
  m_xTokenVec[1]->vCopy(""_Stz"");
  if (!bExecuteRequest())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop table                                                                 */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_DropTableR35(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 i_Token;
  i8 iNumTokens;
  
  ASSERT(m_xTokenVec[0]->bSame("drop"_Stz));
  ASSERT(m_xTokenVec[1]->bSame("R35table"_Stz));
  
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! m_xTokenVec[i_Token]->bHasText())
      break;
  }
  
  iNumTokens = i_Token;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // create the job
  Job_DropTable* pJob_DropTable = new Job_DropTable(m_pSEitf);
  
  // set the table name
  pJob_DropTable->m_sObjectName = m_xTokenVec[2]->aText();
  
  // set the job in SEitf
  m_pSEitf->m_pJob = pJob_DropTable;
  
  // drop a table
  m_xTokenVec[0]->vCopy("rmtblR35"_Stz);
  m_xTokenVec[1]->vCopy(""_Stz);
  if (!bExecuteRequest())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop tenant                                                                */
/*----------------------------------------------------------------------------*/
bool SePromptR35::bCmd_DropTenantR35(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 i_Token;
  i8 iNumTokens;
  
  ASSERT(m_xTokenVec[0]->bSame("drop"_Stz));
  ASSERT(m_xTokenVec[1]->bSame("R35tenant"_Stz));
  
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! m_xTokenVec[i_Token]->bHasText())
      break;
  }
  
  iNumTokens = i_Token;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // create the job
  Job_DropTenant* pJob_DropTenant = new Job_DropTenant(m_pSEitf);
  
  // set the table name
  pJob_DropTenant->m_sObjectName = m_xTokenVec[2]->aText();
  
  // set the job in SEitf
  m_pSEitf->m_pJob = pJob_DropTenant;
  
  // drop a schema
  m_xTokenVec[0]->vCopy("rmtenR35"_Stz);
  m_xTokenVec[1]->vCopy(""_Stz"");
  if (!bExecuteRequest())
    return false;
  
  return true;
}
