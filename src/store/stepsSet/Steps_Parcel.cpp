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
  Pipeline* pPipeline = pPipelineJob->pPipeline();
  
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

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
