/*============================================================================*/
/* Steps_RegPipein.cpp -- Register Pipein step group                          */
/*============================================================================*/

#include "Steps.h"

#include "File.hpp" //?

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegPipein(void)
{
  vAddStep("PipeIn[]"_Stz,                &Step::vRegPipeinGet);
  vAddStep("PipeIn[].bide"_Stz,           &Step::vRegPipeinBide);
  vAddStep("PipeIn[].delete"_Stz,         &Step::vRegPipeinDelete);
  vAddStep("PipeIn[].isNull"_Stz,         &Step::vRegPipeinIsNull);
  vAddStep("PipeIn[].kind"_Stz,           &Step::vRegPipeinKind);
  vAddStep("PipeIn[].next"_Stz,           &Step::vRegPipeinNext);
  vAddStep("PipeIn[].peek"_Stz,           &Step::vRegPipeinPeek);
  vAddStep("PipeIn[].pipeOut"_Stz,        &Step::vRegPipeinPipeOut);
  vAddStep("PipeIn[].pipeToJob"_Stz,      &Step::vRegPipeinPipeToJob);
  vAddStep("PipeIn[].set"_Stz,            &Step::vRegPipeinSet);
  vAddStep("PipeIn[].writeProfile"_Stz,   &Step::vRegPipeinWriteProfile);
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].bide -- pipein operand bide                                       */
/*----------------------------------------------------------------------------*/
void Step::vRegPipeinBide(void)
{
  ENTRY();
  
  m_pLogicUnit->m_xpPipein = m_pTask->m_pJob->m_pOp->pPopQueue();
  
  if (m_pLogicUnit->m_xpPipein.bIsNull()) {
    m_pTask->vScheduleBide();
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].delete -- operand delete                                          */
/*----------------------------------------------------------------------------*/
void Step::vRegPipeinDelete(void)
{
  ENTRY();
  
  AM::vDeleteOperand(*m_pLogicUnit->m_xpPipein);
  
  m_pLogicUnit->m_xpPipein = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].get -- fetch operand                                              */
/*----------------------------------------------------------------------------*/
void Step::vRegPipeinGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpPipein);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].isNull -- is operand null ?                                       */
/*----------------------------------------------------------------------------*/
void Step::vRegPipeinIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpPipein.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].kind -- decode operand into its operand kind                      */
/*----------------------------------------------------------------------------*/
void Step::vRegPipeinKind(void)
{
  ENTRY();
  
  Operand* pOperand = *m_pLogicUnit->m_xpPipein;
  
  eZK eOperand = (eZK)AM::iOperandZkKind(pOperand);
    
  m_pDataStack->vPushInt(eOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].next -- get next operand                                          */
/*----------------------------------------------------------------------------*/
void Step::vRegPipeinNext(void)
{
  ENTRY();
  
  if (m_pLogicUnit->m_xpPipein.bIsNull())
    m_pLogicUnit->m_xpPipein = m_pTask->m_pJob->m_pOp->pPopQueue();
  ASSERT(*m_pLogicUnit->m_xpPipein);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].peek -- peek next operand                                         */
/*----------------------------------------------------------------------------*/
void Step::vRegPipeinPeek(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pTask->m_pJob->m_pOp->bPeekQueue() ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].pipeOut -- append operand onto next operator in pipeline          */
/*----------------------------------------------------------------------------*/
void Step::vRegPipeinPipeOut(void)
{
  ENTRY();
  
  ASSERT(*m_pLogicUnit->m_xpPipein);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Distributed->m_pOp;

  pOp->vPipeOut_nextOp(*m_pLogicUnit->m_xpPipein,m_pTask);
  
  m_pLogicUnit->m_xpPipein = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].pipeToJob -- append operand onto the pipeline job buffer queue    */
/*----------------------------------------------------------------------------*/
void Step::vRegPipeinPipeToJob(void)
{
  ENTRY();
  
  ASSERT(*m_pLogicUnit->m_xpPipein);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Distributed->m_pOp;
  
  pOp->vPipeToJob(*m_pLogicUnit->m_xpPipein);
  
  m_pLogicUnit->m_xpPipein = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].set -- set operand                                                */
/*----------------------------------------------------------------------------*/
void Step::vRegPipeinSet(void)
{
  ENTRY();
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  ASSERT(! pOperand || m_pLogicUnit->m_xpPipein.bIsNull());
  m_pLogicUnit->m_xpPipein = _pOperand(pOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].writeProfile -- write profile to file as csv                      */
/*----------------------------------------------------------------------------*/
void Step::vRegPipeinWriteProfile(void)
{
  ENTRY();
  
  ASSERT(*m_pLogicUnit->m_xpPipein);
  
  Profile* pProfile = _pProfile(*m_pLogicUnit->m_xpPipein);
  
  File* pFile = _pFile(*m_pLogicUnit->m_xpFile0);
  
  pProfile->vWriteReport(pFile);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
