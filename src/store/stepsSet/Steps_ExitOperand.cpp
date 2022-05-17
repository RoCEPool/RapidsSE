/*============================================================================*/
/* Steps_ExitOperand.cpp - exit operand step group                            */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "eYX.h"

#include "ExitOperand.hpp"
#include "Op.hpp"
#include "LogicUnit.hpp"

#include "Parcel.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_ExitOperand(void)
{
  vAddStep("ExitOperand[]"_Stz,                &Step::vExitOperandGet);
  vAddStep("ExitOperand[]\\clear"_Stz,         &Step::vExitOperandGetAndClear);
  vAddStep("ExitOperand[].delete"_Stz,         &Step::vExitOperandDelete);
  vAddStep("ExitOperand[].isNotNull"_Stz,      &Step::vExitOperandIsNotNull);
  vAddStep("ExitOperand[].isNull"_Stz,         &Step::vExitOperandIsNull);
  vAddStep("ExitOperand[].newExitOperand"_Stz, &Step::vExitOperandNewExitOperand);
  vAddStep("ExitOperand[].pipeIn"_Stz,         &Step::vExitOperandPipeIn);
  vAddStep("ExitOperand[].pipeOut"_Stz,        &Step::vExitOperandPipeOut);
  vAddStep("ExitOperand[].pipeToOutput"_Stz,   &Step::vExitOperandPipeToOutput);
  vAddStep("ExitOperand[].set"_Stz,            &Step::vExitOperandSet);
}

/*----------------------------------------------------------------------------*/
/* ExitOperand[].delete -- operand delete                                     */
/*----------------------------------------------------------------------------*/
void Step::vExitOperandDelete(void)
{
  ENTRY();

  AM::vDeleteOperand(*m_pLogicUnit->m_xpExitOperand);

  m_pLogicUnit->m_xpExitOperand = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ExitOperand[].get -- fetch operand                                         */
/*----------------------------------------------------------------------------*/
void Step::vExitOperandGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpExitOperand);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ExitOperand[].getAndClear -- fetch operand and set to nullptr              */
/*----------------------------------------------------------------------------*/
void Step::vExitOperandGetAndClear(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpExitOperand);

  m_pLogicUnit->m_xpExitOperand = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ExitOperand[].isNotNull -- is narker not null ?                            */
/*----------------------------------------------------------------------------*/
void Step::vExitOperandIsNotNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpExitOperand.bIsNull() ? 0 : 1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ExitOperand[].isNull -- is operand null ?                                  */
/*----------------------------------------------------------------------------*/
void Step::vExitOperandIsNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpExitOperand.bIsNull() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ExitOperand[].newExitOperand -- new an error marker                        */
/*----------------------------------------------------------------------------*/
void Step::vExitOperandNewExitOperand(void)
{
  ENTRY();

  ASSERT (m_pLogicUnit->m_xpExitOperand.bIsNull());

  m_pLogicUnit->m_xpExitOperand = new ExitOperand;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ExitOperand[].pipeIn -- move pipein operand into ExitOperand               */
/*----------------------------------------------------------------------------*/
void Step::vExitOperandPipeIn(void)
{
  ENTRY();

  Operand* pOperand = * m_pLogicUnit->m_xpPipein;

  m_pLogicUnit->m_xpPipein = nullptr;

  m_pLogicUnit->m_xpExitOperand = pOperand;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ExitOperand[].pipeOut -- append exitOperand onto next operator in pipeline */
/*----------------------------------------------------------------------------*/
void Step::vExitOperandPipeOut(void)
{
  ENTRY();

  ASSERT(*m_pLogicUnit->m_xpExitOperand);

  Op* pOp = *m_pOp;

  AM::OP::vPipeToNextOp(pOp,*m_pLogicUnit->m_xpExitOperand,this);
  m_pLogicUnit->m_xpExitOperand = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ExitOperand[].pipeToOutput -- append operand to pipeline job buffer queue  */
/*----------------------------------------------------------------------------*/
void Step::vExitOperandPipeToOutput(void)
{
  ENTRY();

  ASSERT(*m_pLogicUnit->m_xpExitOperand);

  // revised job/op model
  Op* pOp = *m_pOp;

  pOp->vPipeToOutput(*m_pLogicUnit->m_xpExitOperand);

  m_pLogicUnit->m_xpExitOperand = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* ExitOperand[].set -- set operand                                           */
/*----------------------------------------------------------------------------*/
void Step::vExitOperandSet(void)
{
  ENTRY();

  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

  ASSERT(! pOperand || m_pLogicUnit->m_xpExitOperand.bIsNull());

  m_pLogicUnit->m_xpExitOperand = _pOperand(pOperand);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
