/*============================================================================*/
/* Steps_PlanOperand.cpp -- Register PlanOperand step group                           */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eYX.h"

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
#include "MY.hpp"
#include "PlanOperand.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_PlanOperand(void)
{
  vAddStep("PlanOperand[]"_Stz,                &Step::vPlanOperandGet);
  vAddStep("PlanOperand[]\\clear"_Stz,         &Step::vPlanOperandGetAndClear);
  vAddStep("PlanOperand[].delete"_Stz,         &Step::vPlanOperandDelete);
  vAddStep("PlanOperand[].isNotNull"_Stz,      &Step::vPlanOperandIsNotNull);
  vAddStep("PlanOperand[].isNull"_Stz,         &Step::vPlanOperandIsNull);
  vAddStep("PlanOperand[].set"_Stz,            &Step::vPlanOperandSet);
}

/*----------------------------------------------------------------------------*/
/* PlanOperand[].delete -- planOperand delete                                 */
/*----------------------------------------------------------------------------*/
void Step::vPlanOperandDelete(void)
{
  ENTRY();
  
  AM::vDeleteOperand(*m_p_TaskLogicUnit->m_xpPlanOperand);
  
  m_p_TaskLogicUnit->m_xpPlanOperand = nullptr;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PlanOperand[].get -- fetch planOperand                                     */
/*----------------------------------------------------------------------------*/
void Step::vPlanOperandGet(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpPlanOperand);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PlanOperand[].getAndClear -- fetch planOperand and set to nullptr          */
/*----------------------------------------------------------------------------*/
void Step::vPlanOperandGetAndClear(void)
{
  ENTRY();
  
  m_pDataStack->vPushPtr(*m_p_TaskLogicUnit->m_xpPlanOperand);
  
  m_p_TaskLogicUnit->m_xpPlanOperand = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PlanOperand[].isNotNull -- is planOperand not null ?                       */
/*----------------------------------------------------------------------------*/
void Step::vPlanOperandIsNotNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xpPlanOperand.bIsNull() ? 0 : 1);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PlanOperand[].isNull -- is planOperand null ?                              */
/*----------------------------------------------------------------------------*/
void Step::vPlanOperandIsNull(void)
{
  ENTRY();
  
  m_pDataStack->vPushInt(m_p_TaskLogicUnit->m_xpPlanOperand.bIsNull() ? 1 : 0);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PlanOperand[].set -- set planOperand                                       */
/*----------------------------------------------------------------------------*/
void Step::vPlanOperandSet(void)
{
  ENTRY();
  
  PlanOperand* pPlanOperand = _pPlanOperand(m_pDataStack->pPopPtr(__func__));
  
  ASSERT(! pPlanOperand || m_p_TaskLogicUnit->m_xpPlanOperand.bIsNull());
  
  m_p_TaskLogicUnit->m_xpPlanOperand = _pPlanOperand(pPlanOperand);
  
  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
