/*============================================================================*/
/* SEitf.cpp - se interface                                                   */
/*============================================================================*/

#include "Interface.h"
#include "StorageEngine.h"

extern Heap*      gpMediumHeap;
extern Dic*       gpMainDic;
extern SEitfList* gpSEitfList;
extern AttachMap* gpAttachMap;
extern Grid*      gpGrid;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SEitf* Interface::_pSEitf(void* aAddr)
{
  SEitf* pSEitf = reinterpret_cast<SEitf*>(aAddr);
  
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

  m_iSlots = gpGrid->iSlots();
  m_iPartition  = iPartition;
  
  //m_xPathStack;
  m_pRequestQueue   = nullptr;
  m_pResponseQueue  = nullptr;
  m_eErrorCode      = EC_NONE;
  m_sErrorMsg       = aEC[m_eErrorCode]->a();
  m_sResponse       = ""_Str;
  m_pRowPtr         = nullptr;
  m_pCurrentTable   = nullptr;
  m_pTxRef          = nullptr;
  m_iCurrentRequestId = 0;
  m_iNextRequestId    = -1;
  m_iRowPartition     = -1;
  
  SEitf* pSEitf = this;
  gpSEitfList->vAppend(pSEitf);
  
  for (i8 iPartition = 0; iPartition < C_SLOTS_PER_NODE; iPartition++)
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
  delete m_pResponseQueue;
  
  gpSEitfList->vRemove(pSEitf);
}

/*----------------------------------------------------------------------------*/
/* get path stack                                                             */
/*----------------------------------------------------------------------------*/
DicPath* SEitf::pPathStack(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);

  return &m_xPathStack;
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
void SEitf::vContentDump(void)
{
  vTrace("SEitf %p: pid %lld (%s)\n"_Str,
         this,
         m_iPid,
         Process::bTestPid(m_iPid) ? "exists"_Str : "non-existant"_Str
         );
  
  vTrace("  RequestQueue %p  ResponseQueue %p\n"_Str,
         m_pRequestQueue,
         m_pResponseQueue);
}

/*----------------------------------------------------------------------------*/
/* dump seitf                                                                 */
/*----------------------------------------------------------------------------*/
void SEitf::vDump(eCN eClass, Object* pObject, Str* aTab)
{
  ASSERT(eClass == CN_SEITF);
  ASSERT(pObject->eClass() == CN_SEITF);
  
  SEitf* pSEitf = reinterpret_cast<SEitf*>(pObject);
  
  vTrace("pid %lld %s partitions %lld partition %lld"_Str
         " requestQ %p responseQ %p response '%s'\n"_Str,
         pSEitf->m_iPid,
         Process::bTestPid(pSEitf->m_iPid) ? "exists"_Str : "non-existant"_Str,
         pSEitf->m_iSlots,
         pSEitf->m_iPartition,
         pSEitf->m_pRequestQueue,
         pSEitf->m_pResponseQueue,
         pSEitf->m_sResponse.aStr());
}

