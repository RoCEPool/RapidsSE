/*============================================================================*/
/* Steps_RegOperandOoO.cpp -- Register Operand Out-of_Order step group        */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RegOperandOoO(void)
{
  vAddStep("OperandOoO[]"_Stz,             &Step::vRegOperandOoOGet);
  vAddStep("OperandOoO[].delete"_Stz,      &Step::vRegOperandOoODelete);
  vAddStep("OperandOoO[].isNull"_Stz,      &Step::vRegOperandOoOIsNull);
  vAddStep("OperandOoO[].kind"_Stz,        &Step::vRegOperandOoOKind);
  vAddStep("OperandOoO[].peek"_Stz,        &Step::vRegOperandOoOPeek);
  vAddStep("OperandOoO[].pipeOut"_Stz,     &Step::vRegOperandOoOPipeOut);
  vAddStep("OperandOoO[].pipeToJob"_Stz,   &Step::vRegOperandOoOPipeToJob);
  vAddStep("OperandOoO[].set"_Stz,         &Step::vRegOperandOoOSet);
}

/*----------------------------------------------------------------------------*/
/* OperandOoO[].delete -- operand delete                                      */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandOoODelete(void)
{
  ENTRY(); // ENTRY("vOperandOoODelete"_Stz);
  
  AM::vDeleteOperand(*m_pRegSet->m_xpOperandOoO);
  
  m_pRegSet->m_xpOperandOoO = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandOoO[].get -- fetch operand                                          */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandOoOGet(void)
{
  ENTRY(); // ENTRY("vOperandOoOGet"_Stz);
  
  m_pDataStack->vPushPtr(*m_pRegSet->m_xpOperandOoO);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandOoO[].isNull -- is operand null ?                                   */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandOoOIsNull(void)
{
  ENTRY(); // ENTRY("vOperandOoOIsNull"_Stz);
  
  m_pDataStack->vPushInt(m_pRegSet->m_xpOperandOoO.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandOoO[].kind -- decode operand into its operand kind                  */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandOoOKind(void)
{
  ENTRY(); // ENTRY("vOperandOoOKind"_Stz);
  
  eZK eOperand = ZK_NONE;
  
  eCN eClass_op = m_pRegSet->m_xpOperandOoO->eClass();
  switch (eClass_op) {
    
    case CN_CRWBUFFER:       eOperand = ZK_CRWBUFFER;       break;
    case CN_CSVBUFFER:       eOperand = ZK_CSVBUFFER;       break;
    case CN_ENTBUFFER:       eOperand = ZK_ENTBUFFER;       break;
    case CN_INFOSET  :       eOperand = ZK_INFOSET;         break;
    case CN_IRWBUFFER:       eOperand = ZK_IRWBUFFER;       break;
    case CN_MARKER: {
      Marker* pMarker = _pMarker(*m_pRegSet->m_xpOperandOoO);

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
/* OperandOoO[].peek -- peek and see if there is another operand              */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandOoOPeek(void)
{
  ENTRY(); // ENTRY("vRegOperandOoOPeek"_Stz);
  
  m_pRegSet->m_xpOperandOoO = m_pTask->m_pJob->m_pOp->pPeekQueue();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandOoO[].pipeOut -- append operand onto next operator in pipeline      */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandOoOPipeOut(void)
{
  ENTRY(); // ENTRY("vOperandOoOPipeOut"_Stz);
  
  ASSERT(*m_pRegSet->m_xpOperandOoO);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Universal->m_pOp;

  pOp->vPipeOut_nextOp(*m_pRegSet->m_xpOperandOoO);
  
  m_pRegSet->m_xpOperandOoO = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandOoO[].pipeToJob -- append operand onto the pipeline job buffer queue*/
/*----------------------------------------------------------------------------*/
void Step::vRegOperandOoOPipeToJob(void)
{
  ENTRY(); // ENTRY("vOperandOoOPipeToJob"_Stz);
  
  ASSERT(*m_pRegSet->m_xpOperandOoO);
  
  Op* pOp = m_pTask->m_pJob->m_pOp; //  pJob_Universal->m_pOp;
  
  pOp->vPipeToJob(*m_pRegSet->m_xpOperandOoO);
  
  m_pRegSet->m_xpOperandOoO = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* OperandOoO[].set -- set operand                                            */
/*----------------------------------------------------------------------------*/
void Step::vRegOperandOoOSet(void)
{
  ENTRY(); // ENTRY("vOperandOoOSet"_Stz);
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  ASSERT(! pOperand || m_pRegSet->m_xpOperandOoO.bIsNull());
  m_pRegSet->m_xpOperandOoO = _pOperand(pOperand);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
