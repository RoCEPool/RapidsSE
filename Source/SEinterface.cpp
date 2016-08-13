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
  
  sPrompt = "seitf> ";
  while (true) {
    
    sCommand  = "<no command>";
    sResponse = "<no response>";
    
    if (Prompt::bGetInput(sPrompt,sCommand)) {
    
      if ( ! bDecode(sCommand) )
        continue;
      
      while (! bPopResponse(iStatus,sResponse))
        ;
      
      vTrace("%lld %s\n",iStatus,sResponse.a());
    }
  }
}

/*----------------------------------------------------------------------------*/
/* decode request                                                             */
/*----------------------------------------------------------------------------*/
bool SEinterface::bDecode(s& sCommand)
{
  if (String::bEqualTo("?",sCommand.a(),C_NO_CASE) ||
      String::bEqualTo("help",sCommand.a(),C_NO_CASE)) {
    bHelp();
    return false;
  }
  
  if (String::bEqualTo("bye",sCommand.a(),C_NO_CASE)) {
    exit(EXIT_SUCCESS);
  }
  
  if (String::bEqualTo("cd",sCommand.a(),C_NO_CASE)) {
    bPushDic();
    return true;
  }
  
  if (String::bEqualTo("con",sCommand.a(),C_NO_CASE)) {
    bConnect();
    return true;
  }

  if (String::bEqualTo("cpy",sCommand.a(),C_NO_CASE)) {
    bCopyTable();
    return true;
  }

  if (String::bEqualTo("dis",sCommand.a(),C_NO_CASE)) {
    bDisconnect();
    return true;
  }
  
  if (String::bEqualTo("exp",sCommand.a(),C_NO_CASE)) {
    bExportTable();
    return true;
  }
  
  if (String::bEqualTo("lod",sCommand.a(),C_NO_CASE)) {
    bLoadTable();
    return true;
  }
  
  if (String::bEqualTo("ls",sCommand.a(),C_NO_CASE)) {
    bGetDicContents();
    return true;
  }
  
  if (String::bEqualTo("mkcat",sCommand.a(),C_NO_CASE)) {
    bCreateCatalog();
    return true;
  }
  
  if (String::bEqualTo("mksch",sCommand.a(),C_NO_CASE)) {
    bCreateSchema();
    return true;
  }
  
  if (String::bEqualTo("mktbl",sCommand.a(),C_NO_CASE)) {
    bCreateTable();
    return true;
  }
  
  if (String::bEqualTo("mkten",sCommand.a(),C_NO_CASE)) {
    bCreateTenant();
    return true;
  }
  
  if (String::bEqualTo("pop",sCommand.a(),C_NO_CASE)) {
    bPopDic();
    return true;
  }
  
  if (String::bEqualTo("pwd",sCommand.a(),C_NO_CASE)) {
    bGetDicPath();
    return true;
  }
  
  if (String::bEqualTo("rmcat",sCommand.a(),C_NO_CASE)) {
    bDropCatalog();
    return true;
  }
  
  if (String::bEqualTo("rmsch",sCommand.a(),C_NO_CASE)) {
    bDropSchema();
    return true;
  }
  
  if (String::bEqualTo("rmtbl",sCommand.a(),C_NO_CASE)) {
    bDropTable();
    return true;
  }
  
  if (String::bEqualTo("rmten",sCommand.a(),C_NO_CASE)) {
    bDropTenant();
    return true;
  }
  
  if (String::bEqualTo("sel",sCommand.a(),C_NO_CASE)) {
    bSelectTable();
    return true;
  }
  
  if (String::bEqualTo("tr",sCommand.a(),C_NO_CASE)) {
    bTruncateTable();
    return true;
  }
  
  if (String::bEqualTo("use",sCommand.a(),C_NO_CASE)) {
    bSetDicPath();
    return true;
  }
  
  if (String::bEqualTo("x",sCommand.a(),C_NO_CASE)) {
    bX();
    return true;
  }
  
  if (0 == String::iLength(sCommand.a()))
    return false;
    
  vTrace("Unknown command: %s\n",sCommand.a());
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
  sResponse = pValWsp->sStr();
  
  b = xValSet.bExtract(pCursor,pValWsp);
  ASSERT(b);
  iResult = pValWsp->iInt();

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