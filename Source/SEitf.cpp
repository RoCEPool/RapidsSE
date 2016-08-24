/*============================================================================*/
/* SEitf.cpp - se interface                                                   */
/*============================================================================*/

#include "Interface.h"

extern Heap*        gpMediumHeap;
extern Dictionary*  gpDictionary;
extern SEitfList*   gpSEitfList;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* SEitf::operator new(size_t iSize)
{
  Object* pObject = gpMediumHeap->pAllocObject();
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void SEitf::operator delete(void* pSEinterfaceAddress)
{
  SEitf* pSEitf = reinterpret_cast<SEitf*>(pSEinterfaceAddress);
  
  gpMediumHeap->vFreeObject(pSEitf);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SEitf::SEitf(void)
: Object(CN_SEITF)
{
  m_iPid = 0;
  m_iPartitions = 1;
  m_xDictionary.vName("seitf");
  //m_xDicStack;
  m_pRequestQueue  = nullptr;
  m_pResponseQueue = nullptr;
  //m_sResponse
  m_iInput = 0;
  //m_xInputStream[]
  //m_sPrompt
  m_iCommand = 0;
  m_bSkipHistory = false;
  
  SEitf* pSEitf = this;
  gpSEitfList->vAppend(pSEitf);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SEitf::~SEitf(void)
{
  SEitf* pSEitf = this;
  
  gpSEitfList->vRemove(pSEitf);
}

/*----------------------------------------------------------------------------*/
/* pop dictionary from dictionary stack                                       */
/*----------------------------------------------------------------------------*/
DictionaryStack* SEitf::pDicStack(void)
{
  return &m_xDicStack;
}

/*----------------------------------------------------------------------------*/
/* push dictionary onto dictionary stack                                      */
/*----------------------------------------------------------------------------*/
void SEitf::vPushDic(Dictionary* pDictionary)
{
  m_xDicStack.vPush(pDictionary);
}

/*----------------------------------------------------------------------------*/
/* pop dictionary from dictionary stack                                       */
/*----------------------------------------------------------------------------*/
void SEitf::vPopDic(void)
{
  m_xDicStack.vDiscard();
}

/*----------------------------------------------------------------------------*/
/* find word given name                                                       */
/*----------------------------------------------------------------------------*/
Word* SEitf::pFindWord(s& sName)
{
  _poolAddress_(this);
  
  Word* pWord = m_xDicStack.pFind(sName.a());
  
  return pWord;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void SEitf::vDump(void)
{
  vTrace("SEitf %p: pid %lld (%s)\n",
         this,
         m_iPid,
         Process::bTestPid(m_iPid) ? "exists" : "non-existant"
         );
  
  vTrace("  RequestQueue %p  ResponseQueue %p\n",
         m_pRequestQueue,
         m_pResponseQueue);
}

/*----------------------------------------------------------------------------*/
/* initialise this seitf                                                      */
/*----------------------------------------------------------------------------*/
void SEitf::vInit(void)
{
  // pid
  m_iPid = Process::iGetPid();
  m_xDictionary.vAddConstant("pid",m_iPid);
  
  // response queue
  Word* pQueueWord = gpDictionary->pFind("queue");
  Word* pRspQueWord = new(&m_xDictionary) Word("rspque");
  pRspQueWord->vWordType(WT_OBJECT);
  pRspQueWord->vKind(pQueueWord);
  pRspQueWord->vFunc(nullptr);
  pRspQueWord->pAllot(sizeof(Queue));
  
  m_pResponseQueue = reinterpret_cast<Queue*>(pRspQueWord->pBodyArea());
  m_pResponseQueue->vSetMagic();
  m_pResponseQueue->vClass(CN_QUEUE);
  m_pResponseQueue->vInitialize();
  
  // request queue
  Word* pReqQueWord = gpDictionary->pFind("requestQueue");
  ASSERT(pReqQueWord);
  
  m_pRequestQueue = reinterpret_cast<Queue*>(pReqQueWord->pBodyArea());
  
  // dictionary stack
  m_xDicStack.vPush(gpDictionary);
  m_xDicStack.vPush(&m_xDictionary);
}

/*----------------------------------------------------------------------------*/
/* run seitf>                                                                 */
/*----------------------------------------------------------------------------*/
void SEitf::vPrompt2(void)
{
  s32  sPrompt;
  s256 sCommand;
  s256 sResponse;
  
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
  
  sPrompt = "seitf> ";

  while (true) {
    sCommand  = "<no command>";
    sResponse = "<no response>";
    
    if (Prompt::bGetInput(sPrompt,sCommand)) {
      vParse(sCommand,pWords);
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
   
    }
  }
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

void SEitf::vPrompt(void)
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
  
  m_sPrompt = "seitf> ";
  
  pInput()->vUseStdin(m_sInputLine);
  pInput()->vPrompt(m_sPrompt.a());
  pInput()->vLineOriented(true);
  
  vHistoryOpen();
  
  while (true) {
    
    for (i8 i = 0; i < 16; i++)
      *pWords[i] = '\0';
    
    InputStream* pInput = this->pInput();
    
    i8 i = 0;
    pInput->vEOL(false);
    do {
      pInput->eNextLiteral(pWords[i]);
      //vTrace("Word [%lld] %s\n",i,pWords[i]);
      i++;
    }
    while (! pInput->bEOL() && ! pInput->bEOF());
    
    if (pInput->bEOF()) {
      vPopFile();
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
    
  }
}

/*----------------------------------------------------------------------------*/
/* parse command and break into words                                         */
/*----------------------------------------------------------------------------*/
void SEitf::vParse(s& sCommand, char* pWords[])
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
void SEitf::vExecute(char* pWords[])
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
  
  vTrace("%lld %s (%lldus)\n",iStatus,m_sResponse.a(),xTimer.iMicroseconds());
}

/*----------------------------------------------------------------------------*/
/* iterate rows                                                               */
/*----------------------------------------------------------------------------*/
void SEitf::vIterateRows(void)
{
  i8 iAddress = String::iToI8(m_sResponse.a());
  
  RowCursor* pRowCursor = reinterpret_cast<RowCursor*>(iAddress);
  ASSERT(CN_ROWCURSOR == pRowCursor->eClass());
  
  pRowCursor->vBegin();
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
void SEitf::vAppendRequest(s& sRequest)
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
  
  bool b = m_pRequestQueue->bAppend(&xValSet);
  ASSERT(b);
}

/*----------------------------------------------------------------------------*/
/* pop response from reponse queue (response is from pointer in seitf(shm))   */
/*----------------------------------------------------------------------------*/
i8 SEitf::iGetResponse(void)
{
  i8 iStatus;
  while (! bPopResponse(iStatus, m_sResponse))
  ;
  
  return iStatus;
}

/*----------------------------------------------------------------------------*/
/* pop response from reponse queue                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bPopResponse(s& sResponse)
{
  ValWsp xResponse;
  
  if (! m_pResponseQueue->bPop(&xResponse))
    return false;
  
  sResponse = xResponse.sStr();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* pop response from reponse queue                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bPopResponse(i8& iResult, s& sResponse)
{
  bool b;
  ValSet xValSet;
  char* pCursor = nullptr;
  
  if (! m_pResponseQueue->bPop(&xValSet))
    return false;
  
  ValWsp* pValWsp;
  
  b = xValSet.bExtract(pCursor,pValWsp);
  ASSERT(b);
  iResult = pValWsp->iInt();
  
  b = xValSet.bExtract(pCursor,pValWsp);
  ASSERT(b);
  sResponse = pValWsp->sStr();
  
  b = xValSet.bExtract(pCursor,pValWsp);
  ASSERT(! b);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get request queue                                                          */
/*----------------------------------------------------------------------------*/
Queue* SEitf::pRequestQueue(void)
{
  return m_pRequestQueue;
}

/*----------------------------------------------------------------------------*/
/* set request queue                                                          */
/*----------------------------------------------------------------------------*/
void SEitf::vRequestQueue(Queue* pRequestQueue)
{
  m_pRequestQueue = pRequestQueue;
}
/*----------------------------------------------------------------------------*/
/* get response queue                                                         */
/*----------------------------------------------------------------------------*/
Queue* SEitf::pResponseQueue(void)
{
  return m_pResponseQueue;
}

/*----------------------------------------------------------------------------*/
/* set response queue                                                         */
/*----------------------------------------------------------------------------*/
void SEitf::vResponseQueue(Queue* pResponseQueue)
{
  m_pResponseQueue = pResponseQueue;
}

/*----------------------------------------------------------------------------*/
/* do                                                                         */
/*----------------------------------------------------------------------------*/
void SEitf::vDo(const char* aCommand)
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
void SEitf::vHistoryAppend(char* pWords[])
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
void SEitf::vHistoryClose(void)
{
  m_xHistoryFile.vClose();
}

/*----------------------------------------------------------------------------*/
/* seek to end of command history file                                        */
/*----------------------------------------------------------------------------*/
void SEitf::vHistoryEnd(void)
{
  m_xHistoryFile.vSeekEnd();
}

/*----------------------------------------------------------------------------*/
/* get command from history file                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bHistoryGet(i8 iCommand, char* pWords[])
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
        pInput->vEOL(false);
        do {
          pInput->eNextLiteral(pWords[i]);
          vTrace("Word [%lld] %s\n",i,pWords[i]);
          i++;
        }
        while (! pInput->bEOL() && ! pInput->bEOF());
        
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
void SEitf::vHistoryOpen(void)
{
  char wFileName[C_1KB];
  
  String::vSprint(wFileName,"/tmp/SEitf_%lld_history.txt",Process::iGetPid());
  
  bool b = m_xHistoryFile.bOpen(wFileName,C_OPEN_READ_WRITE_CREATE);
  ASSERT(b);
}

/*----------------------------------------------------------------------------*/
/* print command history file                                                 */
/*----------------------------------------------------------------------------*/
void SEitf::vHistoryPrint(void)
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
bool SEitf::bHistoryRead(char* wCommand)
{
  bool bResult = m_xHistoryFile.bReadLine(wCommand);
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* recall command from history file                                           */
/*----------------------------------------------------------------------------*/
bool SEitf::bHistoryRecall(char* pWords[])
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
void SEitf::vHistoryStart(void)
{
  m_xHistoryFile.vSeek(0);
}


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* decode request                                                             */
/*----------------------------------------------------------------------------*/
bool SEitf::bDecode(char* pWords[])
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
bool SEitf::bTestFile(char* pWord)
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
void SEitf::vPushFile(char* pWord)
{
  ASSERT(m_iInput < C_INPUT_ARRAY - 1);
  m_iInput++;
  
  InputStream* pInput = this->pInput();
  
  bool b = pInput->bOpenFile(pWord);
  ASSERT(b);
  
  pInput->vLineOriented(true);
}

/*----------------------------------------------------------------------------*/
/* pop command file                                                           */
/*----------------------------------------------------------------------------*/
void SEitf::vPopFile(void)
{
  ASSERT(m_iInput);

  InputStream* pInput = this->pInput();
  
  pInput->vCloseFile();
  
  m_iInput--;
}

/*----------------------------------------------------------------------------*/
/* get current input stream                                                   */
/*----------------------------------------------------------------------------*/
InputStream* SEitf::pInput(void)
{
  return &m_xInputStream[m_iInput];
}

/*----------------------------------------------------------------------------*/
/* help                                                                       */
/*----------------------------------------------------------------------------*/
bool SEitf::bHelp(char* pWords[])
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
bool SEitf::bConnect(char* pWords[])
{
  s256 sRequest;
  
  sRequest = "connect";
  vAppendRequest(sRequest);

  return true;
}

/*----------------------------------------------------------------------------*/
/* disconnect from se                                                         */
/*----------------------------------------------------------------------------*/
bool SEitf::bDisconnect(char* pWords[])
{
  s256 sRequest;

  sRequest = "disconnect";
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop a dictionary                                                          */
/*----------------------------------------------------------------------------*/
bool SEitf::bDropDictionary(char* pWords[])
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
bool SEitf::bCreateTenant(char* pWords[])
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
bool SEitf::bListTenants(char* pWords[])
{
  s256 sRequest;
  
  sRequest = "list_tenants";
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop a tenant                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bDropTenant(char* pWords[])
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
bool SEitf::bCreateCatalog(char* pWords[])
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
bool SEitf::bListCatalogs(char* pWords[])
{
  s256 sRequest;
  
  sRequest = "list_catalogs";
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop a catalog                                                             */
/*----------------------------------------------------------------------------*/
bool SEitf::bDropCatalog(char* pWords[])
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
bool SEitf::bCreateSchema(char* pWords[])
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
bool SEitf::bListSchemas(char* pWords[])
{
  s256 sRequest;
  
  sRequest = "list_schemas";
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop a schema                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bDropSchema(char* pWords[])
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
bool SEitf::bCreateTable(char* pWords[])
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
bool SEitf::bListTables(char* pWords[])
{
  s256 sRequest;
  
  sRequest = "list_tables";
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* dump a table                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bDumpTable(char* pWords[])
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
bool SEitf::bDropTable(char* pWords[])
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
bool SEitf::bAddColumn(char* pWords[16])
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
bool SEitf::bListColumns(char* pWords[])
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
bool SEitf::bCopyTable(char* pWords[])
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
bool SEitf::bExportTable(char* pWords[])
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
bool SEitf::bLoadTable(char* pWords[])
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
bool SEitf::bSelectTable(char* pWords[])
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
bool SEitf::bAddressTable(char* pWords[])
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
bool SEitf::bSizeTable(char* pWords[])
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
bool SEitf::bTruncateTable(char* pWords[])
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
bool SEitf::bPopDic(char* pWords[])
{
  return false;
}

/*----------------------------------------------------------------------------*/
/* push dictionary                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bPushDic(char* pWords[])
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
/* get dictionary path                                                        */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetDicPath(char* pWords[])
{
  s256 sRequest;
  
  sRequest = "pwd";
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* set dictionary path                                                        */
/*----------------------------------------------------------------------------*/
bool SEitf::bSetDicPath(char* pWords[])
{
  return true;
}

/*----------------------------------------------------------------------------*/
/* get dictionary contents                                                    */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetDicContents(char* pWords[])
{
  s256 sRequest;
  
  sRequest = "ls";
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get monitor string                                                         */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetMonitor(char* pWords[])
{
  s256 sRequest;
  
  sRequest = "get_monitor";
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* print command history                                                      */
/*----------------------------------------------------------------------------*/
bool SEitf::bPrintHistory(char* pWords[])
{
  vHistoryPrint();
  return false;
}

/*----------------------------------------------------------------------------*/
/* ping test                                                                  */
/*----------------------------------------------------------------------------*/
bool SEitf::bPing(char* pWords[])
{
  s256 sRequest;
  
  sRequest = "ping";
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* x test                                                                     */
/*----------------------------------------------------------------------------*/
bool SEitf::bX(char* pWords[])
{
  s256 sRequest;
  
  sRequest = "x";
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* do                                                                         */
/*----------------------------------------------------------------------------*/
bool SEitf::bDo(char* pWords[])
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
const char* SEitf::pGetLastError(void) {
  return m_sInterfaceError.a();
}

/*----------------------------------------------------------------------------*/
/* clear last error                                                           */
/*----------------------------------------------------------------------------*/
void SEitf::vClearLastError(void) {
  m_eInterfaceError = IE_NONE;
  String::vCopy(m_sInterfaceError.a(), "");
}

/*----------------------------------------------------------------------------*/
/* set last error                                                             */
/*----------------------------------------------------------------------------*/
void SEitf::vSetLastError(eIE eInterfaceError) {
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
bool SEitf::bTpcH(char* pWords[])
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