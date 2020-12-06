/*============================================================================*/
/* Steps_RegPipein.cpp -- Register Pipein step group                          */
/*============================================================================*/

#include "Steps.h"

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
  
  eZK eOperand = ZK_NONE;
  
  eCN eClass_op = m_pLogicUnit->m_xpPipein->eClass();
  switch (eClass_op) {
    
    case CN_CRWBUFFER:            eOperand = ZK_CRWBUFFER;       break;
    case CN_CSVBUFFER:            eOperand = ZK_CSVBUFFER;       break;
    case CN_ENTBUFFER:            eOperand = ZK_ENTBUFFER;       break;
    case CN_INDEXDEF:             eOperand = ZK_INDEXDEF;        break;
    case CN_INFOSET  : {
      InfoSet* pInfoSet = _pInfoSet(*m_pLogicUnit->m_xpPipein);
      switch(pInfoSet->eOpInfoSet()) {
        case OI_CALLPIPELINE:     eOperand = ZK_CALLPIPELINE;    break;
        case OI_COUNTINFO:        eOperand = ZK_COUNTINFO;       break;
        case OI_LOCKSTATS:        eOperand = ZK_LOCKSTATS;       break;
        case OI_NODEINFO:         eOperand = ZK_NODEINFO;        break;
        case OI_PATHINFO:         eOperand = ZK_PATHINFO;        break;
        case OI_PROGRESSUNITS:    eOperand = ZK_PROGRESSUNITS;   break;
        case OI_RETURNPIPELINE:   eOperand = ZK_RETURNPIPELINE;  break;
        case OI_SEEKOFFSETS:      eOperand = ZK_SEEKOFFSETS;     break;
        case OI_TOTALUNITS:       eOperand = ZK_TOTALUNITS;      break;
        default: RASSERT(0);
      }
      break;
    }
    case CN_MARKER: {
      Marker* pMarker = _pMarker(*m_pLogicUnit->m_xpPipein);

       switch(pMarker->eOpMarker()) {
       
         case OM_CANCELMARKER:    eOperand = ZK_CANCELMARKER;    break;
         case OM_DATAPULSE:       eOperand = ZK_DATAPULSE;       break;
         case OM_DESTROYMARKER:   eOperand = ZK_DESTROYMARKER;   break;
         case OM_EOSMARKER:       eOperand = ZK_EOSMARKER;       break;
         case OM_ERRORMARKER:     eOperand = ZK_ERRORMARKER;     break;
         case OM_ERROREDMARKER:   eOperand = ZK_ERROREDMARKER;   break;
         case OM_EXITMARKER:      eOperand = ZK_EXITMARKER;      break;
         case OM_EXITEDMARKER:    eOperand = ZK_EXITEDMARKER;    break;
         case OM_PULSEDMARKER:    eOperand = ZK_PULSEDMARKER;    break;
         case OM_STARTMARKER:     eOperand = ZK_STARTMARKER;     break;
         case OM_STARTEDMARKER:   eOperand = ZK_STARTEDMARKER;   break;
         default:  RASSERT(0);
       }
      break;
    }
    case CN_PROFILE:              eOperand = ZK_PROFILE;         break;
    case CN_ROWBUFFER:            eOperand = ZK_ROWBUFFER;       break;
    case CN_SRTBUFFER:            eOperand = ZK_SRTBUFFER;       break;
    case CN_STREAMDEF:            eOperand = ZK_STREAMDEF;       break;
    case CN_STREAMSET:            eOperand = ZK_STREAMSET;       break;
      
    default:
      RASSERT(0);
      break;
  }
  
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
