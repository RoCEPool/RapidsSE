/*============================================================================*/
/* Steps_Operand.cpp -- Register Operand step group                           */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "ePX.h"

#include "Marker.hpp"

#include "RowBuffer.hpp"
#include "EachRowBuffer.hpp"

#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "Crw.hpp"
#include "CrwPacker.hpp"
#include "CrwBuffer.hpp"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"
#include "CrvBuffer.hpp"
#include "TablePtn.hpp"
#include "Table.hpp"
#include "Node_I.hpp"
#include "Op.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"

#include "Parcel.hpp"


#ifdef WITH_PYTHON_SUPPORT
  #include "/usr/include/python2.7/Python.h"
#endif

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Marker(void)
{
  vAddStep("Marker[]"_Stz,                &Step::vMarkerGet);
  vAddStep("Marker[]\\clear"_Stz,         &Step::vMarkerGetAndClear);
  vAddStep("Marker[].delete"_Stz,         &Step::vMarkerDelete);
  vAddStep("Marker[].isNotNull"_Stz,      &Step::vMarkerIsNotNull);
  vAddStep("Marker[].isNull"_Stz,         &Step::vMarkerIsNull);
  vAddStep("Marker[].newCancelMarker"_Stz,&Step::vMarkerNewCancelMarker);
  vAddStep("Marker[].newDataPulse"_Stz,   &Step::vMarkerNewDataPulse);
  vAddStep("Marker[].newDestroyMarker"_Stz,      &Step::vMarkerNewDestroyMarker);
  vAddStep("Marker[].newEachScanEosMarker"_Stz,   &Step::vMarkerNewEachScanEosMarker);
  vAddStep("Marker[].newEndCrwRhtMarker"_Stz, &Step::vMarkerNewEndCrwRhtMarker);
  vAddStep("Marker[].newEndRowRhtMarker"_Stz, &Step::vMarkerNewEndRowRhtMarker);
  vAddStep("Marker[].newEosMarker"_Stz,   &Step::vMarkerNewEosMarker);
  vAddStep("Marker[].newErrorMarker"_Stz, &Step::vMarkerNewErrorMarker);
  vAddStep("Marker[].newErroredMarker"_Stz,&Step::vMarkerNewErroredMarker);
  vAddStep("Marker[].newExitMarker"_Stz,  &Step::vMarkerNewExitMarker);
  vAddStep("Marker[].newExitEarlyMarker"_Stz,&Step::vMarkerNewExitEarlyMarker);
  vAddStep("Marker[].newExitedMarker"_Stz,&Step::vMarkerNewExitedMarker);
  vAddStep("Marker[].newExitedEarlyMarker"_Stz,&Step::vMarkerNewExitedEarlyMarker);
  vAddStep("Marker[].newNextBufferMarker"_Stz,&Step::vMarkerNewNextBufferMarker);
  vAddStep("Marker[].newPulsedMarker"_Stz,&Step::vMarkerNewPulsedMarker);
  vAddStep("Marker[].newStartedMarker"_Stz, &Step::vMarkerNewStartedMarker);
  vAddStep("Marker[].newStartMarker"_Stz, &Step::vMarkerNewStartMarker);
  vAddStep("Marker[].newStartCrwRhtMarker"_Stz, &Step::vMarkerNewStartCrwRhtMarker);
  vAddStep("Marker[].newStartRowRhtMarker"_Stz, &Step::vMarkerNewStartRowRhtMarker);
  vAddStep("Marker[].pipeIn"_Stz,         &Step::vMarkerPipeIn);
  vAddStep("Marker[].pipeOutSet"_Stz,     &Step::vMarkerPipeOutSet);
  vAddStep("Marker[].pipeToOutput"_Stz,   &Step::vMarkerPipeToOutput);
  vAddStep("Marker[].set"_Stz,            &Step::vMarkerSet);
}

/*----------------------------------------------------------------------------*/
/* Marker[].delete -- operand delete                                         */
/*----------------------------------------------------------------------------*/
void Step::vMarkerDelete(void)
{
  ENTRY();
  
  AM::vDeleteOperand(*m_pJob->pLogicUnit_job()->m_xpOperandA);
  
  m_pJob->pLogicUnit_job()->m_xpOperandA = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].get -- fetch operand                                             */
/*----------------------------------------------------------------------------*/
void Step::vMarkerGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pJob->pLogicUnit_job()->m_xpOperandA);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].getAndClear -- fetch operand and set to nullptr                  */
/*----------------------------------------------------------------------------*/
void Step::vMarkerGetAndClear(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pJob->pLogicUnit_job()->m_xpOperandA);
  
  m_pJob->pLogicUnit_job()->m_xpOperandA = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].isNotNull -- is operand not null ?                               */
