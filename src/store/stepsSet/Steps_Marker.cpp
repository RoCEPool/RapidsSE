/*============================================================================*/
/* Steps_Error.cpp -- Register Operand step group                             */
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
  vAddStep("Marker[]"_Stz,                      &Step::vMarkerGet);
  vAddStep("Marker[]\\clear"_Stz,               &Step::vMarkerGetAndClear);
  vAddStep("Marker[].delete"_Stz,               &Step::vMarkerDelete);
  vAddStep("Marker[].isNotNull"_Stz,            &Step::vMarkerIsNotNull);
  vAddStep("Marker[].isNull"_Stz,               &Step::vMarkerIsNull);
  vAddStep("Marker[].newCancelling"_Stz,        &Step::vMarkerNewCancelling);
  vAddStep("Marker[].newCancelledMarker"_Stz,   &Step::vMarkerNewCancelledMarker);
  vAddStep("Marker[].newCsvBuffer"_Stz,         &Step::vMarkerNewCsvBuffer);
  vAddStep("Marker[].newCleaningMarker"_Stz,    &Step::vMarkerNewCleaningMarker);
  vAddStep("Marker[].newCleanedupMarker"_Stz,   &Step::vMarkerNewCleanedupMarker);
  vAddStep("Marker[].newDataPulse"_Stz,         &Step::vMarkerNewDataPulse);
  vAddStep("Marker[].newDestroyMarker"_Stz,     &Step::vMarkerNewDestroyMarker);
  vAddStep("Marker[].newEachScanEosMarker"_Stz, &Step::vMarkerNewEachScanEosMarker);
  vAddStep("Marker[].newEndMarker"_Stz,         &Step::vMarkerNewEndMarker);
  vAddStep("Marker[].newEndingMarker"_Stz,      &Step::vMarkerNewEndingMarker);
  vAddStep("Marker[].newEndedMarker"_Stz,       &Step::vMarkerNewEndedMarker);
  vAddStep("Marker[].newEndCrwRhtMarker"_Stz,   &Step::vMarkerNewEndCrwRhtMarker);
  vAddStep("Marker[].newEndRowRhtMarker"_Stz,   &Step::vMarkerNewEndRowRhtMarker);
  vAddStep("Marker[].newEosMarker"_Stz,         &Step::vMarkerNewEosMarker);
  vAddStep("Marker[].newErroringMarker"_Stz,    &Step::vMarkerNewErroringMarker);
  vAddStep("Marker[].newErroredMarker"_Stz,     &Step::vMarkerNewErroredMarker);
  vAddStep("Marker[].newExitingMarker"_Stz,     &Step::vMarkerNewExitingMarker);
  vAddStep("Marker[].newExitedMarker"_Stz,      &Step::vMarkerNewExitedMarker);
  vAddStep("Marker[].newLeaveMarker"_Stz,       &Step::vMarkerNewLeaveMarker);
  vAddStep("Marker[].newLeavingMarker"_Stz,     &Step::vMarkerNewLeavingMarker);
  vAddStep("Marker[].newLeftMarker"_Stz,        &Step::vMarkerNewLeftMarker);
  vAddStep("Marker[].newNextBufferMarker"_Stz,  &Step::vMarkerNewNextBufferMarker);
  vAddStep("Marker[].newStartedMarker"_Stz,     &Step::vMarkerNewStartedMarker);
  vAddStep("Marker[].newStartMarker"_Stz,       &Step::vMarkerNewStartMarker);
  vAddStep("Marker[].newStartCrwRhtMarker"_Stz, &Step::vMarkerNewStartCrwRhtMarker);
  vAddStep("Marker[].newStartRowRhtMarker"_Stz, &Step::vMarkerNewStartRowRhtMarker);
  vAddStep("Marker[].newStreamDefRht"_Stz,      &Step::vMarkerNewStreamDefRht);
  vAddStep("Marker[].newTrigger"_Stz,           &Step::vMarkerNewTrigger);
  vAddStep("Marker[].pipeIn"_Stz,               &Step::vMarkerPipeIn);
  vAddStep("Marker[].pipeToOutput"_Stz,         &Step::vMarkerPipeToOutput);
  vAddStep("Marker[].set"_Stz,                  &Step::vMarkerSet);
}

