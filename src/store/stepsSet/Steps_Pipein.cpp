/*============================================================================*/
/* Steps_RegPipein.cpp -- Register Pipein step group                          */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eYX.h"

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
  vAddStep("PipeIn[].isBuffer"_Stz,       &Step::vPipeinIsBuffer);
  vAddStep("PipeIn[].isNull"_Stz,         &Step::vPipeinIsNull);
  vAddStep("PipeIn[].kind"_Stz,           &Step::vPipeinKind);
  vAddStep("PipeIn[].next"_Stz,           &Step::vPipeinNext);
  vAddStep("PipeIn[].peek"_Stz,           &Step::vPipeinPeek);
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
  
  Operand* pOperand;
  
  // revised job/op model
  Op* pOp = *m_pOp;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  if (m_pThread->eDedicated()) {
        
    while ( ! (pOperand = pOp->pPopOoOLeft()) ) {
      vDropTaskLock((Stz*)__func__);
      m_pThread->pPsxThd()->vSleepMsec(10,CN_TASK);

    }

    m_pLogicUnit->vSetPipein(pOperand);
    
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  else {
    
    pOperand = pOp->pPopOoOLeft();
    m_pLogicUnit->vSetPipein(pOperand);
    
    if (m_pLogicUnit->m_xpPipein.bIsNull()) {
      RASSERT(bIsLocked());
      vScheduleBide();
    }
    
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].bideNext -- get next operand (after biding)                       */
/*----------------------------------------------------------------------------*/
void Step::vPipeinBideNext(void)
{
  ENTRY();
 
  Operand* pOperand;
  
  // revised job/op model
    Op* pOp = *m_pOp;

  if (m_pLogicUnit->m_xpPipein.bIsNull()) {
    
    if ( (pOperand = pOp->pPopOoOLeft()) ) {
      m_pLogicUnit->vSetPipein(pOperand);
    }
    else {
      NEVER_REACH;
    }
    
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].delete -- operand delete                                          */
/*----------------------------------------------------------------------------*/
void Step::vPipeinDelete(void)
{
  ENTRY();
  
  AM::vDeleteOperand(*m_pLogicUnit->m_xpPipein);
  
  m_pLogicUnit->m_xpPipein = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].get -- fetch operand                                              */
/*----------------------------------------------------------------------------*/
void Step::vPipeinGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpPipein);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[]\clear -- fetch operand and clear pipein[]                         */
/*----------------------------------------------------------------------------*/
void Step::vPipeinGetAndClear(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpPipein);
  
  m_pLogicUnit->m_xpPipein = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].isBuffer -- is operand buffer ?                                   */
/*----------------------------------------------------------------------------*/
void Step::vPipeinIsBuffer(void)
{
  ENTRY();

  Operand* pOperand = *m_pLogicUnit->m_xpPipein;

  m_pDataStack->vPushInt(pOperand->bIsBuffer() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].isNull -- is operand null ?                                       */
/*----------------------------------------------------------------------------*/
void Step::vPipeinIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpPipein.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].kind -- decode operand into its operand kind                      */
/*----------------------------------------------------------------------------*/
void Step::vPipeinKind(void)
{
  ENTRY();
    
  eZK ePipeInOperand = m_pLogicUnit->m_ePipeInOperand;
  m_pDataStack->vPushInt(ePipeInOperand);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].next -- get next operand                                          */
/*----------------------------------------------------------------------------*/
void Step::vPipeinNext(void)
{
  ENTRY();
  
  RASSERT(m_pLogicUnit->m_xpPipein.bIsNull());
      
  // revised job/op model
    Op* pOp = *m_pOp;

  Operand* pOperand = pOp->pPopOoOLeft();
  
  m_pLogicUnit->vSetPipein(pOperand);
  
  RASSERT(m_pLogicUnit->m_xpPipein.bIsNotNull());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].peek -- peek next operand                                         */
/*----------------------------------------------------------------------------*/
void Step::vPipeinPeek(void)
{
  ENTRY();
  
  // revised job/op model
    Op* pOp = *m_pOp;

  m_pDataStack->vPushInt(pOp->bPeekLeft() ? true : false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].pipeToOutput -- append operand onto the pipeline job buffer queue */
/*----------------------------------------------------------------------------*/
void Step::vPipeinPipeToOutput(void)
{
  ENTRY();
  
  ASSERT(*m_pLogicUnit->m_xpPipein);
  
  // revised job/op model
    Op* pOp = *m_pOp;

  pOp->vPipeToOutput(*m_pLogicUnit->m_xpPipein);
  
  m_pLogicUnit->m_xpPipein = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].set -- set operand                                                */
/*----------------------------------------------------------------------------*/
void Step::vPipeinSet(void)
{
  ENTRY();
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  ASSERT(! pOperand || m_pLogicUnit->m_xpPipein.bIsNull());
  
  ASSERT(! pOperand || ! pOperand->pNextItem());
  ASSERT(! pOperand || ! pOperand->pPrevItem());

  
  m_pLogicUnit->m_xpPipein = _pOperand(pOperand);
  
  if (m_pLogicUnit->m_xpPipein.bIsNull()) {
    m_pLogicUnit->m_ePipeInOperand = ZK_NONE;
  }
  else {
    eZK eOperand = (eZK)AM::iOperandZkKind(pOperand);
    m_pLogicUnit->m_ePipeInOperand = eOperand;
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].writeProfile -- write profile to file as csv                      */
/*----------------------------------------------------------------------------*/
void Step::vPipeinWriteProfile(void)
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
