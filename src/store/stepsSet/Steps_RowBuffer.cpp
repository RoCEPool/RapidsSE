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

extern SizeHeap  gxaSizeHeap[];

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_RowBuffer(void)
{
  vAddStep("rowBuffer.operandDump"_Stz,      &Step::vRowBufferOperandDump);
	vAddStep("RowBuffer[].addAnalyzeIndex"_Stz,&Step::vRowBufferAddAnalyzeIndex);
	vAddStep("RowBuffer[].addAnalyzeTable"_Stz,&Step::vRowBufferAddAnalyzeTable);
	vAddStep("RowBuffer[].addBpaObjects"_Stz,  &Step::vRowBufferAddBpaObjects);
	vAddStep("RowBuffer[].addBpaRegions"_Stz,  &Step::vRowBufferAddBpaRegions);
	vAddStep("RowBuffer[].addColumnDef"_Stz,   &Step::vRowBufferAddColumnDef);
	vAddStep("RowBuffer[].addGridSanity"_Stz,  &Step::vRowBufferAddGridSanity);
	vAddStep("RowBuffer[].addListContents"_Stz,&Step::vRowBufferAddListContents);
	vAddStep("RowBuffer[].addListFolder"_Stz,  &Step::vRowBufferAddListFolder);
	vAddStep("RowBuffer[].addListIndex"_Stz,   &Step::vRowBufferAddListIndex);
	vAddStep("RowBuffer[].addListPartitions"_Stz,&Step::vRowBufferAddListPartitions);
	vAddStep("RowBuffer[].addLogMsg"_Stz,        &Step::vRowBufferAddLogMsg);
	vAddStep("RowBuffer[].addMemoryLeaks"_Stz,   &Step::vRowBufferAddMemoryLeaks);
	vAddStep("RowBuffer[].addNodeHeartbeat"_Stz, &Step::vRowBufferAddNodeHeartbeat);
	vAddStep("RowBuffer[].addNodeStatus"_Stz,    &Step::vRowBufferAddNodeStatus);
	vAddStep("RowBuffer[].addPingInfo"_Stz,      &Step::vRowBufferAddPingInfo);
	vAddStep("RowBuffer[].addPwfInfo"_Stz,       &Step::vRowBufferAddPwfInfo);
	vAddStep("RowBuffer[].addRowCount"_Stz,      &Step::vRowBufferAddRowCount);
	vAddStep("RowBuffer[].addShowCluster"_Stz,   &Step::vRowBufferAddShowCluster);
	vAddStep("RowBuffer[].addSizeIndex"_Stz,     &Step::vRowBufferAddSizeIndex);
	vAddStep("RowBuffer[].addSizeTable"_Stz,     &Step::vRowBufferAddSizeTable);
	vAddStep("RowBuffer[].addSpaceInfo"_Stz,     &Step::vRowBufferAddSpaceInfo);
	vAddStep("RowBuffer[].addTableSizes"_Stz,    &Step::vRowBufferAddTableSizes);
	vAddStep("RowBuffer[].addVerifyIndex"_Stz,   &Step::vRowBufferAddVerifyIndex);
	vAddStep("RowBuffer[].addVerifyTable"_Stz,   &Step::vRowBufferAddVerifyTable);
  vAddStep("RowBuffer[]"_Stz,                &Step::vRowBufferGet);
  vAddStep("RowBuffer[]\\clear"_Stz,         &Step::vRowBufferGetAndClear);
  vAddStep("RowBuffer[].bufferId"_Stz,       &Step::vRowBufferGetBufferIdAsAddr);
  vAddStep("RowBuffer[].copy"_Stz,           &Step::vRowBufferCopy);
  vAddStep("RowBuffer[].decile"_Stz,         &Step::vRowBufferDecile);
  vAddStep("RowBuffer[].delete"_Stz,         &Step::vRowBufferDelete);
  vAddStep("RowBuffer[].get"_Stz,            &Step::vRowBufferGet);
  vAddStep("RowBuffer[].insdelIntoTable"_Stz,&Step::vRowBufferInsdelIntoTable);
  vAddStep("RowBuffer[].isNotNull"_Stz,      &Step::vRowBufferIsNotNull);
  vAddStep("RowBuffer[].isNull"_Stz,         &Step::vRowBufferIsNull);
  vAddStep("RowBuffer[].iterate"_Stz,        &Step::vRowBufferIterate);
	vAddStep("RowBuffer[].new"_Stz,            &Step::vRowBufferNew);
  vAddStep("RowBuffer[].print"_Stz,          &Step::vRowBufferPrint);
  vAddStep("RowBuffer[].ptnId"_Stz,          &Step::vRowBufferPtnId);
  vAddStep("RowBuffer[].rows"_Stz,           &Step::vRowBufferRows);
  vAddStep("RowBuffer[].set"_Stz,            &Step::vRowBufferSet);
  vAddStep("RowBuffer[].setDeciderCopies"_Stz, &Step::vRowBufferSetDeciderCopies);
  vAddStep("RowBuffer[].setLglPtn"_Stz,      &Step::vRowBufferSetLglPtn);
  vAddStep("RowBuffer[].toCsv"_Stz,          &Step::vRowBufferToCsv);
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
/* rowBuffer[].addAnalyzeIndex -- add a row to the streamSet index analysis   */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddAnalyzeIndex(void)
{
	ENTRY();

	// get the table
	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the index to be analyzed
	Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));

	// get the tablePtn
	i8 i_TablePtn = m_pDataStack->iPopInt(__func__);

	// get the streamDef from the table
	StreamDef* pStreamDef = pTable->pStreamDef();

	// get the stream set
	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	i8 nBuffers = 0;
	i8 nEntries = 0;

	pIndex->vAnalyze(i_TablePtn, nBuffers, nEntries);

	Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
																		C_VARGS_BEGIN,
																		AM::pGrid()->pThisNode()->szNodeNm(),
																		pTable->tTablePtnToLglPtn(i_TablePtn).iAsI8(),
																		nBuffers,
																		nEntries,
																		C_VARGS_END);

	//ASSERT(pRow);
	//pRow->vDump(pStreamDef);

	m_pDataStack->vPushInt(pRow ? true : false);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].addAnalyzeTable -- add a row to the streamSet table analysis   */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddAnalyzeTable(void)
{
	ENTRY();

	// get the table to be analyzed
	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the tablePtn
	i8 i_TablePtn = m_pDataStack->iPopInt(__func__);

	// get the streamDef from the table
	StreamDef* pStreamDef = pTable->pStreamDef();

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

	// Get the warning infoset and definition
	InfoSet* pWarnInfoSet = * m_pLogicUnit->m_xpInfoSet;
	ASSERT(pWarnInfoSet);
	ASSERT(pWarnInfoSet->eOpInfoSet() == OI_WARNINGS);

	Table* pWarnTable = _pTable(* m_pLogicUnit->m_xpInfoSetTable);
	ASSERT(pWarnTable);
	StreamDef* pWarnDef = pWarnTable->pStreamDef();
	ASSERT(pWarnDef);


	// get the streamset table
	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the streamDef from the table
	StreamDef* pStreamDef = pTable->pStreamDef();

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	PoolHelper::vObjectSummary(pRowBuffer, pStreamDef, pWarnInfoSet, pWarnDef);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].addBpaRegions -- add rows about bpa region analysis            */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddBpaRegions(void)
{
	ENTRY();

	// Get the warning infoset and definition
	InfoSet* pWarnInfoSet = * m_pLogicUnit->m_xpInfoSet;
	ASSERT(pWarnInfoSet);
	ASSERT(pWarnInfoSet->eOpInfoSet() == OI_WARNINGS);

	Table* pWarnTable = _pTable(* m_pLogicUnit->m_xpInfoSetTable);
	ASSERT(pWarnTable);
	StreamDef* pWarnDef = pWarnTable->pStreamDef();
	ASSERT(pWarnDef);


	// get the table
	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the streamDef from the table
	StreamDef* pStreamDef = pTable->pStreamDef();

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	PoolHelper::vRegionSummary(pRowBuffer, pStreamDef, pWarnInfoSet, pWarnDef);

	EXIT();
}

