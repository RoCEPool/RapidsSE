/*============================================================================*/
/* SEitf.cpp - se interface                                                   */
/*============================================================================*/

#include "Interface.h"
#include "StorageEngine.h"

extern Heap*      gpMediumHeap;
extern Dic*       gpMainDic;
extern SEitfList* gpSEitfList;
extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SEitf* Interface::_pSEitf(void* pAddr)
{
  SEitf* pSEitf = reinterpret_cast<SEitf*>(pAddr);
  
  RASSERT( !pSEitf || CN_SEITF == pSEitf->eClass() );
  
  return pSEitf;
}

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SEitf* Interface::_pSEitf(i8 iAddr)
{
  SEitf* pSEitf = reinterpret_cast<SEitf*>(iAddr);
  
  RASSERT( !pSEitf || CN_SEITF == pSEitf->eClass() );
  
  return pSEitf;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* SEitf::operator new(size_t iSize)
{
  RASSERT(iSize);
  
  Object* pObject = Heap::pAllocate(CN_SEITF);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void SEitf::operator delete(void* pSEinterfaceAddress)
{
  SEitf* pSEitf = reinterpret_cast<SEitf*>(pSEinterfaceAddress);
  
  Heap::vFree(pSEitf);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SEitf::SEitf(i8 iPartition)
: Object(CN_SEITF)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  // pid
  m_iPid = Process::iGetPid();

  m_iPartitions = gpAttachMap->m_iPartitions;
  m_iPartition  = iPartition;
  
  m_xDic.vName("seitf");
  //m_xDicStack;
  m_pRequestQueue   = nullptr;
  m_pResponseQueue  = nullptr;
  m_eErrorCode      = EC_NONE;
  m_sErrorMsg       = pEC[m_eErrorCode];
  m_sResponse       = "";
  m_pRowPtr         = nullptr;
  m_pCurrentTable   = nullptr;
  m_pTxRef          = nullptr;
  m_iCurrentRequestId = 0;
  m_iNextRequestId    = -1;
  
  SEitf* pSEitf = this;
  gpSEitfList->vAppend(pSEitf);
  
  for (i8 iPartition = 0; iPartition < C_TABLE_PARTITIONS; iPartition++)
    m_pRowBuffer[iPartition] = nullptr;

  _checkInitialised_(CN_SEITF);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SEitf::~SEitf(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);

  SEitf* pSEitf = this;
  
  m_pResponseQueue->vFinalize();
  Word* pRspQueWord = m_xDic.pFind("rspque");
  delete pRspQueWord;

  gpSEitfList->vRemove(pSEitf);
}

/*----------------------------------------------------------------------------*/
/* pop dictionary from dictionary stack                                       */
/*----------------------------------------------------------------------------*/
DicStack* SEitf::pDicStack(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);

  return &m_xDicStack;
}

/*----------------------------------------------------------------------------*/
/* push dictionary onto dictionary stack                                      */
/*----------------------------------------------------------------------------*/
void SEitf::vPushDic(Dic* pDic)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);

  m_xDicStack.vPush(pDic);
}

/*----------------------------------------------------------------------------*/
/* pop dictionary from dictionary stack                                       */
/*----------------------------------------------------------------------------*/
void SEitf::vPopDic(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);

  m_xDicStack.vDiscard();
}

/*----------------------------------------------------------------------------*/
/* find word given name                                                       */
/*----------------------------------------------------------------------------*/
Word* SEitf::pFindWord(s& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  Word* pWord = m_xDicStack.pFind(sName.a());
  
  return pWord;
}

/*----------------------------------------------------------------------------*/
/* find word given name                                                       */
/*----------------------------------------------------------------------------*/
Word* SEitf::pFindWordInCurrent(s& sName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  Word* pWord = m_xDicStack.pFindInCurrent(sName.a());
  
  return pWord;
}

/*----------------------------------------------------------------------------*/
/* set the current table                                                      */
/*----------------------------------------------------------------------------*/
void SEitf::vCurrentTable(Table* pTable)
{
  m_pCurrentTable = pTable;
}

