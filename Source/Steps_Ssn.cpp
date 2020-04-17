/*============================================================================*/
/* Steps_SsnState.cpp - SsnState step group                                   */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_SsnState(void)
{
  vAddStep("ssn::incBegun"_Stz,     &Step::vSsnIncBegun);
  vAddStep("ssn::incCancelled"_Stz, &Step::vSsnIncCancelled);
  vAddStep("ssn::incEnded"_Stz,     &Step::vSsnIncEnded);
  vAddStep("ssn::incExecuted"_Stz,  &Step::vSsnIncExecuted);
  vAddStep("ssn::senLoc"_Stz,       &Step::vSsnSenLoc);
  vAddStep("ssn::traceSession"_Stz, &Step::vSsnTraceSession);
}

/*----------------------------------------------------------------------------*/
/* ssn::incBegun -- increment the number of __begun received                  */
/*----------------------------------------------------------------------------*/
void Step::vSsnIncBegun(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 i_Session = pJob->m_xReqTuple.m_i_Session;
  
  Session* pSession = AM::pSessionArray()->pGetSession(i_Session);
  
  SessionArray::vMarkIncBegun(pSession);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ssn::incCancelled -- increment the number of __cancelled received          */
/*----------------------------------------------------------------------------*/
void Step::vSsnIncCancelled(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 i_Session = pJob->m_xReqTuple.m_i_Session;
  
  Session* pSession = AM::pSessionArray()->pGetSession(i_Session);

  SessionArray::vMarkIncCancelled(pSession);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ssn::incEnded -- increment the number of __ended received                  */
/*----------------------------------------------------------------------------*/
void Step::vSsnIncEnded(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 i_Session = pJob->m_xReqTuple.m_i_Session;
  
  Session* pSession = AM::pSessionArray()->pGetSession(i_Session);

  SessionArray::vMarkIncEnded(pSession);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ssn::incExecuted-- increment the number of __executed received             */
/*----------------------------------------------------------------------------*/
void Step::vSsnIncExecuted(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 i_Session = pJob->m_xReqTuple.m_i_Session;
  
  Session* pSession = AM::pSessionArray()->pGetSession(i_Session);

  SessionArray::vMarkIncExecuted(pSession);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ssn::senLoc -- set last reported sen location                              */
/*----------------------------------------------------------------------------*/
void Step::vSsnSenLoc(void)
{
  ENTRY();
  
  i8 iPosition = m_pDataStack->iPopInt(__func__);

  //  Session* pSession = m_i_Session->m_pSession;
  //  ApiState::vMarkSenLoc(pSession,iPosition);
  
  //! need to pass m_i_Session
  //  ApiState::vMarkSenLoc(m_i_Session->m_i_Session,iPosition);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ssn::traceSession -- trace the api state for this session                  */
/*----------------------------------------------------------------------------*/
void Step::vSsnTraceSession(void)
{
  ENTRY();
  
  Job* pJob = _pJob(m_pDataStack->pPopPtr(__func__));
  
  i8 i_Session = pJob->m_xReqTuple.m_i_Session;
  
  Session* pSession = AM::pSessionArray()->pGetSession(i_Session);
  
  //pSession->vTraceState();
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
