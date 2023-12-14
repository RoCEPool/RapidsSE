/*============================================================================*/
/* Steps_RowBuffer.cpp - RowBuffer step group                                 */
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

extern SizeHeap  gxaSizeHeap[];

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RowBuffer(void)
{

  vAddStep("rowBuffer.operandDump"_Stz,        &Step::vRowBufferOperandDump);

	vAddStep("RowBuffer[]"_Stz,                  &Step::vRowBufferGet);
	vAddStep("RowBuffer[]\\clear"_Stz,           &Step::vRowBufferGetAndClear);

	vAddStep("RowBuffer[].addAnalyzeIndex"_Stz,  &Step::vRowBufferAddAnalyzeIndex);
	vAddStep("RowBuffer[].addAnalyzeTable"_Stz,  &Step::vRowBufferAddAnalyzeTable);
	vAddStep("RowBuffer[].addBpaObjects"_Stz,    &Step::vRowBufferAddBpaObjects);
	vAddStep("RowBuffer[].addBpaRegions"_Stz,    &Step::vRowBufferAddBpaRegions);
	vAddStep("RowBuffer[].addColumnDef"_Stz,     &Step::vRowBufferAddColumnDef);
	vAddStep("RowBuffer[].addGridSanity"_Stz,    &Step::vRowBufferAddGridSanity);
	vAddStep("RowBuffer[].addListContents"_Stz,  &Step::vRowBufferAddListContents);
	vAddStep("RowBuffer[].addListFolder"_Stz,    &Step::vRowBufferAddListFolder);
	vAddStep("RowBuffer[].addListIndex"_Stz,     &Step::vRowBufferAddListIndex);
	vAddStep("RowBuffer[].addListPartitions"_Stz,&Step::vRowBufferAddListPartitions);
	vAddStep("RowBuffer[].addLogMsg"_Stz,        &Step::vRowBufferAddLogMsg);
	vAddStep("RowBuffer[].addLockStats"_Stz,     &Step::vRowBufferAddLockStats);
	vAddStep("RowBuffer[].addMemoryLeaks"_Stz,   &Step::vRowBufferAddMemoryLeaks);
	vAddStep("RowBuffer[].addNodeHeartbeat"_Stz, &Step::vRowBufferAddNodeHeartbeat);
	vAddStep("RowBuffer[].addNodeInfo"_Stz,      &Step::vRowBufferAddNodeInfo);
	vAddStep("RowBuffer[].addNodeStatus"_Stz,    &Step::vRowBufferAddNodeStatus);
	vAddStep("RowBuffer[].addPingInfo"_Stz,      &Step::vRowBufferAddPingInfo);
	vAddStep("RowBuffer[].addProgressUnits"_Stz, &Step::vRowBufferAddProgressUnits);
	vAddStep("RowBuffer[].addPwfInfo"_Stz,       &Step::vRowBufferAddPwfInfo);
	vAddStep("RowBuffer[].addRowCount"_Stz,      &Step::vRowBufferAddRowCount);
	vAddStep("RowBuffer[].addRowVargs"_Stz,      &Step::vRowBufferAddRowVargs);
	vAddStep("RowBuffer[].addTotalCount"_Stz,    &Step::vRowBufferAddTotalCount);
	vAddStep("RowBuffer[].addTotalUnits"_Stz,    &Step::vRowBufferAddTotalUnits);
	vAddStep("RowBuffer[].addShowCluster"_Stz,   &Step::vRowBufferAddShowCluster);
	vAddStep("RowBuffer[].addSizeIndex"_Stz,     &Step::vRowBufferAddSizeIndex);
	vAddStep("RowBuffer[].addSizeTable"_Stz,     &Step::vRowBufferAddSizeTable);
	vAddStep("RowBuffer[].addSpaceInfo"_Stz,     &Step::vRowBufferAddSpaceInfo);
	vAddStep("RowBuffer[].addTableSizes"_Stz,    &Step::vRowBufferAddTableSizes);
	vAddStep("RowBuffer[].addVerifyIndex"_Stz,   &Step::vRowBufferAddVerifyIndex);
	vAddStep("RowBuffer[].addVerifyTable"_Stz,   &Step::vRowBufferAddVerifyTable);
	vAddStep("RowBuffer[].addWarning"_Stz,       &Step::vRowBufferAddWarning);

  vAddStep("RowBuffer[].bufferId@"_Stz,        &Step::vRowBufferBufferIdAsAddrFetch);
	vAddStep("RowBuffer[].bufferName@"_Stz,      &Step::vRowBufferBufferNameFetch);
	vAddStep("RowBuffer[].bufferType@"_Stz,      &Step::vRowBufferBufferTypeFetch);
	vAddStep("RowBuffer[].bufferType!"_Stz,      &Step::vRowBufferBufferTypeStore);
	vAddStep("RowBuffer[].chunkFile"_Stz,        &Step::vRowBufferChunkFile);
  vAddStep("RowBuffer[].copy"_Stz,             &Step::vRowBufferCopy);
	vAddStep("RowBuffer[].colVal@"_Stz,          &Step::vRowBufferColValFetch);
  vAddStep("RowBuffer[].decile"_Stz,           &Step::vRowBufferDecile);
  vAddStep("RowBuffer[].delete"_Stz,           &Step::vRowBufferDelete);
  vAddStep("RowBuffer[].get"_Stz,              &Step::vRowBufferGet);
	vAddStep("RowBuffer[].firstRow"_Stz,         &Step::vRowBufferFirstRow);
	vAddStep("RowBuffer[].insdelIntoTable"_Stz,  &Step::vRowBufferInsdelIntoTable);
	vAddStep("RowBuffer[].isProgress"_Stz,       &Step::vRowBufferIsProgress);
	vAddStep("RowBuffer[].isTotalUnits"_Stz,     &Step::vRowBufferIsTotalUnits);
	vAddStep("RowBuffer[].isWarning"_Stz,        &Step::vRowBufferIsWarning);
  vAddStep("RowBuffer[].isNotNull"_Stz,        &Step::vRowBufferIsNotNull);
  vAddStep("RowBuffer[].isNull"_Stz,           &Step::vRowBufferIsNull);
  vAddStep("RowBuffer[].iterate"_Stz,          &Step::vRowBufferIterate);
	vAddStep("RowBuffer[].name"_Stz,             &Step::vRowBufferName);
	vAddStep("RowBuffer[].new"_Stz,              &Step::vRowBufferNew);
	vAddStep("RowBuffer[].nextRow"_Stz,          &Step::vRowBufferNextRow);
  vAddStep("RowBuffer[].print"_Stz,            &Step::vRowBufferPrint);
  vAddStep("RowBuffer[].ptnId"_Stz,            &Step::vRowBufferPtnId);
	vAddStep("RowBuffer[].rdCsvChunk"_Stz,       &Step::vRowBufferRdCsvChunk);
  vAddStep("RowBuffer[].rows"_Stz,             &Step::vRowBufferRows);
  vAddStep("RowBuffer[].set"_Stz,              &Step::vRowBufferSet);
  vAddStep("RowBuffer[].setDeciderCopies"_Stz, &Step::vRowBufferSetDeciderCopies);
  vAddStep("RowBuffer[].setLglPtn"_Stz,        &Step::vRowBufferSetLglPtn);
  vAddStep("RowBuffer[].tableDefName@"_Stz,    &Step::vRowBufferTableDefNameFetch);
	vAddStep("RowBuffer[].toCsv"_Stz,            &Step::vRowBufferToCsv);
	vAddStep("RowBuffer[].writeCsv"_Stz,         &Step::vRowBufferWriteCsv);
}