/*----------------------------------------------------------------------------*/
/* get the current table                                                      */
/*----------------------------------------------------------------------------*/
Table* SEitf::pCurrentTable(void)
{
  return m_pCurrentTable;
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
/* dump seitf                                                                 */
/*----------------------------------------------------------------------------*/
void SEitf::vDump(eCN eClass, Object* pObject, const char* pTab)
{
  ASSERT(eClass == CN_SEITF);
  ASSERT(pObject->eClass() == CN_SEITF);
  
  SEitf* pSEitf = reinterpret_cast<SEitf*>(pObject);
  
  vTrace("%sSEitf %p:\n", pTab, pSEitf);
  vTrace("%s  pid %lld\n", pTab, pSEitf->m_iPid);
  vTrace("%s  pid %s\n", pTab, Process::bTestPid(pSEitf->m_iPid) ? "exists" : "non-existant");
  vTrace("%s  partitions %lld\n", pTab, pSEitf->m_iPartitions);
  vTrace("%s  partition  %lld\n", pTab, pSEitf->m_iPartition);
  vTrace("%s  requestQ   %p\n", pTab, pSEitf->m_pRequestQueue);
  vTrace("%s  responseQ  %p\n", pTab, pSEitf->m_pResponseQueue);
  vTrace("%s  response   '%s'\n\n", pTab, pSEitf->m_sResponse.a());
}

/*----------------------------------------------------------------------------*/
/* initialise this seitf                                                      */
/*----------------------------------------------------------------------------*/
void SEitf::vInit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  // response queue
  Word* pQueueWord = gpMainDic->pFind("queue");
  Word* pRspQueWord = new(&m_xDic) Word("rspque");
  pRspQueWord->vWordType(WT_OBJECT);
  pRspQueWord->vKind(pQueueWord);

  //pRspQueWord->vEntryStep(nullptr);
  
  Word* pEmptyDoesWord = gpMainDic->pFind("emptyDoes:");
  pRspQueWord->vEntryStep(pEmptyDoesWord->pEntryStep());
  pRspQueWord->vEntryCell(pEmptyDoesWord->pEntryCell() + 2);
 
  
  pRspQueWord->pAllot(sizeof(Queue));
  
  m_pResponseQueue = reinterpret_cast<Queue*>(pRspQueWord->pDataArea());
  m_pResponseQueue->vSetMagic();
  m_pResponseQueue->vClass(CN_QUEUE);
  new(m_pResponseQueue) Queue;
  
  // request queue
  Word* pReqQueWord = gpMainDic->pFind("requestQueue");
  ASSERT(pReqQueWord);
  
  m_pRequestQueue = reinterpret_cast<Queue*>(pReqQueWord->pDataArea());
  
  // dictionary stack
  m_xDicStack.vPush(gpMainDic);
  m_xDicStack.vPush(&m_xDic);
  
  // find tenants
  Word* pTenants = gpMainDic->pFind("tenants");
  ASSERT(pTenants);
  
  Cell* pCell = pTenants->pFirstCell();
  m_xDicStack.vPush(reinterpret_cast<Dic*>(pCell));
  
  // find tenant boray
  Word* pTenant = m_xDicStack.pFindInCurrent("boray");
  ASSERT(pTenant);
  
  pCell = pTenant->pFirstCell();
  m_xDicStack.vPush(reinterpret_cast<Dic*>(pCell));
  
  // find catalogs
  Word* pCatalogs = m_xDicStack.pFindInCurrent("catalogs");
  ASSERT(pCatalogs);
  
  pCell = pCatalogs->pFirstCell();
  m_xDicStack.vPush(reinterpret_cast<Dic*>(pCell));
 
  // find catalog rapidsse
  Word* pCatalog = m_xDicStack.pFindInCurrent("rapidsse");
  ASSERT(pCatalog);
  
  pCell = pCatalog->pFirstCell();
  m_xDicStack.vPush(reinterpret_cast<Dic*>(pCell));
  
  // find schemas
  Word* pSchema = m_xDicStack.pFindInCurrent("schemas");
  ASSERT(pSchema);
  
  pCell = pSchema->pFirstCell();
  m_xDicStack.vPush(reinterpret_cast<Dic*>(pCell));
  
}

