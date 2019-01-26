/*============================================================================*/
/* Session.cpp - se session                                                   */
/*============================================================================*/

#include "Interface.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

struct ReqTable {
  Stz* aCommand;
  bool (Session::*aFunc)(TokenVec& xTokenVec, i8 iReqNo);
  Stz* aRequest;
};

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Session* Interface::_pSession(void* aAddr)
{
  Session* pSession = reinterpret_cast<Session*>(aAddr);
  
  if (pSession && CN_SESSION != pSession->eClass()) {
    vTrace("INVALID SESSION %p %s\n"_Stz, pSession, aCN[pSession->eClass()]);
    //return nullptr;
  }
  
  RASSERT_TYPE(pSession,CN_SESSION);
  
  return pSession;
}

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Session* Interface::_pSession(i8 iAddr)
{
  Session* pSession = reinterpret_cast<Session*>(iAddr);
  
  if (pSession && CN_SESSION != pSession->eClass()) {
    vTrace("INVALID SESSION %p %s\n"_Stz, pSession, aCN[pSession->eClass()]);
    return nullptr;
  }
  
  RASSERT_TYPE(pSession,CN_SESSION);
  
  return pSession;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Session::operator new(size_t)
{
  Object* pObject = HeapMem::pAllocate(CN_SESSION);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Session::operator delete(void* pSEinterfaceAddress)
{
  Session* pSession = reinterpret_cast<Session*>(pSEinterfaceAddress);
  
  HeapMem::vFree(pSession);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Session::Session(i8 iPartition)
: Object(CN_SESSION)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  // pid
  m_iPid = Process::iGetPid();

  m_iPartition  = iPartition;
  
  //m_oDicStack;
  m_sResponse       = ""_Stz;
  m_pJob            = nullptr;
  m_pTx             = nullptr;
  m_bSessionExiting = false;
  m_pJobStack       = new Stack;


  m_pSessionState     = gpAttachMap->m_pApiState->pAllocState(this);
  m_pPerformanceTrace = nullptr;
  m_pCommandProcessor = new CommandProcessor;

  _checkInitialised_(CN_SESSION);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Session::~Session(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  gpAttachMap->m_pApiState->vFreeState(this);
  
  RASSERT(0 == m_pJobStack->nValues());
  delete m_pJobStack;
  
  delete m_pCommandProcessor;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Session::vContentDump(void)
{
  vTrace("Session %p: pid %lld (%s)\n"_Stz,
         this,
         m_iPid,
         Process::bTestPid(m_iPid) ? "exists"_Stz : "non-existant"_Stz
         );
}

/*----------------------------------------------------------------------------*/
/* begin a tx                                                                 */
/*----------------------------------------------------------------------------*/
bool Session::bBeginTx()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  m_pTx = new Tx;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* commit a tx                                                                */
/*----------------------------------------------------------------------------*/
void Session::vCommitTx()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  m_pTx->vPhase(TP_COMMITTED);
}

/*----------------------------------------------------------------------------*/
/* end a tx                                                                   */
/*----------------------------------------------------------------------------*/
void Session::vEndTx()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  delete m_pTx;
  
  m_pTx = nullptr;
}

/*----------------------------------------------------------------------------*/
/* dump session                                                               */
/*----------------------------------------------------------------------------*/
void Session::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  Session* pSession = _pSession(pObject);
  
  vTrace("pid %lld %s partition %lld slot %lld"_Stz
         " response '%s'\n"_Stz,
         pSession->m_iPid,
         Process::bTestPid(pSession->m_iPid) ? "exists"_Stz : "non-existant"_Stz,
         pSession->m_iPartition,
         pSession->iSlot(),
         pSession->m_sResponse.aChar());
}

/*----------------------------------------------------------------------------*/
/* initialise this session                                                    */
/*----------------------------------------------------------------------------*/
eEC Session::eInit(Stz* pCatalogName, Stz* pSchemaName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  //m_oDicStack.vContentDump();
  
  // first discard whatever is there on the DicStack
  for (i8 iCount = m_oDicStack.iCount(); iCount; iCount--)
    m_oDicStack.vDiscard();
  
  // set up initial default path
  
  // /tenants
  //Word* pTenants = gpAttachMap->m_oRootDic.pFindWord("tenants"_Stz);
  Word* pTenants = gpAttachMap->m_pTenants;
  ASSERT(pTenants);
  Cell* aCell = pTenants->aFirstCell();
  m_oDicStack.vPush(reinterpret_cast<Dic*>(aCell));

  // /tenants/boray
  Word* pTenant = m_oDicStack.pFindInCurrent("boray"_Stz);
  ASSERT(pTenant);
  aCell = pTenant->aFirstCell();
  m_oDicStack.vPush(reinterpret_cast<Dic*>(aCell));
  
  // /tenants/boray/catalogs
  Word* pCatalogs = m_oDicStack.pFindInCurrent("catalogs"_Stz);
  ASSERT(pCatalogs);
  aCell = pCatalogs->aFirstCell();
  m_oDicStack.vPush(reinterpret_cast<Dic*>(aCell));

  if ( ! pCatalogName)
    return EC_NONE;
  
  // /tenants/boray/catalogs/rapidsse
  Word* pCatalog = m_oDicStack.pFindInCurrent(pCatalogName);
  if ( ! pCatalog)
    return EC_NO_SUCH_CATALOG;
  
  aCell = pCatalog->aFirstCell();
  m_oDicStack.vPush(reinterpret_cast<Dic*>(aCell));
  
  // /tenants/boray/catalogs/rapidsse/schemas
  Word* pSchemas = m_oDicStack.pFindInCurrent("schemas"_Stz);
  ASSERT(pSchemas);
  aCell = pSchemas->aFirstCell();
  m_oDicStack.vPush(reinterpret_cast<Dic*>(aCell));
  
  if ( ! pSchemaName )
    return EC_NONE;
  
  // /tenants/boray/catalogs/rapidsse/schemas/public
  Word* pSchema = m_oDicStack.pFindInCurrent(pSchemaName);
  if ( ! pSchema)
    return EC_NO_SUCH_SCHEMA;
  
  aCell = pSchema->aFirstCell();
  m_oDicStack.vPush(reinterpret_cast<Dic*>(aCell));

  // /tenants/boray/catalogs/rapidsse/schemas/public/tables
  Word* pTables = m_oDicStack.pFindInCurrent("tables"_Stz);
  ASSERT(pTables);
  
  aCell = pTables->aFirstCell();
  m_oDicStack.vPush(reinterpret_cast<Dic*>(aCell));
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* which partition                                                            */
/*----------------------------------------------------------------------------*/
i8 Session::iPartition(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  return m_iPartition;
}

/*----------------------------------------------------------------------------*/
/* get path stack                                                             */
/*----------------------------------------------------------------------------*/
DicStack* Session::pPathStack(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  return &m_oDicStack;
}

/*----------------------------------------------------------------------------*/
/* get performance trace                                                      */
/*----------------------------------------------------------------------------*/
PerformanceTrace* Session::pPerformanceTrace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  return m_pPerformanceTrace;
}

/*----------------------------------------------------------------------------*/
/* set performance trace                                                      */
/*----------------------------------------------------------------------------*/
void Session::vPerformanceTrace(PerformanceTrace* pPerformanceTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  m_pPerformanceTrace = pPerformanceTrace;
}

/*----------------------------------------------------------------------------*/
/* report this session                                                        */
/*----------------------------------------------------------------------------*/
void Session::vReport(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  vTrace("Session %llx job %llx\n"_Stz,
         this,
         m_pJob);
}

/*----------------------------------------------------------------------------*/
/* which slot                                                                 */
/*----------------------------------------------------------------------------*/
i8 Session::iSlot(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  return TablePtn::iPartitionToSlot(m_iPartition);
}

/*----------------------------------------------------------------------------*/
/* get tx                                                                     */
/*----------------------------------------------------------------------------*/
Tx* Session::pTx(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  return m_pTx;
}

/*----------------------------------------------------------------------------*/
/* set tx                                                                     */
/*----------------------------------------------------------------------------*/
void Session::vTx(Tx* pTx)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  m_pTx = pTx;
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Session);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

