/*============================================================================*/
/* SePrompt.cpp -- se> prompt                                                 */
/*============================================================================*/

#include "SePrompt.hpp"

extern BufferPool*  gpBufferPool;
extern Heap*        gpMediumHeap;
extern Arguments    gxArguments;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* SePrompt::operator new(size_t iSize)
{
  ASSERT(iSize);
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
SePrompt::SePrompt(i8 iPartition)
: Object(CN_SEPROMPT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_iPartition = iPartition;

  eIE eResult = RapidsSE_SEitfNew((void**)&m_pSEitf,m_iPartition);
  ASSERT(IE_NONE == eResult);
  
  m_bContinue = true;
  m_sPrompt = "se 0.1.1> ";
  
  m_sResponse  = "";
  //m_xHistory
  
  // initialise the token set
  for (i8 iToken = 0; iToken < C_TOKENS; iToken++) {
    m_pToken[iToken] = &m_wTokenArea[C_TOKEN_SPACE * iToken];
    m_pToken2[iToken] = &m_wTokenArea2[C_TOKEN_SPACE * iToken];
  }
  
  //m_wWorkarea
  
  
  //----
  m_xTokeniser.vMalloc(false);
  m_xTokeniser.vPush();
  
  if (gxArguments.m_bCommand) {
    m_xTokeniser.vUseString(gxArguments.m_sCommand);
  }
  else if (gxArguments.m_bPrompt) {
    m_xTokeniser.vPrompt(m_sPrompt);
    m_xTokeniser.vUseStdin();
  }
  
  m_xTokeniser.vLineMode(true);
  m_xTokeniser.vNormal(true);
  
  _checkInitialised_(sizeof(SePrompt));
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SePrompt::~SePrompt(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  RapidsSE_SEitfDelete(m_pSEitf);
}

/*----------------------------------------------------------------------------*/
/* dump seprompt                                                              */
/*----------------------------------------------------------------------------*/
void SePrompt::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_SEPROMPT);
  ASSERT(pObject->eClass() == CN_SEPROMPT);
  
  SePrompt* pSePrompt = reinterpret_cast<SePrompt*>(pObject);
  
  vTrace("%sSePrompt %p:\n", pTab, pSePrompt);
  vTrace("%s  partition %lld\n", pTab, pSePrompt->m_iPartition);
  vTrace("%s  seitf %p\n", pTab, pSePrompt->m_pSEitf);
  vTrace("%s  continue %s\n", pTab, pSePrompt->m_bContinue ? "true" : "false");
  vTrace("%s  prompt '%s'\n", pTab, pSePrompt->m_sResponse.a());
  vTrace("%s  response '%s'\n\n", pTab, pSePrompt->m_sResponse.a());
}

/*----------------------------------------------------------------------------*/
/* prompt for commands and process them                                       */
/*----------------------------------------------------------------------------*/
void SePrompt::vLoop(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);

  // open the command history file
  m_xHistory.vOpen();
  
  // get and process commands
  while (m_bContinue) {
    vGetCommand();
    bProcessCommand();
  }
  
  // close history file
  m_xHistory.vClose();
  
  // SHOULD REMOVE HISTORY FILE?
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* execute command from given string                                          */
/*----------------------------------------------------------------------------*/
bool SePrompt::bExecuteCommand(s& sCommand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  m_xTokeniser.vUseString(sCommand);
  i8 iToken = 0;
  while (TK_NONE != m_xTokeniser.eNextToken(m_pToken[iToken])) {
    
    //vTrace("vGetCommand : [%lld] %s\n",iToken,m_pToken[iToken]);
    
    iToken++;
    ASSERT(iToken < C_TOKENS);
  }
  bool bResult = bProcessCommand();
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
  
  //String::vCopy(m_pToken[0],gxArguments.m_sCommand.a());
  //String::vCopy(m_pToken[1],"");

  i8 iToken = 0;
  while (TK_NONE != m_xTokeniser.eNextToken(m_pToken[iToken])) {
    
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
    
    m_xTokeniser.vResetTokens(m_pToken,C_TOKENS);
    
    i8 iToken = 0;
    while (TK_NONE != m_xTokeniser.eNextToken(m_pToken[iToken])) {
      
      //vTrace("vGetCommand : [%lld] %s\n",iToken,m_pToken[iToken]);
      
      iToken++;
      ASSERT(iToken < C_TOKENS);
    }
    
    // end of file? if so, pop and retry
    if (! iToken && m_xTokeniser.bEOF()) {
      bool b = m_xTokeniser.bPop();
      ASSERT(b);
      continue;
    }
    
    break;
  }
  
}

