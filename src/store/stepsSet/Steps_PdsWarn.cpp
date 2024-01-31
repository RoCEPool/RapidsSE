/*============================================================================*/
/* Steps_PdsWarn.cpp - PdsWarn step group                                     */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"

#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "IndexTree.hpp"

#include "LglPtnMap.hpp"

#include "LogicUnit.hpp"
#include "Index.hpp"
#include "Table.hpp"
#include "Node.hpp"
#include "Grid.hpp"
#include "Op.hpp"
#include "Job.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_PdsWarn(void)
{

  vAddStep("PdsWarn[].bufferId"_Stz,     &Step::vPdsWarnGetBufferIdAsAddr);
  vAddStep("PdsWarn[]"_Stz,              &Step::vPdsWarnGet);
  vAddStep("PdsWarn[].delete"_Stz,          &Step::vPdsWarnDelete);
  vAddStep("PdsWarn[]\\clear"_Stz,       &Step::vPdsWarnGetAndClear);
  vAddStep("PdsWarn[].isNull"_Stz,       &Step::vPdsWarnIsNull);
  vAddStep("PdsWarn[].new"_Stz,          &Step::vPdsWarnNew);
  vAddStep("PdsWarn[].rows"_Stz,         &Step::vPdsWarnRows);
  vAddStep("PdsWarn[].set"_Stz,             &Step::vPdsWarnSet);
}

/*----------------------------------------------------------------------------*/
/* PdsWarn[].get -- fetch operand                                             */
/*----------------------------------------------------------------------------*/
void Step::vPdsWarnGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpPdsWarn);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PdsWarn[].delete -- delete the buffer                                      */
/*----------------------------------------------------------------------------*/
void Step::vPdsWarnDelete(void)
{
  ENTRY();

  AM::vDeleteOperand(*m_pLogicUnit->m_xpPdsWarn,nullptr);

  m_pLogicUnit->m_xpPdsWarn = nullptr;

  EXIT();
}


/*----------------------------------------------------------------------------*/
/* PdsWarn[].bufferId -- fetch buffer id as address                           */
/*----------------------------------------------------------------------------*/
void Step::vPdsWarnGetBufferIdAsAddr(void)
{
  ENTRY();

  Pds* pPds = *m_pLogicUnit->m_xpPdsWarn;

  m_pDataStack->vPushPtr((Object*)pPds->oGetBufferId().m_pvAsAddress);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PdsWarn[].getAndClear -- fetch operand and set to nullptr                  */
/*----------------------------------------------------------------------------*/
void Step::vPdsWarnGetAndClear(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpPdsWarn);

  m_pLogicUnit->m_xpPdsWarn = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PdsWarn[].isNull -- is Pds null ?                                          */
/*----------------------------------------------------------------------------*/
void Step::vPdsWarnIsNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpPdsWarn.bIsNull() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PdsWarn[].new -- new an PdsWarn operand                                    */
/*----------------------------------------------------------------------------*/
void Step::vPdsWarnNew(void)
{
  ENTRY();

  ASSERT(m_pLogicUnit->m_xpPds.bIsNull());

  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  eBT eBufferType = (eBT)m_pDataStack->iPopInt(__func__);

  StreamDef* pStreamDef = pTable->pStreamDef();

  m_pLogicUnit->m_xpPdsWarn = new Pds(pStreamDef, false, eBufferType);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PdsWarn[].rows -- get number of rows                                       */
/*----------------------------------------------------------------------------*/
void Step::vPdsWarnRows(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpPdsWarn->nItems());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PdsWarn[].set -- set operand                                               */
/*----------------------------------------------------------------------------*/
void Step::vPdsWarnSet(void)
{
  ENTRY();

  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

  ASSERT(! pOperand || m_pLogicUnit->m_xpPdsWarn.bIsNull());

  m_pLogicUnit->m_xpPdsWarn = _pPds(pOperand);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
