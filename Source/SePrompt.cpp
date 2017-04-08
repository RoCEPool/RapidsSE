/*============================================================================*/
/* SePrompt.cpp -- se> prompt                                                 */
/*============================================================================*/

#include "SePrompt.hpp"

extern BufferPool*  gpBufferPool;
extern Heap*        gpMediumHeap;
extern Arguments    gxArguments;
extern AttachMap*   gpAttachMap;
extern Grid*        gpGrid;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* SePrompt::operator new(size_t iSize)
{
  RASSERT(iSize);
  SePrompt* pSePrompt = (SePrompt*)gpBufferPool->pAllocBuffer();
  
  return pSePrompt;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void SePrompt::operator delete(void* pSePromptAddress)
{
  Buffer* pBuffer = reinterpret_cast<Buffer*>(pSePromptAddress);
  
  gpBufferPool->vFree(pBuffer);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SePrompt::SePrompt(void)
: Buffer(CN_SEPROMPT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_iPartition = 0;

  eEC eResult = RapidsSE_SEitfNew((void**)&m_pSEitf,m_iPartition, /*trace*/ 0);
  ASSERT(EC_NONE == eResult);
  
  m_bContinue = true;
  
  m_sPrompt.vSprint("[N%lld %lld] se %s%c> "_Str,
                    gpGrid->iThisNode(),
                    Process::iGetPid(),
                    RAPIDSSE_VERSION_STRING,
#ifdef _DEBUG
                    'd'
#else
                    'r'
#endif
                    );
  
  m_bEcho   = false;
  
  m_bExpect = false;
  m_sExpect = ""_Str;
  
  m_sResponse  = ""_Str;
  //m_xHistory
  
  // initialise the token set
  for (i8 iToken = 0; iToken < C_TOKENS; iToken++) {
    m_aToken[iToken]  = (Str*)&m_xTokenArea.aArea()[C_TOKEN_SPACE * iToken];
    m_aToken2[iToken] = (Str*)&m_xTokenArea2.aArea()[C_TOKEN_SPACE * iToken];
  }
  
  //m_wWorkarea
  
  //----
  m_pTokeniser = new Tokeniser;
  m_pTokeniser->vMalloc(false);
  m_pTokeniser->vPush();
  
  if (gxArguments.m_bCommand) {
    m_pTokeniser->vUseString(gxArguments.m_sCommand);
  }
  else if (gxArguments.m_bPrompt) {
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
SePrompt::~SePrompt(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // close history file
  //m_xHistory.vClose();
  
  RapidsSE_SEitfDelete(m_pSEitf, /*trace*/ 0);
  
  delete m_pTokeniser;
}

/*----------------------------------------------------------------------------*/
/* dump seprompt                                                              */
/*----------------------------------------------------------------------------*/
void SePrompt::vDump(eCN eClass, Object* pObject, Str* aTab)
{
  ASSERT(eClass == CN_SEPROMPT);
  ASSERT(pObject->eClass() == CN_SEPROMPT);
  
  SePrompt* pSePrompt = reinterpret_cast<SePrompt*>(pObject);
  
  vTrace("partition %lld seitf %p continue %s\n"_Str,
         pSePrompt->m_iPartition,
         pSePrompt->m_pSEitf,
         pSePrompt->m_bContinue ? "true"_Str : "false"_Str);
}

/*----------------------------------------------------------------------------*/
/* prompt for commands and process them                                       */
/*----------------------------------------------------------------------------*/
void SePrompt::vLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  // open the command history file
  //m_xHistory.vOpen();
  
  // get and process commands
  while (m_bContinue) {
    
    vGetCommand();
    
    if (0 == m_aToken[0]->iLength())
      continue;
    
    bool b = bProcessCommand();
    
    if (m_bExpect) {
      if ( ! m_sExpect.bSame(m_sResponse) ) {
        vTrace("Expected \"%s\" : Got \"%s\"\n"_Str,m_sExpect.s(),m_sResponse.s());
        PosixThread::vSleepSec(10000);
      }
    }
    
    if (m_bEcho) {
      vTrace("cmd: "_Str);
      for (i8 iToken = 0; iToken < C_TOKENS; iToken++) {
        if (m_aToken[iToken]->bEqual(""_Str))
          break;
        vTrace("%s "_Str,m_aToken[iToken]);
      }
      vTrace(": got: %s\n"_Str,m_sResponse.s());
      continue;
    }
    
    if (b) {
      if ( ! m_bExpect )
        if ( !m_sResponse.bEqual("<ok>"_Str) ) {
          vTrace("%s\n"_Str,m_sResponse.s());
      }
    }
    else {
      if (m_pSEitf->eGetLastError() != EC_NONE)
        if ( ! m_bExpect )
         vTrace("Error: %s\n"_Str,m_pSEitf->pGetLastError());
    }
  }
  
  // close history file
  //m_xHistory.vClose();
  
  // SHOULD REMOVE HISTORY FILE?
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* execute request from given string                                          */
/*----------------------------------------------------------------------------*/
bool SePrompt::bExecuteString(StrVal& sRequest, eEC& eErrorCode)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_pTokeniser->vUseString(sRequest);
  i8 iToken = 0;
  while (TK_NONE != m_pTokeniser->eNextToken(m_aToken[iToken]->s())) {
    
    //vTrace("vGetCommand : [%lld] %s\n"_Str,iToken,m_aToken[iToken]);
    
    iToken++;
    ASSERT(iToken < C_TOKENS);
  }
  bool bResult = bProcessCommand(false);
  
  eErrorCode = m_pSEitf->m_eErrorCode;
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* execute request from given string                                          */
/*----------------------------------------------------------------------------*/
bool SePrompt::bExecuteString(StrVal& sRequest, eEC& eErrorCode, StrVal& sResponse)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_pTokeniser->vUseString(sRequest);
  i8 iToken = 0;
  while (TK_NONE != m_pTokeniser->eNextToken(m_aToken[iToken]->s())) {
    
    //vTrace("vGetCommand : [%lld] %s\n"_Str,iToken,m_aToken[iToken]);
    
    iToken++;
    ASSERT(iToken < C_TOKENS);
  }
  bool bResult = bProcessCommand(false);
  
  eErrorCode = m_pSEitf->m_eErrorCode;
  sResponse = m_pSEitf->m_sResponse;
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* execute request from given string                                          */
/*----------------------------------------------------------------------------*/
bool SePrompt::bExecuteString(StrVal& sRequest, eEC& eErrorCode, Str** ppRowPtr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_pTokeniser->vUseString(sRequest);
  i8 iToken = 0;
  while (TK_NONE != m_pTokeniser->eNextToken(m_aToken[iToken]->s())) {
    
    //vTrace("vGetCommand : [%lld] %s\n"_Str,iToken,m_aToken[iToken]);
    
    iToken++;
    ASSERT(iToken < C_TOKENS);
  }
  bool bResult = bProcessCommand(false);
  
  eErrorCode = m_pSEitf->m_eErrorCode;
  *ppRowPtr = m_pSEitf->m_aRowPtr;
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* get command                                                                */
/*----------------------------------------------------------------------------*/
void SePrompt::vGetCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  if (gxArguments.m_bCommand)
    vGetCommandArgument();
  else
    vGetCommandInput();
}

/*----------------------------------------------------------------------------*/
/* get command from arguments                                                 */
/*----------------------------------------------------------------------------*/
void SePrompt::vGetCommandArgument(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iToken = 0;
  while (TK_NONE != m_pTokeniser->eNextToken(m_aToken[iToken]->s())) {
    
    //vTrace("vGetCommand : [%lld] %s\n"_Str,iToken,m_aToken[iToken]);
    
    iToken++;
    ASSERT(iToken < C_TOKENS);
  }
  
  m_bContinue = false;
}

/*----------------------------------------------------------------------------*/
/* get command from input                                                     */
/*----------------------------------------------------------------------------*/
void SePrompt::vGetCommandInput(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  while (true) {
    
    m_pTokeniser->vResetTokens(m_aToken,C_TOKENS);
    
    i8 iToken = 0;
    while (TK_NONE != m_pTokeniser->eNextToken(m_aToken[iToken]->s())) {
      
      //vTrace("vGetCommand : [%lld] %s\n"_Str,iToken,m_aToken[iToken]);
      
      iToken++;
      ASSERT(iToken < C_TOKENS);
    }
    
    // end of file? if so, pop and retry
    if (! iToken && m_pTokeniser->bEOF()) {
      bool b = m_pTokeniser->bPop();
      ASSERT(b);
      continue;
    }
    
    break;
  }
  
}

/*----------------------------------------------------------------------------*/
/* process command                                                            */
/*----------------------------------------------------------------------------*/
bool SePrompt::bProcessCommand(bool bPostProcess)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // - test for echo prefix
  if (m_aToken[0]->bEqual("echo"_Str))
    vSetEcho();
  else
    vClearEcho();
  
  // - test for expect prefix
  if (m_aToken[0]->bEqual("expect"_Str))
    vSetExpect();
  else
    vClearExpect();
 
  
  // - test for builtin comands
  
  if (m_aToken[0]->bEqual("?"_Str) ||
      m_aToken[0]->bEqual("help"_Str)) {
    vHelp();
    return true;
  }
  
  if (m_aToken[0]->bEqual("bye"_Str)) {
    m_bContinue = false;
    return true;
  }
  
  if (m_aToken[0]->bEqual("sleep"_Str)) {
    return bSleep();
  }
  
  if (m_aToken[0]->bEqual("cd"_Str)) {
    return bChangeWorkingDir();
  }
  
  if (m_aToken[0]->bEqual("cf"_Str)) {
    return bExecuteRequest();
  }
  
  if (m_aToken[0]->bEqual("docol"_Str)) {
    return bDocol();
  }
  
  if (m_aToken[0]->bEqual("history"_Str)) {
    m_xHistory.vPrint();
    return true;
  }
  
  if (m_aToken[0]->bEqual("!!"_Str)) {
    if (! m_xHistory.bRecall(m_aToken,C_TOKENS))
      return true;
  }
  
  if ('!' == *m_aToken[0]->s()) {
    if (! m_xHistory.bRecall(m_aToken,C_TOKENS))
      return true;
  }
  
  if (m_aToken[0]->bEqual("begin"_Str)) {
    if (m_aToken[1]->bEqual("insert"_Str)) {
      return bBeginInsert();
    }
  }
  
  if (m_aToken[0]->bEqual("create"_Str)) {
    
    if (m_aToken[1]->bEqual("tenant"_Str)) {
      return bCreateTenant();
    }
    if (m_aToken[1]->bEqual("catalog"_Str)) {
      return bCreateCatalog();
    }
    if (m_aToken[1]->bEqual("schema"_Str)) {
      return bCreateSchema();
    }
    if (m_aToken[1]->bEqual("table"_Str)) {
      return bCreateTable();
    }
    if (m_aToken[1]->bEqual("index"_Str)) {
      return bCreateIndex();
    }
  }
  
  if (m_aToken[0]->bEqual("connectCluster"_Str)) {
    return bConnectCluster();
  }
  
  if (m_aToken[0]->bEqual("drop"_Str)) {
    if (m_aToken[1]->bEqual("tenant"_Str)) {
      return bDropTenant();
    }
    if (m_aToken[1]->bEqual("catalog"_Str)) {
      return bDropCatalog();
    }
    if (m_aToken[1]->bEqual("index"_Str)) {
      return bDropIndex();
    }
    if (m_aToken[1]->bEqual("schema"_Str)) {
      return bDropSchema();
    }
    if (m_aToken[1]->bEqual("table"_Str)) {
      return bDropTable();
    }
  }
  
  if (m_aToken[0]->bEqual("end"_Str)) {
    if (m_aToken[1]->bEqual("insert"_Str)) {
      return bEndInsert();
    }
  }
  
  if (m_aToken[0]->bEqual("antbl"_Str)) {
    return bAnalyzeTable();
  }
  
  if (m_aToken[0]->bEqual("dantbl"_Str)) {
    return bDanalyzeTable();
  }
  
  if (m_aToken[0]->bEqual("pwd"_Str)) {
    return bPrintWorkingDir();
  }
  
  if (m_aToken[0]->bEqual("ls"_Str)) {
    return bListDir();
  }
  
  if (m_aToken[0]->bEqual("first"_Str)) {
    return bGetFirstRow();
  }
  
  if (m_aToken[0]->bEqual("insert"_Str)) {
    if (m_aToken[1]->bEqual("row"_Str)) {
      return bInsertRow();
    }
  }
  
  if (m_aToken[0]->bEqual("next"_Str)) {
    return bGetNextRow();
  }

  if (m_aToken[0]->bEqual("object"_Str)) {
    return bObject();
  }
  
  if (m_aToken[0]->bEqual("space"_Str)) {
    return bPoolSpace();
  }

  if (m_aToken[0]->bEqual("use"_Str)) {
    if (m_aToken[1]->bEqual("schema"_Str)) {
      return bUseSchema();
    }
  }
  
  m_xHistory.vAppend(m_aToken);
  
  if (bTestFile(m_aToken[0])) {
    vPushFile(m_aToken[0]);
    m_pTokeniser->vLineMode(true);
    m_sResponse = "<ok>"_Str;
    return true;
  }
  
  // - execute seitf request
  if (! bExecuteRequest())
    return false;
  
  if (!bPostProcess)
    return true;
  
  // - post process requests
  if (m_aToken[0]->bEqual("sel"_Str)) {
    vIterateRows();
    return bReturnTable();
  }
  
  // - the following never return
  
  if (m_aToken[0]->bEqual("mon"_Str)) {
    while (true) {
      PosixThread::vSleepSec(10);
      if (!bExecuteRequest())
        return true;
    }
  }
  
  if (m_aToken[0]->bEqual("x"_Str)) {
    while (true) {
      bExecuteRequest();
    }
  }
  
  return true;
}


/*----------------------------------------------------------------------------*/
/* set echo result                                                            */
/*----------------------------------------------------------------------------*/
void SePrompt::vSetEcho(void)
{
  m_bEcho = true;
  
  vClipTokens(1);
}

/*----------------------------------------------------------------------------*/
/* clear echo result                                                          */
/*----------------------------------------------------------------------------*/
void SePrompt::vClearEcho(void)
{
  m_bEcho = false;
}

/*----------------------------------------------------------------------------*/
/* collect and set expected result                                            */
/*----------------------------------------------------------------------------*/
void SePrompt::vSetExpect(void)
{
  m_sExpect = m_aToken[1];
  
  m_bExpect = true;
  
  vClipTokens(2);
}

/*----------------------------------------------------------------------------*/
/* clear expected result                                                      */
/*----------------------------------------------------------------------------*/
void SePrompt::vClearExpect(void)
{
  m_sExpect = ""_Str;
  
  m_bExpect = false;
}

/*----------------------------------------------------------------------------*/
/* clip tokens                                                                */
/*----------------------------------------------------------------------------*/
void SePrompt::vClipTokens(i8 nTokens)
{
  i8 iToToken = 0;
  
  for (i8 iFromToken = nTokens; iFromToken < C_TOKENS; iFromToken++) {
    
    m_aToken[iToToken++]->vCopy(m_aToken[iFromToken]);
    
    if (m_aToken[iFromToken]->bEqual(""_Str))
      break;
  }
  
}

/*----------------------------------------------------------------------------*/
/* execute an se request                                                      */
/*----------------------------------------------------------------------------*/
bool SePrompt::bExecuteRequest(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Timer xTimer;
  Str* aResponse = nullptr;
  i8 iTokens;
  
  xTimer.vStart();
  
  // get count of words in pWords
  for (iTokens = 0; iTokens < C_TOKENS; iTokens++)
    if (m_aToken[iTokens]->bEqual(""_Str))
      break;

                                                 //! make this part of verbose??
  //m_pTokeniser->vDumpTokens("Command : "_Str,m_aToken,C_TOKENS);
  
  bool bResult = RapidsSE_ExecuteRequest(m_pSEitf,m_aToken,iTokens,&aResponse);
  
  if (bResult)
    m_sResponse = aResponse ? (Str*)aResponse : ""_Str;
  else
    m_sResponse = aResponse ? (Str*)aResponse : ""_Str;
  
  xTimer.vUpdate();
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* iterate rows                                                               */
/*----------------------------------------------------------------------------*/
void SePrompt::vIterateRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iAddress = m_sResponse.iToI8();
  
  SelCursor* pSelCursor = reinterpret_cast<SelCursor*>(iAddress);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  i8 iRowCount = 0;
  
  for (i8 iPartition = 0; iPartition < gxArguments.m_nSlots; iPartition++) {
    vTrace("Partition %lld: \n"_Str, Partition::iSlotToPartition(iPartition));
    pSelCursor->vBegin(iPartition);
    pSelCursor->vFirst();
    Str* pStr = pSelCursor->aRowAsStr();
    while (pStr) {
      iRowCount++;
      vTrace("Row: %s\n"_Str,pStr);
      pSelCursor->vNext();
      pStr = pSelCursor->aRowAsStr();
    }
  }
  
  pSelCursor->vEnd();
  delete pSelCursor;
}

/*----------------------------------------------------------------------------*/
/* return table                                                               */
/*----------------------------------------------------------------------------*/
bool SePrompt::bReturnTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_aToken[0]->vCopy("rettbl"_Str);
  m_aToken[1]->vCopy(""_Str);
  
  if (!bExecuteRequest()) {
    vTrace("bReturnTable: (%s) Return Table FAILED!!! : %s\n"_Str, m_sResponse.s());
    return false;
  }
  return true;
}

/*----------------------------------------------------------------------------*/
/* get the first row                                                          */
/*----------------------------------------------------------------------------*/
bool SePrompt::bGetFirstRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iAddress = m_sResponse.iToI8();
  
  SelCursor* pSelCursor = reinterpret_cast<SelCursor*>(iAddress);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  pSelCursor->vBegin(m_iPartition);
  pSelCursor->vFirst();
  Str* pStr = pSelCursor->aRowAsStr();
  
  if (pStr == nullptr) {
    pSelCursor->vEnd();
    delete pSelCursor;
    m_pSEitf->m_aRowPtr = nullptr;
    return false;
  }
  else {
    m_pSEitf->m_aRowPtr = pStr;
    return true;
  }
}

/*----------------------------------------------------------------------------*/
/* get the next row                                                           */
/*----------------------------------------------------------------------------*/
bool SePrompt::bGetNextRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iAddress = m_sResponse.iToI8();
  
  SelCursor* pSelCursor = reinterpret_cast<SelCursor*>(iAddress);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  pSelCursor->vNext();
  Str* pStr = pSelCursor->aRowAsStr();
  
  if (pStr == nullptr) {
    pSelCursor->vEnd();
    delete pSelCursor;
    m_pSEitf->m_aRowPtr = nullptr;
    
    // now return the table
    bool bResult = bReturnTable();
    ASSERT(bResult);
    return false;
  }
  else {
    m_pSEitf->m_aRowPtr = pStr;
    return true;
  }
}

