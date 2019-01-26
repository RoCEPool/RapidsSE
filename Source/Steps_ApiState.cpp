/*============================================================================*/
/* Steps_ApiState.cpp - AoiState step group                                   */
/*============================================================================*/

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ApiState(void)
{
  vAddStep("apiState::incBegun"_Stz,     &Step::vApiStateIncBegun);
  vAddStep("apiState::incCancelled"_Stz, &Step::vApiStateIncCancelled);
  vAddStep("apiState::incEnded"_Stz,     &Step::vApiStateIncEnded);
  vAddStep("apiState::incExecuted"_Stz,  &Step::vApiStateIncExecuted);
  vAddStep("apiState::setPosition"_Stz,  &Step::vApiStateSetPosition);
  vAddStep("apiState::traceSession"_Stz, &Step::vApiStateTraceSession);
}

/*----------------------------------------------------------------------------*/
/* c -- increment the number of __begun received             */
/*----------------------------------------------------------------------------*/
void Step::vApiStateIncBegun(MP_Task mpTask)
{
  ENTRY("vApiIncBegun"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("apiState::incBegun"_Stz);
  
  Job* pJob = _pJob(pJobPtr);
  
  i8 i_Session = pJob->m_iSession;
  
  gpAttachMap->m_pApiState->vIncBegun(i_Session);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* apiState::incCancelled -- increment the number of __cancelled received     */
/*----------------------------------------------------------------------------*/
void Step::vApiStateIncCancelled(MP_Task mpTask)
{
  ENTRY("vApiIncBegun"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("apiState::incBegun"_Stz);
  
  Job* pJob = _pJob(pJobPtr);
  
  i8 i_Session = pJob->m_iSession;

  gpAttachMap->m_pApiState->vIncCancelled(i_Session);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* apiState::incEnded -- increment the number of __ended received             */
/*----------------------------------------------------------------------------*/
void Step::vApiStateIncEnded(MP_Task mpTask)
{
  ENTRY("vApiIncBegun"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("apiState::incBegun"_Stz);
  
  Job* pJob = _pJob(pJobPtr);
  
  i8 i_Session = pJob->m_iSession;

  gpAttachMap->m_pApiState->vIncEnded(i_Session);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* apiState::incExecuted-- increment the number of __executed received        */
/*----------------------------------------------------------------------------*/
void Step::vApiStateIncExecuted(MP_Task mpTask)
{
  ENTRY("vApiIncExecuted"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("apiState::incBegun"_Stz);
  
  Job* pJob = _pJob(pJobPtr);
  
  i8 i_Session = pJob->m_iSession;

  gpAttachMap->m_pApiState->vIncExecuted(i_Session);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* apiState::setPosition -- set last reported position                        */
/*----------------------------------------------------------------------------*/
void Step::vApiStateSetPosition(MP_Task mpTask)
{
  ENTRY("vApiStateSetPosition"_Stz);
  
  i8 iPosition;
  if ( ! mpTask->m_pDataStack->bPop(iPosition) )
    THROW_INVALID_DSTACK("apiState::setPosition"_Stz);
  
  Session* pSession = mpTask->m_pSession;
  
  gpAttachMap->m_pApiState->vSenLoc(pSession,iPosition);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* apiState::traceSession -- trace the api state for this session             */
/*----------------------------------------------------------------------------*/
void Step::vApiStateTraceSession(MP_Task mpTask)
{
  ENTRY("vApiStateTraceSession"_Stz);
  
  Object* pJobPtr;
  if ( ! mpTask->m_pDataStack->bPop(pJobPtr) )
    THROW_INVALID_DSTACK("apiState::traceSession"_Stz);
  
  Job* pJob = _pJob(pJobPtr);
  
  i8 i_Session = pJob->m_iSession;
  SessionState* pState = &gpAttachMap->m_pApiState->m_oSessionState[i_Session];
  
  pState->vTraceState();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