/*----------------------------------------------------------------------------*/
/* process command                                                            */
/*----------------------------------------------------------------------------*/
#ifdef _RELEASE_VERSION
bool SePrompt::bProcessCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
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
  
  if (String::bEqualTo("cd",m_pToken[0],C_NO_CASE)) {
    vChangeWorkingDir();
    return true;
  }
  
  if (String::bEqualTo("create",m_pToken[0],C_NO_CASE)) {
    
    if (String::bEqualTo("tenant",m_pToken[1],C_NO_CASE)) {
      return bCreateTenant();
    }
    if (String::bEqualTo("catalog",m_pToken[1],C_NO_CASE)) {
      vCreateCatalog();
      return true;
    }
    if (String::bEqualTo("schema",m_pToken[1],C_NO_CASE)) {
      vCreateSchema();
      return true;
    }
    if (String::bEqualTo("table",m_pToken[1],C_NO_CASE)) {
      vCreateTable();
      return true;
    }
  }
  
  if (String::bEqualTo("drop",m_pToken[0],C_NO_CASE)) {
    if (String::bEqualTo("tenant",m_pToken[1],C_NO_CASE)) {
      vDropTenant();
      return true;
    }
    if (String::bEqualTo("catalog",m_pToken[1],C_NO_CASE)) {
      vDropCatalog();
      return true;
    }
    if (String::bEqualTo("schema",m_pToken[1],C_NO_CASE)) {
      vDropSchema();
      return true;
    }
    if (String::bEqualTo("table",m_pToken[1],C_NO_CASE)) {
      vDropTable();
      return true;
    }
  }
  
  if (String::bEqualTo("pwd",m_pToken[0],C_NO_CASE)) {
    vPrintWorkingDir();
    return true;
  }
  
  if (String::bEqualTo("ls",m_pToken[0],C_NO_CASE)) {
    vListDir();
    return true;
  }
  
  if (String::bEqualTo("cf",m_pToken[0],C_NO_CASE)) {
    bExecuteRequest();
    return true;
  }
  
  if (String::bEqualTo("cd",m_pToken[0],C_NO_CASE)) {
    bExecuteRequest();
    return true;
  }
  
  if (String::bEqualTo("lf",m_pToken[0],C_NO_CASE)) {
    bExecuteRequest();
    return true;
  }
  
  if (String::bEqualTo("ls",m_pToken[0],C_NO_CASE)) {
    bExecuteRequest();
    return true;
  }
  
  if (String::bEqualTo("lscol",m_pToken[0],C_NO_CASE)) {
    bExecuteRequest();
    return true;
  }
  
  if (String::bEqualTo("pwd",m_pToken[0],C_NO_CASE)) {
    bExecuteRequest();
    return true;
  }
  
  if (String::bEqualTo("pwf",m_pToken[0],C_NO_CASE)) {
    bExecuteRequest();
    return true;
  }
  
  if (String::bEqualTo("",m_pToken[0],C_NO_CASE)) {
    bExecuteRequest();
    return true;
  }
  
  m_xHistory.vAppend(m_pToken);
  
  if (bTestFile(m_pToken[0])) {
    vPushFile(m_pToken[0]);
    m_xTokeniser.vLineMode(true);
    return true;
  }
  
  vTrace("Invalid command\n");
  return true;
}
#else
bool SePrompt::bProcessCommand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
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
  
  if (String::bEqualTo("cd",m_pToken[0],C_NO_CASE)) {
    vChangeWorkingDir();
    return true;
  }
  
  if (String::bEqualTo("docol",m_pToken[0],C_NO_CASE)) {
    vDocol();
    return true;
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
  
  if (String::bEqualTo("create",m_pToken[0],C_NO_CASE)) {
    
    if (String::bEqualTo("tenant",m_pToken[1],C_NO_CASE)) {
      return bCreateTenant();
    }
    if (String::bEqualTo("catalog",m_pToken[1],C_NO_CASE)) {
      vCreateCatalog();
      return true;
    }
    if (String::bEqualTo("schema",m_pToken[1],C_NO_CASE)) {
      vCreateSchema();
      return true;
    }
    if (String::bEqualTo("table",m_pToken[1],C_NO_CASE)) {
      vCreateTable();
      return true;
    }
  }
  
  if (String::bEqualTo("drop",m_pToken[0],C_NO_CASE)) {
    if (String::bEqualTo("tenant",m_pToken[1],C_NO_CASE)) {
      vDropTenant();
      return true;
    }
    if (String::bEqualTo("catalog",m_pToken[1],C_NO_CASE)) {
      vDropCatalog();
      return true;
    }
    if (String::bEqualTo("schema",m_pToken[1],C_NO_CASE)) {
      vDropSchema();
      return true;
    }
    if (String::bEqualTo("table",m_pToken[1],C_NO_CASE)) {
      vDropTable();
      return true;
    }
  }
  
  if (String::bEqualTo("pwd",m_pToken[0],C_NO_CASE)) {
    vPrintWorkingDir();
    return true;
  }
  
  if (String::bEqualTo("ls",m_pToken[0],C_NO_CASE)) {
    vListDir();
    return true;
  }
  
  if (String::bEqualTo("space",m_pToken[0],C_NO_CASE)) {
    vPoolSpace();
    return true;
  }
  
  m_xHistory.vAppend(m_pToken);
  
  if (bTestFile(m_pToken[0])) {
    vPushFile(m_pToken[0]);
    m_xTokeniser.vLineMode(true);
    return true;
  }
  
  // - execute seitf request
  
  if (! bExecuteRequest())
    return true;
  
  // - post process requests
  
  if (String::bEqualTo("sel",m_pToken[0],C_NO_CASE)) {
    vIterateRows();
    return true;
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

#endif

/*----------------------------------------------------------------------------*/
/* execute an se request                                                      */
/*----------------------------------------------------------------------------*/
bool SePrompt::bExecuteRequest(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Timer xTimer;
  char* aResponse = nullptr;
  
  // ALKA ECHO
  //  for (i8 i = 0; i < 16 && *m_pToken[i]; i++)
  //    vTrace("%s ",m_pToken[i]);
  //  vTrace("\n");
  
  xTimer.vStart();
  
  eIE eResult = RapidsSE_ExecuteRequest(m_pSEitf,m_pToken,&aResponse);
  
  if (aResponse)
    m_sResponse = aResponse;
  
  if (IE_INVALID_COMMAND == eResult)
    return false;
  
  xTimer.vUpdate();
  
  if (IE_NONE == eResult) {
    if ( ! String::bEqualTo(aResponse, "<ok>") ) {
      vTrace("%s\n",aResponse);
      return true;
    }
  }
  else {
    i8 iError = 0;
    RapidsSE_GetErrorCode(m_pSEitf,&iError);
    vTrace("Error %lld : %s\n",iError,aResponse);
    return false;
  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* iterate rows                                                               */
/*----------------------------------------------------------------------------*/
void SePrompt::vIterateRows(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  i8 iAddress = String::iToI8(m_sResponse.a());
  
  RowCursor* pRowCursor = reinterpret_cast<RowCursor*>(iAddress);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  pRowCursor->vBegin(m_iPartition);
  pRowCursor->vFirst();
  char* pStr = pRowCursor->pRowAsStr();
  while (pStr) {
    vTrace("Row: %s\n",pStr);
    pRowCursor->vNext();
    pStr = pRowCursor->pRowAsStr();
  }
  
  pRowCursor->vEnd();
  delete pRowCursor;
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
  
  m_xTokeniser.vPush();
  
  bool b = m_xTokeniser.bUseFile(sPath);
  ASSERT(b);
}

/*----------------------------------------------------------------------------*/
/* help                                                                       */
/*----------------------------------------------------------------------------*/
void SePrompt::vHelp(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
#ifdef _RELEASE_VERSION
  vTrace("se commands:\n");
  vTrace("bye            -- quit se prompt\n");
  vTrace("cd             -- change directory\n");
  vTrace("cf             -- change folder\n");
  vTrace("create catalog -- create a catalog\n");
  vTrace("create schema  -- create a scheam\n");
  vTrace("create table   -- create a table\n");
  vTrace("create tenant  -- create a tenant\n");
  vTrace("drop catalog   -- drop a catalog\n");
  vTrace("drop schema    -- drop a schema\n");
  vTrace("drop table     -- drop a table\n");
  vTrace("drop tenant    -- drop a tenant\n");
  vTrace("lf             -- list folder contents\n");
  vTrace("ls             -- list directory contents\n");
  vTrace("lscol          -- list columns of a table\n");
  vTrace("pwf            -- print working folder\n");
  vTrace("pwd            -- print working directory\n");
  vTrace("?              -- print this help\n");
  vTrace("help           -- print this help\n");
#else
  vTrace("se temporary commands:\n");
  vTrace("add    -- add column to table\n");
  vTrace("adr    -- get address of table\n");
  vTrace("bye    -- quit se prompt\n");
  vTrace("cd     -- change directory\n");
  vTrace("cf     -- change folder\n");
  vTrace("con    -- connect\n");
  vTrace("cp     -- copy table\n");
  vTrace("create -- create table\n");
  vTrace("dis    -- disconnect\n");
  vTrace("do     -- do shorthand\n");
  vTrace("drop   -- dropTable\n");
  vTrace("dump   -- dump table\n");R
  vTrace("ex     -- export table t\n");
  vTrace("ld     -- load table t\n");
  vTrace("lf     -- list folder contents\n");
  vTrace("ls     -- list directory contents\n");powf(<#float#>, <#float#>)
  vTrace("lscat  -- list catalogs contents\n");
  vTrace("lscol  -- list columns\n");
  vTrace("lssch  -- list schemas contents\n");
  vTrace("lstbl  -- list tables contents\n");
  vTrace("lsten  -- list tenants contents\n");
  vTrace("mkcat  -- create catalog\n");
  vTrace("mksch  -- create schema\n");
  vTrace("mktbl  -- create table\n");
  vTrace("mkten  -- create tenant\n");
  vTrace("pwf    -- print working folder\n");
  vTrace("pwd    -- print working directory\n");
  vTrace("rmcat  -- drop catalog\n");
  vTrace("rmdic  -- drop dictionary\n");
  vTrace("rmsch  -- drop schema\n");
  vTrace("rmtbl  -- drop table\n");
  vTrace("rmten  -- drop tenant\n");
  vTrace("sel    -- select * from table\n");
  vTrace("space  -- pool space\n");
  vTrace("sz     -- size of table\n");
  vTrace("tpch   -- tpch shorthand\n");
  vTrace("tr     -- truncate table\n");
  vTrace("?      -- print this help\n");
  vTrace("help   -- print this help\n");
#endif
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* change working directory                                                   */
/*----------------------------------------------------------------------------*/
void SePrompt::vChangeWorkingDir(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  s256 sDir;
  Dir xDir;
  
  ASSERT(String::bEqualTo(m_pToken[0],"cd"));
  
  String::vCopy(sDir.a(), m_pToken[1]);
  
  i8 iToken;
  for (iToken = 2; iToken < C_TOKENS; iToken++) {
    if (! *m_pToken[iToken])
      break;
    String::vConcat(sDir.a(), m_pToken[iToken]);
  }

  if ( ! xDir.bSetCurrent(sDir) )
    vTrace("Unable to set current directory\n");
}

/*----------------------------------------------------------------------------*/
/* docol                                                                      */
/*----------------------------------------------------------------------------*/
void SePrompt::vDocol(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(String::bEqualTo(m_pToken[0],"docol"));
  if (!bExecuteRequest())
    return;
}

/*----------------------------------------------------------------------------*/
/* create catalog                                                             */
/*----------------------------------------------------------------------------*/
void SePrompt::vCreateCatalog(void)
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
    vTrace("Error: Invalid statement.\n");
    return;
  }
  
  // last token should be ';'
  if (!String::bEqualTo(m_pToken2[3] , ";")) {
    vTrace("Error: Invalid statement.\n");
    return;
  }
  
  // create a catalog
  String::vCopy(m_pToken[0], "mkcat");
  String::vCopy(m_pToken[1], m_pToken2[2]);
  
  if (!bExecuteRequest())
    return;
  
  vTrace("Catalog created: %s\n", m_pToken2[2]);
}

/*----------------------------------------------------------------------------*/
/* create schema                                                              */
/*----------------------------------------------------------------------------*/
void SePrompt::vCreateSchema(void)
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
    vTrace("Error: Invalid statement.\n");
    return;
  }
  
  // last token should be ';'
  if (!String::bEqualTo(m_pToken2[3] , ";")) {
    vTrace("Error: Invalid statement.\n");
    return;
  }
  
  // create a schema
  String::vCopy(m_pToken[0], "mksch");
  String::vCopy(m_pToken[1], m_pToken2[2]);
  
  if (!bExecuteRequest())
    return;
  
  vTrace("Schema created: %s\n", m_pToken2[2]);
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
    vTrace("Error: Invalid statement.\n");
    return false;
  }
  
  // last token should be ';'
  if (!String::bEqualTo(m_pToken2[3] , ";")) {
    vTrace("Error: Invalid statement.\n");
    return false;
  }
  
  // create a tenant
  String::vCopy(m_pToken[0], "mkten");
  String::vCopy(m_pToken[1], m_pToken2[2]);
  
  if (!bExecuteRequest())
    return false;
  
  vTrace("Tenant created: %s\n", m_pToken2[2]);
  return true;
}

/*----------------------------------------------------------------------------*/
/* create table                                                               */
/*----------------------------------------------------------------------------*/
void SePrompt::vCreateTable(void)
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
    vTrace("Error: Invalid statement.\n");
    return;
  }
  
  // token[3] should be '('
  if (!String::bEqualTo(m_pToken2[3] , "(")) {
    vTrace("Error: Invalid statement.\n");
    return;
  }
  
  // last-1 token should be ')'
  if (!String::bEqualTo(m_pToken2[iNumTokens-2] , ")")) {
    vTrace("Error: Invalid statement.\n");
    return;
  }
    
  // last token should be ';'
  if (!String::bEqualTo(m_pToken2[iNumTokens-1] , ";")) {
    vTrace("Error: Invalid statement.\n");
    return;
  }
      
  // create the table
  String::vCopy(m_pToken[0], "mktbl");
  String::vCopy(m_pToken[1], m_pToken2[2]);
  
  if (!bExecuteRequest())
    return;
  
  // now collect the column names and their types
  i8 iCurrentToken = 4;
  while(iCurrentToken < iNumTokens - 2) {
    
    if (iCurrentToken != 4 && !String::bEqualTo(m_pToken2[iCurrentToken++],",")) {
      vDropTable(m_pToken2[2]);
      vTrace("Error: Invalid statement.\n");
      return;
    }
    
    // add columns to the table
    i8 j = 0;
    String::vCopy(m_pToken[j++], "add");
    String::vCopy(m_pToken[j++], m_pToken2[2]);
    String::vCopy(m_pToken[j++], m_pToken2[iCurrentToken++]);
    
    if (String::bEqualTo(m_pToken2[iCurrentToken], "varchar", C_NO_CASE) ||
        String::bEqualTo(m_pToken2[iCurrentToken], "char", C_NO_CASE)) {
      
      // copy varchar/char
      String::vCopy(m_pToken[j], m_pToken2[iCurrentToken++]);
      
      if (String::bEqualTo(m_pToken2[iCurrentToken], "(")) {
        
        // concat '('
        String::vConcat(m_pToken[j], m_pToken2[iCurrentToken++]);
        
        // concat the size
        String::vConcat(m_pToken[j], m_pToken2[iCurrentToken]);
        
        // check if it was numeric
        if (!String::bExtractI8(&m_pToken2[iCurrentToken++], &iValue)) {
          vDropTable(m_pToken2[2]);
          vTrace("Error: Invalid statement.\n");
          return;
        }
        
        if (!String::bEqualTo(m_pToken2[iCurrentToken],")")) {
          vDropTable(m_pToken2[2]);
          vTrace("Error: Invalid statement.\n");
          return;
        }
        
        // concat ')'
        String::vConcat(m_pToken[j], m_pToken2[iCurrentToken++]);
        
      }
    }
    else if (String::bEqualTo(m_pToken2[iCurrentToken], "decimal", C_NO_CASE)) {
      
      // copy varchar/char
      String::vCopy(m_pToken[j], m_pToken2[iCurrentToken++]);
      
      if (String::bEqualTo(m_pToken2[iCurrentToken], "(")) {
        
        // concat '('
        String::vConcat(m_pToken[j], m_pToken2[iCurrentToken++]);
        
        // concat the size
        String::vConcat(m_pToken[j], m_pToken2[iCurrentToken]);
        
        // check if it was numeric
        if (!String::bExtractI8(&m_pToken2[iCurrentToken++], &iValue)) {
          vDropTable(m_pToken2[2]);
          vTrace("Error: Invalid statement.\n");
          return;
        }
        
        if (!String::bEqualTo(m_pToken2[iCurrentToken],",")) {
          vDropTable(m_pToken2[2]);
          vTrace("Error: Invalid statement.\n");
          return;
        }
        
        // concat ','
        String::vConcat(m_pToken[j], m_pToken2[iCurrentToken++]);
        
        // concat the size
        String::vConcat(m_pToken[j], m_pToken2[iCurrentToken]);
        
        // check if it was numeric
        if (!String::bExtractI8(&m_pToken2[iCurrentToken++], &iValue)) {
          vDropTable(m_pToken2[2]);
          vTrace("Error: Invalid statement.\n");
          return;
        }
        
        if (!String::bEqualTo(m_pToken2[iCurrentToken],")")) {
          vDropTable(m_pToken2[2]);
          vTrace("Error: Invalid statement.\n");
          return;
        }
        
        // concat ')'
        String::vConcat(m_pToken[j], m_pToken2[iCurrentToken++]);
      }
    }
    else if (String::bEqualTo(m_pToken2[iCurrentToken], "integer", C_NO_CASE) ||
             String::bEqualTo(m_pToken2[iCurrentToken], "int", C_NO_CASE) ||
             String::bEqualTo(m_pToken2[iCurrentToken], "date", C_NO_CASE)) {
      String::vCopy(m_pToken[j], m_pToken2[iCurrentToken++]);
    }
    else {
      // drop the table that was created
      vDropTable(m_pToken2[2]);
      vTrace("Error: Invalid type %s for column '%s' \n", m_pToken[j], m_pToken[j - 1]);
      return;
    }
    
    // check for NOT NULL
    if (String::bEqualTo(m_pToken2[iCurrentToken],"not", C_NO_CASE)) {
      
      String::vConcat(m_pToken[j], " ");
      String::vConcat(m_pToken[j], m_pToken2[iCurrentToken++]);
      
      if (!String::bEqualTo(m_pToken2[iCurrentToken],"null", C_NO_CASE)) {
        vDropTable(m_pToken2[2]);
        vTrace("Error: Invalid statement.\n");
        return;
      }
      String::vConcat(m_pToken[j], " ");
      String::vConcat(m_pToken[j], m_pToken2[iCurrentToken++]);
    }
    
    // now we should see a column type
    // validate it
    String::vToLower(m_pToken[j]);
    
    // add the column
    if (!bExecuteRequest())
      return;
  }
  
  vTrace("Table created: %s\n", m_pToken2[2]);
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
  i8 iToken = m_xTokeniser.iFindToken(";",m_pToken,C_TOKENS);
  
  if (iToken >= 0) {
    //m_xTokeniser.vDumpTokens("\nCommand : ",m_pToken,C_TOKENS);
    return;
  }
  
  iToken = m_xTokeniser.iCountTokens(m_pToken,C_TOKENS);
  
  // switch to terminal mode
  m_xTokeniser.vLineMode(false);
  
  s32 sContinuation("  > ");
  m_xTokeniser.vPrompt(sContinuation);
  
  // get tokens delimited by ';'
  for ( ; iToken < C_TOKENS; iToken++) {
    
    eTK eToken = m_xTokeniser.eNextToken(m_pToken[iToken]);
    ASSERT(TK_NONE != eToken);
    
    // look for terminal
    if (String::bEqualTo(m_pToken[iToken],";"))
      break;
  }
  
  // switch back to line mode
  m_xTokeniser.vLineMode(true);
  m_xTokeniser.vPrompt(m_sPrompt);
  
  //m_xTokeniser.vDumpTokens("Command : ",m_pToken,C_TOKENS);
}