/*----------------------------------------------------------------------------*/
/* which partition                                                            */
/*----------------------------------------------------------------------------*/
i8 SEitf::iPartition(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  return m_iPartition;
}

/*----------------------------------------------------------------------------*/
/* get request queue                                                          */
/*----------------------------------------------------------------------------*/
Queue* SEitf::pRequestQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  return m_pRequestQueue;
}

/*----------------------------------------------------------------------------*/
/* set request queue                                                          */
/*----------------------------------------------------------------------------*/
void SEitf::vRequestQueue(Queue* pRequestQueue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  m_pRequestQueue = pRequestQueue;
}

/*----------------------------------------------------------------------------*/
/* get response queue                                                         */
/*----------------------------------------------------------------------------*/
Queue* SEitf::pResponseQueue(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  return m_pResponseQueue;
}

/*----------------------------------------------------------------------------*/
/* set response queue                                                         */
/*----------------------------------------------------------------------------*/
void SEitf::vResponseQueue(Queue* pResponseQueue)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  m_pResponseQueue = pResponseQueue;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* begin a tx                                                                 */
/*----------------------------------------------------------------------------*/
bool SEitf::bBeginTx()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  m_pTxRef = new TxRef();
  
  //vTrace("txref: %x\n", m_pTxRef);
  bool bResult = m_pTxRef->bBeginTx();
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* commit a tx                                                                */
/*----------------------------------------------------------------------------*/
void SEitf::vCommitTx()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  bool bResult = m_pTxRef->bCommitTx();
  ASSERT(bResult);
}

/*----------------------------------------------------------------------------*/
/* end a tx                                                                   */
/*----------------------------------------------------------------------------*/
void SEitf::vEndTx()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  bool bResult = m_pTxRef->bEndTx();
  ASSERT(bResult);
  delete m_pTxRef;
  m_pTxRef = nullptr;
}

