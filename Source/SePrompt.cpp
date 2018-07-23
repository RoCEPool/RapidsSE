/*============================================================================*/
/* SePrompt.cpp -- se> prompt                                                 */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

extern AttachMap*    gpAttachMap;
extern BufferPool*   gpBufferPool;
extern Arguments     gxArguments;
extern pthread_key_t giTLS_ThreadKey;

PromptCatalog gxPromptCatalog;
extern JobCatalog gxJobCatalog;

//
// list of commands which need intercept processing by SePrompt
//
const Stz* C_CMD_ANTBL      = "antbl"_Stz;
const Stz* C_CMD_BEGIN      = "begin"_Stz;
const Stz* C_CMD_CREATE     = "create"_Stz;
const Stz* C_CMD_DANTBL     = "dantbl"_Stz;
const Stz* C_CMD_DOCOL      = "docol"_Stz;
const Stz* C_CMD_DISTPARLOAD = "dpld"_Stz;
const Stz* C_CMD_DROP       = "drop"_Stz;
const Stz* C_CMD_END        = "end"_Stz;
const Stz* C_CMD_FIRST      = "first"_Stz;
const Stz* C_CMD_INSERT     = "insert"_Stz;
const Stz* C_CMD_LOAD       = "ld"_Stz;
const Stz* C_CMD_NEXT       = "next"_Stz;
const Stz* C_CMD_USE        = "use"_Stz;

const Stz* C_ONE_CATALOG = "catalog"_Stz;
const Stz* C_ONE_INDEX   = "index"_Stz;
const Stz* C_ONE_INSERT  = "insert"_Stz;
const Stz* C_ONE_ROW     = "row"_Stz;
const Stz* C_ONE_SCHEMA  = "schema"_Stz;
const Stz* C_ONE_TABLE   = "table"_Stz;
const Stz* C_ONE_TENANT  = "tenant"_Stz;
const Stz* C_ONE_UNIQUE  = "unique"_Stz;

struct CmdTable {
  const Stz* aCommand;
  const Stz* aAlternate;
  bool (SePrompt::*aFunc)(void);
};

