/*============================================================================*/
/* Steps_PdsErr.cpp - PdsErr step group                                       */
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
void Step::vAddSteps_PdsErr(void)
{
	vAddStep("PdsErr[].addExecError"_Stz,    &Step::vPdsErrAddExecError);
	vAddStep("PdsErr[].bufferType@"_Stz,     &Step::vPdsErrBufferTypeFetch);
	vAddStep("PdsErr[].delete"_Stz,          &Step::vPdsErrDelete);
	vAddStep("PdsErr[]"_Stz,                 &Step::vPdsErrGet);
	vAddStep("PdsErr[]\\clear"_Stz,          &Step::vPdsErrGetAndClear);
	vAddStep("PdsErr[].bufferId"_Stz,        &Step::vPdsErrGetBufferIdAsAddr);
	vAddStep("PdsErr[].isNotNull"_Stz,       &Step::vPdsErrIsNotNull);
	vAddStep("PdsErr[].isNull"_Stz,          &Step::vPdsErrIsNull);
	vAddStep("PdsErr[].new"_Stz,             &Step::vPdsErrNew);
	vAddStep("PdsErr[].rows"_Stz,            &Step::vPdsErrRows);
	vAddStep("PdsErr[].set"_Stz,             &Step::vPdsErrSet);
}

/*----------------------------------------------------------------------------*/
/* PdsErr[].bufferType@ -- fetch the Pds bufferType               */
/*----------------------------------------------------------------------------*/
void Step::vPdsErrBufferTypeFetch(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpPdsErr->eBufferType());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PdsErr[].addExecError -- add a row to Pds with the exec error              */
/*----------------------------------------------------------------------------*/
void Step::vPdsErrAddExecError(void)
{
  ENTRY();

  s4K sErrorText;

  Pds* pPds = _pPds(*m_pLogicUnit->m_xpPdsErr);

  // get the error information
  eEC eErrorCode = (eEC) m_pDataStack->iPopInt(__func__);
  m_pDataStack->vPopStr(sErrorText, __func__);

  Grid* pGrid = AM::pGrid();

  //pStreamDef->vListContents(1);

  Row* pRow = pPds->pAllotRow(C_VARGS_BEGIN,
                                    pGrid->pThisNode()->szNodeNm(),
                                    (i8)eErrorCode,
                                    sErrorText.azChar(),
                                    C_VARGS_END);

  //pRow->vDump(pStreamDef);

  m_pDataStack->vPushInt(pRow ? true : false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PdsErr[].get -- fetch operand                                              */
/*----------------------------------------------------------------------------*/
void Step::vPdsErrGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpPdsErr);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PdsErr[].getAndClear -- fetch operand and set to nullptr                   */
/*----------------------------------------------------------------------------*/
void Step::vPdsErrGetAndClear(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpPdsErr);

  m_pLogicUnit->m_xpPdsErr = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PdsErr[].bufferId -- fetch buffer id as address                            */
/*----------------------------------------------------------------------------*/
void Step::vPdsErrGetBufferIdAsAddr(void)
{
  ENTRY();

  Pds* pPds = *m_pLogicUnit->m_xpPdsErr;

  m_pDataStack->vPushPtr((Object*)pPds->oGetBufferId().m_pvAsAddress);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PdsErr[].delete -- delete the buffer                                       */
/*----------------------------------------------------------------------------*/
void Step::vPdsErrDelete(void)
{
  ENTRY();

  AM::vDeleteOperand(*m_pLogicUnit->m_xpPdsErr,nullptr);

  m_pLogicUnit->m_xpPdsErr = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PdsErr[].isNotNull -- is Pds not null ?                                    */
/*----------------------------------------------------------------------------*/
void Step::vPdsErrIsNotNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpPdsErr.bIsNull() ? 0 : 1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PdsErr[].isNull -- is Pds null ?                                           */
/*----------------------------------------------------------------------------*/
void Step::vPdsErrIsNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpPdsErr.bIsNull() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PdsErr[].new -- new an PdsErr operand                                      */
/*----------------------------------------------------------------------------*/
void Step::vPdsErrNew(void)
{
  ENTRY();

  ASSERT(m_pLogicUnit->m_xpPdsErr.bIsNull());

  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
  eBT eBufferType = (eBT)m_pDataStack->iPopInt(__func__);

  StreamDef* pStreamDef = pTable->pStreamDef();

  m_pLogicUnit->m_xpPdsErr = new Pds(pStreamDef, false, eBufferType);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PdsErr[].rows -- get number of rows                                        */
/*----------------------------------------------------------------------------*/
void Step::vPdsErrRows(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpPdsErr->nItems());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* PdsErr[].set -- set operand                                                */
/*----------------------------------------------------------------------------*/
void Step::vPdsErrSet(void)
{
  ENTRY();

  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

  ASSERT(! pOperand || m_pLogicUnit->m_xpPdsErr.bIsNull());

  m_pLogicUnit->m_xpPdsErr = _pPds(pOperand);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
