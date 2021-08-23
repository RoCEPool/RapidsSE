/*============================================================================*/
/* Steps_Error.cpp -- Register Operand step group                           */
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
  vAddStep("Marker[].newEosMarker"_Stz,   &Step::vMarkerNewEosMarker);
  vAddStep("Marker[].pipeIn"_Stz,         &Step::vMarkerPipeIn);
  vAddStep("Marker[].pipeOut"_Stz,        &Step::vMarkerPipeOut);
  vAddStep("Marker[].pipeToOutput"_Stz,   &Step::vMarkerPipeToOutput);
  vAddStep("Marker[].set"_Stz,            &Step::vMarkerSet);
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
/* Marker[].newEosMarker -- new an eos marker                                 */
/*----------------------------------------------------------------------------*/
void Step::vMarkerNewEosMarker(void)
{
  ENTRY();
  
  ASSERT (m_p_TaskLogicUnit->m_xpMarker.bIsNull());
  
  m_p_TaskLogicUnit->m_xpMarker = new Marker(OM_EOSMARKER);
  
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
/* Marker[].pipeOut -- append marker onto next operator in pipeline           */
/*----------------------------------------------------------------------------*/
void Step::vMarkerPipeOut(void)
{
  ENTRY();

  ASSERT(*m_p_TaskLogicUnit->m_xpMarker);

  // revised job/op model
    Op* pOp = *m_pOp;

  pOp->vPipeToNextOp(*m_p_TaskLogicUnit->m_xpMarker,this);

  m_p_TaskLogicUnit->m_xpMarker = nullptr;

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