/*----------------------------------------------------------------------------*/
/* test for command file                                                      */
/*----------------------------------------------------------------------------*/
bool SePrompt::bTestFile(Str* aFileName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  File xFile;
  
  if (xFile.bExists(aFileName))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* push command file                                                          */
/*----------------------------------------------------------------------------*/
void SePrompt::vPushFile(Str* aFileName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  s256 sPath;
  
  sPath = aFileName;
  
  m_pTokeniser->vPush();
  
  bool b = m_pTokeniser->bUseFile(sPath);
  ASSERT(b);
}

/*----------------------------------------------------------------------------*/
/* help                                                                       */
/*----------------------------------------------------------------------------*/
void SePrompt::vHelp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  vTrace("se temporary commands:\n"_Str);
  vTrace("add    -- add column to table\n"_Str);
  vTrace("gettbl -- get address of table\n"_Str);
  vTrace("bye    -- quit se prompt\n"_Str);
  vTrace("cd     -- change directory\n"_Str);
  vTrace("cf     -- change folder\n"_Str);
  //vTrace("con    -- connect\n"_Str);
  vTrace("connectCluster    -- connect the cluster\n"_Str);
  vTrace("cp     -- copy table\n"_Str);
  vTrace("create -- create table\n"_Str);
  //vTrace("dis    -- disconnect\n"_Str);
  vTrace("do     -- do shorthand\n"_Str);
  vTrace("drop   -- dropTable\n"_Str);
  vTrace("dump   -- dump table\n"_Str);
  vTrace("ex     -- export table t\n"_Str);
  vTrace("ld     -- load table t\n"_Str);
  vTrace("lf     -- list folder contents\n"_Str);
  vTrace("ls     -- list directory contents\n"_Str);
  vTrace("lscat  -- list catalogs contents\n"_Str);
  vTrace("lscol  -- list columns\n"_Str);
  vTrace("lsidx  -- list indices\n"_Str);
  vTrace("lssch  -- list schemas contents\n"_Str);
  vTrace("lstbl  -- list tables contents\n"_Str);
  vTrace("lsten  -- list tenants contents\n"_Str);
  vTrace("mkcat  -- create catalog\n"_Str);
  vTrace("mkidx  -- create index\n"_Str);
  vTrace("mksch  -- create schema\n"_Str);
  vTrace("mktbl  -- create table\n"_Str);
  vTrace("mkten  -- create tenant\n"_Str);
  vTrace("print  -- print string\n"_Str);
  vTrace("pwf    -- print working folder\n"_Str);
  vTrace("pwd    -- print working directory\n"_Str);
  vTrace("rettbl -- return usage of table\n"_Str);
  vTrace("rmcat  -- drop catalog\n"_Str);
  vTrace("rmdic  -- drop dictionary\n"_Str);
  vTrace("rmidx  -- drop index\n"_Str);
  vTrace("rmsch  -- drop schema\n"_Str);
  vTrace("rmtbl  -- drop table\n"_Str);
  vTrace("rmten  -- drop tenant\n"_Str);
  vTrace("sel    -- select * from table\n"_Str);
  vTrace("space  -- pool space\n"_Str);
  vTrace("status -- grid status\n"_Str);
  vTrace("sz     -- size of table\n"_Str);
  vTrace("tpch   -- tpch shorthand\n"_Str);
  vTrace("tr     -- truncate table\n"_Str);
  vTrace("usesch -- use schema\n"_Str);
  vTrace("?      -- print this help\n"_Str);
  vTrace("help   -- print this help\n"_Str);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* begin insert                                                               */
/*----------------------------------------------------------------------------*/
bool SePrompt::bBeginInsert(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iToken;
  i8 iNumTokens;
  
  ASSERT(m_aToken[0]->bEqual("begin"_Str));
  ASSERT(m_aToken[1]->bEqual("insert"_Str));
  
  vCollectStatement();
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_aToken[iToken]->s())
      break;
    m_aToken2[iToken]->vCopy(m_aToken[iToken]);
    m_aToken[iToken]->vCopy(""_Str);
  }
  
  iNumTokens = iToken;
  // there should be 4 tokens
  if (iNumTokens != 4) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!m_aToken2[3]->bEqual(";"_Str)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // get table pointer
  m_aToken[0]->vCopy("gettbl"_Str);
  m_aToken[1]->vCopy(m_aToken2[2]);
  if (!bExecuteRequest()) {
    vTrace("SePrompt::bBeginInsert : gettbl FAILED!!!\n"_Str);
    return false;
  }
  
  bool bResult = m_pSEitf->bBeginTx();
  
  if (!bResult) {
    m_pSEitf->vSetLastError(EC_BEGIN_TX_FAILED);
    return false;
  }
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* sleep n seconds                                                            */
/*----------------------------------------------------------------------------*/
bool SePrompt::bSleep(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(m_aToken[0]->bEqual("sleep"_Str));
  
  i8 nSeconds = m_aToken[1]->iToI8();

  PosixThread::vSleepSec(nSeconds);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* change working directory                                                   */
/*----------------------------------------------------------------------------*/
bool SePrompt::bChangeWorkingDir(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  s256 sDir;
  Dir xDir;
  
  ASSERT(m_aToken[0]->bEqual("cd"_Str));
  
  sDir = m_aToken[1];
  
  i8 iToken;
  for (iToken = 2; iToken < C_TOKENS; iToken++) {
    if (! *m_aToken[iToken]->s())
      break;
    sDir.vConcat(m_aToken[iToken]);
  }

  if ( ! xDir.bChangeCurrent(sDir) ) {
    m_pSEitf->vSetLastError(EC_NO_SUCH_DIRECTORY);
    return false;
  }
  return true;
}

/*----------------------------------------------------------------------------*/
/* analyze table                                                              */
/*----------------------------------------------------------------------------*/
bool SePrompt::bAnalyzeTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  s256 sDir;
  Dir xDir;
  
  ASSERT(m_aToken[0]->bEqual("antbl"_Str));
  
  return bExecuteRequest();
}

