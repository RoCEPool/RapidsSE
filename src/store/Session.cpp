/*============================================================================*/
/* Session.cpp - se session                                                   */
/*============================================================================*/

#include "BASE.h"

#include "StreamDef.hpp"

#include "Session.hpp"
#include "Sessions.hpp"
#include "Output.hpp"

#include "TagConstants.h"

Atomic gixSession;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Session,CN_SESSION)
__aNewFunction(Session,CN_SESSION)
__vDeleteFunction(Session,CN_SESSION)
__aFooterFunction(Session)

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Session* STORE::_pSession(i8 iAddr)
{
  Session* pSession = reinterpret_cast<Session*>(iAddr);
  
  if (pSession && CN_SESSION != pSession->eGetClassNo()) {
    vTrace("INVALID SESSION %p %s\n"_Stz, pSession, aCN[pSession->eGetClassNo()]);
    return nullptr;
  }
  
  RASSERT_TYPE(pSession,CN_SESSION);
  
  return pSession;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Session::Session(void)
: Object(CN_SESSION)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  m_i_Session  = -1;
  m_eSession   = SK_NONE;
  
  m_eApiFun    = AC_NONE;
  m_iApiLoc    = 0;
  m_eApiError  = EC_NONE;
  m_bRunning   = false;
  
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

  m_i_FixedPtn  = C_UNKNOWN;
  
  //m_oDicStack;
  m_pTx         = nullptr;

  m_sToDo       = ""_Stz;
    
  m_pStmtList   = new StmtList;
  
  m_bSystemCloser = false;
  
  // -- -- -- -- -- --
  
  m_iCmdNo = 0;

  
  m_tLastJobId_ssn = 0;
  
  _checkInitialised_(CN_SESSION);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  - -
    
  ++ gixSession;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Session::~Session(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
    
  delete m_pStmtList;
  
  AM::pSessions()->vFreeSession(this);
          
  -- gixSession;
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void Session::vContentDump(void)
{
  _static_
  
  vTrace("See Session::\n"_Stz);
  
  vTrace("  this %p\n"_Stz
         ,this);
  
  vTrace("  pid  %lld %s\n"_Stz
         ,m_iPid
         ,Process::bTestPid(m_iPid) ? "exists"_Stz : "non-existant"_Stz
        );
  
  vTrace(";;\n\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Punlic ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* append cascade                                                             */
/*----------------------------------------------------------------------------*/
void Session::vAppendCascade(Stz* szCascade)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
    
  //vTrace("vAppendCascade: %s\n"_Stz,szCascade);
  Statement* pStatement = new Statement(szCascade);
  
  m_pStmtList->vAppend(pStatement);
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
/* get sCascade                                                               */
/*----------------------------------------------------------------------------*/
Stz*  Session::szCascade(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
      
  Statement* pStatement = m_pStmtList->pHead();

  Stz* szText = pStatement->szText();
  
  return szText;
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
  UNUSED(eClass);
  UNUSED(aTab);
  
  Session* pSession = _pSession(pObject);
  
  vTrace("pid %lld %s stream %lld"_Stz,
         pSession->m_iPid,
         Process::bTestPid(pSession->m_iPid) ? "exists"_Stz : "non-existant"_Stz,
         pSession->i_FixedPtn());
}

/*----------------------------------------------------------------------------*/
/* which partition                                                            */
/*----------------------------------------------------------------------------*/
i8 Session::i_FixedPtn(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  return m_i_FixedPtn;
}

/*----------------------------------------------------------------------------*/
/* set partition                                                              */
/*----------------------------------------------------------------------------*/
void Session::vFixedPtn(i8 i_FixedPtn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  m_i_FixedPtn = i_FixedPtn;
}

/*----------------------------------------------------------------------------*/
/* free the session state                                                     */
/*----------------------------------------------------------------------------*/
void Session::vFinaliseSession(i8 i_NextFree)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  RASSERT(m_i_Session >= 0 && m_i_Session < C_MAX_SESSIONS);
  m_i_NextFree = i_NextFree;
  
  m_eApiFun    = AC_NONE;
  m_iApiLoc    = 0;
  m_eApiError  = EC_NONE;
  m_bRunning   = false;
  
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
  
  m_pTx        = nullptr;
  
}

/*----------------------------------------------------------------------------*/
/* get bHasCascade                                                            */
/*----------------------------------------------------------------------------*/
bool Session::bHasCascade(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
      
  if (m_pStmtList->nItems())
    return true;
  else
    return false;
}


/*----------------------------------------------------------------------------*/
/* initialise this session                                                    */
/*----------------------------------------------------------------------------*/
eEC Session::eInitaliseSession(Stz* szCatalogName, Stz* szSchemaName)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  Dic* pDic = nullptr;
  //m_oDicStack.vContentDump();
 
  //fprintf(stderr,"eInit A\n");
  
  // first discard whatever is there on the DicStack
  for (i8 iCount = m_oDicStack_ssn.nItems(); iCount; iCount--)
    m_oDicStack_ssn.vDiscard();
  
  //fprintf(stderr,"eInit B\n");
  
  // set up initial default path
  
  // /tenants
  //Word* pTenants = gpAttachMap->m_oRootDic.pFindWord("tenants"_Stz);
  Word* pTenants = AM::pTenants();
  //fprintf(stderr,"eInit C\n");
  
  RASSERT(pTenants);

  pDic = _pDic(pTenants->pObject());
  m_oDicStack_ssn.vPush(pDic);
  
  //fprintf(stderr,"eInit D\n");
  
  // /tenants/boray
  Word* pTenant = m_oDicStack_ssn.pFindInCurrent("boray"_Stz);
  RASSERT(pTenant);

  pDic = _pDic(pTenant->pObject());
  m_oDicStack_ssn.vPush(pDic);

  //fprintf(stderr,"eInit E\n");
  
  // /tenants/boray/catalogs
  Word* pCatalogs = m_oDicStack_ssn.pFindInCurrent("catalogs"_Stz);
  RASSERT(pCatalogs);

  pDic = _pDic(pCatalogs->pObject());
  m_oDicStack_ssn.vPush(pDic);

  //fprintf(stderr,"eInit F\n");
  
  if ( ! szCatalogName)
    return EC_NONE;
  
  // /tenants/boray/catalogs/rapidsse
  Word* pCatalog = m_oDicStack_ssn.pFindInCurrent(szCatalogName);
  if ( ! pCatalog)
    return EC_NO_SUCH_CATALOG;
  
  pDic = _pDic(pCatalog->pObject());
  m_oDicStack_ssn.vPush(pDic);

  //fprintf(stderr,"eInit G\n");
  
  // /tenants/boray/catalogs/rapidsse/schemas
  Word* pSchemas = m_oDicStack_ssn.pFindInCurrent("schemas"_Stz);
  RASSERT(pSchemas);
  
  //fprintf(stderr,"eInit H\n");
  pDic = _pDic(pSchemas->pObject());
  //fprintf(stderr,"eInit I\n");
  
  m_oDicStack_ssn.vPush(pDic);

  if ( ! szSchemaName )
    return EC_NONE;
  
  //fprintf(stderr,"eInit J\n");
  
  // /tenants/boray/catalogs/rapidsse/schemas/public
  Word* pSchema = m_oDicStack_ssn.pFindInCurrent(szSchemaName);
  if ( ! pSchema)
    return EC_NO_SUCH_SCHEMA;
  
  pDic = _pDic(pSchema->pObject());
  m_oDicStack_ssn.vPush(pDic);

  //fprintf(stderr,"eInit K\n");
  
  // /tenants/boray/catalogs/rapidsse/schemas/public/tables
  Word* pTables = m_oDicStack_ssn.pFindInCurrent("tables"_Stz);
  RASSERT(pTables);
  
  //fprintf(stderr,"eInit L\n");
  
  pDic = _pDic(pTables->pObject());
  m_oDicStack_ssn.vPush(pDic);

  //fprintf(stderr,"eInit M\n");
  
  return EC_NONE;
}

/*----------------------------------------------------------------------------*/
/* last job run                                                               */
/*----------------------------------------------------------------------------*/
JobId Session::tLastJobId(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  return m_tLastJobId_ssn;
}

/*----------------------------------------------------------------------------*/
/* outline                                                                    */
/*----------------------------------------------------------------------------*/
void Session::vOutline(s4K& sOutline)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  sOutline.vSprint("<%s focal>"_Stz,aCN[eGetClassNo()]);
}

/*----------------------------------------------------------------------------*/
/* get path stack                                                             */
/*----------------------------------------------------------------------------*/
DicStack* Session::pPathStack(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  return &m_oDicStack_ssn;
}

/*----------------------------------------------------------------------------*/
/* pop cascade                                                                */
/*----------------------------------------------------------------------------*/
void Session::vPopCascade(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
    
  Statement* pStatement = m_pStmtList->pPop();

  Stz* szText = pStatement->szText();
  //vTrace("vPopCascade: %s\n"_Stz,szText);

  delete pStatement;
}

/*----------------------------------------------------------------------------*/
/* see this session                                                           */
/*----------------------------------------------------------------------------*/
void Session::vSee(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SESSION,_FOOTER_);
  
  vTrace("See Session::\n"_Stz);
  
  vTrace("  this %p\n"_Stz
         ,this);
  
  vTrace("  pid  %lld (%s)\n"_Stz
         ,m_iPid
         ,Process::bTestPid(m_iPid) ? "exists"_Stz : "non-existant"_Stz
        );
  if (m_bSystemCloser)
    vTrace("  system closer\n"_Stz);

  vTrace(";;\n\n"_Stz);
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
         "     %s apiError %s time %lldus\n"
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
/* compare two sessions by their sessionId                                    */
/*----------------------------------------------------------------------------*/
bool STORE::bSsnCompare_id(void* aSsnInsert, void* aSsnScan, bool& bMatch)
{
  Session* pSsnInsert = _pSession(aSsnInsert);
  Session* pSsnScan   = _pSession(aSsnScan);
    
  if (! pSsnScan)
    return false;
  
  // do the id's match?
  if (pSsnInsert->m_xSessionId == pSsnScan->m_xSessionId)
    bMatch = true;
  else
    bMatch = false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // is this job to be inserted lexicography after the scan?
  
  if (pSsnInsert->m_xSessionId > pSsnScan->m_xSessionId)
    return true;
  else
    return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/

