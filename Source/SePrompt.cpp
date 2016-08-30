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
    vTrace("Error %lld : %s\n",eResult,aResponse);
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
/* help                                                                       */
/*----------------------------------------------------------------------------*/
void SePrompt::vHelp(void)
{
  vTrace("se temporary commands:\n");
  vTrace("add   -- add column to table\n");
  vTrace("adr   -- get address of table\n");
  vTrace("bye   -- quit seitf\n");
  vTrace("cd    -- change dictionary\n");
  vTrace("con   -- connect\n");
  vTrace("cp    -- copy table\n");
  vTrace("dis   -- disconnect\n");
  vTrace("do    -- do shorthand\n");
  vTrace("dump  -- dump table\n");
  vTrace("ex    -- export table t\n");
  vTrace("ld    -- load table t\n");
  vTrace("ls    -- list dictionary contents\n");
  vTrace("lscat -- list catalogs contents\n");
  vTrace("lscol -- list columns\n");
  vTrace("lssch -- list schemas contents\n");
  vTrace("lstbl -- list tables contents\n");
  vTrace("lsten -- list tenants contents\n");
  vTrace("mkcat -- create catalog\n");
  vTrace("mksch -- create schema\n");
  vTrace("mktbl -- create table\n");
  vTrace("mkten -- create tenant\n");
  vTrace("pwd   -- print working dictionary\n");
  vTrace("rmcat -- drop catalog\n");
  vTrace("rmdic -- drop dictionary\n");
  vTrace("rmsch -- drop schema\n");
  vTrace("rmtbl -- drop table\n");
  vTrace("rmten -- drop tenant\n");
  vTrace("sel   -- select * from table\n");
  vTrace("sz    -- size of table\n");
  vTrace("tpch  -- tpch shorthand\n");
  vTrace("tr    -- truncate table\n");
  vTrace("?     -- print this help\n");
  vTrace("help  -- print this help\n");
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/