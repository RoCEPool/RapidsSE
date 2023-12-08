/*============================================================================*/
/* Steps_Error.cpp -- Register Operand step group                             */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eYX.h"

#include "Marker.hpp"

#include "RowBuffer.hpp"

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


//#ifdef WITH_PYTHON_SUPPORT
  //#include "/usr/include/python2.7/Python.h"
//#endif

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
  vAddStep("Marker[].newEndMarker"_Stz,         &Step::vMarkerNewEndMarker);
  vAddStep("Marker[].newEndingMarker"_Stz,      &Step::vMarkerNewEndingMarker);
  vAddStep("Marker[].newEndedMarker"_Stz,       &Step::vMarkerNewEndedMarker);
  vAddStep("Marker[].newEosMarker"_Stz,         &Step::vMarkerNewEosMarker);
  vAddStep("Marker[].newErroringMarker"_Stz,    &Step::vMarkerNewErroringMarker);
  vAddStep("Marker[].newErroredMarker"_Stz,     &Step::vMarkerNewErroredMarker);
  vAddStep("Marker[].newExit"_Stz,              &Step::vMarkerNewExitMarker);
  vAddStep("Marker[].newExitingMarker"_Stz,     &Step::vMarkerNewExitingMarker);
  vAddStep("Marker[].newExitedMarker"_Stz,      &Step::vMarkerNewExitedMarker);
  vAddStep("Marker[].newFinishedMarker"_Stz,    &Step::vMarkerNewFinishedMarker);
  vAddStep("Marker[].newFlushMarker"_Stz,       &Step::vMarkerNewFlushMarker);
  vAddStep("Marker[].newInternalError"_Stz,      &Step::vMarkerNewInternalError);
  vAddStep("Marker[].newInvokeMarker"_Stz,      &Step::vMarkerNewInvokeMarker);
  vAddStep("Marker[].newLeaveMarker"_Stz,       &Step::vMarkerNewLeaveMarker);
  vAddStep("Marker[].newLeavingMarker"_Stz,     &Step::vMarkerNewLeavingMarker);
  vAddStep("Marker[].newLeftMarker"_Stz,        &Step::vMarkerNewLeftMarker);
  vAddStep("Marker[].newLowMemory"_Stz,         &Step::vMarkerNewLowMemory);
  vAddStep("Marker[].newStartedMarker"_Stz,     &Step::vMarkerNewStartedMarker);
  vAddStep("Marker[].newStartMarker"_Stz,       &Step::vMarkerNewStartMarker);
  vAddStep("Marker[].newStreamDefRht"_Stz,      &Step::vMarkerNewStreamDefRht);
  vAddStep("Marker[].newTrigger"_Stz,           &Step::vMarkerNewTrigger);
  vAddStep("Marker[].newTriggeredMarker"_Stz,   &Step::vMarkerNewTriggeredMarker);
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
  
  AM::vDeleteOperand(*m_pLogicUnit->m_xpMarker,nullptr);
  
  m_pLogicUnit->m_xpMarker = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].get -- fetch operand                                              */
/*----------------------------------------------------------------------------*/
void Step::vMarkerGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpMarker);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].getAndClear -- fetch operand and set to nullptr                   */
/*----------------------------------------------------------------------------*/
void Step::vMarkerGetAndClear(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpMarker);
  
  m_pLogicUnit->m_xpMarker = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].isNotNull -- is narker not null ?                                 */