/*----------------------------------------------------------------------------*/
/* detailed analyze table                                                     */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDanalyzeTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  s256 sDir;
  Dir xDir;
  
  ASSERT(m_aToken[0]->bEqual("dantbl"_Str));
  
  return bExecuteRequest();
}

/*----------------------------------------------------------------------------*/
/* docol                                                                      */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDocol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(m_aToken[0]->bEqual("docol"_Str));
  return bExecuteRequest();
}

/*----------------------------------------------------------------------------*/
/* create catalog                                                             */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCreateCatalog(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(m_aToken[0]->bEqual("create"_Str));
  ASSERT(m_aToken[1]->bEqual("catalog"_Str));
  vCollectStatement();
  
  i8 iToken;
  
  // copy tokens from m_aToken to m_aToken2
  // while doing that also validate the create statament
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_aToken[iToken]->s())
      break;
    m_aToken2[iToken]->vCopy(m_aToken[iToken]);
    m_aToken[iToken]->vCopy(""_Str);
  }
  
  i8 iNumTokens = iToken;
  // there should be 4 tokens
  if (iNumTokens != 4) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!m_aToken2[3]->bEqual(";"_Str)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // create a catalog
  m_aToken[0]->vCopy("mkcat"_Str);
  m_aToken[1]->vCopy(m_aToken2[2]);
  
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Catalog created: %s\n"_Str, m_aToken2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* create index                                                               */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCreateIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(m_aToken[0]->bEqual("create"_Str));
  ASSERT(m_aToken[1]->bEqual("index"_Str));
  vCollectStatement();
  
  i8 iToken;
  
  // copy tokens from m_aToken to m_aToken2
  // while doing that also validate the create statament
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_aToken[iToken]->s())
      break;
    m_aToken2[iToken]->vCopy(m_aToken[iToken]);
    m_aToken[iToken]->vCopy(""_Str);
  }
  
  i8 iValue;
  i8 iNumTokens = iToken;
  
  // some validations
  /// there should be a minumum of 8 tokens
  if (iNumTokens < 8) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // token[3] should be 'on'
  if (!m_aToken2[3]->bEqual("on"_Str)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // token[5] should be '('
  if (!m_aToken2[5]->bEqual("("_Str)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // 7 token should be ')'
  if (!m_aToken2[7] ->bEqual(")"_Str)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!m_aToken2[iNumTokens-1]->bEqual(";"_Str)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // create the index
  m_aToken[0]->vCopy("mkidx"_Str);
  m_aToken[1]->vCopy(m_aToken2[2]);
  m_aToken[2]->vCopy(m_aToken2[4]);
  m_aToken[3]->vCopy(m_aToken2[6]);
  
  if (!bExecuteRequest())
    return false;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* create schema                                                              */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCreateSchema(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  ASSERT(m_aToken[0]->bEqual("create"_Str));
  ASSERT(m_aToken[1]->bEqual("schema"_Str));
  vCollectStatement();
  
  i8 iToken;
  
  // copy tokens from m_aToken to m_aToken2
  // while doing that also validate the create statament
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_aToken[iToken]->s())
      break;
    m_aToken2[iToken]->vCopy(m_aToken[iToken]);
    m_aToken[iToken]->vCopy(""_Str);
  }
  
  i8 iNumTokens = iToken;
  
  // there should be 4 tokens
  if (iNumTokens != 4) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!m_aToken2[3]->bEqual(";"_Str)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // create a schema
  m_aToken[0]->vCopy("mksch"_Str);
  m_aToken[1]->vCopy(m_aToken2[2]);
  
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Schema created: %s\n"_Str, m_aToken2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* create tenant                                                              */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCreateTenant(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(m_aToken[0]->bEqual("create"_Str));
  ASSERT(m_aToken[1]->bEqual("tenant"_Str));
  vCollectStatement();
  
  i8 iToken;
  
  // copy tokens from m_aToken to m_aToken2
  // while doing that also validate the create statament
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_aToken[iToken]->s())
      break;
    m_aToken2[iToken]->vCopy(m_aToken[iToken]);
    m_aToken[iToken]->vCopy(""_Str);
  }
  
  i8 iNumTokens = iToken;
  
  // there should be 4 tokens
  if (iNumTokens != 4) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!m_aToken2[3]->bEqual(";"_Str)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // create a tenant
  m_aToken[0]->vCopy("mkten"_Str);
  m_aToken[1]->vCopy(m_aToken2[2]);
  
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Tenant created: %s\n"_Str, m_aToken2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* create table                                                               */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCreateTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(m_aToken[0]->bSame("create"_Str));
  ASSERT(m_aToken[1]->bSame("table"_Str));
  vCollectStatement();
  
  i8 iToken;
  
  // copy tokens from m_aToken to m_aToken2
  // while doing that also validate the create statament
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_aToken[iToken]->s())
      break;
    m_aToken2[iToken]->vCopy(m_aToken[iToken]);
    m_aToken[iToken]->vCopy(""_Str);
  }
  
  i8 iNumTokens = iToken;
  
  // some validations
  /// there should be a minumum of 8 tokens
  if (iNumTokens < 8) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // token[3] should be '('
  if (!m_aToken2[3]->bEqual("("_Str)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last-1 token should be ')'
  if (!m_aToken2[iNumTokens-2]->bEqual(")"_Str)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
    
  // last token should be ';'
  if (!m_aToken2[iNumTokens-1]->bEqual(";"_Str)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
      
  // create the table
  m_aToken[0]->vCopy("mktbl"_Str);
  m_aToken[1]->vCopy(m_aToken2[2]);
  
  if (!bExecuteRequest())
    return false;
  
  // change folder to table
  m_aToken[0]->vCopy("cf"_Str);
  m_aToken[1]->vCopy(m_aToken2[2]);
  
  if (!bExecuteRequest()) {
    vTrace("SePrompt::bCreateTable : cf FAILED!!!\n"_Str);
    
    // create the table
    m_aToken[0]->vCopy("rmtbl"_Str);
    m_aToken[1]->vCopy(m_aToken2[2]);
    
    if (!bExecuteRequest()) {
      return false;
    }
    
    return false;
  }
  
  // get reference to pTable
  m_aToken[0]->vCopy("gettbl"_Str);
  m_aToken[1]->vCopy(m_aToken2[2]);
  
  if (!bExecuteRequest()) {
    vTrace("SePrompt::bCreateTable : gettbl FAILED!!!\n"_Str);
    
    // create the table
    m_aToken[0]->vCopy("rmtbl"_Str);
    m_aToken[1]->vCopy(m_aToken2[2]);
    
    if (!bExecuteRequest()) {      
      return false;
    }
    
    return false;
  }
  
  // now collect the column names and their types
  i8 iCurrentToken = 4;
  while(iCurrentToken < iNumTokens - 2) {
    
    if (iCurrentToken != 4 && !m_aToken2[iCurrentToken++]->bEqual(","_Str)) {
      bDropReturnTable(m_aToken2[2]);
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      return false;
    }
    
    // add columns to the table
    i8 j = 0;
    m_aToken[j++]->vCopy("addcol"_Str);
    m_aToken[j++]->vCopy(m_aToken2[2]);
    m_aToken[j++]->vCopy(m_aToken2[iCurrentToken++]);
    
    if (m_aToken2[iCurrentToken]->bSame("integer"_Str) ||
        m_aToken2[iCurrentToken]->bSame("varchar"_Str) ||
        m_aToken2[iCurrentToken]->bSame("varbinary"_Str) ||
        m_aToken2[iCurrentToken]->bSame("char"_Str) ||
        m_aToken2[iCurrentToken]->bSame("float"_Str)) {
      
      // copy varchar/char
      m_aToken[j]->vCopy(m_aToken2[iCurrentToken++]);
      
      if (m_aToken2[iCurrentToken]->bEqual("("_Str)) {
        
        // str.cat '('
        m_aToken[j]->vConcat(m_aToken2[iCurrentToken++]);
        
        // str.cat the size
        m_aToken[j]->vConcat(m_aToken2[iCurrentToken]);
        
        // check if it was numeric
        if ( ! m_aToken2[iCurrentToken++]->bIsNumeric() ) {
          // drop the table
          bDropReturnTable(m_aToken2[2]);
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        }
        
        if (!m_aToken2[iCurrentToken]->bEqual(")"_Str)) {
          // drop the table
          bDropReturnTable(m_aToken2[2]);
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        }
        
        // str.cat ')'
        m_aToken[j]->vConcat(m_aToken2[iCurrentToken++]);
        
      }
    }
    else if (m_aToken2[iCurrentToken]->bSame("decimal"_Str)) {
      
      // copy varchar/char
      m_aToken[j]->vCopy(m_aToken2[iCurrentToken++]);
      
      if (m_aToken2[iCurrentToken]->bEqual("("_Str)) {
        
        // str.cat '('
        m_aToken[j]->vConcat(m_aToken2[iCurrentToken++]);
        
        // str.cat the size
        m_aToken[j]->vConcat(m_aToken2[iCurrentToken]);
        
        // check if it was numeric
        if ( ! m_aToken2[iCurrentToken++]->bIsNumeric() ) {
          bDropReturnTable(m_aToken2[2]);
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        }
        
        if (!m_aToken2[iCurrentToken]->bEqual(","_Str)) {
          bDropReturnTable(m_aToken2[2]);
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        }
        
        // str.cat ','
        m_aToken[j]->vConcat(m_aToken2[iCurrentToken++]);
        
        // str.cat the size
        m_aToken[j]->vConcat(m_aToken2[iCurrentToken]);
        
        // check if it was numeric
        if ( ! m_aToken2[iCurrentToken++]->bIsNumeric() ) {
          bDropReturnTable(m_aToken2[2]);
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        }
        
        if (!m_aToken2[iCurrentToken]->bEqual(")"_Str)) {
          bDropReturnTable(m_aToken2[2]);
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        }
        
        // str.cat ')'
        m_aToken[j]->vConcat(m_aToken2[iCurrentToken++]);
      }
    }
    else if (m_aToken2[iCurrentToken]->bSame("date"_Str) ||
             m_aToken2[iCurrentToken]->bSame("timestamp"_Str) ||
             m_aToken2[iCurrentToken]->bSame("interval"_Str) ||
             m_aToken2[iCurrentToken]->bSame("boolean"_Str)) {
      m_aToken[j]->vCopy(m_aToken2[iCurrentToken++]);
    }
    else {
      // drop the table that was created
      bDropReturnTable(m_aToken2[2]);
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      return false;
    }
    
    // check for NOT NULL
    if (m_aToken2[iCurrentToken]->bSame("not"_Str)) {
      
      m_aToken[j]->vConcat(" "_Str);
      m_aToken[j]->vConcat(m_aToken2[iCurrentToken++]);
      
      if (!m_aToken2[iCurrentToken]->bSame("null"_Str)) {
        bDropReturnTable(m_aToken2[2]);
        m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
        return false;
      }
      m_aToken[j]->vConcat(" "_Str);
      m_aToken[j]->vConcat(m_aToken2[iCurrentToken++]);
    }
    
    // check for PRIMARY KEY
    if (m_aToken2[iCurrentToken]->bSame("primary"_Str)) {
      
      m_aToken[j]->vConcat(" "_Str);
      m_aToken[j]->vConcat(m_aToken2[iCurrentToken++]);
      
      if (!m_aToken2[iCurrentToken]->bSame("key"_Str)) {
        bDropReturnTable(m_aToken2[2]);
        m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
        return false;
      }
      m_aToken[j]->vConcat(" "_Str);
      m_aToken[j]->vConcat(m_aToken2[iCurrentToken++]);
    }

    // check for UNIQUE
    if (m_aToken2[iCurrentToken]->bSame("unique"_Str)) {
      
      m_aToken[j]->vConcat(" "_Str);
      m_aToken[j]->vConcat(m_aToken2[iCurrentToken++]);
    }

    
    // now we should see a column type
    // validate it
    m_aToken[j]->vToLower();
    
    // add the column
    if (!bExecuteRequest()) {
      
      // save error returned by addColumn
      eEC eError = m_pSEitf->eGetLastError();
      
      // drop the table that was created
      bDropReturnTable(m_aToken2[2]);
      
      // return error returned by addColumn
      m_pSEitf->vSetLastError(eError);
      return false;
    }
  }
  
  // end of add columns
  m_aToken[0]->vCopy("endmktbl"_Str);
  m_aToken[1]->vCopy(m_aToken2[2]);
  m_aToken[2]->vCopy(""_Str);
  
  if (!bExecuteRequest()) {
    vTrace("SePrompt::bCreateTable : endmktbl FAILED!!!\n"_Str);
  }
  
  // derefernce and return the table first
  bool bResult = bReturnTable();
  ASSERT(bResult);

  //vTrace("Table created: %s\n"_Str, m_aToken2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* collect ';' delimited statement                                            */
/*----------------------------------------------------------------------------*/
void SePrompt::vCollectStatement(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // we should have a first token (e.g. create or drop)
  ASSERT(! m_aToken[0]->bEqual(""_Str));

  // have we already seen the ';'
  i8 iToken = m_pTokeniser->iFindToken(";"_Str,m_aToken,C_TOKENS);
  
  if (iToken >= 0) {
    //m_pTokeniser->vDumpTokens("\nCommand : "_Str,m_aToken,C_TOKENS);
    return;
  }
  
  iToken = m_pTokeniser->iCountTokens(m_aToken,C_TOKENS);
  
  // switch to terminal mode
  m_pTokeniser->vLineMode(false);
  
  s32 sContinuation("  > "_Str);
  m_pTokeniser->vPrompt(sContinuation);
  
  // get tokens delimited by ';'
  for ( ; iToken < C_TOKENS; iToken++) {
    
    eTK eToken = m_pTokeniser->eNextToken(m_aToken[iToken]->s());
    ASSERT(TK_NONE != eToken);
    
    // look for terminal
    if (m_aToken[iToken]->bEqual(";"_Str))
      break;
  }
  
  // switch back to line mode
  m_pTokeniser->vLineMode(true);
  m_pTokeniser->vPrompt(m_sPrompt);
  
  //m_pTokeniser->vDumpTokens("Command : "_Str,m_aToken,C_TOKENS);
}

/*----------------------------------------------------------------------------*/
/* connect the cluster                                                        */
/*----------------------------------------------------------------------------*/
bool SePrompt::bConnectCluster(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(m_aToken[0]->bEqual("connectCluster"_Str));
  
  if (!bExecuteRequest()) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop catalog                                                               */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDropCatalog(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iToken;
  i8 iNumTokens;
  
  ASSERT(m_aToken[0]->bEqual("drop"_Str));
  ASSERT(m_aToken[1]->bEqual("catalog"_Str));
  
  vCollectStatement();
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_aToken[iToken]->s())
      break;
    m_aToken2[iToken]->vCopy(m_aToken[iToken]);
    m_aToken[iToken]->vCopy(""_Str);
  }
  
  iNumTokens = iToken;
  
  // there should be 4 tokens
  if (iNumTokens != 4) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!m_aToken2[3]->bEqual(";"_Str)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a catalog
  m_aToken[0]->vCopy("rmcat"_Str);
  m_aToken[1]->vCopy(m_aToken2[2]);
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Catalog dropped: %s\n"_Str, m_aToken2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop index                                                                 */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDropIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iToken;
  i8 iNumTokens;
  
  ASSERT(m_aToken[0]->bEqual("drop"_Str));
  ASSERT(m_aToken[1]->bEqual("index"_Str));
  
  vCollectStatement();
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_aToken[iToken]->s())
      break;
    m_aToken2[iToken]->vCopy(m_aToken[iToken]);
    m_aToken[iToken]->vCopy(""_Str);
  }
  
  iNumTokens = iToken;
  
  // there should be 4 tokens
  if (iNumTokens != 4) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!m_aToken2[3] ->bEqual(";"_Str)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a table
  m_aToken[0]->vCopy("rmidx"_Str);
  m_aToken[1]->vCopy(m_aToken2[2]);
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Table dropped: %s\n"_Str, m_aToken2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop schema                                                                */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDropSchema(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iToken;
  i8 iNumTokens;
  
  ASSERT(m_aToken[0]->bEqual("drop"_Str));
  ASSERT(m_aToken[1]->bEqual("schema"_Str));
  
  vCollectStatement();
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_aToken[iToken]->s())
      break;
    m_aToken2[iToken]->vCopy(m_aToken[iToken]);
    m_aToken[iToken]->vCopy(""_Str);
  }
  
  iNumTokens = iToken;
  // there should be 4 tokens
  if (iNumTokens != 4) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!m_aToken2[3]->bEqual(";"_Str)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a schema
  m_aToken[0]->vCopy("rmsch"_Str);
  m_aToken[1]->vCopy(m_aToken2[2]);
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Schema dropped: %s\n"_Str, m_aToken2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop table                                                                 */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDropTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iToken;
  i8 iNumTokens;
  
  ASSERT(m_aToken[0]->bEqual("drop"_Str));
  ASSERT(m_aToken[1]->bEqual("table"_Str));
  
  vCollectStatement();
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_aToken[iToken]->s())
      break;
    m_aToken2[iToken]->vCopy(m_aToken[iToken]);
    m_aToken[iToken]->vCopy(""_Str);
  }
  
  iNumTokens = iToken;
  
  // there should be 4 tokens
  if (iNumTokens != 4) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!m_aToken2[3]->bEqual(";"_Str)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a table
  m_aToken[0]->vCopy("rmtbl"_Str);
  m_aToken[1]->vCopy(m_aToken2[2]);
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Table dropped: %s\n"_Str, m_aToken2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop return table                                                          */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDropReturnTable(Str* aTableName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // derefernce and return the table first
  bool bResult = bReturnTable();
  ASSERT(bResult);
  
  // change folder to table
  m_aToken[0]->vCopy("cf"_Str);
  m_aToken[1]->vCopy(".."_Str);
  
  if (!bExecuteRequest()) {
    return false;
  }
  m_aToken[0]->vCopy("rmtbl"_Str);
  m_aToken[1]->vCopy(aTableName);
  m_aToken[2]->vCopy(""_Str);
  
  if (!bExecuteRequest()) {
    //vTrace("Error: (%s) Drop Table %s\n"_Str, m_sResponse.s(), pTableName);
    return false;
  }
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop tenant                                                                */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDropTenant(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iToken;
  i8 iNumTokens;
  
  ASSERT(m_aToken[0]->bEqual("drop"_Str));
  ASSERT(m_aToken[1]->bEqual("tenant"_Str));
  
  vCollectStatement();
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_aToken[iToken]->s())
      break;
    m_aToken2[iToken]->vCopy(m_aToken[iToken]);
    m_aToken[iToken]->vCopy(""_Str);
  }
  
  iNumTokens = iToken;
  
  // there should be 4 tokens
  if (iNumTokens != 4) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!m_aToken2[3]->bEqual(";"_Str)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a tenant
  m_aToken[0]->vCopy("rmten"_Str);
  m_aToken[1]->vCopy(m_aToken2[2]);
  if (!bExecuteRequest()) {
    return false;
  }
  
  //vTrace("Tenant dropped: %s\n"_Str, m_aToken2[2]);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* end insert                                                                 */
