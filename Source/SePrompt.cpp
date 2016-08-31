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
  //m_xInput
  m_sResponse  = "";
  //m_xHistory
  
  //m_wWorkarea
  //m_pWords
  
  m_bContinue = true;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SePrompt::~SePrompt(void)
{
  RapidsSE_SEitfDelete(m_pSEitf);
}

/*----------------------------------------------------------------------------*/
/* prompt for commands and process                                            */
/*----------------------------------------------------------------------------*/
void SePrompt::vPrompt(void)
{
  // initialise the onput handler
  m_xInput.vInit(m_wWorkarea,m_pWords);
  m_xInput.vPrompt("se> ");
  
  // open the command history file
  m_xHistory.vOpen();
  
  // get and process commands
  while (m_bContinue) {
    m_xInput.vNext(m_pWords);
    vProcess();
  }
  
  // close history file
  m_xHistory.vClose();
  
  // should remove history file?
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
void SePrompt::vExecute(void)
{
  Timer xTimer;
  char* aResponse = nullptr;
  
  xTimer.vStart();

  eIE eResult = RapidsSE_ExecuteCommand(m_pSEitf,m_pWords,&aResponse);

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
    if (String::iLength(m_pWords[i]) != 0) {
      
      // ignore )
      if (String::bEqualTo(m_pWords[i], ")")) {
        continue;
      }
      
      // ignore (
      if (String::bEqualTo(m_pWords[i], "(")) {
        continue;
      }
      
      // ignore ,
      if (String::bEqualTo(m_pWords[i], ",")) {
        continue;
      }
      
      // ignore not
      if (String::bEqualTo(m_pWords[i], "not", C_NO_CASE)) {
        continue;
      }
      
      // ignore null
      if (String::bEqualTo(m_pWords[i], "null", C_NO_CASE)) {
        continue;
      }

      // copy all other words
      String::vCopy(pWords[iTokens], m_pWords[i]);
      
      // clear m_pWords for later use
      String::vCopy(m_pWords[i], "");
      
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
  String::vCopy(m_pWords[0], "mktbl");
  String::vCopy(m_pWords[1], pWords[2]);
  vExecute();
  
  i8 iCurrentToken = 3;
  while(iCurrentToken < iTokens - 1) {
  
    // add columns to the table
    i8 j = 0;
    String::vCopy(m_pWords[j++], "add");
    String::vCopy(m_pWords[j++], pWords[2]);
    String::vCopy(m_pWords[j++], pWords[iCurrentToken]);
    iCurrentToken++;
    
    // now we should see a column type
    // validate it
    if (!bValidateType(pWords[iCurrentToken])) {
      String::vCopy(m_sResponse.a(),"Invalid type '");
      String::vConcat(m_sResponse.a(),pWords[iCurrentToken]);
      String::vConcat(m_sResponse.a(),"' for column '");
      String::vConcat(m_sResponse.a(),m_pWords[j - 1]);
      String::vConcat(m_sResponse.a(),"'");
      vTrace("Error: %s\n", m_sResponse.a());
      return;
    }
    
    // copy the column type
    String::vCopy(m_pWords[j++], pWords[iCurrentToken]);
    iCurrentToken++;
    
    // add the column
    vExecute();
  }
  
}

/*----------------------------------------------------------------------------*/
/* process input                                                              */
/*----------------------------------------------------------------------------*/
void SePrompt::vProcess(void)
{
  // - test for builtin comands
  
  if (String::bEqualTo("?",   m_pWords[0],C_NO_CASE) ||
      String::bEqualTo("help",m_pWords[0],C_NO_CASE)) {
    vHelp();
    return;
  }

  if (String::bEqualTo("bye",m_pWords[0],C_NO_CASE)) {
    m_bContinue = false;
    return;
  }
  
  if (String::bEqualTo("history",m_pWords[0],C_NO_CASE)) {
    m_xHistory.vPrint();
    return;
  }
  
  if (String::bEqualTo("!!",m_pWords[0],C_NO_CASE)) {
    if (! m_xHistory.bRecall(m_pWords))
      return;
  }
  
  if ('!' == *m_pWords[0]) {
    if (! m_xHistory.bRecall(m_pWords))
      return;
  }
  
  if (String::bEqualTo("create",m_pWords[0],C_NO_CASE)) {
    vCreateTable();
    return;
  }
  
  m_xHistory.vAppend(m_pWords);
  
  if (bTestFile(m_pWords[0])) {
    m_xInput.vPushFile(m_pWords[0]);
    return;
  }
  
  // - execute seitf commands
  
  vExecute();
  
  // - post process commands
  
  if (String::bEqualTo("sel",m_pWords[0],C_NO_CASE)) {
    vIterateRows();
    return;
  }
  
  // - the following never return
  
  if (String::bEqualTo("mon",m_pWords[0],C_NO_CASE)) {
    while (true) {
      PosixThread::vSleepSec(10);
      vExecute();
    }
  }
  
  if (String::bEqualTo("x",m_pWords[0],C_NO_CASE)) {
    while (true) {
      vExecute();
    }
  }

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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/