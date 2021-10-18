/*============================================================================*/
/* Steps_Session.cpp - Session step group                                     */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "Session.hpp"
#include "Sessions.hpp"
#include "Job.hpp"

#include "MY.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Session(void)
{
  vAddStep("session::this"_Stz,          &Step::vSessionThis);
  
  vAddStep("session::cascade@"_Stz,      &Step::vSessionCascadeFetch);
  vAddStep("session::cascade!"_Stz,      &Step::vSessionCascadeStore);
  vAddStep("session::dicStack@"_Stz,     &Step::vSessionDicStackFetch);
  vAddStep("session::dump"_Stz,          &Step::vSessionDump);
  vAddStep("session::hasCascade@"_Stz,   &Step::vSessionHasCascadeFetch);
  vAddStep("session::hasCascade!"_Stz,   &Step::vSessionHasCascadeStore);
  vAddStep("session::pid"_Stz,           &Step::vSessionPid);
  vAddStep("session::see"_Stz,           &Step::vSessionSee);
  vAddStep("session::systemCloser@"_Stz, &Step::vSessionSystemCloserFetch);
  vAddStep("session::systemCloser!"_Stz, &Step::vSessionSystemCloserStore);
  vAddStep("session::tablePtn@"_Stz,     &Step::vSessionTablePtnFetch);
  vAddStep("session::tagSet@"_Stz,       &Step::vSessionTagSetFetch);
  vAddStep("session::todo@"_Stz,         &Step::vSessionToDoFetch);
  vAddStep("session::todo!"_Stz,         &Step::vSessionToDoStore);
  vAddStep("session::tx@"_Stz,           &Step::vSessionTxFetch);
  vAddStep("session::tx!"_Stz,           &Step::vSessionTxStore);
  
  vAddStep("session[].signal"_Stz,       &Step::vSessionSignal);

}

