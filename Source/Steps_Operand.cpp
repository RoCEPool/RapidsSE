/*============================================================================*/
/* Steps_Operand.cpp -- Operand step group                                    */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Operand(void)
{
  vAddStep("Operand[].bide"_Stz,      &Step::vOperandBide);
  vAddStep("Operand[].handleEnd"_Stz, &Step::vOperandHandleEnd);
  vAddStep("Operand[].kind"_Stz,      &Step::vOperandKind);
  vAddStep("Operand[].next"_Stz,      &Step::vOperandNext);
  vAddStep("Operand[].pipeOut"_Stz,   &Step::vOperandPipeOut);
  vAddStep("Operand[].workDone"_Stz,  &Step::vOperandWorkDone);
}

/*----------------------------------------------------------------------------*/
/* Operand[].bide -- operand bide                                             */
/*----------------------------------------------------------------------------*/
void Step::vOperandBide(void)
{
  ENTRY("vOperandBide"_Stz);
  
  m_pRegSet->m_pOperand = m_pTask->m_pJob->m_pOp->pPopQueue2();
  
  if (! m_pRegSet->m_pOperand) {
    ASSERT(m_pTask->bIsLocked());
    m_pTask->vSchedulerAct(SA_BIDE);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].handleEnd -- handle end marker                                   */
/*----------------------------------------------------------------------------*/
void Step::vOperandHandleEnd(void)
{
  ENTRY("vOperandHandleEnd"_Stz);
  
  ASSERT(m_pRegSet->m_pOperand);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Universal->m_pOp;
  
  if (--pOp->m_nLeftEndMarkers == 0) {
    if (pOp->bIsCancelling()) {
      delete _pEndMarker(m_pRegSet->m_pOperand);
      pOp->vPipeToJob(new CancelMarker);
    }
    else
      pOp->vPipeToJob(m_pRegSet->m_pOperand);
    
    if (AM::bJobHasError(pOp->m_pJobUniversal)) {
      AM::vSetJobLocalError(pOp->m_pJobUniversal, pOp->m_pPipeline->pJob());
    }
    if (pOp->m_bAckJobEnd)
      pOp->vAckJob();
    
  }
  else
    delete (EndMarker*)m_pRegSet->m_pOperand;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].kind -- decode operand into its operand kind                     */
/*----------------------------------------------------------------------------*/
void Step::vOperandKind(void)
{
  ENTRY("vOperandKind"_Stz);
  
  eZK eOperand = ZK_NONE;
  
  eCN eClass_op = m_pRegSet->m_pOperand->eClass();
  switch (eClass_op) {
      
    case CN_CRWBUFFER: eOperand = ZK_CRWBUFFER; break;
    case CN_CSVBUFFER: eOperand = ZK_CSVBUFFER; break;
    case CN_ENDMARKER: eOperand = ZK_ENDMARKER; break;
    case CN_ENTBUFFER: eOperand = ZK_ENTBUFFER; break;
    case CN_IRWBUFFER: eOperand = ZK_IRWBUFFER; break;
    case CN_SRTBUFFER: eOperand = ZK_SRTBUFFER; break;
    case CN_STREAMDEF: eOperand = ZK_STREAMDEF; break;
      
    default:
      RASSERT(0);
      break;
  }
  
  m_pDataStack->vPushI8(eOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].next -- get next operand                                         */
/*----------------------------------------------------------------------------*/
void Step::vOperandNext(void)
{
  ENTRY("vOperandNext"_Stz);
  
  if (! m_pRegSet->m_pOperand)
    m_pRegSet->m_pOperand = m_pTask->m_pJob->m_pOp->pPopQueue2();
  ASSERT(m_pRegSet->m_pOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].pipeOut -- append operand onto output pipe                       */
/*----------------------------------------------------------------------------*/
void Step::vOperandPipeOut(void)
{
  ENTRY("vOperandPipeOut"_Stz);
  
  ASSERT(m_pRegSet->m_pOperand);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Universal->m_pOp;

  pOp->vPipeToJob(m_pRegSet->m_pOperand);
  
  m_pRegSet->m_pOperand = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].workDone -- update work done                                     */
/*----------------------------------------------------------------------------*/
void Step::vOperandWorkDone(void)
{
  ENTRY("vOperandWorkDone"_Stz);
  
  ASSERT(m_pRegSet->m_pOperand);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Universal->m_pOp;
  
  eCN eClass_op = m_pRegSet->m_pOperand->eClass();
  
  switch (eClass_op) {
      
    case CN_CRWBUFFER: {
      CrwBuffer* pCrwBuffer = _pCrwBuffer(m_pRegSet->m_pOperand);
      pOp->m_iWorkDone += pCrwBuffer->nRowCount();
      break;
    }
      
    case CN_CSVBUFFER: {
      CsvBuffer* pCsvBuffer = _pCsvBuffer(m_pRegSet->m_pOperand);
      pOp->m_iWorkDone += pCsvBuffer->nRecords();
      break;
    }
      
    case CN_ENDMARKER: {
      RASSERT(0);
      break;
    }
      
    case CN_ENTBUFFER: {
      EntBuffer* pEntBuffer = _pEntBuffer(m_pRegSet->m_pOperand);
      pOp->m_iWorkDone += pEntBuffer->nItems();
      break;
    }
      
    case CN_IRWBUFFER: {
      IrwBuffer* pIrwBuffer = _pIrwBuffer(m_pRegSet->m_pOperand);
      pOp->m_iWorkDone += pIrwBuffer->nItems();
      break;
    }
      
    case CN_SRTBUFFER: {
      SrtBuffer* pSrtBuffer = _pSrtBuffer(m_pRegSet->m_pOperand);
      pOp->m_iWorkDone += pSrtBuffer->nItems();
      break;
    }
      
    case CN_STREAMDEF: {
      break;
    }
      
    default:
      RASSERT(0);
      break;
  }

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
