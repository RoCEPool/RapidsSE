/*============================================================================*/
/* SEitf.cpp - se interface                                                   */
/*============================================================================*/

#include "Interface.h"
#include "StorageEngine.h"

extern Heap*        gpMediumHeap;
extern Dictionary*  gpMainDic;
extern SEitfList*   gpSEitfList;
extern AttachMap*   gpAttachMap;

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SEitf* Interface::_pSEitf(void* pAddr)
{
  SEitf* pSEitf = reinterpret_cast<SEitf*>(pAddr);
  
  return pSEitf;
}

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SEitf* Interface::_pSEitf(i8 iAddr)
{
  SEitf* pSEitf = reinterpret_cast<SEitf*>(iAddr);
  
  return pSEitf;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* SEitf::operator new(size_t iSize)
{
  RASSERT(iSize);
  Object* pObject = gpMediumHeap->pAllocObject(CN_SEITF);
  
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
SEitf::SEitf(i8 iPartition)
: Object(CN_SEITF)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  m_iPid = 0;
  m_iPartitions = gpAttachMap->m_iPartitions;
  m_iPartition  = iPartition;
  
  m_xDictionary.vName("seitf");
  //m_xDicStack;
  m_pRequestQueue   = nullptr;
  m_pResponseQueue  = nullptr;
  m_eErrorCode      = EC_NONE;
  m_sErrorMsg       = pEC[m_eErrorCode];
  m_sResponse       = "";
  
  SEitf* pSEitf = this;
  gpSEitfList->vAppend(pSEitf);
  
  _checkInitialised_(sizeof(SEitf));
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SEitf::~SEitf(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);

  SEitf* pSEitf = this;
  
  gpSEitfList->vRemove(pSEitf);
}

/*----------------------------------------------------------------------------*/
/* pop dictionary from dictionary stack                                       */
/*----------------------------------------------------------------------------*/
DictionaryStack* SEitf::pDicStack(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);

  return &m_xDicStack;
}

/*----------------------------------------------------------------------------*/
/* push dictionary onto dictionary stack                                      */
/*----------------------------------------------------------------------------*/
void SEitf::vPushDic(Dictionary* pDictionary)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);

  m_xDicStack.vPush(pDictionary);
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
  
  // pid
  m_iPid = Process::iGetPid();
  m_xDictionary.vAddConstant("pid",m_iPid);
  
  // response queue
  Word* pQueueWord = gpMainDic->pFind("queue");
  Word* pRspQueWord = new(&m_xDictionary) Word("rspque");
  pRspQueWord->vWordType(WT_OBJECT);
  pRspQueWord->vKind(pQueueWord);
  pRspQueWord->vDoesStep(nullptr);
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
  m_xDicStack.vPush(&m_xDictionary);
  
  // find tenants
  Word* pTenants = gpMainDic->pFind("tenants");
  ASSERT(pTenants);
  
  Cell* pCell = pTenants->pFirstCell();
  m_xDicStack.vPush(reinterpret_cast<Dictionary*>(pCell));
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
/* decode request                                                             */
/*----------------------------------------------------------------------------*/
bool SEitf::bDecode(char* pWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  if (0 == String::iLength(pWords[0])) {
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
  
  if (String::bEqualTo("cf",pWords[0],C_NO_CASE)) {
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
  
  if (String::bEqualTo("docol",pWords[0],C_NO_CASE)) {
    bDocol(pWords);
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
  
  if (String::bEqualTo("getFreeSpace",pWords[0],C_NO_CASE)) {
    bGetFreeSpace(pWords);
    return true;
  }
  
  if (String::bEqualTo("getInUseSpace",pWords[0],C_NO_CASE)) {
    bGetInUseSpace(pWords);
    return true;
  }
  
  if (String::bEqualTo("getTotalSpace",pWords[0],C_NO_CASE)) {
    bGetTotalSpace(pWords);
    return true;
  }
  
  if (String::bEqualTo("ld",pWords[0],C_NO_CASE)) {
    bLoadTable(pWords);
    return true;
  }
  
  if (String::bEqualTo("lf",pWords[0],C_NO_CASE)) {
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
  
  if (String::bEqualTo("pwf",pWords[0],C_NO_CASE)) {
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
  
  if (String::bEqualTo("__x",pWords[0],C_NO_CASE)) {
    bX(pWords);
    return true;
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
  
  vTrace("Unknown command: %s\n",pWords[0]);
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
  vAppendRequest(sRequest);
  
  return true;
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
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop a dictionary                                                          */
/*----------------------------------------------------------------------------*/
bool SEitf::bDropDictionary(char* pWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(String::bEqualTo(pWords[0],"lsten"));
  sRequest = "list_tenants";
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop a tenant                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bDropTenant(char* pWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(String::bEqualTo(pWords[0],"lscat"));
  sRequest = "list_catalogs";
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop a catalog                                                             */
/*----------------------------------------------------------------------------*/
bool SEitf::bDropCatalog(char* pWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(String::bEqualTo(pWords[0],"lssch"));
  sRequest = "list_schemas";
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop a schema                                                              */
/*----------------------------------------------------------------------------*/
bool SEitf::bDropSchema(char* pWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  s256 sRequest;
  
  ASSERT(String::bEqualTo(pWords[0],"lstbl"));
  sRequest = "list_tables";
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* dump a table                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bDumpTable(char* pWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
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
  
  vAppendRequest(sRequest);
  return true;
}

/*----------------------------------------------------------------------------*/
/* drop a table                                                               */
/*----------------------------------------------------------------------------*/
bool SEitf::bDropTable(char* pWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
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
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
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
  
  vAppendRequest(sRequest);
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
bool SEitf::bPrint(char* pWords[])
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  printf("%s\n",pWords[1]);
  
  return true;
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
  vAppendRequest(sRequest);
  
  return true;
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
  vAppendRequest(sRequest);
  
  return true;
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
  vAppendRequest(sRequest);
  
  return true;
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
  vAppendRequest(sRequest);
  
  return true;
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
  vAppendRequest(sRequest);
  
  return true;
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
  vAppendRequest(sRequest);
  
  return true;
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
  vAppendRequest(sRequest);
  
  return true;
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
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append request to request queue                                            */
/*----------------------------------------------------------------------------*/
void SEitf::vAppendRequest(s& sRequest)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
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
eEC SEitf::eGetResponse(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  while (! bPopResponse(m_eErrorCode, m_sResponse))
    ;
  
  return m_eErrorCode;
}

/*----------------------------------------------------------------------------*/
/* pop response from reponse queue                                            */
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