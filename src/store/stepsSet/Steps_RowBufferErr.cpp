/*============================================================================*/
/* Steps_RowBufferErr.cpp - RowBufferErr step group                           */
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
void Step::vAddSteps_RowBufferErr(void)
{
	vAddStep("RowBufferErr[].addCommandError"_Stz, &Step::vRowBufferErrAddCommandError);
	vAddStep("RowBufferErr[].addExecError"_Stz,    &Step::vRowBufferErrAddExecError);
	vAddStep("RowBufferErr[].bufferType@"_Stz,     &Step::vRowBufferErrBufferTypeFetch);
	vAddStep("RowBufferErr[].delete"_Stz,          &Step::vRowBufferErrDelete);
	vAddStep("RowBufferErr[]"_Stz,                 &Step::vRowBufferErrGet);
	vAddStep("RowBufferErr[]\\clear"_Stz,          &Step::vRowBufferErrGetAndClear);
	vAddStep("RowBufferErr[].bufferId"_Stz,        &Step::vRowBufferErrGetBufferIdAsAddr);
	vAddStep("RowBufferErr[].isNotNull"_Stz,       &Step::vRowBufferErrIsNotNull);
	vAddStep("RowBufferErr[].isNull"_Stz,          &Step::vRowBufferErrIsNull);
	vAddStep("RowBufferErr[].new"_Stz,             &Step::vRowBufferErrNew);
	vAddStep("RowBufferErr[].rows"_Stz,            &Step::vRowBufferErrRows);
	vAddStep("RowBufferErr[].set"_Stz,             &Step::vRowBufferErrSet);
}

/*----------------------------------------------------------------------------*/
/* RowBufferErr[].addCommandError -- add a row to RowBuffer with the command error */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferErrAddCommandError(void)
{
	ENTRY();

	s4K sErrorText;

	// get the table
	Table* pStreamTable = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the streamDef from the table
	StreamDef* pStreamDef = pStreamTable->pStreamDef();

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBufferErr);
	pRowBuffer->vBufferType(BT_COMPILEERRORS);

	// get the error information
	eEC eErrorCode = (eEC) m_pDataStack->iPopInt(__func__);
	i8 iLineNo = m_pDataStack->iPopInt(__func__);
	i8 iColumnNo = m_pDataStack->iPopInt(__func__);
	m_pDataStack->vPopStr(sErrorText, __func__);

	Grid* pGrid = AM::pGrid();

	Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
																		C_VARGS_BEGIN,
																		pGrid->pThisNode()->szNodeNm(),
																		eErrorCode,
																		iLineNo,
																		iColumnNo,
																		sErrorText.azChar(),
																		C_VARGS_END);

	//pRow->vDump(pStreamDef);

	m_pDataStack->vPushInt(pRow ? true : false);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBufferErr[].bufferType@ -- fetch the rowBuffer bufferType               */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferErrBufferTypeFetch(void)
{
	ENTRY();

	m_pDataStack->vPushInt(m_pLogicUnit->m_xpRowBufferErr->eBufferType());

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBufferErr[].addExecError -- add a row to RowBuffer with the exec error  */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferErrAddExecError(void)
{
	ENTRY();

	s4K sErrorText;

	// get the table
	Table* pStreamTable = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the streamDef from the table
	StreamDef* pStreamDef = pStreamTable->pStreamDef();

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBufferErr);
	pRowBuffer->vBufferType(BT_EXECERRORS);

	// get the error information
	eEC eErrorCode = (eEC) m_pDataStack->iPopInt(__func__);
	m_pDataStack->vPopStr(sErrorText, __func__);

	Grid* pGrid = AM::pGrid();

	//pStreamDef->vListContents(1);

	Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
																		C_VARGS_BEGIN,
																		pGrid->pThisNode()->szNodeNm(),
																		(i8)eErrorCode,
																		sErrorText.azChar(),
																		C_VARGS_END);

	//pRow->vDump(pStreamDef);

	m_pDataStack->vPushInt(pRow ? true : false);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBufferErr[].get -- fetch operand                                       */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferErrGet(void)
{
	ENTRY();

	m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpRowBufferErr);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBufferErr[].getAndClear -- fetch operand and set to nullptr             */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferErrGetAndClear(void)
{
	ENTRY();

	m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpRowBufferErr);

	m_pLogicUnit->m_xpRowBufferErr = nullptr;

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBufferErr[].bufferId -- fetch buffer id as address                     */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferErrGetBufferIdAsAddr(void)
{
	ENTRY();

	RowBuffer* pRowBuffer = *m_pLogicUnit->m_xpRowBufferErr;

	m_pDataStack->vPushPtr((Object*)pRowBuffer->oGetBufferId().m_pvAsAddress);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBufferErr[].delete -- delete the buffer                                 */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferErrDelete(void)
{
	ENTRY();

	AM::vDeleteOperand(*m_pLogicUnit->m_xpRowBufferErr,nullptr);

	m_pLogicUnit->m_xpRowBufferErr = nullptr;

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBufferErr[].isNotNull -- is RowBuffer not null ?                        */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferErrIsNotNull(void)
{
	ENTRY();

	m_pDataStack->vPushInt(m_pLogicUnit->m_xpRowBufferErr.bIsNull() ? 0 : 1);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBufferErr[].isNull -- is RowBuffer null ?                               */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferErrIsNull(void)
{
	ENTRY();

	m_pDataStack->vPushInt(m_pLogicUnit->m_xpRowBufferErr.bIsNull() ? 1 : 0);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBufferErr[].new -- new an rowBufferWarn operand                        */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferErrNew(void)
{
	ENTRY();

	ASSERT(m_pLogicUnit->m_xpRowBufferErr.bIsNull());

	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  eBT eBufferType = (eBT)m_pDataStack->iPopInt(__func__);

	StreamDef* pStreamDef = pTable->pStreamDef();

	m_pLogicUnit->m_xpRowBufferErr = new RowBuffer(pStreamDef);

	m_pLogicUnit->m_xpRowBufferErr->vBufferType(eBufferType);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBufferErr[].rows -- get number of rows                                  */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferErrRows(void)
{
	ENTRY();

	m_pDataStack->vPushInt(m_pLogicUnit->m_xpRowBufferErr->nRows());

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBufferErr[].set -- set operand                                         */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferErrSet(void)
{
	ENTRY();

	Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

	ASSERT(! pOperand || m_pLogicUnit->m_xpRowBufferErr.bIsNull());

	m_pLogicUnit->m_xpRowBufferErr = _pRowBuffer(pOperand);

	EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
