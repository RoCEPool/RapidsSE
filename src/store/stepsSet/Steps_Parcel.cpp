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


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Parcel(void)
{
  vAddStep("parcel.operand!"_Stz,   &Step::vParcelOperandStore);
  vAddStep("parcel.pipeOut"_Stz,    &Step::vParcelPipeOut);
  vAddStep("parcel.sendOut"_Stz,    &Step::vParcelSendOut);
}

/*----------------------------------------------------------------------------*/
/* Parcel.operand! -- store operand in the sendOperand                        */
/*----------------------------------------------------------------------------*/
void Step::vParcelOperandStore(void)
{
  ENTRY();
  
  Parcel* pParcel = _pParcel(m_pDataStack->pPopPtr(__func__));
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  
  pParcel->vSetOperand(pOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Parcel.pipeOut -- pipe out the operand                                     */
/*----------------------------------------------------------------------------*/
void Step::vParcelPipeOut(void)
{
  ENTRY();
  
  Parcel* pParcel = _pParcel(m_pDataStack->pPopPtr(__func__));
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  
  // find the pipeline participant job
  i8 iPipelineJob = pParcel->m_iPipelineJobId;
  Job* pPipelineJob = AM::pJob_FindParticipating(iPipelineJob);

  // get the next stage the send is supposed to send to
  Stage* pStage = pPipelineJob->m_pPipeline->pGet(pParcel->m_iNextStage);
  
  // get the op to forward to depending on the cross topology
  Op* pOp = pStage->pPipeOutOp(pParcel->m_eCrossTopo,
                               pParcel->m_tPtnNo,
                               pParcel->m_iNextOp);
  
  //! ALKA come back to this. May need a m_bAppendLeft in pParcel
  // append to left queue
  if (pParcel->pGetOperand())
    pOp->vAppendLeft(pOperand,this);
  
  // now delete the pParcel
  delete pParcel;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Parcel.sendOut -- send parcel out to next operator in pipeline             */
/*----------------------------------------------------------------------------*/
void Step::vParcelSendOut(void)
{
  ENTRY();
  
  Op* pOp = m_pJob->m_pOp;
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  
  Parcel::vSendOut(pOperand,pOp);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
