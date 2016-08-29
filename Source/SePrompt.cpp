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
  
  vTrace("SePrompt::vPrompt\n");

  vAssertTrap();
  
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
/* parse command and break into words                                         */
/*----------------------------------------------------------------------------*/
void SePrompt::vParse(s& sCommand, char* pWords[])
{
  char* ps = sCommand.a();
  
  String::vSkipWhite(&ps);
  for (i8 i = 0; String::bExtractWordString(&ps,pWords[i]); i++) {
    //vTrace("Word[%lld] %s\n",i,pWords[i]);
    String::vSkipWhite(&ps);
  }
}

/*----------------------------------------------------------------------------*/
/* execute command                                                            */
/*----------------------------------------------------------------------------*/
void SePrompt::vExecute(char* pWords[])
{
  i8   iStatus;
  //s256 sResponse;
  
  Timer xTimer;
  
  xTimer.vStart();
  
  if ( ! bDecode(pWords) )
    return;
  
  if (! m_bSkipHistory)
    vHistoryAppend(pWords);
  m_bSkipHistory = false;
  
  while (! bPopResponse(iStatus,m_sResponse))
    ;
  
  xTimer.vUpdate();
  
  //  vTrace("%lld %s (%lldus)\n",iStatus,m_sResponse.a(),xTimer.iMicroseconds());
  if (0 == iStatus) {
    if ( ! String::bEqualTo(m_sResponse.a(), "<ok>") )
      vTrace("%s\n",m_sResponse.a());
  }
  else {
    vTrace("Error %lld : %s\n",iStatus,m_sResponse.a());
  }
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void SePrompt::vExecute2(char* pWords[])
{
  Timer xTimer;
  
  xTimer.vStart();

  char* aResponse = nullptr;
  eIE eResult = RapidsSE_ExecuteCommand(m_pSEitf,pWords,&aResponse);

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
/* append request to request queue                                            */
/*----------------------------------------------------------------------------*/
void SePrompt::vAppendRequest(s& sRequest)
{
  char wItfAddr[32];
  char wWorkspace[C_1KB * 16];
  char* pCursor = nullptr;
  
  ValWsp xVal;
  ValSet xValSet;
  
  String::wToString((i8)this,wItfAddr);
  String::vCopy(wWorkspace,wItfAddr);
  String::vConcat(wWorkspace," >seitf task.this task.seitf! ");
  xVal = wWorkspace;
  //vTrace("%s\n",xVal.sStr());
  xValSet.vAppend(pCursor,xVal);
  
  xVal = sRequest.a();
  //vTrace("%s\n",xVal.sStr());
  xValSet.vAppend(pCursor,xVal);

//!!
//  bool b = m_pRequestQueue->bAppend(&xValSet);
//  ASSERT(b);
}

/*----------------------------------------------------------------------------*/
/* pop response from reponse queue (response is from pointer in seitf(shm))   */
/*----------------------------------------------------------------------------*/
i8 SePrompt::iGetResponse(void)
{
  i8 iStatus;
  while (! bPopResponse(iStatus, m_sResponse))
    ;
  
  return iStatus;
}

/*----------------------------------------------------------------------------*/
/* pop response from reponse queue                                            */
/*----------------------------------------------------------------------------*/
bool SePrompt::bPopResponse(s& sResponse)
{
//  ValWsp xResponse;
//  
//  if (! m_pResponseQueue->bPop(&xResponse))
//    return false;
//  
//  sResponse = xResponse.sStr();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop response from reponse queue                                            */
/*----------------------------------------------------------------------------*/
bool SePrompt::bPopResponse(i8& iResult, s& sResponse)
{
//  bool b;
//  ValSet xValSet;
//  char* pCursor = nullptr;
//  
//  if (! m_pResponseQueue->bPop(&xValSet))
//    return false;
//  
//  ValWsp* pValWsp;
//  
//  b = xValSet.bExtract(pCursor,pValWsp);
//  ASSERT(b);
//  iResult = pValWsp->iInt();
//  
//  b = xValSet.bExtract(pCursor,pValWsp);
//  ASSERT(b);
//  sResponse = pValWsp->sStr();
//  
//  b = xValSet.bExtract(pCursor,pValWsp);
//  ASSERT(! b);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* do                                                                         */
/*----------------------------------------------------------------------------*/
void SePrompt::vDo(const char* aCommand)
{
  s256 sCommand;
  
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
  
  sCommand = aCommand;
  
  vParse(sCommand,pWords);
  vExecute(pWords);
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
/* decode request                                                             */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDecode(char* pWords[])
{
  if (0 == String::iLength(pWords[0])) {
    return false;
  }
  
  if (String::bEqualTo("history",pWords[0],C_NO_CASE)) {
    return bPrintHistory(pWords);
  }
  
  if (String::bEqualTo("!!",pWords[0],C_NO_CASE)) {
    if (! bHistoryRecall(pWords))
      return false;
    m_bSkipHistory = true;
  }
  
  if ('!' == *pWords[0]) {
    if (! bHistoryRecall(pWords))
      return false;
    m_bSkipHistory = true;
  }
  
  if (String::bEqualTo("?",pWords[0],C_NO_CASE) ||
      String::bEqualTo("help",pWords[0],C_NO_CASE)) {
    bHelp(pWords);
    return false;
  }
  
  if (String::bEqualTo("add",pWords[0],C_NO_CASE)) {
    bAddColumn(pWords);
    return true;
  }
  
  if (String::bEqualTo("adr",pWords[0],C_NO_CASE)) {
    bAddressTable(pWords);
    return true;
  }
  
  if (String::bEqualTo("bye",pWords[0],C_NO_CASE)) {
    exit(EXIT_SUCCESS);
  }
  
  if (String::bEqualTo("cd",pWords[0],C_NO_CASE)) {
    bPushDic(pWords);
    return true;
  }
  
  if (String::bEqualTo("con",pWords[0],C_NO_CASE)) {
    bConnect(pWords);
    return true;
  }
  
  if (String::bEqualTo("cp",pWords[0],C_NO_CASE)) {
    bCopyTable(pWords);
    return true;
  }
  
  if (String::bEqualTo("dis",pWords[0],C_NO_CASE)) {
    bDisconnect(pWords);
    return true;
  }
  
  if (String::bEqualTo("dump",pWords[0],C_NO_CASE)) {
    bDumpTable(pWords);
    return true;
  }
  
  if (String::bEqualTo("ex",pWords[0],C_NO_CASE)) {
    bExportTable(pWords);
    return true;
  }
  
  if (String::bEqualTo("ld",pWords[0],C_NO_CASE)) {
    bLoadTable(pWords);
    return true;
  }
  
  if (String::bEqualTo("ls",pWords[0],C_NO_CASE)) {
    bGetDicContents(pWords);
    return true;
  }
  
  if (String::bEqualTo("lscat",pWords[0],C_NO_CASE)) {
    bListCatalogs(pWords);
    return true;
  }
  
  if (String::bEqualTo("lscol",pWords[0],C_NO_CASE)) {
    bListColumns(pWords);
    return true;
  }
  
  if (String::bEqualTo("lssch",pWords[0],C_NO_CASE)) {
    bListSchemas(pWords);
    return true;
  }
  
  if (String::bEqualTo("lstbl",pWords[0],C_NO_CASE)) {
    bListTables(pWords);
    return true;
  }
  
  if (String::bEqualTo("lsten",pWords[0],C_NO_CASE)) {
    bListTenants(pWords);
    return true;
  }
  
  if (String::bEqualTo("mkcat",pWords[0],C_NO_CASE)) {
    bCreateCatalog(pWords);
    return true;
  }
  
  if (String::bEqualTo("mksch",pWords[0],C_NO_CASE)) {
    bCreateSchema(pWords);
    return true;
  }
  
  if (String::bEqualTo("mktbl",pWords[0],C_NO_CASE)) {
    bCreateTable(pWords);
    return true;
  }
  
  if (String::bEqualTo("mkten",pWords[0],C_NO_CASE)) {
    bCreateTenant(pWords);
    return true;
  }
  
  if (String::bEqualTo("pwd",pWords[0],C_NO_CASE)) {
    bGetDicPath(pWords);
    return true;
  }
  
  if (String::bEqualTo("rmcat",pWords[0],C_NO_CASE)) {
    bDropCatalog(pWords);
    return true;
  }
  
  if (String::bEqualTo("rmdic",pWords[0],C_NO_CASE)) {
    bDropDictionary(pWords);
    return true;
  }
  
  if (String::bEqualTo("rmsch",pWords[0],C_NO_CASE)) {
    bDropSchema(pWords);
    return true;
  }
  
  if (String::bEqualTo("rmtbl",pWords[0],C_NO_CASE)) {
    bDropTable(pWords);
    return true;
  }
  
  if (String::bEqualTo("rmten",pWords[0],C_NO_CASE)) {
    bDropTenant(pWords);
    return true;
  }
  
  if (String::bEqualTo("sel",pWords[0],C_NO_CASE)) {
    bSelectTable(pWords);
    return true;
  }
  
  if (String::bEqualTo("sz",pWords[0],C_NO_CASE)) {
    bSizeTable(pWords);
    return true;
  }
  
  if (String::bEqualTo("tr",pWords[0],C_NO_CASE)) {
    bTruncateTable(pWords);
    return true;
  }
  
  if (String::bEqualTo("x",pWords[0],C_NO_CASE)) {
    bX(pWords);
    return true;
  }
  
  if (String::bEqualTo("do",pWords[0],C_NO_CASE)) {
    return bDo(pWords);
  }
  
  if (String::bEqualTo("tpch",pWords[0],C_NO_CASE)) {
    return bTpcH(pWords);
  }
  
  if (String::bEqualTo("ping",pWords[0],C_NO_CASE)) {
    return bPing(pWords);
  }
  
  if (String::bEqualTo("mon",pWords[0],C_NO_CASE)) {
    return bGetMonitor(pWords);
  }
  
  if (String::bEqualTo("print",pWords[0],C_NO_CASE)) {
    bPrint(pWords);
    return false;
  }
  
  if (bTestFile(pWords[0])) {
    vPushFile(pWords[0]);
    return false;
  }
  
  vTrace("Unknown command: %s\n",pWords[0]);
  return false;
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
/* get current input stream                                                   */
/*----------------------------------------------------------------------------*/
InputStream* SePrompt::pInput(void)
{
  return &m_xInputStream[m_iInput];
}

/*----------------------------------------------------------------------------*/
/* help                                                                       */
/*----------------------------------------------------------------------------*/
bool SePrompt::bHelp(char* pWords[])
{
  vTrace("seitf temporary commands:\n");
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

/*----------------------------------------------------------------------------*/
/* connect to se                                                              */
/*----------------------------------------------------------------------------*/
bool SePrompt::bConnect(char* pWords[])
{
  s256 sRequest;
  
  sRequest = "connect";
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* disconnect from se                                                         */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDisconnect(char* pWords[])
{
  s256 sRequest;
  
  sRequest = "disconnect";
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop a dictionary                                                          */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDropDictionary(char* pWords[])
{
  s256 sRequest;
  
  char* a = sRequest.a();
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," drop_dictionary");
  
  vAppendRequest(sRequest);
  return true;
}

/*----------------------------------------------------------------------------*/
/* create a tenant                                                            */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCreateTenant(char* pWords[])
{
  s256 sRequest;
  
  char* a = sRequest.a();
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," create_tenant");
  
  vAppendRequest(sRequest);
  return true;
}

/*----------------------------------------------------------------------------*/
/* list tenants                                                               */
/*----------------------------------------------------------------------------*/
bool SePrompt::bListTenants(char* pWords[])
{
  s256 sRequest;
  
  sRequest = "list_tenants";
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop a tenant                                                              */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDropTenant(char* pWords[])
{
  s256 sRequest;
  
  char* a = sRequest.a();
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," drop_tenant");
  
  vAppendRequest(sRequest);
  return true;
}

/*----------------------------------------------------------------------------*/
/* create a catalog                                                           */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCreateCatalog(char* pWords[])
{
  s256 sRequest;
  
  char* a = sRequest.a();
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," create_catalog");
  
  vAppendRequest(sRequest);
  return true;
}

/*----------------------------------------------------------------------------*/
/* list catalogs                                                              */
/*----------------------------------------------------------------------------*/
bool SePrompt::bListCatalogs(char* pWords[])
{
  s256 sRequest;
  
  sRequest = "list_catalogs";
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop a catalog                                                             */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDropCatalog(char* pWords[])
{
  s256 sRequest;
  
  char* a = sRequest.a();
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," drop_catalog");
  
  vAppendRequest(sRequest);
  return true;
}

/*----------------------------------------------------------------------------*/
/* create a schema                                                            */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCreateSchema(char* pWords[])
{
  s256 sRequest;
  
  char* a = sRequest.a();
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," create_schema");
  
  vAppendRequest(sRequest);
  return true;
}

/*----------------------------------------------------------------------------*/
/* list schemas                                                               */
/*----------------------------------------------------------------------------*/
bool SePrompt::bListSchemas(char* pWords[])
{
  s256 sRequest;
  
  sRequest = "list_schemas";
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop a schema                                                              */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDropSchema(char* pWords[])
{
  s256 sRequest;
  
  char* a = sRequest.a();
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," drop_schema");
  
  vAppendRequest(sRequest);
  return true;
}

/*----------------------------------------------------------------------------*/
/* create a table                                                             */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCreateTable(char* pWords[])
{
  s256 sRequest;
  
  char* a = sRequest.a();
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," create_table");
  
  vAppendRequest(sRequest);
  return true;
}

/*----------------------------------------------------------------------------*/
/* list tables                                                                */
/*----------------------------------------------------------------------------*/
bool SePrompt::bListTables(char* pWords[])
{
  s256 sRequest;
  
  sRequest = "list_tables";
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* dump a table                                                               */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDumpTable(char* pWords[])
{
  s256 sRequest;
  
  char* a = sRequest.a();
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," dump_table");
  
  vAppendRequest(sRequest);
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop a table                                                               */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDropTable(char* pWords[])
{
  s256 sRequest;
  
  char* a = sRequest.a();
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," drop_table");
  
  vAppendRequest(sRequest);
  return true;
}

/*----------------------------------------------------------------------------*/
/* add a column                                                               */
/*----------------------------------------------------------------------------*/
bool SePrompt::bAddColumn(char* pWords[16])
{
  s256 sRequest;
  
  char* a = sRequest.a();
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[3]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," \"");
  String::vConcat(a,pWords[2]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," \"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," add_column");
  
  vAppendRequest(sRequest);
  return true;
}

/*----------------------------------------------------------------------------*/
/* list columns                                                               */
/*----------------------------------------------------------------------------*/
bool SePrompt::bListColumns(char* pWords[])
{
  s256 sRequest;
  
  char* a = sRequest.a();
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," list_columns");
  
  vAppendRequest(sRequest);
  return true;
}

/*----------------------------------------------------------------------------*/
/* copy a table                                                               */
/*----------------------------------------------------------------------------*/
bool SePrompt::bCopyTable(char* pWords[])
{
  s256 sRequest;
  
  char* a = sRequest.a();
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[2]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," \"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," copy_table");
  
  vAppendRequest(sRequest);
  return true;
}

/*----------------------------------------------------------------------------*/
/* export a table                                                             */
/*----------------------------------------------------------------------------*/
bool SePrompt::bExportTable(char* pWords[])
{
  s256 sRequest;
  
  char* a = sRequest.a();
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[2]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," \"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," export_table");
  
  vAppendRequest(sRequest);
  return true;
}

/*----------------------------------------------------------------------------*/
/* load a table from file                                                     */
/*----------------------------------------------------------------------------*/
bool SePrompt::bLoadTable(char* pWords[])
{
  s256 sRequest;
  
  char* a = sRequest.a();
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[2]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," \"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," load_table");
  
  vAppendRequest(sRequest);
  return true;
}

/*----------------------------------------------------------------------------*/
/* select rows from a table                                                   */
/*----------------------------------------------------------------------------*/
bool SePrompt::bSelectTable(char* pWords[])
{
  s256 sRequest;
  
  char* a = sRequest.a();
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," select_table");
  
  vAppendRequest(sRequest);
  return true;
}

/*----------------------------------------------------------------------------*/
/* get address a table                                                        */
/*----------------------------------------------------------------------------*/
bool SePrompt::bAddressTable(char* pWords[])
{
  s256 sRequest;
  
  char* a = sRequest.a();
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," address_table");
  
  vAppendRequest(sRequest);
  return true;
}

/*----------------------------------------------------------------------------*/
/* size of table                                                              */
/*----------------------------------------------------------------------------*/
bool SePrompt::bSizeTable(char* pWords[])
{
  s256 sRequest;
  
  char* a = sRequest.a();
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," size_table");
  
  vAppendRequest(sRequest);
  return true;
}