/*----------------------------------------------------------------------------*/
void Step::vMarkerIsNotNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pJob->pLogicUnit_job()->m_xpOperandA.bIsNull() ? 0 : 1);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].isNull -- is operand null ?                                       */
/*----------------------------------------------------------------------------*/
void Step::vMarkerIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pJob->pLogicUnit_job()->m_xpOperandA.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newCancelMarker -- new a cancel marker                            */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewCancelMarker(void)
{
  ENTRY();
  
  ASSERT (m_pJob->pLogicUnit_job()->m_xpOperandA.bIsNull());
  
  m_pJob->pLogicUnit_job()->m_xpOperandA = new Marker(OM_CANCELMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newDataPulse -- new a data pulse                                  */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewDataPulse(void)
{
  ENTRY();
  
  ASSERT (m_pJob->pLogicUnit_job()->m_xpOperandA.bIsNull());
  
  m_pJob->pLogicUnit_job()->m_xpOperandA = new Marker(OM_DATAPULSE);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newDestroyMarker -- new a destry marker                           */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewDestroyMarker(void)
{
  ENTRY();
  
  ASSERT (m_pJob->pLogicUnit_job()->m_xpOperandA.bIsNull());
  
  m_pJob->pLogicUnit_job()->m_xpOperandA = new Marker(OM_DESTROYMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newEachScanEosMarker -- new an each scan end marker               */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewEachScanEosMarker(void)
{
  ENTRY();

  ASSERT (m_pJob->pLogicUnit_job()->m_xpOperandA.bIsNull());

  m_pJob->pLogicUnit_job()->m_xpOperandA = new Marker(OM_EACHSCANEOSMARKER);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newEndCrwRhtMarker -- new an end crw right marker                 */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewEndCrwRhtMarker(void)
{
  ENTRY();

  ASSERT (m_pJob->pLogicUnit_job()->m_xpOperandA.bIsNull());

  m_pJob->pLogicUnit_job()->m_xpOperandA = new Marker(OM_END_CRWRHTMARKER);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newEndRowRhtMarker -- new an end row right marker                 */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewEndRowRhtMarker(void)
{
  ENTRY();

  ASSERT (m_pJob->pLogicUnit_job()->m_xpOperandA.bIsNull());

  m_pJob->pLogicUnit_job()->m_xpOperandA = new Marker(OM_END_ROWRHTMARKER);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newEosMarker -- new an end marker                                 */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewEosMarker(void)
{
  ENTRY();
  
  ASSERT (m_pJob->pLogicUnit_job()->m_xpOperandA.bIsNull());
  
  m_pJob->pLogicUnit_job()->m_xpOperandA = new Marker(OM_EOSMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newErrorMarker -- new an error marker                             */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewErrorMarker(void)
{
  ENTRY();
  
  //ASSERT (m_pJob->pLogicUnit_job()->m_xpOperandA.bIsNull());
  
  m_pJob->pLogicUnit_job()->m_xpOperandA = new Marker(OM_ERRORMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newErroredMarker -- new an errored marker                         */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewErroredMarker(void)
{
  ENTRY();
  
  ASSERT (m_pJob->pLogicUnit_job()->m_xpOperandA.bIsNull());
  
  m_pJob->pLogicUnit_job()->m_xpOperandA = new Marker(OM_ERROREDMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newExitMarker -- new an exit marker                               */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewExitMarker(void)
{
  ENTRY();
  
  ASSERT (m_pJob->pLogicUnit_job()->m_xpOperandA.bIsNull());
  
  m_pJob->pLogicUnit_job()->m_xpOperandA = new Marker(OM_EXITMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newExitEarlyMarker -- new an exit early marker                    */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewExitEarlyMarker(void)
{
  ENTRY();

  ASSERT (m_pJob->pLogicUnit_job()->m_xpOperandA.bIsNull());

  m_pJob->pLogicUnit_job()->m_xpOperandA = new Marker(OM_EXITEARLYMARKER);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newExitedMarker -- new an exited marker                           */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewExitedMarker(void)
{
  ENTRY();
  
  ASSERT (m_pJob->pLogicUnit_job()->m_xpOperandA.bIsNull());
  
  m_pJob->pLogicUnit_job()->m_xpOperandA = new Marker(OM_EXITEDMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newExitedEarlyMarker -- new an exited early marker                */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewExitedEarlyMarker(void)
{
  ENTRY();

  ASSERT (m_pJob->pLogicUnit_job()->m_xpOperandA.bIsNull());

  m_pJob->pLogicUnit_job()->m_xpOperandA = new Marker(OM_EXITEDEARLYMARKER);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newNextBufferMarker -- new a next buffer marker                   */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewNextBufferMarker(void)
{
  ENTRY();

  ASSERT (m_pJob->pLogicUnit_job()->m_xpOperandA.bIsNull());

  m_pJob->pLogicUnit_job()->m_xpOperandA = new Marker(OM_NEXTBUFFERMARKER);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newPulsedMarker -- new a pulsed marker                            */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewPulsedMarker(void)
{
  ENTRY();
  
  ASSERT (m_pJob->pLogicUnit_job()->m_xpOperandA.bIsNull());
  
  m_pJob->pLogicUnit_job()->m_xpOperandA = new Marker(OM_PULSEDMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newStartedMarker -- new a started marker                          */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewStartedMarker(void)
{
  ENTRY();
  
  ASSERT (m_pJob->pLogicUnit_job()->m_xpOperandA.bIsNull());
  
  m_pJob->pLogicUnit_job()->m_xpOperandA = new Marker(OM_STARTEDMARKER);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newStartMarker -- new a start marker                              */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewStartMarker(void)
{
  ENTRY();

  ASSERT (m_pJob->pLogicUnit_job()->m_xpOperandA.bIsNull());

  Operand* pOperand = new Marker(OM_STARTMARKER);
  m_pJob->pLogicUnit_job()->m_xpOperandA = pOperand;

  Op* pOp = m_pJob->m_pOp;
  pOp->vOpTrace(pOperand,(Stz*)__func__,true);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newStartCrwRhtMarker -- new a start crw right marker              */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewStartCrwRhtMarker(void)
{
  ENTRY();

  ASSERT (m_pJob->pLogicUnit_job()->m_xpOperandA.bIsNull());

  m_pJob->pLogicUnit_job()->m_xpOperandA = new Marker(OM_START_CRWRHTMARKER);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newStartRowRhtMarker -- new a start crw right marker              */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewStartRowRhtMarker(void)
{
  ENTRY();

  ASSERT (m_pJob->pLogicUnit_job()->m_xpOperandA.bIsNull());

  m_pJob->pLogicUnit_job()->m_xpOperandA = new Marker(OM_START_ROWRHTMARKER);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].pipeIn -- move pipein operand into operandA                      */
/*----------------------------------------------------------------------------*/
void Step::vMarkerPipeIn(void)
{
  ENTRY();
  
  Operand* pOperand = * m_pJob->pLogicUnit_job()->m_xpPipein;

  m_pJob->pLogicUnit_job()->m_xpPipein = nullptr;
    
  m_pJob->pLogicUnit_job()->m_xpOperandA = pOperand;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].pipeOutSet -- append operand set onto next operator in pipeline   */
/*----------------------------------------------------------------------------*/
void Step::vMarkerPipeOutSet(void)
{
  ENTRY();
  
  ASSERT(*m_pJob->pLogicUnit_job()->m_xpOperandA);
  
  Op* pOp = m_pJob->m_pOp; //  pJob_Distributed->m_pOp;
  
  pOp->vPipeToNextOp(*m_pJob->pLogicUnit_job()->m_xpOperandA,this);
  
  m_pJob->pLogicUnit_job()->m_xpOperandA = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].pipeToOutput -- append operand onto the pipeline job buffer queue */
/*----------------------------------------------------------------------------*/
void Step::vMarkerPipeToOutput(void)
{
  ENTRY();
  
  ASSERT(*m_pJob->pLogicUnit_job()->m_xpOperandA);
  
  Op* pOp = m_pJob->m_pOp; //  pJob_Distributed->m_pOp;
  
  pOp->vPipeToOutput(*m_pJob->pLogicUnit_job()->m_xpOperandA);
  
  m_pJob->pLogicUnit_job()->m_xpOperandA = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].set -- set operand                                               */
/*----------------------------------------------------------------------------*/
void Step::vMarkerSet(void)
{
  ENTRY();
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  
  ASSERT(! pOperand || m_pJob->pLogicUnit_job()->m_xpOperandA.bIsNull());
  
  m_pJob->pLogicUnit_job()->m_xpOperandA = _pOperand(pOperand);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
