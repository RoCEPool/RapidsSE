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
  //m_xDictionary;
  m_pRequestQueue  = nullptr;
  m_pResponseQueue = nullptr;
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SEinterface::~SEinterface(void)
{
}

/*----------------------------------------------------------------------------*/
/* get dictionary                                                             */
/*----------------------------------------------------------------------------*/
Dictionary* SEinterface::pDictionary(void)
{
  return &m_xDictionary;
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
  if (String::bEqualTo("con",sCommand.a(),C_NO_CASE)) {
    bConnect();
    return true;
  }

  if (String::bEqualTo("dis",sCommand.a(),C_NO_CASE)) {
    bDisconnect();
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
  String::vConcat(wWorkspace," >seitf task.this task.itf! ");
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
  
  sRequest = "mkten boray";
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
bool SEinterface::bDroptable()
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
/* fetch rows from a table                                                    */
/*----------------------------------------------------------------------------*/
bool SEinterface::bFetchTable()
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
  return false;
}

/*----------------------------------------------------------------------------*/
/* get dictionary path                                                        */
/*----------------------------------------------------------------------------*/
bool SEinterface::bGetDicPath()
{
  return false;
}

/*----------------------------------------------------------------------------*/
/* set dictionary path                                                        */
/*----------------------------------------------------------------------------*/
bool SEinterface::bSetDicPath()
{
  return false;
}

/*----------------------------------------------------------------------------*/
/* get dictionary contents                                                    */
/*----------------------------------------------------------------------------*/
bool SEinterface::bGetDicContents()
{
  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/