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
  //String::vConcat(m_sPrompt.aStr(), RAPIDSSE_VERSION_STRING);
  //String::vConcat(m_sPrompt.aStr(), "> ");
  
  String::vSprint(m_sPrompt.aStr(),
                  "[N%lld %lld] se %s%c> ",
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
  m_sExpect = "";
  
  m_sResponse  = "";
  //m_xHistory
  
  // initialise the token set
  for (i8 iToken = 0; iToken < C_TOKENS; iToken++) {
    m_pToken[iToken]  = &m_xTokenArea.aWspArea()[C_TOKEN_SPACE * iToken];
    m_pToken2[iToken] = &m_xTokenArea2.aWspArea()[C_TOKEN_SPACE * iToken];
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
void SePrompt::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_SEPROMPT);
  ASSERT(pObject->eClass() == CN_SEPROMPT);
  
  SePrompt* pSePrompt = reinterpret_cast<SePrompt*>(pObject);
  
  vTrace("partition %lld seitf %p continue %s\n",
         pSePrompt->m_iPartition,
         pSePrompt->m_pSEitf,
         pSePrompt->m_bContinue ? "true" : "false");
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
    
    if (0 == String::iLength(m_pToken[0]))
      continue;
    
    bool b = bProcessCommand();
    
    if (m_bExpect) {
      if ( ! String::bEqualTo(m_sExpect.aStr(),m_sResponse.aStr(),C_NO_CASE)) {
        vTrace("Expected \"%s\" : Got \"%s\"\n",m_sExpect.aStr(),m_sResponse.aStr());
        PosixThread::vSleepSec(10000);
      }
    }
    
    if (m_bEcho) {
      vTrace("cmd: ");
      for (i8 iToken = 0; iToken < C_TOKENS; iToken++) {
        if (String::bEqualTo("",m_pToken[iToken]))
          break;
        vTrace("%s ",m_pToken[iToken]);
      }
      vTrace(": got: %s\n",m_sResponse.aStr());
      continue;
    }
    
    if (b) {
      if ( ! m_bExpect )
        if ( ! String::bEqualTo(m_sResponse.aStr(), "<ok>") ) {
          vTrace("%s\n",m_sResponse.aStr());
      }
    }
    else {
      if (m_pSEitf->eGetLastError() != EC_NONE)
        if ( ! m_bExpect )
         vTrace("Error: %s\n",m_pSEitf->pGetLastError());
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
  while (TK_NONE != m_pTokeniser->eNextToken(m_pToken[iToken])) {
    
    //vTrace("vGetCommand : [%lld] %s\n",iToken,m_pToken[iToken]);
    
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
  while (TK_NONE != m_pTokeniser->eNextToken(m_pToken[iToken])) {
    
    //vTrace("vGetCommand : [%lld] %s\n",iToken,m_pToken[iToken]);
    
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
  while (TK_NONE != m_pTokeniser->eNextToken(m_pToken[iToken])) {
    
    //vTrace("vGetCommand : [%lld] %s\n",iToken,m_pToken[iToken]);
    
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
  
  //String::vCopy(m_pToken[0],gxArguments.m_sCommand.aStr());
  //String::vCopy(m_pToken[1],"");

  i8 iToken = 0;
  while (TK_NONE != m_pTokeniser->eNextToken(m_pToken[iToken])) {
    
    //vTrace("vGetCommand : [%lld] %s\n",iToken,m_pToken[iToken]);
    
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
    
    m_pTokeniser->vResetTokens(m_pToken,C_TOKENS);
    
    i8 iToken = 0;
    while (TK_NONE != m_pTokeniser->eNextToken(m_pToken[iToken])) {
      
      //vTrace("vGetCommand : [%lld] %s\n",iToken,m_pToken[iToken]);
      
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
  if (String::bEqualTo("echo",m_pToken[0],C_NO_CASE))
    vSetEcho();
  else
    vClearEcho();
  
  // - test for expect prefix
  if (String::bEqualTo("expect",m_pToken[0],C_NO_CASE))
    vSetExpect();
  else
    vClearExpect();
 
  
  // - test for builtin comands
  
  if (String::bEqualTo("?",   m_pToken[0],C_NO_CASE) ||
      String::bEqualTo("help",m_pToken[0],C_NO_CASE)) {
    vHelp();
    return true;
  }
  
  if (String::bEqualTo("bye",m_pToken[0],C_NO_CASE)) {
    m_bContinue = false;
    return true;
  }
  
  if (String::bEqualTo("sleep",m_pToken[0],C_NO_CASE)) {
    return bSleep();
  }
  
  if (String::bEqualTo("cd",m_pToken[0],C_NO_CASE)) {
    return bChangeWorkingDir();
  }
  
  if (String::bEqualTo("cf",m_pToken[0],C_NO_CASE)) {
    return bExecuteRequest();
  }
  
  if (String::bEqualTo("docol",m_pToken[0],C_NO_CASE)) {
    return bDocol();
  }
  
  if (String::bEqualTo("history",m_pToken[0],C_NO_CASE)) {
    m_xHistory.vPrint();
    return true;
  }
  
  if (String::bEqualTo("!!",m_pToken[0],C_NO_CASE)) {
    if (! m_xHistory.bRecall(m_pToken,C_TOKENS))
      return true;
  }
  
  if ('!' == *m_pToken[0]) {
    if (! m_xHistory.bRecall(m_pToken,C_TOKENS))
      return true;
  }
  
  if (String::bEqualTo("begin",m_pToken[0],C_NO_CASE)) {
    if (String::bEqualTo("insert",m_pToken[1],C_NO_CASE)) {
      return bBeginInsert();
    }
  }
  
  if (String::bEqualTo("create",m_pToken[0],C_NO_CASE)) {
    
    if (String::bEqualTo("tenant",m_pToken[1],C_NO_CASE)) {
      return bCreateTenant();
    }
    if (String::bEqualTo("catalog",m_pToken[1],C_NO_CASE)) {
      return bCreateCatalog();
    }
    if (String::bEqualTo("schema",m_pToken[1],C_NO_CASE)) {
      return bCreateSchema();
    }
    if (String::bEqualTo("table",m_pToken[1],C_NO_CASE)) {
      return bCreateTable();
    }
    if (String::bEqualTo("index",m_pToken[1],C_NO_CASE)) {
      return bCreateIndex();
    }
  }
  
  if (String::bEqualTo("connectCluster",m_pToken[0],C_NO_CASE)) {
    return bConnectCluster();
  }
  
  if (String::bEqualTo("drop",m_pToken[0],C_NO_CASE)) {
    if (String::bEqualTo("tenant",m_pToken[1],C_NO_CASE)) {
      return bDropTenant();
    }
    if (String::bEqualTo("catalog",m_pToken[1],C_NO_CASE)) {
      return bDropCatalog();
    }
    if (String::bEqualTo("schema",m_pToken[1],C_NO_CASE)) {
      return bDropSchema();
    }
    if (String::bEqualTo("table",m_pToken[1],C_NO_CASE)) {
      return bDropTable();
    }
  }
  
  if (String::bEqualTo("end",m_pToken[0],C_NO_CASE)) {
    if (String::bEqualTo("insert",m_pToken[1],C_NO_CASE)) {
      return bEndInsert();
    }
  }
  
  if (String::bEqualTo("antbl",m_pToken[0],C_NO_CASE)) {
    return bAnalyzeTable();
  }
  
  if (String::bEqualTo("dantbl",m_pToken[0],C_NO_CASE)) {
    return bDanalyzeTable();
  }
  
  if (String::bEqualTo("pwd",m_pToken[0],C_NO_CASE)) {
    return bPrintWorkingDir();
  }
  
  if (String::bEqualTo("ls",m_pToken[0],C_NO_CASE)) {
    return bListDir();
  }
  
  if (String::bEqualTo("first",m_pToken[0],C_NO_CASE)) {
    return bGetFirstRow();
  }
  
  if (String::bEqualTo("insert",m_pToken[0],C_NO_CASE)) {
    if (String::bEqualTo("row",m_pToken[1],C_NO_CASE)) {
      return bInsertRow();
    }
  }
  
  if (String::bEqualTo("next",m_pToken[0],C_NO_CASE)) {
    return bGetNextRow();
  }

  if (String::bEqualTo("object",m_pToken[0],C_NO_CASE)) {
    return bObject();
  }
  
  if (String::bEqualTo("space",m_pToken[0],C_NO_CASE)) {
    return bPoolSpace();
  }

  if (String::bEqualTo("use",m_pToken[0],C_NO_CASE)) {
    if (String::bEqualTo("schema",m_pToken[1],C_NO_CASE)) {
      return bUseSchema();
    }
  }
  
  m_xHistory.vAppend(m_pToken);
  
  if (bTestFile(m_pToken[0])) {
    vPushFile(m_pToken[0]);
    m_pTokeniser->vLineMode(true);
    m_sResponse = "<ok>";
    return true;
  }
  
  // - execute seitf request
  if (! bExecuteRequest())
    return false;
  
  if (!bPostProcess)
    return true;
  
  // - post process requests
  if (String::bEqualTo("sel",m_pToken[0],C_NO_CASE)) {
    vIterateRows();
    return bReturnTable();
  }
  
  // - the following never return
  
  if (String::bEqualTo("mon",m_pToken[0],C_NO_CASE)) {
    while (true) {
      PosixThread::vSleepSec(10);
      if (!bExecuteRequest())
        return true;
    }
  }
  
  if (String::bEqualTo("x",m_pToken[0],C_NO_CASE)) {
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
  m_sExpect = m_pToken[1];
  
  m_bExpect = true;
  
  vClipTokens(2);
}

/*----------------------------------------------------------------------------*/
/* clear expected result                                                      */
/*----------------------------------------------------------------------------*/
void SePrompt::vClearExpect(void)
{
  m_sExpect = "";
  
  m_bExpect = false;
}

/*----------------------------------------------------------------------------*/
/* clip tokens                                                                */
/*----------------------------------------------------------------------------*/
void SePrompt::vClipTokens(i8 nTokens)
{
  i8 iToToken = 0;
  
  for (i8 iFromToken = nTokens; iFromToken < C_TOKENS; iFromToken++) {
    
    String::vCopy(m_pToken[iToToken++],m_pToken[iFromToken]);
    
    if (String::bEqualTo("",m_pToken[iFromToken]))
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
    if (String::bEqualTo("",m_pToken[iTokens]))
      break;
  
  bool bResult = RapidsSE_ExecuteRequest(m_pSEitf,m_pToken,iTokens,&aResponse);
  
  if (bResult)
    m_sResponse = aResponse ? aResponse : "";
  else
    m_sResponse = aResponse ? aResponse : "";
  
  xTimer.vUpdate();
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* iterate rows                                                               */
/*----------------------------------------------------------------------------*/
void SePrompt::vIterateRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iAddress = String::iToI8(m_sResponse.aStr());
  
  SelCursor* pSelCursor = reinterpret_cast<SelCursor*>(iAddress);
  ASSERT(CN_SELCURSOR == pSelCursor->eClass());
  
  i8 iRowCount = 0;
  
  for (i8 iPartition = 0; iPartition < gxArguments.m_nSlots; iPartition++) {
    vTrace("Partition %lld: \n", Partition::iSlotToPartition(iPartition));
    pSelCursor->vBegin(iPartition);
    pSelCursor->vFirst();
    char* pStr = pSelCursor->pRowAsStr();
    while (pStr) {
      iRowCount++;
      vTrace("Row: %s\n",pStr);
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
  
  String::vCopy(m_pToken[0], "rettbl");
  String::vCopy(m_pToken[1], "");
  
  if (!bExecuteRequest()) {
    vTrace("bReturnTable: (%s) Return Table FAILED!!! : %s\n", m_sResponse.aStr());
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
  
  i8 iAddress = String::iToI8(m_sResponse.aStr());
  
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
  
  i8 iAddress = String::iToI8(m_sResponse.aStr());
  
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
  
  if (xFile.bExists(pWord))
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
  
  vTrace("se temporary commands:\n");
  vTrace("add    -- add column to table\n");
  vTrace("gettbl -- get address of table\n");
  vTrace("bye    -- quit se prompt\n");
  vTrace("cd     -- change directory\n");
  vTrace("cf     -- change folder\n");
  //vTrace("con    -- connect\n");
  vTrace("connectCluster    -- connect the cluster\n");
  vTrace("cp     -- copy table\n");
  vTrace("create -- create table\n");
  //vTrace("dis    -- disconnect\n");
  vTrace("do     -- do shorthand\n");
  vTrace("drop   -- dropTable\n");
  vTrace("dump   -- dump table\n");
  vTrace("ex     -- export table t\n");
  vTrace("ld     -- load table t\n");
  vTrace("lf     -- list folder contents\n");
  vTrace("ls     -- list directory contents\n");
  vTrace("lscat  -- list catalogs contents\n");
  vTrace("lscol  -- list columns\n");
  vTrace("lsidx  -- list indices\n");
  vTrace("lssch  -- list schemas contents\n");
  vTrace("lstbl  -- list tables contents\n");
  vTrace("lsten  -- list tenants contents\n");
  vTrace("mkcat  -- create catalog\n");
  vTrace("mkidx  -- create index\n");
  vTrace("mksch  -- create schema\n");
  vTrace("mktbl  -- create table\n");
  vTrace("mkten  -- create tenant\n");
  vTrace("print  -- print string\n");
  vTrace("pwf    -- print working folder\n");
  vTrace("pwd    -- print working directory\n");
  vTrace("rettbl -- return usage of table\n");
  vTrace("rmcat  -- drop catalog\n");
  vTrace("rmdic  -- drop dictionary\n");
  vTrace("rmsch  -- drop schema\n");
  vTrace("rmtbl  -- drop table\n");
  vTrace("rmten  -- drop tenant\n");
  vTrace("sel    -- select * from table\n");
  vTrace("space  -- pool space\n");
  vTrace("status -- grid status\n");
  vTrace("sz     -- size of table\n");
  vTrace("tpch   -- tpch shorthand\n");
  vTrace("tr     -- truncate table\n");
  vTrace("usesch -- use schema\n");
  vTrace("?      -- print this help\n");
  vTrace("help   -- print this help\n");
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
  
  ASSERT(String::bEqualTo(m_pToken[0],"begin"));
  ASSERT(String::bEqualTo(m_pToken[1],"insert"));
  
  vCollectStatement();
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_pToken[iToken])
      break;
    String::vCopy(m_pToken2[iToken], m_pToken[iToken]);
    String::vCopy(m_pToken[iToken], "");
  }
  
  iNumTokens = iToken;
  // there should be 4 tokens
  if (iNumTokens != 4) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!String::bEqualTo(m_pToken2[3] , ";")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // get table pointer
  String::vCopy(m_pToken[0], "gettbl");
  String::vCopy(m_pToken[1], m_pToken2[2]);
  if (!bExecuteRequest()) {
    vTrace("SePrompt::bBeginInsert : gettbl FAILED!!!\n");
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
  
  ASSERT(String::bEqualTo(m_pToken[0],"sleep"));
  
  i8 nSeconds = String::iToI8(m_pToken[1]);

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
  
  ASSERT(String::bEqualTo(m_pToken[0],"cd"));
  
  String::vCopy(sDir.aStr(), m_pToken[1]);
  
  i8 iToken;
  for (iToken = 2; iToken < C_TOKENS; iToken++) {
    if (! *m_pToken[iToken])
      break;
    String::vConcat(sDir.aStr(), m_pToken[iToken]);
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
  
  ASSERT(String::bEqualTo(m_pToken[0],"antbl"));
  
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
  
  ASSERT(String::bEqualTo(m_pToken[0],"dantbl"));
  
  return bExecuteRequest();
}

/*----------------------------------------------------------------------------*/
/* docol                                                                      */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDocol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(String::bEqualTo(m_pToken[0],"docol"));
  return bExecuteRequest();
}

/*----------------------------------------------------------------------------*/
/* create catalog                                                             */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCreateCatalog(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(String::bEqualTo(m_pToken[0],"create"));
  ASSERT(String::bEqualTo(m_pToken[1],"catalog"));
  vCollectStatement();
  
  i8 iToken;
  
  // copy tokens from m_pToken to m_pToken2
  // while doing that also validate the create statament
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_pToken[iToken])
      break;
    String::vCopy(m_pToken2[iToken], m_pToken[iToken]);
    String::vCopy(m_pToken[iToken], "");
  }
  
  i8 iNumTokens = iToken;
  // there should be 4 tokens
  if (iNumTokens != 4) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!String::bEqualTo(m_pToken2[3] , ";")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // create a catalog
  String::vCopy(m_pToken[0], "mkcat");
  String::vCopy(m_pToken[1], m_pToken2[2]);
  
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Catalog created: %s\n", m_pToken2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* create index                                                               */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCreateIndex(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(String::bEqualTo(m_pToken[0],"create"));
  ASSERT(String::bEqualTo(m_pToken[1],"index"));
  vCollectStatement();
  
  i8 iToken;
  
  // copy tokens from m_pToken to m_pToken2
  // while doing that also validate the create statament
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_pToken[iToken])
      break;
    String::vCopy(m_pToken2[iToken], m_pToken[iToken]);
    String::vCopy(m_pToken[iToken], "");
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
  if (!String::bEqualTo(m_pToken2[3] , "on")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // token[5] should be '('
  if (!String::bEqualTo(m_pToken2[5] , "(")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // 7 token should be ')'
  if (!String::bEqualTo(m_pToken2[7] , ")")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!String::bEqualTo(m_pToken2[iNumTokens-1] , ";")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // create the index
  String::vCopy(m_pToken[0], "mkidx");
  String::vCopy(m_pToken[1], m_pToken2[2]);
  String::vCopy(m_pToken[2], m_pToken2[4]);
  String::vCopy(m_pToken[3], m_pToken2[6]);
  
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

  ASSERT(String::bEqualTo(m_pToken[0],"create"));
  ASSERT(String::bEqualTo(m_pToken[1],"schema"));
  vCollectStatement();
  
  i8 iToken;
  
  // copy tokens from m_pToken to m_pToken2
  // while doing that also validate the create statament
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_pToken[iToken])
      break;
    String::vCopy(m_pToken2[iToken], m_pToken[iToken]);
    String::vCopy(m_pToken[iToken], "");
  }
  
  i8 iNumTokens = iToken;
  
  // there should be 4 tokens
  if (iNumTokens != 4) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!String::bEqualTo(m_pToken2[3] , ";")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // create a schema
  String::vCopy(m_pToken[0], "mksch");
  String::vCopy(m_pToken[1], m_pToken2[2]);
  
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Schema created: %s\n", m_pToken2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* create tenant                                                              */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCreateTenant(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(String::bEqualTo(m_pToken[0],"create"));
  ASSERT(String::bEqualTo(m_pToken[1],"tenant"));
  vCollectStatement();
  
  i8 iToken;
  
  // copy tokens from m_pToken to m_pToken2
  // while doing that also validate the create statament
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_pToken[iToken])
      break;
    String::vCopy(m_pToken2[iToken], m_pToken[iToken]);
    String::vCopy(m_pToken[iToken], "");
  }
  
  i8 iNumTokens = iToken;
  
  // there should be 4 tokens
  if (iNumTokens != 4) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!String::bEqualTo(m_pToken2[3] , ";")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // create a tenant
  String::vCopy(m_pToken[0], "mkten");
  String::vCopy(m_pToken[1], m_pToken2[2]);
  
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Tenant created: %s\n", m_pToken2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* create table                                                               */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCreateTable(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(String::bEqualTo(m_pToken[0],"create"));
  ASSERT(String::bEqualTo(m_pToken[1],"table"));
  vCollectStatement();
  
  i8 iToken;
  
  // copy tokens from m_pToken to m_pToken2
  // while doing that also validate the create statament
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_pToken[iToken])
      break;
    String::vCopy(m_pToken2[iToken], m_pToken[iToken]);
    String::vCopy(m_pToken[iToken], "");
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
  if (!String::bEqualTo(m_pToken2[3] , "(")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last-1 token should be ')'
  if (!String::bEqualTo(m_pToken2[iNumTokens-2] , ")")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
    
  // last token should be ';'
  if (!String::bEqualTo(m_pToken2[iNumTokens-1] , ";")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
      
  // create the table
  String::vCopy(m_pToken[0], "mktbl");
  String::vCopy(m_pToken[1], m_pToken2[2]);
  
  if (!bExecuteRequest())
    return false;
  
  // get reference to pTable
  String::vCopy(m_pToken[0], "gettbl");
  String::vCopy(m_pToken[1], m_pToken2[2]);
  
  if (!bExecuteRequest()) {
    vTrace("SePrompt::bCreateTable : gettbl FAILED!!!\n");
    
    // create the table
    String::vCopy(m_pToken[0], "rmtbl");
    String::vCopy(m_pToken[1], m_pToken2[2]);
    
    if (!bExecuteRequest()) {      
      return false;
    }
    
    return false;
  }
  
  // now collect the column names and their types
  i8 iCurrentToken = 4;
  while(iCurrentToken < iNumTokens - 2) {
    
    if (iCurrentToken != 4 && !String::bEqualTo(m_pToken2[iCurrentToken++],",")) {
      bDropReturnTable(m_pToken2[2]);
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      return false;
    }
    
    // add columns to the table
    i8 j = 0;
    String::vCopy(m_pToken[j++], "add");
    String::vCopy(m_pToken[j++], m_pToken2[2]);
    String::vCopy(m_pToken[j++], m_pToken2[iCurrentToken++]);
    
    if (String::bEqualTo(m_pToken2[iCurrentToken], "integer", C_NO_CASE) ||
        String::bEqualTo(m_pToken2[iCurrentToken], "varchar", C_NO_CASE) ||
        String::bEqualTo(m_pToken2[iCurrentToken], "varbinary", C_NO_CASE) ||
        String::bEqualTo(m_pToken2[iCurrentToken], "char", C_NO_CASE) ||
        String::bEqualTo(m_pToken2[iCurrentToken], "float", C_NO_CASE)) {
      
      // copy varchar/char
      String::vCopy(m_pToken[j], m_pToken2[iCurrentToken++]);
      
      if (String::bEqualTo(m_pToken2[iCurrentToken], "(")) {
        
        // str.cat '('
        String::vConcat(m_pToken[j], m_pToken2[iCurrentToken++]);
        
        // str.cat the size
        String::vConcat(m_pToken[j], m_pToken2[iCurrentToken]);
        
        // check if it was numeric
        if (!String::bExtractI8(&m_pToken2[iCurrentToken++], &iValue)) {
          // drop the table
          bDropReturnTable(m_pToken2[2]);
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        }
        
        if (!String::bEqualTo(m_pToken2[iCurrentToken],")")) {
          // drop the table
          bDropReturnTable(m_pToken2[2]);
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        }
        
        // str.cat ')'
        String::vConcat(m_pToken[j], m_pToken2[iCurrentToken++]);
        
      }
    }
    else if (String::bEqualTo(m_pToken2[iCurrentToken], "decimal", C_NO_CASE)) {
      
      // copy varchar/char
      String::vCopy(m_pToken[j], m_pToken2[iCurrentToken++]);
      
      if (String::bEqualTo(m_pToken2[iCurrentToken], "(")) {
        
        // str.cat '('
        String::vConcat(m_pToken[j], m_pToken2[iCurrentToken++]);
        
        // str.cat the size
        String::vConcat(m_pToken[j], m_pToken2[iCurrentToken]);
        
        // check if it was numeric
        if (!String::bExtractI8(&m_pToken2[iCurrentToken++], &iValue)) {
          bDropReturnTable(m_pToken2[2]);
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        }
        
        if (!String::bEqualTo(m_pToken2[iCurrentToken],",")) {
          bDropReturnTable(m_pToken2[2]);
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        }
        
        // str.cat ','
        String::vConcat(m_pToken[j], m_pToken2[iCurrentToken++]);
        
        // str.cat the size
        String::vConcat(m_pToken[j], m_pToken2[iCurrentToken]);
        
        // check if it was numeric
        if (!String::bExtractI8(&m_pToken2[iCurrentToken++], &iValue)) {
          bDropReturnTable(m_pToken2[2]);
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        }
        
        if (!String::bEqualTo(m_pToken2[iCurrentToken],")")) {
          bDropReturnTable(m_pToken2[2]);
          m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
          return false;
        }
        
        // str.cat ')'
        String::vConcat(m_pToken[j], m_pToken2[iCurrentToken++]);
      }
    }
    else if (String::bEqualTo(m_pToken2[iCurrentToken], "date", C_NO_CASE) ||
             String::bEqualTo(m_pToken2[iCurrentToken], "timestamp", C_NO_CASE) ||
             String::bEqualTo(m_pToken2[iCurrentToken], "interval", C_NO_CASE) ||
             String::bEqualTo(m_pToken2[iCurrentToken], "boolean", C_NO_CASE)) {
      String::vCopy(m_pToken[j], m_pToken2[iCurrentToken++]);
    }
    else {
      // drop the table that was created
      bDropReturnTable(m_pToken2[2]);
      m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
      return false;
    }
    
    // check for NOT NULL
    if (String::bEqualTo(m_pToken2[iCurrentToken],"not", C_NO_CASE)) {
      
      String::vConcat(m_pToken[j], " ");
      String::vConcat(m_pToken[j], m_pToken2[iCurrentToken++]);
      
      if (!String::bEqualTo(m_pToken2[iCurrentToken],"null", C_NO_CASE)) {
        bDropReturnTable(m_pToken2[2]);
        m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
        return false;
      }
      String::vConcat(m_pToken[j], " ");
      String::vConcat(m_pToken[j], m_pToken2[iCurrentToken++]);
    }
    
    // check for PRIMARY KEY
    if (String::bEqualTo(m_pToken2[iCurrentToken],"primary", C_NO_CASE)) {
      
      String::vConcat(m_pToken[j], " ");
      String::vConcat(m_pToken[j], m_pToken2[iCurrentToken++]);
      
      if (!String::bEqualTo(m_pToken2[iCurrentToken],"key", C_NO_CASE)) {
        bDropReturnTable(m_pToken2[2]);
        m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
        return false;
      }
      String::vConcat(m_pToken[j], " ");
      String::vConcat(m_pToken[j], m_pToken2[iCurrentToken++]);
    }

    // check for UNIQUE
    if (String::bEqualTo(m_pToken2[iCurrentToken],"unique", C_NO_CASE)) {
      
      String::vConcat(m_pToken[j], " ");
      String::vConcat(m_pToken[j], m_pToken2[iCurrentToken++]);
    }

    
    // now we should see a column type
    // validate it
    String::vToLower(m_pToken[j]);
    
    // add the column
    if (!bExecuteRequest()) {
      
      // save error returned by addColumn
      eEC eError = m_pSEitf->eGetLastError();
      
      // drop the table that was created
      bDropReturnTable(m_pToken2[2]);
      
      // return error returned by addColumn
      m_pSEitf->vSetLastError(eError);
      return false;
    }
  }
  
  // derefernce and return the table first
  bool bResult = bReturnTable();
  ASSERT(bResult);

  //vTrace("Table created: %s\n", m_pToken2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* collect ';' delimited statement                                            */
/*----------------------------------------------------------------------------*/
void SePrompt::vCollectStatement(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  // we should have a first token (e.g. create or drop)
  ASSERT(! String::bEqualTo(m_pToken[0],""));

  // have we already seen the ';'
  i8 iToken = m_pTokeniser->iFindToken(";",m_pToken,C_TOKENS);
  
  if (iToken >= 0) {
    //m_pTokeniser->vDumpTokens("\nCommand : ",m_pToken,C_TOKENS);
    return;
  }
  
  iToken = m_pTokeniser->iCountTokens(m_pToken,C_TOKENS);
  
  // switch to terminal mode
  m_pTokeniser->vLineMode(false);
  
  s32 sContinuation("  > ");
  m_pTokeniser->vPrompt(sContinuation);
  
  // get tokens delimited by ';'
  for ( ; iToken < C_TOKENS; iToken++) {
    
    eTK eToken = m_pTokeniser->eNextToken(m_pToken[iToken]);
    ASSERT(TK_NONE != eToken);
    
    // look for terminal
    if (String::bEqualTo(m_pToken[iToken],";"))
      break;
  }
  
  // switch back to line mode
  m_pTokeniser->vLineMode(true);
  m_pTokeniser->vPrompt(m_sPrompt);
  
  //m_pTokeniser->vDumpTokens("Command : ",m_pToken,C_TOKENS);
}

/*----------------------------------------------------------------------------*/
/* connect the cluster                                                        */
/*----------------------------------------------------------------------------*/
bool SePrompt::bConnectCluster(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(String::bEqualTo(m_pToken[0],"connectCluster"));
  
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
  
  ASSERT(String::bEqualTo(m_pToken[0],"drop"));
  ASSERT(String::bEqualTo(m_pToken[1],"catalog"));
  
  vCollectStatement();
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_pToken[iToken])
      break;
    String::vCopy(m_pToken2[iToken], m_pToken[iToken]);
    String::vCopy(m_pToken[iToken], "");
  }
  
  iNumTokens = iToken;
  
  // there should be 4 tokens
  if (iNumTokens != 4) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!String::bEqualTo(m_pToken2[3] , ";")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a catalog
  String::vCopy(m_pToken[0], "rmcat");
  String::vCopy(m_pToken[1], m_pToken2[2]);
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Catalog dropped: %s\n", m_pToken2[2]);
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
  
  ASSERT(String::bEqualTo(m_pToken[0],"drop"));
  ASSERT(String::bEqualTo(m_pToken[1],"schema"));
  
  vCollectStatement();
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_pToken[iToken])
      break;
    String::vCopy(m_pToken2[iToken], m_pToken[iToken]);
    String::vCopy(m_pToken[iToken], "");
  }
  
  iNumTokens = iToken;
  // there should be 4 tokens
  if (iNumTokens != 4) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!String::bEqualTo(m_pToken2[3] , ";")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a schema
  String::vCopy(m_pToken[0], "rmsch");
  String::vCopy(m_pToken[1], m_pToken2[2]);
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Schema dropped: %s\n", m_pToken2[2]);
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
  
  ASSERT(String::bEqualTo(m_pToken[0],"drop"));
  ASSERT(String::bEqualTo(m_pToken[1],"table"));
  
  vCollectStatement();
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_pToken[iToken])
      break;
    String::vCopy(m_pToken2[iToken], m_pToken[iToken]);
    String::vCopy(m_pToken[iToken], "");
  }
  
  iNumTokens = iToken;
  
  // there should be 4 tokens
  if (iNumTokens != 4) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!String::bEqualTo(m_pToken2[3] , ";")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a table
  String::vCopy(m_pToken[0], "rmtbl");
  String::vCopy(m_pToken[1], m_pToken2[2]);
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Table dropped: %s\n", m_pToken2[2]);
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
  
  String::vCopy(m_pToken[0], "rmtbl");
  String::vCopy(m_pToken[1], pTableName);
  String::vCopy(m_pToken[2], "");
  
  if (!bExecuteRequest()) {
    //vTrace("Error: (%s) Drop Table %s\n", m_sResponse.aStr(), pTableName);
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
  
  ASSERT(String::bEqualTo(m_pToken[0],"drop"));
  ASSERT(String::bEqualTo(m_pToken[1],"tenant"));
  
  vCollectStatement();
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_pToken[iToken])
      break;
    String::vCopy(m_pToken2[iToken], m_pToken[iToken]);
    String::vCopy(m_pToken[iToken], "");
  }
  
  iNumTokens = iToken;
  
  // there should be 4 tokens
  if (iNumTokens != 4) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!String::bEqualTo(m_pToken2[3] , ";")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a tenant
  String::vCopy(m_pToken[0], "rmten");
  String::vCopy(m_pToken[1], m_pToken2[2]);
  if (!bExecuteRequest()) {
    return false;
  }
  
  //vTrace("Tenant dropped: %s\n", m_pToken2[2]);
  
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
  
  ASSERT(String::bEqualTo(m_pToken[0],"end"));
  ASSERT(String::bEqualTo(m_pToken[1],"insert"));
  
  vCollectStatement();
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_pToken[iToken])
      break;
    String::vCopy(m_pToken2[iToken], m_pToken[iToken]);
    String::vCopy(m_pToken[iToken], "");
  }
  
  iNumTokens = iToken;
  // there should be 3 tokens
  if (iNumTokens != 3) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!String::bEqualTo(m_pToken2[3] , ";")) {
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
  //  vTrace("SePrompt::bEndInsert : rettbl FAILED!!!\n");
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
  
  ASSERT(String::bEqualTo(m_pToken[0],"insert"));
  ASSERT(String::bEqualTo(m_pToken[1],"row"));
  
  vCollectStatement();
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_pToken[iToken])
      break;
    String::vCopy(m_pToken2[iToken], m_pToken[iToken]);
    String::vCopy(m_pToken[iToken], "");
  }
  
  iNumTokens = iToken;
  // there should be 3 tokens
  if (iNumTokens != 4) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!String::bEqualTo(m_pToken2[3] , ";")) {
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
  
  char* pRow = m_pToken2[2];
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
  
  ASSERT(String::bEqualTo(m_pToken[0],"ls"));
  
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
    vTrace("%c %s\n", iEntryType == DT_DIR ? 'd' : 'f', sDirEntry.aStr());
  }
  
  // close the current directory
  xDir.vClose();
  
  // clear the response
  m_sResponse = "";
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* send object (temp)                                                         */
/*----------------------------------------------------------------------------*/
bool SePrompt::bObject(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(String::bEqualTo(m_pToken[0],"object"));
  
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
  
  ASSERT(String::bEqualTo(m_pToken[0],"space"));
    
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
  
  ASSERT(String::bEqualTo(m_pToken[0],"status"));
  
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
  
  ASSERT(String::bEqualTo(m_pToken[0],"pwd"));
  
  if (xDir.bGetCurrent(sDir)) {
    vTrace("%s", sDir.aStr());
  }
  
  // clear the respnse
  m_sResponse = "";
  
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
  
  ASSERT(String::bEqualTo(m_pToken[0],"use"));
  ASSERT(String::bEqualTo(m_pToken[1],"schema"));
  
  vCollectStatement();
  
  for (iToken = 0; iToken < C_TOKENS; iToken++) {
    if (! *m_pToken[iToken])
      break;
    String::vCopy(m_pToken2[iToken], m_pToken[iToken]);
    String::vCopy(m_pToken[iToken], "");
  }
  
  iNumTokens = iToken;
  // there should be 4 tokens
  if (iNumTokens != 4) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // last token should be ';'
  if (!String::bEqualTo(m_pToken2[3] , ";")) {
    m_pSEitf->vSetLastError(EC_INVALID_COMMAND);
    return false;
  }
  
  // drop a schema
  String::vCopy(m_pToken[0], "usesch");
  String::vCopy(m_pToken[1], m_pToken2[2]);
  if (!bExecuteRequest())
    return false;
  
  //vTrace("Schema changed: %s\n", m_pToken2[2]);
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
