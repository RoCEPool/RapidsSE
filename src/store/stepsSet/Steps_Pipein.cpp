/*============================================================================*/
/* Steps_RegPipein.cpp -- Register Pipein step group                          */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "ePX.h"

#include "Op.hpp"
#include "Job.hpp"
#include "Node_I.hpp"
#include "LogicUnit.hpp"
#include "StageSpec.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Pipein(void)
{
  vAddStep("PipeIn[]"_Stz,                &Step::vPipeinGet);
  vAddStep("PipeIn[]\\clear"_Stz,         &Step::vPipeinGetAndClear);
  vAddStep("PipeIn[].bide"_Stz,           &Step::vPipeinBide);
  vAddStep("PipeIn[].bideNext"_Stz,       &Step::vPipeinBideNext);
  vAddStep("PipeIn[].delete"_Stz,         &Step::vPipeinDelete);
  vAddStep("PipeIn[].isNull"_Stz,         &Step::vPipeinIsNull);
  vAddStep("PipeIn[].kind"_Stz,           &Step::vPipeinKind);
  vAddStep("PipeIn[].next"_Stz,           &Step::vPipeinNext);
  vAddStep("PipeIn[].peek"_Stz,           &Step::vPipeinPeek);
  vAddStep("PipeIn[].pipeOut"_Stz,        &Step::vPipeinPipeOut);
  vAddStep("PipeIn[].pipeToOutput"_Stz,   &Step::vPipeinPipeToOutput);
  vAddStep("PipeIn[].set"_Stz,            &Step::vPipeinSet);
  vAddStep("PipeIn[].writeProfile"_Stz,   &Step::vPipeinWriteProfile);
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].bide -- pipein operand bide                                       */
/*----------------------------------------------------------------------------*/
void Step::vPipeinBide(void)
{
  ENTRY();
  
  m_pJob->pLogicUnit_job()->m_xpPipein = m_pJob->m_pOp->pPopQueue();
  
  if (m_pJob->pLogicUnit_job()->m_xpPipein.bIsNull()) {
    vScheduleBide();
  }
  else {
    Operand* pOperand = * m_pJob->pLogicUnit_job()->m_xpPipein;
    
    eZK eOperand = (eZK)AM::iOperandZkKind(pOperand);
    m_pJob->pLogicUnit_job()->m_ePipeInOperand = eOperand;
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].bideNext -- get next operand (after biding)                       */
/*----------------------------------------------------------------------------*/
void Step::vPipeinBideNext(void)
{
  ENTRY();
    
  if (m_pJob->pLogicUnit_job()->m_xpPipein.bIsNull()) {
    
    Operand* pOperand = m_pJob->m_pOp->pPopQueue();
    m_pJob->pLogicUnit_job()->m_xpPipein = pOperand;
    
    eZK eOperand = (eZK)AM::iOperandZkKind(pOperand);
    m_pJob->pLogicUnit_job()->m_ePipeInOperand = eOperand;
  }
  
  RASSERT(*m_pJob->pLogicUnit_job()->m_xpPipein);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].delete -- operand delete                                          */
/*----------------------------------------------------------------------------*/
void Step::vPipeinDelete(void)
{
  ENTRY();
  
  AM::vDeleteOperand(*m_pJob->pLogicUnit_job()->m_xpPipein);
  
  m_pJob->pLogicUnit_job()->m_xpPipein = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].get -- fetch operand                                              */
/*----------------------------------------------------------------------------*/
void Step::vPipeinGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pJob->pLogicUnit_job()->m_xpPipein);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[]\clear -- fetch operand and clear pipein[]                         */
/*----------------------------------------------------------------------------*/
void Step::vPipeinGetAndClear(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pJob->pLogicUnit_job()->m_xpPipein);
  
  m_pJob->pLogicUnit_job()->m_xpPipein = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].isNull -- is operand null ?                                       */
/*----------------------------------------------------------------------------*/
void Step::vPipeinIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pJob->pLogicUnit_job()->m_xpPipein.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].kind -- decode operand into its operand kind                      */
/*----------------------------------------------------------------------------*/
void Step::vPipeinKind(void)
{
  ENTRY();
    
  eZK ePipeInOperand = m_pJob->pLogicUnit_job()->m_ePipeInOperand;
  m_pDataStack->vPushInt(ePipeInOperand);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].next -- get next operand                                          */
/*----------------------------------------------------------------------------*/
void Step::vPipeinNext(void)
{
  ENTRY();
  
  RASSERT(m_pJob->pLogicUnit_job()->m_xpPipein.bIsNull());
      
  Operand* pOperand = m_pJob->m_pOp->pPopQueue();
  m_pJob->pLogicUnit_job()->m_xpPipein = pOperand;
  
  eZK eOperand = (eZK)AM::iOperandZkKind(pOperand);
  m_pJob->pLogicUnit_job()->m_ePipeInOperand = eOperand;
  
  RASSERT(m_pJob->pLogicUnit_job()->m_xpPipein.bIsNotNull());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].peek -- peek next operand                                         */
/*----------------------------------------------------------------------------*/
void Step::vPipeinPeek(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pJob->m_pOp->bPeekQueue() ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].pipeOut -- append operand onto next operator in pipeline          */
/*----------------------------------------------------------------------------*/
void Step::vPipeinPipeOut(void)
{
  ENTRY();
  
  ASSERT(*m_pJob->pLogicUnit_job()->m_xpPipein);
  
  Op* pOp = m_pJob->m_pOp; //  pJob_Distributed->m_pOp;

  pOp->vPipeOut_nextOp(*m_pJob->pLogicUnit_job()->m_xpPipein,this);
  
  m_pJob->pLogicUnit_job()->m_xpPipein = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].pipeToOutput -- append operand onto the pipeline job buffer queue */
/*----------------------------------------------------------------------------*/
void Step::vPipeinPipeToOutput(void)
{
  ENTRY();
  
  ASSERT(*m_pJob->pLogicUnit_job()->m_xpPipein);
  
  Op* pOp = m_pJob->m_pOp; //  pJob_Distributed->m_pOp;
  
  pOp->vPipeToOutput(*m_pJob->pLogicUnit_job()->m_xpPipein);
  
  m_pJob->pLogicUnit_job()->m_xpPipein = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].set -- set operand                                                */
/*----------------------------------------------------------------------------*/
void Step::vPipeinSet(void)
{
  ENTRY();
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  ASSERT(! pOperand || m_pJob->pLogicUnit_job()->m_xpPipein.bIsNull());
  
  ASSERT(! pOperand || ! pOperand->pNextItem());
  ASSERT(! pOperand || ! pOperand->pPrevItem());

  
  m_pJob->pLogicUnit_job()->m_xpPipein = _pOperand(pOperand);
  
  if (m_pJob->pLogicUnit_job()->m_xpPipein.bIsNull()) {
    m_pJob->pLogicUnit_job()->m_ePipeInOperand = ZK_NONE;
  }
  else {
    eZK eOperand = (eZK)AM::iOperandZkKind(pOperand);
    m_pJob->pLogicUnit_job()->m_ePipeInOperand = eOperand;
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].writeProfile -- write profile to file as csv                      */
/*----------------------------------------------------------------------------*/
void Step::vPipeinWriteProfile(void)
{
  ENTRY();
  
  ASSERT(*m_pJob->pLogicUnit_job()->m_xpPipein);
  
  Profile* pProfile = _pProfile(*m_pJob->pLogicUnit_job()->m_xpPipein);
  
  File* pFile = _pFile(*m_pJob->pLogicUnit_job()->m_xpFile0);
  
  pProfile->vWriteReport(pFile);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
