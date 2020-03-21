/*============================================================================*/
/* Steps_RegPipein.cpp -- Register Pipein step group                          */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegPipein(void)
{
  vAddStep("Pipein[]"_Stz,                &Step::vRegPipeinGet);
  vAddStep("Pipein[].bide"_Stz,           &Step::vRegPipeinBide);
  vAddStep("Pipein[].delete"_Stz,         &Step::vRegPipeinDelete);
  vAddStep("Pipein[].isNull"_Stz,         &Step::vRegPipeinIsNull);
  vAddStep("Pipein[].kind"_Stz,           &Step::vRegPipeinKind);
  vAddStep("Pipein[].next"_Stz,           &Step::vRegPipeinNext);
  vAddStep("Pipein[].peekOoO"_Stz,        &Step::vRegPipeinPeekOoO);
  vAddStep("Pipein[].pipeOut"_Stz,        &Step::vRegPipeinPipeOut);
  vAddStep("Pipein[].pipeToJob"_Stz,      &Step::vRegPipeinPipeToJob);
  vAddStep("Pipein[].set"_Stz,            &Step::vRegPipeinSet);
}

/*----------------------------------------------------------------------------*/
/* Pipein[].bide -- pipein operand bide                                       */
/*----------------------------------------------------------------------------*/
void Step::vRegPipeinBide(void)
{
  ENTRY(); // ENTRY("vOperandBide"_Stz);
  
  m_pRegSet->m_xpPipein = m_pTask->m_pJob->m_pOp->pPopQueue();
  
  if (m_pRegSet->m_xpPipein.bIsNull()) {
    ASSERT(m_pTask->bIsLocked());
    m_pTask->vSchedulerAct(SA_BIDE);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipein[].delete -- operand delete                                         */
/*----------------------------------------------------------------------------*/
void Step::vRegPipeinDelete(void)
{
  ENTRY(); // ENTRY("vOperandDelete"_Stz);
  
  AM::vDeleteOperand(*m_pRegSet->m_xpPipein);
  
  m_pRegSet->m_xpPipein = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipein[].get -- fetch operand                                             */
/*----------------------------------------------------------------------------*/
void Step::vRegPipeinGet(void)
{
  ENTRY(); // ENTRY("vOperandGet"_Stz);
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpPipein);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipein[].isNull -- is operand null ?                                      */
/*----------------------------------------------------------------------------*/
void Step::vRegPipeinIsNull(void)
{
  ENTRY(); // ENTRY("vOperandIsNull"_Stz);
  
  m_pDataStack->vPushInt(m_pRegSet->m_xpPipein.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipein[].kind -- decode operand into its operand kind                     */
/*----------------------------------------------------------------------------*/
void Step::vRegPipeinKind(void)
{
  ENTRY(); // ENTRY("vOperandKind"_Stz);
  
  eZK eOperand = ZK_NONE;
  
  eCN eClass_op = m_pRegSet->m_xpPipein->eClass();
  switch (eClass_op) {
    
    case CN_CRWBUFFER:       eOperand = ZK_CRWBUFFER;       break;
    case CN_CSVBUFFER:       eOperand = ZK_CSVBUFFER;       break;
    case CN_ENTBUFFER:       eOperand = ZK_ENTBUFFER;       break;
    case CN_INFOSET  :       eOperand = ZK_INFOSET;         break;
    case CN_IRWBUFFER:       eOperand = ZK_IRWBUFFER;       break;
    case CN_MARKER: {
      Marker* pMarker = _pMarker(*m_pRegSet->m_xpPipein);

       switch(pMarker->eOpMarker()) {
       
        case OM_CANCELMARKER:     eOperand = ZK_CANCELMARKER;    break;
         case OM_EOSMARKER:       eOperand = ZK_EOSMARKER;       break;
         case OM_ERRORMARKER:     eOperand = ZK_ERRORMARKER;     break;
         case OM_PULSEMARKER:     eOperand = ZK_PULSEMARKER;     break;
         case OM_STARTMARKER:     eOperand = ZK_STARTMARKER;     break;
         case OM_STARTEDMARKER:   eOperand = ZK_STARTEDMARKER;   break;
         case OM_STOPMARKER:      eOperand = ZK_STOPMARKER;      break;
         case OM_STOPPEDMARKER:   eOperand = ZK_STOPPEDMARKER;   break;
         default:  ASSERT(0);
       }
      break;
    }
    case CN_SRTBUFFER:       eOperand = ZK_SRTBUFFER;       break;
    case CN_STREAMDEF:       eOperand = ZK_STREAMDEF;       break;
      
    default:
      RASSERT(0);
      break;
  }
  
  m_pDataStack->vPushInt(eOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipein[].next -- get next operand                                          */
/*----------------------------------------------------------------------------*/
void Step::vRegPipeinNext(void)
{
  ENTRY(); // ENTRY("vOperandNext"_Stz);
  
  if (m_pRegSet->m_xpPipein.bIsNull())
    m_pRegSet->m_xpPipein = m_pTask->m_pJob->m_pOp->pPopQueue();
  ASSERT(*m_pRegSet->m_xpPipein);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipein[].peekOoO -- peek and see if there is an Out-of-Order operand       */
/*----------------------------------------------------------------------------*/
void Step::vRegPipeinPeekOoO(void)
{
  ENTRY(); // ENTRY("vRegPipeinPeekOoO"_Stz);
  
  Operand* pOperand = _pOperand(m_pTask->m_pJob->m_pOp->pPeekQueue());
  
  // did we get a marker ?
  if ( pOperand && pOperand->bIsMarker() ) {
    
    Marker* pMarker = _pMarker(pOperand);
    
    // is it a CANCELMARKER ?
    if (pMarker->eOpMarker() == OM_CANCELMARKER)
      m_pDataStack->vPushInt(true);
    else
      m_pDataStack->vPushInt(false);
  }
  else {
    m_pDataStack->vPushInt(false);
  }
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipein[].pipeOut -- append operand onto next operator in pipeline          */
/*----------------------------------------------------------------------------*/
void Step::vRegPipeinPipeOut(void)
{
  ENTRY(); // ENTRY("vOperandPipeOut"_Stz);
  
  ASSERT(*m_pRegSet->m_xpPipein);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Universal->m_pOp;

  pOp->vPipeOut_nextOp(*m_pRegSet->m_xpPipein);
  
  m_pRegSet->m_xpPipein = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipein[].pipeToJob -- append operand onto the pipeline job buffer queue   */
/*----------------------------------------------------------------------------*/
void Step::vRegPipeinPipeToJob(void)
{
  ENTRY(); // ENTRY("vOperandPipeToJob"_Stz);
  
  ASSERT(*m_pRegSet->m_xpPipein);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Universal->m_pOp;
  
  pOp->vPipeToJob(*m_pRegSet->m_xpPipein);
  
  m_pRegSet->m_xpPipein = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pipein[].set -- set operand                                               */
/*----------------------------------------------------------------------------*/
void Step::vRegPipeinSet(void)
{
  ENTRY(); // ENTRY("vOperandSet"_Stz);
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  ASSERT(! pOperand || m_pRegSet->m_xpPipein.bIsNull());
  m_pRegSet->m_xpPipein = _pOperand(pOperand);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