/*----------------------------------------------------------------------------*/
/* Marker[].delete -- operand delete                                          */
/*----------------------------------------------------------------------------*/
void Step::vMarkerDelete(void)
{
  ENTRY();
  
  AM::vDeleteOperand(*m_p_TaskLogicUnit->m_xpMarker);
  
  m_p_TaskLogicUnit->m_xpMarker = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].get -- fetch operand                                              */
/*----------------------------------------------------------------------------*/
void Step::vMarkerGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpMarker);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].getAndClear -- fetch operand and set to nullptr                   */
/*----------------------------------------------------------------------------*/
void Step::vMarkerGetAndClear(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpMarker);
  
  m_p_TaskLogicUnit->m_xpMarker = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].isNotNull -- is narker not null ?                                 */
/*----------------------------------------------------------------------------*/
void Step::vMarkerIsNotNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xpMarker.bIsNull() ? 0 : 1);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].isNull -- is operand null ?                                       */
/*----------------------------------------------------------------------------*/
void Step::vMarkerIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xpMarker.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newCancelling -- new a cancelling marker                          */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewCancelling(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpMarker.bIsNull());

  m_p_TaskLogicUnit->m_xpMarker = new Marker(OM_CANCELLING);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newCancelledMarker -- new a cancelled marker                      */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewCancelledMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpMarker.bIsNull());

  m_p_TaskLogicUnit->m_xpMarker = new Marker(OM_CANCELLEDMARKER);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newCsbBuffer -- new a csv buffer                                  */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewCsvBuffer(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpMarker.bIsNull());

  m_p_TaskLogicUnit->m_xpMarker = new CsvBuffer;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newCleaningMarker -- new a cleanup marker                         */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewCleaningMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpMarker.bIsNull());

  m_p_TaskLogicUnit->m_xpMarker = new Marker(OM_CLEANING);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newCleanedupMarker -- new a cleanedup marker                      */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewCleanedupMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpMarker.bIsNull());

  m_p_TaskLogicUnit->m_xpMarker = new Marker(OM_CLEANED);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newDataPulse -- new a data pulse                                  */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewDataPulse(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpMarker.bIsNull());

  m_p_TaskLogicUnit->m_xpMarker = new Marker(OM_DATAPULSE);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newDestroyMarker -- new a destry marker                           */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewDestroyMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpMarker.bIsNull());

  m_p_TaskLogicUnit->m_xpMarker = new Marker(OM_DESTROYMARKER);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newEachScanEosMarker -- new an each scan end marker               */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewEachScanEosMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpMarker.bIsNull());

  m_p_TaskLogicUnit->m_xpMarker = new Marker(OM_EACHSCANEOSMARKER);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newEnd -- new an end marker                                       */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewEndMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpMarker.bIsNull());

  m_p_TaskLogicUnit->m_xpMarker = new Marker(OM_ENDMARKER);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newEnded -- new an ended marker                                   */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewEndedMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpMarker.bIsNull());

  m_p_TaskLogicUnit->m_xpMarker = new Marker(OM_ENDEDMARKER);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newEnding -- new an ending marker                                 */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewEndingMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpMarker.bIsNull());

  m_p_TaskLogicUnit->m_xpMarker = new Marker(OM_ENDINGMARKER);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newEndCrwRhtMarker -- new an end crw right marker                 */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewEndCrwRhtMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpMarker.bIsNull());

  m_p_TaskLogicUnit->m_xpMarker = new Marker(OM_ENDCRWRHT);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newEndRowRhtMarker -- new an end row right marker                 */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewEndRowRhtMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpMarker.bIsNull());

  m_p_TaskLogicUnit->m_xpMarker = new Marker(OM_ENDROWRHT);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newEosMarker -- new an end marker                                 */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewEosMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpMarker.bIsNull());

  m_p_TaskLogicUnit->m_xpMarker = new Marker(OM_EOSMARKER);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newErroringMarker -- new an erroring marker                       */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewErroringMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpMarker.bIsNull());

  m_p_TaskLogicUnit->m_xpMarker = new Marker(OM_ERRORINGMARKER);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newErroredMarker -- new an errored marker                         */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewErroredMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpMarker.bIsNull());

  m_p_TaskLogicUnit->m_xpMarker = new Marker(OM_ERROREDMARKER);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newExitingMarker -- new an exiting marker                        */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewExitingMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpMarker.bIsNull());

  m_p_TaskLogicUnit->m_xpMarker = new Marker(OM_EXITING);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newExitedMarker -- new an exited marker                           */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewExitedMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpMarker.bIsNull());

  m_p_TaskLogicUnit->m_xpMarker = new Marker(OM_EXITED);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newLeaveMarker -- new a leave marker                              */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewLeaveMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpMarker.bIsNull());
  m_p_TaskLogicUnit->m_xpMarker = new Marker(OM_LEAVEMARKER);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newLeavingMarker -- new a leaving marker                          */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewLeavingMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpMarker.bIsNull());

  m_p_TaskLogicUnit->m_xpMarker = new Marker(OM_LEAVINGMARKER);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newLeftMarker -- new a left marker                                */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewLeftMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpMarker.bIsNull());

  m_p_TaskLogicUnit->m_xpMarker = new Marker(OM_LEFTMARKER);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newNextBufferMarker -- new a next buffer marker                   */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewNextBufferMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpMarker.bIsNull());

  m_p_TaskLogicUnit->m_xpMarker = new Marker(OM_NEXTBUFFERMARKER);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newStartedMarker -- new a started marker                          */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewStartedMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpMarker.bIsNull());

  m_p_TaskLogicUnit->m_xpMarker = new Marker(OM_STARTEDMARKER);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newStartMarker -- new a start marker                              */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewStartMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpMarker.bIsNull());

  // revised job/op model
  Op* pOp = *m_pOp;

  Operand* pOperand = new Marker(OM_STARTMARKER);
  m_p_TaskLogicUnit->m_xpMarker = pOperand;

  pOp->vOpTrace(pOperand,(Stz*)__func__,true);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newStartCrwRhtMarker -- new a start crw right marker              */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewStartCrwRhtMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpMarker.bIsNull());

  m_p_TaskLogicUnit->m_xpMarker = new Marker(OM_STARTCRWRHT);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newStartRowRhtMarker -- new a start crw right marker              */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewStartRowRhtMarker(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpMarker.bIsNull());

  m_p_TaskLogicUnit->m_xpMarker = new Marker(OM_STARTROWRHT);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newStreamDefRht -- new a streamDefRht marker                      */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewStreamDefRht(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpMarker.bIsNull());

  m_p_TaskLogicUnit->m_xpMarker = new Marker(OM_STREAMDEFRHT);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newTrigger -- new a trigger marker                                */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewTrigger(void)
{
  ENTRY();

  ASSERT (m_p_TaskLogicUnit->m_xpMarker.bIsNull());

  m_p_TaskLogicUnit->m_xpMarker = new Marker(OM_TRIGGER);

  EXIT();
}



