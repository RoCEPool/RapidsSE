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
  m_sErrorMsg       = aEC[m_eErrorCode];
  m_sResponse       = ""_Str;
  m_aRowPtr         = nullptr;
  m_pIndexBuffer    = nullptr;
  m_pEntry          = nullptr;
  m_pCurrentTable   = nullptr;
  m_pTxRef          = nullptr;
  m_sMetadata       = ""_Str;
  m_bDisplay        = true;
  m_iRowsProcessed    = 0;
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
         pSEitf->m_sResponse.s());
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
  Word* pTenant = m_xPathStack.pFindInCurrent("boray"_Str);
  ASSERT(pTenant);
  pCell = pTenant->pFirstCell();
  m_xPathStack.vPush(reinterpret_cast<Dic*>(pCell));
  
  // /tenants/boray/catalogs
  Word* pCatalogs = m_xPathStack.pFindInCurrent("catalogs"_Str);
  ASSERT(pCatalogs);
  pCell = pCatalogs->pFirstCell();
  m_xPathStack.vPush(reinterpret_cast<Dic*>(pCell));
 
  // /tenants/boray/catalogs/rapidsse
  Word* pCatalog = m_xPathStack.pFindInCurrent("rapidsse"_Str);
  ASSERT(pCatalog);
  pCell = pCatalog->pFirstCell();
  m_xPathStack.vPush(reinterpret_cast<Dic*>(pCell));
  
  // /tenants/boray/catalogs/rapidsse/schemas
  Word* pSchemas = m_xPathStack.pFindInCurrent("schemas"_Str);
  ASSERT(pSchemas);
  pCell = pSchemas->pFirstCell();
  m_xPathStack.vPush(reinterpret_cast<Dic*>(pCell));
  
  // /tenants/boray/catalogs/rapidsse/schemas/public
  Word* pSchema = m_xPathStack.pFindInCurrent("public"_Str);
  ASSERT(pSchema);
  pCell = pSchema->pFirstCell();
  m_xPathStack.vPush(reinterpret_cast<Dic*>(pCell));

  // /tenants/boray/catalogs/rapidsse/schemas/public/tables
  Word* pTables = m_xPathStack.pFindInCurrent("tables"_Str);
  ASSERT(pTables);
  
  pCell = pTables->pFirstCell();
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
bool SEitf::bDecode(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  if (0 == xTokenSet[0]->iLength())
    return false;
  
  if (xTokenSet[0]->bSame("addcol"_Str)) return bAddColumn(xTokenSet);
  if (xTokenSet[0]->bSame("antbl"_Str)) return bAnalyzeTable(xTokenSet);
  if (xTokenSet[0]->bSame("anidx"_Str)) return bAnalyzeIndex(xTokenSet);
  if (xTokenSet[0]->bSame("addidxcol"_Str)) return bAddIndexColumn(xTokenSet);
  if (xTokenSet[0]->bSame("addpartcol"_Str)) return bAddPartitionColumn(xTokenSet);
  if (xTokenSet[0]->bSame("cf"_Str)) return bPushDic(xTokenSet);
  if (xTokenSet[0]->bSame("connectCluster"_Str)) return bConnectCluster(xTokenSet);
  if (xTokenSet[0]->bSame("cp"_Str)) return bCopyTable(xTokenSet);
  if (xTokenSet[0]->bSame("danidx"_Str)) return bDanalyzeIndex(xTokenSet);
  if (xTokenSet[0]->bSame("dantbl"_Str)) return bDanalyzeTable(xTokenSet);
  if (xTokenSet[0]->bSame("dex"_Str)) return bDistExportTable(xTokenSet);
  if (xTokenSet[0]->bSame("docol"_Str)) return bDocol(xTokenSet);
  if (xTokenSet[0]->bSame("displayOff"_Str)) return bDisplayOff(xTokenSet);
  if (xTokenSet[0]->bSame("displayOn"_Str)) return bDisplayOn(xTokenSet);
  if (xTokenSet[0]->bSame("dump"_Str)) return bDumpTable(xTokenSet);
  if (xTokenSet[0]->bSame("endmkidx"_Str)) return bEndCreateIndex(xTokenSet);
  if (xTokenSet[0]->bSame("endmktbl"_Str)) return bEndCreateTable(xTokenSet);
  if (xTokenSet[0]->bSame("ex"_Str)) return bExportTable(xTokenSet);
  if (xTokenSet[0]->bSame("get"_Str)) return bGet(xTokenSet);
  if (xTokenSet[0]->bSame("getFreeSpace"_Str)) return bGetFreeSpace(xTokenSet);
  if (xTokenSet[0]->bSame("getInUseSpace"_Str)) return bGetInUseSpace(xTokenSet);
  if (xTokenSet[0]->bSame("gettbl"_Str)) return bGetTable(xTokenSet);
  if (xTokenSet[0]->bSame("gettsk"_Str)) return bGetTask(xTokenSet);
  if (xTokenSet[0]->bSame("getTotalSpace"_Str)) return bGetTotalSpace(xTokenSet);
  if (xTokenSet[0]->bSame("dumpidx"_Str)) return bDumpIndex(xTokenSet);
  if (xTokenSet[0]->bSame("TblInUse"_Str)) return bInUseTable(xTokenSet);
  if (xTokenSet[0]->bSame("ld"_Str)) return bLoadTable(xTokenSet);
  if (xTokenSet[0]->bSame("lds"_Str)) return bLoadTableStream(xTokenSet);
  if (xTokenSet[0]->bSame("lf"_Str)) return bGetDicContents(xTokenSet);
  if (xTokenSet[0]->bSame("lfG"_Str)) return bGetDicContentsG(xTokenSet);
  if (xTokenSet[0]->bSame("keyselidx"_Str)) return bKeySelectIndex(xTokenSet);
  if (xTokenSet[0]->bSame("lscat"_Str)) return bListCatalogs(xTokenSet);
  if (xTokenSet[0]->bSame("lscol"_Str)) return bListColumns(xTokenSet);
  if (xTokenSet[0]->bSame("lsidx"_Str)) return bListIndices(xTokenSet);
  if (xTokenSet[0]->bSame("lsidxcol"_Str)) return bListIndexColumns(xTokenSet);
  if (xTokenSet[0]->bSame("lspartcol"_Str)) return bListPartitionColumns(xTokenSet);
  if (xTokenSet[0]->bSame("lssch"_Str)) return bListSchemas(xTokenSet);
  if (xTokenSet[0]->bSame("lstbl"_Str)) return bListTables(xTokenSet);
  if (xTokenSet[0]->bSame("lsten"_Str)) return bListTenants(xTokenSet);
  if (xTokenSet[0]->bSame("lwrsel"_Str)) return bLowerSelectTable(xTokenSet);
  if (xTokenSet[0]->bSame("lwrselidx"_Str)) return bLowerSelectIndex(xTokenSet);
  if (xTokenSet[0]->bSame("mkcat"_Str)) return bCreateCatalog(xTokenSet);
  if (xTokenSet[0]->bSame("mkidx"_Str)) return bCreateIndex(xTokenSet);
  if (xTokenSet[0]->bSame("mksch"_Str)) return bCreateSchema(xTokenSet);
  if (xTokenSet[0]->bSame("mktbl"_Str)) return bCreateTable(xTokenSet);
  if (xTokenSet[0]->bSame("mkten"_Str)) return bCreateTenant(xTokenSet);
  if (xTokenSet[0]->bSame("object"_Str)) return bObject(xTokenSet);
  if (xTokenSet[0]->bSame("pRB"_Str)) return bPrintRowBuffer(xTokenSet);
  if (xTokenSet[0]->bSame("pwf"_Str)) return bGetDicPath(xTokenSet);
  if (xTokenSet[0]->bSame("rettbl"_Str)) return bRetTable(xTokenSet);
  if (xTokenSet[0]->bSame("rettsk"_Str)) return bRetTask(xTokenSet);
  if (xTokenSet[0]->bSame("rf"_Str)) return bRemoveFolder(xTokenSet);
  if (xTokenSet[0]->bSame("rgesel"_Str)) return bRangeSelectTable(xTokenSet);
  if (xTokenSet[0]->bSame("rmcat"_Str)) return bDropCatalog(xTokenSet);
  if (xTokenSet[0]->bSame("rmdic"_Str)) return bRemoveFolder(xTokenSet);
  if (xTokenSet[0]->bSame("rmidx"_Str)) return bDropIndex(xTokenSet);
  if (xTokenSet[0]->bSame("rmsch"_Str)) return bDropSchema(xTokenSet);
  if (xTokenSet[0]->bSame("rmtbl"_Str)) return bDropTable(xTokenSet);
  if (xTokenSet[0]->bSame("rmten"_Str)) return bDropTenant(xTokenSet);
  if (xTokenSet[0]->bSame("rgeselidx"_Str)) return bRangeSelectIndex(xTokenSet);
  if (xTokenSet[0]->bSame("sel"_Str)) return bSelectTable(xTokenSet);
  if (xTokenSet[0]->bSame("set"_Str)) return bSet(xTokenSet);
  if (xTokenSet[0]->bSame("setidxunique"_Str)) return bIndexSetUnique(xTokenSet);
  if (xTokenSet[0]->bSame("space"_Str)) return bPoolSpace(xTokenSet);
  if (xTokenSet[0]->bSame("status"_Str)) return bNodeStatus(xTokenSet);
  if (xTokenSet[0]->bSame("szidx"_Str)) return bSizeIndex(xTokenSet);
  if (xTokenSet[0]->bSame("sz"_Str)) return bSizeTable(xTokenSet);
  if (xTokenSet[0]->bSame("tableSizes"_Str)) return bSizeTables(xTokenSet);
  if (xTokenSet[0]->bSame("tr"_Str)) return bTruncateTable(xTokenSet);
  if (xTokenSet[0]->bSame("tridx"_Str)) return bTruncateIndex(xTokenSet);
  if (xTokenSet[0]->bSame("uprselidx"_Str)) return bUpperSelectIndex(xTokenSet);
  if (xTokenSet[0]->bSame("usesch"_Str)) return bUseSchema(xTokenSet);
  if (xTokenSet[0]->bSame("__x"_Str)) return bX(xTokenSet);
  if (xTokenSet[0]->bSame("ping"_Str)) return bPing(xTokenSet);
  if (xTokenSet[0]->bSame("mon"_Str)) return bGetMonitor(xTokenSet);
  if (xTokenSet[0]->bSame("print"_Str)) return ! bPrint(xTokenSet);
  if (xTokenSet[0]->bSame("seitf"_Str)) return bSeitfCount(xTokenSet);
  if (xTokenSet[0]->bSame("version"_Str)) return bGetVersion(xTokenSet);
  if (xTokenSet[0]->bSame("visidx"_Str)) return bVisualizeIndex(xTokenSet);
  
  vSetLastError(EC_INVALID_COMMAND);
  return false;
}

