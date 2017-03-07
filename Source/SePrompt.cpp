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

  eEC eResult = RapidsSE_SEitfNew(gpAttachMap, (void**)&m_pSEitf,m_iPartition);
  ASSERT(EC_NONE == eResult);
  
  m_bContinue = true;
  
  //m_sPrompt = "se ";
  //StrLib::vConcat(m_sPrompt.aStr(), RAPIDSSE_VERSION_STRING);
  //StrLib::vConcat(m_sPrompt.aStr(), "> ");
  
  StrLib::vSprint(m_sPrompt.aStr(),
                  "[N%lld %lld] se %s%c> "_Str,
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
    m_aToken[iToken]  = &m_xTokenArea.aArea()[C_TOKEN_SPACE * iToken];
    m_aToken2[iToken] = &m_xTokenArea2.aArea()[C_TOKEN_SPACE * iToken];
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
  m_xHistory.vClose();
  
  // SHOULD REMOVE HISTORY FILE?
  
  RapidsSE_SEitfDelete(gpAttachMap, m_pSEitf);
  
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
    
    if (0 == StrLib::iLength(m_aToken[0]))
      continue;
    
    bool b = bProcessCommand();
    
    if (m_bExpect) {
      if ( ! StrLib::bEqualTo(m_sExpect.aStr(),m_sResponse.aStr(),C_NO_CASE)) {
        vTrace("Expected \"%s\" : Got \"%s\"\n"_Str,m_sExpect.aStr(),m_sResponse.aStr());
        PosixThread::vSleepSec(10000);
      }
    }
    
    if (m_bEcho) {
      vTrace("cmd: "_Str);
      for (i8 iToken = 0; iToken < C_TOKENS; iToken++) {
        if (StrLib::bEqualTo("",m_aToken[iToken]))
          break;
        vTrace("%s "_Str,m_aToken[iToken]);
      }
      vTrace(": got: %s\n"_Str,m_sResponse.aStr());
      continue;
    }
    
    if (b) {
      if ( ! m_bExpect )
        if ( ! StrLib::bEqualTo(m_sResponse.aStr(), "<ok>") ) {
          vTrace("%s\n"_Str,m_sResponse.aStr());
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
  while (TK_NONE != m_pTokeniser->eNextToken(m_aToken[iToken])) {
    
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
  while (TK_NONE != m_pTokeniser->eNextToken(m_aToken[iToken])) {
    
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
bool SePrompt::bExecuteString(StrVal& sRequest, eEC& eErrorCode, char** ppRowPtr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_pTokeniser->vUseString(sRequest);
  i8 iToken = 0;
  while (TK_NONE != m_pTokeniser->eNextToken(m_aToken[iToken])) {
    
    //vTrace("vGetCommand : [%lld] %s\n"_Str,iToken,m_aToken[iToken]);
    
    iToken++;
    ASSERT(iToken < C_TOKENS);
  }
  bool bResult = bProcessCommand(false);
  
  eErrorCode = m_pSEitf->m_eErrorCode;
  *ppRowPtr = m_pSEitf->m_pRowPtr;
  
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
  
  //StrLib::vCopy(m_aToken[0],gxArguments.m_sCommand.aStr());
  //StrLib::vCopy(m_aToken[1],"");

  i8 iToken = 0;
  while (TK_NONE != m_pTokeniser->eNextToken(m_aToken[iToken])) {
    
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
    while (TK_NONE != m_pTokeniser->eNextToken(m_aToken[iToken])) {
      
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
  if (StrLib::bEqualTo("echo",m_aToken[0],C_NO_CASE))
    vSetEcho();
  else
    vClearEcho();
  
  // - test for expect prefix
  if (StrLib::bEqualTo("expect",m_aToken[0],C_NO_CASE))
    vSetExpect();
  else
    vClearExpect();
 
  
  // - test for builtin comands
  
  if (StrLib::bEqualTo("?",   m_aToken[0],C_NO_CASE) ||
      StrLib::bEqualTo("help",m_aToken[0],C_NO_CASE)) {
    vHelp();
    return true;
  }
  
  if (StrLib::bEqualTo("bye",m_aToken[0],C_NO_CASE)) {
    m_bContinue = false;
    return true;
  }
  
  if (StrLib::bEqualTo("sleep",m_aToken[0],C_NO_CASE)) {
    return bSleep();
  }
  
  if (StrLib::bEqualTo("cd",m_aToken[0],C_NO_CASE)) {
    return bChangeWorkingDir();
  }
  
  if (StrLib::bEqualTo("cf",m_aToken[0],C_NO_CASE)) {
    return bExecuteRequest();
  }
  
  if (StrLib::bEqualTo("docol",m_aToken[0],C_NO_CASE)) {
    return bDocol();
  }
  
  if (StrLib::bEqualTo("history",m_aToken[0],C_NO_CASE)) {
    m_xHistory.vPrint();
    return true;
  }
  
  if (StrLib::bEqualTo("!!",m_aToken[0],C_NO_CASE)) {
    if (! m_xHistory.bRecall(m_aToken,C_TOKENS))
      return true;
  }
  
  if ('!' == *m_aToken[0]) {
    if (! m_xHistory.bRecall(m_aToken,C_TOKENS))
      return true;
  }
  
  if (StrLib::bEqualTo("begin",m_aToken[0],C_NO_CASE)) {
    if (StrLib::bEqualTo("insert",m_aToken[1],C_NO_CASE)) {
      return bBeginInsert();
    }
  }
  
  if (StrLib::bEqualTo("create",m_aToken[0],C_NO_CASE)) {
    
    if (StrLib::bEqualTo("tenant",m_aToken[1],C_NO_CASE)) {
      return bCreateTenant();
    }
    if (StrLib::bEqualTo("catalog",m_aToken[1],C_NO_CASE)) {
      return bCreateCatalog();
    }
    if (StrLib::bEqualTo("schema",m_aToken[1],C_NO_CASE)) {
      return bCreateSchema();
    }
    if (StrLib::bEqualTo("table",m_aToken[1],C_NO_CASE)) {
      return bCreateTable();
    }
    if (StrLib::bEqualTo("index",m_aToken[1],C_NO_CASE)) {
      return bCreateIndex();
    }
  }
  
  if (StrLib::bEqualTo("connectCluster",m_aToken[0],C_NO_CASE)) {
    return bConnectCluster();
  }
  
  if (StrLib::bEqualTo("drop",m_aToken[0],C_NO_CASE)) {
    if (StrLib::bEqualTo("tenant",m_aToken[1],C_NO_CASE)) {
      return bDropTenant();
    }
    if (StrLib::bEqualTo("catalog",m_aToken[1],C_NO_CASE)) {
      return bDropCatalog();
    }
    if (StrLib::bEqualTo("index",m_aToken[1],C_NO_CASE)) {
      return bDropIndex();
    }
    if (StrLib::bEqualTo("schema",m_aToken[1],C_NO_CASE)) {
      return bDropSchema();
    }
    if (StrLib::bEqualTo("table",m_aToken[1],C_NO_CASE)) {
      return bDropTable();
    }
  }
  
  if (StrLib::bEqualTo("end",m_aToken[0],C_NO_CASE)) {
    if (StrLib::bEqualTo("insert",m_aToken[1],C_NO_CASE)) {
      return bEndInsert();
    }
  }
  
  if (StrLib::bEqualTo("antbl",m_aToken[0],C_NO_CASE)) {
    return bAnalyzeTable();
  }
  
  if (StrLib::bEqualTo("dantbl",m_aToken[0],C_NO_CASE)) {
    return bDanalyzeTable();
  }
  
  if (StrLib::bEqualTo("pwd",m_aToken[0],C_NO_CASE)) {
    return bPrintWorkingDir();
  }
  
  if (StrLib::bEqualTo("ls",m_aToken[0],C_NO_CASE)) {
    return bListDir();
  }
  
  if (StrLib::bEqualTo("first",m_aToken[0],C_NO_CASE)) {
    return bGetFirstRow();
  }
  
  if (StrLib::bEqualTo("insert",m_aToken[0],C_NO_CASE)) {
    if (StrLib::bEqualTo("row",m_aToken[1],C_NO_CASE)) {
      return bInsertRow();
    }
  }
  
  if (StrLib::bEqualTo("next",m_aToken[0],C_NO_CASE)) {
    return bGetNextRow();
  }

  if (StrLib::bEqualTo("object",m_aToken[0],C_NO_CASE)) {
    return bObject();
  }
  
  if (StrLib::bEqualTo("space",m_aToken[0],C_NO_CASE)) {
    return bPoolSpace();
  }

  if (StrLib::bEqualTo("use",m_aToken[0],C_NO_CASE)) {
    if (StrLib::bEqualTo("schema",m_aToken[1],C_NO_CASE)) {
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
  if (StrLib::bEqualTo("sel",m_aToken[0],C_NO_CASE)) {
    vIterateRows();
    return bReturnTable();
  }
  
  // - the following never return
  
  if (StrLib::bEqualTo("mon",m_aToken[0],C_NO_CASE)) {
    while (true) {
      PosixThread::vSleepSec(10);
      if (!bExecuteRequest())
        return true;
    }
  }
  
  if (StrLib::bEqualTo("x",m_aToken[0],C_NO_CASE)) {
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
    
    StrLib::vCopy(m_aToken[iToToken++],m_aToken[iFromToken]);
    
    if (StrLib::bEqualTo("",m_aToken[iFromToken]))
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
  char* aResponse = nullptr;
  i8 iTokens;
  
  xTimer.vStart();
  
  // get count of words in pWords
  for (iTokens = 0; iTokens < C_TOKENS; iTokens++)
    if (StrLib::bEqualTo("",m_aToken[iTokens]))
      break;
  
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
  
  i8 iAddress = StrLib::iToI8(m_sResponse.aStr());
  
  SelCursor* pSelCursor = reinterpret_cast<SelCursor*>(iAddress);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  i8 iRowCount = 0;
  
  for (i8 iPartition = 0; iPartition < gxArguments.m_nSlots; iPartition++) {
    vTrace("Partition %lld: \n"_Str, Partition::iSlotToPartition(iPartition));
    pSelCursor->vBegin(iPartition);
    pSelCursor->vFirst();
    char* pStr = pSelCursor->pRowAsStr();
    while (pStr) {
      iRowCount++;
      vTrace("Row: %s\n"_Str,pStr);
      pSelCursor->vNext();
      pStr = pSelCursor->pRowAsStr();
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
  
  StrLib::vCopy(m_aToken[0], "rettbl");
  StrLib::vCopy(m_aToken[1], "");
  
  if (!bExecuteRequest()) {
    vTrace("bReturnTable: (%s) Return Table FAILED!!! : %s\n"_Str, m_sResponse.aStr());
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
  
  i8 iAddress = StrLib::iToI8(m_sResponse.aStr());
  
  SelCursor* pSelCursor = reinterpret_cast<SelCursor*>(iAddress);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  pSelCursor->vBegin(m_iPartition);
  pSelCursor->vFirst();
  char* pStr = pSelCursor->pRowAsStr();
  
  if (pStr == nullptr) {
    pSelCursor->vEnd();
    delete pSelCursor;
    m_pSEitf->m_pRowPtr = nullptr;
    return false;
  }
  else {
    m_pSEitf->m_pRowPtr = pStr;
    return true;
  }
}

/*----------------------------------------------------------------------------*/
/* get the next row                                                           */
/*----------------------------------------------------------------------------*/
bool SePrompt::bGetNextRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iAddress = StrLib::iToI8(m_sResponse.aStr());
  
  SelCursor* pSelCursor = reinterpret_cast<SelCursor*>(iAddress);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  pSelCursor->vNext();
  char* pStr = pSelCursor->pRowAsStr();
  
  if (pStr == nullptr) {
    pSelCursor->vEnd();
    delete pSelCursor;
    m_pSEitf->m_pRowPtr = nullptr;
    
    // now return the table
    bool bResult = bReturnTable();
    ASSERT(bResult);
    return false;
  }
  else {
    m_pSEitf->m_pRowPtr = pStr;
    return true;
  }
}

/*----------------------------------------------------------------------------*/
/* test for command file                                                      */
/*----------------------------------------------------------------------------*/
bool SePrompt::bTestFile(char* pWord)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  File xFile;
  
  if (xFile.bExists((Str*)pWord))
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* push command file                                                          */
/*----------------------------------------------------------------------------*/
void SePrompt::vPushFile(char* pPath)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  s256 sPath(pPath);
  
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
  
  ASSERT(StrLib::bEqualTo(m_aToken[0],"begin"));
  ASSERT(StrLib::bEqualTo(m_aToken[1],"insert"));
  
  vCollectStatement();
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_aToken[iToken])
      break;
    StrLib::vCopy(m_aToken2[iToken], m_aToken[iToken]);
    StrLib::vCopy(m_aToken[iToken], "");
  }
  
  iNumTokens = iToken;
  // there should be 4 tokens
  if (iNumTokens != 4) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!StrLib::bEqualTo(m_aToken2[3] , ";")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // get table pointer
  StrLib::vCopy(m_aToken[0], "gettbl");
  StrLib::vCopy(m_aToken[1], m_aToken2[2]);
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
  
  ASSERT(StrLib::bEqualTo(m_aToken[0],"sleep"));
  
  i8 nSeconds = StrLib::iToI8(m_aToken[1]);

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
  
  ASSERT(StrLib::bEqualTo(m_aToken[0],"cd"));
  
  StrLib::vCopy(sDir.aStr(), m_aToken[1]);
  
  i8 iToken;
  for (iToken = 2; iToken < C_TOKENS; iToken++) {
    if (! *m_aToken[iToken])
      break;
    StrLib::vConcat(sDir.aStr(), m_aToken[iToken]);
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
  
  ASSERT(StrLib::bEqualTo(m_aToken[0],"antbl"));
  
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
  
  ASSERT(StrLib::bEqualTo(m_aToken[0],"dantbl"));
  
  return bExecuteRequest();
}

/*----------------------------------------------------------------------------*/
/* docol                                                                      */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDocol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(StrLib::bEqualTo(m_aToken[0],"docol"));
  return bExecuteRequest();
}

/*----------------------------------------------------------------------------*/
/* create catalog                                                             */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCreateCatalog(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(StrLib::bEqualTo(m_aToken[0],"create"));
  ASSERT(StrLib::bEqualTo(m_aToken[1],"catalog"));
  vCollectStatement();
  
  i8 iToken;
  
  // copy tokens from m_aToken to m_aToken2
  // while doing that also validate the create statament
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_aToken[iToken])
      break;
    StrLib::vCopy(m_aToken2[iToken], m_aToken[iToken]);
    StrLib::vCopy(m_aToken[iToken], "");
  }
  
  i8 iNumTokens = iToken;
  // there should be 4 tokens
  if (iNumTokens != 4) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!StrLib::bEqualTo(m_aToken2[3] , ";")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // create a catalog
  StrLib::vCopy(m_aToken[0], "mkcat");
  StrLib::vCopy(m_aToken[1], m_aToken2[2]);
  
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
  
  ASSERT(StrLib::bEqualTo(m_aToken[0],"create"));
  ASSERT(StrLib::bEqualTo(m_aToken[1],"index"));
  vCollectStatement();
  
  i8 iToken;
  
  // copy tokens from m_aToken to m_aToken2
  // while doing that also validate the create statament
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_aToken[iToken])
      break;
    StrLib::vCopy(m_aToken2[iToken], m_aToken[iToken]);
    StrLib::vCopy(m_aToken[iToken], "");
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
  if (!StrLib::bEqualTo(m_aToken2[3] , "on")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // token[5] should be '('
  if (!StrLib::bEqualTo(m_aToken2[5] , "(")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // 7 token should be ')'
  if (!StrLib::bEqualTo(m_aToken2[7] , ")")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!StrLib::bEqualTo(m_aToken2[iNumTokens-1] , ";")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // create the index
  StrLib::vCopy(m_aToken[0], "mkidx");
  StrLib::vCopy(m_aToken[1], m_aToken2[2]);
  StrLib::vCopy(m_aToken[2], m_aToken2[4]);
  StrLib::vCopy(m_aToken[3], m_aToken2[6]);
  
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

  ASSERT(StrLib::bEqualTo(m_aToken[0],"create"));
  ASSERT(StrLib::bEqualTo(m_aToken[1],"schema"));
  vCollectStatement();
  
  i8 iToken;
  
  // copy tokens from m_aToken to m_aToken2
  // while doing that also validate the create statament
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_aToken[iToken])
      break;
    StrLib::vCopy(m_aToken2[iToken], m_aToken[iToken]);
    StrLib::vCopy(m_aToken[iToken], "");
  }
  
  i8 iNumTokens = iToken;
  
  // there should be 4 tokens
  if (iNumTokens != 4) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!StrLib::bEqualTo(m_aToken2[3] , ";")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // create a schema
  StrLib::vCopy(m_aToken[0], "mksch");
  StrLib::vCopy(m_aToken[1], m_aToken2[2]);
  
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
  
  ASSERT(StrLib::bEqualTo(m_aToken[0],"create"));
  ASSERT(StrLib::bEqualTo(m_aToken[1],"tenant"));
  vCollectStatement();
  
  i8 iToken;
  
  // copy tokens from m_aToken to m_aToken2
  // while doing that also validate the create statament
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_aToken[iToken])
      break;
    StrLib::vCopy(m_aToken2[iToken], m_aToken[iToken]);
    StrLib::vCopy(m_aToken[iToken], "");
  }
  
  i8 iNumTokens = iToken;
  
  // there should be 4 tokens
  if (iNumTokens != 4) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!StrLib::bEqualTo(m_aToken2[3] , ";")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // create a tenant
  StrLib::vCopy(m_aToken[0], "mkten");
  StrLib::vCopy(m_aToken[1], m_aToken2[2]);
  
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
  
  ASSERT(StrLib::bEqualTo(m_aToken[0],"create"));
  ASSERT(StrLib::bEqualTo(m_aToken[1],"table"));
  vCollectStatement();
  
  i8 iToken;
  
  // copy tokens from m_aToken to m_aToken2
  // while doing that also validate the create statament
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_aToken[iToken])
      break;
    StrLib::vCopy(m_aToken2[iToken], m_aToken[iToken]);
    StrLib::vCopy(m_aToken[iToken], "");
  }
  
  i8 iValue;
  i8 iNumTokens = iToken;
  
  // some validations
  /// there should be a minumum of 8 tokens
  if (iNumTokens < 8) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // token[3] should be '('
  if (!StrLib::bEqualTo(m_aToken2[3] , "(")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last-1 token should be ')'
  if (!StrLib::bEqualTo(m_aToken2[iNumTokens-2] , ")")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
    
  // last token should be ';'
  if (!StrLib::bEqualTo(m_aToken2[iNumTokens-1] , ";")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
      
  // create the table
  StrLib::vCopy(m_aToken[0], "mktbl");
  StrLib::vCopy(m_aToken[1], m_aToken2[2]);
  
  if (!bExecuteRequest())
    return false;
  
  // change folder to table
  StrLib::vCopy(m_aToken[0], "cf");
  StrLib::vCopy(m_aToken[1], m_aToken2[2]);
  
  if (!bExecuteRequest()) {
    vTrace("SePrompt::bCreateTable : cf FAILED!!!\n"_Str);
    
    // create the table
    StrLib::vCopy(m_aToken[0], "rmtbl");
    StrLib::vCopy(m_aToken[1], m_aToken2[2]);
    
    if (!bExecuteRequest()) {
      return false;
    }
    
    return false;
  }
  
  // get reference to pTable
  StrLib::vCopy(m_aToken[0], "gettbl");
  StrLib::vCopy(m_aToken[1], m_aToken2[2]);
  
  if (!bExecuteRequest()) {
    vTrace("SePrompt::bCreateTable : gettbl FAILED!!!\n"_Str);
    
    // create the table
    StrLib::vCopy(m_aToken[0], "rmtbl");
    StrLib::vCopy(m_aToken[1], m_aToken2[2]);
    
    if (!bExecuteRequest()) {      
      return false;
    }
    
    return false;
  }
  
  // now collect the column names and their types
  i8 iCurrentToken = 4;
  while(iCurrentToken < iNumTokens - 2) {
    
    if (iCurrentToken != 4 && !StrLib::bEqualTo(m_aToken2[iCurrentToken++],",")) {
      bDropReturnTable(m_aToken2[2]);
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      return false;
    }
    
    // add columns to the table
    i8 j = 0;
    StrLib::vCopy(m_aToken[j++], "add");
    StrLib::vCopy(m_aToken[j++], m_aToken2[2]);
    StrLib::vCopy(m_aToken[j++], m_aToken2[iCurrentToken++]);
    
    if (StrLib::bEqualTo(m_aToken2[iCurrentToken], "integer", C_NO_CASE) ||
        StrLib::bEqualTo(m_aToken2[iCurrentToken], "varchar", C_NO_CASE) ||
        StrLib::bEqualTo(m_aToken2[iCurrentToken], "varbinary", C_NO_CASE) ||
        StrLib::bEqualTo(m_aToken2[iCurrentToken], "char", C_NO_CASE) ||
        StrLib::bEqualTo(m_aToken2[iCurrentToken], "float", C_NO_CASE)) {
      
      // copy varchar/char
      StrLib::vCopy(m_aToken[j], m_aToken2[iCurrentToken++]);
      
      if (StrLib::bEqualTo(m_aToken2[iCurrentToken], "(")) {
        
        // str.cat '('
        StrLib::vConcat(m_aToken[j], m_aToken2[iCurrentToken++]);
        
        // str.cat the size
        StrLib::vConcat(m_aToken[j], m_aToken2[iCurrentToken]);
        
        // check if it was numeric
        if (!StrLib::bExtractI8(&m_aToken2[iCurrentToken++], &iValue)) {
          // drop the table
          bDropReturnTable(m_aToken2[2]);
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        }
        
        if (!StrLib::bEqualTo(m_aToken2[iCurrentToken],")")) {
          // drop the table
          bDropReturnTable(m_aToken2[2]);
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        }
        
        // str.cat ')'
        StrLib::vConcat(m_aToken[j], m_aToken2[iCurrentToken++]);
        
      }
    }
    else if (StrLib::bEqualTo(m_aToken2[iCurrentToken], "decimal", C_NO_CASE)) {
      
      // copy varchar/char
      StrLib::vCopy(m_aToken[j], m_aToken2[iCurrentToken++]);
      
      if (StrLib::bEqualTo(m_aToken2[iCurrentToken], "(")) {
        
        // str.cat '('
        StrLib::vConcat(m_aToken[j], m_aToken2[iCurrentToken++]);
        
        // str.cat the size
        StrLib::vConcat(m_aToken[j], m_aToken2[iCurrentToken]);
        
        // check if it was numeric
        if (!StrLib::bExtractI8(&m_aToken2[iCurrentToken++], &iValue)) {
          bDropReturnTable(m_aToken2[2]);
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        }
        
        if (!StrLib::bEqualTo(m_aToken2[iCurrentToken],",")) {
          bDropReturnTable(m_aToken2[2]);
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        }
        
        // str.cat ','
        StrLib::vConcat(m_aToken[j], m_aToken2[iCurrentToken++]);
        
        // str.cat the size
        StrLib::vConcat(m_aToken[j], m_aToken2[iCurrentToken]);
        
        // check if it was numeric
        if (!StrLib::bExtractI8(&m_aToken2[iCurrentToken++], &iValue)) {
          bDropReturnTable(m_aToken2[2]);
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        }
        
        if (!StrLib::bEqualTo(m_aToken2[iCurrentToken],")")) {
          bDropReturnTable(m_aToken2[2]);
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        }
        
        // str.cat ')'
        StrLib::vConcat(m_aToken[j], m_aToken2[iCurrentToken++]);
      }
    }
    else if (StrLib::bEqualTo(m_aToken2[iCurrentToken], "date", C_NO_CASE) ||
             StrLib::bEqualTo(m_aToken2[iCurrentToken], "timestamp", C_NO_CASE) ||
             StrLib::bEqualTo(m_aToken2[iCurrentToken], "interval", C_NO_CASE) ||
             StrLib::bEqualTo(m_aToken2[iCurrentToken], "boolean", C_NO_CASE)) {
      StrLib::vCopy(m_aToken[j], m_aToken2[iCurrentToken++]);
    }
    else {
      // drop the table that was created
      bDropReturnTable(m_aToken2[2]);
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      return false;
    }
    
    // check for NOT NULL
    if (StrLib::bEqualTo(m_aToken2[iCurrentToken],"not", C_NO_CASE)) {
      
      StrLib::vConcat(m_aToken[j], " ");
      StrLib::vConcat(m_aToken[j], m_aToken2[iCurrentToken++]);
      
      if (!StrLib::bEqualTo(m_aToken2[iCurrentToken],"null", C_NO_CASE)) {
        bDropReturnTable(m_aToken2[2]);
        m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
        return false;
      }
      StrLib::vConcat(m_aToken[j], " ");
      StrLib::vConcat(m_aToken[j], m_aToken2[iCurrentToken++]);
    }
    
    // check for PRIMARY KEY
    if (StrLib::bEqualTo(m_aToken2[iCurrentToken],"primary", C_NO_CASE)) {
      
      StrLib::vConcat(m_aToken[j], " ");
      StrLib::vConcat(m_aToken[j], m_aToken2[iCurrentToken++]);
      
      if (!StrLib::bEqualTo(m_aToken2[iCurrentToken],"key", C_NO_CASE)) {
        bDropReturnTable(m_aToken2[2]);
        m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
        return false;
      }
      StrLib::vConcat(m_aToken[j], " ");
      StrLib::vConcat(m_aToken[j], m_aToken2[iCurrentToken++]);
    }

    // check for UNIQUE
    if (StrLib::bEqualTo(m_aToken2[iCurrentToken],"unique", C_NO_CASE)) {
      
      StrLib::vConcat(m_aToken[j], " ");
      StrLib::vConcat(m_aToken[j], m_aToken2[iCurrentToken++]);
    }

    
    // now we should see a column type
    // validate it
    StrLib::vToLower(m_aToken[j]);
    
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
  ASSERT(! StrLib::bEqualTo(m_aToken[0],""));

  // have we already seen the ';'
  i8 iToken = m_pTokeniser->iFindToken(";"_Str,m_aToken,C_TOKENS);
  
  if (iToken >= 0) {
    //m_pTokeniser->vDumpTokens("\nCommand : "_Str,m_aToken,C_TOKENS);
    return;
  }
  
  iToken = m_pTokeniser->iCountTokens(m_aToken,C_TOKENS);
  
  // switch to terminal mode
  m_pTokeniser->vLineMode(false);
  
  s32 sContinuation("  > ");
  m_pTokeniser->vPrompt(sContinuation);
  
  // get tokens delimited by ';'
  for ( ; iToken < C_TOKENS; iToken++) {
    
    eTK eToken = m_pTokeniser->eNextToken(m_aToken[iToken]);
    ASSERT(TK_NONE != eToken);
    
    // look for terminal
    if (StrLib::bEqualTo(m_aToken[iToken],";"))
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
  
  ASSERT(StrLib::bEqualTo(m_aToken[0],"connectCluster"));
  
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
  
  ASSERT(StrLib::bEqualTo(m_aToken[0],"drop"));
  ASSERT(StrLib::bEqualTo(m_aToken[1],"catalog"));
  
  vCollectStatement();
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_aToken[iToken])
      break;
    StrLib::vCopy(m_aToken2[iToken], m_aToken[iToken]);
    StrLib::vCopy(m_aToken[iToken], "");
  }
  
  iNumTokens = iToken;
  
  // there should be 4 tokens
  if (iNumTokens != 4) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!StrLib::bEqualTo(m_aToken2[3] , ";")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a catalog
  StrLib::vCopy(m_aToken[0], "rmcat");
  StrLib::vCopy(m_aToken[1], m_aToken2[2]);
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
  
  ASSERT(StrLib::bEqualTo(m_aToken[0],"drop"));
  ASSERT(StrLib::bEqualTo(m_aToken[1],"index"));
  
  vCollectStatement();
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_aToken[iToken])
      break;
    StrLib::vCopy(m_aToken2[iToken], m_aToken[iToken]);
    StrLib::vCopy(m_aToken[iToken], "");
  }
  
  iNumTokens = iToken;
  
  // there should be 4 tokens
  if (iNumTokens != 4) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!StrLib::bEqualTo(m_aToken2[3] , ";")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a table
  StrLib::vCopy(m_aToken[0], "rmidx");
  StrLib::vCopy(m_aToken[1], m_aToken2[2]);
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
  
  ASSERT(StrLib::bEqualTo(m_aToken[0],"drop"));
  ASSERT(StrLib::bEqualTo(m_aToken[1],"schema"));
  
  vCollectStatement();
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_aToken[iToken])
      break;
    StrLib::vCopy(m_aToken2[iToken], m_aToken[iToken]);
    StrLib::vCopy(m_aToken[iToken], "");
  }
  
  iNumTokens = iToken;
  // there should be 4 tokens
  if (iNumTokens != 4) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!StrLib::bEqualTo(m_aToken2[3] , ";")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a schema
  StrLib::vCopy(m_aToken[0], "rmsch");
  StrLib::vCopy(m_aToken[1], m_aToken2[2]);
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
  
  ASSERT(StrLib::bEqualTo(m_aToken[0],"drop"));
  ASSERT(StrLib::bEqualTo(m_aToken[1],"table"));
  
  vCollectStatement();
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_aToken[iToken])
      break;
    StrLib::vCopy(m_aToken2[iToken], m_aToken[iToken]);
    StrLib::vCopy(m_aToken[iToken], "");
  }
  
  iNumTokens = iToken;
  
  // there should be 4 tokens
  if (iNumTokens != 4) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!StrLib::bEqualTo(m_aToken2[3] , ";")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a table
  StrLib::vCopy(m_aToken[0], "rmtbl");
  StrLib::vCopy(m_aToken[1], m_aToken2[2]);
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Table dropped: %s\n"_Str, m_aToken2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop table                                                                 */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDropReturnTable(char* pTableName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // derefernce and return the table first
  bool bResult = bReturnTable();
  ASSERT(bResult);
  
  // change folder to table
  StrLib::vCopy(m_aToken[0], "cf");
  StrLib::vCopy(m_aToken[1], "..");
  
  if (!bExecuteRequest()) {
    return false;
  }
  StrLib::vCopy(m_aToken[0], "rmtbl");
  StrLib::vCopy(m_aToken[1], pTableName);
  StrLib::vCopy(m_aToken[2], "");
  
  if (!bExecuteRequest()) {
    //vTrace("Error: (%s) Drop Table %s\n"_Str, m_sResponse.aStr(), pTableName);
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
  
  ASSERT(StrLib::bEqualTo(m_aToken[0],"drop"));
  ASSERT(StrLib::bEqualTo(m_aToken[1],"tenant"));
  
  vCollectStatement();
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_aToken[iToken])
      break;
    StrLib::vCopy(m_aToken2[iToken], m_aToken[iToken]);
    StrLib::vCopy(m_aToken[iToken], "");
  }
  
  iNumTokens = iToken;
  
  // there should be 4 tokens
  if (iNumTokens != 4) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!StrLib::bEqualTo(m_aToken2[3] , ";")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a tenant
  StrLib::vCopy(m_aToken[0], "rmten");
  StrLib::vCopy(m_aToken[1], m_aToken2[2]);
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
  
  ASSERT(StrLib::bEqualTo(m_aToken[0],"end"));
  ASSERT(StrLib::bEqualTo(m_aToken[1],"insert"));
  
  vCollectStatement();
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_aToken[iToken])
      break;
    StrLib::vCopy(m_aToken2[iToken], m_aToken[iToken]);
    StrLib::vCopy(m_aToken[iToken], "");
  }
  
  iNumTokens = iToken;
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!StrLib::bEqualTo(m_aToken2[3] , ";")) {
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
  
  //eEC eResult = eExecuteRequest(m_pSEitf, "rettbl");
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
  
  ASSERT(StrLib::bEqualTo(m_aToken[0],"insert"));
  ASSERT(StrLib::bEqualTo(m_aToken[1],"row"));
  
  vCollectStatement();
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_aToken[iToken])
      break;
    StrLib::vCopy(m_aToken2[iToken], m_aToken[iToken]);
    StrLib::vCopy(m_aToken[iToken], "");
  }
  
  iNumTokens = iToken;
  // there should be 3 tokens
  if (iNumTokens != 4) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!StrLib::bEqualTo(m_aToken2[3] , ";")) {
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
  
  char* pRow = m_aToken2[2];
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
  
  ASSERT(StrLib::bEqualTo(m_aToken[0],"ls"));
  
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
    vTrace("%c %s\n"_Str, iEntryType == DT_DIR ? 'd' : 'f', sDirEntry.aStr());
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
  
  ASSERT(StrLib::bEqualTo(m_aToken[0],"object"));
  
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
  
  ASSERT(StrLib::bEqualTo(m_aToken[0],"space"));
    
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
  
  ASSERT(StrLib::bEqualTo(m_aToken[0],"status"));
  
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
  
  ASSERT(StrLib::bEqualTo(m_aToken[0],"pwd"));
  
  if (xDir.bGetCurrent(sDir)) {
    vTrace("%s"_Str, sDir.aStr());
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
  
  ASSERT(StrLib::bEqualTo(m_aToken[0],"use"));
  ASSERT(StrLib::bEqualTo(m_aToken[1],"schema"));
  
  vCollectStatement();
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_aToken[iToken])
      break;
    StrLib::vCopy(m_aToken2[iToken], m_aToken[iToken]);
    StrLib::vCopy(m_aToken[iToken], "");
  }
  
  iNumTokens = iToken;
  // there should be 4 tokens
  if (iNumTokens != 4) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!StrLib::bEqualTo(m_aToken2[3] , ";")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a schema
  StrLib::vCopy(m_aToken[0], "usesch");
  StrLib::vCopy(m_aToken[1], m_aToken2[2]);
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