/*----------------------------------------------------------------------------*/
/* initialise this seitf                                                      */
/*----------------------------------------------------------------------------*/
void SEitf::vInit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  // request queue
  Word* pReqQueWord = gpMainDic->pFindWord("requestQueue"_Str);
  ASSERT(pReqQueWord);
  m_pRequestQueue = reinterpret_cast<Queue*>(pReqQueWord->aBodyArea());
  
  // response queue
  m_pResponseQueue = new Queue;
  
  // set up initial default path
  
  // /tenants
  Word* pTenants = gpMainDic->pFindWord("tenants"_Str);
  ASSERT(pTenants);
  Cell* pCell = pTenants->pFirstCell();
  m_xPathStack.vPush(reinterpret_cast<Dic*>(pCell));
  
  // /tenants/boray
  Word* pTenant = m_xPathStack.pFindInCurrent("boray");
  ASSERT(pTenant);
  pCell = pTenant->pFirstCell();
  m_xPathStack.vPush(reinterpret_cast<Dic*>(pCell));
  
  // /tenants/boray/catalogs
  Word* pCatalogs = m_xPathStack.pFindInCurrent("catalogs");
  ASSERT(pCatalogs);
  pCell = pCatalogs->pFirstCell();
  m_xPathStack.vPush(reinterpret_cast<Dic*>(pCell));
 
  // /tenants/boray/catalogs/rapidsse
  Word* pCatalog = m_xPathStack.pFindInCurrent("rapidsse");
  ASSERT(pCatalog);
  pCell = pCatalog->pFirstCell();
  m_xPathStack.vPush(reinterpret_cast<Dic*>(pCell));
  
  // /tenants/boray/catalogs/rapidsse/schemas
  Word* pSchema = m_xPathStack.pFindInCurrent("schemas");
  ASSERT(pSchema);
  pCell = pSchema->pFirstCell();
  m_xPathStack.vPush(reinterpret_cast<Dic*>(pCell));
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
  
  //vTrace("txref: %x\n"_Str, m_pTxRef);
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
bool SEitf::bDecode(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  if (0 == aWords[0]->iLength())
    return false;
  
  if (aWords[0]->bSame("antbl"_Str)) return bAnalyzeTable(aWords, iWords);
  if (aWords[0]->bSame("add"_Str)) return bAddColumn(aWords, iWords);
  if (aWords[0]->bSame("cf"_Str)) return bPushDic(aWords);
  if (aWords[0]->bSame("con"_Str)) return bConnect(aWords);
  if (aWords[0]->bSame("connectCluster"_Str)) return bConnectCluster(aWords);
  if (aWords[0]->bSame("cp"_Str)) return bCopyTable(aWords, iWords);
  if (aWords[0]->bSame("dantbl"_Str)) return bDanalyzeTable(aWords, iWords);
  if (aWords[0]->bSame("dex"_Str)) return bDistExportTable(aWords, iWords);
  if (aWords[0]->bSame("dis"_Str)) return bDisconnect(aWords);
  if (aWords[0]->bSame("docol"_Str)) return bDocol(aWords);
  if (aWords[0]->bSame("dump"_Str)) return bDumpTable(aWords, iWords);
  if (aWords[0]->bSame("ex"_Str)) return bExportTable(aWords, iWords);
  if (aWords[0]->bSame("getFreeSpace"_Str)) return bGetFreeSpace(aWords);
  if (aWords[0]->bSame("getInUseSpace"_Str)) return bGetInUseSpace(aWords);
  if (aWords[0]->bSame("gettbl"_Str)) return bGetTable(aWords, iWords);
  if (aWords[0]->bSame("getTotalSpace"_Str)) return bGetTotalSpace(aWords);
  if (aWords[0]->bSame("idump"_Str)) return bDumpIndex(aWords, iWords);
  if (aWords[0]->bSame("TblInUse"_Str)) return bInUseTable(aWords, iWords);
  if (aWords[0]->bSame("ld"_Str)) return bLoadTable(aWords, iWords);
  if (aWords[0]->bSame("lds"_Str)) return bLoadTableStream(aWords, iWords);
  if (aWords[0]->bSame("lf"_Str)) return bGetDicContents(aWords);
  if (aWords[0]->bSame("lfG"_Str)) return bGetDicContentsG(aWords);
  if (aWords[0]->bSame("lscat"_Str)) return bListCatalogs(aWords, iWords);
  if (aWords[0]->bSame("lscol"_Str)) return bListColumns(aWords, iWords);
  if (aWords[0]->bSame("lsidx"_Str)) return bListIndices(aWords, iWords);
  if (aWords[0]->bSame("lssch"_Str)) return bListSchemas(aWords, iWords);
  if (aWords[0]->bSame("lstbl"_Str)) return bListTables(aWords, iWords);
  if (aWords[0]->bSame("lsten"_Str)) return bListTenants(aWords, iWords);
  if (aWords[0]->bSame("mkcat"_Str)) return bCreateCatalog(aWords, iWords);
  if (aWords[0]->bSame("mkidx"_Str)) return bCreateIndex(aWords, iWords);
  if (aWords[0]->bSame("mksch"_Str)) return bCreateSchema(aWords, iWords);
  if (aWords[0]->bSame("mktbl"_Str)) return bCreateTable(aWords, iWords);
  if (aWords[0]->bSame("mkten"_Str)) return bCreateTenant(aWords, iWords);
  if (aWords[0]->bSame("object"_Str)) return bObject(aWords);
  if (aWords[0]->bSame("pRB"_Str)) return bPrintRowBuffer(aWords);
  if (aWords[0]->bSame("pwf"_Str)) return bGetDicPath(aWords);
  if (aWords[0]->bSame("rettbl"_Str)) return bRetTable(aWords, iWords);
  if (aWords[0]->bSame("rf"_Str)) return bRemoveFolder(aWords, iWords);
  if (aWords[0]->bSame("rmcat"_Str)) return bDropCatalog(aWords, iWords);
  if (aWords[0]->bSame("rmdic"_Str)) return bRemoveFolder(aWords, iWords);
  if (aWords[0]->bSame("rmidx"_Str)) return bDropIndex(aWords, iWords);
  if (aWords[0]->bSame("rmsch"_Str)) return bDropSchema(aWords, iWords);
  if (aWords[0]->bSame("rmtbl"_Str)) return bDropTable(aWords, iWords);
  if (aWords[0]->bSame("rmten"_Str)) return bDropTenant(aWords, iWords);
  if (aWords[0]->bSame("sel"_Str)) return bSelectTable(aWords, iWords);
  if (aWords[0]->bSame("space"_Str)) return bPoolSpace(aWords, iWords);
  if (aWords[0]->bSame("status"_Str)) return bNodeStatus(aWords, iWords);
  if (aWords[0]->bSame("sz"_Str)) return bSizeTable(aWords, iWords);
  if (aWords[0]->bSame("tr"_Str)) return bTruncateTable(aWords, iWords);
  if (aWords[0]->bSame("usesch"_Str)) return bUseSchema(aWords, iWords);
  if (aWords[0]->bSame("__x"_Str)) return bX(aWords);
  if (aWords[0]->bSame("ping"_Str)) return bPing(aWords);
  if (aWords[0]->bSame("mon"_Str)) return bGetMonitor(aWords);
  if (aWords[0]->bSame("print"_Str)) return ! bPrint(aWords);
  if (aWords[0]->bSame("seitf"_Str)) return bSeitfCount(aWords);
  if (aWords[0]->bSame("version"_Str)) return bGetVersion(aWords);
  
  vSetLastError(EC_INVALID_COMMAND);
  return false;
}