/*----------------------------------------------------------------------------*/
bool SePrompt::bEndInsert(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iToken;
  i8 iNumTokens;
  
  ASSERT(m_aToken[0]->bEqual("end"_Str));
  ASSERT(m_aToken[1]->bEqual("insert"_Str));
  
  vCollectStatement();
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_aToken[iToken]->s())
      break;
    m_aToken2[iToken]->vCopy(m_aToken[iToken]);
    m_aToken[iToken]->vCopy(""_Str);
  }
  
  iNumTokens = iToken;
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!m_aToken2[3]->bEqual(";"_Str)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // clear the last error
  m_pSEitf->vClearLastError();
  
  bool bResult = m_pSEitf->bInsertBuffer();
  
  // if rowBuffer request was appended to requestQ, then wait for resonse
  if (bResult) {
    // get response from se
    eEC eResult = eGetResponse(m_pSEitf);
    
    if (eResult != EC_NONE) {
      m_pSEitf->vSetLastError(eResult);
      return eResult;
    }
  }
  
  // now return the table
  bResult = bReturnTable();
  if (!bResult)
    return false;
  
  //eEC eResult = eExecuteRequest(m_pSEitf, "rettbl"_Str);
  //if (EC_NONE != eResult) {
  //  vTrace("SePrompt::bEndInsert : rettbl FAILED!!!\n"_Str);
  //  return eResult;
  //}
  
  // get response
  //eResult = eGetResponse(m_pSEitf);
  //if (EC_NONE != eResult) {
  //  return eResult;
  //}

  //m_pSEitf->vCurrentTable(nullptr);
  
  m_pSEitf->vCommitTx();
  m_pSEitf->vEndTx();

  return true;
}