/*----------------------------------------------------------------------------*/
/* rowBuffer.operandDump -- operand dump of rowBuffer                         */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferOperandDump(void)
{
  ENTRY();
  
  RowBuffer* pRowBuffer = _pRowBuffer(m_pDataStack->pPopPtr(__func__));

  i8 iDetail = m_pDataStack->iPopInt(__func__);
  
  if (iDetail)
    pRowBuffer->vDumpAllRows();
  
  pRowBuffer->vSlabReport();
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* rowBuffer[].addAnalyzeIndex -- add a row to the rowBuffer index analysis   */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddAnalyzeIndex(void)
{
	ENTRY();

	// get the streamSet table
	Table* pTableDef = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the index to be analyzed
	Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));

	// get the tablePtn
	i8 i_TablePtn = m_pDataStack->iPopInt(__func__);

	// get the streamDef from the tableDef
	StreamDef* pStreamDef = pTableDef->pStreamDef();

	// get the rowBuffer
	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	i8 nBuffers = 0;
	i8 nEntries = 0;

	pIndex->vAnalyze(i_TablePtn, nBuffers, nEntries);

	Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
																		C_VARGS_BEGIN,
																		AM::pGrid()->pThisNode()->szNodeNm(),
																		pTableDef->tTablePtnToLglPtn(i_TablePtn).iAsI8(),
																		nBuffers,
																		nEntries,
																		C_VARGS_END);

	//ASSERT(pRow);
	//pRow->vDump(pStreamDef);

	m_pDataStack->vPushInt(pRow ? true : false);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].addAnalyzeTable -- add a row to the rowBuffer table analysis   */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddAnalyzeTable(void)
{
	ENTRY();

	// get the tableDef table
	Table* pTableDef = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the table to be analyzed
	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the tablePtn
	i8 i_TablePtn = m_pDataStack->iPopInt(__func__);

	// get the streamDef from the table
	StreamDef* pStreamDef = pTableDef->pStreamDef();

	// get the stream set
	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	i8 nBuffers = 0;
	i8 nRows = 0;
	i8 njRowSize = 0;

	pTable->vAnalyze(i_TablePtn, nBuffers, nRows, njRowSize);

	Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
																		C_VARGS_BEGIN,
																		AM::pGrid()->pThisNode()->szNodeNm(),
																		pTable->tTablePtnToLglPtn(i_TablePtn).iAsI8(),
																		nBuffers,
																		nRows,
																		C_VARGS_END);

	//ASSERT(pRow);
	//pRow->vDump(pStreamDef);

	m_pDataStack->vPushInt(pRow ? true : false);
	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].addBpaObjects -- add rows about bpa object analysis            */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddBpaObjects(void)
{
	ENTRY();

	// Get the warning RowBuffer and definition
	RowBuffer* pRowBufferWarn = * m_pLogicUnit->m_xpRowBufferWarn;
	ASSERT(pRowBufferWarn);
	ASSERT(pRowBufferWarn->eBufferType() == BT_WARNINGS);

	Table* pWarnTable = _pTable(* m_pLogicUnit->m_xpTableDefWarn);
	ASSERT(pWarnTable);
	StreamDef* pWarnDef = pWarnTable->pStreamDef();
	ASSERT(pWarnDef);


	// get the table
	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the streamDef from the table
	StreamDef* pStreamDef = pTable->pStreamDef();

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	PoolHelper::vObjectSummary(pRowBuffer, pStreamDef, pRowBufferWarn, pWarnDef);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].addBpaRegions -- add rows about bpa region analysis            */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddBpaRegions(void)
{
	ENTRY();

	// Get the warning RowBuffer and definition
	RowBuffer* pRowBufferWarn = * m_pLogicUnit->m_xpRowBufferWarn;
	ASSERT(pRowBufferWarn);
	ASSERT(pRowBufferWarn->eBufferType() == BT_WARNINGS);

	Table* pWarnTable = _pTable(* m_pLogicUnit->m_xpTableDefWarn);
	ASSERT(pWarnTable);
	StreamDef* pWarnDef = pWarnTable->pStreamDef();
	ASSERT(pWarnDef);


	// get the table
	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the streamDef from the table
	StreamDef* pStreamDef = pTable->pStreamDef();

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	PoolHelper::vRegionSummary(pRowBuffer, pStreamDef, pRowBufferWarn, pWarnDef);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].addColumnDef -- add a row to the rowBuffer with the column def */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddColumnDef(void)
{
	ENTRY();

	// get the table
	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the streamDef from the table
	StreamDef* pStreamDef = pTable->pStreamDef();

	Column* pColumn = _pColumn(m_pDataStack->pPopPtr(__func__));

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
																		C_VARGS_BEGIN,
																		pColumn->szColNm(),
																		aCT[pColumn->eColumnType()],
																		pColumn->iColumnLength(),
																		pColumn->iColumnScale(),
																		(i8)pColumn->bIsNullable(),
																		(i8)pColumn->bIsPtnKey(),
																		C_VARGS_END);
	//ASSERT(pRow);
	//pRow->vDump(pStreamDef);

	m_pDataStack->vPushInt(pRow ? true : false);
	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].addGridSanity -- add a row to the RowBuffer with the gridSanity  */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddGridSanity(void)
{
	ENTRY();

	// get the table
	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the streamDef from the table
	StreamDef* pStreamDef = pTable->pStreamDef();

	Row* pNodeInfo = _pRow(m_pDataStack->pPopPtr(__func__));
	//StreamDef* pStreamDefNI = _pStreamDef(m_pDataStack->pPopPtr(__func__));

	//pNodeInfo->vDump(pStreamDefNI);

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	//Grid* pGrid = AM::pGrid();

	// get regionSize on this node
	bool bRegionSize = false;
	i8 iRegionSize = AM::iRegionSize();
	if (pNodeInfo->iColVal_int(1) == iRegionSize)
		bRegionSize = true;

	// get partitions per table for this node
	bool bPartitions = false;
	i8 nPtnsPerNode = AM::PTNMAP::nPtnsPerNode();
	if (pNodeInfo->iColVal_int(2) == nPtnsPerNode)
		bPartitions = true;

	// check if the regionSize and partitions match
	bool bHomogenous = false;
	if ( bRegionSize && bPartitions )
		bHomogenous = true;

	Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
																		C_VARGS_BEGIN,
																		pNodeInfo->szColVal_ofs(0),
																		bHomogenous,
																		C_VARGS_END);
	//ASSERT(pRow);
	//pRow->vDump(pStreamDef);
	m_pDataStack->vPushInt(pRow ? true : false);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].addListContents -- add a row to RowBuffer with the list contents */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddListContents(void)
{
	ENTRY();

	// get the table
	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the streamDef from the table
	StreamDef* pStreamDef = pTable->pStreamDef();

	s256 sContents;
	m_pDataStack->vPopStr(sContents, __func__);

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
																		C_VARGS_BEGIN,
																		sContents.azChar(),
																		C_VARGS_END);
	//ASSERT(pRow);
	//pRow->vDump(pStreamDef);

	m_pDataStack->vPushInt(pRow ? true : false);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].addListFolder -- add a row to RowBuffer with the list folder     */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddListFolder(void)
{
	ENTRY();

	// get the table
	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the streamDef from the table
	StreamDef* pStreamDef = pTable->pStreamDef();

	// get the name
	s256 sName;
	m_pDataStack->vPopStr(sName, __func__);

	// get the kind
	s256 sKind;
	m_pDataStack->vPopStr(sKind, __func__);

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
																		C_VARGS_BEGIN,
																		sName.azChar(),
																		sKind.azChar(),
																		C_VARGS_END);
	//ASSERT(pRow);
	//pRow->vDump(pStreamDef);

	m_pDataStack->vPushInt(pRow ? true : false);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].addListIndex -- add a row to RowBuffer with the list index       */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddListIndex(void)
{
	ENTRY();

	// get the table
	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the streamDef from the table
	StreamDef* pStreamDef = pTable->pStreamDef();

	// get the index
	Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
																		C_VARGS_BEGIN,
																		pIndex->szIndexNm(),
																		(i8)pIndex->bIsUnique(),
																		(i8)pIndex->bIsPrimaryKey(),
																		C_VARGS_END);
	//ASSERT(pRow);
	//pRow->vDump(pStreamDef);

	m_pDataStack->vPushInt(pRow ? true : false);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].addListPartitions -- add a row to RowBuffer with the partitions  */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddListPartitions(void)
{
	ENTRY();

	// get the table
	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	LglPtn tLglPtn;
	tLglPtn = m_pDataStack->iPopInt(__func__);

	// get the streamDef from the table
	StreamDef* pStreamDef = pTable->pStreamDef();

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	Grid* pGrid = AM::pGrid();

	i8 iNodeId = AM::pLglPtnMap()->iNodeId(tLglPtn);
	Node* pNode = pGrid->pFindNodeById(iNodeId);
	Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
																		C_VARGS_BEGIN,
																		tLglPtn.iAsI8(),
																		pNode->szNodeNm(),
																		C_VARGS_END);


	//ASSERT(pRow);
	//pRow->vDump(pStreamDef);

	m_pDataStack->vPushInt(pRow ? true : false);

	EXIT();
}


