/*============================================================================*/
/* Steps_Parcel.cpp -- parcel steps                                           */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "ePX.h"

#include "Parcel.hpp"

#include "Op.hpp"
#include "Stage.hpp"
#include "Pipeline.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Parcel(void)
{
  vAddStep("parcel::delete"_Stz,          &Step::vParcelDeleteOld);
  vAddStep("parcel.label"_Stz,            &Step::vParcelLabel);
  vAddStep("parcel.pipeOut"_Stz,          &Step::vParcelPipeOut);
  vAddStep("parcel.sendOut"_Stz,          &Step::vParcelSendOut);

  vAddStep("Parcel[]"_Stz,                &Step::vParcelGet);
  vAddStep("Parcel[]\\clear"_Stz,         &Step::vParcelGetAndClear);
  vAddStep("Parcel[].delete"_Stz,         &Step::vParcelDelete);
  vAddStep("Parcel[].isNotNull"_Stz,      &Step::vParcelIsNotNull);
  vAddStep("Parcel[].isNull"_Stz,         &Step::vParcelIsNull);
  vAddStep("Parcel[].set"_Stz,            &Step::vParcelSet);
}

/*----------------------------------------------------------------------------*/
/* parcel::delete -- delete parcel                                            */
/*----------------------------------------------------------------------------*/
void Step::vParcelDeleteOld(void)
{
  ENTRY();

  Parcel* pParcel = _pParcel(m_pDataStack->pPopPtr(__func__));
  delete pParcel;
  //AM::vTraceDumpGrabLock();
  //vTrace(">>>>>>>>Step::vParcelDelete %p\n"_Stz, pParcel);
  //AM::vTraceDumpDropLock();

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* parcel::label -- label parcel                                              */
/*----------------------------------------------------------------------------*/
void Step::vParcelLabel(void)
{
  ENTRY();


  // revised job/op model
    Op* pOp = *m_pOp;


  Parcel* pParcel = _pParcel(m_pDataStack->pPopPtr(__func__));

  pParcel->vLabel(pOp->eCrossTopo(),
                  pOp->pPipelineJob()->iJobId()
                  );

  //m_pDataStack->vPushPtr(pParcel);
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* parcel.pipeOut -- pipe out parcel out to next operator in pipeline         */
/*----------------------------------------------------------------------------*/
void Step::vParcelPipeOut(void)
{
  ENTRY();
  
  Parcel* pParcel = _pParcel(m_pDataStack->pPopPtr(__func__));
  
  //Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  Operand* pOperand = pParcel->pGetOperand();

  // find the pipeline participant job
  i8 iPipelineJob = pParcel->iPipelineJobId();
  Job* pPipelineJob = AM::pJob_FindParticipating(iPipelineJob);

  // get the next stage the send is supposed to send to
  Pipeline* pPipeline = pPipelineJob->pPipeline_job();
  
  Stage* pStage = pPipeline->pGet(pParcel->iDstStage());

  // Where do we see the BktNo, so that we can map it to the partition?

  // get the op to forward to depending on the cross topology
  Op* pOp = pStage->pPipeOutOp(pParcel->eCrossTopo(),
                               pParcel->tDstPtn(),
                               pParcel->iDstOp());
  
  //! ALKA come back to this. May need a m_bAppendLeft in pParcel

  if (pParcel->bOriginalParcel()) {
    pOp->vAppendLeft(pParcel,this);
  } else {
    if (pParcel->pGetOperand()) {
        pOp->vAppendLeft(pOperand, this);
    }
  }

  LogicUnit* pLogicUnit = pOp->pOpLogicUnit();
  if (pLogicUnit) {
    pLogicUnit->m_xpStats->m_nParcelRecvs++;
    pLogicUnit->m_xpStats->m_nObjectsRecv += pParcel->nItems();
  }

  /* Origin from master
    if (pParcel->pGetOperand()) {
      if (pParcel->bAppendRight())
        pOp->vAppendRight(pOperand, this);
      else
        pOp->vAppendLeft(pOperand, this);
    }
    */
  
  // now delete the pParcel
  delete pParcel;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* parcel.sendOut -- send parcel out to next operator in pipeline             */
/*----------------------------------------------------------------------------*/
void Step::vParcelSendOut(void)
{
  ENTRY();
    
  Parcel* pParcel = _pParcel(m_pDataStack->pPopPtr(__func__));
  
  // revised job/op model
    Op* pOp = *m_pOp;

  pParcel->vSendOut(pOp);

  LogicUnit* pLogicUnit = pOp->pOpLogicUnit();
  if (pLogicUnit) {
    pLogicUnit->m_xpStats->m_nParcelSends++;
    pLogicUnit->m_xpStats->m_nObjectsSent += pParcel->nItems();
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Parcel[].delete -- parcel delete                                           */
/*----------------------------------------------------------------------------*/
void Step::vParcelDelete(void)
{
  ENTRY();

  AM::vDeleteOperand(*m_p_TaskLogicUnit->m_xpParcel);

  m_p_TaskLogicUnit->m_xpParcel = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Parcel[].get -- fetch parcel                                               */
/*----------------------------------------------------------------------------*/
void Step::vParcelGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpParcel);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Parcel[].getAndClear -- fetch parcel and set to nullptr                    */
/*----------------------------------------------------------------------------*/
void Step::vParcelGetAndClear(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpParcel);

  m_p_TaskLogicUnit->m_xpParcel = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Parcel[].isNotNull -- is parcel not null ?                                 */
/*----------------------------------------------------------------------------*/
void Step::vParcelIsNotNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xpParcel.bIsNull() ? 0 : 1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Parcel[].isNull -- is parcel null ?                                        */
/*----------------------------------------------------------------------------*/
void Step::vParcelIsNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xpParcel.bIsNull() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Parcel[].set -- set parcel                                                 */
/*----------------------------------------------------------------------------*/
void Step::vParcelSet(void)
{
  ENTRY();

  Parcel* pParcel = _pParcel(m_pDataStack->pPopPtr(__func__));

  ASSERT(! pParcel || m_p_TaskLogicUnit->m_xpParcel.bIsNull());

  m_p_TaskLogicUnit->m_xpParcel = _pParcel(pParcel);

  EXIT();
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
