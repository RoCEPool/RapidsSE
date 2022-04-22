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
  vAddStep("[session].cascade@"_Stz,      &Step::vSessionCascadeFetch);
  vAddStep("[session].cascade!"_Stz,      &Step::vSessionCascadeStore);
  vAddStep("[session].hasCascade@"_Stz,   &Step::vSessionHasCascadeFetch);
  vAddStep("[session].hasCascade!"_Stz,   &Step::vSessionHasCascadeStore);
  vAddStep("[session].lastJobId"_Stz,     &Step::vSessionLastJobId);
  vAddStep("[session].systemCloser@"_Stz, &Step::vSessionSystemCloserFetch);
  vAddStep("[session].systemCloser!"_Stz, &Step::vSessionSystemCloserStore);
  vAddStep("[session].tagSet@"_Stz,       &Step::vSessionTagSetFetch);
}

/*----------------------------------------------------------------------------*/
/* [session].cascade@ -- fetch sCascade from session                           */
/*----------------------------------------------------------------------------*/
void Step::vSessionCascadeFetch(void)
{
  ENTRY();
  
  Session* pSession = MY::pSession();

  m_pDataStack->vPushStz(pSession->szCascade());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* s[session].cascade! -- store sCascade into session                          */
/*----------------------------------------------------------------------------*/
void Step::vSessionCascadeStore(void)
{
  ENTRY();
  
  Session* pSession = MY::pSession();

  s16KB sStmt;
  m_pDataStack->vPopStr(sStmt,__func__);
  
  pSession->vAppendCascade(sStmt);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [session].hasCascade@ -- fetch bHasCascade from session                    */
/*----------------------------------------------------------------------------*/
void Step::vSessionHasCascadeFetch(void)
{
  ENTRY();
  
  Session* pSession = MY::pSession();

  m_pDataStack->vPushInt(pSession->bHasCascade());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [session].hasCascade! -- store bHasCascade into session                    */
/*----------------------------------------------------------------------------*/
void Step::vSessionHasCascadeStore(void)
{
  ENTRY();
  
  Session* pSession = MY::pSession();

  i8 iCascade = m_pDataStack->iPopBol(__func__);
    
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [session].lastJobId -- get last job id                                     */
/*----------------------------------------------------------------------------*/
void Step::vSessionLastJobId(void)
{
  ENTRY();
  
  Session* pSession = MY::pSession();

  JobId tLastJobId = pSession->tLastJobId();
  
  m_pDataStack->vPushInt(tLastJobId.iAsI8());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [session].systemCloser@ -- fetch m_bSystemCloser from session               */
/*----------------------------------------------------------------------------*/
void Step::vSessionSystemCloserFetch(void)
{
  ENTRY();
  
  Session* pSession = MY::pSession();

  m_pDataStack->vPushInt(pSession->m_bSystemCloser);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [session].systemCloser! -- store m_bSystemCloser into session              */
/*----------------------------------------------------------------------------*/
void Step::vSessionSystemCloserStore(void)
{
  ENTRY();
  
  Session* pSession = MY::pSession();

  pSession->m_bSystemCloser = m_pDataStack->iPopInt(__func__);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* [session].tagSet@ -- fetch tagset of session                               */
/*----------------------------------------------------------------------------*/
void Step::vSessionTagSetFetch(void)
{
  ENTRY();
  
  Session* pSession = MY::pSession();

  TagSet* pSsnTagSet = & pSession->m_oSsnTagSet;
  
  m_pDataStack->vPushPtr(pSsnTagSet);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
