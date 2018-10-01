/*============================================================================*/
/* SEitf.cpp - se interface                                                   */
/*============================================================================*/

#include "Interface.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

struct ReqTable {
  Stz* aCommand;
  bool (SEitf::*aFunc)(TokenVec& xTokenVec, i8 iReqNo);
  Stz* aRequest;
};

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SEitf* Interface::_pSEitf(void* aAddr)
{
  SEitf* pSEitf = reinterpret_cast<SEitf*>(aAddr);
  
  if (pSEitf && CN_SEITF != pSEitf->eClass())
    vTrace("INVALID SEITF %p %s\n"_Stz, pSEitf, aCN[pSEitf->eClass()]);
  
  RASSERT( !pSEitf || CN_SEITF == pSEitf->eClass() );
  
  return pSEitf;
}

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
SEitf* Interface::_pSEitf(i8 iAddr)
{
  SEitf* pSEitf = reinterpret_cast<SEitf*>(iAddr);
  
  if (pSEitf && CN_SEITF != pSEitf->eClass())
    vTrace("INVALID SEITF %p %s\n"_Stz, pSEitf, aCN[pSEitf->eClass()]);
  
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

  m_iPartition  = iPartition;
  
  //m_xPathStack;
  m_sResponse       = ""_Stz;
  m_pJob            = nullptr;
  m_pTx             = nullptr;
  m_bDisplay        = true;
  
  m_bSeitfExiting     = false;
  
  m_pRowOnce = nullptr;
  
  //m_xFormatters
  
  m_pPerformanceTrace = nullptr;
  
  m_pCommandProcessor = new CommandProcessor;

  _checkInitialised_(CN_SEITF);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SEitf::~SEitf(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  if (m_pRowOnce)
    delete m_pRowOnce;
  
  delete m_pCommandProcessor;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void SEitf::vContentDump(void)
{
  vTrace("SEitf %p: pid %lld (%s)\n"_Stz,
         this,
         m_iPid,
         Process::bTestPid(m_iPid) ? "exists"_Stz : "non-existant"_Stz
         );
}

/*----------------------------------------------------------------------------*/
/* begin a tx                                                                 */
/*----------------------------------------------------------------------------*/
bool SEitf::bBeginTx()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  m_pTx = new Tx;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* commit a tx                                                                */
/*----------------------------------------------------------------------------*/
void SEitf::vCommitTx()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  m_pTx->vPhase(TP_COMMITTED);
}

/*----------------------------------------------------------------------------*/
/* end a tx                                                                   */
/*----------------------------------------------------------------------------*/
void SEitf::vEndTx()
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  delete m_pTx;
  
  m_pTx = nullptr;
}

/*----------------------------------------------------------------------------*/
/* dump seitf                                                                 */
/*----------------------------------------------------------------------------*/
void SEitf::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  RASSERT(eClass == CN_SEITF);
  RASSERT(pObject->eClass() == CN_SEITF);
  
  SEitf* pSEitf = reinterpret_cast<SEitf*>(pObject);
  
  vTrace("pid %lld %s partition %lld slot %lld"_Stz
         " response '%s'\n"_Stz,
         pSEitf->m_iPid,
         Process::bTestPid(pSEitf->m_iPid) ? "exists"_Stz : "non-existant"_Stz,
         pSEitf->m_iPartition,
         pSEitf->iSlot(),
         pSEitf->m_sResponse.aChar());
}