/*----------------------------------------------------------------------------*/
/* truncate a table                                                           */
/*----------------------------------------------------------------------------*/
bool SePrompt::bTruncateTable(char* pWords[])
{
  s256 sRequest;
  
  char* a = sRequest.a();
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," truncate_table");
  
  vAppendRequest(sRequest);
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop dictionary                                                             */
/*----------------------------------------------------------------------------*/
bool SePrompt::bPopDic(char* pWords[])
{
  return false;
}

/*----------------------------------------------------------------------------*/
/* push dictionary                                                            */
/*----------------------------------------------------------------------------*/
bool SePrompt::bPushDic(char* pWords[])
{
  s256 sRequest;
  
  char* a = sRequest.a();
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," cd");
  
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
bool SePrompt::bPrint(char* pWords[])
{
  printf("%s\n",pWords[1]);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get dictionary path                                                        */
/*----------------------------------------------------------------------------*/
bool SePrompt::bGetDicPath(char* pWords[])
{
  s256 sRequest;
  
  sRequest = "pwd";
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* set dictionary path                                                        */
/*----------------------------------------------------------------------------*/
bool SePrompt::bSetDicPath(char* pWords[])
{
  return true;
}

/*----------------------------------------------------------------------------*/
/* get dictionary contents                                                    */
/*----------------------------------------------------------------------------*/
bool SePrompt::bGetDicContents(char* pWords[])
{
  s256 sRequest;
  
  sRequest = "ls";
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get monitor string                                                         */
/*----------------------------------------------------------------------------*/
bool SePrompt::bGetMonitor(char* pWords[])
{
  s256 sRequest;
  
  sRequest = "get_monitor";
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* print command history                                                      */
/*----------------------------------------------------------------------------*/
bool SePrompt::bPrintHistory(char* pWords[])
{
  vHistoryPrint();
  return false;
}

/*----------------------------------------------------------------------------*/
/* ping test                                                                  */
/*----------------------------------------------------------------------------*/
bool SePrompt::bPing(char* pWords[])
{
  s256 sRequest;
  
  sRequest = "ping";
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* x test                                                                     */
/*----------------------------------------------------------------------------*/
bool SePrompt::bX(char* pWords[])
{
  
  s256 sRequest;
  
  sRequest = "ping";
  vAppendRequest(sRequest);
  
  //  static bool bFirst = true;
  //  if (bFirst) {
  //    bFirst = false;
  //    vAppendRequest(sRequest);
  //    vAppendRequest(sRequest);
  //    vAppendRequest(sRequest);
  //    vAppendRequest(sRequest);
  //    vAppendRequest(sRequest);
  //  }
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* do                                                                         */
/*----------------------------------------------------------------------------*/
bool SePrompt::bDo(char* pWords[])
{
  vDo("ping");
  vDo("cd tenants");
  vDo("mkten ten");
  vDo("cd ten");
  vDo("cd catalogs");
  vDo("mkcat cat");
  vDo("cd cat");
  vDo("cd schemas");
  vDo("mksch sch");
  vDo("cd sch");
  vDo("cd tables");
  vDo("mktbl t");
  vDo("pwd");
  vDo("ls");
  vDo("ld t \"/tpch/nation.tbl\"");
  vDo("ex t \"/tmp/nation.txt\"");
  vDo("sz t ");
  //  vDo("tr t ");
  //  vDo("sz t ");
  //  vDo("rmtbl t ");
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* get last error                                                             */
/*----------------------------------------------------------------------------*/
const char* SePrompt::pGetLastError(void) {
  return m_sInterfaceError.a();
}

/*----------------------------------------------------------------------------*/
/* clear last error                                                           */
/*----------------------------------------------------------------------------*/
void SePrompt::vClearLastError(void) {
  m_eInterfaceError = IE_NONE;
  String::vCopy(m_sInterfaceError.a(), "");
}

/*----------------------------------------------------------------------------*/
/* set last error                                                             */
/*----------------------------------------------------------------------------*/
void SePrompt::vSetLastError(eIE eInterfaceError) {
  m_eInterfaceError = eInterfaceError;
  String::vCopy(m_sInterfaceError.a(), pIE[m_eInterfaceError]);
  
  if (String::iLength(m_sResponse.a()) != 0) {
    String::vConcat(m_sInterfaceError.a(), " (");
    String::vConcat(m_sInterfaceError.a(), m_sResponse.a());
    String::vConcat(m_sInterfaceError.a(), ") ");
  }
}

/*----------------------------------------------------------------------------*/
/* tpch                                                                       */
/*----------------------------------------------------------------------------*/
bool SePrompt::bTpcH(char* pWords[])
{
  vDo("ping");
  vDo("cd tenants");
  vDo("mkten boray");
  vDo("cd boray");
  vDo("cd catalogs");
  vDo("mkcat borayCatalog");
  vDo("cd borayCatalog");
  vDo("cd schemas");
  vDo("mksch boraySchema");
  vDo("cd boraySchema");
  vDo("cd tables");
  vDo("mktbl customer");
  vDo("mktbl lineitem");
  vDo("mktbl nation");
  vDo("add nation N_NATIONKEY  \"INTEGER NOT NULL\"");
  vDo("add nation N_NAME       \"CHAR(25) NOT NULL\"");
  vDo("add nation N_REGIONKEY  \"INTEGER NOT NULL\"");
  vDo("add nation N_COMMENT    \"VARCHAR(152)\"");
  vDo("mktbl orders");
  vDo("mktbl part");
  vDo("mktbl partsupp");
  vDo("mktbl region");
  vDo("mktbl supplier");
  //vDo("ld customer \"/tpch/customer.tbl\"");
  //vDo("ld lineitem \"/tpch/lineitem.tbl\"");
  vDo("ld nation   \"/tpch/nation.tbl\"");
  
  
  //vDo("ld orders   \"/tpch/orders.tbl\"");
  //vDo("ld part     \"/tpch/part.tbl\"");
  //vDo("ld partsupp \"/tpch/partsupp.tbl\"");
  vDo("ld region   \"/tpch/region.tbl\"");
  vDo("ld supplier \"/tpch/supplier.tbl\"");
  vDo("pwd");
  vDo("ls");
  //  vDo("tr t ");
  //  vDo("sz t ");
  //  vDo("rmtbl t ");
  
  return false;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/