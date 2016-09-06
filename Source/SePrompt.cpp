/*============================================================================*/
/* SePrompt.cpp -- se> prompt                                                 */
/*============================================================================*/

#include "SePrompt.hpp"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SePrompt::SePrompt(i8 iPartition)
{
  m_iPartition = iPartition;

  eIE eResult = RapidsSE_SEitfNew((void**)&m_pSEitf,m_iPartition);
  ASSERT(IE_NONE == eResult);
  
  m_bContinue = true;
  m_sPrompt = "se.2> ";
  
  m_sResponse  = "";
  //m_xHistory
  
  // initialise the token set
  for (i8 iToken = 0; iToken < C_TOKENS; iToken++)
    m_pToken[iToken] = &m_wTokenArea[C_TOKEN_SPACE * iToken];
  //m_wWorkarea
  
  //----
  m_xTokenizer.vMalloc(true);
  m_xTokenizer.vPush();
  m_xTokenizer.vPrompt(m_sPrompt);
  m_xTokenizer.vUseStdin();
  m_xTokenizer.vLineMode(true);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SePrompt::~SePrompt(void)
{
  RapidsSE_SEitfDelete(m_pSEitf);
}

/*----------------------------------------------------------------------------*/
/* prompt for commands and process them                                       */
/*----------------------------------------------------------------------------*/
void SePrompt::vLoop(void)
{
  // open the command history file
  m_xHistory.vOpen();
  
  // get and process commands
  while (m_bContinue) {
    vGetCommand();
    vProcessCommand();
  }
  
  // close history file
  m_xHistory.vClose();
  
  // SHOULD REMOVE HISTORY FILE?
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get command                                                                */
/*----------------------------------------------------------------------------*/
void SePrompt::vGetCommand(void)
{
  while (true) {
    
    m_xTokenizer.vResetTokens(m_pToken,C_TOKENS);
    
    i8 i = 0;
    while (TK_NONE != m_xTokenizer.eNextToken(m_pToken[i])) {
      //vTrace("Word [%lld] %s\n",i,pWords[i]);
      i++;
      ASSERT(i < 16);
    }
    
    // end of file? if so, pop and retry
    if (m_xTokenizer.bEOF()) {
      bool b = m_xTokenizer.bPop();
      ASSERT(b);
      continue;
    }
    
    break;
  }
  
}

/*----------------------------------------------------------------------------*/
/* process command                                                            */
/*----------------------------------------------------------------------------*/
void SePrompt::vProcessCommand(void)
{
  // - test for builtin comands
  
  if (String::bEqualTo("?",   m_pToken[0],C_NO_CASE) ||
      String::bEqualTo("help",m_pToken[0],C_NO_CASE)) {
    vHelp();
    return;
  }
  
  if (String::bEqualTo("bye",m_pToken[0],C_NO_CASE)) {
    m_bContinue = false;
    return;
  }
  
  if (String::bEqualTo("history",m_pToken[0],C_NO_CASE)) {
    m_xHistory.vPrint();
    return;
  }
  
  if (String::bEqualTo("!!",m_pToken[0],C_NO_CASE)) {
    if (! m_xHistory.bRecall(m_pToken,C_TOKENS))
      return;
  }
  
  if ('!' == *m_pToken[0]) {
    if (! m_xHistory.bRecall(m_pToken,C_TOKENS))
      return;
  }
  
  if (String::bEqualTo("create",m_pToken[0],C_NO_CASE)) {
    vCreateTable();
    return;
  }
  
  if (String::bEqualTo("create2",m_pToken[0],C_NO_CASE)) {
    vCreateTable2();
    return;
  }
  
  m_xHistory.vAppend(m_pToken,C_TOKENS);
  
  if (bTestFile(m_pToken[0])) {
    vPushFile(m_pToken[0]);
    m_xTokenizer.vLineMode(true);
    return;
  }
  
  // - execute seitf request
  
  vExecuteRequest();
  
  // - post process requests
  
  if (String::bEqualTo("sel",m_pToken[0],C_NO_CASE)) {
    vIterateRows();
    return;
  }
  
  // - the following never return
  
  if (String::bEqualTo("mon",m_pToken[0],C_NO_CASE)) {
    while (true) {
      PosixThread::vSleepSec(10);
      vExecuteRequest();
    }
  }
  
  if (String::bEqualTo("x",m_pToken[0],C_NO_CASE)) {
    while (true) {
      vExecuteRequest();
    }
  }
  
}

/*----------------------------------------------------------------------------*/
/* execute an se request                                                      */
/*----------------------------------------------------------------------------*/
void SePrompt::vExecuteRequest(void)
{
  Timer xTimer;
  char* aResponse = nullptr;
  
  // ALKA ECHO
  //  for (i8 i = 0; i < 16 && *m_pToken[i]; i++)
  //    vTrace("%s ",m_pToken[i]);
  //  vTrace("\n");
  
  xTimer.vStart();
  
  eIE eResult = RapidsSE_ExecuteCommand(m_pSEitf,m_pToken,&aResponse);
  
  if (aResponse)
    m_sResponse = aResponse;
  
  if (IE_INVALID_COMMAND == eResult)
    return;
  
  xTimer.vUpdate();
  
  if (IE_NONE == eResult) {
    if ( ! String::bEqualTo(aResponse, "<ok>") )
      vTrace("%s\n",aResponse);
  }
  else {
    i8 iError = 0;
    RapidsSE_GetErrorCode(m_pSEitf,&iError);
    vTrace("Error %lld : %s\n",iError,aResponse);
  }
}

/*----------------------------------------------------------------------------*/
/* iterate rows                                                               */
/*----------------------------------------------------------------------------*/
void SePrompt::vIterateRows(void)
{
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
  s256 sPath(pPath);
  
  m_xTokenizer.vPush();
  
  bool b = m_xTokenizer.bUseFile(sPath);
  ASSERT(b);
}

/*----------------------------------------------------------------------------*/
/* help                                                                       */
/*----------------------------------------------------------------------------*/
void SePrompt::vHelp(void)
{
  vTrace("se temporary commands:\n");
  vTrace("add    -- add column to table\n");
  vTrace("adr    -- get address of table\n");
  vTrace("bye    -- quit seitf\n");
  vTrace("cd     -- change dictionary\n");
  vTrace("con    -- connect\n");
  vTrace("cp     -- copy table\n");
  vTrace("create -- create table\n");
  vTrace("dis    -- disconnect\n");
  vTrace("do     -- do shorthand\n");
  vTrace("dump   -- dump table\n");
  vTrace("ex     -- export table t\n");
  vTrace("ld     -- load table t\n");
  vTrace("ls     -- list dictionary contents\n");
  vTrace("lscat  -- list catalogs contents\n");
  vTrace("lscol  -- list columns\n");
  vTrace("lssch  -- list schemas contents\n");
  vTrace("lstbl  -- list tables contents\n");
  vTrace("lsten  -- list tenants contents\n");
  vTrace("mkcat  -- create catalog\n");
  vTrace("mksch  -- create schema\n");
  vTrace("mktbl  -- create table\n");
  vTrace("mkten  -- create tenant\n");
  vTrace("pwd    -- print working dictionary\n");
  vTrace("rmcat  -- drop catalog\n");
  vTrace("rmdic -- drop dictionary\n");
  vTrace("rmsch  -- drop schema\n");
  vTrace("rmtbl  -- drop table\n");
  vTrace("rmten  -- drop tenant\n");
  vTrace("sel    -- select * from table\n");
  vTrace("sz     -- size of table\n");
  vTrace("tpch   -- tpch shorthand\n");
  vTrace("tr     -- truncate table\n");
  vTrace("?      -- print this help\n");
  vTrace("help   -- print this help\n");
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* parse the create table words                                               */
/*----------------------------------------------------------------------------*/
void SePrompt::vCreateTable(void)
{
  int iTokens = 0;
  char xArray[C_1KB * 16];
  char* pWords[16] = {
    &xArray[C_1KB * 0],
    &xArray[C_1KB * 1],
    &xArray[C_1KB * 2],
    &xArray[C_1KB * 3],
    &xArray[C_1KB * 4],
    &xArray[C_1KB * 5],
    &xArray[C_1KB * 6],
    &xArray[C_1KB * 7],
    &xArray[C_1KB * 8],
    &xArray[C_1KB * 9],
    &xArray[C_1KB * 10],
    &xArray[C_1KB * 11],
    &xArray[C_1KB * 12],
    &xArray[C_1KB * 13],
    &xArray[C_1KB * 14],
    &xArray[C_1KB * 15]
  };

  // remove all braces
  for (int i = 0; i < 16 ; i++) {
    if (String::iLength(m_pToken[i]) != 0) {
      
      // ignore )
      if (String::bEqualTo(m_pToken[i], ")")) {
        continue;
      }
      
      // ignore (
      if (String::bEqualTo(m_pToken[i], "(")) {
        continue;
      }
      
      // ignore ,
      if (String::bEqualTo(m_pToken[i], ",")) {
        continue;
      }
      
      // ignore not
      if (String::bEqualTo(m_pToken[i], "not", C_NO_CASE)) {
        continue;
      }
      
      // ignore null
      if (String::bEqualTo(m_pToken[i], "null", C_NO_CASE)) {
        continue;
      }

      // copy all other words
      String::vCopy(pWords[iTokens], m_pToken[i]);
      
      // clear m_pToken for later use
      String::vCopy(m_pToken[i], "");
      
      iTokens++;
    }
  }
  
  // last token should be a ;
  if (!String::bEqualTo(pWords[iTokens-1], ";")) {
    m_sResponse = "Invalid create table command. Not properly terminated.";
    vTrace("Error: %s\n", m_sResponse.a());
    return;
  }
  
  // validate create table
  ASSERT(String::bEqualTo(pWords[0], "create", C_NO_CASE));
  if (!String::bEqualTo(pWords[1], "table", C_NO_CASE)) {
    m_sResponse = "Invalid create table command";
    vTrace("Error: %s\n", m_sResponse.a());
    return;
  }
  
  // create a table
  String::vCopy(m_pToken[0], "mktbl");
  String::vCopy(m_pToken[1], pWords[2]);
  vExecuteRequest();
  
  i8 iCurrentToken = 3;
  while(iCurrentToken < iTokens - 1) {
  
    // add columns to the table
    i8 j = 0;
    String::vCopy(m_pToken[j++], "add");
    String::vCopy(m_pToken[j++], pWords[2]);
    String::vCopy(m_pToken[j++], pWords[iCurrentToken]);
    iCurrentToken++;
    
    // now we should see a column type
    // validate it
    if (!bValidateType(pWords[iCurrentToken])) {
      String::vCopy(m_sResponse.a(),"Invalid type '");
      String::vConcat(m_sResponse.a(),pWords[iCurrentToken]);
      String::vConcat(m_sResponse.a(),"' for column '");
      String::vConcat(m_sResponse.a(),m_pToken[j - 1]);
      String::vConcat(m_sResponse.a(),"'");
      vTrace("Error: %s\n", m_sResponse.a());
      return;
    }
    
    // copy the column type
    String::vCopy(m_pToken[j++], pWords[iCurrentToken]);
    iCurrentToken++;
    
    // add the column
    vExecuteRequest();
  }
  
}

/*----------------------------------------------------------------------------*/
/* testing new approach for these types of commands                           */
/*----------------------------------------------------------------------------*/
void SePrompt::vCreateTable2(void)
{
  // for the moment >>create2<< should be the only token
  ASSERT(String::bEqualTo(m_pToken[0],"create2"));
  ASSERT(String::bEqualTo(m_pToken[1],""));
  
  // switch to terminal mode
  m_xTokenizer.vLineMode(false);
  
  for (i8 iToken = 1; iToken < C_TOKENS; iToken++) {
    
    eTK eToken = m_xTokenizer.eNextToken(m_pToken[iToken]);
    ASSERT(TK_NONE != eToken);
    
    //vTrace("CreateTable2: %s\n", m_pToken[iToken]);
    
    // look for terminal
    if (String::bEqualTo(m_pToken[iToken],";"))
      break;
  }

  // switch back to line mode
  m_xTokenizer.vLineMode(true);

  vTrace("CreateTable2 : ");
  for (i8 iToken = 1; iToken < C_TOKENS; iToken++) {
    if (! *m_pToken[iToken])
      break;
    vTrace("%s ", m_pToken[iToken]);
  }
  vTrace("\n");
}

/*----------------------------------------------------------------------------*/
/* validate type                                                              */
/*----------------------------------------------------------------------------*/
bool SePrompt::bValidateType(char* pType)
{
  if (String::bEqualTo(pType, "int", C_NO_CASE))
    return true;
  
  if (String::bEqualTo(pType, "integer", C_NO_CASE))
    return true;
  
  if (String::bFind(&pType, "varchar", C_NO_CASE))
    return true;
  
  if (String::bFind(&pType, "char", C_NO_CASE))
    return true;

  if (String::bFind(&pType, "decimal", C_NO_CASE))
    return true;

  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/