/*----------------------------------------------------------------------------*/
/* decode request                                                             */
/*----------------------------------------------------------------------------*/
bool SEitf::bDecode(char* pWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  if (0 == String::iLength(pWords[0])) {
    return false;
  }
  
  if (String::bEqualTo("add",pWords[0],C_NO_CASE)) {
    return bAddColumn(pWords, iWords);
  }
  
  if (String::bEqualTo("cf",pWords[0],C_NO_CASE)) {
    return bPushDic(pWords);
  }
  
  if (String::bEqualTo("con",pWords[0],C_NO_CASE)) {
    return bConnect(pWords);
  }
  
  if (String::bEqualTo("cp",pWords[0],C_NO_CASE)) {
    return bCopyTable(pWords, iWords);
  }
  
  if (String::bEqualTo("dis",pWords[0],C_NO_CASE)) {
    return bDisconnect(pWords);
  }
  
  if (String::bEqualTo("docol",pWords[0],C_NO_CASE)) {
    return bDocol(pWords);
  }
  
  if (String::bEqualTo("dump",pWords[0],C_NO_CASE)) {
    return bDumpTable(pWords, iWords);
  }
  
  if (String::bEqualTo("ex",pWords[0],C_NO_CASE)) {
    return bExportTable(pWords, iWords);
  }
  
  if (String::bEqualTo("getFreeSpace",pWords[0],C_NO_CASE)) {
    return bGetFreeSpace(pWords);
  }
  
  if (String::bEqualTo("getInUseSpace",pWords[0],C_NO_CASE)) {
    return bGetInUseSpace(pWords);
  }
  
  if (String::bEqualTo("gettbl",pWords[0],C_NO_CASE)) {
    return bGetTable(pWords, iWords);
  }

  if (String::bEqualTo("getTotalSpace",pWords[0],C_NO_CASE)) {
    return bGetTotalSpace(pWords);
  }
  
  if (String::bEqualTo("TblInUse",pWords[0],C_NO_CASE)) {
    return bInUseTable(pWords, iWords);
  }
  
  if (String::bEqualTo("ld",pWords[0],C_NO_CASE)) {
    return bLoadTable(pWords, iWords);
  }

  if (String::bEqualTo("lds",pWords[0],C_NO_CASE)) {
    return bLoadTableStream(pWords, iWords);
  }
  
  if (String::bEqualTo("lf",pWords[0],C_NO_CASE)) {
    return bGetDicContents(pWords);
  }
  
  if (String::bEqualTo("lfG",pWords[0],C_NO_CASE)) {
    return bGetDicContentsG(pWords);
  }
  
  if (String::bEqualTo("lscat",pWords[0],C_NO_CASE)) {
    return bListCatalogs(pWords, iWords);
  }
  
  if (String::bEqualTo("lscol",pWords[0],C_NO_CASE)) {
    return bListColumns(pWords, iWords);
  }
  
  if (String::bEqualTo("lssch",pWords[0],C_NO_CASE)) {
    return bListSchemas(pWords, iWords);
  }
  
  if (String::bEqualTo("lstbl",pWords[0],C_NO_CASE)) {
    return bListTables(pWords, iWords);
  }
  
  if (String::bEqualTo("lsten",pWords[0],C_NO_CASE)) {
    return bListTenants(pWords, iWords);
  }
  
  if (String::bEqualTo("mkcat",pWords[0],C_NO_CASE)) {
    return bCreateCatalog(pWords, iWords);
  }
  
  if (String::bEqualTo("mksch",pWords[0],C_NO_CASE)) {
    return bCreateSchema(pWords, iWords);
  }
  
  if (String::bEqualTo("mktbl",pWords[0],C_NO_CASE)) {
    return bCreateTable(pWords, iWords);
  }
  
  if (String::bEqualTo("mkten",pWords[0],C_NO_CASE)) {
    return bCreateTenant(pWords, iWords);
  }
  
  if (String::bEqualTo("pRB",pWords[0],C_NO_CASE)) {
    return bPrintRowBuffer(pWords);
  }
  
  if (String::bEqualTo("pwf",pWords[0],C_NO_CASE)) {
    return bGetDicPath(pWords);
  }
  
  if (String::bEqualTo("rettbl",pWords[0],C_NO_CASE)) {
    return bRetTable(pWords, iWords);
  }
  
  if (String::bEqualTo("rmcat",pWords[0],C_NO_CASE)) {
    return bDropCatalog(pWords, iWords);
  }
  
  if (String::bEqualTo("rf",pWords[0],C_NO_CASE) ||
      String::bEqualTo("rmdic",pWords[0],C_NO_CASE)) {
    return bDropDictionary(pWords, iWords);
  }
  
  if (String::bEqualTo("rmsch",pWords[0],C_NO_CASE)) {
    return bDropSchema(pWords, iWords);
  }
  
  if (String::bEqualTo("rmtbl",pWords[0],C_NO_CASE)) {
    return bDropTable(pWords, iWords);
  }
  
  if (String::bEqualTo("rmten",pWords[0],C_NO_CASE)) {
    return bDropTenant(pWords, iWords);
  }
  
  if (String::bEqualTo("sel",pWords[0],C_NO_CASE)) {
    return bSelectTable(pWords, iWords);
  }
  
  if (String::bEqualTo("space",pWords[0],C_NO_CASE)) {
    return bPoolSpace(pWords, iWords);
  }
  
  if (String::bEqualTo("status",pWords[0],C_NO_CASE)) {
    return bNodeStatus(pWords, iWords);
  }
  
  if (String::bEqualTo("sz",pWords[0],C_NO_CASE)) {
    return bSizeTable(pWords, iWords);
  }
  
  if (String::bEqualTo("tr",pWords[0],C_NO_CASE)) {
    return bTruncateTable(pWords, iWords);
  }
  
  if (String::bEqualTo("usesch",pWords[0],C_NO_CASE)) {
    return bUseSchema(pWords, iWords);
  }
  
  if (String::bEqualTo("__x",pWords[0],C_NO_CASE)) {
    return bX(pWords);
  }
  
  if (String::bEqualTo("ping",pWords[0],C_NO_CASE)) {
    return bPing(pWords);
  }
  
  if (String::bEqualTo("mon",pWords[0],C_NO_CASE)) {
    return bGetMonitor(pWords);
  }
  
  if (String::bEqualTo("print",pWords[0],C_NO_CASE)) {
    vPrint(pWords);
    return false;
  }
  
  if (String::bEqualTo("seitf",pWords[0],C_NO_CASE)) {
    return bSeitfCount(pWords);
  }
  
  if (String::bEqualTo("version",pWords[0],C_NO_CASE)) {
    return bGetVersion(pWords);
  }
  
  //vTrace("Unknown command: %s\n",pWords[0]);
  vSetLastError(EC_INVALID_COMMAND);
  return false;
}