/*----------------------------------------------------------------------------*/
/* RowBuffer[].addColumnDef -- add a row to the streamSet with the column def */
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
/* RowBuffer[].addGridSanity -- add a row to the infoset with the gridSanity  */
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
/* RowBuffer[].addListContents -- add a row to infoset with the list contents */
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
/* RowBuffer[].addListFolder -- add a row to infoset with the list folder     */
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
/* RowBuffer[].addListIndex -- add a row to infoset with the list index       */
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
/* RowBuffer[].addListPartitions -- add a row to infoset with the partitions  */
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
/* RowBuffer[].addLogMsg -- add rows to streamSet with log message info       */
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
/* RowBuffer[].addMemoryLeaks -- add rows to streamSet with memory leak info  */
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
/* RowBuffer[].addNodeInfo -- add a row to the streamSet with this node info  */
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
/* RowBuffer[].addPingInfo -- add a row to infoset with the ping response     */
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
/* RowBuffer[].addProgressUnits -- add a row to rowBuffer with progressUnits  */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddProgressUnits(void)
{
	ENTRY();

	i8 iProgressUnits = m_pDataStack->iPopInt(__func__);

	StreamDef* pStreamDef = _pStreamDef(m_pDataStack->pPopPtr(__func__));
	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

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
/* RowBuffer[].addShowCluster -- add a row to rowBuffer with the cluster info */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferAddRowCount(void)
{
	ENTRY();

	// get the table
	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

	// get the streamDef from the table
	StreamDef* pStreamDef = pTable->pStreamDef();

	i8 i_Count = m_pDataStack->iPopInt(__func__);

	RowBuffer* pRowBuffer = _pRowBuffer(*m_pLogicUnit->m_xpRowBuffer);

	Row* pRow = pRowBuffer->pAllotRow(pStreamDef,
																		C_VARGS_BEGIN,
																		i_Count,
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
	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
	StreamDef* pStreamDef = pTable->pStreamDef();

	// get the index
	Index* pIndex = _pIndex(m_pDataStack->pPopPtr(__func__));

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
	Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));
	StreamDef* pStreamDef = pTable->pStreamDef();

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
/* RowBuffer[].bufferId -- fetch buffer id as address                         */
/*----------------------------------------------------------------------------*/
void Step::vRowBufferGetBufferIdAsAddr(void)
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

	StreamDef* pStreamDef = pTable->pStreamDef();

	m_pLogicUnit->m_xpRowBuffer = new RowBuffer(pStreamDef);

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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
