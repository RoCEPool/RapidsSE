/*============================================================================*/
/* Steps_Operand.cpp -- Operand step group                                    */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Operand(void)
{
  vAddStep("Operand[].bide"_Stz,         &Step::vOperandBide);
  vAddStep("Operand[].delete"_Stz,       &Step::vOperandDelete);
  vAddStep("Operand[].firstRow@"_Stz,    &Step::vOperandFirstRowFetch);
  vAddStep("Operand[].get"_Stz,          &Step::vOperandGet);
  vAddStep("Operand[].handleAggEnd"_Stz, &Step::vOperandHandleAggEnd);
  vAddStep("Operand[].handleEnd"_Stz,    &Step::vOperandHandleEnd);
  vAddStep("Operand[].isNull"_Stz,       &Step::vOperandIsNull);
  vAddStep("Operand[].itemCount@"_Stz,   &Step::vOperandItemCountFetch);
  vAddStep("Operand[].kind"_Stz,         &Step::vOperandKind);
  vAddStep("Operand[].newEndMarker"_Stz, &Step::vOperandNewEndMarker);
  vAddStep("Operand[].newStreamDef"_Stz, &Step::vOperandNewStreamDef);
  vAddStep("Operand[].next"_Stz,         &Step::vOperandNext);
  vAddStep("Operand[].pipeOut"_Stz,      &Step::vOperandPipeOut);
  vAddStep("Operand[].pipeToJob"_Stz,    &Step::vOperandPipeToJob);
  vAddStep("Operand[].rowCount@"_Stz,    &Step::vOperandRowCountFetch);
  vAddStep("Operand[].set"_Stz,          &Step::vOperandSet);
  vAddStep("Operand[].toCsv"_Stz,        &Step::vOperandToCsv);
  vAddStep("Operand[].workDone"_Stz,     &Step::vOperandWorkDone);
  vAddStep("Operand[].writeToFile"_Stz,  &Step::vOperandWriteToFile);
}