/*----------------------------------------------------------------------------*/
/* drop catalog                                                               */
/*----------------------------------------------------------------------------*/
void SePrompt::vDropCatalog(void)
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
  }
  
  iNumTokens = iToken;
  
  // there should be 4 tokens
  if (iNumTokens != 4) {
    vTrace("Error: Invalid statement.\n");
    return;
  }
  
  // last token should be ';'
  if (!String::bEqualTo(m_pToken2[3] , ";")) {
    vTrace("Error: Invalid statement.\n");
    return;
  }
  
  // drop a catalog
  String::vCopy(m_pToken[0], "rmcat");
  String::vCopy(m_pToken[1], m_pToken2[2]);
  if (!bExecuteRequest())
    return;
  
  vTrace("Catalog dropped: %s\n", m_pToken2[2]);
}

/*----------------------------------------------------------------------------*/
/* drop schema                                                                */
/*----------------------------------------------------------------------------*/
void SePrompt::vDropSchema(void)
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
  }
  
  iNumTokens = iToken;
  // there should be 4 tokens
  if (iNumTokens != 4) {
    vTrace("Error: Invalid statement.\n");
    return;
  }
  
  // last token should be ';'
  if (!String::bEqualTo(m_pToken2[3] , ";")) {
    vTrace("Error: Invalid statement.\n");
    return;
  }
  
  // drop a schema
  String::vCopy(m_pToken[0], "rmsch");
  String::vCopy(m_pToken[1], m_pToken2[2]);
  if (!bExecuteRequest())
    return;
  
  vTrace("Schema dropped: %s\n", m_pToken2[2]);
}

