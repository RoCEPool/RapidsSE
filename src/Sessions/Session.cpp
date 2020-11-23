/*============================================================================*/
/* Session.cpp - se session                                                   */
/*============================================================================*/

#include "Interface.h"

Atomic gixSession;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Session* Sessions::_pSession(void* aAddr)
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
Session* Sessions::_pSession(i8 iAddr)
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
  Session* pSession = reinterpret_cast<Session*>(HeapMem::pAllocate(CN_SESSION));
  
  return pSession;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Session::operator delete(void* aAddress)
{
  Session* pSession = reinterpret_cast<Session*>(aAddress);
  
  HeapMem::vFree(pSession);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Session::Session(void)
: Focal(CN_SESSION, AM::iNextSessionId(), C_UNKNOWN),
  m_oSsnMutex(MK_JOB,this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  m_i_Session  = -1;
  m_eSession   = SK_NONE;
  
  m_eApiFun    = AC_NONE;
  m_iApiLoc    = 0;
  m_eApiError  = EC_NONE;
  m_bRunning   = false;
  m_bSignalled = false;
  
  m_aStackBase = nullptr;
  m_aStackPtr  = nullptr;
  
  m_eJobClass  = CN_NONE;
  m_iJobLoc    = 0;
  m_iSenLoc    = 0;
  
  m_oTimer.vReset();
  m_qTotalUsec = 0;
  
  m_nBegun     = 0;
  m_nExecuted  = 0;
  m_nCancelled = 0;
  m_nEnded     = 0;
  
  m_nApiCalls  = 0;
  m_nApiErrors = 0;
  m_nJobNew    = 0;
  m_nJobDelete = 0;
  
  m_i_NextFree  = -1;
  
  m_iPid = Process::iGetPid();

  m_i_Strand      = C_UNKNOWN;
  
  //m_oDicStack;
  m_pSsnJob     = nullptr;
  m_pTx         = nullptr;
  m_bExiting    = false;
  m_pJobStack   = new Stack(KK_JOB_STACK);

  m_sToDo       = ""_Stz;
    
    
  _checkInitialised_(CN_SESSION);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  - -
  
  // set initial session <path>
  m_oSsnTagSet.vInsert(C_TAG_PATH,C_SE_PATH);
  
  ++ gixSession;
  AM::vLogSsn(SL_INF,this,AA_SSN_CONSTRUCT,
              "Session construct : %p (%lld tasks exist)"_Stz, //!format-LogSsn
              this,* gixSession);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Session::~Session(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  AM::pSessionArray()->vFreeSession(this);
  
  RASSERT(0 == m_pJobStack->nValues());
  delete m_pJobStack;
        
  -- gixSession;
  AM::vLogSsn(SL_INF,this,AA_SSN_DESTRUCT,
              "Session destruct : %p (%lld tasks exist)"_Stz, //!format-LogSsn
              this,* gixSession);
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
  
  m_pTx->vPhase(XP_COMMITTED);
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
  
  vTrace("pid %lld %s strand %lld"_Stz,
         pSession->m_iPid,
         Process::bTestPid(pSession->m_iPid) ? "exists"_Stz : "non-existant"_Stz,
         pSession->i_Strand());
}

/*----------------------------------------------------------------------------*/
/* focal outline                                                              */
/*----------------------------------------------------------------------------*/
void Session::vFocalOutline(s4K& sOutline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  sOutline.vSprint("<%s focal>"_Stz,aCN[eClass()]);
}

/*----------------------------------------------------------------------------*/
/* initialise this session                                                    */
/*----------------------------------------------------------------------------*/
eEC Session::eInit(Stz* pCatalogName, Stz* pSchemaName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  Dic* pDic = nullptr;
  Cell* aCell =  nullptr;
  //m_oDicStack.vContentDump();
 
  //fprintf(stderr,"eInit A\n");
  
  // first discard whatever is there on the DicStack
  for (i8 iCount = m_oDicStack.iCount(); iCount; iCount--)
    m_oDicStack.vDiscard();
  
  //fprintf(stderr,"eInit B\n");
  
  // set up initial default path
  
  // /tenants
  //Word* pTenants = gpAttachMap->m_oRootDic.pFindWord("tenants"_Stz);
  Word* pTenants = AM::pTenants();
  //fprintf(stderr,"eInit C\n");
  
  RASSERT(pTenants);

  pDic = _pDic(pTenants->pObject());
  m_oDicStack.vPush(pDic);
  
  //fprintf(stderr,"eInit D\n");
  
  // /tenants/boray
  Word* pTenant = m_oDicStack.pFindInCurrent("boray"_Stz);
  RASSERT(pTenant);

  pDic = _pDic(pTenant->pObject());
  m_oDicStack.vPush(pDic);

  //fprintf(stderr,"eInit E\n");
  
  // /tenants/boray/catalogs
  Word* pCatalogs = m_oDicStack.pFindInCurrent("catalogs"_Stz);
  RASSERT(pCatalogs);

  pDic = _pDic(pCatalogs->pObject());
  m_oDicStack.vPush(pDic);

  //fprintf(stderr,"eInit F\n");
  
  if ( ! pCatalogName)
    return EC_NONE;
  
  // /tenants/boray/catalogs/rapidsse
  Word* pCatalog = m_oDicStack.pFindInCurrent(pCatalogName);
  if ( ! pCatalog)
    return EC_NO_SUCH_CATALOG;
  
  pDic = _pDic(pCatalog->pObject());
  m_oDicStack.vPush(pDic);

  //fprintf(stderr,"eInit G\n");
  
  // /tenants/boray/catalogs/rapidsse/schemas
  Word* pSchemas = m_oDicStack.pFindInCurrent("schemas"_Stz);
  RASSERT(pSchemas);
  
  //fprintf(stderr,"eInit H\n");
  pDic = _pDic(pSchemas->pObject());
  //fprintf(stderr,"eInit I\n");
  
  m_oDicStack.vPush(pDic);

  if ( ! pSchemaName )
    return EC_NONE;
  
  //fprintf(stderr,"eInit J\n");
  
  // /tenants/boray/catalogs/rapidsse/schemas/public
  Word* pSchema = m_oDicStack.pFindInCurrent(pSchemaName);
  if ( ! pSchema)
    return EC_NO_SUCH_SCHEMA;
  
  pDic = _pDic(pSchema->pObject());
  m_oDicStack.vPush(pDic);

  //fprintf(stderr,"eInit K\n");
  
  // /tenants/boray/catalogs/rapidsse/schemas/public/tables
  Word* pTables = m_oDicStack.pFindInCurrent("tables"_Stz);
  RASSERT(pTables);
  
  //fprintf(stderr,"eInit L\n");
  
  pDic = _pDic(pTables->pObject());
  m_oDicStack.vPush(pDic);

  //fprintf(stderr,"eInit M\n");
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* which partition                                                            */
/*----------------------------------------------------------------------------*/
i8 Session::i_Strand(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  return m_i_Strand;
}

/*----------------------------------------------------------------------------*/
/* set partition                                                              */
/*----------------------------------------------------------------------------*/
void Session::vStrand(i8 i_Strand)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  m_i_Strand = i_Strand;
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
/* report this session                                                        */
/*----------------------------------------------------------------------------*/
void Session::vReport(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  vTrace("Session %llx job %llx\n"_Stz,
         this,
         m_pSsnJob);
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
/* free the session state                                                     */
/*----------------------------------------------------------------------------*/
void Session::vFreeSession(i8 i_NextFree)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  RASSERT(m_i_Session >= 0 && m_i_Session < C_MAX_SESSIONS);
  m_i_NextFree = i_NextFree;
  
  m_eApiFun    = AC_NONE;
  m_iApiLoc    = 0;
  m_eApiError  = EC_NONE;
  m_bRunning   = false;
  m_bSignalled = false;
  
  m_aStackBase = nullptr;
  m_aStackPtr  = nullptr;
  
  m_eJobClass  = CN_NONE;
  m_iJobLoc    = 0;
  m_iSenLoc    = 0;
  
  m_oTimer.vReset();
  m_qTotalUsec = 0;
  
  m_nBegun     = 0;
  m_nExecuted  = 0;
  m_nCancelled = 0;
  m_nEnded     = 0;
  
  m_nApiCalls  = 0;
  m_nApiErrors = 0;
  m_nJobNew    = 0;
  m_nJobDelete = 0;
  
  m_pSsnJob    = nullptr;
  m_pTx        = nullptr;
  m_bExiting   = false;
  
  RASSERT(0 == m_pJobStack->nValues());
}

/*----------------------------------------------------------------------------*/
/* trace                                                                      */
/*----------------------------------------------------------------------------*/
void Session::vTraceSession(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  if (m_bRunning)
    m_oTimer.vUpdate();
  
  i8 nBytes;
  if (m_aStackBase >= m_aStackPtr)
    nBytes = nDifference(m_aStackBase,m_aStackPtr);
  else
    nBytes = nDifference(m_aStackPtr,m_aStackBase);
  
  vTrace("  %lld: session %p base %llx ptr %llx (%lldB)\n"
         "     apiFun %s apiLoc %lld job %s jobLoc %lld senLoc %lld \n"
         "     %s %s apiError %s time %lldus\n"
         "     dist2PC: begun %lld executed %lld cancelled %lld ended %lld\n"
         "     #calls %lld, #errors %lld, #jobNew %lld #jobDelete %lld total %lldus\n"_Stz,
         
         m_i_Session,
         this,
         m_aStackBase,
         m_aStackPtr,
         nBytes,
         
         aAC[m_eApiFun],
         m_iApiLoc,
         aCN[m_eJobClass],
         m_iJobLoc,
         m_iSenLoc,
         
         m_bRunning   ? "running"   : "completed",
         m_bSignalled ? "signalled" : "not signalled",
         aEC[m_eApiError],
         m_oTimer.iMicroseconds(),
         
         m_nBegun,
         m_nExecuted,
         m_nCancelled,
         m_nEnded,
         
         m_nApiCalls,
         m_nApiErrors,
         m_nJobNew,
         m_nJobDelete,
         m_qTotalUsec
         );
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Session);

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