/*----------------------------------------------------------------------------*/
/* RowBuffer[].addLogMsg -- add rows to rowBuffer with log message info       */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddLogMsg(void)
{
	ENTRY();

	// get the table
	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the timestamp to use
	i8 iTsp = m_pDataStack->iPopInt(__func__);

	// get the log message to use
	s2K sMsg;
	m_pDataStack->vPopStr(sMsg, __func__ );


	// get the streamDef from the table
	StreamDef* pStreamDef = pTable->pStreamDef();

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);
	ASSERT(pRowBuffer);

	Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
																		C_VARGS_BEGIN,
																		iTsp,
																		sMsg.szStz(),
																		C_VARGS_END);

	m_pDataStack->vPushInt(pRow ? true : false);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].addMemoryLeaks -- add rows to rowBuffer with memory leak info  */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddMemoryLeaks(void)
{
	ENTRY();

	// get the table
	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the streamDef from the table
	StreamDef* pStreamDef = pTable->pStreamDef();

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	i8 nLeaks = 0;

	ObjectSnapshot* pSnapshotThen = AM::MEMORY::pSnapshotAtTrackingMemory();
	ObjectSnapshot* pSnapshotNow  = AM::MEMORY::pSnapshotAtMemoryLeaks();

	for (i8 i_Class = 0 ; i_Class < CN_ENUMS; i_Class++) {

		eCN eClass = (eCN)i_Class;

		if (eClass == CN_FREEPOOLBUFFER || eClass == CN_HEAPBUFFER)
			continue;

		// check if this class to be ignored for memory leak detection
		if (gxaSizeHeap[eClass].m_bIgnoreMemoryLeak)
			continue;

		i8 nDelta = pSnapshotNow->m_nObjects[eClass] -
								pSnapshotThen->m_nObjects[eClass];

		if (nDelta <= 0)
			continue;

		++nLeaks;
		/*Row* pRow =*/ pRowBuffer->pAllotRow(pStreamDef,
																					C_VARGS_BEGIN,
																					aCN[eClass],
																					nDelta,
																					C_VARGS_END);
	}

	m_pDataStack->vPushBol(nLeaks > 0);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].AddNodeHeartbeat -- add a row to the rowBuffer with node hb    */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddNodeHeartbeat(void)
{
	ENTRY();

	// get the table
	Table *pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the streamDef from the table
	StreamDef *pStreamDef = pTable->pStreamDef();

	RowBuffer *pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	Node* pThisNode = MY::pNode();

	Row *pRow = nullptr;
	for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext()) {

		if (pThisNode != *oNode_I)
			continue;

		pRow = pRowBuffer->pAllotRow(pStreamDef,
																 C_VARGS_BEGIN,
																 (*oNode_I)->szNodeNm(),
																 (*oNode_I)->xGetHeartbeat().m_acRegionName,
																 (*oNode_I)->eState() == NS_ACTIVE ? "running"_Stz : "not running"_Stz,
																 (*oNode_I)->pSendSocket() ? true : false,
																 (*oNode_I)->pRecvSocket() ? true : false,
																 C_VARGS_END);

		break;
	}

	// done
	m_pDataStack->vPushInt(pRow ? true : false);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].addNodeStatus -- add a row to the rowBuffer with node status   */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddNodeStatus(void)
{
	ENTRY();

	// get the table
	Table *pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the streamDef from the table
	StreamDef *pStreamDef = pTable->pStreamDef();

	RowBuffer *pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	/* commented out, the old status command which is required to go to all nodes
	 * for each node, it gets back a row and its status.
	 * With the new model, we need a grid of the requested node,
	 * each individual node in the grid will have its own status.
	 */
	/*
	Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
												C_VARGS_BEGIN,
												pGrid->pThisNode()->szNodeNm(),
												sRegionNm.azChar(),
												Region::bTest(sRegionNm) ? "running"_Stz : "not running"_Stz,
												C_VARGS_END);
	//ASSERT(pRow);
	//pRow->vDump(pStreamDef);

	m_pDataStack->vPushInt(pRow ? true : false);
	*/
	Row *pRow = nullptr;
	for (Node_I oNode_I; oNode_I.bMore(); oNode_I.vNext()) {

		pRow = pRowBuffer->pAllotRow(pStreamDef,
																 C_VARGS_BEGIN,
																 (*oNode_I)->szNodeNm(),
																 (*oNode_I)->xGetHeartbeat().m_acRegionName,
																 (*oNode_I)->eState() == NS_ACTIVE ? "running"_Stz : "not running"_Stz,
																 (*oNode_I)->pSendSocket() ? true : false,
																 (*oNode_I)->pRecvSocket() ? true : false,
																 C_VARGS_END);

		if (!pRow) {
			// call again
			m_pDataStack->vPushInt(oNode_I.i_Node());
			break;
		}
	}

	// done
	m_pDataStack->vPushInt(pRow ? true : false);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].addPingInfo -- add a row to RowBuffer with the ping response     */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddPingInfo(void)
{
	ENTRY();

	// get the table
	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the streamDef from the table
	StreamDef* pStreamDef = pTable->pStreamDef();
	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
																		C_VARGS_BEGIN,
																		"pong"_Stz,
																		C_VARGS_END);
	//ASSERT(pRow);
	//pRow->vDump(pStreamDef);

	m_pDataStack->vPushInt(pRow ? true : false);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].addStr -- add a row to rowBuffer with a __pwf response         */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddPwfInfo(void)
{
	ENTRY();

	// get the table
	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	s256 sString;
	m_pDataStack->vPopStr(sString, __func__);

	// get the streamDef from the table
	StreamDef* pStreamDef = pTable->pStreamDef();

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
																		C_VARGS_BEGIN,
																		sString.azChar(),
																		C_VARGS_END);
	//ASSERT(pRow);
	//pRow->vDump(pStreamDef);

	m_pDataStack->vPushInt(pRow ? true : false);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].addTotalCount -- add a row to rowBuffer with the totalCount    */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddTotalCount(void)
{
	ENTRY();

	// get the table
	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the streamDef from the table
	StreamDef* pStreamDef = pTable->pStreamDef();

	i8 i_TotalCount = m_pDataStack->iPopInt(__func__);

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
																		C_VARGS_BEGIN,
																		i_TotalCount,
																		C_VARGS_END);
	//ASSERT(pRow);
	//pRow->vDump(pStreamDef);

	m_pDataStack->vPushInt(pRow ? true : false);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].addShowCluster -- add a row to rowBuffer with the cluster info */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddShowCluster(void)
{
	ENTRY();

	// get the table
	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the streamDef from the table
	StreamDef* pStreamDef = pTable->pStreamDef();

	i8 i_Node = m_pDataStack->iPopInt(__func__);

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	Node* pNode = AM::pGrid()->pFindNodeById(i_Node);
	Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
																		C_VARGS_BEGIN,
																		pNode->szNodeNm(),
																		pNode->szIPAddr(),
																		pNode->iPortNo(),
																		(i8)(pNode->pSendSocket() ? true : false),
																		C_VARGS_END);
	//ASSERT(pRow);
	//pRow->vDump(pStreamDef);

	m_pDataStack->vPushInt(pRow ? true : false);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].addSizeIndex -- add a row to rowBuffer with the index size     */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddSizeIndex(void)
{
	ENTRY();

	// get the table
	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the streamDef from the table
	StreamDef* pStreamDef = pTable->pStreamDef();

	i8 iSize = m_pDataStack->iPopInt(__func__);

	s256 sSize;
	m_pDataStack->vPopStr(sSize, __func__);

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
																		C_VARGS_BEGIN,
																		AM::pGrid()->pThisNode()->szNodeNm(),
																		iSize,
																		sSize.azChar(),
																		C_VARGS_END);
	//ASSERT(pRow);
	//pRow->vDump(pStreamDef);

	m_pDataStack->vPushInt(pRow ? true : false);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].addSizeTable -- add a row to rowBuffer with the table size     */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddSizeTable(void)
{
	ENTRY();

	// get the table
	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the streamDef from the table
	StreamDef* pStreamDef = pTable->pStreamDef();

	i8 iSize = m_pDataStack->iPopInt(__func__);

	s256 sSize;
	m_pDataStack->vPopStr(sSize, __func__);

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
																		C_VARGS_BEGIN,
																		AM::pGrid()->pThisNode()->szNodeNm(),
																		iSize,
																		sSize.azChar(),
																		C_VARGS_END);
	//ASSERT(pRow);
	//pRow->vDump(pStreamDef);

	m_pDataStack->vPushInt(pRow ? true : false);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].addSpaceInfo -- add a row to the rowBuffer with the spaceInfo  */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddSpaceInfo(void)
{
	ENTRY();

	// get the table
	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the streamDef from the table
	StreamDef* pStreamDef = pTable->pStreamDef();

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	Grid* pGrid = AM::pGrid();

	i8 iTotalSpace = AM::POOL::iTotalSpace();
	i8 iInUseSpace = AM::POOL::iUsedSpace();
	i8 iFreeSpace  = AM::POOL::iFreeSpace();

	i8 iFreeSpaceInPool = PoolHelper::iPoolFree();
	i8 iTotalFreeSpace  = iFreeSpace + iFreeSpaceInPool;

	s256 sTotalSpace;
	MagLib::vByteMag(iTotalSpace, 0, sTotalSpace);

	s256 sInUseSpace;
	MagLib::vByteMag(iInUseSpace, 0, sInUseSpace);

	s256 sTotalFreeSpace;
	MagLib::vByteMag(iTotalFreeSpace, 0, sTotalFreeSpace);

	Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
																		C_VARGS_BEGIN,
																		pGrid->pThisNode()->szNodeNm(),
																		iTotalSpace,
																		sTotalSpace.azChar(),
																		iInUseSpace,
																		sInUseSpace.azChar(),
																		iTotalFreeSpace,
																		sTotalFreeSpace.azChar(),
																		C_VARGS_END);
	//ASSERT(pRow);
	//pRow->vDump(pStreamDef);

	m_pDataStack->vPushInt(pRow ? true : false);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].addTableSizes -- add a row to rowBuffer with the table sizes   */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddTableSizes(void)
{
	ENTRY();

	// get the table
	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the streamDef from the table
	StreamDef* pStreamDef = pTable->pStreamDef();

	s256 sTableName;
	m_pDataStack->vPopStr(sTableName, __func__);

	s256 sSize;
	m_pDataStack->vPopStr(sSize, __func__);

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
																		C_VARGS_BEGIN,
																		AM::pGrid()->pThisNode()->szNodeNm(),
																		sTableName.azChar(),
																		sSize.azChar(),
																		C_VARGS_END);
	//ASSERT(pRow);
	//pRow->vDump(pStreamDef);

	m_pDataStack->vPushInt(pRow ? true : false);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].addVerifyIndex -- add a row to rowBuffer with the index verify */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddVerifyIndex(void)
{
	ENTRY();

	// get the table
	Table* pTableDef = _pTable(m_pDataStack->pPopPtr(__func__));
	StreamDef* pStreamDef = pTableDef->pStreamDef();

	// get the index
	Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));

	// get the table from the index
	Table* pTable = pIndex->pTable();

	i8 iStatus = m_pDataStack->iPopInt(__func__);

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
																		C_VARGS_BEGIN,
																		AM::pGrid()->pThisNode()->szNodeNm(),
																		pTable->szTableNm(),
																		pIndex->szIndexNm(),
																		iStatus ? "Valid"_Stz : "Invalid"_Stz,
																		C_VARGS_END);
	//ASSERT(pRow);
	//pRow->vDump(pStreamDef);

	m_pDataStack->vPushInt(pRow ? true : false);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].addVerifyTable -- add a row to rowBuffer with the table verify */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddVerifyTable(void)
{
	ENTRY();

	// get the table
	Table* pTableDef = _pTable(m_pDataStack->pPopPtr(__func__));
	StreamDef* pStreamDef = pTableDef->pStreamDef();

	// get the table
	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	i8 iStatus = m_pDataStack->iPopInt(__func__);

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
																		C_VARGS_BEGIN,
																		AM::pGrid()->pThisNode()->szNodeNm(),
																		pTable->szTableNm(),
																		iStatus ? "valid"_Stz : "invalid"_Stz,
																		C_VARGS_END);
	//ASSERT(pRow);
	//pRow->vDump(pStreamDef);

	m_pDataStack->vPushInt(pRow ? true : false);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].bufferName@ -- fetch the rowBuffer buffer name                 */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferBufferNameFetch(void)
{
	ENTRY();

	s256 sBufferName = aBT[m_pLogicUnit->m_xpRowBuffer->eBufferType()];

	m_pDataStack->vPushStr(sBufferName);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].bufferType@ -- fetch the rowBuffer bufferType                  */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferBufferTypeFetch(void)
{
	ENTRY();

	m_pDataStack->vPushInt(m_pLogicUnit->m_xpRowBuffer->eBufferType());

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].bufferType! -- store buffer type                               */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferBufferTypeStore(void)
{
	ENTRY();

	eBT eBufferType = (eBT)m_pDataStack->iPopInt(__func__);

	m_pLogicUnit->m_xpRowBuffer->vBufferType(eBufferType);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].copy-- copy the row buffer                                     */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferCopy(void)
{
  ENTRY();

  RowBuffer* pRowBuffer = *m_pLogicUnit->m_xpRowBuffer;
  RowBuffer* pNewRowBuffer = new RowBuffer;

  RowPix* pPix = pRowBuffer->pRowPix();
  pNewRowBuffer->vCopyRowPix(pPix);

  i8 nRows = pRowBuffer->nRows();

  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    Row* pRow = pRowBuffer->pRowAddress(i_Row);
    Row* pNewRow = pNewRowBuffer->pAllotRow(pRow);
    RASSERT(pNewRow);
  }

  m_pDataStack->vPushPtr(pNewRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].decile -- get rowBuffer space used decile                      */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferDecile(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpRowBuffer->iDecile());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].delete -- operand delete                                       */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferDelete(void)
{
  ENTRY();

  AM::vDeleteOperand(*m_pLogicUnit->m_xpRowBuffer,nullptr);

  m_pLogicUnit->m_xpRowBuffer = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].get -- fetch operand                                           */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].getAndClear -- fetch operand and set to nullptr                */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferGetAndClear(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpRowBuffer);

  m_pLogicUnit->m_xpRowBuffer = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].bufferId@ -- fetch buffer id as address                         */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferBufferIdAsAddrFetch(void)
{
  ENTRY();

  RowBuffer* pRowBuffer = *m_pLogicUnit->m_xpRowBuffer;

  m_pDataStack->vPushPtr((Object*)pRowBuffer->oGetBufferId().m_pvAsAddress);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].insdelIntoTable -- insert-delete rowBuffer into the table      */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferInsdelIntoTable(void)
{
  ENTRY();

//  ASSERT(*m_pLogicUnit->m_xpRowBuffer);
//
//  Op* pOp = m_pJob->m_pOp;
//  PtnId tLglPtn = pOp->tLglPtn();
//  i8 i_TablePtn = Table::iPtnNoToTablePtn(tLglPtn);
//

//  // will delete this buffer
//  RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);
//  pRowBuffer->vIsPermanent(false);
//  m_pLogicUnit->m_xpTable->vInsdelBuffer(C_UNKNOWN, i_TablePtn, pRowBuffer);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].isNotNull -- is RowBuffer not null ?                           */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferIsNotNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpRowBuffer.bIsNull() ? 0 : 1);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].isNull -- is RowBuffer null ?                                  */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferIsNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpRowBuffer.bIsNull() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].iterate -- iterate the row buffer                              */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferIterate(void)
{
  ENTRY();

  RowBuffer* pRowBuffer = *m_pLogicUnit->m_xpRowBuffer;

  i8 nRows = pRowBuffer->nRows();

  // iterate the buffer
  for (i8 i_Row = 0; i_Row < nRows; i_Row++) {
    IGNORE_RETURN(pRowBuffer->pRowAddress(i_Row));
  }

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].new -- new an rowBuffer operand                                */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferNew(void)
{
	ENTRY();

	ASSERT(m_pLogicUnit->m_xpRowBuffer.bIsNull());

	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	eBT eBufferType = (eBT)m_pDataStack->iPopInt(__func__);

	StreamDef* pStreamDef = pTable->pStreamDef();

	m_pLogicUnit->m_xpRowBuffer = new RowBuffer(pStreamDef, eBufferType);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].print -- print row buffer                                      */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferPrint(void)
{
  ENTRY();

  m_pLogicUnit->m_xpRowBuffer->vPrintRows(nullptr,"see:"_Stz);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].ptnId -- get ptnid                                             */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferPtnId(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpRowBuffer->tLglPtn().iAsI8());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].rows -- get number of rows                                    */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferRows(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpRowBuffer->nRows());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].set -- set operand                                             */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferSet(void)
{
  ENTRY();

  Operand* pOperand = _pOperand(m_pDataStack->pPopPtr(__func__));

  ASSERT(! pOperand || m_pLogicUnit->m_xpRowBuffer.bIsNull());

  m_pLogicUnit->m_xpRowBuffer = _pRowBuffer(pOperand);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].setDeciderCopies -- set decider copies                         */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferSetDeciderCopies(void)
{
  ENTRY();

  i8 iDeciderCopies = m_pDataStack->iPopInt(__func__);

  m_pLogicUnit->m_xpRowBuffer->vDeciderCopies(iDeciderCopies);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].setLglPtn -- set logical partition                             */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferSetLglPtn(void)
{
  ENTRY();

  i8 iLglPtn = m_pDataStack->iPopInt(__func__);

  m_pLogicUnit->m_xpRowBuffer->vLglPtn((LglPtn)iLglPtn);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].tableDefName@ -- fetch the corresoonding tableDef name         */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferTableDefNameFetch(void)
{
	ENTRY();

	s256 sTdefNm;

	eBT eBufferType = m_pLogicUnit->m_xpRowBuffer->eBufferType();

	switch(eBufferType) {
		case BT_AFFECTEDROWS:
			sTdefNm = C_TDEF_AFFECTEDROWS;
			break;
		case BT_COMMAND:
			sTdefNm = C_TDEF_COMMAND;
			break;
		case BT_COMPILEERRORS:
			sTdefNm = C_TDEF_COMPILEERRORS;
			break;
		case BT_COUNT:
			sTdefNm = C_TDEF_COUNT;
			break;
		case BT_DEREFMETRICS:
			sTdefNm = C_TDEF_DEREFMETRICS;
			break;
		case BT_EXECERRORS:
			sTdefNm = C_TDEF_EXECERRORS;
			break;
		case BT_EXPLAIN:
			sTdefNm = C_TDEF_EXPLAIN;
			break;
		//case BT_FORKCOMMAND:
		//	break;
		case BT_JOBMETRICS:
			sTdefNm = C_TDEF_JOBMETRICS;
			break;
		case BT_JOBNETDISSECT:
			sTdefNm = C_TDEF_JOBNETDISSECT;
			break;
		case BT_JOBNETWORK:
			sTdefNm = C_TDEF_JOBNETWORK;
			break;
		case BT_JOBOPERANDS:
			sTdefNm = C_TDEF_JOBOPERANDS;
			break;
		case BT_JOBOPERATORS:
			sTdefNm = C_TDEF_JOBOPERATORS;
			break;
		//case BT_LOADRESULT:
		//	sTdefNm = C_TDEF_AFFECTEDROWS;
		//	break;
		case BT_LOCKSTATS:
			sTdefNm = C_TDEF_LOCKSTATS;
			break;
		case BT_NODEINFO:
			sTdefNm = C_TDEF_NODEINFO;
			break;
		case BT_PAGEINREQUEST:
			sTdefNm = C_TDEF_PAGEINREQUEST;
			break;
		case BT_PATH:
			sTdefNm = C_TDEF_PATH;
			break;
		case BT_PROGRESSUNITS:
			sTdefNm = C_TDEF_PROGRESSUNITS;
			break;
		case BT_PROMPTOUT:
			sTdefNm = C_TDEF_PROMPTOUT;
			break;
		case BT_SEEKOFFSETS:
			sTdefNm = C_TDEF_SEEKOFFSETS;
			break;
		case BT_SYSLOCKS:
			sTdefNm = C_TDEF_SYSLOCKS;
			break;
		case BT_SYSMETRICS:
			sTdefNm = C_TDEF_SYSMETRICS;
			break;
		case BT_TOTALUNITS:
			sTdefNm = C_TDEF_TOTALUNITS;
			break;
		case BT_WARNINGS:
			sTdefNm = C_TDEF_WARNINGS;
			break;
		case BT_DATA:
		default:
			NEVER_REACH
	}

	m_pDataStack->vPushStr(sTdefNm);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].toCsv -- convert rowBuffer operand to csvBuffer                */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferToCsv(void)
{
  ENTRY();

  RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

  Tub* pTub = _pTub(m_pDataStack->pPopPtr(__func__));

  pRowBuffer->vToCsv(pTub);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].addLockStats -- add a row to RowBuffer with the lockStats      */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddLockStats(void)
{
	ENTRY();

	// get the table
	Table* pStreamTable = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the streamDef from the table
	StreamDef* pStreamDef = pStreamTable->pStreamDef();

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);
	pRowBuffer->vBufferType(BT_LOCKSTATS);

	Row* pRow = LockMetrics::pSample(pStreamDef, pRowBuffer);

	//pRow->vDump(pStreamDef);

	m_pDataStack->vPushInt(pRow ? true : false);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].addNodeInfo -- add a row to the RowBuffer with the nodeInfo    */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddNodeInfo(void)
{
	ENTRY();

	// get the table
	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the streamDef from the table
	StreamDef* pStreamDef = pTable->pStreamDef();

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);
	pRowBuffer->vBufferType(BT_NODEINFO);

	Grid* pGrid = AM::pGrid();

	Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
																	C_VARGS_BEGIN,
																	pGrid->pThisNode()->szNodeNm(),
																	AM::iRegionSize(),
																	AM::PTNMAP::nPtnsPerNode(),
																	(i8)AM::SYSTEM::bStarted(),
																	C_VARGS_END);
	//ASSERT(pRow);
	//pRow->vDump(pStreamDef);

	m_pDataStack->vPushInt(pRow ? true : false);
	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].addProgressUnits -- add a row to RowBuffer with the progressUnits  */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddProgressUnits(void)
{
	ENTRY();

	// get the table
	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	// get no of progress units
	i8 iProgressUnits = m_pDataStack->iPopInt(__func__);

	// get the streamDef
	StreamDef* pStreamDef = pTable->pStreamDef();

	// get the infoSet
	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);
	pRowBuffer->vBufferType(BT_PROGRESSUNITS);

	Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
																	C_VARGS_BEGIN,
																	iProgressUnits,
																	C_VARGS_END);
	//ASSERT(pRow);
	//pRow->vDump(pStreamDef);

	m_pDataStack->vPushInt(pRow ? true : false);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].addRowCount -- add a row to RowBuffer with the row counts for ptn  */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddRowCount(void)
{
	ENTRY();

	// get the RowBuffer table
	Table* pTableDef = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the table
	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the streamDef from the table
	StreamDef* pStreamDef = pTableDef->pStreamDef();

	LglPtn tLglPtn = m_pOp->tLglPtn();
	i8 i_TablePtn = pTable->iPtnNoToTablePtn(tLglPtn);

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);
	pRowBuffer->vBufferType(BT_COUNT);

	// RAY: UNCOMMENT next 2 lines to see relative sizes when running "count tableName;"
	//i8 iRelativeSize = pTable->iRelativeSize();
	//vTrace("Relative size of table %s is %lld\n"_Stz, pTable->szTableNm(), iRelativeSize);

	i8 nBuffers = 0;
	i8 nRows = 0;
	i8 jSumRowBytes = 0;

	pTable->vAnalyze(i_TablePtn, nBuffers, nRows, jSumRowBytes);

	Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
																	C_VARGS_BEGIN,
																	nRows,
																	C_VARGS_END);
	//ASSERT(pRow);
	//pRow->vDump(pStreamDef);

	m_pDataStack->vPushInt(pRow ? true : false);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].addRowVargs -- add a row by variable arguments                 */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddRowVargs(void)
{
	ENTRY();

	Table* pTable = _pTable(*m_pLogicUnit->m_xpTableDef);
	StreamDef* pStreamDef = pTable->pStreamDef();
	i8 nCols = pStreamDef->nColumns();
	i8 nArgs = m_pDataStack->iPopInt(__func__);

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	Row* aRow = pRowBuffer->RowBuffer::pAllotRow();
	Stz* szTail;
	i8   nSize;

	i8 iInt;
	s256 sStr;
	for (i8 i_Arg = 0; i_Arg < nArgs && i_Arg < nCols; i_Arg++) {

		Column* pColumn = pStreamDef->pGetColumn(i_Arg);

		eDM eDatum = pColumn->eDatum();

		switch (eDatum) {

			case DM_INT:
				iInt = m_pDataStack->iPopInt(__func__);
				aRow->vColVal_int(i_Arg,iInt);
				break;

			case DM_VCH:

				// STRING_DEPOSIT
				m_pDataStack->vPopStr(sStr,__func__);
				nSize = sStr.nSize();
				szTail = (Stz*)pRowBuffer->aAllotTail(nSize);
				StrLib::vCopy(szTail->azChar(), nSize, sStr.azChar());
				//aRow->vColVal_ofs(i_Arg,iOfs);
				aRow->vColVal_ofs(i_Arg,szTail);
				// END STRING_DEPOSIT

				break;

			default:
				RASSERT(0);
		}

	}

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].addTotalUnits -- add a row to RowBuffer with the totalUnits        */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddTotalUnits(void)
{
	ENTRY();

	// get the table
	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the total units
	i8 iTotalUnits = m_pDataStack->iPopInt(__func__);

	// get the file name
	s256 sFileName;
	m_pDataStack->vPopStr(sFileName, __func__);

	// get the streamDef from the table
	StreamDef* pStreamDef = pTable->pStreamDef();

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);
	pRowBuffer->vBufferType(BT_TOTALUNITS);

	Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
																	C_VARGS_BEGIN,
																	sFileName.azChar(),
																	iTotalUnits,
																	C_VARGS_END);
	//ASSERT(pRow);
	//pRow->vDump(pStreamDef);

	m_pDataStack->vPushInt(pRow ? true : false);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].addWarning -- add a row to RowBuffer with a warning            */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddWarning(void)
{
	ENTRY();

	s4K sReason;

	// get the table
	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	// Get the warning reason
	m_pDataStack->vPopStr(sReason, __func__);

	// get the streamDef from the table
	StreamDef* pStreamDef = pTable->pStreamDef();

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);
	pRowBuffer->vBufferType(BT_WARNINGS);

	Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
																	C_VARGS_BEGIN,
																	sReason.szStz(),
																	nullptr,
																	nullptr,
																	C_VARGS_END);

	m_pDataStack->vPushBol(pRow != nullptr);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].chunkFile -- RowBuffer chunk file                              */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferChunkFile(void)
{
	ENTRY();

	// get the seekOffsets
	RowBuffer* pSeekOffsets = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);
	pSeekOffsets->vBufferType(BT_SEEKOFFSETS);
	i8 nRowsInSeekOffsets = 0;
	Row* pSeekOffsetRow = nullptr;

	// get the file we are chunking
	File* pFile = _pFile(*m_pLogicUnit->m_xpFile0);

	// allocate a work area for reading in last block of chunk
	char* aLastBlock = (char*)MemLib::aMallocBlock(C_CHUNK_BLOCK);

	i8 nSumBytes = 0;
	i8 nChunkBytes = C_UNKNOWN;
	i8 i_OfsOfLastBlockInChunk = C_UNKNOWN;

	// get the file offset of the EOF
	i8 i_EofOfs = m_pLogicUnit->m_iCounter1;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// process chunks till eof or till we get C_MAX_SEEKOFFSETS

	for (;;) {

		// get the file offset of next chunk
		i8 i_ChunkOfs = m_pLogicUnit->m_iCounter0;

		RASSERT (i_ChunkOfs < i_EofOfs);

		// allot row for seek offset info
		pSeekOffsetRow = pSeekOffsets->RowBuffer::pAllotRow();

		// test for final chunk in file
		if (i_ChunkOfs + C_CHUNK_SIZE >= i_EofOfs) {
			// + final chunk in file

			// compute the total chunk bytes
			nChunkBytes = i_EofOfs - i_ChunkOfs;

			// end of file
			m_pLogicUnit->m_iCounter0 = i_EofOfs;
		}
		else {
			// -not final chunk in file

			// compute offset of last block in current chunk
			i_OfsOfLastBlockInChunk = i_ChunkOfs + C_CHUNK_SIZE - C_CHUNK_BLOCK;

			// read last block of current chunk
			pFile->vSeek(i_OfsOfLastBlockInChunk);
			i8 nBytesRead = pFile->nReadBytes(aLastBlock, C_CHUNK_BLOCK);

			// compute address of last data byte of last block of current chunk
			char *aLastDataByte = &aLastBlock[nBytesRead - 1];

			// find the last end of line in last block of current chunk
			char *aLastEol = aLastDataByte;
			bool bFoundEol = StrLib::bBackward(&aLastEol, aLastBlock, '\n');
			if (!bFoundEol)
				bFoundEol = StrLib::bBackward(&aLastEol, aLastBlock, '\r');
			RASSERT(bFoundEol);

			// compute the number of bytes which belong to the last block in chunk
			i8 nLastBlockBytes = nDifference(aLastEol, aLastBlock) + 1;

			// compute the total chunk bytes
			nChunkBytes = C_WSP_DATACHUNK - C_CHUNK_BLOCK + nLastBlockBytes;

			// compute end location of chunk
			i8 i_EndChunkLoc = i_ChunkOfs + nChunkBytes - 1;

			// advance to start of next chunk
			m_pLogicUnit->m_iCounter0 = i_EndChunkLoc + 1;
		}

		if (MY::pJob()->pJobTags()->bHasName("<OP_Chunk>"_Stz))
			vTrace("<OP_Chunk>: i_ChunkOfs %lld nChunkBytes %lld\n"_Stz,
						 i_ChunkOfs,nChunkBytes);

		pSeekOffsetRow->vColVal_int(0,i_ChunkOfs);
		pSeekOffsetRow->vColVal_int(1,nChunkBytes);
		
		nSumBytes += nChunkBytes;

		if (++nRowsInSeekOffsets >= C_MAX_SEEKOFFSETS)
			break;
		if (m_pLogicUnit->m_iCounter0 >= i_EofOfs)
			break;
	}


	// free the read block
	MemLib::vFreeBlock(aLastBlock, C_CHUNK_BLOCK);

	if (MY::pJob()->pJobTags()->bHasName("<OP_Chunk>"_Stz))
		vTrace("<OP_Chunk>: nSumBytes %lld\n"_Stz,nSumBytes);

	m_pDataStack->vPushInt(nSumBytes);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].colVal@ -- column value fetch                                    */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferColValFetch(void)
{
	ENTRY();

	Table* pTable = _pTable(*m_pLogicUnit->m_xpTableDef);
	StreamDef* pStreamDef = pTable->pStreamDef();
	i8 nCols = pStreamDef->nColumns();
	UNUSED(nCols);

	i8 i_Col = m_pDataStack->iPopInt(__func__);

	Column* pColumn = pStreamDef->pGetColumn(i_Col);
	eDM eDatum = pColumn->eDatum();

	Row* aRow = *m_pLogicUnit->m_xpRow;

	i8 iInt;
	s256 sStr;
//  Dtm wDtm;

	switch (eDatum) {

		case DM_INT:
			iInt = aRow->iColVal_int(i_Col);
			m_pDataStack->vPushInt(iInt);
			break;

		case DM_VCH:
//      wDtm = aRow->wColDtm(i_Col);
			sStr = aRow->szColVal_ofs(i_Col);
			m_pDataStack->vPushStz(sStr);
			break;

		default:
			RASSERT(0);
	}

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].firstRow -- get first row from the RowBuffer                       */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferFirstRow(void)
{
	ENTRY();

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	m_pDataStack->vPushPtr(pRowBuffer->pFirstRow());
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].isProgress --is rowBffer a progress RowBuffer ?                */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferIsProgress(void)
{
	ENTRY();

	ASSERT ( ! m_pLogicUnit->m_xpRowBuffer.bIsNull() );

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	bool bIsProgress = pRowBuffer->eBufferType() == BT_PROGRESSUNITS ||
										 pRowBuffer->eBufferType() == BT_TOTALUNITS;
	m_pDataStack->vPushInt(bIsProgress);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].isTotalUnits --is infor set a totalUnits RowBuffer ?              */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferIsTotalUnits(void)
{
	ENTRY();

	ASSERT ( ! m_pLogicUnit->m_xpRowBuffer.bIsNull() );

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	m_pDataStack->vPushInt(pRowBuffer->eBufferType() == BT_TOTALUNITS ? true : false);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].isWarning --is RowBuffer a warning RowBuffer ?                      */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferIsWarning(void)
{
	ENTRY();

	ASSERT ( ! m_pLogicUnit->m_xpRowBuffer.bIsNull() );

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	m_pDataStack->vPushInt(pRowBuffer->eBufferType() == BT_WARNINGS ? true : false);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].name -- get RowBuffer name                                         */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferName(void)
{
	ENTRY();

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	Stz* szBufferNm = pRowBuffer->szBufferNm();

	m_pDataStack->vPushStz(szBufferNm);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].nextRow -- get next row from the RowBuffer                     */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferNextRow(void)
{
	ENTRY();

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);
	Row* pRow = _pRow(*m_pLogicUnit->m_xpRow);

	ASSERT(pRow);

	m_pDataStack->vPushPtr(pRowBuffer->pNextRow(pRow));
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].rdCsvChunk -- reach a chunk from file given the RowBuffer      */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferRdCsvChunk(void)
{
	ENTRY();

	ASSERT(*m_pLogicUnit->m_xpRowBuffer);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// get file and info arguments

	File* pCsvFile = _pFile(*m_pLogicUnit->m_xpFile0);
	Stz* szFilePath = pCsvFile->sFilePath();

	Row* pInfoRow = _pRow(m_pDataStack->pPopPtr(__func__));
	i8 iOffset = pInfoRow->iColVal_int(0);
	i8 nBytes = pInfoRow->iColVal_int(1);

	//vTrace("reading chunk @%lld of %lld bytes\n"_Stz, iOffset, nBytes);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// create new csvBuffer and set csvMeta

#ifdef WITH_OOM_FAULTINJECTION
	PsxThd::vTlsSet(giTLS_CurrentPoint,(void*)6);
#endif

	CsvBuffer* pCsvBuffer = new CsvBuffer;

	CsvMeta* pCsvMeta = pCsvBuffer->pCsvMeta();

	pCsvMeta->vOriginNm(AM::NODE::szNodeNm());
	pCsvMeta->vFilePath(szFilePath);
	pCsvMeta->vOffset(iOffset);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// read chunk into buffer

	char* azInto = (char*)pCsvBuffer->ayDataArea();

	pCsvFile->vSeek(iOffset);
	pCsvBuffer->vTake(nBytes);
	i8 nBytesRead = pCsvFile->nReadBytes(azInto,nBytes);

//  if (nBytesRead != nBytes) {
//    nBytesRead = pCsvFile->nReadBytes(azInto,nBytes);
//    ASSERT(nBytesRead == nBytes);
//  }

	ASSERT(nBytesRead == nBytes);
	UNUSED(nBytesRead);

	azInto[nBytes] = '\0';

	//vTrace("trailing: %s\n"_Stz,azInto + nBytes - 64);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// return the CsvBuffer

	m_pDataStack->vPushInt(nBytes);
	m_pDataStack->vPushInt(iOffset);

	m_pDataStack->vPushPtr(pCsvBuffer);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].writeCsv -- write RowBuffer to file as csv                     */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferWriteCsv(void)
{
	ENTRY();

	ASSERT(*m_pLogicUnit->m_xpRowBuffer);

	Table* pTableDef = _pTable(m_pDataStack->pPopPtr(__func__));

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	StreamDef* pStreamDef = pTableDef->pStreamDef();

	File* pFile = _pFile(*m_pLogicUnit->m_xpFile0);

	MetricsHelper::vWriteCsv(pRowBuffer,pStreamDef,pFile);

	EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
