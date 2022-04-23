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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