/*----------------------------------------------------------------------------*/
/* insert a row                                                               */
/*----------------------------------------------------------------------------*/
bool SePrompt::bInsertRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iToken;
  i8 iNumTokens;
  
  ASSERT(m_aToken[0]->bEqual("insert"_Str));
  ASSERT(m_aToken[1]->bEqual("row"_Str));
  
  vCollectStatement();
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_aToken[iToken]->s())
      break;
    m_aToken2[iToken]->vCopy(m_aToken[iToken]);
    m_aToken[iToken]->vCopy(""_Str);
  }
  
  iNumTokens = iToken;
  // there should be 3 tokens
  if (iNumTokens != 4) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!m_aToken2[3]->bEqual(";"_Str)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // clear the last error
  m_pSEitf->vClearLastError();
  
  //if (!pRow) {
  //  m_pSEitf->vSetLastError(EC_INVALID_PARAMETER);
  //  return EC_INVALID_PARAMETER;
  //}
  
  if (!m_pSEitf->pCurrentTable()) {
    m_pSEitf->vSetLastError(EC_NO_SUCH_TABLE);
    return EC_NO_SUCH_TABLE;
  }
  
  Str* pRow = m_aToken2[2];
  bool bResult = m_pSEitf->bInsertRow(pRow);
  
  if (!bResult) {
    m_pSEitf->vSetLastError(m_pSEitf->m_eErrorCode);
    return bResult;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* list directory                                                             */
/*----------------------------------------------------------------------------*/
bool SePrompt::bListDir(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Dir  xDir;
  s256 sDir;
  s256 sDirEntry;
  i8 iEntryType;
  
  ASSERT(m_aToken[0]->bEqual("ls"_Str));
  
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
  while(xDir.bRead()) {
    sDirEntry = xDir.sEntryName();
    iEntryType = xDir.iEntryType();
    vTrace("%c %s\n"_Str, iEntryType == DT_DIR ? 'd' : 'f', sDirEntry.s());
  }
  
  // close the current directory
  xDir.vClose();
  
  // clear the response
  m_sResponse = ""_Str;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* send object (temp)                                                         */
/*----------------------------------------------------------------------------*/
bool SePrompt::bObject(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(m_aToken[0]->bEqual("object"_Str));
  
  if (!bExecuteRequest()) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* print pool space                                                           */
/*----------------------------------------------------------------------------*/
bool SePrompt::bPoolSpace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(m_aToken[0]->bEqual("space"_Str));
    
  if (!bExecuteRequest()) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* print node status                                                          */
/*----------------------------------------------------------------------------*/
bool SePrompt::bNodeStatus(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(m_aToken[0]->bEqual("status"_Str));
  
  if (!bExecuteRequest()) {
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* print working directory                                                    */
/*----------------------------------------------------------------------------*/
bool SePrompt::bPrintWorkingDir(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Dir xDir;
  s256 sDir;
  
  ASSERT(m_aToken[0]->bEqual("pwd"_Str));
  
  if (xDir.bGetCurrent(sDir)) {
    vTrace("%s"_Str, sDir.s());
  }
  
  // clear the respnse
  m_sResponse = ""_Str;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* use schema                                                                 */
/*----------------------------------------------------------------------------*/
bool SePrompt::bUseSchema(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iToken;
  i8 iNumTokens;
  
  ASSERT(m_aToken[0]->bEqual("use"_Str));
  ASSERT(m_aToken[1]->bEqual("schema"_Str));
  
  vCollectStatement();
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_aToken[iToken]->s())
      break;
    m_aToken2[iToken]->vCopy(m_aToken[iToken]);
    m_aToken[iToken]->vCopy(""_Str);
  }
  
  iNumTokens = iToken;
  // there should be 4 tokens
  if (iNumTokens != 4) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!m_aToken2[3]->bEqual(";"_Str)) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a schema
  m_aToken[0]->vCopy("usesch"_Str);
  m_aToken[1]->vCopy(m_aToken2[2]);
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Schema changed: %s\n"_Str, m_aToken2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SePrompt::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