/*----------------------------------------------------------------------------*/
/* connect to se                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bConnect(Str* aWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(aWords[0]->bEqual("con"_Str));
  sRequest = "connect"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* connect the se cluster nodes                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bConnectCluster(Str* aWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(aWords[0]->bEqual("connectCluster"_Str));
  sRequest = "connectCluster"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* disconnect from se                                                         */
/*----------------------------------------------------------------------------*/
bool SEitf::bDisconnect(Str* aWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(aWords[0]->bEqual("dic"_Str));
  sRequest = "disconnect"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* remove a folder                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bRemoveFolder(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.aStr();
  
  StrLib::vCopy(a,"\"");
  StrLib::vConcat(a,aWords[1]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," remove_folder");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* create a tenant                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bCreateTenant(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.aStr();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }

  StrLib::vCopy(a,"\"");
  StrLib::vConcat(a,aWords[1]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," create_tenant");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* list tenants                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bListTenants(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (iWords != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }

  ASSERT(aWords[0]->bEqual("lsten"_Str));
  sRequest = "list_tenants"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* drop a tenant                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bDropTenant(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.aStr();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  StrLib::vCopy(a,"\"");
  StrLib::vConcat(a,aWords[1]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," drop_tenant");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* create a catalog                                                           */
/*----------------------------------------------------------------------------*/
bool SEitf::bCreateCatalog(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.aStr();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  StrLib::vCopy(a,"\"");
  StrLib::vConcat(a,aWords[1]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," create_catalog");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* list catalogs                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bListCatalogs(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (iWords != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  ASSERT(aWords[0]->bEqual("lscat"_Str));
  sRequest = "list_catalogs"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* drop a catalog                                                             */
/*----------------------------------------------------------------------------*/
bool SEitf::bDropCatalog(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.aStr();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }

  StrLib::vCopy(a,"\"");
  StrLib::vConcat(a,aWords[1]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," drop_catalog");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* create a schema                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bCreateSchema(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.aStr();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  StrLib::vCopy(a,"\"");
  StrLib::vConcat(a,aWords[1]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," create_schema");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* list schemas                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bListSchemas(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (iWords != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  ASSERT(aWords[0]->bEqual("lssch"_Str));
  sRequest = "list_schemas"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* drop a schema                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bDropSchema(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.aStr();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }

  StrLib::vCopy(a,"\"");
  StrLib::vConcat(a,aWords[1]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," drop_schema");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* use a schema                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bUseSchema(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.aStr();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  StrLib::vCopy(a,"\"");
  StrLib::vConcat(a,aWords[1]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," use_schema");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* create an index                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bCreateIndex(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.aStr();
  
  if (iWords != 4) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  StrLib::vCopy(a,"\"");
  StrLib::vConcat(a,aWords[3]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," \"");
  StrLib::vConcat(a,aWords[2]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," \"");
  StrLib::vConcat(a,aWords[1]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," create_index");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* list indices                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bListIndices(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (iWords != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  ASSERT(aWords[0]->bEqual("lsidx"_Str));
  sRequest = "list_indexes"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* create a table                                                             */
/*----------------------------------------------------------------------------*/
bool SEitf::bCreateTable(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.aStr();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  StrLib::vCopy(a,"\"");
  StrLib::vConcat(a,aWords[1]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," create_table");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* list tables                                                                */
/*----------------------------------------------------------------------------*/
bool SEitf::bListTables(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (iWords != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  ASSERT(aWords[0]->bEqual("lstbl"_Str));
  sRequest = "list_tables"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* dump an index                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bDumpIndex(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.aStr();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  StrLib::vCopy(a,"\"");
  StrLib::vConcat(a,aWords[1]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," dump_index");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* dump a table                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bDumpTable(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.aStr();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  StrLib::vCopy(a,"\"");
  StrLib::vConcat(a,aWords[1]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," dump_table");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* docol                                                                      */
/*----------------------------------------------------------------------------*/
bool SEitf::bDocol(Str* aWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.aStr();
  
  StrLib::vCopy(a,"\"");
  StrLib::vConcat(a,aWords[1]->a());
  StrLib::vConcat(a,"\"");
  
  //
  StrLib::vConcat(a," docol_command");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* drop an index                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bDropIndex(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.aStr();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  StrLib::vCopy(a,"\"");
  StrLib::vConcat(a,aWords[1]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," drop_index");
  
  return bAppendRequest(sRequest);
}


/*----------------------------------------------------------------------------*/
/* drop a table                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bDropTable(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.aStr();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  StrLib::vCopy(a,"\"");
  StrLib::vConcat(a,aWords[1]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," drop_table");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* add a column                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bAddColumn(Str* aWords[16], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.aStr();
  
  if (iWords != 4) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  StrLib::vCopy(a,"\"");
  StrLib::vConcat(a,aWords[3]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," \"");
  StrLib::vConcat(a,aWords[2]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," \"");
  StrLib::vConcat(a,aWords[1]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," add_column");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* list columns                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bListColumns(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.aStr();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  StrLib::vCopy(a,"\"");
  StrLib::vConcat(a,aWords[1]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," list_columns");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* analyze a table                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bAnalyzeTable(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.aStr();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  StrLib::vCopy(a,"\"");
  StrLib::vConcat(a,aWords[1]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," analyze_table");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* analyze a table                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bDanalyzeTable(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.aStr();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  StrLib::vCopy(a,"\"");
  StrLib::vConcat(a,aWords[1]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," danalyze_table");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* copy a table                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bCopyTable(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.aStr();
  
  if (iWords != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  StrLib::vCopy(a,"\"");
  StrLib::vConcat(a,aWords[2]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," \"");
  StrLib::vConcat(a,aWords[1]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," copy_table");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* distributed export of a table                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bDistExportTable(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char*  a = sRequest.aStr();
  
  if (iWords != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  StrLib::vCopy(a,"\"");
  StrLib::vConcat(a,aWords[2]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," \"");
  StrLib::vConcat(a,aWords[1]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," dist_export_table");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* export a table                                                             */
/*----------------------------------------------------------------------------*/
bool SEitf::bExportTable(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;

  char*  a = sRequest.aStr();
  
  if (iWords != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  StrLib::vCopy(a,"\"");
  StrLib::vConcat(a,aWords[2]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," \"");
  StrLib::vConcat(a,aWords[1]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," export_table");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* load a table from file                                                     */
/*----------------------------------------------------------------------------*/
bool SEitf::bLoadTable(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.aStr();
  
  if (iWords != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  StrLib::vCopy(a,"\"");
  StrLib::vConcat(a,aWords[2]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," \"");
  StrLib::vConcat(a,aWords[1]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," load_table");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* load a table from stream                                                   */
/*----------------------------------------------------------------------------*/
bool SEitf::bLoadTableStream(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.aStr();
  
  if (iWords != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  StrLib::vCopy(a,"\"");
  StrLib::vConcat(a,aWords[2]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," \"");
  StrLib::vConcat(a,aWords[1]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," load_table_stream");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* select rows from a table                                                   */
/*----------------------------------------------------------------------------*/
bool SEitf::bSelectTable(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.aStr();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  StrLib::vCopy(a,"\"");
  StrLib::vConcat(a,aWords[1]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," select_table");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get address/reference of a table                                           */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetTable(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.aStr();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  StrLib::vCopy(a,"\"");
  StrLib::vConcat(a,aWords[1]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," get_table");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* return whether table is in use or not                                      */
/*----------------------------------------------------------------------------*/
bool SEitf::bInUseTable(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.aStr();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  StrLib::vCopy(a,"\"");
  StrLib::vConcat(a,aWords[1]->a());
  StrLib::vConcat(a,"\"");

  StrLib::vConcat(a," table_inuse");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* return address/refernce of a table                                         */
/*----------------------------------------------------------------------------*/
bool SEitf::bRetTable(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.aStr();
  
  if (iWords != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  StrLib::vConcat(a," return_table");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get pool space                                                             */
/*----------------------------------------------------------------------------*/
bool SEitf::bPoolSpace(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.aStr();
  
  if (iWords != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  StrLib::vConcat(a," space");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get node status                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bNodeStatus(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.aStr();
  
  if (iWords != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  StrLib::vConcat(a," status");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* size of table                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bSizeTable(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.aStr();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  StrLib::vCopy(a,"\"");
  StrLib::vConcat(a,aWords[1]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," size_table");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* truncate a table                                                           */
/*----------------------------------------------------------------------------*/
bool SEitf::bTruncateTable(Str* aWords[], i8 iWords)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.aStr();
  
  if (iWords != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }

  StrLib::vCopy(a,"\"");
  StrLib::vConcat(a,aWords[1]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," truncate_table");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* insert row into row buffer                                                 */
/*----------------------------------------------------------------------------*/
bool SEitf::bInsertRow(char* aRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
                              
  // check for working buffer
  if (! m_pRowBuffer[m_iPartition]) {
    m_pRowBuffer[m_iPartition] = RowBuffer::pNewRowBuffer();
    if (! m_pRowBuffer[m_iPartition]) {
      vTrace("RowBuffer allocation failed\n"_Str);
      m_eErrorCode = EC_LOW_BUFFERS;
      return false;
    }
  }
  
  // get working buffer
  RowBuffer* pRowBuffer = m_pRowBuffer[m_iPartition];
  
  vTrace("Buffer: %s Partition:%d\n"_Str,pRowBuffer, m_iPartition);
  
  // insert row into working buffer
  if (pRowBuffer->bInsertRow(m_pTxRef,aRow,StrLib::iSize(aRow)))
    return true;
    
  // buffer full, append working buffer into table
  //m_pCurrentTable->vAppendBuffer(m_iPartition,pRowBuffer);
  
  vTrace("Buffer full... Inserting buffer\n"_Str);
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
    vTrace("RowBuffer allocation failed\n"_Str);
    m_eErrorCode = EC_LOW_BUFFERS;
    return false;
  }
  
  pRowBuffer = m_pRowBuffer[m_iPartition];
  
  // insert row into new working buffer
  bool bInserted = pRowBuffer->bInsertRow(m_pTxRef,aRow,StrLib::iSize(aRow));
  
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
  
  i8 iSlot = Partition::iPartitionToSlot(iPartition);
                              
  char xArray[C_1KB * 2];
  char* aWords[16] = {
    &xArray[C_1KB * 0],
    &xArray[C_1KB * 1]
  };

  s256 sRequest;
  s256 sBuffer;

  char* a = sRequest.aStr();
  
  StrLib::wToString(iSlot, sBuffer.aStr());
  StrLib::vCopy(a,"\"");
  StrLib::vConcat(a,sBuffer.aStr());
  StrLib::vConcat(a,"\"");
  
  StrLib::wToString((i8)pRowBuffer, sBuffer.aStr());
  StrLib::vConcat(a," \"");
  StrLib::vConcat(a,sBuffer.aStr());
  StrLib::vConcat(a,"\"");
  
  StrLib::wToString((i8)m_pCurrentTable, sBuffer.aStr());
  StrLib::vConcat(a," \"");
  StrLib::vConcat(a,sBuffer.aStr());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," append_table");
  
  vTrace("%s\n"_Str,a);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* push dictionary                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bPushDic(Str* aWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  char* a = sRequest.aStr();
  
  StrLib::vCopy(a,"\"");
  StrLib::vConcat(a,aWords[1]->a());
  StrLib::vConcat(a,"\"");
  
  StrLib::vConcat(a," cf");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
bool SEitf::bPrint(Str* aWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  printf("%s\n",aWords[1]->a());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get dictionary path                                                        */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetDicPath(Str* aWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(aWords[0]->bEqual("pwf"_Str));
  sRequest = "pwf"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get dictionary contents                                                    */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetDicContents(Str* aWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(aWords[0]->bEqual("lf"_Str));
  sRequest = "lf"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get dictionary contents                                                    */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetDicContentsG(Str* aWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(aWords[0]->bEqual("lfG"_Str));
  sRequest = "lfG"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get monitor string                                                         */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetMonitor(Str* aWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(aWords[0]->bEqual("mon"_Str));
  sRequest = "get_monitor"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get free space                                                             */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetFreeSpace(Str* aWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(aWords[0]->bEqual("getFreeSpace"_Str));
  sRequest = "getFreeSpace"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get inUse space                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetInUseSpace(Str* aWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(aWords[0]->bEqual("getInUseSpace"_Str));
  sRequest = "getInUseSpace"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get total space                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetTotalSpace(Str* aWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(aWords[0]->bEqual("getTotalSpace"_Str));
  sRequest = "getTotalSpace"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get version                                                                */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetVersion(Str* aWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(aWords[0]->bEqual("version"_Str));
  sRequest = "get_version"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* ping test                                                                  */
/*----------------------------------------------------------------------------*/
bool SEitf::bPing(Str* aWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(aWords[0]->bEqual("ping"_Str));
  sRequest = "ping"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* send object                                                                */
/*----------------------------------------------------------------------------*/
bool SEitf::bObject(Str* aWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  char* a = sRequest.aStr();
  
  StrLib::vConcat(a,"object");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* print row buffer                                                           */
/*----------------------------------------------------------------------------*/
bool SEitf::bPrintRowBuffer(Str* aWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  char* a = sRequest.aStr();
  
  StrLib::vCopy(a,"\"");
  StrLib::vConcat(a,aWords[1]->a());
  StrLib::vConcat(a,"\"");

  ASSERT(aWords[0]->bEqual("pRB"_Str));
  StrLib::vConcat(a," pRB");
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get seitf count                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bSeitfCount(Str* aWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(aWords[0]->bEqual("seitf"_Str));
  sRequest = "seitf_count"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* x test                                                                     */
/*----------------------------------------------------------------------------*/
bool SEitf::bX(Str* aWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(aWords[0]->bEqual("ping"_Str));
  sRequest = "ping"_Str;
  
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
bool SEitf::bAppendRequest(StrVal& sRequest)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  char wItfAddr[32];
  char wWorkspace[C_1KB * 16];
  char* aCursor = nullptr;
  
  ValWsp xVal;
  ValSet xValSet;
  
  // Before appending request for se, ensure that se is running
  // get the se pid from the attachmap
  i8 iPid = gpAttachMap->m_iPid;
  if (!Process::bTestPid(iPid)) {
    // SE is not running anymore!
    vTrace("Cannot find pid %lld\n"_Str, gpAttachMap->m_iPid);
    vSetLastError(EC_SE_NOT_RUNNING);
    return false;
  }
  
  StrLib::wToString((i8)this,wItfAddr);
  StrLib::vCopy(wWorkspace,wItfAddr);
  StrLib::vConcat(wWorkspace," >seitf task.this task.seitf! ");
  xVal = wWorkspace;
  //vTrace("%s\n"_Str,xVal.aStr());
  xValSet.vAppend(aCursor,xVal);
  
  xVal = sRequest.aStr();
  //vTrace("%s\n"_Str,xVal.aStr());
  xValSet.vAppend(aCursor,xVal);
  
  // if the current request id matches the next request id, it means
  // we have not read a response of the previous request
  if (m_iCurrentRequestId == m_iNextRequestId) {
    vTrace("Response for previous request not retrieved. Out-Of-Sync request-response.\n"_Str);
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
  //vTrace("NextRequest %lld\n"_Str, m_iNextRequestId);
  
  return m_eErrorCode;
}

/*----------------------------------------------------------------------------*/
/* pop response from response queue                                           */
/*----------------------------------------------------------------------------*/
bool SEitf::bPopResponse(eEC& eResult, StrVal& sResponse)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  bool b;
  ValSet xValSet;
  char* aCursor = nullptr;
  
  if (! m_pResponseQueue->bPop(&xValSet))
    return false;
  
  ValWsp* pValWsp;
  
  b = xValSet.bExtract(aCursor,pValWsp);
  ASSERT(b);
  eResult = (eEC)_pIntVal(pValWsp)->iInt();
  
  b = xValSet.bExtract(aCursor,pValWsp);
  ASSERT(b);
  *_pStrVal(&sResponse) = _pStrVal(pValWsp)->aStr();
  
  b = xValSet.bExtract(aCursor,pValWsp);
  ASSERT(! b);
  
//  vTrace("sResponse: >>>\n%s\n<<<\n\n"_Str,sResponse.aStr());
//  vTrace("\n---\n\n"_Str,sResponse.aStr());
  
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
  
  return m_sErrorMsg.aStr();
}

/*----------------------------------------------------------------------------*/
/* clear last error                                                           */
/*----------------------------------------------------------------------------*/
void SEitf::vClearLastError(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  m_eErrorCode = EC_NONE;
  StrLib::vCopy(m_sErrorMsg.aStr(), "");
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
  StrLib::vCopy(m_sErrorMsg.aStr(), aEC[m_eErrorCode]->a());
  
//  if (StrLib::iLength(m_sResponse.aStr()) != 0) {
//    StrLib::vConcat(m_sInterfaceError.aStr(), " (");
//    StrLib::vConcat(m_sInterfaceError.aStr(), m_sResponse.aStr());
//    StrLib::vConcat(m_sInterfaceError.aStr(), ") ");
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
