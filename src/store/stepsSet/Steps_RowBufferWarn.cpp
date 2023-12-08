/*============================================================================*/
/* Steps_RowBufferWarn.cpp - RowBufferWarn step group                         */
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
#include "RowBuffer.hpp"
#include "Index.hpp"
#include "Table.hpp"
#include "TablePtn.hpp"
#include "Node.hpp"
#include "Grid.hpp"
#include "eBT.h"
#include "MetricsHelper.hpp"
#include "LockMetrics.hpp"
#include "Op.hpp"
#include "Job.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RowBufferWarn(void)
{
	vAddStep("RowBufferWarn[].bufferId"_Stz,     &Step::vRowBufferWarnGetBufferIdAsAddr);
	vAddStep("RowBufferWarn[]"_Stz,              &Step::vRowBufferWarnGet);
	vAddStep("RowBufferWarn[].delete"_Stz,       &Step::vRowBufferWarnDelete);
	vAddStep("RowBufferWarn[]\\clear"_Stz,       &Step::vRowBufferWarnGetAndClear);
	vAddStep("RowBufferWarn[].isNull"_Stz,       &Step::vRowBufferWarnIsNull);
	vAddStep("RowBufferWarn[].new"_Stz,          &Step::vRowBufferWarnNew);
	vAddStep("RowBufferWarn[].set"_Stz,          &Step::vRowBufferWarnSet);
}

/*----------------------------------------------------------------------------*/
/* RowBufferWarn[].get -- fetch operand                                       */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferWarnGet(void)
{
	ENTRY();

	m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpRowBufferWarn);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBufferWarn[].delete -- delete the buffer                                 */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferWarnDelete(void)
{
	ENTRY();

	AM::vDeleteOperand(*m_pLogicUnit->m_xpRowBufferWarn,nullptr);

	m_pLogicUnit->m_xpRowBufferWarn = nullptr;

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBufferWarn[].bufferId -- fetch buffer id as address                     */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferWarnGetBufferIdAsAddr(void)
{
	ENTRY();

	RowBuffer* pRowBuffer = *m_pLogicUnit->m_xpRowBufferWarn;

	m_pDataStack->vPushPtr((Object*)pRowBuffer->oGetBufferId().m_pvAsAddress);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBufferWarn[].getAndClear -- fetch operand and set to nullptr                */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferWarnGetAndClear(void)
{
	ENTRY();

	m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpRowBufferWarn);

	m_pLogicUnit->m_xpRowBufferWarn = nullptr;

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBufferWarn[].isNull -- is RowBuffer null ?                              */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferWarnIsNull(void)
{
	ENTRY();

	m_pDataStack->vPushInt(m_pLogicUnit->m_xpRowBufferWarn.bIsNull() ? 1 : 0);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBufferWarn[].new -- new an rowBufferWarn operand                        */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferWarnNew(void)
{
	ENTRY();

	ASSERT(m_pLogicUnit->m_xpRowBuffer.bIsNull());

	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
	eBT eBufferType = (eBT)m_pDataStack->iPopInt(__func__);

	StreamDef* pStreamDef = pTable->pStreamDef();

	m_pLogicUnit->m_xpRowBufferWarn = new RowBuffer(pStreamDef);

	m_pLogicUnit->m_xpRowBufferWarn->vBufferType(eBufferType);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBufferWarn[].set -- set operand                                         */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferWarnSet(void)
{
	ENTRY();

	Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

	ASSERT(! pOperand || m_pLogicUnit->m_xpRowBufferWarn.bIsNull());

	m_pLogicUnit->m_xpRowBufferWarn = _pRowBuffer(pOperand);

	EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