CmdTable SePromptCommands[] =
{
  { C_CMD_ANTBL,      nullptr,       &SePrompt::bCmd_AnalyzeTable  },
  { C_CMD_BEGIN,      C_ONE_INSERT,  &SePrompt::bCmd_BeginInsert   },
  { C_CMD_CREATE,     C_ONE_CATALOG, &SePrompt::bCmd_CreateCatalog },
  { C_CMD_CREATE,     C_ONE_INDEX,   &SePrompt::bCmd_CreateIndex   },
  { C_CMD_CREATE,     C_ONE_SCHEMA,  &SePrompt::bCmd_CreateSchema  },
  //{ C_CMD_CREATE,     C_ONE_TABLE,   &SePrompt::bCmd_CreateTable   },
  { C_CMD_CREATE,     C_ONE_TENANT,  &SePrompt::bCmd_CreateTenant  },
  { C_CMD_CREATE,     C_ONE_UNIQUE,  &SePrompt::bCmd_CreateUnique  },
  { C_CMD_DANTBL,     nullptr,       &SePrompt::bCmd_DanalyzeTable },
  //{ C_CMD_DOCOL,      nullptr,       &SePrompt::bCmd_Docol         },
  { C_CMD_DISTPARLOAD,nullptr,       &SePrompt::bCmd_DistParLoadTable},
  { C_CMD_DROP,       C_ONE_CATALOG, &SePrompt::bCmd_DropCatalog   },
  { C_CMD_DROP,       C_ONE_INDEX,   &SePrompt::bCmd_DropIndex     },
  { C_CMD_DROP,       C_ONE_SCHEMA,  &SePrompt::bCmd_DropSchema    },
  //{ C_CMD_DROP,       C_ONE_TABLE,   &SePrompt::bCmd_DropTable     },
  { C_CMD_DROP,       C_ONE_TENANT,  &SePrompt::bCmd_DropTenant    },
  { C_CMD_END,        C_ONE_INSERT,  &SePrompt::bCmd_EndInsert     },
  { C_CMD_INSERT,     C_ONE_ROW,     &SePrompt::bCmd_InsertRow     },
  { C_CMD_LOAD,       nullptr,       &SePrompt::bCmd_LoadTable     },
  { C_CMD_USE,        nullptr,       &SePrompt::bCmd_UseSchema     },
  
  { nullptr,          nullptr,       nullptr                              }
};

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* SePrompt::operator new(size_t iSize)
{
  RASSERT(iSize);
  SePrompt* pSePrompt = (SePrompt*)Heap::pAllocate(CN_SEPROMPT);
  
  return pPoolCheck<void*>(pSePrompt);
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void SePrompt::operator delete(void* pSePromptAddress)
{
  SePrompt* pSePrompt = static_cast<SePrompt*>(pSePromptAddress);
  
  Heap::vFree(pSePrompt);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SePrompt::SePrompt(void)
: Object(CN_SEPROMPT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iPartition = 0;

  eEC eResult = RapidsSE_SEitfNew((void**)&m_pSEitf,iPartition, /*trace*/ 0);
  ASSERT(EC_NONE == eResult);
  
  m_sResponse  = ""_Stz;
  
  m_pCommandProcessor = m_pSEitf->m_pCommandProcessor;
  m_pCommandProcessor->m_pPrompt = this;

  _checkInitialised_(CN_SEPROMPT);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SePrompt::~SePrompt(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  RapidsSE_SEitfDelete(m_pSEitf, /*trace*/ 0);
}

/*----------------------------------------------------------------------------*/
/* dump seprompt                                                              */
/*----------------------------------------------------------------------------*/
void SePrompt::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(eClass == CN_SEPROMPT);
  RASSERT(pObject->eClass() == CN_SEPROMPT);
  
  SePrompt* pSePrompt = reinterpret_cast<SePrompt*>(pObject);
  
  vTrace("SePrompt %p seitf %p \n"_Stz,
         pSePrompt,
         pSePrompt->m_pSEitf);
}

/*----------------------------------------------------------------------------*/
/*-- Task Primitives ---------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* prepare and dispatch job                                                   */
/*----------------------------------------------------------------------------*/
/*
bool SePrompt::bPrepareJob(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for echo prefix
  //if (bCmdIs("echo"_Stz))
  //  vSetEcho();
  //else
  //  vClearEcho();
  
  //vTrace("command: %s\n"_Stz,pTkn(0)->sText().aChar());
  
  for (i8 i_Cmd = 0; SePromptCommands[i_Cmd].aCommand; i_Cmd++) {
    
    if ( ! bCmdIs((Stz*)SePromptCommands[i_Cmd].aCommand))
      continue;
    
    if (! SePromptCommands[i_Cmd].aAlternate)
      return (this->*SePromptCommands[i_Cmd].aFunc)();
    
    if (bOneIs((Stz*)SePromptCommands[i_Cmd].aAlternate)) {
      return (this->*SePromptCommands[i_Cmd].aFunc)();
    }
  }
 
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for command file
  //if (bTestFile(pTkn(0)->aText()))
  //  return bRunFile();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // execute seitf request
  if (! m_pCommandProcessor->bDispatchJob())
    return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // post process requests
  //if (! bPostProcess)
  //return true;
  
  if ( bCmdIs("fetch"_Stz) || bCmdIs("dfetch"_Stz)) {
    vIterateBuffers();
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // the following never return
  
  if (pTkn(0)->bSame("mon"_Stz)) {
    while (true) {
      PosixThread::vSleepSec(10);
      if (!bExecuteRequest())
        return true;
    }
  }
  
  if (pTkn(0)->bSame("x"_Stz)) {
    while (true)
      bExecuteRequest();
  }
  
  return true;
}
*/

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
void SePrompt::vExecuteCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // attempt to execute command file
  if (bExecuteFile()) {
    m_pCommandProcessor->m_eCommand = CK_COMMAND_FILE;
    return;
  }
  
  // attempt to execute internal command
  if (bExecuteFunction()) {
    m_pCommandProcessor->m_eCommand = CK_PROMPT_FUNCTION;
    return;
  }
  
  // attempt to execute se job
  if (m_pCommandProcessor->bFindJobClass()) {
    m_pCommandProcessor->m_eCommand = CK_SE_JOB;
    return;
  }
 /*
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  bool bProcessResult = bProcessCommand();
  
  if (m_bEcho) {
    vTrace("cmd: "_Stz);
    for (i8 i_Token = 0; i_Token < C_TOKENS; i_Token++) {
      if ( ! pTkn(i_Token)->bHasText() )
        break;
      vTrace("%s "_Stz,pTkn(i_Token));
    }
    vTrace(": got: %s\n"_Stz,m_sResponse.aChar());
    return;
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
  */
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get the seitf                                                              */
/*----------------------------------------------------------------------------*/
SEitf* SePrompt::pSEitf(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return m_pSEitf;
}

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* clip tokens                                                                */
/*----------------------------------------------------------------------------*/
void SePrompt::vClipTokens(i8 i_FromToken)
{
  i8 i_ToToken = 0;
  for ( ; i_FromToken < C_TOKENS; i_FromToken++) {
    
    pTkn(i_ToToken++)->vCopy(pTkn(i_FromToken));
    
    if ( ! pTkn(i_FromToken)->bHasText() )
      break;
  }
}

/*----------------------------------------------------------------------------*/
/* check if command token 0 is given argument                                 */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCmdIs(Stz* aArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  if (pTkn(0)->bSame(aArg))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* potentially execute a command file                                         */
/*----------------------------------------------------------------------------*/
bool SePrompt::bExecuteFile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  if (! bTestFile(pTkn(0)->aText()))
    return false;
  
  bool b = bRunFile();
  ASSERT(b);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* potentially execute a prompt function                                      */
/*----------------------------------------------------------------------------*/
bool SePrompt::bExecuteFunction(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  PromptFun* pPromptFun =
    gxPromptCatalog.pFindFun(pTkn(0)->aText(), pTkn(1)->aText());
  
  if (! pPromptFun)
    return false;
  
  (*pPromptFun->m_fPrompt)(this);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* print output in metadata format                                            */
/*----------------------------------------------------------------------------*/
void SePrompt::vMetaFormat(void)
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
/* check if command token 1 is given argument                                 */
/*----------------------------------------------------------------------------*/
bool SePrompt::bOneIs(Stz* aArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  if (pTkn(1)->bSame(aArg))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* push command file                                                          */
/*----------------------------------------------------------------------------*/
void SePrompt::vPushFile(Stz* aFileName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  s256 sPath;
  
  sPath = aFileName;
  
  m_pCommandProcessor->m_pTokeniser->vPush();
  
  bool b = m_pCommandProcessor->m_pTokeniser->bUseFile(sPath);
  ASSERT(b);
}

/*----------------------------------------------------------------------------*/
/* set file to run                                                            */
/*----------------------------------------------------------------------------*/
bool SePrompt::bRunFile(void)
{
  if ( ! bTokenCheck(1) )
    return false;
  
  vPushFile(pTkn(0)->aText());
  m_pCommandProcessor->m_pTokeniser->vLineMode(true);
  m_sResponse = "<ok>"_Stz;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* test for command file                                                      */
/*----------------------------------------------------------------------------*/
bool SePrompt::bTestFile(Stz* aFileName)
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
bool SePrompt::bTokenCheck(i8 nTokens)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  if (pTkn(nTokens)->bHasText()) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get token vector                                                           */
/*----------------------------------------------------------------------------*/
TokenVec* SePrompt::pTknVec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return &m_pCommandProcessor->m_xTokenVec;
}

/*----------------------------------------------------------------------------*/
/* get second token vector                                                    */
/*----------------------------------------------------------------------------*/
TokenVec* SePrompt::pTknVec2(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  return &m_pCommandProcessor->m_xTokenVec2;
}

/*----------------------------------------------------------------------------*/
/* get token from token vector                                                */
/*----------------------------------------------------------------------------*/
Token* SePrompt::pTkn(i8 i_Token)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  TokenVec* pTknVec = this->pTknVec();
  
  Token* pTkn = (*pTknVec)[i_Token];
  
  return pTkn;
}

/*----------------------------------------------------------------------------*/
/* get token from second token vector                                         */
/*----------------------------------------------------------------------------*/
Token* SePrompt::pTkn2(i8 i_Token)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  TokenVec* pTknVec2 = this->pTknVec2();
  
  Token* pTkn = (*pTknVec2)[i_Token];
  
  return pTkn;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SePrompt::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT);
  
  return &m_xFooter;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SePrompt* Prompt::_pSePrompt(void* aAddr)
{
  SePrompt* pSePrompt = reinterpret_cast<SePrompt*>(aAddr);
  
  RASSERT( !pSePrompt || CN_SEPROMPT == pSePrompt->eClass() );
  
  return pSePrompt;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/








/*----------------------------------------------------------------------------*/
/* execute an se request                                                      */
/*----------------------------------------------------------------------------*/
bool SePrompt::bExecuteRequest(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Timer xTimer;
  Stz* aResponse = nullptr;
  
  xTimer.vStart();
  
  pTknVec()->vSetTokens();
  
  //! make this part of verbose??
  //m_pCommandProcessor->m_pTokeniser->vDumpTokens("Command : "_Stz,m_xTokenVec,C_TOKENS);
  
  TokenVec* pTokenVec = pTknVec();

  bool bResult = RapidsSE_ExecuteRequest(m_pSEitf,*pTokenVec,&aResponse);
  
  if (pTknVec()->bDetachedRequest())
    return bResult;
  
  if (bResult)
    m_sResponse = aResponse ? (Stz*)aResponse : ""_Stz;
  else
    m_sResponse = aResponse ? (Stz*)aResponse : ""_Stz;
  
  xTimer.vUpdate();
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* process command                                                            */
/*----------------------------------------------------------------------------*/
bool SePrompt::bProcessCommand(bool bPostProcess)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  ASSERT(0);
  
  
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // test for echo prefix
//  if (bCmdIs("echo"_Stz))
//    vSetEcho();
//  else
//    vClearEcho();
//
//  //vTrace("command: %s\n"_Stz,pTkn(0)->sText().aChar());
//  /*
//  for (i8 i_Cmd = 0; SePromptCommands[i_Cmd].aCommand; i_Cmd++) {
//
//    if ( ! bCmdIs((Stz*)SePromptCommands[i_Cmd].aCommand))
//      continue;
//
//    if (! SePromptCommands[i_Cmd].aAlternate)
//      return (this->*SePromptCommands[i_Cmd].aFunc)();
//
//    if (bOneIs((Stz*)SePromptCommands[i_Cmd].aAlternate)) {
//      return (this->*SePromptCommands[i_Cmd].aFunc)();
//    }
//  }
//  */
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // test for command file
//  if (bTestFile(pTkn(0)->aText()))
//    return bRunFile();
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // execute seitf request
//  if (! bExecuteRequest())
//    return false;
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // post process requests
//  if (! bPostProcess)
//    return true;
//
//  if ( bCmdIs("fetch"_Stz) || bCmdIs("dfetch"_Stz)) {
//    vIterateBuffers();
//    return false;
//  }
//
//  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
//  // the following never return
//
//  if (pTkn(0)->bSame("mon"_Stz)) {
//    while (true) {
//      PosixThread::vSleepSec(10);
//      if (!bExecuteRequest())
//        return true;
//    }
//  }
//
//  if (pTkn(0)->bSame("x"_Stz)) {
//    while (true)
//      bExecuteRequest();
//  }
//
  return true;
}


/*----------------------------------------------------------------------------*/
/* return table                                                               */
/*----------------------------------------------------------------------------*/
bool SePrompt::bReturnTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  pTkn(0)->vCopy("rettbl"_Stz);
  pTkn(1)->vCopy(""_Stz);
  
  if (!bExecuteRequest()) {
    vTrace("bReturnTable: (%s) Return Table FAILED!!! : %s\n"_Stz, m_sResponse.aChar());
    return false;
  }
  return true;
}


/*----------------------------------------------------------------------------*/
/*-- Intercepted Commands ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* analyze table                                                              */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCmd_AnalyzeTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  
  s256 sDir;
  Dir xDir;
  
  ASSERT(pTkn(0)->bSame("antbl"_Stz));
  
  return bExecuteRequest();
}

/*----------------------------------------------------------------------------*/
/* begin insert                                                               */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCmd_BeginInsert(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(pTkn(0)->bSame("begin"_Stz));
  ASSERT(pTkn(1)->bSame("insert"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy tokens from m_xTokenVec to m_xTokenVec2
  i8 iNumTokens = iCopyTokens(pTknVec(), pTknVec2());
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // get table pointer
  pTkn(0)->vCopy("gettbl"_Stz);
  pTkn(1)->vCopy(pTkn2(2));
  if (!bExecuteRequest()) {
    vTrace("SePrompt::bBeginInsert : gettbl FAILED!!!\n"_Stz);
    return false;
  }
  
  // begin a transaction
  pTkn(0)->vCopy("txBeg"_Stz);
  pTkn(1)->vCopy(""_Stz);
  if (!bExecuteRequest()) {
     vTrace("SePrompt::bBeginInsert : txBegin FAILED!!!\n"_Stz);
     m_pSEitf->vSetLastError(EC_TX_BEGIN_FAILED);
    return false;
  }
  
  m_sResponse = "<ok>"_Stz;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* create catalog                                                             */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCmd_CreateCatalog(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(pTkn(0)->bSame("create"_Stz));
  ASSERT(pTkn(1)->bSame("catalog"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy tokens from m_xTokenVec to m_xTokenVec2
  i8 iNumTokens = iCopyTokens(pTknVec(), pTknVec2());
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // create a catalog
  pTkn(0)->vCopy("mkcat"_Stz);
  pTkn(1)->vCopy(pTkn2(2));
  
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Catalog created: %s\n"_Stz, pTkn2(2));
  return true;
}

/*----------------------------------------------------------------------------*/
/* create index                                                               */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCmd_CreateIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iIfUnique = 0;
  
  ASSERT(pTkn(0)->bSame("create"_Stz));
  
  // copy tokens from m_xTokenVec to m_xTokenVec2
  i8 iNumTokens = iCopyTokens(pTknVec(), pTknVec2());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // some validations
  bool bValid = bValidateCreateIndex(iNumTokens);
  if ( ! bValid )
    return false;
  
  if (pTkn2(1)->bSame("unique"_Stz))
    iIfUnique = 1;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // get reference to pTable
  pTkn(0)->vCopy("gettbl"_Stz);
  pTkn(1)->vCopy(pTkn2(4 + iIfUnique));
  
  if (!bExecuteRequest()) {
    vTrace("SePrompt::bCreateIndex : gettbl FAILED!!!\n"_Stz);
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the index
  
  pTkn(0)->vCopy("mkidx"_Stz);
  pTkn(1)->vCopy(pTkn2(4 + iIfUnique));
  pTkn(2)->vCopy(pTkn2(2 + iIfUnique));
  
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
    
    if (iCurrentToken != 6 + iIfUnique && !pTkn2(iCurrentToken++)->bSame(","_Stz)) {
      bIndexReturnTable(pTkn2(4 + iIfUnique)->aText(),
                        pTkn2(2 + iIfUnique)->aText());
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      return false;
    }
    
    // add columns to the table
    i8 j = 0;
    pTkn(j++)->vCopy("addidxcol"_Stz);
    pTkn(j++)->vCopy(pTkn2(4 + iIfUnique));
    pTkn(j++)->vCopy(pTkn2(2 + iIfUnique));
    pTkn(j++)->vCopy(pTkn2(iCurrentToken++));
    
    // add the column
    if (!bExecuteRequest()) {
      
      // save error returned by addColumn
      eEC eError = m_pSEitf->eGetLastError();
      
      // drop the table that was created
      bIndexReturnTable(pTkn2(4 + iIfUnique)->aText(),
                        pTkn2(2 + iIfUnique)->aText());
      
      // return error returned by addColumn
      m_pSEitf->vSetLastError(eError);
      return false;
    }
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // uniqueness
  
  if (iIfUnique) {
    
    pTkn(0)->vCopy("setidxunique"_Stz);
    pTkn(1)->vCopy(pTkn2(4 + iIfUnique));
    pTkn(2)->vCopy(pTkn2(2 + iIfUnique));
    pTkn(3)->vCopy("true"_Stz);
    pTkn(4)->vCopy(""_Stz);
    
    if (!bExecuteRequest()) {
      vTrace("SePrompt::bCreateIndex : setidxunique FAILED!!!\n"_Stz);
      // save error returned by addColumn
      eEC eError = m_pSEitf->eGetLastError();
      
      // drop the table that was created
      bIndexReturnTable(pTkn2(4 + iIfUnique)->aText(),
                        pTkn2(2 + iIfUnique)->aText());
      
      // return error returned by addColumn
      m_pSEitf->vSetLastError(eError);
      return false;
    }
    
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // end of make index
  
  pTkn(0)->vCopy("endmkidx"_Stz);
  pTkn(1)->vCopy(pTkn2(4 + iIfUnique));
  pTkn(2)->vCopy(pTkn2(2 + iIfUnique));
  pTkn(3)->vCopy(""_Stz);
  
  if (!bExecuteRequest()) {
    //vTrace("SePrompt::bCreateIndex : endmkidx FAILED!!!\n"_Stz);
    // save error returned by addColumn
    eEC eError = m_pSEitf->eGetLastError();
    
    // drop the table that was created
    bIndexReturnTable(pTkn2(4 + iIfUnique)->aText(),
                      pTkn2(2 + iIfUnique)->aText());
    
    // return error returned by addColumn
    m_pSEitf->vSetLastError(eError);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* create schema                                                              */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCmd_CreateSchema(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(pTkn(0)->bSame("create"_Stz));
  ASSERT(pTkn(1)->bSame("schema"_Stz));
  
  i8 i_Token;
  
  // copy tokens from m_xTokenVec to m_xTokenVec2
  // while doing that also validate the create statament
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! pTkn(i_Token)->bHasText())
      break;
    pTkn2(i_Token)->vCopy(pTkn(i_Token));
    pTkn(i_Token)->vCopy(""_Stz);
  }
  
  i8 iNumTokens = i_Token;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // create a schema
  pTkn(0)->vCopy("mksch"_Stz);
  pTkn(1)->vCopy(pTkn2(2));
  
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Schema created: %s\n"_Stz, pTkn2(2));
  return true;
}

/*----------------------------------------------------------------------------*/
/* create tenant                                                              */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCmd_CreateTenant(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(pTkn(0)->bSame("create"_Stz));
  ASSERT(pTkn(1)->bSame("tenant"_Stz));
  
  i8 i_Token;
  
  // copy tokens from m_xTokenVec to m_xTokenVec2
  // while doing that also validate the create statament
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! pTkn(i_Token)->bHasText())
      break;
    pTkn2(i_Token)->vCopy(pTkn(i_Token));
    pTkn(i_Token)->vCopy(""_Stz);
  }
  
  i8 iNumTokens = i_Token;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // create a tenant
  pTkn(0)->vCopy("mkten"_Stz);
  pTkn(1)->vCopy(pTkn2(2));
  
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Tenant created: %s\n"_Stz, pTkn2(2));
  return true;
}

/*----------------------------------------------------------------------------*/
/* create index                                                               */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCmd_CreateUnique(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(pTkn(0)->bSame("create"_Stz));
  ASSERT(pTkn(1)->bSame("unique"_Stz));
  
  return bCmd_CreateIndex();
}

/*----------------------------------------------------------------------------*/
/* detailed analyze table                                                     */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCmd_DanalyzeTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  s256 sDir;
  Dir xDir;
  
  ASSERT(pTkn(0)->bSame("dantbl"_Stz));
  
  return bExecuteRequest();
}

/*----------------------------------------------------------------------------*/
/* docol                                                                      */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCmd_Docol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(pTkn(0)->bSame("docol"_Stz));
  return bExecuteRequest();
}

/*----------------------------------------------------------------------------*/
/* load a table from a file  (distributed and parallel)                       */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCmd_DistParLoadTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  bool bResult = false;
  i8 iIndex;
  i8 iCurrentIndex;
  bool bSameMachine = false;
  
  ASSERT(pTkn(0)->bSame("dpld"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy tokens from m_xTokenVec to m_xTokenVec2
  i8 iNumTokens = iCopyTokens(pTknVec(), pTknVec2());
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be at least 3 tokens
  if (iNumTokens < 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    goto resetFormatter;
  }
  
  iIndex = 0;
  pTkn(iIndex++)->vCopy(pTkn2(0));
  pTkn(iIndex++)->vCopy(pTkn2(1));
  pTkn(iIndex++)->vCopy(pTkn2(2));
  
  iCurrentIndex = 3;
  
  while (iNumTokens > iCurrentIndex) {
    
    //--------------------------------------------------------------------------
    // C_FIELD_DELIMITER
    if (pTkn2(iCurrentIndex)->bSame(C_FIELD_DELIMITER)) {
      
      if (iNumTokens < iCurrentIndex + 2) {
        m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
        goto resetFormatter;
      }
      
      m_pSEitf->m_xFormatters.m_cFieldDelimiter = *pTkn2(++iCurrentIndex)->sText().aChar();
      if (m_pSEitf->m_xFormatters.m_cFieldDelimiter == '0')
        m_pSEitf->m_xFormatters.m_cFieldDelimiter = '\0';
    }
    //--------------------------------------------------------------------------
    // C_SAME_MACHINE
    else if (pTkn2(iCurrentIndex)->bSame(C_SAME_MACHINE)) {
      bSameMachine = true;
    }
    else {
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      goto resetFormatter;
    }
    iCurrentIndex++;
  }
  
  pTkn(iIndex++)->vCopy(bSameMachine ? "1"_Stz : "0"_Stz);
  
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
bool SePrompt::bCmd_DropCatalog(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 i_Token;
  i8 iNumTokens;
  
  ASSERT(pTkn(0)->bSame("drop"_Stz));
  ASSERT(pTkn(1)->bSame("catalog"_Stz));
  
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! pTkn(i_Token)->bHasText())
      break;
    pTkn2(i_Token)->vCopy(pTkn(i_Token));
    pTkn(i_Token)->vCopy(""_Stz);
  }
  
  iNumTokens = i_Token;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a catalog
  pTkn(0)->vCopy("rmcat"_Stz);
  pTkn(1)->vCopy(pTkn2(2));
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Catalog dropped: %s\n"_Stz, pTkn2(2));
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop index                                                                 */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCmd_DropIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 i_Token;
  i8 iNumTokens;
  
  ASSERT(pTkn(0)->bSame("drop"_Stz));
  ASSERT(pTkn(1)->bSame("index"_Stz));
  
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! pTkn(i_Token)->bHasText())
      break;
    pTkn2(i_Token)->vCopy(pTkn(i_Token));
    pTkn(i_Token)->vCopy(""_Stz);
  }
  
  iNumTokens = i_Token;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 5 tokens
  if (iNumTokens != 5) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // 4th token should be '.'
  if (!pTkn2(3)->bSame("."_Stz)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a table
  pTkn(0)->vCopy("rmidx"_Stz);
  pTkn(1)->vCopy(pTkn2(2));
  pTkn(2)->vCopy(pTkn2(4));
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Table dropped: %s\n"_Stz, pTkn2(2));
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop schema                                                                */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCmd_DropSchema(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 i_Token;
  i8 iNumTokens;
  
  ASSERT(pTkn(0)->bSame("drop"_Stz));
  ASSERT(pTkn(1)->bSame("schema"_Stz));
  
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! pTkn(i_Token)->bHasText())
      break;
    pTkn2(i_Token)->vCopy(pTkn(i_Token));
    pTkn(i_Token)->vCopy(""_Stz);
  }
  
  iNumTokens = i_Token;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a schema
  pTkn(0)->vCopy("rmsch"_Stz);
  pTkn(1)->vCopy(pTkn2(2));
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Schema dropped: %s\n"_Stz, pTkn2(2));
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop tenant                                                                */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCmd_DropTenant(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 i_Token;
  i8 iNumTokens;
  
  ASSERT(pTkn(0)->bSame("drop"_Stz));
  ASSERT(pTkn(1)->bSame("tenant"_Stz));
  
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! pTkn(i_Token)->bHasText())
      break;
    pTkn2(i_Token)->vCopy(pTkn(i_Token));
    pTkn(i_Token)->vCopy(""_Stz);
  }
  
  iNumTokens = i_Token;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a tenant
  pTkn(0)->vCopy("rmten"_Stz);
  pTkn(1)->vCopy(pTkn2(2));
  if (!bExecuteRequest()) {
    return false;
  }
  
  //vTrace("Tenant dropped: %s\n"_Stz, pTkn2(2));
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* end insert                                                                 */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCmd_EndInsert(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 i_Token;
  i8 iNumTokens;
  
  ASSERT(pTkn(0)->bSame("end"_Stz));
  ASSERT(pTkn(1)->bSame("insert"_Stz));
  
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! pTkn(i_Token)->bHasText())
      break;
    pTkn2(i_Token)->vCopy(pTkn(i_Token));
    pTkn(i_Token)->vCopy(""_Stz);
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
  pTkn(0)->vCopy("txCmt"_Stz);
  if (!bExecuteRequest()) {
    vTrace("SePrompt::EndInsert : txCmt FAILED!!!\n"_Stz);
    m_pSEitf->vSetLastError(EC_TX_COMMIT_FAILED);
    return false;
  }
  
  // begin a transaction
  pTkn(0)->vCopy("txEnd"_Stz);
  if (!bExecuteRequest()) {
    vTrace("SePrompt::EndInsert : txEnd FAILED!!!\n"_Stz);
    m_pSEitf->vSetLastError(EC_TX_END_FAILED);
    return false;
  }
  
  m_sResponse = "<ok>"_Stz;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* help                                                                       */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCmd_Help(void)
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
/* insert a row                                                               */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCmd_InsertRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 i_Token;
  i8 iNumTokens;
  bool bBlankLine = false;
  
  ASSERT(pTkn(0)->bSame("insert"_Stz));
  ASSERT(pTkn(1)->bSame("row"_Stz));
  
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! pTkn(i_Token)->bHasText())
      break;
    pTkn2(i_Token)->vCopy(pTkn(i_Token));
    pTkn(i_Token)->vCopy(""_Stz);
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
  
  Stz* pRow = pTkn2(2)->aText();
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
bool SePrompt::bCmd_LoadTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  bool bResult = false;
  i8 iIndex;
  i8 iCurrentIndex;
  
  ASSERT(pTkn(0)->bSame("ld"_Stz));
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy tokens from m_xTokenVec to m_xTokenVec2
  i8 iNumTokens = iCopyTokens(pTknVec(), pTknVec2());

  // - - - - - - - - - - - - - - - - - - - - -
  // there should be at least 3 tokens
  if (iNumTokens < 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    goto resetFormatter;
  }
  
  iIndex = 0;
  pTkn(iIndex++)->vCopy(pTkn2(0));
  pTkn(iIndex++)->vCopy(pTkn2(1));
  pTkn(iIndex++)->vCopy(pTkn2(2));
  
  iCurrentIndex = 3;
    
  while (iNumTokens > iCurrentIndex) {
    
    if (iNumTokens < iCurrentIndex + 2) {
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      goto resetFormatter;
    }
    
    //--------------------------------------------------------------------------
    // C_RECORD_TERMINATOR
    if (pTkn2(iCurrentIndex)->bSame(C_RECORD_TERMINATOR)) {
      m_pSEitf->m_xFormatters.m_cRecordTerminator = *pTkn2(++iCurrentIndex)->sText().aChar();
      if (m_pSEitf->m_xFormatters.m_cRecordTerminator == '0')
        m_pSEitf->m_xFormatters.m_cRecordTerminator = '\0';
    }
    //--------------------------------------------------------------------------
    // C_RECORD_ENDS
    else if (pTkn2(iCurrentIndex)->bSame(C_RECORD_ENDS)) {
      char cRecordEnds = *pTkn2(++iCurrentIndex)->sText().aChar();
      
      // is record_ends 1 character ?
      if (pTkn2(iCurrentIndex)->iLength() == 1) {
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
      else if (pTkn2(iCurrentIndex)->iLength() == 2) {
        
        if (pTkn2(iCurrentIndex)->bSame("\r\n"_Stz)) {
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
    else if (pTkn2(iCurrentIndex)->bSame(C_FIELD_DELIMITER)) {
      m_pSEitf->m_xFormatters.m_cFieldDelimiter = *pTkn2(++iCurrentIndex)->sText().aChar();
      if (m_pSEitf->m_xFormatters.m_cFieldDelimiter == '0')
        m_pSEitf->m_xFormatters.m_cFieldDelimiter = '\0';
    }
    //--------------------------------------------------------------------------
    // C_ENCLOSED_BY_LEADING
    else if (pTkn2(iCurrentIndex)->bSame(C_ENCLOSED_BY_LEADING)) {
      m_pSEitf->m_xFormatters.m_cEnclosedBy_Leading =
        *pTkn2(++iCurrentIndex)->sText().aChar();
      if (m_pSEitf->m_xFormatters.m_cEnclosedBy_Leading == '0')
        m_pSEitf->m_xFormatters.m_cEnclosedBy_Leading = '\0';
      
      // default set trailing same as leading
      m_pSEitf->m_xFormatters.m_cEnclosedBy_Trailing =
        m_pSEitf->m_xFormatters.m_cEnclosedBy_Leading;
    }
    //--------------------------------------------------------------------------
    // C_ENCLOSED_BY_TRAILING
    else if (pTkn2(iCurrentIndex)->bSame(C_ENCLOSED_BY_TRAILING)) {
      
      // if trailing is specified, leading should also have been specified
      if (m_pSEitf->m_xFormatters.m_cEnclosedBy_Leading == '\0') {
        if ( ! gpAttachMap->m_bMetadata)
          vWarn("Invalid load : enclosed_by_trailing specified but no enclosed_by_leading\n"_Stz);
        m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
        goto resetFormatter;
      }
      m_pSEitf->m_xFormatters.m_cEnclosedBy_Trailing =
        *pTkn2(++iCurrentIndex)->sText().aChar();
      if (m_pSEitf->m_xFormatters.m_cEnclosedBy_Trailing == '0')
        m_pSEitf->m_xFormatters.m_cEnclosedBy_Trailing = '\0';
    }
//
//    DATE_FORMAT='YYYY.MM.DD’
//
//
    //--------------------------------------------------------------------------
    // C_DATE_FORMAT
    else if (pTkn2(iCurrentIndex)->bSame(C_DATE_FORMAT)) {
      // extract date format
      if ( ! m_pSEitf->bExtractDateFormat(pTkn2(++iCurrentIndex)->sText()) ) {
        if ( ! gpAttachMap->m_bMetadata)
          vWarn("Invalid load : enclosed_by_trailing specified but no enclosed_by_leading\n"_Stz);
        m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
        goto resetFormatter;
      }
    }
    
    //--------------------------------------------------------------------------
    // C_ESCAPE_CHAR
    else if (pTkn2(iCurrentIndex)->bSame(C_ESCAPE_CHAR)) {
      m_pSEitf->m_xFormatters.m_cEscapeChar = *pTkn2(++iCurrentIndex)->sText().aChar();
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
/* print working directory                                                    */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCmd_PrintDir(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Dir xDir;
  s256 sDir;
  
  ASSERT(pTkn(0)->bSame("pwd"_Stz));
  
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
/* use schema                                                                 */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCmd_UseSchema(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 i_Token;
  i8 iNumTokens;
  
  ASSERT(pTkn(0)->bSame("use"_Stz));
  ASSERT(pTkn(1)->bSame("schema"_Stz));
  
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! pTkn(i_Token)->bHasText())
      break;
    pTkn2(i_Token)->vCopy(pTkn(i_Token));
    pTkn(i_Token)->vCopy(""_Stz);
  }
  
  iNumTokens = i_Token;
  
  // - - - - - - - - - - - - - - - - - - - - -
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a schema
  pTkn(0)->vCopy("usesch"_Stz);
  pTkn(1)->vCopy(pTkn2(2));
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Schema changed: %s\n"_Stz, pTkn2(2));
  return true;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* copy tokens from a token vector to another                                 */
/*----------------------------------------------------------------------------*/
i8 SePrompt::iCopyTokens(TokenVec* pFromTokenVec, TokenVec* pToTokenVec)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // copy tokens from m_xTokenVec to m_xTokenVec2
  // while doing that also validate the create statament
  i8 i_Token;
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! (*pFromTokenVec)[i_Token]->bHasText())
      break;
    (*pToTokenVec)[i_Token]->vCopy((*pFromTokenVec)[i_Token]);
    (*pFromTokenVec)[i_Token]->vCopy(""_Stz);
  }
  
  return i_Token;
}

/*----------------------------------------------------------------------------*/
/* drop return table                                                          */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDropReturnTable(Stz* aTableName)
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
/* index return table                                                         */
/*----------------------------------------------------------------------------*/
bool SePrompt::bIndexReturnTable(Stz* aTableName, Stz* aIndexName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // derefernce and return the table first
  bool bResult = bReturnTable();
  ASSERT(bResult);
  
  pTkn(0)->vCopy("rmidx"_Stz);
  pTkn(1)->vCopy(aTableName);
  pTkn(2)->vCopy(aIndexName);
  pTkn(3)->vCopy(""_Stz);
  
  if (!bExecuteRequest()) {
    //vFatal("Error: (%s) Drop Table %s\n"_Stz, m_sResponse.aChar(), pTableName);
    return false;
  }
  return true;
}

/*----------------------------------------------------------------------------*/
/* iterate entry buffers frim the seitf queue                                 */
/*----------------------------------------------------------------------------*/
void SePrompt::vIterateBuffers(void)
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
/* remove a table                                                             */
/*----------------------------------------------------------------------------*/
bool SePrompt::bRemoveTable(Stz* pTableName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // create the table
  pTkn(0)->vCopy("rmtbl"_Stz);
  pTkn(1)->vCopy(pTableName);
  pTkn(2)->vCopy(""_Stz);
  
  if (!bExecuteRequest()) {
    return false;
  }
  
  return true;
}
  
/*----------------------------------------------------------------------------*/
/* validate a create index request                                            */
/*----------------------------------------------------------------------------*/
bool SePrompt::bValidateCreateIndex(i8 iNumTokens)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iIfUnique = 0;
  
  if (pTkn2(1)->bSame("unique"_Stz)) {
    iIfUnique = 1;
    if ( ! pTkn2(2)->bSame("index"_Stz)) {
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      return false;
    }
  }
  else {
    if ( ! pTkn2(1)->bSame("index"_Stz)) {
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
  if (!pTkn2(3 + iIfUnique)->bSame("on"_Stz)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }

  // token[5] should be '('
  if (!pTkn2(5 + iIfUnique)->bSame("("_Stz)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }

  // last-1 token should be ')'
  if (!pTkn2(iNumTokens - 1)->bSame(")"_Stz)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* validate a create index request                                            */
/*----------------------------------------------------------------------------*/
bool SePrompt::bValidateCreateIndexR35(i8 iNumTokens)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iIfUnique = 0;
  
  if (pTkn(1)->bSame("unique"_Stz)) {
    iIfUnique = 1;
    if ( ! pTkn(2)->bSame("R35index"_Stz)) {
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      return false;
    }
  }
  else {
    if ( ! pTkn(1)->bSame("R35index"_Stz)) {
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
  if (!pTkn(3 + iIfUnique)->bSame("on"_Stz)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // token[5] should be '('
  if (!pTkn(5 + iIfUnique)->bSame("("_Stz)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last-1 token should be ')'
  if (!pTkn(iNumTokens - 1)->bSame(")"_Stz)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* create catalog                                                             */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCmd_CreateCatalogR35(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(pTkn(0)->bSame("create"_Stz));
  ASSERT(pTkn(1)->bSame("R35catalog"_Stz));
  
  i8 i_Token;
  
  // validate the create statament
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! pTkn(i_Token)->bHasText())
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
  pJob_CreateCatalog->m_sObjectName = pTkn(2)->aText();
  
  // set the job in SEitf
  m_pSEitf->m_pJob = pJob_CreateCatalog;
  
  // create a schema
  pTkn(0)->vCopy("mkcatR35"_Stz);
  pTkn(1)->vCopy(""_Stz);
  
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Schema created: %s\n"_Stz, pTkn2(2));
  return true;
}

/*----------------------------------------------------------------------------*/
/* create index                                                               */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCmd_CreateIndexR35(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iIfUnique = 0;
  
  ASSERT(pTkn(0)->bSame("create"_Stz));
  
  // validate the create statament
  i8 i_Token;
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! pTkn(i_Token)->bHasText())
      break;
  }
  
  i8 iNumTokens = i_Token;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // some validations
  bool bValid = bValidateCreateIndexR35(iNumTokens);
  if ( ! bValid )
    return false;
  
  if (pTkn(1)->bSame("unique"_Stz))
    iIfUnique = 1;
  
  
  // create the job
  Job_CreateIndex* pJob_CreateIndex = new Job_CreateIndex(m_pSEitf);
  
  // set the index name
  pJob_CreateIndex->m_sObjectName = pTkn(2 + iIfUnique)->aText();
  
  // set the table name
  pJob_CreateIndex->m_sTableName = pTkn(4 + iIfUnique)->aText();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // create the index columns
  
  // now collect the column names and their types
  i8 iCurrentToken = 6 + iIfUnique;
  i8 iColumn = 0;
  while (iCurrentToken < iNumTokens - 1) {
    
    if (iCurrentToken != 6 + iIfUnique && !pTkn2(iCurrentToken++)->bSame(","_Stz)) {
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      return false;
    }
    pJob_CreateIndex->m_sColumnName[iColumn] = pTkn(iCurrentToken++)->aText();
  }
  pJob_CreateIndex->m_iColumns = iColumn;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // uniqueness
  
  if (iIfUnique) {
    pJob_CreateIndex->m_bIsUnique = true;
  }
  
  // create a schema
  pTkn(0)->vCopy("mkidxR35"_Stz);
  pTkn(1)->vCopy(""_Stz);
  
  if (!bExecuteRequest())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* create schema                                                              */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCmd_CreateSchemaR35(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(pTkn(0)->bSame("create"_Stz));
  ASSERT(pTkn(1)->bSame("R35schema"_Stz));
  
  i8 i_Token;
  
  // validate the create statament
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! pTkn(i_Token)->bHasText())
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
  pJob_CreateSchema->m_sObjectName = pTkn(2)->aText();
  
  // set the job in SEitf
  m_pSEitf->m_pJob = pJob_CreateSchema;
  
  // create a schema
  pTkn(0)->vCopy("mkschR35"_Stz);
  pTkn(1)->vCopy(""_Stz);
  
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Schema created: %s\n"_Stz, pTkn2(2));
  return true;
}

/*----------------------------------------------------------------------------*/
/* create tenant                                                              */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCmd_CreateTenantR35(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(pTkn(0)->bSame("create"_Stz));
  ASSERT(pTkn(1)->bSame("R35tenant"_Stz));
  
  i8 i_Token;
  
  // validate the create statament
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! pTkn(i_Token)->bHasText())
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
  pJob_CreateTenant->m_sObjectName = pTkn(2)->aText();
  
  // set the job in SEitf
  m_pSEitf->m_pJob = pJob_CreateTenant;
  
  // create a schema
  pTkn(0)->vCopy("mktenR35"_Stz);
  pTkn(1)->vCopy(""_Stz);
  
  if (!bExecuteRequest())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop catalog                                                               */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCmd_DropCatalogR35(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 i_Token;
  i8 iNumTokens;
  
  ASSERT(pTkn(0)->bSame("drop"_Stz));
  ASSERT(pTkn(1)->bSame("R35catalog"_Stz));
  
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! pTkn(i_Token)->bHasText())
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
  pJob_DropCatalog->m_sObjectName = pTkn(2)->aText();
  
  // set the job in SEitf
  m_pSEitf->m_pJob = pJob_DropCatalog;
  
  // drop a schema
  pTkn(0)->vCopy("rmcatR35"_Stz);
  pTkn(1)->vCopy(""_Stz"");
  if (!bExecuteRequest())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop index                                                                 */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCmd_DropIndexR35(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 i_Token;
  i8 iNumTokens;
  
  ASSERT(pTkn(0)->bSame("drop"_Stz));
  ASSERT(pTkn(1)->bSame("R35index"_Stz));
  
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! pTkn(i_Token)->bHasText())
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
  if (!pTkn(3)->bSame("."_Stz)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // create the job
  Job_DropIndex* pJob_DropIndex = new Job_DropIndex(m_pSEitf);
  
  // set the table name
  pJob_DropIndex->m_sObjectName = pTkn(2)->aText();
  
  // set the job in SEitf
  m_pSEitf->m_pJob = pJob_DropIndex;
  
  // drop a table
  pTkn(0)->vCopy("rmidxR35"_Stz);
  pTkn(1)->vCopy(""_Stz);
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Table dropped: %s\n"_Stz, pTkn2(2));
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop schema                                                                */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCmd_DropSchemaR35(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 i_Token;
  i8 iNumTokens;
  
  ASSERT(pTkn(0)->bSame("drop"_Stz));
  ASSERT(pTkn(1)->bSame("R35schema"_Stz));
  
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! pTkn(i_Token)->bHasText())
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
  pJob_DropSchema->m_sObjectName = pTkn(2)->aText();
  
  // set the job in SEitf
  m_pSEitf->m_pJob = pJob_DropSchema;
  
  // drop a schema
  pTkn(0)->vCopy("rmschR35"_Stz);
  pTkn(1)->vCopy(""_Stz"");
  if (!bExecuteRequest())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop tenant                                                                */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCmd_DropTenantR35(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 i_Token;
  i8 iNumTokens;
  
  ASSERT(pTkn(0)->bSame("drop"_Stz));
  ASSERT(pTkn(1)->bSame("R35tenant"_Stz));
  
  for (i_Token = 0; i_Token < C_TOKENS; i_Token++) {
    if (! pTkn(i_Token)->bHasText())
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
  pJob_DropTenant->m_sObjectName = pTkn(2)->aText();
  
  // set the job in SEitf
  m_pSEitf->m_pJob = pJob_DropTenant;
  
  // drop a schema
  pTkn(0)->vCopy("rmtenR35"_Stz);
  pTkn(1)->vCopy(""_Stz"");
  if (!bExecuteRequest())
    return false;
  
  return true;
}
