/*============================================================================*/
/* Steps_Pds.cpp -- Pds                                                       */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "LogicUnit.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Pds(void)
{
  vAddStep("Pds[]"_Stz,                     &Step::vPdsGet);

  vAddStep("Pds[].bufferId"_Stz  ,          &Step::vPdsGetBufferIdAsAddr);
  vAddStep("Pds[].delete"_Stz  ,            &Step::vPdsDelete);
  vAddStep("Pds[].isNull"_Stz  ,            &Step::vPdsIsNull);
  vAddStep("Pds[].set"_Stz  ,               &Step::vPdsSet);
}

/*----------------------------------------------------------------------------*/
/* Pds[] -- get pds                                                           */
/*----------------------------------------------------------------------------*/
void Step::vPdsGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpPds);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].bufferId -- fetch buffer id as address                               */
/*----------------------------------------------------------------------------*/
void Step::vPdsGetBufferIdAsAddr(void)
{
  ENTRY();

  Pds* pPds = *m_pLogicUnit->m_xpPds;

  m_pDataStack->vPushPtr((Object*)pPds->oGetBufferId().m_pvAsAddress);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].delete -- operand delete                                             */
/*----------------------------------------------------------------------------*/
void Step::vPdsDelete(void)
{
  ENTRY();

  AM::vDeleteOperand(*m_pLogicUnit->m_xpPds,nullptr);

  m_pLogicUnit->m_xpPds = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].isNull --                                                            */
/*----------------------------------------------------------------------------*/
void Step::vPdsIsNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpPds.bIsNull() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].set -- set the pds                                                   */
/*----------------------------------------------------------------------------*/
void Step::vPdsSet(void)
{
  ENTRY();

  m_pLogicUnit->m_xpPds = _pPds(m_pDataStack->pPopPtr(__func__));

  EXIT();
}
