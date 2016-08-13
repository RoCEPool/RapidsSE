/*============================================================================*/
/* SEinterface.cpp - se interface                                             */
/*============================================================================*/

#include "StorageEngine.h"

extern Heap*        gpSmallHeap;
extern Dictionary*  gpDictionary;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* SEinterface::operator new(size_t iSize)
{
  Object* pObject = gpSmallHeap->pAllocObject();
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void SEinterface::operator delete(void* pSEinterfaceAddress)
{
  SEinterface* pSEitf = reinterpret_cast<SEinterface*>(pSEinterfaceAddress);
  
  gpSmallHeap->vFreeObject(pSEitf);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SEinterface::SEinterface(void)
: Object(CN_SEINTERFACE)
{
  m_iPid = 0;
  //m_xDictionary;
  //m_xDicStack;
  m_pRequestQueue  = nullptr;
  m_pResponseQueue = nullptr;
  
  m_xDictionary.vName("seitf");
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SEinterface::~SEinterface(void)
{
}

/*----------------------------------------------------------------------------*/
/* pop dictionary from dictionary stack                                       */
/*----------------------------------------------------------------------------*/
DictionaryStack* SEinterface::pDicStack(void)
{
  return &m_xDicStack;
}

/*----------------------------------------------------------------------------*/
/* push dictionary onto dictionary stack                                      */
/*----------------------------------------------------------------------------*/
void SEinterface::vPushDic(Dictionary* pDictionary)
{
  m_xDicStack.vPush(pDictionary);
}

/*----------------------------------------------------------------------------*/
/* pop dictionary from dictionary stack                                       */
/*----------------------------------------------------------------------------*/
void SEinterface::vPopDic(void)
{
  m_xDicStack.vDiscard();
}

/*----------------------------------------------------------------------------*/
/* find word given name                                                       */
/*----------------------------------------------------------------------------*/
Word* SEinterface::pFindWord(s& sName)
{
  _poolAddress_(this);
  
  Word* pWord = m_xDicStack.pFind(sName.a());
  
  return pWord;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void SEinterface::vDump(const char* aTitle)
{
  vTrace("%s %p:\n",aTitle,this);
  vTrace("  RequestQueue %p  ResponseQueue %p\n",
         m_pRequestQueue,
         m_pResponseQueue);
}

/*----------------------------------------------------------------------------*/
/* initialise this seitf                                                      */
/*----------------------------------------------------------------------------*/
void SEinterface::vInit(void)
{
  m_iPid = Process::iGetPid();
  
  Word* pReqQueWord = gpDictionary->pFind("requestQueue");
  
  ASSERT(pReqQueWord);
  
  m_pRequestQueue = reinterpret_cast<Queue*>(pReqQueWord->pBodyArea());
  
  Word* pRspQueWord = new(&m_xDictionary) Word("rspque");
  pRspQueWord->vWordType(WT_OBJECT);
  pRspQueWord->vFunc(&Task::vNop);
  pRspQueWord->pAllot(sizeof(Queue));
//  pRspQueWord->vEncode("encodeDoes");
//  pRspQueWord->vEncode("exit");
  
  m_pResponseQueue = reinterpret_cast<Queue*>(pRspQueWord->pBodyArea());
  m_pResponseQueue->vSetMagic();
  m_pResponseQueue->vClass(CN_QUEUE);
  m_pResponseQueue->vInitialize();
  
  m_xDicStack.vPush(gpDictionary);
  m_xDicStack.vPush(&m_xDictionary);
}

/*----------------------------------------------------------------------------*/
/* run seitf>                                                                 */
/*----------------------------------------------------------------------------*/
void SEinterface::vPrompt(void)
{
  s32  sPrompt;
  s256 sCommand;
  i8   iStatus;
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
    
      if ( ! bDecode(pWords) )
        continue;
      
      while (! bPopResponse(iStatus,sResponse))
        ;
      
      vTrace("%lld %s\n",iStatus,sResponse.a());
    }
  }
}

/*----------------------------------------------------------------------------*/
/* parse command and break into words                                         */
/*----------------------------------------------------------------------------*/
void SEinterface::vParse(s& sCommand, char* pWords[])
{
  char* ps = sCommand.a();
  
  String::vSkipWhite(&ps);
  for (i8 i = 0; String::bExtractWord(&ps,pWords[i]); i++) {
    vTrace("Word[%lld] %s\n",i,pWords[i]);
    String::vSkipWhite(&ps);
  }
}

/*----------------------------------------------------------------------------*/
/* decode request                                                             */
/*----------------------------------------------------------------------------*/
bool SEinterface::bDecode(char* pWords[])
{
  if (String::bEqualTo("?",pWords[0],C_NO_CASE) ||
      String::bEqualTo("help",pWords[0],C_NO_CASE)) {
    bHelp();
    return false;
  }
  
  if (String::bEqualTo("bye",pWords[0],C_NO_CASE)) {
    exit(EXIT_SUCCESS);
  }
  
  if (String::bEqualTo("cd",pWords[0],C_NO_CASE)) {
    bPushDic();
    return true;
  }
  
  if (String::bEqualTo("con",pWords[0],C_NO_CASE)) {
    bConnect();
    return true;
  }

  if (String::bEqualTo("cpy",pWords[0],C_NO_CASE)) {
    bCopyTable();
    return true;
  }

  if (String::bEqualTo("dis",pWords[0],C_NO_CASE)) {
    bDisconnect();
    return true;
  }
  
  if (String::bEqualTo("exp",pWords[0],C_NO_CASE)) {
    bExportTable();
    return true;
  }
  
  if (String::bEqualTo("lod",pWords[0],C_NO_CASE)) {
    bLoadTable();
    return true;
  }
  
  if (String::bEqualTo("ls",pWords[0],C_NO_CASE)) {
    bGetDicContents();
    return true;
  }
  
  if (String::bEqualTo("mkcat",pWords[0],C_NO_CASE)) {
    bCreateCatalog();
    return true;
  }
  
  if (String::bEqualTo("mksch",pWords[0],C_NO_CASE)) {
    bCreateSchema();
    return true;
  }
  
  if (String::bEqualTo("mktbl",pWords[0],C_NO_CASE)) {
    bCreateTable();
    return true;
  }
  
  if (String::bEqualTo("mkten",pWords[0],C_NO_CASE)) {
    bCreateTenant();
    return true;
  }
  
  if (String::bEqualTo("pop",pWords[0],C_NO_CASE)) {
    bPopDic();
    return true;
  }
  
  if (String::bEqualTo("pwd",pWords[0],C_NO_CASE)) {
    bGetDicPath();
    return true;
  }
  
  if (String::bEqualTo("rmcat",pWords[0],C_NO_CASE)) {
    bDropCatalog();
    return true;
  }
  
  if (String::bEqualTo("rmsch",pWords[0],C_NO_CASE)) {
    bDropSchema();
    return true;
  }
  
  if (String::bEqualTo("rmtbl",pWords[0],C_NO_CASE)) {
    bDropTable();
    return true;
  }
  
  if (String::bEqualTo("rmten",pWords[0],C_NO_CASE)) {
    bDropTenant();
    return true;
  }
  
  if (String::bEqualTo("sel",pWords[0],C_NO_CASE)) {
    bSelectTable();
    return true;
  }
  
  if (String::bEqualTo("tr",pWords[0],C_NO_CASE)) {
    bTruncateTable();
    return true;
  }
  
  if (String::bEqualTo("use",pWords[0],C_NO_CASE)) {
    bSetDicPath();
    return true;
  }
  
  if (String::bEqualTo("x",pWords[0],C_NO_CASE)) {
    bX();
    return true;
  }
  
  if (0 == String::iLength(pWords[0]))
    return false;
    
  vTrace("Unknown command: %s\n",pWords[0]);
  return false;
}

/*----------------------------------------------------------------------------*/
/* append request to request queue                                            */
/*----------------------------------------------------------------------------*/
void SEinterface::vAppendRequest(s& sRequest)
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
/* pop response from reponse queue                                            */
/*----------------------------------------------------------------------------*/
bool SEinterface::bPopResponse(s& sResponse)
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
bool SEinterface::bPopResponse(i8& iResult, s& sResponse)
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
Queue* SEinterface::pRequestQueue(void)
{
  return m_pRequestQueue;
}

/*----------------------------------------------------------------------------*/
/* set request queue                                                          */
/*----------------------------------------------------------------------------*/
void SEinterface::vRequestQueue(Queue* pRequestQueue)
{
  m_pRequestQueue = pRequestQueue;
}
/*----------------------------------------------------------------------------*/
/* get response queue                                                         */
/*----------------------------------------------------------------------------*/
Queue* SEinterface::pResponseQueue(void)
{
  return m_pResponseQueue;
}

/*----------------------------------------------------------------------------*/
/* set response queue                                                         */
/*----------------------------------------------------------------------------*/
void SEinterface::vResponseQueue(Queue* pResponseQueue)
{
  m_pResponseQueue = pResponseQueue;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* help                                                                       */
/*----------------------------------------------------------------------------*/
bool SEinterface::bHelp()
{
  vTrace("seitf temporary commands:\n");
  vTrace("bye   -- quit seitf\n");
  vTrace("cd    -- change dictionary\n");
  vTrace("con   -- connect\n");
  vTrace("cpy   -- select into t2 from t;\n");
  vTrace("dis   -- disconnect\n");
  vTrace("exp   -- export table t\n");
  vTrace("lod   -- load table t\n");
  vTrace("ls    -- list dictionary contents\n");
  vTrace("mkcat -- create catalog ten.cat;\n");
  vTrace("mksch -- create schema ten.cat.sch\n");
  vTrace("mktbl -- create table ten.cat.sch.t\n");
  vTrace("mkten -- create tenant ten;\n");
  vTrace("pop   -- cd ..\n");
  vTrace("pwd   -- print working dictionary\n");
  vTrace("rmcat -- drop catalog ten.cat;\n");
  vTrace("rmsch -- drop schema ten.cat.sch;\n");
  vTrace("rmtbl -- drop table ten.cat.sch.t;\n");
  vTrace("rmten -- drop tenant ten;\n");
  vTrace("sel   -- select * from t;\n");
  vTrace("tr    -- truncate table t;\n");
  vTrace("use   -- set path to ten.cat.sch\n");
  vTrace("?     -- print this help\n");
  vTrace("help  -- print this help\n");
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* connect to se                                                              */
/*----------------------------------------------------------------------------*/
bool SEinterface::bConnect()
{
  s256 sRequest;
  
  sRequest = "connect";
  vAppendRequest(sRequest);

  return true;
}

/*----------------------------------------------------------------------------*/
/* disconnect from se                                                         */
/*----------------------------------------------------------------------------*/
bool SEinterface::bDisconnect()
{
  s256 sRequest;

  sRequest = "disconnect";
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* create a tenant                                                            */
/*----------------------------------------------------------------------------*/
bool SEinterface::bCreateTenant()
{
  s256 sRequest;
  
  sRequest = "\"ten\" create_tenant";
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop a tenant                                                              */
/*----------------------------------------------------------------------------*/
bool SEinterface::bDropTenant()
{
  return false;
}

/*----------------------------------------------------------------------------*/
/* create a catalog                                                           */
/*----------------------------------------------------------------------------*/
bool SEinterface::bCreateCatalog()
{
  s256 sRequest;
  
  sRequest = "mkcat cat1";
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop a catalog                                                             */
/*----------------------------------------------------------------------------*/
bool SEinterface::bDropCatalog()
{
  return false;
}

/*----------------------------------------------------------------------------*/
/* create a schema                                                            */
/*----------------------------------------------------------------------------*/
bool SEinterface::bCreateSchema()
{
  s256 sRequest;
  
  sRequest = "mksch sch1";
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop a schema                                                              */
/*----------------------------------------------------------------------------*/
bool SEinterface::bDropSchema()
{
  return false;
}

/*----------------------------------------------------------------------------*/
/* create a table                                                             */
/*----------------------------------------------------------------------------*/
bool SEinterface::bCreateTable()
{
  s256 sRequest;
  
  sRequest = "mktbl T";
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop a table                                                               */
/*----------------------------------------------------------------------------*/
bool SEinterface::bDropTable()
{
  return false;
}

/*----------------------------------------------------------------------------*/
/* copy a table                                                               */
/*----------------------------------------------------------------------------*/
bool SEinterface::bCopyTable()
{
  return false;
}

/*----------------------------------------------------------------------------*/
/* export a table                                                             */
/*----------------------------------------------------------------------------*/
bool SEinterface::bExportTable()
{
  return false;
}

/*----------------------------------------------------------------------------*/
/* load a table from file                                                     */
/*----------------------------------------------------------------------------*/
bool SEinterface::bLoadTable()
{
  return false;
}

/*----------------------------------------------------------------------------*/
/* select rows from a table                                                   */
/*----------------------------------------------------------------------------*/
bool SEinterface::bSelectTable()
{
  return false;
}

/*----------------------------------------------------------------------------*/
/* truncate a table                                                           */
/*----------------------------------------------------------------------------*/
bool SEinterface::bTruncateTable()
{
  return false;
}

/*----------------------------------------------------------------------------*/
/* pop dictionary                                                             */
/*----------------------------------------------------------------------------*/
bool SEinterface::bPopDic()
{
  return false;
}

/*----------------------------------------------------------------------------*/
/* push dictionary                                                            */
/*----------------------------------------------------------------------------*/
bool SEinterface::bPushDic()
{
  s256 sRequest;
  
  sRequest = "\"tenants\" cd";
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get dictionary path                                                        */
/*----------------------------------------------------------------------------*/
bool SEinterface::bGetDicPath()
{
  s256 sRequest;
  
  sRequest = "pwd";
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* set dictionary path                                                        */
/*----------------------------------------------------------------------------*/
bool SEinterface::bSetDicPath()
{
  return true;
}

/*----------------------------------------------------------------------------*/
/* get dictionary contents                                                    */
/*----------------------------------------------------------------------------*/
bool SEinterface::bGetDicContents()
{
  return false;
}

/*----------------------------------------------------------------------------*/
/* x test                                                                     */
/*----------------------------------------------------------------------------*/
bool SEinterface::bX()
{
  s256 sRequest;
  
  sRequest = "x";
  vAppendRequest(sRequest);
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/