/*----------------------------------------------------------------------------*/
/* initialise this seitf                                                      */
/*----------------------------------------------------------------------------*/
void SEitf::vInit(Stz* pCatalogName, Stz* pSchemaName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  //m_xPathStack.vContentDump();
  
  // first discard whatever is there on the DicStack
  for (i8 iCount = m_xPathStack.iCount(); iCount; iCount--)
    m_xPathStack.vDiscard();
  
  // set up initial default path
  
  // /tenants
  //Word* pTenants = gpAttachMap->m_xMainDic.pFindWord("tenants"_Stz);
  Word* pTenants = gpAttachMap->m_pTenants;
  ASSERT(pTenants);
  Cell* pCell = pTenants->pFirstCell();
  m_xPathStack.vPush(reinterpret_cast<Dic*>(pCell));

  // /tenants/boray
  Word* pTenant = m_xPathStack.pFindInCurrent("boray"_Stz);
  ASSERT(pTenant);
  pCell = pTenant->pFirstCell();
  m_xPathStack.vPush(reinterpret_cast<Dic*>(pCell));
  
  // /tenants/boray/catalogs
  Word* pCatalogs = m_xPathStack.pFindInCurrent("catalogs"_Stz);
  ASSERT(pCatalogs);
  pCell = pCatalogs->pFirstCell();
  m_xPathStack.vPush(reinterpret_cast<Dic*>(pCell));

  if ( ! pCatalogName)
    return;
  
  // /tenants/boray/catalogs/rapidsse
  Word* pCatalog = m_xPathStack.pFindInCurrent(pCatalogName);
  ASSERT(pCatalog);
  pCell = pCatalog->pFirstCell();
  m_xPathStack.vPush(reinterpret_cast<Dic*>(pCell));
  
  // /tenants/boray/catalogs/rapidsse/schemas
  Word* pSchemas = m_xPathStack.pFindInCurrent("schemas"_Stz);
  ASSERT(pSchemas);
  pCell = pSchemas->pFirstCell();
  m_xPathStack.vPush(reinterpret_cast<Dic*>(pCell));
  
  if ( ! pSchemaName )
    return;
  
  // /tenants/boray/catalogs/rapidsse/schemas/public
  Word* pSchema = m_xPathStack.pFindInCurrent(pSchemaName);
  ASSERT(pSchema);
  pCell = pSchema->pFirstCell();
  m_xPathStack.vPush(reinterpret_cast<Dic*>(pCell));

  // /tenants/boray/catalogs/rapidsse/schemas/public/tables
  Word* pTables = m_xPathStack.pFindInCurrent("tables"_Stz);
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
/* get path stack                                                             */
/*----------------------------------------------------------------------------*/
DicStack* SEitf::pPathStack(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  return &m_xPathStack;
}

/*----------------------------------------------------------------------------*/
/* get performance trace                                                      */
/*----------------------------------------------------------------------------*/
PerformanceTrace* SEitf::pPerformanceTrace(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  return m_pPerformanceTrace;
}

/*----------------------------------------------------------------------------*/
/* set performance trace                                                      */
/*----------------------------------------------------------------------------*/
void SEitf::vPerformanceTrace(PerformanceTrace* pPerformanceTrace)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  m_pPerformanceTrace = pPerformanceTrace;
}

/*----------------------------------------------------------------------------*/
/* which slot                                                                 */
/*----------------------------------------------------------------------------*/
i8 SEitf::iSlot(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  return TablePtn::iPartitionToSlot(m_iPartition);
}

/*----------------------------------------------------------------------------*/
/* get tx                                                                     */
/*----------------------------------------------------------------------------*/
Tx* SEitf::pTx(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  return m_pTx;
}

/*----------------------------------------------------------------------------*/
/* set tx                                                                     */
/*----------------------------------------------------------------------------*/
void SEitf::vTx(Tx* pTx)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  m_pTx = pTx;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* create row once object                                                     */
/*----------------------------------------------------------------------------*/
void SEitf::vCreateRowOnce(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  ASSERT(! m_pRowOnce);
  
  m_pRowOnce = new RowOnce;
}
/*----------------------------------------------------------------------------*/
/* test if row has been seen so far                                           */
/*----------------------------------------------------------------------------*/
bool SEitf::bRowSeen(Id xRowId)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF,_FOOTER_);
  
  if (! m_pRowOnce)
    return false;
  
  bool bSeen = m_pRowOnce->bRowSeen(xRowId);
  
  return bSeen;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* SEitf::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEITF);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