/*----------------------------------------------------------------------------*/
/* session::cascade@ -- fetch sCascade from session                           */
/*----------------------------------------------------------------------------*/
void Step::vSessionCascadeFetch(void)
{
  ENTRY();
  
  Session* pSession = MY::pSession();

  m_pDataStack->vPushStz(pSession->szCascade());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ssession::cascade! -- store sCascade into session                          */
/*----------------------------------------------------------------------------*/
void Step::vSessionCascadeStore(void)
{
  ENTRY();
  
  Session* pSession = MY::pSession();

  s16KB sStmt;
  m_pDataStack->vPopStr(sStmt,__func__);
  
  pSession->vCascade(sStmt);
  pSession->vAppendCascade(sStmt);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.dicStack@ -- get session dic stack                                 */
/*----------------------------------------------------------------------------*/
void Step::vSessionDicStackFetch(void)
{
  ENTRY();
  
  Session* pSession = MY::pSession();

  m_pDataStack->vPushPtr(pSession->pPathStack());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.dump -- dump of SE interface                                       */
/*----------------------------------------------------------------------------*/
void Step::vSessionDump(void)
{
  ENTRY();
  
  Session* pSession = MY::pSession();

  pSession->vContentDump();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session::hasCascade@ -- fetch bHasCascade from session                     */
/*----------------------------------------------------------------------------*/
void Step::vSessionHasCascadeFetch(void)
{
  ENTRY();
  
  Session* pSession = MY::pSession();

  m_pDataStack->vPushInt(pSession->bHasCascade());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ssession::hasCascade! -- store bHasCascade into session                    */
/*----------------------------------------------------------------------------*/
void Step::vSessionHasCascadeStore(void)
{
  ENTRY();
  
  Session* pSession = MY::pSession();

  i8 iCascade = m_pDataStack->iPopInt(__func__);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.pid -- get pid of SE session                                       */
/*----------------------------------------------------------------------------*/
void Step::vSessionPid(void)
{
  ENTRY();
  
  Session* pSession = MY::pSession();

  m_pDataStack->vPushInt(pSession->m_iPid);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session::see -- see session                                                */
/*----------------------------------------------------------------------------*/
void Step::vSessionSee(void)
{
  ENTRY();
  
  Session* pSession = MY::pSession();

  pSession->vSee();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session::systemCloser@ -- fetch m_bSystemCloser from session               */
/*----------------------------------------------------------------------------*/
void Step::vSessionSystemCloserFetch(void)
{
  ENTRY();
  
  Session* pSession = MY::pSession();

  m_pDataStack->vPushInt(pSession->m_bSystemCloser);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ssession::systemCloser! -- store m_bSystemCloser into session              */
/*----------------------------------------------------------------------------*/
void Step::vSessionSystemCloserStore(void)
{
  ENTRY();
  
  Session* pSession = MY::pSession();

  pSession->m_bSystemCloser = m_pDataStack->iPopInt(__func__);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.tablePtn@ -- fetch tablePtn of SE session                           */
/*----------------------------------------------------------------------------*/
void Step::vSessionTablePtnFetch(void)
{
  ENTRY();
  
  Session* pSession = MY::pSession();

  m_pDataStack->vPushInt(pSession->i_FixedPtn());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.tagSet@ -- fetch tagset of session                                 */
/*----------------------------------------------------------------------------*/
void Step::vSessionTagSetFetch(void)
{
  ENTRY();
  
  Session* pSession = MY::pSession();

  TagSet* pSsnTagSet = & pSession->m_oSsnTagSet;
  
  m_pDataStack->vPushPtr(pSsnTagSet);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session::this -- get current session                                       */
/*----------------------------------------------------------------------------*/
void Step::vSessionThis(void)
{
  ENTRY();
 
  Session* pSession = MY::pSession();

  m_pDataStack->vPushPtr(pSession);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.todo@ -- fetch todo of SE session                                  */
/*----------------------------------------------------------------------------*/
void Step::vSessionToDoFetch(void)
{
  ENTRY();
  
  Session* pSession = MY::pSession();

  m_pDataStack->vPushStr(pSession->m_sToDo);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.toDo! -- store todo of SE session                                  */
/*----------------------------------------------------------------------------*/
void Step::vSessionToDoStore(void)
{
  ENTRY();
  
  Session* pSession = MY::pSession();

  s4K sToDo;
  m_pDataStack->vPopStr(sToDo,__func__);

  pSession->m_sToDo = sToDo;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.tx@ -- fetch tx of SE session                                      */
/*----------------------------------------------------------------------------*/
void Step::vSessionTxFetch(void)
{
  ENTRY();
  
  Session* pSession = MY::pSession();

  Tx* pTx = pSession->pTx();
  
  m_pDataStack->vPushPtr(pTx);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session.tx! -- store tx of SE session                                      */
/*----------------------------------------------------------------------------*/
void Step::vSessionTxStore(void)
{
  ENTRY();
  
  Session* pSession = MY::pSession();

  Tx* pTx = _pTx(m_pDataStack->pPopPtr(__func__));
  
  pSession->vTx(pTx);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* session[].signal -- signal the waiting session                             */
/*----------------------------------------------------------------------------*/
void Step::vSessionSignal(void)
{
  ENTRY();
  
  // there will be a thread waiting on completion of the job
  // this could be a prompt thread or a java thread via the connector
  
  Job* pJob = MY::pJob();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//  vGrabTaskLock((Stz*)__func__);
  
  //pJob->vJobState(JS_SIGNALLED);

  i8 i_Session = pJob->m_i_Session;
  
  Session* pSession = AM::pSessions()->pGetSession(i_Session);
  Sessions::vMarkSignalled(pSession);

    
  pSession->m_oMutex.vLock(this);
  //vTrace("SIGNAL session %p mutex %p\n"_Stz,pSession,&pSession->m_oMutex);
  pSession->m_oMutex.vSignal(this);
  pSession->m_oMutex.vUnlock(this);

//  vDropTaskLock((Stz*)__func__);


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  //vScheduleCompletion();
  

  
  EXIT();
}



/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