/*----------------------------------------------------------------------------*/
void Step::vMarkerIsNotNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpMarker.bIsNull() ? 0 : 1);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].isNull -- is operand null ?                                       */
/*----------------------------------------------------------------------------*/
void Step::vMarkerIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_pLogicUnit->m_xpMarker.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newCancelling -- new a cancelling marker                          */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewCancelling(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpMarker.bIsNull());

  m_pLogicUnit->m_xpMarker =
    AM::MARKER::pNewMarker(OM_CANCELLING,pOuterJobOfTask());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newCancelledMarker -- new a cancelled marker                      */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewCancelledMarker(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpMarker.bIsNull());

  m_pLogicUnit->m_xpMarker =
    AM::MARKER::pNewMarker(OM_CANCELLEDMARKER,pOuterJobOfTask());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newCsbBuffer -- new a csv buffer                                  */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewCsvBuffer(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpMarker.bIsNull());

  m_pLogicUnit->m_xpMarker = new CsvBuffer;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newCleaningMarker -- new a cleanup marker                         */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewCleaningMarker(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpMarker.bIsNull());

  m_pLogicUnit->m_xpMarker =
    AM::MARKER::pNewMarker(OM_CLEANING,pOuterJobOfTask());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newCleanedupMarker -- new a cleanedup marker                      */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewCleanedupMarker(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpMarker.bIsNull());

  m_pLogicUnit->m_xpMarker =
    AM::MARKER::pNewMarker(OM_CLEANED,pOuterJobOfTask());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newDataPulse -- new a data pulse                                  */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewDataPulse(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpMarker.bIsNull());

  m_pLogicUnit->m_xpMarker =
    AM::MARKER::pNewMarker(OM_DATAPULSE,pOuterJobOfTask());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newDestroyMarker -- new a destry marker                           */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewDestroyMarker(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpMarker.bIsNull());

  m_pLogicUnit->m_xpMarker =
    AM::MARKER::pNewMarker(OM_DESTROYMARKER,pOuterJobOfTask());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newEnd -- new an end marker                                       */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewEndMarker(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpMarker.bIsNull());

  m_pLogicUnit->m_xpMarker =
    AM::MARKER::pNewMarker(OM_ENDMARKER,pOuterJobOfTask());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newEnded -- new an ended marker                                   */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewEndedMarker(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpMarker.bIsNull());

  m_pLogicUnit->m_xpMarker =
    AM::MARKER::pNewMarker(OM_ENDEDMARKER,pOuterJobOfTask());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newEnding -- new an ending marker                                 */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewEndingMarker(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpMarker.bIsNull());

  m_pLogicUnit->m_xpMarker =
    AM::MARKER::pNewMarker(OM_ENDINGMARKER,pOuterJobOfTask());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newEosMarker -- new an end marker                                 */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewEosMarker(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpMarker.bIsNull());

  m_pLogicUnit->m_xpMarker =
    AM::MARKER::pNewMarker(OM_EOSMARKER,pOuterJobOfTask());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newErroringMarker -- new an erroring marker                       */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewErroringMarker(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpMarker.bIsNull());

  m_pLogicUnit->m_xpMarker =
    AM::MARKER::pNewMarker(OM_ERRORINGMARKER,pOuterJobOfTask());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newErroredMarker -- new an errored marker                         */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewErroredMarker(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpMarker.bIsNull());

  m_pLogicUnit->m_xpMarker =
    AM::MARKER::pNewMarker(OM_ERROREDMARKER,pOuterJobOfTask());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newExitMarker -- new an exit marker                               */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewExitMarker(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpMarker.bIsNull());

  m_pLogicUnit->m_xpMarker =
      AM::MARKER::pNewMarker(OM_EXIT,pOuterJobOfTask());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newExitingMarker -- new an exiting marker                        */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewExitingMarker(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpMarker.bIsNull());

  m_pLogicUnit->m_xpMarker =
    AM::MARKER::pNewMarker(OM_EXITING,pOuterJobOfTask());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newExitedMarker -- new an exited marker                           */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewExitedMarker(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpMarker.bIsNull());

  m_pLogicUnit->m_xpMarker =
    AM::MARKER::pNewMarker(OM_EXITED,pOuterJobOfTask());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newFinishedMarker -- new a finished marker                        */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewFinishedMarker(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpMarker.bIsNull());

  m_pLogicUnit->m_xpMarker =
    AM::MARKER::pNewMarker(OM_FINISHED,pOuterJobOfTask());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newFlushMarker -- new a flush marker                              */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewFlushMarker(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpMarker.bIsNull());

  m_pLogicUnit->m_xpMarker =
    AM::MARKER::pNewMarker(OM_FLUSH,pOuterJobOfTask());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newInternalError -- new a InternalError marker                    */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewInternalError(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpMarker.bIsNull());

  m_pLogicUnit->m_xpMarker =
      AM::MARKER::pNewMarker(OM_INTERNALERROR,pOuterJobOfTask());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newInvokeMarker -- new an invoke marker                           */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewInvokeMarker(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpMarker.bIsNull());

  m_pLogicUnit->m_xpMarker =
    AM::MARKER::pNewMarker(OM_INVOKEMARKER,pOuterJobOfTask());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newLeaveMarker -- new a leave marker                              */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewLeaveMarker(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpMarker.bIsNull());
  m_pLogicUnit->m_xpMarker =
    AM::MARKER::pNewMarker(OM_LEAVEMARKER,pOuterJobOfTask());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newLeavingMarker -- new a leaving marker                          */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewLeavingMarker(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpMarker.bIsNull());

  m_pLogicUnit->m_xpMarker =
    AM::MARKER::pNewMarker(OM_LEAVINGMARKER,pOuterJobOfTask());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newLeftMarker -- new a left marker                                */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewLeftMarker(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpMarker.bIsNull());

  m_pLogicUnit->m_xpMarker =
    AM::MARKER::pNewMarker(OM_LEFTMARKER,pOuterJobOfTask());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newLowMemory -- new a lowMemory marker                            */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewLowMemory(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpMarker.bIsNull());

  m_pLogicUnit->m_xpMarker =
      AM::MARKER::pNewMarker(OM_LOWMEMORY,pOuterJobOfTask());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newStartedMarker -- new a started marker                          */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewStartedMarker(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpMarker.bIsNull());

  m_pLogicUnit->m_xpMarker =
    AM::MARKER::pNewMarker(OM_STARTEDMARKER,pOuterJobOfTask());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newStartMarker -- new a start marker                              */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewStartMarker(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpMarker.bIsNull());

  // revised job/op model
  //Op* pOp = *m_pOp;

  m_pLogicUnit->m_xpMarker =
    AM::MARKER::pNewMarker(OM_STARTMARKER,pOuterJobOfTask());

  //pOp->vOpTrace(pOperand,(Stz*)__func__,true);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newStreamDefRht -- new a streamDefRht marker                      */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewStreamDefRht(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpMarker.bIsNull());

  m_pLogicUnit->m_xpMarker =
    AM::MARKER::pNewMarker(OM_STREAMDEFRHT,pOuterJobOfTask());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newTrigger -- new a trigger marker                                */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewTrigger(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpMarker.bIsNull());

  m_pLogicUnit->m_xpMarker =
    AM::MARKER::pNewMarker(OM_TRIGGER,pOuterJobOfTask());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].newTriggered -- new a triggered marker                            */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewTriggeredMarker(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpMarker.bIsNull());

  m_pLogicUnit->m_xpMarker =
    AM::MARKER::pNewMarker(OM_TRIGGERED,pOuterJobOfTask());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].pipeIn -- move pipein operand into Marker                         */
/*----------------------------------------------------------------------------*/
void Step::vMarkerPipeIn(void)
{
  ENTRY();
  
  Operand* pOperand = * m_pLogicUnit->m_xpPipein;

  m_pLogicUnit->m_xpPipein = nullptr;
    
  m_pLogicUnit->m_xpMarker = pOperand;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].pipeToOutput -- append operand to pipeline job buffer queue       */
/*----------------------------------------------------------------------------*/
void Step::vMarkerPipeToOutput(void)
{
  ENTRY();
  
  ASSERT(*m_pLogicUnit->m_xpMarker);
  
  // revised job/op model
    Op* pOp = *m_pOp;

  pOp->vPipeToOutput(*m_pLogicUnit->m_xpMarker);
  
  m_pLogicUnit->m_xpMarker = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Marker[].set -- set operand                                                */
/*----------------------------------------------------------------------------*/
void Step::vMarkerSet(void)
{
  ENTRY();
  
  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));
  
  ASSERT(! pOperand || m_pLogicUnit->m_xpMarker.bIsNull());
  
  m_pLogicUnit->m_xpMarker = _pOperand(pOperand);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
