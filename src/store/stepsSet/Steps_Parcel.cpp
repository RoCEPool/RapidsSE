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
  vAddStep("parcel::delete"_Stz,    &Step::vParcelDelete);
  vAddStep("parcel.label"_Stz,      &Step::vParcelLabel);
  vAddStep("parcel.pipeOut"_Stz,    &Step::vParcelPipeOut);
  vAddStep("parcel.sendOut"_Stz,    &Step::vParcelSendOut);
}

/*----------------------------------------------------------------------------*/
/* parcel::delete -- delete parcel                                            */
/*----------------------------------------------------------------------------*/
void Step::vParcelDelete(void)
{
  ENTRY();

  Parcel* pParcel = _pParcel(m_pDataStack->pPopPtr(__func__));
  vTrace(">>>>>>>>Step::vParcelDelete %p\n"_Stz, pParcel);
  delete pParcel;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* parcel::label -- label parcel                                              */
/*----------------------------------------------------------------------------*/
void Step::vParcelLabel(void)
{
  ENTRY();


  Op* pSrcOp = m_pJob->m_pOp;

  Parcel* pParcel = _pParcel(m_pDataStack->pPopPtr(__func__));

  pParcel->vLabel(pSrcOp->pPipelineJob()->iJobId(),
                  C_UNKNOWN,
                  C_UNKNOWN,
                  pSrcOp->eCrossTopo());

  m_pDataStack->vPushPtr(pParcel);
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
  i8 iPipelineJob = pParcel->m_iPipelineJobId;
  Job* pPipelineJob = AM::pJob_FindParticipating(iPipelineJob);

  // get the next stage the send is supposed to send to
  Stage* pStage = pPipelineJob->m_pPipeline->pGet(pParcel->m_iDstStage);
  
  // get the op to forward to depending on the cross topology
  Op* pOp = pStage->pPipeOutOp(pParcel->m_eCrossTopo,
                               pParcel->m_tDstPtn,
                               pParcel->m_iDstOp);
  
  //! ALKA come back to this. May need a m_bAppendLeft in pParcel
  // append to left queue

  if (pParcel->bIsOriginal()) {
      pOp->vAppendLeft(pParcel,this);
  } else {
    if (pParcel->pGetOperand())
      pOp->vAppendLeft(pOperand, this);

    // now delete the pParcel
    delete pParcel;
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* parcel.sendOut -- send parcel out to next operator in pipeline             */
/*----------------------------------------------------------------------------*/
void Step::vParcelSendOut(void)
{
  ENTRY();
    
  Parcel* pParcel = _pParcel(m_pDataStack->pPopPtr(__func__));
  
  Op* pSrcOp = m_pJob->m_pOp;

  pParcel->vSendOut(pSrcOp);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
