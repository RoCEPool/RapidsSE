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
	vAddStep("PipeIn[].bufferType"_Stz,     &Step::vPipeinBufferType);
  vAddStep("PipeIn[].delete"_Stz,         &Step::vPipeinDelete);
  vAddStep("PipeIn[].isBuffer"_Stz,       &Step::vPipeinIsBuffer);
  vAddStep("PipeIn[].isMarker"_Stz,       &Step::vPipeinIsMarker);
  vAddStep("PipeIn[].isNull"_Stz,         &Step::vPipeinIsNull);
	vAddStep("PipeIn[].isRowBuffer"_Stz,    &Step::vPipeinIsRowBuffer);
  vAddStep("PipeIn[].kind"_Stz,           &Step::vPipeinKind);
  vAddStep("PipeIn[].name"_Stz,           &Step::vPipeinName);
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
  bool bPageIn = false;
  if (m_pThread->eDedicated()) {
        
    while ( ! (pOperand = pOp->pPopOoOLeft(&bPageIn)) ) {
      vDropTaskLock((Stz*)__func__);
      m_pThread->pPsxThd()->vSleepMsec(10,CN_TASK);

    }

    m_pLogicUnit->vSetPipein(pOperand);
    
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  else {
    pOperand = pOp->pPopOoOLeft(&bPageIn);
    m_pLogicUnit->vSetPipein(pOperand);
    
    if (m_pLogicUnit->m_xpPipein.bIsNull()) {
      RASSERT(bIsLocked());
      if(bPageIn)
        vScheduleBidePageIn();
      else
        vScheduleBideForOperand();
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
    bool bPageIn = false;
    if ( (pOperand = pOp->pPopOoOLeft(&bPageIn)) ) {
      m_pLogicUnit->vSetPipein(pOperand);
    }
    else {
      if(bPageIn) {
        // Bide while Operand is brought into memory
        // Re-run this step after
        m_aInxPtr--;
        vScheduleBidePageIn();
      }
      else
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

  // TODO: Arjun - We need to free the slot in the page file here

  AM::vDeleteOperand(*m_pLogicUnit->m_xpPipein,pOuterJobOfTask());
  
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
/* PipeIn[].bufferType -- decode operand into its buffer type                 */
/*----------------------------------------------------------------------------*/
void Step::vPipeinBufferType(void)
{
	ENTRY();

	eBT ePipeInBufferType = m_pLogicUnit->m_ePipeInBufferType;
	m_pDataStack->vPushInt(ePipeInBufferType);

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
/* PipeIn[].isMarker -- is operand null ?                                       */
/*----------------------------------------------------------------------------*/
void Step::vPipeinIsMarker(void)
{
  ENTRY();

  Operand* pOperand = _pOperand(*m_pLogicUnit->m_xpPipein);;

  m_pDataStack->vPushInt(pOperand->bIsMarker() ? 1 : 0);

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
/* PipeIn[].isRowBuffer -- is operand a row buffer ?                          */
/*----------------------------------------------------------------------------*/
void Step::vPipeinIsRowBuffer(void)
{
	ENTRY();

	Operand* pOperand = _pOperand(*m_pLogicUnit->m_xpPipein);;

	m_pDataStack->vPushInt(pOperand->bIsRowBuffer() ? 1 : 0);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].kind -- decode operand into its operand kind                      */
/*----------------------------------------------------------------------------*/
void Step::vPipeinKind(void)
{
  ENTRY();

  eEV ePipeInOperand = m_pLogicUnit->m_ePipeInOperand;
  m_pDataStack->vPushInt(ePipeInOperand);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PipeIn[].name -- get name of operand                                       */
/*----------------------------------------------------------------------------*/
void Step::vPipeinName(void)
{
  ENTRY();

  RASSERT( ! m_pLogicUnit->m_xpPipein.bIsNull() );

  Operand* pOperand = _pOperand(*m_pLogicUnit->m_xpPipein);

  s256 azName;
  if (pOperand->bIsMarker()) {
    Marker* pMarker = _pMarker(pOperand);
    azName = aOM[pMarker->eOpMarker()];
    m_pDataStack->vPushStr(azName);
  }
  else {
    azName = aCN[pOperand->eGetClassNo()];
    m_pDataStack->vPushStr(azName);
  }


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

  bool bPageIn = false;
  Operand* pOperand = pOp->pPopOoOLeft(&bPageIn);
  // TODO: Arjun - Check page in scenario
  
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
    m_pLogicUnit->m_ePipeInOperand = EV_NONE;
		m_pLogicUnit->m_ePipeInBufferType = BT_NONE;
  }
  else {
    eEV eOperand = (eEV)AM::iOperandEvKind(pOperand);
    m_pLogicUnit->m_ePipeInOperand = eOperand;
		if (eOperand == EV_D_ROWBUFFER) {
			RowBuffer *pRowBuffer = _pRowBuffer(pOperand);
			m_pLogicUnit->m_ePipeInBufferType = pRowBuffer->eBufferType();
		}
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