/*----------------------------------------------------------------------------*/
/* Marker[].pipeIn -- move pipein operand into Marker                         */
/*----------------------------------------------------------------------------*/
void Step::vMarkerPipeIn(void)
{
  ENTRY();
  
  Operand* pOperand = * m_p_TaskLogicUnit->m_xpPipein;

  m_p_TaskLogicUnit->m_xpPipein = nullptr;
    
  m_p_TaskLogicUnit->m_xpMarker = pOperand;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].pipeToOutput -- append operand to pipeline job buffer queue       */
/*----------------------------------------------------------------------------*/
void Step::vMarkerPipeToOutput(void)
{
  ENTRY();
  
  ASSERT(*m_p_TaskLogicUnit->m_xpMarker);
  
  // revised job/op model
    Op* pOp = *m_pOp;

  pOp->vPipeToOutput(*m_p_TaskLogicUnit->m_xpMarker);
  
  m_p_TaskLogicUnit->m_xpMarker = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].set -- set operand                                                */
/*----------------------------------------------------------------------------*/
void Step::vMarkerSet(void)
{
  ENTRY();
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  
  ASSERT(! pOperand || m_p_TaskLogicUnit->m_xpMarker.bIsNull());
  
  m_p_TaskLogicUnit->m_xpMarker = _pOperand(pOperand);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