/*----------------------------------------------------------------------------*/
/* connect to se                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bConnect(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(xTokenSet[0]->bEqual("con"_Str));
  sRequest = "connect"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* connect the se cluster nodes                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bConnectCluster(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(xTokenSet[0]->bEqual("connectCluster"_Str));
  sRequest = "connectCluster"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* disconnect from se                                                         */
/*----------------------------------------------------------------------------*/
bool SEitf::bDisconnect(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(xTokenSet[0]->bEqual("dic"_Str));
  sRequest = "disconnect"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get value from attachmap                                                   */
/*----------------------------------------------------------------------------*/
bool SEitf::bGet(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" get"_Str);

  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* set value in attachmap                                                     */
/*----------------------------------------------------------------------------*/
bool SEitf::bSet(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat("\""_Str);
  sRequest.vConcat(xTokenSet[2]);
  sRequest.vConcat("\""_Str);

  sRequest.vConcat(" set"_Str);
  
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* remove a folder                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bRemoveFolder(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" remove_folder"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* create a tenant                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bCreateTenant(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }

  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" create_tenant"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* list tenants                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bListTenants(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }

  ASSERT(xTokenSet[0]->bEqual("lsten"_Str));
  sRequest = "list_tenants"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* drop a tenant                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bDropTenant(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" drop_tenant"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* create a catalog                                                           */
/*----------------------------------------------------------------------------*/
bool SEitf::bCreateCatalog(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" create_catalog"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* list catalogs                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bListCatalogs(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
    
  if (xTokenSet.nTokens() != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  ASSERT(xTokenSet[0]->bEqual("lscat"_Str));
  sRequest = "list_catalogs"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* drop a catalog                                                             */
/*----------------------------------------------------------------------------*/
bool SEitf::bDropCatalog(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }

  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" drop_catalog"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* create a schema                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bCreateSchema(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" create_schema"_Str);
  
  return bAppendRequest(sRequest);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* list schemas                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bListSchemas(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  ASSERT(xTokenSet[0]->bEqual("lssch"_Str));
  sRequest = "list_schemas"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* drop a schema                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bDropSchema(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }

  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" drop_schema"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* use a schema                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bUseSchema(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" use_schema"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* create an index                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bCreateIndex(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vConcat(" \""_Str);
  sRequest.vConcat(xTokenSet[2]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" \""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" create_index"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* list indices                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bListIndices(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);

  ASSERT(xTokenSet[0]->bEqual("lsidx"_Str));
  
  sRequest.vConcat(" list_indexes"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* set index uniqueness                                                       */
/*----------------------------------------------------------------------------*/
bool SEitf::bIndexSetUnique(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 4) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  //sRequest.vCopy("\""_Str);
  sRequest.vCopy(xTokenSet[3]);
  //sRequest.vConcat("\""_Str);
  
  sRequest.vConcat("\""_Str);
  sRequest.vConcat(xTokenSet[2]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  ASSERT(xTokenSet[0]->bEqual("setidxunique"_Str));
  
  sRequest.vConcat(" index_setUnique"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* create a table                                                             */
/*----------------------------------------------------------------------------*/
bool SEitf::bCreateTable(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" create_table"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* list tables                                                                */
/*----------------------------------------------------------------------------*/
bool SEitf::bListTables(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  ASSERT(xTokenSet[0]->bEqual("lstbl"_Str));
  sRequest = "list_tables"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* dump an index                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bDumpIndex(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[2]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" dump_index"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* dump a table                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bDumpTable(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" dump_table"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* docol                                                                      */
/*----------------------------------------------------------------------------*/
bool SEitf::bDocol(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  //
  sRequest.vConcat(" docol_command"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* display off selected rows                                                  */
/*----------------------------------------------------------------------------*/
bool SEitf::bDisplayOff(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  //
  sRequest.vConcat(" displayOff"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* display selected rows                                                      */
/*----------------------------------------------------------------------------*/
bool SEitf::bDisplayOn(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  //
  sRequest.vConcat(" displayOn"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* drop an index                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bDropIndex(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[2]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" drop_index"_Str);
  
  return bAppendRequest(sRequest);
}


/*----------------------------------------------------------------------------*/
/* drop a table                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bDropTable(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" drop_table"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* add a column                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bAddColumn(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 4) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[3]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" \""_Str);
  sRequest.vConcat(xTokenSet[2]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" \""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" add_column"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* add an index column                                                        */
/*----------------------------------------------------------------------------*/
bool SEitf::bAddIndexColumn(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 4) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy(" \""_Str);
  sRequest.vConcat(xTokenSet[3]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" \""_Str);
  sRequest.vConcat(xTokenSet[2]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" \""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" add_idxcolumn"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* add a partition column                                                     */
/*----------------------------------------------------------------------------*/
bool SEitf::bAddPartitionColumn(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vConcat(" \""_Str);
  sRequest.vConcat(xTokenSet[2]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" \""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" add_partcolumn"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* end of create index sequence                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bEndCreateIndex(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[2]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" end_create_index"_Str);
        
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* end of create table sequence                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bEndCreateTable(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" end_create_table"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* list columns                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bListColumns(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" list_columns"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* list index columns                                                         */
/*----------------------------------------------------------------------------*/
bool SEitf::bListIndexColumns(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat("\""_Str);
  sRequest.vConcat(xTokenSet[2]);
  sRequest.vConcat("\""_Str);

  sRequest.vConcat(" list_indexcolumns"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* list partition columns                                                     */
/*----------------------------------------------------------------------------*/
bool SEitf::bListPartitionColumns(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" list_partitioncolumns"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* analyze an index                                                           */
/*----------------------------------------------------------------------------*/
bool SEitf::bAnalyzeIndex(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[2]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);

  sRequest.vConcat(" analyze_Index"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* analyze a table                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bAnalyzeTable(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" analyze_table"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* detailed analysis of an index                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bDanalyzeIndex(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[2]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" danalyze_index"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* detailed analysis of a table                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bDanalyzeTable(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" danalyze_table"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* copy a table                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bCopyTable(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[2]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" \""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" copy_table"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* distributed export of a table                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bDistExportTable(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[2]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" \""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" dist_export_table"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* export a table                                                             */
/*----------------------------------------------------------------------------*/
bool SEitf::bExportTable(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;

  if (xTokenSet.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[2]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" \""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" export_table"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* load a table from file                                                     */
/*----------------------------------------------------------------------------*/
bool SEitf::bLoadTable(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[2]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" \""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" load_table"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* load a table from stream                                                   */
/*----------------------------------------------------------------------------*/
bool SEitf::bLoadTableStream(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[2]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" \""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" load_table_stream"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* lower select rows from a table                                             */
/*----------------------------------------------------------------------------*/
bool SEitf::bLowerSelectTable(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vConcat(" \""_Str);
  sRequest.vConcat(xTokenSet[2]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" \""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" lower_select_table"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* select rows from a table                                                   */
/*----------------------------------------------------------------------------*/
bool SEitf::bSelectTable(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" select_table"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* range select rows from a table                                             */
/*----------------------------------------------------------------------------*/
bool SEitf::bRangeSelectTable(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 4) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }

  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[3]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" \""_Str);
  sRequest.vConcat(xTokenSet[2]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" \""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" range_select_table"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get address/reference of a table                                           */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetTable(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" get_table"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* return whether table is in use or not                                      */
/*----------------------------------------------------------------------------*/
bool SEitf::bInUseTable(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);

  sRequest.vConcat(" table_inuse"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* return address/refernce of a table                                         */
/*----------------------------------------------------------------------------*/
bool SEitf::bRetTable(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vConcat(" return_table"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get pool space                                                             */
/*----------------------------------------------------------------------------*/
bool SEitf::bPoolSpace(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vConcat(" space"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get node status                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bNodeStatus(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vConcat(" status"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* exact key select from a table given an index                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bKeySelectIndex(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 4) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vConcat(" \""_Str);
  sRequest.vConcat(xTokenSet[3]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" \""_Str);
  sRequest.vConcat(xTokenSet[2]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" \""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" key_select_index"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* lower select rows from a table given an index                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bLowerSelectIndex(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 4) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vConcat(" \""_Str);
  sRequest.vConcat(xTokenSet[3]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" \""_Str);
  sRequest.vConcat(xTokenSet[2]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" \""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" lower_select_index"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* range select rows from a table given an index                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bRangeSelectIndex(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 5) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vConcat(" \""_Str);
  sRequest.vConcat(xTokenSet[4]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" \""_Str);
  sRequest.vConcat(xTokenSet[3]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" \""_Str);
  sRequest.vConcat(xTokenSet[2]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" \""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" range_select_index"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* size of index                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bSizeIndex(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[2]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" size_index"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* size of table                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bSizeTable(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" size_table"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* size of table                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bSizeTables(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 1) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vConcat(" size_tables"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* truncate an index                                                          */
/*----------------------------------------------------------------------------*/
bool SEitf::bTruncateIndex(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[2]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" truncate_index"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* upper select rows from a table on a given index                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bUpperSelectIndex(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 4) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vConcat(" \""_Str);
  sRequest.vConcat(xTokenSet[3]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" \""_Str);
  sRequest.vConcat(xTokenSet[2]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" \""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" upper_select_index"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* truncate a table                                                           */
/*----------------------------------------------------------------------------*/
bool SEitf::bTruncateTable(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 2) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }

  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" truncate_table"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* visualize an index                                                         */
/*----------------------------------------------------------------------------*/
bool SEitf::bVisualizeIndex(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  if (xTokenSet.nTokens() != 3) {
    vSetLastError(EC_INVALID_PARAMETERS);
    return false;
  }
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[2]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" visualize_index"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* insert row into row buffer                                                 */
/*----------------------------------------------------------------------------*/
bool SEitf::bInsertRow(Str* aRow)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
                              
  // check for working buffer
  if (! m_pRowBuffer[m_iPartition]) {
    m_pRowBuffer[m_iPartition] = (EntryBuffer*)EntryBuffer::pNewEntryBuffer(CN_ROWBUFFER);
    if (! m_pRowBuffer[m_iPartition]) {
      vTrace("EntryBuffer allocation failed\n"_Str);
      m_eErrorCode = EC_LOW_BUFFERS;
      return false;
    }
  }

  // get working buffer
  EntryBuffer* pRowBuffer = m_pRowBuffer[m_iPartition];
    //vTrace("Buffer: %s Partition:%d\n"_Str,pRowBuffer, m_iPartition);
  
  // insert row into working buffer
  //if (pRowBuffer->bInsertRow(m_pTxRef,aRow,aRow->nSize()))
  //  return true;
  
 if (m_pCurrentTable->bInsertRow(pRowBuffer, m_iPartition, m_pTxRef, aRow, aRow->nSize()))
   return true;
  
  //vTrace("Buffer full... Inserting buffer\n"_Str);
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
  m_pRowBuffer[m_iPartition] = (EntryBuffer*)EntryBuffer::pNewEntryBuffer(CN_ROWBUFFER);
  if (! m_pRowBuffer[m_iPartition]) {
    vTrace("EntryBuffer allocation failed\n"_Str);
    m_eErrorCode = EC_LOW_BUFFERS;
    return false;
  }
  
  pRowBuffer = m_pRowBuffer[m_iPartition];
  
  // insert row into new working buffer
  //bool bInserted = pRowBuffer->bInsertRow(m_pTxRef,aRow,aRow->nSize());
  
  bool bInserted = m_pCurrentTable->bInsertRow(pRowBuffer, m_iPartition, m_pTxRef, aRow, aRow->nSize());
  
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
bool SEitf::bInsertBuffer(EntryBuffer* pRowBuffer, i8 iPartition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  i8 iSlot = Partition::iPartitionToSlot(iPartition);

  s256 sRequest;
  s256 sBuffer;

  sBuffer = iSlot;
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(sBuffer);
  sRequest.vConcat("\""_Str);
  
  sBuffer = (i8)pRowBuffer;
  sRequest.vConcat(" \""_Str);
  sRequest.vConcat(sBuffer);
  sRequest.vConcat("\""_Str);
  
  sBuffer = (i8)m_pCurrentTable;
  sRequest.vConcat(" \""_Str);
  sRequest.vConcat(sBuffer);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" append_table"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* push dictionary                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bPushDic(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" cf"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
bool SEitf::bPrint(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  vInfo("%s\n"_Str,xTokenSet[1]->s());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* get dictionary path                                                        */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetDicPath(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(xTokenSet[0]->bEqual("pwf"_Str));
  sRequest = "pwf"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get dictionary contents                                                    */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetDicContents(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(xTokenSet[0]->bEqual("lf"_Str));
  sRequest = "lf"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get dictionary contents                                                    */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetDicContentsG(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(xTokenSet[0]->bEqual("lfG"_Str));
  sRequest = "lfG"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get monitor string                                                         */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetMonitor(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(xTokenSet[0]->bEqual("mon"_Str));
  sRequest = "get_monitor"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get free space                                                             */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetFreeSpace(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(xTokenSet[0]->bEqual("getFreeSpace"_Str));
  sRequest = "getFreeSpace"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get inUse space                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetInUseSpace(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(xTokenSet[0]->bEqual("getInUseSpace"_Str));
  sRequest = "getInUseSpace"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get total space                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetTotalSpace(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(xTokenSet[0]->bEqual("getTotalSpace"_Str));
  sRequest = "getTotalSpace"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get version                                                                */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetVersion(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(xTokenSet[0]->bEqual("version"_Str));
  sRequest = "get_version"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* ping test                                                                  */
/*----------------------------------------------------------------------------*/
bool SEitf::bPing(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(xTokenSet[0]->bEqual("ping"_Str));
  sRequest = "ping"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* send object                                                                */
/*----------------------------------------------------------------------------*/
bool SEitf::bObject(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  sRequest.vConcat("object"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* print row buffer                                                           */
/*----------------------------------------------------------------------------*/
bool SEitf::bPrintRowBuffer(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);

  ASSERT(xTokenSet[0]->bEqual("pRB"_Str));
  sRequest.vConcat(" pRB"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get seitf count                                                            */
/*----------------------------------------------------------------------------*/
bool SEitf::bSeitfCount(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(xTokenSet[0]->bEqual("seitf"_Str));
  sRequest = "seitf_count"_Str;
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* x test                                                                     */
/*----------------------------------------------------------------------------*/
bool SEitf::bX(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(xTokenSet[0]->bEqual("ping"_Str));
  sRequest = "ping"_Str;
  
  return bAppendRequest(sRequest);
}


/*----------------------------------------------------------------------------*/
/* return task                                                                */
/*----------------------------------------------------------------------------*/
bool SEitf::bRetTask(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(xTokenSet[0]->bEqual("rettsk"_Str));
  
  sRequest.vCopy("\""_Str);
  sRequest.vConcat(xTokenSet[1]);
  sRequest.vConcat("\""_Str);
  
  sRequest.vConcat(" return_task"_Str);
  
  return bAppendRequest(sRequest);
}

/*----------------------------------------------------------------------------*/
/* get task                                                                   */
/*----------------------------------------------------------------------------*/
bool SEitf::bGetTask(TokenSet& xTokenSet)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(xTokenSet[0]->bEqual("gettsk"_Str));
  
  sRequest.vCopy("get_task"_Str);
  
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
  
  s48K sWorkspace;
  Str* aCursor = nullptr;
  
  ValWsp xValWsp;
  ValSet xValSet;
  
  // Before appending request for se, ensure that se is running
  // get the se pid from the attachmap
  i8 iPid = gpAttachMap->m_iPid;
  if (!Process::bTestPid(iPid)) {
    // SE is not running anymore!
    vError("Cannot find pid %lld\n"_Str, gpAttachMap->m_iPid);
    vSetLastError(EC_SE_NOT_RUNNING);
    return false;
  }
  
  sWorkspace = (i8)this;
  sWorkspace.vConcat(" >seitf task::this task.seitf! "_Str);
  xValWsp = sWorkspace;
  //vTrace("%s\n"_Str,xVal.s());
  xValSet.vAppend(aCursor,xValWsp.pNestedValueSelf());
  
  xValWsp = sRequest;
  //vTrace("%s\n"_Str,xVal.s());
  xValSet.vAppend(aCursor,xValWsp.pNestedValueSelf());
  
  // if the current request id matches the next request id, it means
  // we have not read a response of the previous request
  if (m_iCurrentRequestId == m_iNextRequestId) {
    vError("Response for previous request not retrieved. Out-Of-Sync request-response.\n"_Str);
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
  do {
    m_iNextRequestId = Random::iRandom(0,99999);
  } while (m_iNextRequestId == m_iCurrentRequestId);

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
  Str* aCursor = nullptr;
  
  if (! m_pResponseQueue->bPop(&xValSet))
    return false;
  
  Value* pValue;
  
  b = xValSet.bExtractValue(aCursor,pValue);
  ASSERT(b);
  eResult = (eEC)_pIntVal(pValue)->iInt();
  
  b = xValSet.bExtractValue(aCursor,pValue);
  ASSERT(b);
  *_pStrVal(&sResponse) = _pStrVal(pValue);
  
  b = xValSet.bExtractValue(aCursor,pValue);
  ASSERT(! b);
  
  //  vTrace("sResponse: >>>\n%s\n<<<\n\n"_Str,sResponse.s());
  //  vTrace("\n---\n\n"_Str,sResponse.s());
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get last error                                                             */
/*----------------------------------------------------------------------------*/
Str* SEitf::pGetLastError(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  return m_sErrorMsg;
}

/*----------------------------------------------------------------------------*/
/* clear last error                                                           */
/*----------------------------------------------------------------------------*/
void SEitf::vClearLastError(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  m_eErrorCode = EC_NONE;
  m_sErrorMsg = ""_Str;
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
  m_sErrorMsg = aEC[m_eErrorCode];
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