/*----------------------------------------------------------------------------*/
/* connect to se                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bConnect(char* pWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(String::bEqualTo(pWords[0],"con"));
  sRequest = "connect";
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* disconnect from se                                                         */
/*----------------------------------------------------------------------------*/
bool SEitf::bDisconnect(char* pWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(String::bEqualTo(pWords[0],"dic"));
  sRequest = "disconnect";
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* drop a dictionary                                                          */
/*----------------------------------------------------------------------------*/
bool SEitf::bDropDictionary(char* pWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.a();
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," drop_dictionary");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* create a tenant                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bCreateTenant(char* pWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.a();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }

  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," create_tenant");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* list tenants                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bListTenants(char* pWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (iWords != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }

  ASSERT(String::bEqualTo(pWords[0],"lsten"));
  sRequest = "list_tenants";
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* drop a tenant                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bDropTenant(char* pWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.a();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," drop_tenant");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* create a catalog                                                           */
/*----------------------------------------------------------------------------*/
bool SEitf::bCreateCatalog(char* pWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.a();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," create_catalog");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* list catalogs                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bListCatalogs(char* pWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (iWords != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  ASSERT(String::bEqualTo(pWords[0],"lscat"));
  sRequest = "list_catalogs";
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* drop a catalog                                                             */
/*----------------------------------------------------------------------------*/
bool SEitf::bDropCatalog(char* pWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.a();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }

  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," drop_catalog");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* create a schema                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bCreateSchema(char* pWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.a();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," create_schema");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* list schemas                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bListSchemas(char* pWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (iWords != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  ASSERT(String::bEqualTo(pWords[0],"lssch"));
  sRequest = "list_schemas";
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* drop a schema                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bDropSchema(char* pWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.a();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }

  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," drop_schema");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* use a schema                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bUseSchema(char* pWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.a();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," use_schema");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* create a table                                                             */
/*----------------------------------------------------------------------------*/
bool SEitf::bCreateTable(char* pWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.a();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," create_table");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* list tables                                                                */
/*----------------------------------------------------------------------------*/
bool SEitf::bListTables(char* pWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (iWords != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  ASSERT(String::bEqualTo(pWords[0],"lstbl"));
  sRequest = "list_tables";
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* dump a table                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bDumpTable(char* pWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.a();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," dump_table");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* docol                                                                      */
/*----------------------------------------------------------------------------*/
bool SEitf::bDocol(char* pWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.a();
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  //
  String::vConcat(a," docol_command");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* drop a table                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bDropTable(char* pWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.a();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," drop_table");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* add a column                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bAddColumn(char* pWords[16], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.a();
  
  if (iWords != 4) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
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
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* list columns                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bListColumns(char* pWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.a();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," list_columns");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* copy a table                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bCopyTable(char* pWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.a();
  
  if (iWords != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[2]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," \"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," copy_table");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* export a table                                                             */
/*----------------------------------------------------------------------------*/
bool SEitf::bExportTable(char* pWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;

  char*  a = sRequest.a();
  
  if (iWords != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[2]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," \"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," export_table");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* load a table from file                                                     */
/*----------------------------------------------------------------------------*/
bool SEitf::bLoadTable(char* pWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.a();
  
  if (iWords != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[2]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," \"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," load_table");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* load a table from stream                                                   */
/*----------------------------------------------------------------------------*/
bool SEitf::bLoadTableStream(char* pWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.a();
  
  if (iWords != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[2]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," \"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," load_table_stream");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* select rows from a table                                                   */
/*----------------------------------------------------------------------------*/
bool SEitf::bSelectTable(char* pWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.a();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," select_table");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get address/reference of a table                                           */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetTable(char* pWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.a();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," get_table");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* return whether table is in use or not                                      */
/*----------------------------------------------------------------------------*/
bool SEitf::bInUseTable(char* pWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.a();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");

  String::vConcat(a," table_inuse");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* return address/refernce of a table                                         */
/*----------------------------------------------------------------------------*/
bool SEitf::bRetTable(char* pWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.a();
  
  if (iWords != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  String::vConcat(a," return_table");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get pool space                                                             */
/*----------------------------------------------------------------------------*/
bool SEitf::bPoolSpace(char* pWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.a();
  
  if (iWords != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  String::vConcat(a," space");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get node status                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bNodeStatus(char* pWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.a();
  
  if (iWords != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  String::vConcat(a," status");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* size of table                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bSizeTable(char* pWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.a();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," size_table");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* truncate a table                                                           */
/*----------------------------------------------------------------------------*/
bool SEitf::bTruncateTable(char* pWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.a();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }

  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," truncate_table");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* insert row into row buffer                                                 */
/*----------------------------------------------------------------------------*/
bool SEitf::bInsertRow(char* pRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  // check for working buffer
  if (! m_pRowBuffer[m_iPartition]) {
    m_pRowBuffer[m_iPartition] = RowBuffer::pNewRowBuffer();
    if (! m_pRowBuffer[m_iPartition]) {
      vTrace("RowBuffer allocation failed\n");
      m_eErrorCode = EC_LOW_BUFFERS;
      return false;
    }
  }
  
  // get working buffer
  RowBuffer* pRowBuffer = m_pRowBuffer[m_iPartition];
  
  //vTrace("Buffer: %s Partition:%d\n",pRowBuffer, m_iPartition);
  
  // insert row into working buffer
  if (pRowBuffer->bInsertRow(m_pTxRef,pRow,String::iSize(pRow)))
    return true;
    
  // buffer full, append working buffer into table
  //m_pCurrentTable->vAppendBuffer(m_iPartition,pRowBuffer);
  
  //vTrace("Buffer full... Inserting buffer\n");
  bool bResult = bInsertBuffer(pRowBuffer, m_iPartition);
  
  if (!bResult)
    return bResult;
  
  // get response
  eEC eError = eGetResponse();
  if (EC_NONE != eError) {
    vSetLastError(eError);
    return false;
  }
  
  // get a new working buffer
  m_pRowBuffer[m_iPartition] = RowBuffer::pNewRowBuffer();
  if (! m_pRowBuffer[m_iPartition]) {
    vTrace("RowBuffer allocation failed\n");
    m_eErrorCode = EC_LOW_BUFFERS;
    return false;
  }
  
  pRowBuffer = m_pRowBuffer[m_iPartition];
  
  // insert row into new working buffer
  bool bInserted = pRowBuffer->bInsertRow(m_pTxRef,pRow,String::iSize(pRow));
  
  return bInserted;
  
}

/*----------------------------------------------------------------------------*/
/* insert rowbuffer into current table                                        */
/*----------------------------------------------------------------------------*/
bool SEitf::bInsertBuffer(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  //vTrace("inserting ...\n");
  
  if (! m_pRowBuffer[m_iPartition]) {
    return false;
  }
  
  bool bResult =  bInsertBuffer(m_pRowBuffer[m_iPartition],m_iPartition);
  
  // null the rowBuffer
  m_pRowBuffer[m_iPartition] = nullptr;
  
  return bResult;
}

/*----------------------------------------------------------------------------*/
/* insert rowbuffer into current table                                        */
/*----------------------------------------------------------------------------*/
bool SEitf::bInsertBuffer(RowBuffer* pRowBuffer, i8 iPartition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  //vTrace("inserting %x %d\n", pRowBuffer, m_iPartition);
  
  char xArray[C_1KB * 2];
  char* pWords[16] = {
    &xArray[C_1KB * 0],
    &xArray[C_1KB * 1]
  };

  s256 sRequest;
  s256 sBuffer;

  char* a = sRequest.a();
  
  String::wToString(iPartition, sBuffer.a());
  String::vCopy(a,"\"");
  String::vConcat(a,sBuffer.a());
  String::vConcat(a,"\"");
  
  String::wToString((i8)pRowBuffer, sBuffer.a());
  String::vConcat(a," \"");
  String::vConcat(a,sBuffer.a());
  String::vConcat(a,"\"");
  
  String::wToString((i8)m_pCurrentTable, sBuffer.a());
  String::vConcat(a," \"");
  String::vConcat(a,sBuffer.a());
  String::vConcat(a,"\"");
  
  String::vConcat(a," table_append");
  
  //vTrace("%s\n",a);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* push dictionary                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bPushDic(char* pWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.a();
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");
  
  String::vConcat(a," cf");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void SEitf::vPrint(char* pWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  printf("%s\n",pWords[1]);
}

/*----------------------------------------------------------------------------*/
/* get dictionary path                                                        */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetDicPath(char* pWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(String::bEqualTo(pWords[0],"pwf"));
  sRequest = "pwf";
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get dictionary contents                                                    */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetDicContents(char* pWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(String::bEqualTo(pWords[0],"lf"));
  sRequest = "lf";
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get dictionary contents                                                    */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetDicContentsG(char* pWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(String::bEqualTo(pWords[0],"lfG"));
  sRequest = "lfG";
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get monitor string                                                         */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetMonitor(char* pWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(String::bEqualTo(pWords[0],"mon"));
  sRequest = "get_monitor";
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get free space                                                             */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetFreeSpace(char* pWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(String::bEqualTo(pWords[0],"getFreeSpace"));
  sRequest = "getFreeSpace";
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get inUse space                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetInUseSpace(char* pWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(String::bEqualTo(pWords[0],"getInUseSpace"));
  sRequest = "getInUseSpace";
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get total space                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetTotalSpace(char* pWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(String::bEqualTo(pWords[0],"getTotalSpace"));
  sRequest = "getTotalSpace";
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get version                                                                */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetVersion(char* pWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(String::bEqualTo(pWords[0],"version"));
  sRequest = "get_version";
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* ping test                                                                  */
/*----------------------------------------------------------------------------*/
bool SEitf::bPing(char* pWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(String::bEqualTo(pWords[0],"ping"));
  sRequest = "ping";
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* print row buffer                                                           */
/*----------------------------------------------------------------------------*/
bool SEitf::bPrintRowBuffer(char* pWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  char* a = sRequest.a();
  
  String::vCopy(a,"\"");
  String::vConcat(a,pWords[1]);
  String::vConcat(a,"\"");

  ASSERT(String::bEqualTo(pWords[0],"pRB"));
  String::vConcat(a," pRB");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get seitf count                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bSeitfCount(char* pWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(String::bEqualTo(pWords[0],"seitf"));
  sRequest = "seitf_count";
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* x test                                                                     */
/*----------------------------------------------------------------------------*/
bool SEitf::bX(char* pWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(String::bEqualTo(pWords[0],"ping"));
  sRequest = "ping";
  
  //  static bool bFirst = true;
  //  if (bFirst) {
  //    bFirst = false;
  //    vAppendRequest(sRequest);
  //    vAppendRequest(sRequest);
  //    vAppendRequest(sRequest);
  //    vAppendRequest(sRequest);
  //    vAppendRequest(sRequest);
  //  }
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append request to request queue                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bAppendRequest(s& sRequest)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  char wItfAddr[32];
  char wWorkspace[C_1KB * 16];
  char* pCursor = nullptr;
  
  ValWsp xVal;
  ValSet xValSet;
  
  // Before appending request for se, ensure that se is running
  // get the se pid from the attachmap
  i8 iPid = gpAttachMap->m_iPid;
  if (!Process::bTestPid(iPid)) {
    // SE is not running anymore!
    vTrace("Cannot find pid %lld\n", gpAttachMap->m_iPid);
    vSetLastError(EC_SE_NOT_RUNNING);
    return false;
  }
  
  String::wToString((i8)this,wItfAddr);
  String::vCopy(wWorkspace,wItfAddr);
  String::vConcat(wWorkspace," >seitf task.this task.seitf! ");
  xVal = wWorkspace;
  //vTrace("%s\n",xVal.sStr());
  xValSet.vAppend(pCursor,xVal);
  
  xVal = sRequest.a();
  //vTrace("%s\n",xVal.sStr());
  xValSet.vAppend(pCursor,xVal);
  
  // if the current request id matches the next request id, it means
  // we have not read a response of the previous request
  if (m_iCurrentRequestId == m_iNextRequestId) {
    vTrace("Response for previous request not retrieved. Out-Of-Sync request-response.\n");
    vSetLastError(EC_RESPONSE_PENDING);
    return false;
  }
  
  // set the new current request id to the next one sent by previous response
  m_iCurrentRequestId = m_iNextRequestId;
  
  bool b = m_pRequestQueue->bAppend(&xValSet);
  
  return b;
}

/*----------------------------------------------------------------------------*/
/* pop response from response queue (response is from pointer in seitf(shm))  */
/*----------------------------------------------------------------------------*/
eEC SEitf::eGetResponse(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  while (! bPopResponse(m_eErrorCode, m_sResponse))
    ;
  
  vSetLastError(m_eErrorCode);
  
  // generate a random number and send it back
  m_iNextRequestId = Random::iRandom();
  //vTrace("NextRequest %lld\n", m_iNextRequestId);
  
  return m_eErrorCode;
}

/*----------------------------------------------------------------------------*/
/* pop response from response queue                                           */
/*----------------------------------------------------------------------------*/
bool SEitf::bPopResponse(eEC& eResult, s& sResponse)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  bool b;
  ValSet xValSet;
  char* pCursor = nullptr;
  
  if (! m_pResponseQueue->bPop(&xValSet))
    return false;
  
  ValWsp* pValWsp;
  
  b = xValSet.bExtract(pCursor,pValWsp);
  ASSERT(b);
  eResult = (eEC)pValWsp->iInt();
  
  b = xValSet.bExtract(pCursor,pValWsp);
  ASSERT(b);
  sResponse = pValWsp->sStr();
  
  b = xValSet.bExtract(pCursor,pValWsp);
  ASSERT(! b);
  
//  vTrace("sResponse: >>>\n%s\n<<<\n\n",sResponse.a());
//  vTrace("\n---\n\n",sResponse.a());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get last error                                                             */
/*----------------------------------------------------------------------------*/
const char* SEitf::pGetLastError(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  return m_sErrorMsg.a();
}

/*----------------------------------------------------------------------------*/
/* clear last error                                                           */
/*----------------------------------------------------------------------------*/
void SEitf::vClearLastError(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  m_eErrorCode = EC_NONE;
  String::vCopy(m_sErrorMsg.a(), "");
}

/*----------------------------------------------------------------------------*/
/* get last error                                                             */
/*----------------------------------------------------------------------------*/
eEC SEitf::eGetLastError(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
 
  return m_eErrorCode;
}

/*----------------------------------------------------------------------------*/
/* set last error                                                             */
/*----------------------------------------------------------------------------*/
void SEitf::vSetLastError(eEC eError)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  ASSERT(eError >= EC_NONE);
  ASSERT(eError <  EC_ENUMS);
  
  m_eErrorCode = eError;
  String::vCopy(m_sErrorMsg.a(), pEC[m_eErrorCode]);
  
//  if (String::iLength(m_sResponse.a()) != 0) {
//    String::vConcat(m_sInterfaceError.a(), " (");
//    String::vConcat(m_sInterfaceError.a(), m_sResponse.a());
//    String::vConcat(m_sInterfaceError.a(), ") ");
//  }
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SEitf::pFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
