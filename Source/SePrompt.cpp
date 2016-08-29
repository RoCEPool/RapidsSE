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
  
  //m_xHistory
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
  
  m_xHistory.vOpen();
  
  InputStream* pInput;
  pInput = this->pInput();
  pInput->vUseStdin(m_sInputLine);
  pInput->vPrompt(m_sPrompt.a());
  
  while (true) {
   
//--
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
    
//--
    
    if (String::bEqualTo("history",pWords[0],C_NO_CASE)) {
      m_xHistory.vPrint();
      continue;
    }
    
    if (String::bEqualTo("!!",pWords[0],C_NO_CASE)) {
      if (! m_xHistory.bRecall(pWords))
        continue;
    }
    
    if ('!' == *pWords[0]) {
      if (! m_xHistory.bRecall(pWords))
        continue;
    }
    
    m_xHistory.vAppend(pWords);
    
    if (bTestFile(pWords[0])) {
      vPushFile(pWords[0]);
      continue;
    }

    vExecute(pWords);
    
    if (String::bEqualTo("sel",pWords[0],C_NO_CASE)) {
      vIterateRows();
    }
    
    if (String::bEqualTo("mon",pWords[0],C_NO_CASE)) {
      while (true) {
        PosixThread::vSleepSec(10);
        vExecute(pWords);
      }
    }
    
    if (String::bEqualTo("x",pWords[0],C_NO_CASE)) {
      while (true) {
        vExecute(pWords);
      }
    }
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void SePrompt::vPrompt2(void)
{
  char wWorkarea[C_16KB];
  char* pWords[16];
  
  m_xInput.vInit(wWorkarea,pWords);
  m_xInput.vPrompt("se> ");
  
  m_xHistory.vOpen();
  
  while (true) {
    
    m_xInput.vNext(pWords);
    
    if (String::bEqualTo("history",pWords[0],C_NO_CASE)) {
      m_xHistory.vPrint();
      continue;
    }
    
    if (String::bEqualTo("!!",pWords[0],C_NO_CASE)) {
      if (! m_xHistory.bRecall(pWords))
        continue;
    }
    
    if ('!' == *pWords[0]) {
      if (! m_xHistory.bRecall(pWords))
        continue;
    }
    
    m_xHistory.vAppend(pWords);
    
    if (bTestFile(pWords[0])) {
      m_xInput.vPushFile(pWords[0]);
      continue;
    }
    
    vExecute(pWords);
    
    if (String::bEqualTo("sel",pWords[0],C_NO_CASE)) {
      vIterateRows();
    }
    
    if (String::bEqualTo("mon",pWords[0],C_NO_CASE)) {
      while (true) {
        PosixThread::vSleepSec(10);
        vExecute(pWords);
      }
    }
    
    if (String::bEqualTo("x",pWords[0],C_NO_CASE)) {
      while (true) {
        vExecute(pWords);
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
void SePrompt::vExecute(char* pWords[])
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