/*----------------------------------------------------------------------------*/
/* drop table                                                                 */
/*----------------------------------------------------------------------------*/
void SePrompt::vDropTable(void)
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
  }
  
  iNumTokens = iToken;
  
  // there should be 4 tokens
  if (iNumTokens != 4) {
    vTrace("Error: Invalid statement.\n");
    return;
  }
  
  // last token should be ';'
  if (!String::bEqualTo(m_pToken2[3] , ";")) {
    vTrace("Error: Invalid statement.\n");
    return;
  }
  
  // drop a table
  String::vCopy(m_pToken[0], "rmtbl");
  String::vCopy(m_pToken[1], m_pToken2[2]);
  if (!bExecuteRequest())
    return;
  
  vTrace("Table dropped: %s\n", m_pToken2[2]);
}

/*----------------------------------------------------------------------------*/
/* drop table                                                                 */
/*----------------------------------------------------------------------------*/
void SePrompt::vDropTable(char* pTableName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  String::vCopy(m_pToken[0], "rmtbl");
  String::vCopy(m_pToken[1], pTableName);
  
  if (!bExecuteRequest()) {
    vTrace("Error: (%s) Drop Table %s\n", m_sResponse.a(), pTableName);
  }
}

/*----------------------------------------------------------------------------*/
/* drop tenant                                                                */
/*----------------------------------------------------------------------------*/
void SePrompt::vDropTenant(void)
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
  }
  
  iNumTokens = iToken;
  
  // there should be 4 tokens
  if (iNumTokens != 4) {
    vTrace("Error: Invalid statement.\n");
    return;
  }
  
  // last token should be ';'
  if (!String::bEqualTo(m_pToken2[3] , ";")) {
    vTrace("Error: Invalid statement.\n");
    return;
  }
  
  // drop a tenant
  String::vCopy(m_pToken[0], "rmten");
  String::vCopy(m_pToken[1], m_pToken2[2]);
  if (!bExecuteRequest())
    return;
  
  vTrace("Tenant dropped: %s\n", m_pToken2[2]);
}


