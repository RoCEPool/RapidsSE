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
  m_sPrompt    = "se> ";

  eIE eResult = RapidsSE_SEitfNew((void**)&m_pSEitf,m_iPartition);
  ASSERT(IE_NONE == eResult);
  
  m_sInputLine = "";
  m_iInput     = 0;
  //m_xInputStream
  m_sResponse  = "";
  
  m_wHistory[0] = '\0';
  //m_xHistoryFile
  m_iCommand = 0;
  m_bSkipHistory = false;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SePrompt::~SePrompt(void)
{
  delete m_pSEitf;
}

/*----------------------------------------------------------------------------*/
/* run seitf>                                                                 */
/*----------------------------------------------------------------------------*/
void SePrompt::vPrompt(void)
{
  char xArray[C_16KB];
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
  
  vHistoryOpen();
  
  InputStream* pInput;
  pInput = this->pInput();
  pInput->vUseStdin(m_sInputLine);
  pInput->vPrompt(m_sPrompt.a());
  
  while (true) {
    
    for (i8 i = 0; i < 16; i++)
      *pWords[i] = '\0';
    
    pInput = this->pInput();
    if (pInput->bStdin()) {
      pInput->vUseStdin(m_sInputLine);
      pInput->bGetLine(m_sInputLine);
      pInput->vUseText(m_sInputLine);
    }
    else {
      pInput->vUseFile(m_sInputLine);
      if (! pInput->bFirstChar() ) {
        if ( ! pInput->bGetLine(m_sInputLine) ) {
          vPopFile();
          continue;
        }
      }
      pInput->vUseText(m_sInputLine);
    }
    
    i8 i = 0;
    while (LT_NONE != pInput->eNextLiteral(pWords[i])) {
      //vTrace("Word [%lld] %s\n",i,pWords[i]);
      i++;
    }
    
    if (String::bEqualTo("history",pWords[0],C_NO_CASE)) {
      vHistoryPrint();
      continue;
    }
    
    if (String::bEqualTo("!!",pWords[0],C_NO_CASE)) {
      if (! bHistoryRecall(pWords))
        continue;
    }
    
    if ('!' == *pWords[0]) {
      if (! bHistoryRecall(pWords))
        continue;
      m_bSkipHistory = true;
    }
    
    vHistoryAppend(pWords);
    
    if (bTestFile(pWords[0])) {
      vPushFile(pWords[0]);
      continue;
    }

    vExecute2(pWords);
    
    if (String::bEqualTo("sel",pWords[0],C_NO_CASE)) {
      vIterateRows();
    }
    
    if (String::bEqualTo("mon",pWords[0],C_NO_CASE)) {
      while (true) {
        PosixThread::vSleepSec(10);
        vExecute2(pWords);
      }
    }
    
    if (String::bEqualTo("x",pWords[0],C_NO_CASE)) {
      while (true) {
        vExecute2(pWords);
      }
    }
  }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
void SePrompt::vExecute2(char* pWords[])
{
  Timer xTimer;
  
  xTimer.vStart();

  char* aResponse = nullptr;
  eIE eResult = RapidsSE_ExecuteCommand(m_pSEitf,pWords,&aResponse);

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
/* get current input stream                                                   */
/*----------------------------------------------------------------------------*/
InputStream* SePrompt::pInput(void)
{
  return &m_xInputStream[m_iInput];
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
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append command to history file                                             */
/*----------------------------------------------------------------------------*/
void SePrompt::vHistoryAppend(char* pWords[])
{
  for (i8 i = 0; *pWords[i]; i++) {
    if (i)
      m_xHistoryFile.vPrint("%s"," ");
    m_xHistoryFile.vPrint("%s",pWords[i]);
  }
  m_xHistoryFile.vPrint("%s","\n");
  m_xHistoryFile.vFlush();
  
  m_iCommand++;
}

/*----------------------------------------------------------------------------*/
/* close command history file                                                 */
/*----------------------------------------------------------------------------*/
void SePrompt::vHistoryClose(void)
{
  m_xHistoryFile.vClose();
}

/*----------------------------------------------------------------------------*/
/* seek to end of command history file                                        */
/*----------------------------------------------------------------------------*/
void SePrompt::vHistoryEnd(void)
{
  m_xHistoryFile.vSeekEnd();
}

/*----------------------------------------------------------------------------*/
/* get command from history file                                              */
/*----------------------------------------------------------------------------*/
bool SePrompt::bHistoryGet(i8 iCommand, char* pWords[])
{
  vHistoryStart();
  
  char wCommand[C_4KB];
  
  s16K sCommand;
  
  i8 i = 0;
  while (bHistoryRead(wCommand)) {
    if (++i == iCommand) {
      vHistoryEnd();
      //      String::vCopy(pWords[0],wCommand);
      {
        m_iInput++;
        InputStream* pInput = this->pInput();
        sCommand = wCommand;
        pInput->vUseText(sCommand);
        pInput->vLineOriented(true);
        
        for (i8 i = 0; i < 16; i++)
          *pWords[i] = '\0';
        
        i8 i = 0;
        //pInput->vEOL(false);
        while (LT_NONE != pInput->eNextLiteral(pWords[i])) {
          //vTrace("Word [%lld] %s\n",i,pWords[i]);
          i++;
        }
        
        m_iInput--;
      }
      
      return true;
    }
  }
  
  vHistoryEnd();
  return false;
}

/*----------------------------------------------------------------------------*/
/* open command history file                                                  */
/*----------------------------------------------------------------------------*/
void SePrompt::vHistoryOpen(void)
{
  char wFileName[C_1KB];
  
  String::vSprint(wFileName,"/tmp/SEitf_%lld_history.txt",Process::iGetPid());
  
  bool b = m_xHistoryFile.bOpen(wFileName,C_OPEN_READ_WRITE_CREATE);
  ASSERT(b);
}

/*----------------------------------------------------------------------------*/
/* print command history file                                                 */
/*----------------------------------------------------------------------------*/
void SePrompt::vHistoryPrint(void)
{
  vHistoryStart();
  
  char wCommand[C_4KB];
  
  i8 i = 1;
  while (bHistoryRead(wCommand)) {
    vTrace("%lld %s\n",i++,wCommand);
  }
  
  vHistoryEnd();
}

/*----------------------------------------------------------------------------*/
/* read command history file                                                  */
/*----------------------------------------------------------------------------*/
bool SePrompt::bHistoryRead(char* wCommand)
{
  bool bResult = m_xHistoryFile.bReadLine(wCommand);
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* recall command from history file                                           */
/*----------------------------------------------------------------------------*/
bool SePrompt::bHistoryRecall(char* pWords[])
{
  i8 iCommand;
  
  if (String::bEqualTo("!!",pWords[0],C_NO_CASE)) {
    iCommand = m_iCommand;
    if (! iCommand)
      return false;
  }
  else {
    // get command number
    if (String::iLength(pWords[0]) < 2)
      return false;
    
    iCommand = String::iToI8(pWords[0]+1);
  }
  
  vTrace("Recall: %lld\n",iCommand);
  
  bool b = bHistoryGet(iCommand,pWords);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* seek to start of command history file                                      */
/*----------------------------------------------------------------------------*/
void SePrompt::vHistoryStart(void)
{
  m_xHistoryFile.vSeek(0);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
void SePrompt::vPushFile(char* pWord)
{
  ASSERT(m_iInput < C_INPUT_ARRAY - 1);
  m_iInput++;
  
  InputStream* pInput = this->pInput();
  
  bool b = pInput->bOpenFile(m_sInputLine,pWord);
  ASSERT(b);
  
}

/*----------------------------------------------------------------------------*/
/* pop command file                                                           */
/*----------------------------------------------------------------------------*/
void SePrompt::vPopFile(void)
{
  ASSERT(m_iInput);
  
  InputStream* pInput = this->pInput();
  
  pInput->vCloseFile();
  
  m_iInput--;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* help                                                                       */
/*----------------------------------------------------------------------------*/
bool SePrompt::bHelp(char* pWords[])
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
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/