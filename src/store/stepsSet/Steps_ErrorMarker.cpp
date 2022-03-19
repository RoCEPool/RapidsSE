/*============================================================================*/
/* Steps_Error.cpp - error step group                                         */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eYX.h"

#include "Marker.hpp"
#include "Op.hpp"
#include "LogicUnit.hpp"

#include "Parcel.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ErrorMarker(void)
{
  vAddStep("ErrorMarker[]"_Stz,                &Step::vErrorMarkerGet);
  vAddStep("ErrorMarker[]\\clear"_Stz,         &Step::vErrorMarkerGetAndClear);
  vAddStep("ErrorMarker[].delete"_Stz,         &Step::vErrorMarkerDelete);
  vAddStep("ErrorMarker[].isNotNull"_Stz,      &Step::vErrorMarkerIsNotNull);
  vAddStep("ErrorMarker[].isNull"_Stz,         &Step::vErrorMarkerIsNull);
  vAddStep("ErrorMarker[].newErrorMarker"_Stz, &Step::vErrorMarkerNewErrorMarker);
  vAddStep("ErrorMarker[].pipeIn"_Stz,         &Step::vErrorMarkerPipeIn);
  vAddStep("ErrorMarker[].pipeOut"_Stz,        &Step::vErrorMarkerPipeOut);
  vAddStep("ErrorMarker[].pipeToOutput"_Stz,   &Step::vErrorMarkerPipeToOutput);
  vAddStep("ErrorMarker[].set"_Stz,            &Step::vErrorMarkerSet);
}

/*----------------------------------------------------------------------------*/
/* ErrorMarker[].delete -- operand delete                                     */
/*----------------------------------------------------------------------------*/
void Step::vErrorMarkerDelete(void)
{
  ENTRY();

  AM::vDeleteOperand(*m_pLogicUnit->m_xpErrorMarker);

  m_pLogicUnit->m_xpErrorMarker = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ErrorMarker[].get -- fetch operand                                         */
/*----------------------------------------------------------------------------*/
void Step::vErrorMarkerGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpErrorMarker);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ErrorMarker[].getAndClear -- fetch operand and set to nullptr              */
/*----------------------------------------------------------------------------*/
void Step::vErrorMarkerGetAndClear(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpErrorMarker);

  m_pLogicUnit->m_xpErrorMarker = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ErrorMarker[].isNotNull -- is narker not null ?                            */
/*----------------------------------------------------------------------------*/
void Step::vErrorMarkerIsNotNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpErrorMarker.bIsNull() ? 0 : 1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ErrorMarker[].isNull -- is operand null ?                                  */
/*----------------------------------------------------------------------------*/
void Step::vErrorMarkerIsNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpErrorMarker.bIsNull() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ErrorMarker[].newErrorMarker -- new an error marker                        */
/*----------------------------------------------------------------------------*/
void Step::vErrorMarkerNewErrorMarker(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpErrorMarker.bIsNull());

  m_pLogicUnit->m_xpErrorMarker = new Marker(OM_ERRORMARKER);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ErrorMarker[].pipeIn -- move pipein operand into Marker                    */
/*----------------------------------------------------------------------------*/
void Step::vErrorMarkerPipeIn(void)
{
  ENTRY();

  Operand* pOperand = * m_pLogicUnit->m_xpPipein;

  m_pLogicUnit->m_xpPipein = nullptr;

  m_pLogicUnit->m_xpErrorMarker = pOperand;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ErrorMarker[].pipeOut -- append marker onto next operator in pipeline      */
/*----------------------------------------------------------------------------*/
void Step::vErrorMarkerPipeOut(void)
{
  ENTRY();

  ASSERT(*m_pLogicUnit->m_xpErrorMarker);

  Op* pOp = *m_pOp;

  AM::OP::vPipeToNextOp(pOp,*m_pLogicUnit->m_xpErrorMarker,this);
  m_pLogicUnit->m_xpErrorMarker = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ErrorMarker[].pipeToOutput -- append operand to pipeline job buffer queue  */
/*----------------------------------------------------------------------------*/
void Step::vErrorMarkerPipeToOutput(void)
{
  ENTRY();

  ASSERT(*m_pLogicUnit->m_xpErrorMarker);

  // revised job/op model
  Op* pOp = *m_pOp;

  pOp->vPipeToOutput(*m_pLogicUnit->m_xpErrorMarker);

  m_pLogicUnit->m_xpErrorMarker = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ErrorMarker[].set -- set operand                                           */
/*----------------------------------------------------------------------------*/
void Step::vErrorMarkerSet(void)
{
  ENTRY();

  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

  ASSERT(! pOperand || m_pLogicUnit->m_xpErrorMarker.bIsNull());

  m_pLogicUnit->m_xpErrorMarker = _pOperand(pOperand);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