/*----------------------------------------------------------------------------*/
/* list directory                                                             */
/*----------------------------------------------------------------------------*/
void SePrompt::vListDir(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Dir  xDir;
  s256 sDir;
  s256 sDirEntry;
  i8 iEntryType;
  
  ASSERT(String::bEqualTo(m_pToken[0],"ls"));
  
  // get the current directory
  if (!xDir.bGetCurrent(sDir)) {
    vTrace("Invalid current dir\n");
    return;
  }
  
  // open the current directory
  if (!xDir.bOpen()) {
    vTrace("Unable to open current dir\n");
    return;
  }
  
  // read the current directory
  while(xDir.bRead()) {
    sDirEntry = xDir.sEntryName();
    iEntryType = xDir.iEntryType();
    vTrace("%c %s\n", iEntryType == DT_DIR ? 'd' : 'f', sDirEntry.a());
  }
  
  // close the current directory
  xDir.vClose();
}

/*----------------------------------------------------------------------------*/
/* print pool space                                                           */
/*----------------------------------------------------------------------------*/
void SePrompt::vPoolSpace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  ASSERT(String::bEqualTo(m_pToken[0],"space"));
  
  i8 iFree;
  i8 iInUse;
  i8 iTotal;
  
  s256 sFree;
  s256 sInUse;
  s256 sTotal;

  String::vCopy(m_pToken[0], "getTotalSpace");
  if (!bExecuteRequest())
    return;
  iTotal = String::iToI8(m_sResponse.a());
  Magnitudes::vMagnitude(iTotal, 2, sTotal.a());
             
  String::vCopy(m_pToken[0], "getInUseSpace");
  if (!bExecuteRequest())
    return;
  iInUse = String::iToI8(m_sResponse.a());
  Magnitudes::vMagnitude(iInUse, 2, sInUse.a());
             
  String::vCopy(m_pToken[0], "getFreeSpace");
  if (!bExecuteRequest())
    return;
  iFree = String::iToI8(m_sResponse.a());
  Magnitudes::vMagnitude(iFree, 2, sFree.a());
             
  vTrace("Pool: Size(%s) InUse(%s) Free(%s)\n",
         sTotal.a(), sInUse.a(), sFree.a());
}

/*----------------------------------------------------------------------------*/
/* print working directory                                                    */
/*----------------------------------------------------------------------------*/
void SePrompt::vPrintWorkingDir(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEPROMPT,_FOOTER_);
  
  Dir xDir;
  s256 sDir;
  
  ASSERT(String::bEqualTo(m_pToken[0],"pwd"));
  
  if (xDir.bGetCurrent(sDir)) {
    vTrace("%s\n", sDir.a());
  }
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