/*----------------------------------------------------------------------------*/
/* Operand[].bide -- operand bide                                             */
/*----------------------------------------------------------------------------*/
void Step::vOperandBide(void)
{
  ENTRY(); // ENTRY("vOperandBide"_Stz);
  
  m_pRegSet->m_pOperand = m_pTask->m_pJob->m_pOp->pPopQueue2();
  
  if (! m_pRegSet->m_pOperand) {
    ASSERT(m_pTask->bIsLocked());
    m_pTask->vSchedulerAct(SA_BIDE);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].delete -- operand delete                                         */
/*----------------------------------------------------------------------------*/
void Step::vOperandDelete(void)
{
  ENTRY(); // ENTRY("vOperandDelete"_Stz);
  
  AM::vDeleteOperand(m_pRegSet->m_pOperand);
  
  m_pRegSet->m_pOperand = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].firstRow@ -- fetch operand first row                             */
/*----------------------------------------------------------------------------*/
void Step::vOperandFirstRowFetch(void)
{
  ENTRY(); // ENTRY("vOperandFirstRowFetch"_Stz);
  
  IrwBuffer* pIrwBuffer = _pIrwBuffer(m_pRegSet->m_pOperand);
  m_pRegSet->m_xpRow = pIrwBuffer->aFirstRow();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].get -- fetch operand                                             */
/*----------------------------------------------------------------------------*/
void Step::vOperandGet(void)
{
  ENTRY(); // ENTRY("vOperandGet"_Stz);
  
  m_pDataStack->vPushPtr(m_pRegSet->m_pOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].handleAggEnd -- handle agg end                                   */
/*----------------------------------------------------------------------------*/
void Step::vOperandHandleAggEnd(void)
{
  ENTRY(); // ENTRY("vOperandHandleAggEnd"_Stz);
  
  ASSERT(m_pRegSet->m_pOperand);
  
  i8 nCount;
  
  if ( ! m_pDataStack->bPopInt(nCount) )
    THROW_DSTACK();
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Universal->m_pOp;
  
  if (--pOp->m_nLeftEndMarkers == 0) {
    if (pOp->bIsCancelling()) {
      delete _pEndMarker(m_pRegSet->m_pOperand);
      pOp->vPipeOut(new CancelMarker);
    }
    else {
      
      // create an IrwBuffer
      IrwBuffer* pIrwBuffer = new IrwBuffer();
      
      // get the columnVec defining the rowref
      ColumnVec* pColumnVec = new ColumnVec;
      m_pRegSet->m_xpStreamDef->vAddColumns(pColumnVec);
      
      Row* pRow = pIrwBuffer->aAllotRow(pColumnVec);
      pRow->m_waColVal[0] = nCount;
      
      // pipe out the streamdef
      pOp->vPipeOut(*m_pRegSet->m_xpStreamDef);
      
      // pipe out the buffer with the count
      pOp->vPipeOut(pIrwBuffer);
      
      pOp->vPipeOut(m_pRegSet->m_pOperand);
    }
    
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
/* Operand[].handleEnd -- handle end marker                                   */
/*----------------------------------------------------------------------------*/
void Step::vOperandHandleEnd(void)
{
  ENTRY(); // ENTRY("vOperandHandleEnd"_Stz);
  
  ASSERT(m_pRegSet->m_pOperand);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Universal->m_pOp;
  
  if (--pOp->m_nLeftEndMarkers == 0) {
    if (pOp->bIsCancelling()) {
      delete _pEndMarker(m_pRegSet->m_pOperand);
      
      pOp->vPipeOut(new CancelMarker);
        
    }
    else {
      pOp->vPipeOut(m_pRegSet->m_pOperand);
    }
    
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
/* Operand[].isNull -- is operand null ?                                      */
/*----------------------------------------------------------------------------*/
void Step::vOperandIsNull(void)
{
  ENTRY(); // ENTRY("vOperandIsNull"_Stz);
  
  m_pDataStack->vPushInt(m_pRegSet->m_pOperand ? 0 : 1);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].itemCount@ -- fetch the operand item count                       */
/*----------------------------------------------------------------------------*/
void Step::vOperandItemCountFetch(void)
{
  ENTRY(); // ENTRY("vOperandItemCountFetch"_Stz);
  
  ASSERT(m_pRegSet->m_pOperand);
  IrwBuffer* pIrwBuffer = _pIrwBuffer(m_pRegSet->m_pOperand);
  
  m_pDataStack->vPushInt(pIrwBuffer->nItems());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].kind -- decode operand into its operand kind                     */
/*----------------------------------------------------------------------------*/
void Step::vOperandKind(void)
{
  ENTRY(); // ENTRY("vOperandKind"_Stz);
  
  eZK eOperand = ZK_NONE;
  
  eCN eClass_op = m_pRegSet->m_pOperand->eClass();
  switch (eClass_op) {
      
    case CN_CRWBUFFER:   eOperand = ZK_CRWBUFFER;   break;
    case CN_CSVBUFFER:   eOperand = ZK_CSVBUFFER;   break;
    case CN_ENDMARKER:   eOperand = ZK_ENDMARKER;   break;
    case CN_ENTBUFFER:   eOperand = ZK_ENTBUFFER;   break;
    case CN_IRWBUFFER:   eOperand = ZK_IRWBUFFER;   break;
    case CN_SRTBUFFER:   eOperand = ZK_SRTBUFFER;   break;
    case CN_STARTMARKER: eOperand = ZK_STARTMARKER; break;
    case CN_STREAMDEF:   eOperand = ZK_STREAMDEF;   break;
      
    default:
      RASSERT(0);
      break;
  }
  
  m_pDataStack->vPushInt(eOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].next -- get next operand                                         */
/*----------------------------------------------------------------------------*/
void Step::vOperandNext(void)
{
  ENTRY(); // ENTRY("vOperandNext"_Stz);
  
  if (! m_pRegSet->m_pOperand)
    m_pRegSet->m_pOperand = m_pTask->m_pJob->m_pOp->pPopQueue2();
  ASSERT(m_pRegSet->m_pOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newEndMarker -- new an end marker                                */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewEndMarker(void)
{
  ENTRY(); // ENTRY("vOperandNewEndMarker"_Stz);
  
  ASSERT (! m_pRegSet->m_pOperand);
  
  m_pRegSet->m_pOperand = new EndMarker;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].newStreamDef -- new a stream def                                 */
/*----------------------------------------------------------------------------*/
void Step::vOperandNewStreamDef(void)
{
  ENTRY(); // ENTRY("vOperandNewStreamDef"_Stz);
  
  ASSERT (! m_pRegSet->m_pOperand);
  
  m_pRegSet->m_pOperand = new StreamDef(m_pRegSet->m_xpTable->pStreamDef());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].pipeOut -- append operand onto next operator in pipeline         */
/*----------------------------------------------------------------------------*/
void Step::vOperandPipeOut(void)
{
  ENTRY(); // ENTRY("vOperandPipeOut"_Stz);
  
  ASSERT(m_pRegSet->m_pOperand);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Universal->m_pOp;

  pOp->vPipeOut(m_pRegSet->m_pOperand);
  
  m_pRegSet->m_pOperand = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].pipeToJob -- append operand onto job's buffer queue              */
/*----------------------------------------------------------------------------*/
void Step::vOperandPipeToJob(void)
{
  ENTRY(); // ENTRY("vOperandPipeToJob"_Stz);
  
  ASSERT(m_pRegSet->m_pOperand);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Universal->m_pOp;
  
  pOp->vPipeToJob(m_pRegSet->m_pOperand);
  
  m_pRegSet->m_pOperand = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].rowCount@ -- fetch the operand row count                         */
/*----------------------------------------------------------------------------*/
void Step::vOperandRowCountFetch(void)
{
  ENTRY(); // ENTRY("vOperandRowCountFetch"_Stz);
  
  ASSERT(m_pRegSet->m_pOperand);
  CrwBuffer* pCrwBuffer = _pCrwBuffer(m_pRegSet->m_pOperand);
  
  m_pDataStack->vPushInt(pCrwBuffer->nRowCount());
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].set -- set operand                                               */
/*----------------------------------------------------------------------------*/
void Step::vOperandSet(void)
{
  ENTRY(); // ENTRY("vOperandSet"_Stz);
  
  Object* pPtrObject;
  if ( ! m_pDataStack->bPopPtr(pPtrObject) )
    THROW_DSTACK();
  
  m_pRegSet->m_pOperand = _pOperand(pPtrObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].toCsv -- convert operand to Csv                                  */
/*----------------------------------------------------------------------------*/
void Step::vOperandToCsv(void)
{
  ENTRY(); // ENTRY("vOperandToCsv"_Stz);
  
  CrwBuffer* pCrwBuffer = _pCrwBuffer(m_pRegSet->m_pOperand);
  m_pRegSet->m_pOperand = pCrwBuffer->pToCsvBuffer();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Operand[].workDone -- update work done                                     */
/*----------------------------------------------------------------------------*/
void Step::vOperandWorkDone(void)
{
  ENTRY(); // ENTRY("vOperandWorkDone"_Stz);
  
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

/*----------------------------------------------------------------------------*/
/* Operand[].writeToFile -- write to file                                     */
/*----------------------------------------------------------------------------*/
void Step::vOperandWriteToFile(void)
{
  ENTRY(); // ENTRY("vOperandWriteToFile"_Stz);
  
  ASSERT(m_pRegSet->m_pOperand);
  ASSERT(*m_pRegSet->m_xpFile0);
  ASSERT(m_pRegSet->m_xpFile0->bIsOpen());
  
  CsvBuffer* pCsvBuffer = _pCsvBuffer(m_pRegSet->m_pOperand) ;
  char* aData = pCsvBuffer->aDataArea();
  
  m_pRegSet->m_xpFile0->vPrint("%s"_Stz,aData);
  m_pRegSet->m_xpFile0->vFlush();
  
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
